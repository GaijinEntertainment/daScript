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
    void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list, bool needNames = false );

    LineInfo tokAt ( const struct YYLTYPE & li );
    LineInfo tokRangeAt ( const struct YYLTYPE & li, const struct YYLTYPE & lie );

    Annotation * findAnnotation ( const string & name, const LineInfo & at );

    extern bool das_need_oxford_comma;
    extern bool das_force_oxford_comma;
    extern bool das_supress_errors;
    extern int das_arrow_depth;

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

#line 382 "ds_parser.cpp"

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
#define YYLAST   7064

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  174
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  152
/* YYNRULES -- Number of rules.  */
#define YYNRULES  477
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  846

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
       0,   397,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   413,   425,   426,   430,   434,
     439,   448,   456,   472,   494,   498,   501,   507,   516,   537,
     560,   561,   565,   569,   570,   574,   577,   583,   589,   592,
     598,   599,   603,   604,   605,   614,   615,   619,   628,   643,
     653,   662,   663,   664,   665,   666,   667,   671,   676,   684,
     685,   686,   687,   688,   689,   690,   691,   697,   710,   726,
     727,   731,   738,   750,   754,   761,   762,   766,   767,   768,
     772,   777,   784,   788,   789,   790,   791,   792,   793,   794,
     795,   796,   797,   798,   799,   800,   801,   802,   803,   804,
     805,   806,   807,   808,   809,   810,   811,   812,   813,   814,
     815,   816,   817,   818,   819,   820,   821,   822,   823,   827,
     908,   911,   921,   922,   923,   924,   925,   926,   927,   928,
     929,   930,   931,   932,   933,   934,   935,   936,   937,   938,
     942,   947,   953,   959,   970,   976,   977,   988,   992,   998,
    1001,  1004,  1008,  1014,  1018,  1022,  1025,  1028,  1033,  1036,
    1044,  1047,  1052,  1055,  1063,  1069,  1070,  1074,  1075,  1079,
    1113,  1113,  1113,  1116,  1116,  1116,  1121,  1121,  1121,  1129,
    1133,  1138,  1144,  1148,  1153,  1162,  1165,  1171,  1172,  1179,
    1190,  1191,  1195,  1200,  1255,  1256,  1257,  1258,  1259,  1260,
    1264,  1265,  1266,  1267,  1268,  1269,  1270,  1271,  1272,  1273,
    1274,  1275,  1276,  1277,  1278,  1279,  1283,  1284,  1288,  1299,
    1304,  1314,  1318,  1318,  1318,  1325,  1325,  1325,  1339,  1340,
    1341,  1342,  1343,  1344,  1345,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,  1353,  1354,  1355,  1356,  1357,  1358,  1359,  1360,
    1361,  1362,  1363,  1364,  1365,  1366,  1367,  1368,  1369,  1370,
    1371,  1372,  1373,  1374,  1375,  1376,  1377,  1378,  1382,  1386,
    1389,  1392,  1393,  1394,  1395,  1400,  1405,  1406,  1409,  1412,
    1416,  1420,  1424,  1425,  1426,  1427,  1428,  1429,  1430,  1440,
    1450,  1505,  1506,  1510,  1511,  1515,  1523,  1526,  1533,  1538,
    1546,  1547,  1551,  1554,  1563,  1564,  1568,  1577,  1578,  1583,
    1584,  1588,  1594,  1600,  1603,  1607,  1613,  1622,  1623,  1624,
    1628,  1629,  1633,  1636,  1641,  1646,  1654,  1665,  1668,  1675,
    1676,  1680,  1707,  1707,  1719,  1722,  1731,  1744,  1756,  1757,
    1761,  1765,  1776,  1791,  1792,  1796,  1835,  1901,  1908,  1917,
    1918,  1919,  1920,  1921,  1922,  1923,  1924,  1925,  1926,  1927,
    1928,  1929,  1930,  1931,  1932,  1933,  1934,  1935,  1936,  1937,
    1938,  1939,  1940,  1941,  1945,  1946,  1947,  1948,  1949,  1950,
    1954,  1965,  1969,  1979,  1986,  1995,  1995,  1995,  2008,  2009,
    2010,  2011,  2012,  2026,  2032,  2036,  2041,  2046,  2051,  2058,
    2063,  2067,  2071,  2076,  2080,  2085,  2085,  2085,  2091,  2098,
    2098,  2098,  2103,  2103,  2103,  2109,  2109,  2109,  2114,  2118,
    2118,  2118,  2123,  2123,  2123,  2132,  2136,  2136,  2136,  2141,
    2141,  2141,  2150,  2154,  2154,  2154,  2159,  2159,  2159,  2168,
    2168,  2168,  2174,  2174,  2174,  2183,  2183,  2198,  2198,  2218,
    2219,  2220,  2221,  2225,  2232,  2241,  2246,  2253,  2258,  2264,
    2270,  2277,  2288,  2291,  2305,  2314,  2315,  2319,  2324,  2331,
    2336,  2347,  2352,  2359,  2371,  2372,  2376,  2393
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
  "expr_cast", "$@1", "$@2", "$@3", "$@4", "$@5", "$@6", "expr_type_info",
  "expr_list", "block_or_simple_block", "block_or_lambda", "expr_block",
  "expr_numeric_const", "expr_assign", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_expr", "$@7", "$@8",
  "$@9", "$@10", "expr", "optional_field_annotation", "optional_override",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_declaration", "global_variable_declaration_list",
  "optional_shared", "global_let", "$@11", "enum_list", "single_alias",
  "alias_list", "alias_declaration", "enum_declaration",
  "optional_structure_parent", "structure_name", "structure_declaration",
  "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_type_declaration",
  "$@12", "$@13", "type_declaration", "$@14", "$@15", "$@16", "$@17",
  "$@18", "$@19", "$@20", "$@21", "$@22", "$@23", "$@24", "$@25", "$@26",
  "$@27", "$@28", "$@29", "$@30", "$@31", "$@32", "$@33", "$@34", "$@35",
  "$@36", "$@37", "variant_alias_declaration", "$@38",
  "bitfield_alias_declaration", "$@39", "make_decl", "make_struct_fields",
  "make_struct_dim", "make_struct_decl", "make_tuple", "make_map_tuple",
  "make_any_tuple", "make_dim", "make_dim_decl", "make_table",
  "make_table_decl", "array_comprehension_where", "array_comprehension", YY_NULLPTR
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

#define YYPACT_NINF -515

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-515)))

#define YYTABLE_NINF -437

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -515,    19,  -515,  -515,   -24,    -9,   -23,   -14,  -125,  -515,
      36,  -515,  -515,   -17,  -515,  -515,  -515,  -515,  -515,   338,
    -515,    79,  -515,  -515,  -515,  -515,  -515,  -515,    33,  -515,
      47,    61,    65,  -515,  -515,     3,  -515,   -29,    88,  -515,
     128,   144,  -515,   114,   174,   147,  -515,    41,  -515,   180,
      -6,  -515,    86,    12,   -24,   195,    -9,   199,  -515,   203,
     206,  -515,   291,  -515,   211,  -515,   -95,   181,   192,   231,
    -515,   -24,   -17,  -515,   226,   200,  6908,   345,   346,  -515,
     220,  -515,   395,  -515,  -515,  -515,  -515,  -515,   262,    92,
    -515,  -515,  -515,  -515,   357,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,   242,   -90,  5727,  -515,  -515,   284,   290,
    -515,   -68,  -515,   -72,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,   139,   292,   -83,   269,  -515,   -69,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,    54,  -515,  -515,   -32,  -515,
     297,   303,   314,   315,  -515,  -515,  -515,   298,  -515,  -515,
    -515,  -515,  -515,   322,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,   323,  -515,  -515,  -515,   327,   330,  -515,
    -515,  -515,  -515,   331,   332,  -515,  -515,  -515,  -515,  -515,
      68,  -515,   309,   339,  -515,   316,  -515,  -515,  -115,   -24,
    -515,   -16,  -515,    18,  5727,   317,  -515,  -515,  -515,   198,
    -515,  -515,  -515,    92,  -515,   -79,  4302,  -515,  -515,  -515,
    -515,  -515,   347,  -515,   191,   273,   293,  -515,  -515,  -515,
       2,  -515,  -515,  -515,  -515,  -515,  2304,  -515,  -515,   -71,
    5727,   -70,  -515,   447,   320,   115,   358,  -515,   146,  -515,
     329,  1409,  -515,  -515,   362,  5727,  -515,   185,  -515,  -515,
    -515,  -515,  5650,   334,  -515,   335,   351,   355,   341,   361,
    -515,   363,  4302,  4302,   834,   983,  4302,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,  4302,  4302,  4302,  4302,  4302,  4302,
    -515,   340,  -515,  -515,   -77,  -515,  -515,  -515,   348,  -515,
    -515,  -515,  -515,  -515,   525,   350,  -515,  -515,  -515,  -515,
    -515,  5727,  5727,  5727,  5727,   344,   284,  5727,   220,  5727,
     220,  5727,   220,  5804,   290,   367,  -515,   349,  -515,  -515,
    5798,  -515,  -515,  1409,  -515,  -515,  -515,   358,  -515,  -515,
    -515,  -515,  -515,  5727,  -515,  2433,  -515,  1153,  -515,   420,
    -515,  -515,  -515,  2433,   709,  -515,   384,  4302,  -515,  -515,
    4302,  -515,  5727,   253,   253,   358,  1473,   358,  1618,  6190,
    -515,   189,   310,   253,   253,   -76,   253,   253,  5132,   -55,
    -515,  1901,   220,    24,   385,  4302,  4302,  -515,  -515,  4302,
    4302,  4302,  4302,   386,  4302,   387,  4302,  4302,  4302,  4302,
    4302,  2571,  4302,  4302,  4302,  4302,  4302,  4302,  4302,  4302,
    4302,  4302,   388,  4302,  -515,  2700,    29,   964,   965,   966,
    -515,    48,  4428,   292,  6042,   292,  6629,   292,    -1,  -515,
     119,  1409,   123,  -515,  -515,  -515,  -515,   568,   240,  -515,
     407,  6021,  -515,  4302,  -515,  -515,   358,  1763,  -515,   317,
    4302,  4302,  -515,   354,   396,  2838,  2046,  -515,   490,   359,
     364,  4302,  -515,  -515,  -515,  -515,  -515,  -515,   365,   366,
     370,  -515,  -515,  -515,   508,  -515,   -85,  -515,  6125,  -515,
    2433,  -515,  4515,  2967,  2175,  5200,  5727,  5727,  5268,  5727,
    6630,    10,   -66,  4431,   368,  3105,  6190,   394,    44,   398,
    -515,  -515,    94,    14,  3243,   234,   278,  4302,  4302,   369,
    -515,  4302,   392,  -515,   294,  -515,   409,  -515,    76,  6393,
     292,  5727,  -515,  -515,    49,    49,   197,   197,  6648,  6648,
     378,    20,  -515,  5875,   -33,   -33,    49,    49,   413,  6259,
    6458,   824,  6527,   197,   197,   353,   353,    20,    20,    20,
    -515,  5944,  -515,   101,  -515,  5727,  -515,  -515,  -515,   414,
    -515,   401,  -515,   402,  -515,   403,  5727,  -515,  5804,  -515,
     290,  4302,  -515,  -515,   240,  4302,  4302,  4302,  4302,  4302,
    4302,  4302,  4302,  4302,  4302,  4302,  4302,  4302,  4302,  4302,
     281,    16,  2433,  -515,  -515,  4611,   539,  6393,   281,  -515,
     410,   426,  6393,  2433,  -515,  4707,   383,  -515,  -515,   281,
    -515,  -515,  -515,   518,   358,  -515,  3381,  3510,  -515,  4803,
    -515,  -515,   124,  5727,   425,  5336,  -515,  6671,  6679,  -515,
    6752,   399,  4302,  4302,  -515,   409,   430,   404,   409,  4302,
     405,  4302,  4302,   409,   406,   408,  6393,  -515,  -515,  6324,
    5727,   220,   -65,   116,  4302,  -515,   -81,  1409,  3639,  -515,
    -515,  4302,  -515,  -515,  -515,  6753,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,  1409,  -515,  -515,  -515,  6393,
    -515,  6393,  6393,  6393,  6393,  6393,  6393,  6393,  6393,  6393,
    6393,  6393,  6393,  6393,  6393,  6393,  -515,  4302,  -515,  4899,
    -515,   317,  -515,  -515,  -515,  -515,  4995,  -515,  -515,   313,
    -515,  -515,  4302,  4302,  -515,  -515,  6792,   157,  -515,  -515,
    -515,  -515,  3777,   -87,  6393,   104,   -65,  -515,   394,  6393,
    -515,  -515,   -58,   302,  -515,  -515,  -515,  6794,   292,   393,
    6393,  3915,  -515,  -515,  -515,   126,  6393,  -515,  -515,  -515,
    -515,   -45,  -515,  -515,  -515,  1325,   317,  -515,  -515,  4302,
    -515,  6592,  6592,  -515,  4044,   434,  4302,  4302,  4302,  -515,
    5404,  4302,   415,  4302,  4302,   416,  -515,   424,  -515,  4302,
    6393,  -515,  -515,   317,  -515,  -515,   281,  5727,  5472,   427,
     197,   197,   197,  -515,  5060,  -515,  6393,  5060,  -515,   -72,
    -515,  6393,  -515,   313,  6819,  -515,  4173,   529,   417,   419,
    -515,   -72,  -515,  -515,  5727,  5540,  4302,   421,   422,  -515,
    6825,  -515,  6393,  -515,  -515,  -515
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    75,     1,   167,     0,     0,     0,     0,     0,   168,
       0,   447,   445,     0,    14,     3,    10,     9,     8,     0,
       7,   329,     6,    11,     5,     4,    12,    13,    65,    67,
      23,    35,    32,    33,    25,    30,    24,     0,     0,    15,
       0,     0,    70,   145,     0,    71,    73,     0,    69,     0,
       0,   330,   332,     0,     0,     0,     0,     0,    31,     0,
       0,    28,     0,   334,     0,   338,     0,     0,     0,     0,
     147,     0,     0,    76,   343,     0,     0,     0,     0,    82,
      77,   327,     0,    63,    64,    61,    62,    60,     0,     0,
      59,    68,    36,    34,    30,    27,    26,   374,   377,   375,
     378,   376,   379,     0,     0,     0,   340,   339,     0,     0,
     146,     0,    74,     0,   345,   296,    85,    86,    88,    87,
      89,    90,    91,    92,   111,   112,   109,   110,   102,   113,
     114,   103,   100,   101,   115,   116,   117,   118,   105,   106,
     104,    98,    99,    94,    93,    95,    96,    97,    84,    83,
     107,   108,   291,    80,     0,     0,    16,     0,    55,    56,
      53,    54,    52,    51,    57,     0,    29,   334,     0,   341,
       0,     0,     0,     0,   349,   369,   350,   381,   351,   355,
     356,   357,   358,   373,   362,   363,   364,   365,   366,   367,
     368,   370,   371,   418,   354,   361,   372,   425,   432,   352,
     359,   353,   360,     0,     0,   380,   388,   391,   389,   390,
       0,   383,     0,     0,   307,     0,    72,   344,   291,     0,
      78,     0,   300,     0,     0,     0,   347,   331,   328,   320,
     333,    17,    18,     0,    66,     0,     0,   335,   409,   412,
     415,   405,     0,   385,   419,   426,   433,   439,   442,   395,
       0,   401,   408,   404,   397,   403,     0,   337,   400,     0,
       0,     0,   346,   293,     0,     0,     0,   298,   311,    79,
     291,    81,   140,   119,     0,     0,   321,     0,    58,   342,
     233,   234,     0,     0,   228,     0,     0,     0,     0,     0,
     373,     0,     0,     0,     0,     0,     0,   194,   196,   195,
     197,   198,   199,    19,     0,     0,     0,     0,     0,     0,
     190,   191,   231,   192,   229,   284,   283,   282,    75,   287,
     230,   286,   285,   266,     0,     0,   232,   449,   450,   451,
     452,     0,     0,     0,     0,     0,     0,     0,    77,     0,
      77,     0,    77,     0,     0,   396,   399,     0,   402,   393,
       0,   384,   448,   306,   446,   308,   294,     0,   297,   292,
     299,   310,   309,     0,   312,     0,   301,     0,   348,     0,
     318,   319,   317,     0,   149,   152,     0,     0,   170,   173,
       0,   176,     0,   257,   258,     0,     0,     0,     0,     0,
     471,     0,     0,   238,   237,   271,   236,   235,     0,     0,
     290,     0,    77,     0,     0,     0,     0,   259,   260,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   336,     0,     0,     0,     0,     0,
     382,     0,     0,    80,     0,    80,     0,    80,   145,   304,
       0,   302,     0,   398,   394,   392,   295,   313,     0,   316,
       0,   315,   142,     0,    45,    46,     0,     0,   153,     0,
       0,     0,   154,     0,     0,     0,     0,   122,   120,     0,
       0,     0,   135,   130,   128,   129,   141,   123,     0,     0,
       0,   133,   134,   136,   165,   127,     0,   124,   200,   322,
       0,   326,   200,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   145,   403,     0,     0,   462,   455,     0,   465,
     466,   467,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,    20,   261,   222,   221,     0,   267,     0,   185,
      80,     0,   279,   280,   239,   240,   252,   253,   250,   251,
       0,   276,   265,     0,   288,   289,   241,   242,     0,     0,
     255,   256,   254,   248,   249,   244,   243,   245,   246,   247,
     264,     0,   269,     0,   410,     0,   416,   406,   386,     0,
     420,     0,   427,     0,   434,     0,     0,   440,     0,   443,
       0,     0,   191,   144,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   155,   158,   200,     0,   148,     0,   139,
       0,     0,    39,     0,   162,   200,     0,   137,   138,     0,
     126,   131,   132,     0,     0,   125,     0,     0,   324,   200,
     325,   150,     0,     0,     0,     0,   272,     0,     0,   273,
       0,     0,     0,     0,   458,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   464,   472,   473,     0,
       0,    77,     0,     0,     0,   268,     0,   278,     0,   263,
     281,     0,   262,   270,   411,     0,   417,   407,   387,   421,
     423,   428,   430,   435,   437,   303,   441,   305,   444,   314,
     143,   207,   208,   210,   209,   211,   204,   205,   206,   212,
     213,   202,   203,   214,   215,   201,    49,     0,   159,   200,
     156,     0,    50,    37,    38,   163,   200,   160,   140,    42,
     166,   169,     0,     0,   323,   151,     0,     0,   179,   171,
     174,   177,     0,     0,   453,     0,     0,   457,   456,   463,
     469,   468,     0,     0,   460,   470,    21,     0,    80,     0,
     186,     0,   187,   193,   219,     0,   277,   413,   424,   431,
     438,     0,   157,   164,   161,     0,     0,    40,    41,     0,
      47,   217,   216,   182,     0,     0,     0,     0,     0,   274,
       0,     0,     0,     0,     0,     0,   223,     0,   218,     0,
     188,   220,   414,     0,   121,    43,     0,     0,     0,     0,
     172,   175,   178,   275,   474,   459,   454,   474,   461,     0,
     226,   189,    48,    42,     0,   180,     0,     0,     0,     0,
     224,     0,    44,   183,     0,     0,     0,     0,     0,   227,
       0,   181,   475,   476,   477,   184
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -515,  -515,  -515,   186,   535,  -515,  -515,  -515,  -515,  -515,
    -515,   495,  -515,  -515,   538,  -515,  -515,  -515,  -228,  -515,
    -515,  -515,  -515,  -515,   372,  -515,   542,   -60,  -515,   526,
    -515,   279,  -330,  -429,  -515,  -515,  -219,  -515,  -128,  -352,
     -13,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,   600,
    -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -430,
    -515,  -515,  -427,  -515,  -363,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,   356,   389,  -515,  -515,  -515,   333,  -515,
      21,  -515,  -259,   258,  -261,  -231,   237,  -515,   -26,  -515,
    -515,  -515,  -515,   443,   545,  -515,  -515,  -515,  -515,  -515,
    -515,  -151,  -104,  -515,  -515,  -515,   277,  -515,  -515,  -515,
     -88,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,   336,
    -514,  -379,  -515,  -515,  -276,   -47,   227,  -515,  -515,  -515,
    -201,  -515
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   157,   163,   392,   312,    16,    17,    35,
      36,    61,    18,    32,    33,   479,   480,   779,   780,   481,
     482,   483,   484,   485,   164,   165,    29,    30,    45,    46,
      47,    19,   153,   225,    80,    20,   313,   486,   367,   487,
     314,   488,   315,   489,   490,   491,   492,   493,   634,   494,
     495,   316,   506,   786,   507,   787,   509,   788,   317,   538,
     763,   318,   319,   320,   460,   497,   321,   322,   323,   670,
     819,   671,   831,   539,   221,   357,   264,   218,   222,   223,
     449,   450,   214,   215,   653,   267,   373,   277,   228,   154,
      52,    22,    82,   104,    65,    66,    23,    24,   114,    75,
      25,   268,   325,   103,   207,   208,   212,   209,   336,   688,
     451,   334,   687,   331,   684,   332,   802,   333,   686,   337,
     689,   338,   768,   339,   691,   340,   769,   341,   693,   342,
     770,   343,   696,   344,   698,    26,    41,    27,    40,   326,
     517,   518,   327,   519,   520,   521,   522,   328,   391,   329,
     828,   330
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   206,   355,   229,   496,   573,   273,   365,   443,   525,
     445,   111,   447,   459,   581,    42,   583,   210,   585,     2,
     390,   501,   673,    57,     3,    83,    84,    76,   652,   266,
     594,   593,   662,   345,   717,   360,   415,   416,    64,   403,
     404,   541,   761,   168,   361,   361,    38,     4,   219,     5,
     226,     6,   674,     7,   168,    58,   400,   262,     8,    48,
     249,    43,   351,   213,     9,   250,   231,   232,   403,   404,
      10,    54,   540,   642,   362,   362,   413,   106,    43,   415,
     416,   674,   169,   791,   251,   635,    11,   433,   401,   227,
     272,   534,   205,   279,   674,    77,    78,    44,   216,   249,
     217,   352,   354,    69,   250,   158,   159,    12,   236,    28,
      34,   676,   794,    62,    44,   614,    43,   226,    31,    37,
     206,   407,   408,   251,   624,   803,   456,    79,   432,   413,
     433,   414,   415,   416,   417,    51,   271,   418,   237,    85,
     252,   586,    63,    86,   748,    87,    88,   346,   638,   274,
     407,   408,    44,   274,    59,   274,   206,   542,   413,   265,
     414,   415,   416,   417,    60,   347,   418,   700,    69,    39,
     253,   206,   353,    53,   254,   348,   574,    89,   206,   252,
      72,   432,    13,   433,   269,   255,    54,   369,   270,    14,
     206,   206,   256,   233,   374,   578,   591,   427,   428,   429,
     430,   431,   258,    55,    56,    73,   386,   388,   657,   253,
     432,   205,   433,   254,   658,   674,   403,   404,   579,   160,
     234,    64,   743,   161,   255,   162,    88,   206,   206,   206,
     206,   256,   752,   206,   511,   206,   523,   206,   257,   206,
     674,   258,   675,   436,   437,   438,   439,   205,   765,   442,
     616,   444,   667,   446,    54,   656,   361,    81,   660,   206,
     718,    67,   205,   674,   661,   674,   587,   683,   792,   205,
     589,   725,   403,   404,   658,   457,   745,    68,   206,   359,
     759,   205,   205,    69,   753,   274,   362,   771,   363,   588,
     735,   364,   801,   590,   510,   370,   405,   406,   407,   408,
     403,   404,   219,   371,   784,   220,   413,    70,   414,   415,
     416,   417,    71,    74,   418,   611,   419,   420,   205,   205,
     205,   205,    92,   776,   205,   372,   205,   785,   205,   797,
     205,   355,    94,  -422,   777,   778,    95,   274,  -422,    96,
     275,   758,    49,   276,    50,   427,   428,   429,   430,   431,
     205,   105,   108,    97,   407,   408,  -422,    98,   432,   528,
     433,   529,   413,   109,   110,   415,   416,   417,   113,   205,
     418,   115,   403,   404,    99,   100,   101,   102,   150,   151,
     405,   406,   407,   408,   409,   152,   535,   410,   411,   412,
     413,   716,   414,   415,   416,   417,   155,   156,   418,   722,
     419,   420,   206,   206,   658,   206,   664,   310,   592,    58,
     729,   272,   496,   167,   432,  -429,   433,   211,   647,   648,
    -429,   650,   421,   213,   422,   423,   424,   425,   426,   427,
     428,   429,   430,   431,   224,  -436,  -225,   206,  -429,   230,
    -436,  -225,   432,   238,   433,   156,   530,   531,   661,   239,
     665,   249,   272,   677,   407,   408,   250,   762,  -436,  -225,
     240,   241,   413,   242,   414,   415,   416,   417,   243,   244,
     418,   206,   658,   245,   795,   251,   246,   247,   248,   259,
     335,   260,   206,   229,   206,   793,   261,   685,   272,   356,
     358,   226,   219,   205,   205,   368,   205,   378,   695,   376,
     377,   379,   773,   429,   430,   431,   380,   381,   399,   382,
     440,    13,   453,   454,   432,   435,   433,   504,   543,   550,
     552,   570,   594,   620,   619,   626,   633,   231,   205,   627,
     370,   252,   654,   656,   628,   630,   631,   659,   371,   206,
     632,   668,   672,   678,   403,   404,   680,   351,   690,   692,
     694,   721,   723,   724,   728,   736,   730,   805,   737,   798,
     372,   253,   205,   746,   742,   254,   206,   809,   747,   750,
     754,   820,   755,   205,   826,   205,   255,   836,   532,   815,
     818,   837,   757,   256,   822,   843,   844,   823,    90,   166,
     499,   838,   324,   258,    93,   832,    91,   402,   112,   249,
     775,    21,   452,   366,   250,   278,   500,   263,   731,   697,
     235,   107,   350,   441,   751,   526,   829,     0,   375,     0,
       0,     0,     0,   251,   405,   406,   407,   408,   409,     0,
     205,   410,   411,   412,   413,     0,   414,   415,   416,   417,
       0,     0,   418,     0,   419,   420,     0,     0,   383,   384,
       0,     0,   389,     0,     0,     0,     0,   205,     0,     0,
     393,   394,   395,   396,   397,   398,   421,     0,   422,   423,
     424,   425,   426,   427,   428,   429,   430,   431,   361,   252,
       0,     0,     0,     0,     0,     0,   432,     0,   433,     0,
       0,     0,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,     0,   206,     0,     0,     0,     0,   362,   253,
       0,     0,     0,   254,     0,     0,     0,     0,     0,   824,
       0,   461,     0,   498,   255,     0,     0,     0,     0,   502,
     206,   256,     0,   505,     0,     0,   508,     0,     0,     0,
     249,   258,   516,     0,   516,   250,   840,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   544,   545,     0,   251,   546,   547,   548,   549,     0,
     551,     0,   553,   554,   555,   556,   557,   559,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,     0,   571,
       0,     0,     0,     0,   205,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   830,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   839,   610,
     252,   205,     0,   615,     0,     0,   617,   618,     0,     0,
       0,   622,   625,     0,     0,     0,     0,   629,     0,     0,
       0,     0,     0,   403,   404,   385,     0,     0,     0,     0,
     253,     0,     0,     0,   254,     0,   639,   170,     0,     0,
     645,     0,     0,   171,     0,   255,     0,     0,     0,   397,
       0,   398,   256,     0,   503,     0,     0,     0,   172,     0,
     398,     0,   258,   666,   389,     0,     0,   669,     0,     0,
       0,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   405,   406,   407,   408,   409,     0,     0,
     410,   411,   412,   413,     0,   414,   415,   416,   417,     0,
       0,   418,     0,   419,   420,     0,     0,   699,     0,     0,
       0,   701,   702,   703,   704,   705,   706,   707,   708,   709,
     710,   711,   712,   713,   714,   715,     0,    43,   719,   424,
     425,   426,   427,   428,   429,   430,   431,     0,     0,   726,
       0,     0,     0,     0,     0,   432,     0,   433,     0,     0,
       0,     0,   711,   715,   387,   249,   249,   249,     0,     0,
     250,   250,   250,    44,     0,     0,   170,     0,     0,   744,
       0,     0,   171,     0,     0,   749,     0,   516,     0,   251,
     251,   251,     0,     0,     0,     0,     0,   172,     0,     0,
     760,     0,     0,     0,     0,     0,     0,   766,     0,     0,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,     0,     0,     0,   252,   252,   252,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   781,   782,
       0,     0,     0,     0,     0,     0,     0,     0,   790,     0,
       0,     0,     0,     0,     0,   253,   253,   253,     0,   254,
     254,   254,   576,   577,     0,     0,    43,   800,     0,     0,
     255,   255,   255,     0,     0,     0,     0,   256,   256,   256,
       0,   498,     0,     0,   575,   806,     0,   258,   258,   258,
     808,     0,   810,   811,   812,     0,     0,   814,     0,   816,
     817,     0,    44,     0,   462,   821,     0,     0,     3,     0,
     463,   464,   465,     0,   466,     0,   280,   281,   282,   283,
       0,     0,     0,     0,     0,     0,     0,   467,   284,   468,
     469,     0,   835,     0,     0,     0,     0,     0,     0,   470,
     285,     0,   842,   471,   286,     0,   287,     0,     9,   288,
     472,     0,   473,   289,     0,     0,   474,   475,     0,     0,
       0,   174,   175,   176,     0,   178,   179,   180,   181,   182,
     290,   184,   185,   186,   187,   188,   189,   190,   191,   192,
       0,   194,   195,   196,     0,     0,   199,   200,   201,   202,
       0,     0,   291,   476,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   292,   293,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,   294,   295,   296,
     297,   298,   299,   300,   301,   302,    43,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,   305,   306,     0,     0,     0,     0,   307,   308,
       0,     0,     0,     0,     0,     0,     0,     0,   309,     0,
     310,   311,    44,   477,   272,   478,   462,     0,     0,     0,
       3,     0,   463,   464,   465,     0,   466,     0,   280,   281,
     282,   283,     0,     0,     0,     0,     0,     0,     0,   467,
     284,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,   470,   285,     0,     0,   471,   286,     0,   287,     0,
       9,   288,   472,     0,   473,   289,     0,     0,   474,   475,
       0,     0,     0,   174,   175,   176,     0,   178,   179,   180,
     181,   182,   290,   184,   185,   186,   187,   188,   189,   190,
     191,   192,     0,   194,   195,   196,     0,     0,   199,   200,
     201,   202,     0,     0,   291,   476,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   292,   293,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     249,   458,     0,     0,     0,   250,     0,     0,     0,   294,
     295,   296,   297,   298,   299,   300,   301,   302,    43,   303,
       0,     0,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   304,   305,   306,     0,     0,     0,     0,
     307,   308,     0,     0,     0,     0,   280,   281,   282,   283,
     309,     0,   310,   311,    44,   477,   272,   804,   284,     0,
       0,     0,     0,     0,   249,     0,     0,     0,     0,   250,
     285,     0,     0,     0,   286,     0,   287,     0,     0,   288,
     252,     0,     0,   289,     0,     0,     0,     0,   251,     0,
       0,   174,   175,   176,     0,   178,   179,   180,   181,   182,
     290,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     253,   194,   195,   196,   254,     0,   199,   200,   201,   202,
       0,     0,   291,     0,     0,   255,     0,     0,     0,     0,
       0,     0,   256,     0,   292,   293,     0,     0,     0,     0,
       0,     0,   258,     0,   252,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   512,   303,     0,     0,
       0,     0,     0,     0,   253,     0,     0,     0,   254,     0,
       0,   304,   305,   306,     0,     0,     0,     0,   307,   513,
       0,   280,   281,   282,   283,     0,   256,   514,   515,     0,
     310,   311,    44,   284,   272,     0,   258,     0,     0,   249,
       0,     0,     0,     0,   250,   285,     0,     0,     0,   286,
       0,   287,     0,     0,   288,     0,     0,     0,   289,     0,
       0,     0,     0,   251,     0,     0,   174,   175,   176,     0,
     178,   179,   180,   181,   182,   290,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     0,   194,   195,   196,     0,
       0,   199,   200,   201,   202,     0,     0,   291,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   292,
     293,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   512,   303,     0,     0,     0,     0,     0,     0,   253,
       0,     0,     0,   254,     0,     0,   304,   305,   306,     0,
       0,     0,     0,   307,   513,     0,   280,   281,   282,   283,
       0,   256,     0,   524,     0,   310,   311,    44,   284,   272,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
     285,     0,     0,     0,   286,     0,   287,     0,     0,   288,
       0,     0,     0,   289,     0,     0,     0,     0,     0,     0,
       0,   174,   175,   176,     0,   178,   179,   180,   181,   182,
     290,   184,   185,   186,   187,   188,   189,   190,   191,   192,
       0,   194,   195,   196,     0,     0,   199,   200,   201,   202,
       0,     0,   291,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   292,   293,     0,     0,     0,     0,
       0,     0,     0,   612,     0,     0,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,   294,   295,   296,
     297,   298,   299,   300,   301,   302,    43,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,   305,   306,   280,   281,   282,   283,   307,   308,
       0,     0,     0,     0,     0,     0,   284,     0,   309,     0,
     310,   311,    44,   613,   272,     0,     0,     0,   285,     0,
       0,     0,   286,     0,   287,     0,     0,   288,     0,     0,
       0,   289,     0,     0,     0,     0,     0,     0,     0,   174,
     175,   176,     0,   178,   179,   180,   181,   182,   290,   184,
     185,   186,   187,   188,   189,   190,   191,   192,     0,   194,
     195,   196,     0,     0,   199,   200,   201,   202,     0,     0,
     291,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   292,   293,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   294,   295,   296,   297,   298,
     299,   300,   301,   302,    43,   303,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   304,
     305,   306,     0,     0,     0,     0,   307,   308,     0,   280,
     281,   282,   283,     0,   536,     0,   309,   537,   310,   311,
      44,   284,   272,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   285,     0,     0,     0,   286,     0,   287,
       0,     0,   288,     0,     0,     0,   289,     0,     0,     0,
       0,     0,     0,     0,   174,   175,   176,     0,   178,   179,
     180,   181,   182,   290,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     0,   194,   195,   196,     0,     0,   199,
     200,   201,   202,     0,     0,   291,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   292,   293,     0,
       0,     0,     0,     0,     0,     0,   623,     0,     0,     0,
       0,     0,   458,     0,     0,     0,     0,     0,     0,     0,
     294,   295,   296,   297,   298,   299,   300,   301,   302,    43,
     303,     0,     0,     0,     0,     0,     0,     0,   280,   281,
     282,   283,   643,     0,   304,   305,   306,     0,     0,     0,
     284,   307,   308,     0,     0,     0,     0,     0,     0,     0,
       0,   309,   285,   310,   311,    44,   286,   272,   287,     0,
       0,   288,     0,     0,     0,   289,     0,     0,     0,     0,
       0,     0,     0,   174,   175,   176,     0,   178,   179,   180,
     181,   182,   290,   184,   185,   186,   187,   188,   189,   190,
     191,   192,     0,   194,   195,   196,     0,     0,   199,   200,
     201,   202,     0,     0,   291,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   292,   293,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   294,
     295,   296,   297,   298,   299,   300,   301,   302,    43,   303,
       0,     0,     0,     0,     0,     0,     0,   280,   281,   282,
     283,   644,     0,   304,   305,   306,     0,     0,     0,   284,
     307,   308,     0,     0,     0,     0,     0,     0,     0,     0,
     309,   285,   310,   311,    44,   286,   272,   287,     0,     0,
     288,     0,     0,     0,   289,     0,     0,     0,     0,     0,
       0,     0,   174,   175,   176,     0,   178,   179,   180,   181,
     182,   290,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,   194,   195,   196,     0,     0,   199,   200,   201,
     202,     0,     0,   291,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   292,   293,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   294,   295,
     296,   297,   298,   299,   300,   301,   302,    43,   303,     0,
       0,     0,     0,     0,     0,     0,   280,   281,   282,   283,
       0,     0,   304,   305,   306,     0,     0,     0,   284,   307,
     308,     0,     0,     0,     0,     0,     0,     0,   349,   309,
     285,   310,   311,    44,   286,   272,   287,     0,     0,   288,
       0,     0,     0,   289,     0,     0,     0,     0,     0,     0,
       0,   174,   175,   176,     0,   178,   179,   180,   181,   182,
     290,   184,   185,   186,   187,   188,   189,   190,   191,   192,
       0,   194,   195,   196,     0,     0,   199,   200,   201,   202,
       0,     0,   291,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   292,   293,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,   294,   295,   296,
     297,   298,   299,   300,   301,   302,    43,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,   305,   306,   280,   281,   282,   283,   307,   308,
       0,   558,     0,     0,     0,     0,   284,     0,   309,     0,
     310,   311,    44,     0,   272,     0,     0,     0,   285,     0,
       0,     0,   286,     0,   287,     0,     0,   288,     0,     0,
       0,   289,     0,     0,     0,     0,     0,     0,     0,   174,
     175,   176,     0,   178,   179,   180,   181,   182,   290,   184,
     185,   186,   187,   188,   189,   190,   191,   192,     0,   194,
     195,   196,     0,     0,   199,   200,   201,   202,     0,     0,
     291,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   292,   293,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   294,   295,   296,   297,   298,
     299,   300,   301,   302,    43,   303,     0,     0,     0,     0,
       0,     0,     0,   280,   281,   282,   283,     0,     0,   304,
     305,   306,     0,     0,     0,   284,   307,   308,     0,     0,
       0,     0,     0,     0,     0,     0,   309,   285,   310,   311,
      44,   286,   272,   287,     0,     0,   288,     0,     0,     0,
     289,     0,     0,     0,     0,     0,     0,     0,   174,   175,
     176,     0,   178,   179,   180,   181,   182,   290,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,   194,   195,
     196,     0,     0,   199,   200,   201,   202,     0,     0,   291,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   292,   293,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   294,   295,   296,   297,   298,   299,
     300,   301,   302,    43,   303,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,   305,
     306,   280,   281,   282,   283,   307,   308,     0,     0,     0,
       0,     0,     0,   284,     0,   309,   572,   310,   311,    44,
       0,   272,     0,     0,     0,   285,     0,     0,     0,   286,
       0,   287,     0,     0,   288,     0,     0,     0,   289,     0,
       0,   621,     0,     0,     0,     0,   174,   175,   176,     0,
     178,   179,   180,   181,   182,   290,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     0,   194,   195,   196,     0,
       0,   199,   200,   201,   202,     0,     0,   291,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   292,
     293,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   294,   295,   296,   297,   298,   299,   300,   301,
     302,    43,   303,     0,     0,     0,     0,     0,     0,     0,
     280,   281,   282,   283,     0,     0,   304,   305,   306,     0,
       0,     0,   284,   307,   308,     0,     0,     0,     0,     0,
       0,     0,     0,   309,   285,   310,   311,    44,   286,   272,
     287,     0,     0,   288,     0,     0,     0,   289,     0,     0,
       0,     0,     0,     0,     0,   174,   175,   176,     0,   178,
     179,   180,   181,   182,   290,   184,   185,   186,   187,   188,
     189,   190,   191,   192,     0,   194,   195,   196,     0,     0,
     199,   200,   201,   202,     0,     0,   291,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   292,   293,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   294,   295,   296,   297,   298,   299,   300,   301,   302,
      43,   303,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   304,   305,   306,   280,   281,
     282,   283,   307,   308,     0,     0,     0,     0,     0,     0,
     284,     0,   309,   641,   310,   311,    44,     0,   272,     0,
       0,     0,   285,     0,     0,     0,   286,     0,   287,     0,
       0,   288,     0,     0,     0,   289,     0,     0,     0,     0,
       0,     0,     0,   174,   175,   176,     0,   178,   179,   180,
     181,   182,   290,   184,   185,   186,   187,   188,   189,   190,
     191,   192,     0,   194,   195,   196,     0,     0,   199,   200,
     201,   202,     0,     0,   291,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   292,   293,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   294,
     295,   296,   297,   298,   299,   300,   301,   302,    43,   303,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   304,   305,   306,   280,   281,   282,   283,
     307,   308,     0,     0,     0,     0,     0,     0,   284,     0,
     309,   655,   310,   311,    44,     0,   272,     0,     0,     0,
     285,     0,     0,     0,   286,     0,   287,     0,     0,   288,
       0,     0,     0,   289,     0,     0,     0,     0,     0,     0,
       0,   174,   175,   176,     0,   178,   179,   180,   181,   182,
     290,   184,   185,   186,   187,   188,   189,   190,   191,   192,
       0,   194,   195,   196,     0,     0,   199,   200,   201,   202,
       0,     0,   291,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   292,   293,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   294,   295,   296,
     297,   298,   299,   300,   301,   302,    43,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,   305,   306,   280,   281,   282,   283,   307,   308,
       0,     0,     0,     0,     0,     0,   284,     0,   309,   663,
     310,   311,    44,     0,   272,     0,     0,     0,   285,     0,
       0,     0,   286,     0,   287,     0,     0,   288,     0,     0,
       0,   289,     0,     0,     0,     0,     0,     0,     0,   174,
     175,   176,     0,   178,   179,   180,   181,   182,   290,   184,
     185,   186,   187,   188,   189,   190,   191,   192,     0,   194,
     195,   196,     0,     0,   199,   200,   201,   202,     0,     0,
     291,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   292,   293,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   732,     0,     0,
       0,     0,     0,     0,     0,   294,   295,   296,   297,   298,
     299,   300,   301,   302,    43,   303,     0,     0,     0,     0,
       0,     0,     0,   280,   281,   282,   283,     0,     0,   304,
     305,   306,     0,     0,     0,   284,   307,   308,     0,     0,
       0,     0,     0,     0,     0,     0,   309,   285,   310,   311,
      44,   286,   272,   287,     0,     0,   288,     0,     0,     0,
     289,     0,     0,     0,     0,     0,     0,     0,   174,   175,
     176,     0,   178,   179,   180,   181,   182,   290,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,   194,   195,
     196,     0,     0,   199,   200,   201,   202,     0,     0,   291,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   292,   293,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,     0,     0,     0,
       0,     0,     0,     0,   294,   295,   296,   297,   298,   299,
     300,   301,   302,    43,   303,     0,     0,     0,     0,     0,
       0,     0,   280,   281,   282,   283,     0,     0,   304,   305,
     306,     0,     0,     0,   284,   307,   308,     0,     0,     0,
       0,     0,     0,     0,     0,   309,   285,   310,   311,    44,
     286,   272,   287,     0,     0,   288,     0,     0,     0,   289,
       0,     0,     0,     0,     0,     0,     0,   174,   175,   176,
       0,   178,   179,   180,   181,   182,   290,   184,   185,   186,
     187,   188,   189,   190,   191,   192,     0,   194,   195,   196,
       0,     0,   199,   200,   201,   202,     0,     0,   291,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     292,   293,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   294,   295,   296,   297,   298,   299,   300,
     301,   302,    43,   303,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   304,   305,   306,
     280,   281,   282,   283,   307,   308,     0,     0,     0,     0,
       0,     0,   284,     0,   309,   764,   310,   311,    44,     0,
     272,     0,     0,     0,   285,     0,     0,     0,   286,     0,
     287,     0,     0,   288,     0,     0,     0,   289,     0,     0,
       0,     0,     0,     0,     0,   174,   175,   176,     0,   178,
     179,   180,   181,   182,   290,   184,   185,   186,   187,   188,
     189,   190,   191,   192,     0,   194,   195,   196,     0,     0,
     199,   200,   201,   202,     0,     0,   291,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   292,   293,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   294,   295,   296,   297,   298,   299,   300,   301,   302,
      43,   303,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   304,   305,   306,   280,   281,
     282,   283,   307,   308,     0,     0,     0,     0,     0,     0,
     284,     0,   309,   789,   310,   311,    44,     0,   272,     0,
       0,     0,   285,     0,     0,     0,   286,     0,   287,     0,
       0,   288,     0,     0,     0,   289,     0,     0,     0,     0,
       0,     0,     0,   174,   175,   176,     0,   178,   179,   180,
     181,   182,   290,   184,   185,   186,   187,   188,   189,   190,
     191,   192,     0,   194,   195,   196,     0,     0,   199,   200,
     201,   202,     0,     0,   291,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   292,   293,     0,     0,
       0,     0,     0,     0,     0,   799,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   294,
     295,   296,   297,   298,   299,   300,   301,   302,    43,   303,
       0,     0,     0,     0,     0,     0,     0,   280,   281,   282,
     283,   807,     0,   304,   305,   306,     0,     0,     0,   284,
     307,   308,     0,     0,     0,     0,     0,     0,     0,     0,
     309,   285,   310,   311,    44,   286,   272,   287,     0,     0,
     288,     0,     0,     0,   289,     0,     0,     0,     0,     0,
       0,     0,   174,   175,   176,     0,   178,   179,   180,   181,
     182,   290,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,   194,   195,   196,     0,     0,   199,   200,   201,
     202,     0,     0,   291,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   292,   293,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   294,   295,
     296,   297,   298,   299,   300,   301,   302,    43,   303,     0,
       0,     0,     0,     0,     0,     0,   280,   281,   282,   283,
     834,     0,   304,   305,   306,     0,     0,     0,   284,   307,
     308,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     285,   310,   311,    44,   286,   272,   287,     0,     0,   288,
       0,     0,     0,   289,     0,     0,     0,     0,     0,     0,
       0,   174,   175,   176,     0,   178,   179,   180,   181,   182,
     290,   184,   185,   186,   187,   188,   189,   190,   191,   192,
       0,   194,   195,   196,     0,     0,   199,   200,   201,   202,
       0,     0,   291,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   292,   293,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   294,   295,   296,
     297,   298,   299,   300,   301,   302,    43,   303,     0,     0,
       0,     0,     0,     0,     0,   280,   281,   282,   283,     0,
       0,   304,   305,   306,     0,     0,     0,   284,   307,   308,
       0,     0,     0,     0,     0,     0,     0,     0,   309,   285,
     310,   311,    44,   286,   272,   287,     0,     0,   288,     0,
       0,     0,   289,     0,     0,     0,     0,     0,     0,     0,
     174,   175,   176,     0,   178,   179,   180,   181,   182,   290,
     184,   185,   186,   187,   188,   189,   190,   191,   192,     0,
     194,   195,   196,     0,     0,   199,   200,   201,   202,     0,
       0,   291,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   292,   293,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   294,   295,   296,   297,
     298,   299,   300,   301,   302,    43,   303,     0,     0,     0,
       0,     0,     0,     0,   280,   281,   282,   283,     0,     0,
     304,   305,   306,     0,     0,     0,   284,   307,   308,   249,
       0,     0,     0,     0,   250,     0,     0,   309,   285,   310,
     311,    44,   286,   272,   287,     0,     0,   288,     0,     0,
       0,   289,     0,   251,     0,     0,     0,     0,     0,   174,
     175,   176,     0,   178,   179,   180,   181,   182,   290,   184,
     185,   186,   187,   188,   189,   190,   191,   192,     0,   194,
     195,   196,     0,     0,   199,   200,   201,   202,     0,     0,
     291,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   292,   293,   403,   404,     0,     0,     0,   252,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   294,   295,   296,   297,   298,
     299,   300,   301,   302,    43,   303,     0,     0,     0,   253,
       0,     0,     0,   254,     0,   580,     0,     0,     0,     0,
       0,     0,     0,     0,   255,     0,   307,   308,     0,     0,
       0,   256,     0,     0,     0,     0,   309,     0,   310,   311,
      44,   258,   272,     0,     0,     0,   595,   596,   597,   598,
     599,   600,   601,   602,   405,   406,   407,   408,   409,   603,
     604,   410,   411,   412,   413,   605,   414,   415,   416,   417,
     403,   404,   418,   606,   419,   420,   607,   608,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   609,   421,     0,   422,   423,
     424,   425,   426,   427,   428,   429,   430,   431,     0,     0,
       0,     0,     0,     0,     0,     0,   432,     0,   433,     0,
       0,     0,     0,     0,     0,   640,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   595,   596,   597,   598,   599,   600,   601,   602,
     405,   406,   407,   408,   409,   603,   604,   410,   411,   412,
     413,   605,   414,   415,   416,   417,   403,   404,   418,   606,
     419,   420,   607,   608,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   609,   421,     0,   422,   423,   424,   425,   426,   427,
     428,   429,   430,   431,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,   433,     0,     0,     0,     0,     0,
       0,   720,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   595,   596,
     597,   598,   599,   600,   601,   602,   405,   406,   407,   408,
     409,   603,   604,   410,   411,   412,   413,   605,   414,   415,
     416,   417,   403,   404,   418,   606,   419,   420,   607,   608,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   609,   421,     0,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
       0,     0,     0,     0,     0,     0,     0,     0,   432,     0,
     433,     0,     0,     0,     0,     0,     0,   727,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   595,   596,   597,   598,   599,   600,
     601,   602,   405,   406,   407,   408,   409,   603,   604,   410,
     411,   412,   413,   605,   414,   415,   416,   417,   403,   404,
     418,   606,   419,   420,   607,   608,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   609,   421,     0,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   431,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,   433,     0,     0,     0,
       0,     0,     0,   734,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     595,   596,   597,   598,   599,   600,   601,   602,   405,   406,
     407,   408,   409,   603,   604,   410,   411,   412,   413,   605,
     414,   415,   416,   417,   403,   404,   418,   606,   419,   420,
     607,   608,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   609,
     421,     0,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,     0,     0,     0,     0,     0,     0,     0,     0,
     432,     0,   433,     0,     0,     0,     0,     0,     0,   772,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
     404,     0,     0,     0,     0,     0,   595,   596,   597,   598,
     599,   600,   601,   602,   405,   406,   407,   408,   409,   603,
     604,   410,   411,   412,   413,   605,   414,   415,   416,   417,
       0,     0,   418,   606,   419,   420,   607,   608,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   609,   421,     0,   422,   423,
     424,   425,   426,   427,   428,   429,   430,   431,     0,     0,
       0,   403,   404,     0,     0,     0,   432,     0,   433,   405,
     406,   407,   408,   409,     0,   774,   410,   411,   412,   413,
       0,   414,   415,   416,   417,     0,     0,   418,     0,   419,
     420,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   421,     0,   422,   423,   424,   425,   426,   427,   428,
     429,   430,   431,     0,     0,     0,     0,     0,     0,   403,
     404,   432,     0,   433,     0,     0,     0,     0,     0,     0,
     827,   405,   406,   407,   408,   409,     0,     0,   410,   411,
     412,   413,     0,   414,   415,   416,   417,     0,     0,   418,
       0,   419,   420,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   421,     0,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,     0,     0,   403,   404,     0,
       0,     0,     0,   432,     0,   433,     0,     0,   533,   405,
     406,   407,   408,   409,     0,     0,   410,   411,   412,   413,
       0,   414,   415,   416,   417,     0,     0,   418,     0,   419,
     420,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   421,     0,   422,   423,   424,   425,   426,   427,   428,
     429,   430,   431,     0,     0,   403,   404,     0,     0,     0,
       0,   432,     0,   433,     0,     0,   646,   405,   406,   407,
     408,   409,     0,     0,   410,   411,   412,   413,     0,   414,
     415,   416,   417,     0,     0,   418,     0,   419,   420,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   421,
       0,   422,   423,   424,   425,   426,   427,   428,   429,   430,
     431,     0,     0,   403,   404,     0,     0,     0,     0,   432,
       0,   433,     0,     0,   649,   405,   406,   407,   408,   409,
       0,     0,   410,   411,   412,   413,     0,   414,   415,   416,
     417,     0,     0,   418,     0,   419,   420,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   421,     0,   422,
     423,   424,   425,   426,   427,   428,   429,   430,   431,     0,
       0,   403,   404,     0,     0,     0,     0,   432,     0,   433,
       0,     0,   738,   405,   406,   407,   408,   409,     0,     0,
     410,   411,   412,   413,     0,   414,   415,   416,   417,     0,
       0,   418,     0,   419,   420,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   421,     0,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,     0,     0,   403,
     404,     0,     0,     0,     0,   432,     0,   433,     0,     0,
     813,   405,   406,   407,   408,   409,     0,     0,   410,   411,
     412,   413,     0,   414,   415,   416,   417,     0,     0,   418,
       0,   419,   420,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   421,     0,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,     0,     0,     0,     0,     0,
       0,     0,     0,   432,     0,   433,     0,     0,   825,   405,
     406,   407,   408,   409,     0,     0,   410,   411,   412,   413,
       0,   414,   415,   416,   417,     0,     0,   418,     0,   419,
     420,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   170,     0,     0,     0,     0,     0,   171,
       0,   421,     0,   422,   423,   424,   425,   426,   427,   428,
     429,   430,   431,     0,   172,     0,     0,     0,     0,     0,
       0,   432,     0,   433,     0,     0,   841,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     170,     0,     0,     0,     0,     0,   171,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   172,     0,     0,   294,   295,   296,     0,     0,     0,
       0,     0,     0,    43,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,     0,   403,   404,    44,
       0,     0,     0,     0,     0,     0,     0,   170,     0,     0,
       0,     0,     0,   171,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   172,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,     0,   403,   404,    44,   405,   406,   407,
     408,   409,     0,     0,   410,   411,   412,   413,     0,   414,
     415,   416,   417,     0,     0,   418,     0,   419,   420,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   448,     0,   421,
       0,   422,   423,   424,   425,   426,   427,   428,   429,   430,
     431,     0,     0,     0,     0,     0,     0,     0,     0,   432,
       0,   433,   455,   403,   404,     0,     0,     0,     0,     0,
       0,     0,     0,    44,   405,   406,   407,   408,   409,     0,
       0,   410,   411,   412,   413,     0,   414,   415,   416,   417,
       0,     0,   418,     0,   419,   420,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   421,     0,   422,   423,
     424,   425,   426,   427,   428,   429,   430,   431,     0,     0,
       0,     0,     0,     0,     0,     0,   432,     0,   433,   679,
     403,   404,     0,   405,   406,   407,   408,   409,     0,     0,
     410,   411,   412,   413,     0,   414,   415,   416,   417,     0,
       0,   418,     0,   419,   420,     0,     0,     0,     0,     0,
       0,     0,     0,   249,     0,     0,     0,     0,   250,     0,
       0,     0,     0,     0,     0,   421,     0,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   251,     0,     0,
       0,     0,     0,     0,     0,   432,     0,   433,   682,     0,
       0,     0,   595,   596,   597,   598,   599,   600,   601,   602,
     405,   406,   407,   408,   409,   603,   604,   410,   411,   412,
     413,   605,   414,   415,   416,   417,  -200,     0,   418,   606,
     419,   420,   607,   608,   403,   404,     0,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,     0,     0,     0,
       0,   609,   421,     0,   422,   423,   424,   425,   426,   427,
     428,   429,   430,   431,     0,     0,     0,     0,     0,     0,
       0,     0,   432,   253,   433,     0,     0,   254,     0,   582,
       0,     0,     0,     0,     0,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,   256,     0,     0,     0,   403,
     404,     0,     0,     0,     0,   258,   595,   596,   597,   598,
     599,   600,   601,   602,   405,   406,   407,   408,   409,   603,
     604,   410,   411,   412,   413,   636,   414,   415,   416,   417,
       0,     0,   418,   606,   419,   420,   607,   608,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   637,   421,     0,   422,   423,
     424,   425,   426,   427,   428,   429,   430,   431,   403,   404,
       0,     0,     0,     0,     0,     0,   432,     0,   433,   405,
     406,   407,   408,   409,     0,     0,   410,   411,   412,   413,
       0,   414,   415,   416,   417,     0,     0,   418,     0,   419,
     420,     0,     0,   527,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   421,     0,   422,   423,   424,   425,   426,   427,   428,
     429,   430,   431,   403,   404,     0,     0,     0,     0,     0,
       0,   432,     0,   433,     0,     0,     0,     0,   405,   406,
     407,   408,   409,     0,     0,   410,   411,   412,   413,     0,
     414,   415,   416,   417,     0,     0,   418,     0,   419,   420,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     421,   681,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   403,   404,     0,     0,     0,     0,     0,     0,
     432,     0,   433,   405,   406,   407,   408,   409,     0,     0,
     410,   411,   412,   413,     0,   414,   415,   416,   417,     0,
       0,   418,     0,   419,   420,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   756,     0,     0,   421,     0,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   403,   404,     0,
       0,     0,     0,     0,     0,   432,     0,   433,     0,     0,
       0,     0,   405,   406,   407,   408,   409,     0,     0,   410,
     411,   412,   413,     0,   414,   415,   416,   417,     0,     0,
     418,     0,   419,   420,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   421,     0,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   431,   403,   404,     0,     0,
       0,     0,     0,     0,   432,     0,   433,   405,   406,   407,
     408,   409,     0,     0,   410,   411,   412,   413,     0,   414,
     415,   416,   417,     0,     0,   418,     0,   419,   420,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   423,   424,   425,   426,   427,   428,   429,   430,
     431,   403,   404,     0,     0,     0,     0,     0,     0,   432,
       0,   433,     0,     0,     0,     0,   405,   406,   407,   408,
     409,     0,     0,   410,   411,   412,   413,     0,   414,   415,
     416,   417,     0,     0,   418,     0,   419,   420,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     249,   249,     0,     0,     0,   250,   250,   403,   404,     0,
       0,     0,     0,   425,   426,   427,   428,   429,   430,   431,
       0,     0,     0,     0,   251,   251,     0,     0,   432,     0,
     433,   405,   406,     0,     0,   409,     0,     0,   410,   411,
     412,   413,   249,   414,   415,   416,   417,   250,     0,   418,
     249,   419,   420,     0,     0,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   251,     0,     0,     0,
       0,     0,     0,   421,   251,   422,   423,   424,   425,   426,
     252,   252,     0,   430,   431,     0,     0,   405,   406,   407,
     408,   409,     0,   432,   410,   433,     0,   413,     0,   414,
     415,   416,   417,     0,     0,   418,     0,   419,   420,     0,
     253,   253,     0,     0,   254,   254,   584,   651,     0,     0,
       0,     0,   252,   249,   249,   255,   255,     0,   250,   250,
     252,     0,   256,   256,   425,   426,   427,   428,   429,   430,
     431,     0,   258,   258,     0,     0,     0,   251,   251,   432,
       0,   433,   253,     0,     0,     0,   254,     0,   739,     0,
     253,     0,     0,   249,   254,   249,   740,   255,   250,     0,
     250,     0,     0,     0,   256,   255,     0,     0,     0,     0,
       0,     0,   256,     0,   258,     0,     0,   251,     0,   251,
     249,     0,   258,     0,     0,   250,   249,     0,     0,     0,
       0,   250,     0,   252,   252,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   251,     0,     0,     0,     0,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   253,   253,     0,     0,   254,   254,   741,
     767,     0,     0,   252,     0,   252,     0,     0,   255,   255,
       0,     0,     0,     0,     0,   256,   256,     0,     0,     0,
       0,     0,     0,     0,     0,   258,   258,     0,     0,     0,
     252,     0,     0,   253,     0,   253,   252,   254,     0,   254,
       0,   796,     0,     0,     0,     0,     0,     0,   255,     0,
     255,     0,     0,     0,     0,   256,     0,   256,   783,     0,
     253,     0,     0,     0,   254,   258,   253,   258,     0,     0,
     254,     0,     0,     0,     0,   255,     0,     0,     0,     0,
       0,   255,   256,     0,     0,   833,     0,     0,   256,     0,
       0,   845,   258,     0,     0,     0,     0,     0,   258,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   134,   135,   136,
     137,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,     0,     0,   148,   149
};

static const yytype_int16 yycheck[] =
{
      13,   105,   261,   154,   367,   435,   225,   268,   338,   388,
     340,    71,   342,   365,   443,    32,   445,   105,   447,     0,
     296,   373,   536,    20,     5,    13,    14,    33,    18,    45,
     115,   458,    18,    31,    18,   266,   112,   113,   133,    19,
      20,    17,   123,   133,   110,   110,   171,    28,   163,    30,
     133,    32,   139,    34,   133,    52,   133,   172,    39,    72,
      31,   133,   133,   133,    45,    36,   135,   136,    19,    20,
      51,   139,   402,   503,   140,   140,   109,   172,   133,   112,
     113,   139,   172,   170,    55,   170,    67,   163,   165,   172,
     171,   146,   105,   172,   139,   101,   102,   169,   166,    31,
     113,   172,   172,   169,    36,    13,    14,    88,   140,   133,
     133,   540,   170,   142,   169,   467,   133,   133,   127,   133,
     224,   101,   102,    55,   476,   170,   357,   133,   161,   109,
     163,   111,   112,   113,   114,    56,   224,   117,   170,   127,
     111,   142,   171,   131,   658,   133,   134,   145,   500,   139,
     101,   102,   169,   139,   151,   139,   260,   133,   109,   219,
     111,   112,   113,   114,   161,   163,   117,   594,   169,   133,
     141,   275,   260,   140,   145,   173,   147,   165,   282,   111,
     139,   161,   163,   163,   166,   156,   139,   275,   170,   170,
     294,   295,   163,   139,   282,   147,   457,   148,   149,   150,
     151,   152,   173,   142,   139,   164,   294,   295,   164,   141,
     161,   224,   163,   145,   170,   139,    19,    20,   170,   127,
     166,   133,   652,   131,   156,   133,   134,   331,   332,   333,
     334,   163,   662,   337,   385,   339,   387,   341,   170,   343,
     139,   173,   166,   331,   332,   333,   334,   260,   678,   337,
     469,   339,   528,   341,   139,   139,   110,   171,   164,   363,
     612,   133,   275,   139,   170,   139,   147,   166,   164,   282,
     147,   623,    19,    20,   170,   363,   655,   133,   382,   164,
     164,   294,   295,   169,   663,   139,   140,   717,   142,   170,
     166,   145,   166,   170,   382,   110,    99,   100,   101,   102,
      19,    20,   163,   118,   147,   166,   109,   133,   111,   112,
     113,   114,   165,   133,   117,   466,   119,   120,   331,   332,
     333,   334,   127,    10,   337,   140,   339,   170,   341,   758,
     343,   590,   133,   142,    21,    22,   133,   139,   147,   133,
     142,   671,     4,   145,     6,   148,   149,   150,   151,   152,
     363,   140,   171,    62,   101,   102,   165,    66,   161,   170,
     163,   172,   109,   171,   133,   112,   113,   114,   142,   382,
     117,   171,    19,    20,    83,    84,    85,    86,    33,    33,
      99,   100,   101,   102,   103,   165,   399,   106,   107,   108,
     109,   610,   111,   112,   113,   114,     1,   135,   117,   618,
     119,   120,   506,   507,   170,   509,   172,   167,   168,    52,
     629,   171,   775,   171,   161,   142,   163,   133,   506,   507,
     147,   509,   141,   133,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   142,   142,   142,   541,   165,   170,
     147,   147,   161,   146,   163,   135,   136,   137,   170,   146,
     172,    31,   171,   541,   101,   102,    36,   676,   165,   165,
     146,   146,   109,   165,   111,   112,   113,   114,   146,   146,
     117,   575,   170,   146,   172,    55,   146,   146,   146,   170,
     133,   142,   586,   634,   588,   746,   170,   575,   171,    42,
     170,   133,   163,   506,   507,   133,   509,   146,   586,   165,
     165,   146,   721,   150,   151,   152,   165,   146,   168,   146,
     166,   163,   145,   164,   161,   165,   163,   133,   133,   133,
     133,   133,   115,   127,   170,    35,    18,   135,   541,   170,
     110,   111,   164,   139,   170,   170,   170,   139,   118,   643,
     170,   172,   133,   165,    19,    20,   133,   133,   147,   147,
     147,    12,   142,   127,   171,   643,    38,   776,   133,   166,
     140,   141,   575,   133,   165,   145,   670,   133,   164,   164,
     164,   147,   164,   586,   147,   588,   156,    48,   392,   164,
     164,   164,   670,   163,   803,   164,   164,   806,    53,    94,
     170,   172,   236,   173,    56,   823,    54,   318,    72,    31,
     728,     1,   344,   270,    36,   233,   369,   218,   634,   588,
     167,    66,   256,   336,   661,   388,   817,    -1,   282,    -1,
      -1,    -1,    -1,    55,    99,   100,   101,   102,   103,    -1,
     643,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,   120,    -1,    -1,   292,   293,
      -1,    -1,   296,    -1,    -1,    -1,    -1,   670,    -1,    -1,
     304,   305,   306,   307,   308,   309,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   110,   111,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   807,    -1,    -1,    -1,    -1,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,   807,
      -1,   365,    -1,   367,   156,    -1,    -1,    -1,    -1,   373,
     834,   163,    -1,   377,    -1,    -1,   380,    -1,    -1,    -1,
      31,   173,   386,    -1,   388,    36,   834,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   405,   406,    -1,    55,   409,   410,   411,   412,    -1,
     414,    -1,   416,   417,   418,   419,   420,   421,   422,   423,
     424,   425,   426,   427,   428,   429,   430,   431,    -1,   433,
      -1,    -1,    -1,    -1,   807,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   819,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   831,   463,
     111,   834,    -1,   467,    -1,    -1,   470,   471,    -1,    -1,
      -1,   475,   476,    -1,    -1,    -1,    -1,   481,    -1,    -1,
      -1,    -1,    -1,    19,    20,    11,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,   145,    -1,   500,    23,    -1,    -1,
     504,    -1,    -1,    29,    -1,   156,    -1,    -1,    -1,   513,
      -1,   515,   163,    -1,   165,    -1,    -1,    -1,    44,    -1,
     524,    -1,   173,   527,   528,    -1,    -1,   531,    -1,    -1,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    99,   100,   101,   102,   103,    -1,    -1,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,    -1,
      -1,   117,    -1,   119,   120,    -1,    -1,   591,    -1,    -1,
      -1,   595,   596,   597,   598,   599,   600,   601,   602,   603,
     604,   605,   606,   607,   608,   609,    -1,   133,   612,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,    -1,   623,
      -1,    -1,    -1,    -1,    -1,   161,    -1,   163,    -1,    -1,
      -1,    -1,   636,   637,    11,    31,    31,    31,    -1,    -1,
      36,    36,    36,   169,    -1,    -1,    23,    -1,    -1,   653,
      -1,    -1,    29,    -1,    -1,   659,    -1,   661,    -1,    55,
      55,    55,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
     674,    -1,    -1,    -1,    -1,    -1,    -1,   681,    -1,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    -1,    -1,   111,   111,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   732,   733,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   742,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   141,   141,    -1,   145,
     145,   145,   147,   147,    -1,    -1,   133,   761,    -1,    -1,
     156,   156,   156,    -1,    -1,    -1,    -1,   163,   163,   163,
      -1,   775,    -1,    -1,   170,   779,    -1,   173,   173,   173,
     784,    -1,   786,   787,   788,    -1,    -1,   791,    -1,   793,
     794,    -1,   169,    -1,     1,   799,    -1,    -1,     5,    -1,
       7,     8,     9,    -1,    11,    -1,    13,    14,    15,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    26,
      27,    -1,   826,    -1,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,   836,    40,    41,    -1,    43,    -1,    45,    46,
      47,    -1,    49,    50,    -1,    -1,    53,    54,    -1,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    79,    80,    -1,    -1,    83,    84,    85,    86,
      -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,    -1,    -1,    -1,    -1,   155,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
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
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
     111,    -1,    -1,    50,    -1,    -1,    -1,    -1,    55,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
     141,    78,    79,    80,   145,    -1,    83,    84,    85,    86,
      -1,    -1,    89,    -1,    -1,   156,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,   145,    -1,
      -1,   148,   149,   150,    -1,    -1,    -1,    -1,   155,   156,
      -1,    13,    14,    15,    16,    -1,   163,   164,   165,    -1,
     167,   168,   169,    25,   171,    -1,   173,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    55,    -1,    -1,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    79,    80,    -1,
      -1,    83,    84,    85,    86,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,   145,    -1,    -1,   148,   149,   150,    -1,
      -1,    -1,    -1,   155,   156,    -1,    13,    14,    15,    16,
      -1,   163,    -1,   165,    -1,   167,   168,   169,    25,   171,
      -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    79,    80,    -1,    -1,    83,    84,    85,    86,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,    13,    14,    15,    16,   155,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,   165,    -1,
     167,   168,   169,   170,   171,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    -1,    78,
      79,    80,    -1,    -1,    83,    84,    85,    86,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
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
     156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,   165,
      37,   167,   168,   169,    41,   171,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    79,    80,    -1,    -1,    83,    84,    85,    86,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,    13,    14,    15,    16,   155,   156,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,   165,    -1,
     167,   168,   169,    -1,   171,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    -1,    78,
      79,    80,    -1,    -1,    83,    84,    85,    86,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,    13,    14,    15,    16,   155,   156,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,   165,   166,   167,   168,   169,
      -1,   171,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    79,    80,    -1,
      -1,    83,    84,    85,    86,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    14,    15,    16,    -1,    -1,   148,   149,   150,    -1,
      -1,    -1,    25,   155,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    37,   167,   168,   169,    41,   171,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    -1,    78,    79,    80,    -1,    -1,
      83,    84,    85,    86,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,   150,    13,    14,
      15,    16,   155,   156,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,   165,   166,   167,   168,   169,    -1,   171,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    -1,    78,    79,    80,    -1,    -1,    83,    84,
      85,    86,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,    13,    14,    15,    16,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
     165,   166,   167,   168,   169,    -1,   171,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    79,    80,    -1,    -1,    83,    84,    85,    86,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,    13,    14,    15,    16,   155,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,   165,   166,
     167,   168,   169,    -1,   171,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    -1,    78,
      79,    80,    -1,    -1,    83,    84,    85,    86,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
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
     150,    -1,    -1,    -1,    25,   155,   156,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,    37,   167,   168,   169,
      41,   171,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    -1,    78,    79,    80,
      -1,    -1,    83,    84,    85,    86,    -1,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,
      13,    14,    15,    16,   155,   156,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,   165,   166,   167,   168,   169,    -1,
     171,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    -1,    78,    79,    80,    -1,    -1,
      83,    84,    85,    86,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,   150,    13,    14,
      15,    16,   155,   156,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,   165,   166,   167,   168,   169,    -1,   171,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    -1,    78,    79,    80,    -1,    -1,    83,    84,
      85,    86,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    37,
     167,   168,   169,    41,   171,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    -1,
      78,    79,    80,    -1,    -1,    83,    84,    85,    86,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,
     148,   149,   150,    -1,    -1,    -1,    25,   155,   156,    31,
      -1,    -1,    -1,    -1,    36,    -1,    -1,   165,    37,   167,
     168,   169,    41,   171,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    55,    -1,    -1,    -1,    -1,    -1,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    -1,    78,
      79,    80,    -1,    -1,    83,    84,    85,    86,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,    19,    20,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,   141,
      -1,    -1,    -1,   145,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,   155,   156,    -1,    -1,
      -1,   163,    -1,    -1,    -1,    -1,   165,    -1,   167,   168,
     169,   173,   171,    -1,    -1,    -1,    91,    92,    93,    94,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    19,    20,   117,   118,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    -1,    -1,    -1,    -1,    -1,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      -1,    -1,   117,   118,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,    -1,
      -1,    19,    20,    -1,    -1,    -1,   161,    -1,   163,    99,
     100,   101,   102,   103,    -1,   170,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,   161,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    99,   100,   101,   102,   103,    -1,    -1,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,    -1,    -1,   117,
      -1,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,    -1,    19,    20,    -1,
      -1,    -1,    -1,   161,    -1,   163,    -1,    -1,   166,    99,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,   124,   125,   126,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    19,    20,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    19,    20,   169,    99,   100,   101,
     102,   103,    -1,    -1,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,   141,
      -1,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,   163,   164,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    99,   100,   101,   102,   103,    -1,
      -1,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,   164,
      19,    20,    -1,    99,   100,   101,   102,   103,    -1,    -1,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,    -1,
      -1,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,   163,   164,    -1,
      -1,    -1,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,   141,   163,    -1,    -1,   145,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,    19,
      20,    -1,    -1,    -1,    -1,   173,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      -1,    -1,   117,   118,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,    99,
     100,   101,   102,   103,    -1,    -1,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
     120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,   163,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,   103,    -1,    -1,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,    -1,    -1,   117,    -1,   119,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,   163,    99,   100,   101,   102,   103,    -1,    -1,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,    -1,
      -1,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,   163,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,   103,    -1,    -1,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,    -1,    -1,
     117,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,   163,    99,   100,   101,
     102,   103,    -1,    -1,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,   163,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
     103,    -1,    -1,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    31,    -1,    -1,    -1,    36,    36,    19,    20,    -1,
      -1,    -1,    -1,   146,   147,   148,   149,   150,   151,   152,
      -1,    -1,    -1,    -1,    55,    55,    -1,    -1,   161,    -1,
     163,    99,   100,    -1,    -1,   103,    -1,    -1,   106,   107,
     108,   109,    31,   111,   112,   113,   114,    36,    -1,   117,
      31,   119,   120,    -1,    -1,    36,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    55,   143,   144,   145,   146,   147,
     111,   111,    -1,   151,   152,    -1,    -1,    99,   100,   101,
     102,   103,    -1,   161,   106,   163,    -1,   109,    -1,   111,
     112,   113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,
     141,   141,    -1,    -1,   145,   145,   147,   147,    -1,    -1,
      -1,    -1,   111,    31,    31,   156,   156,    -1,    36,    36,
     111,    -1,   163,   163,   146,   147,   148,   149,   150,   151,
     152,    -1,   173,   173,    -1,    -1,    -1,    55,    55,   161,
      -1,   163,   141,    -1,    -1,    -1,   145,    -1,   147,    -1,
     141,    -1,    -1,    31,   145,    31,   147,   156,    36,    -1,
      36,    -1,    -1,    -1,   163,   156,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    -1,   173,    -1,    -1,    55,    -1,    55,
      31,    -1,   173,    -1,    -1,    36,    31,    -1,    -1,    -1,
      -1,    36,    -1,   111,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   141,    -1,    -1,   145,   145,   147,
     147,    -1,    -1,   111,    -1,   111,    -1,    -1,   156,   156,
      -1,    -1,    -1,    -1,    -1,   163,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   173,   173,    -1,    -1,    -1,
     111,    -1,    -1,   141,    -1,   141,   111,   145,    -1,   145,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
     156,    -1,    -1,    -1,    -1,   163,    -1,   163,   166,    -1,
     141,    -1,    -1,    -1,   145,   173,   141,   173,    -1,    -1,
     145,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,
      -1,   156,   163,    -1,    -1,   166,    -1,    -1,   163,    -1,
      -1,   166,   173,    -1,    -1,    -1,    -1,    -1,   173,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,    -1,   155,   156
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   175,     0,     5,    28,    30,    32,    34,    39,    45,
      51,    67,    88,   163,   170,   176,   181,   182,   186,   205,
     209,   223,   265,   270,   271,   274,   309,   311,   133,   200,
     201,   127,   187,   188,   133,   183,   184,   133,   171,   133,
     312,   310,    32,   133,   169,   202,   203,   204,   214,     4,
       6,    56,   264,   140,   139,   142,   139,    20,    52,   151,
     161,   185,   142,   171,   133,   268,   269,   133,   133,   169,
     133,   165,   139,   164,   133,   273,    33,   101,   102,   133,
     208,   171,   266,    13,    14,   127,   131,   133,   134,   165,
     178,   200,   127,   188,   133,   133,   133,    62,    66,    83,
      84,    85,    86,   277,   267,   140,   172,   268,   171,   171,
     133,   201,   203,   142,   272,   171,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   119,   120,   121,   122,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   155,   156,
      33,    33,   165,   206,   263,     1,   135,   177,    13,    14,
     127,   131,   133,   178,   198,   199,   185,   171,   133,   172,
      23,    29,    44,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,   214,   276,   278,   279,   281,
     284,   133,   280,   133,   256,   257,   166,   214,   251,   163,
     166,   248,   252,   253,   142,   207,   133,   172,   262,   275,
     170,   135,   136,   139,   166,   267,   140,   170,   146,   146,
     146,   146,   165,   146,   146,   146,   146,   146,   146,    31,
      36,    55,   111,   141,   145,   156,   163,   170,   173,   170,
     142,   170,   172,   248,   250,   201,    45,   259,   275,   166,
     170,   284,   171,   210,   139,   142,   145,   261,   198,   172,
      13,    14,    15,    16,    25,    37,    41,    43,    46,    50,
      67,    89,   101,   102,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   134,   148,   149,   150,   155,   156,   165,
     167,   168,   180,   210,   214,   216,   225,   232,   235,   236,
     237,   240,   241,   242,   247,   276,   313,   316,   321,   323,
     325,   287,   289,   291,   285,   133,   282,   293,   295,   297,
     299,   301,   303,   305,   307,    31,   145,   163,   173,   164,
     247,   133,   172,   284,   172,   256,    42,   249,   170,   164,
     259,   110,   140,   142,   145,   258,   252,   212,   133,   284,
     110,   118,   140,   260,   284,   313,   165,   165,   146,   146,
     165,   146,   146,   247,   247,    11,   284,    11,   284,   247,
     318,   322,   179,   247,   247,   247,   247,   247,   247,   168,
     133,   165,   205,    19,    20,    99,   100,   101,   102,   103,
     106,   107,   108,   109,   111,   112,   113,   114,   117,   119,
     120,   141,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   161,   163,   170,   165,   284,   284,   284,   284,
     166,   280,   284,   206,   284,   206,   284,   206,   133,   254,
     255,   284,   257,   145,   164,   164,   259,   284,   116,   213,
     238,   247,     1,     7,     8,     9,    11,    24,    26,    27,
      36,    40,    47,    49,    53,    54,    90,   170,   172,   189,
     190,   193,   194,   195,   196,   197,   211,   213,   215,   217,
     218,   219,   220,   221,   223,   224,   238,   239,   247,   170,
     260,   213,   247,   165,   133,   247,   226,   228,   247,   230,
     284,   275,   133,   156,   164,   165,   247,   314,   315,   317,
     318,   319,   320,   275,   165,   315,   320,   123,   170,   172,
     136,   137,   177,   166,   146,   214,   163,   166,   233,   247,
     206,    17,   133,   133,   247,   247,   247,   247,   247,   247,
     133,   247,   133,   247,   247,   247,   247,   247,    20,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     133,   247,   166,   233,   147,   170,   147,   147,   147,   170,
     147,   207,   147,   207,   147,   207,   142,   147,   170,   147,
     170,   258,   168,   236,   115,    91,    92,    93,    94,    95,
      96,    97,    98,   104,   105,   110,   118,   121,   122,   140,
     247,   275,   110,   170,   213,   247,   210,   247,   247,   170,
     127,    53,   247,   110,   213,   247,    35,   170,   170,   247,
     170,   170,   170,    18,   222,   170,   110,   140,   213,   247,
     170,   166,   233,    17,   146,   247,   166,   284,   284,   166,
     284,   147,    18,   258,   164,   166,   139,   164,   170,   139,
     164,   170,    18,   166,   172,   172,   247,   318,   172,   247,
     243,   245,   133,   314,   139,   166,   207,   284,   165,   164,
     133,   142,   164,   166,   288,   284,   292,   286,   283,   294,
     147,   298,   147,   302,   147,   284,   306,   254,   308,   247,
     236,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   210,    18,   213,   247,
     170,    12,   210,   142,   127,   213,   247,   170,   171,   210,
      38,   262,   116,   116,   170,   166,   284,   133,   166,   147,
     147,   147,   165,   233,   247,   315,   133,   164,   314,   247,
     164,   319,   233,   315,   164,   164,   138,   284,   206,   164,
     247,   123,   210,   234,   166,   233,   247,   147,   296,   300,
     304,   233,   170,   210,   170,   212,    10,    21,    22,   191,
     192,   247,   247,   166,   147,   170,   227,   229,   231,   166,
     247,   170,   164,   258,   170,   172,   147,   207,   166,   110,
     247,   166,   290,   170,   172,   210,   247,    17,   247,   133,
     247,   247,   247,   166,   247,   164,   247,   247,   164,   244,
     147,   247,   210,   210,   284,   166,   147,   170,   324,   324,
     214,   246,   192,   166,    17,   247,    48,   164,   172,   214,
     284,   166,   247,   164,   164,   166
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   174,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   176,   177,   177,   178,   179,
     179,   179,   180,   181,   182,   183,   183,   183,   184,   184,
     185,   185,   186,   187,   187,   188,   188,   189,   190,   190,
     191,   191,   192,   192,   192,   193,   193,   194,   195,   196,
     197,   198,   198,   198,   198,   198,   198,   199,   199,   200,
     200,   200,   200,   200,   200,   200,   200,   201,   201,   202,
     202,   203,   203,   204,   204,   205,   205,   206,   206,   206,
     207,   207,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   209,
     210,   210,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     212,   212,   212,   213,   213,   214,   214,   214,   215,   216,
     216,   216,   216,   217,   218,   219,   219,   219,   219,   219,
     220,   220,   220,   220,   221,   222,   222,   223,   223,   224,
     226,   227,   225,   228,   229,   225,   230,   231,   225,   232,
     232,   232,   232,   232,   232,   233,   233,   234,   234,   234,
     235,   235,   236,   236,   237,   237,   237,   237,   237,   237,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   239,   239,   240,   241,
     241,   242,   243,   244,   242,   245,   246,   242,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   248,   248,   249,   249,   250,   251,   251,   252,   252,
     253,   253,   254,   254,   255,   255,   256,   257,   257,   258,
     258,   259,   259,   259,   259,   259,   259,   260,   260,   260,
     261,   261,   262,   262,   262,   262,   262,   263,   263,   264,
     264,   265,   266,   265,   267,   267,   267,   268,   269,   269,
     270,   271,   271,   272,   272,   273,   274,   275,   275,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   277,   277,   277,   277,   277,   277,
     278,   279,   279,   280,   280,   282,   283,   281,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   285,   286,   284,   284,   287,
     288,   284,   289,   290,   284,   291,   292,   284,   284,   293,
     294,   284,   295,   296,   284,   284,   297,   298,   284,   299,
     300,   284,   284,   301,   302,   284,   303,   304,   284,   305,
     306,   284,   307,   308,   284,   310,   309,   312,   311,   313,
     313,   313,   313,   314,   314,   315,   315,   316,   316,   316,
     316,   316,   317,   317,   318,   319,   319,   320,   320,   321,
     321,   322,   322,   323,   324,   324,   325,   325
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     0,
       2,     4,     3,     2,     2,     1,     3,     3,     2,     4,
       0,     1,     2,     1,     3,     1,     3,     3,     3,     2,
       1,     1,     0,     2,     4,     1,     1,     4,     6,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     1,     5,     1,     3,     1,
       1,     1,     4,     1,     3,     0,     3,     0,     2,     3,
       0,     2,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     6,
       3,     7,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     2,     2,     2,
       0,     2,     2,     3,     2,     1,     3,     2,     2,     2,
       4,     5,     2,     1,     1,     2,     3,     4,     2,     3,
       3,     4,     2,     3,     4,     0,     2,     1,     1,     3,
       0,     0,     7,     0,     0,     7,     0,     0,     7,     5,
       8,    10,     6,     9,    11,     1,     3,     1,     2,     3,
       1,     1,     1,     5,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     4,     6,     5,
       6,     3,     0,     0,     8,     0,     0,     9,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     3,     4,     4,     3,     3,     1,     3,     4,     3,
       4,     2,     4,     4,     6,     7,     3,     5,     4,     3,
       3,     4,     1,     1,     1,     1,     1,     1,     3,     3,
       2,     0,     3,     0,     1,     3,     0,     3,     2,     3,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       1,     1,     2,     3,     5,     3,     3,     1,     1,     1,
       0,     1,     4,     6,     5,     5,     4,     0,     2,     0,
       1,     5,     0,     5,     0,     3,     5,     4,     1,     2,
       4,     5,     7,     0,     2,     2,     6,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     1,     3,     0,     0,     6,     1,     1,
       1,     1,     4,     3,     4,     2,     3,     2,     4,     3,
       2,     2,     3,     2,     2,     0,     0,     6,     2,     0,
       0,     6,     0,     0,     8,     0,     0,     6,     1,     0,
       0,     6,     0,     0,     7,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     0,
       0,     6,     0,     0,     6,     0,     7,     0,     7,     1,
       1,     1,     1,     3,     5,     1,     3,     5,     4,     7,
       5,     7,     1,     3,     3,     1,     1,     1,     3,     5,
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
    case 133: /* "name"  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3229 "ds_parser.cpp"
        break;

    case 177: /* character_sequence  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3235 "ds_parser.cpp"
        break;

    case 178: /* string_constant  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3241 "ds_parser.cpp"
        break;

    case 179: /* string_builder_body  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3247 "ds_parser.cpp"
        break;

    case 180: /* string_builder  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3253 "ds_parser.cpp"
        break;

    case 183: /* require_module_name  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3259 "ds_parser.cpp"
        break;

    case 189: /* expression_label  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3265 "ds_parser.cpp"
        break;

    case 190: /* expression_goto  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3271 "ds_parser.cpp"
        break;

    case 192: /* expression_else  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3277 "ds_parser.cpp"
        break;

    case 194: /* expression_if_then_else  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3283 "ds_parser.cpp"
        break;

    case 195: /* expression_for_loop  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3289 "ds_parser.cpp"
        break;

    case 196: /* expression_while_loop  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3295 "ds_parser.cpp"
        break;

    case 197: /* expression_with  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3301 "ds_parser.cpp"
        break;

    case 198: /* annotation_argument_value  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3307 "ds_parser.cpp"
        break;

    case 199: /* annotation_argument_value_list  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3313 "ds_parser.cpp"
        break;

    case 200: /* annotation_argument  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3319 "ds_parser.cpp"
        break;

    case 201: /* annotation_argument_list  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3325 "ds_parser.cpp"
        break;

    case 202: /* annotation_declaration_name  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3331 "ds_parser.cpp"
        break;

    case 203: /* annotation_declaration  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3337 "ds_parser.cpp"
        break;

    case 204: /* annotation_list  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3343 "ds_parser.cpp"
        break;

    case 205: /* optional_annotation_list  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3349 "ds_parser.cpp"
        break;

    case 206: /* optional_function_argument_list  */
#line 113 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3355 "ds_parser.cpp"
        break;

    case 207: /* optional_function_type  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3361 "ds_parser.cpp"
        break;

    case 208: /* function_name  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3367 "ds_parser.cpp"
        break;

    case 210: /* expression_block  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3373 "ds_parser.cpp"
        break;

    case 211: /* expression_any  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3379 "ds_parser.cpp"
        break;

    case 212: /* expressions  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3385 "ds_parser.cpp"
        break;

    case 213: /* expr_pipe  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3391 "ds_parser.cpp"
        break;

    case 214: /* name_in_namespace  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3397 "ds_parser.cpp"
        break;

    case 215: /* expression_delete  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3403 "ds_parser.cpp"
        break;

    case 216: /* expr_new  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3409 "ds_parser.cpp"
        break;

    case 217: /* expression_break  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3415 "ds_parser.cpp"
        break;

    case 218: /* expression_continue  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3421 "ds_parser.cpp"
        break;

    case 219: /* expression_return  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3427 "ds_parser.cpp"
        break;

    case 220: /* expression_yield  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3433 "ds_parser.cpp"
        break;

    case 221: /* expression_try_catch  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3439 "ds_parser.cpp"
        break;

    case 224: /* expression_let  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3445 "ds_parser.cpp"
        break;

    case 225: /* expr_cast  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3451 "ds_parser.cpp"
        break;

    case 232: /* expr_type_info  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3457 "ds_parser.cpp"
        break;

    case 233: /* expr_list  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3463 "ds_parser.cpp"
        break;

    case 234: /* block_or_simple_block  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3469 "ds_parser.cpp"
        break;

    case 236: /* expr_block  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3475 "ds_parser.cpp"
        break;

    case 237: /* expr_numeric_const  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3481 "ds_parser.cpp"
        break;

    case 238: /* expr_assign  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3487 "ds_parser.cpp"
        break;

    case 239: /* expr_assign_pipe  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3493 "ds_parser.cpp"
        break;

    case 240: /* expr_named_call  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3499 "ds_parser.cpp"
        break;

    case 241: /* expr_method_call  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3505 "ds_parser.cpp"
        break;

    case 242: /* func_addr_expr  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3511 "ds_parser.cpp"
        break;

    case 247: /* expr  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3517 "ds_parser.cpp"
        break;

    case 248: /* optional_field_annotation  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3523 "ds_parser.cpp"
        break;

    case 250: /* structure_variable_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3529 "ds_parser.cpp"
        break;

    case 251: /* struct_variable_declaration_list  */
#line 113 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3535 "ds_parser.cpp"
        break;

    case 252: /* function_argument_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3541 "ds_parser.cpp"
        break;

    case 253: /* function_argument_list  */
#line 113 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3547 "ds_parser.cpp"
        break;

    case 254: /* tuple_type  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3553 "ds_parser.cpp"
        break;

    case 255: /* tuple_type_list  */
#line 113 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3559 "ds_parser.cpp"
        break;

    case 256: /* variant_type  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3565 "ds_parser.cpp"
        break;

    case 257: /* variant_type_list  */
#line 113 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3571 "ds_parser.cpp"
        break;

    case 259: /* variable_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3577 "ds_parser.cpp"
        break;

    case 262: /* let_variable_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3583 "ds_parser.cpp"
        break;

    case 263: /* global_variable_declaration_list  */
#line 113 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3589 "ds_parser.cpp"
        break;

    case 267: /* enum_list  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3595 "ds_parser.cpp"
        break;

    case 272: /* optional_structure_parent  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3601 "ds_parser.cpp"
        break;

    case 275: /* variable_name_with_pos_list  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3607 "ds_parser.cpp"
        break;

    case 278: /* structure_type_declaration  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3613 "ds_parser.cpp"
        break;

    case 279: /* auto_type_declaration  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3619 "ds_parser.cpp"
        break;

    case 280: /* bitfield_bits  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3625 "ds_parser.cpp"
        break;

    case 281: /* bitfield_type_declaration  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3631 "ds_parser.cpp"
        break;

    case 284: /* type_declaration  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3637 "ds_parser.cpp"
        break;

    case 313: /* make_decl  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3643 "ds_parser.cpp"
        break;

    case 314: /* make_struct_fields  */
#line 120 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3649 "ds_parser.cpp"
        break;

    case 315: /* make_struct_dim  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3655 "ds_parser.cpp"
        break;

    case 316: /* make_struct_decl  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3661 "ds_parser.cpp"
        break;

    case 317: /* make_tuple  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3667 "ds_parser.cpp"
        break;

    case 318: /* make_map_tuple  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3673 "ds_parser.cpp"
        break;

    case 319: /* make_any_tuple  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3679 "ds_parser.cpp"
        break;

    case 320: /* make_dim  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3685 "ds_parser.cpp"
        break;

    case 321: /* make_dim_decl  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3691 "ds_parser.cpp"
        break;

    case 322: /* make_table  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3697 "ds_parser.cpp"
        break;

    case 323: /* make_table_decl  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3703 "ds_parser.cpp"
        break;

    case 324: /* array_comprehension_where  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3709 "ds_parser.cpp"
        break;

    case 325: /* array_comprehension  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3715 "ds_parser.cpp"
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
#line 413 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 4015 "ds_parser.cpp"
    break;

  case 16:
#line 425 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4021 "ds_parser.cpp"
    break;

  case 17:
#line 426 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4027 "ds_parser.cpp"
    break;

  case 18:
#line 430 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4033 "ds_parser.cpp"
    break;

  case 19:
#line 434 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4043 "ds_parser.cpp"
    break;

  case 20:
#line 439 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4057 "ds_parser.cpp"
    break;

  case 21:
#line 448 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4067 "ds_parser.cpp"
    break;

  case 22:
#line 456 "ds_parser.ypp"
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
#line 4085 "ds_parser.cpp"
    break;

  case 23:
#line 472 "ds_parser.ypp"
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
#line 4109 "ds_parser.cpp"
    break;

  case 25:
#line 498 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4117 "ds_parser.cpp"
    break;

  case 26:
#line 501 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4128 "ds_parser.cpp"
    break;

  case 27:
#line 507 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4139 "ds_parser.cpp"
    break;

  case 28:
#line 516 "ds_parser.ypp"
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
#line 4165 "ds_parser.cpp"
    break;

  case 29:
#line 537 "ds_parser.ypp"
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
#line 4190 "ds_parser.cpp"
    break;

  case 30:
#line 560 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4196 "ds_parser.cpp"
    break;

  case 31:
#line 561 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4202 "ds_parser.cpp"
    break;

  case 35:
#line 574 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4210 "ds_parser.cpp"
    break;

  case 36:
#line 577 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4218 "ds_parser.cpp"
    break;

  case 37:
#line 583 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4226 "ds_parser.cpp"
    break;

  case 38:
#line 589 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4234 "ds_parser.cpp"
    break;

  case 39:
#line 592 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4242 "ds_parser.cpp"
    break;

  case 40:
#line 598 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4248 "ds_parser.cpp"
    break;

  case 41:
#line 599 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4254 "ds_parser.cpp"
    break;

  case 42:
#line 603 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4260 "ds_parser.cpp"
    break;

  case 43:
#line 604 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4266 "ds_parser.cpp"
    break;

  case 44:
#line 605 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4277 "ds_parser.cpp"
    break;

  case 45:
#line 614 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4283 "ds_parser.cpp"
    break;

  case 46:
#line 615 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4289 "ds_parser.cpp"
    break;

  case 47:
#line 619 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4300 "ds_parser.cpp"
    break;

  case 48:
#line 628 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
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
#line 4317 "ds_parser.cpp"
    break;

  case 49:
#line 643 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4329 "ds_parser.cpp"
    break;

  case 50:
#line 653 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4340 "ds_parser.cpp"
    break;

  case 51:
#line 662 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4346 "ds_parser.cpp"
    break;

  case 52:
#line 663 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4352 "ds_parser.cpp"
    break;

  case 53:
#line 664 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4358 "ds_parser.cpp"
    break;

  case 54:
#line 665 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4364 "ds_parser.cpp"
    break;

  case 55:
#line 666 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4370 "ds_parser.cpp"
    break;

  case 56:
#line 667 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4376 "ds_parser.cpp"
    break;

  case 57:
#line 671 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4386 "ds_parser.cpp"
    break;

  case 58:
#line 676 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4396 "ds_parser.cpp"
    break;

  case 59:
#line 684 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4402 "ds_parser.cpp"
    break;

  case 60:
#line 685 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4408 "ds_parser.cpp"
    break;

  case 61:
#line 686 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4414 "ds_parser.cpp"
    break;

  case 62:
#line 687 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4420 "ds_parser.cpp"
    break;

  case 63:
#line 688 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4426 "ds_parser.cpp"
    break;

  case 64:
#line 689 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4432 "ds_parser.cpp"
    break;

  case 65:
#line 690 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4438 "ds_parser.cpp"
    break;

  case 66:
#line 691 "ds_parser.ypp"
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4446 "ds_parser.cpp"
    break;

  case 67:
#line 697 "ds_parser.ypp"
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
#line 4464 "ds_parser.cpp"
    break;

  case 68:
#line 710 "ds_parser.ypp"
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
#line 4482 "ds_parser.cpp"
    break;

  case 69:
#line 726 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4488 "ds_parser.cpp"
    break;

  case 70:
#line 727 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4494 "ds_parser.cpp"
    break;

  case 71:
#line 731 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4506 "ds_parser.cpp"
    break;

  case 72:
#line 738 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4520 "ds_parser.cpp"
    break;

  case 73:
#line 750 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4529 "ds_parser.cpp"
    break;

  case 74:
#line 754 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4538 "ds_parser.cpp"
    break;

  case 75:
#line 761 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4544 "ds_parser.cpp"
    break;

  case 76:
#line 762 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4550 "ds_parser.cpp"
    break;

  case 77:
#line 766 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4556 "ds_parser.cpp"
    break;

  case 78:
#line 767 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4562 "ds_parser.cpp"
    break;

  case 79:
#line 768 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4568 "ds_parser.cpp"
    break;

  case 80:
#line 772 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4578 "ds_parser.cpp"
    break;

  case 81:
#line 777 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4587 "ds_parser.cpp"
    break;

  case 82:
#line 784 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s);
	}
#line 4596 "ds_parser.cpp"
    break;

  case 83:
#line 788 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4602 "ds_parser.cpp"
    break;

  case 84:
#line 789 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4608 "ds_parser.cpp"
    break;

  case 85:
#line 790 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4614 "ds_parser.cpp"
    break;

  case 86:
#line 791 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4620 "ds_parser.cpp"
    break;

  case 87:
#line 792 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4626 "ds_parser.cpp"
    break;

  case 88:
#line 793 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4632 "ds_parser.cpp"
    break;

  case 89:
#line 794 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4638 "ds_parser.cpp"
    break;

  case 90:
#line 795 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4644 "ds_parser.cpp"
    break;

  case 91:
#line 796 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4650 "ds_parser.cpp"
    break;

  case 92:
#line 797 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4656 "ds_parser.cpp"
    break;

  case 93:
#line 798 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4662 "ds_parser.cpp"
    break;

  case 94:
#line 799 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4668 "ds_parser.cpp"
    break;

  case 95:
#line 800 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4674 "ds_parser.cpp"
    break;

  case 96:
#line 801 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4680 "ds_parser.cpp"
    break;

  case 97:
#line 802 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4686 "ds_parser.cpp"
    break;

  case 98:
#line 803 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4692 "ds_parser.cpp"
    break;

  case 99:
#line 804 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4698 "ds_parser.cpp"
    break;

  case 100:
#line 805 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4704 "ds_parser.cpp"
    break;

  case 101:
#line 806 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4710 "ds_parser.cpp"
    break;

  case 102:
#line 807 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4716 "ds_parser.cpp"
    break;

  case 103:
#line 808 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4722 "ds_parser.cpp"
    break;

  case 104:
#line 809 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4728 "ds_parser.cpp"
    break;

  case 105:
#line 810 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4734 "ds_parser.cpp"
    break;

  case 106:
#line 811 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4740 "ds_parser.cpp"
    break;

  case 107:
#line 812 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4746 "ds_parser.cpp"
    break;

  case 108:
#line 813 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4752 "ds_parser.cpp"
    break;

  case 109:
#line 814 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4758 "ds_parser.cpp"
    break;

  case 110:
#line 815 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4764 "ds_parser.cpp"
    break;

  case 111:
#line 816 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4770 "ds_parser.cpp"
    break;

  case 112:
#line 817 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4776 "ds_parser.cpp"
    break;

  case 113:
#line 818 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4782 "ds_parser.cpp"
    break;

  case 114:
#line 819 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4788 "ds_parser.cpp"
    break;

  case 115:
#line 820 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4794 "ds_parser.cpp"
    break;

  case 116:
#line 821 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4800 "ds_parser.cpp"
    break;

  case 117:
#line 822 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4806 "ds_parser.cpp"
    break;

  case 118:
#line 823 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4812 "ds_parser.cpp"
    break;

  case 119:
#line 828 "ds_parser.ypp"
    {
        auto pFunction = make_smart<Function>();
        pFunction->at = tokAt((yylsp[-3]));
        pFunction->atDecl = tokRangeAt((yylsp[-4]), (yylsp[0]));
        /*
        TextPrinter tp;
        tp  << *$name << " at " << pFunction->at.describe(true) << "\n"
            << " range: " << pFunction->atDecl.describe(true) << "\n"
            << " from:" << tokAt(@from).describe(true) << "\n"
            << " block: " << tokAt(@block).describe(true) << "\n";
        */
        pFunction->name = *(yyvsp[-3].s);
        pFunction->body = ExpressionPtr((yyvsp[0].pExpression));
        pFunction->result = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
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
#line 4894 "ds_parser.cpp"
    break;

  case 120:
#line 908 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4902 "ds_parser.cpp"
    break;

  case 121:
#line 911 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4914 "ds_parser.cpp"
    break;

  case 122:
#line 921 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4920 "ds_parser.cpp"
    break;

  case 123:
#line 922 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4926 "ds_parser.cpp"
    break;

  case 124:
#line 923 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4932 "ds_parser.cpp"
    break;

  case 125:
#line 924 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4938 "ds_parser.cpp"
    break;

  case 126:
#line 925 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4944 "ds_parser.cpp"
    break;

  case 127:
#line 926 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4950 "ds_parser.cpp"
    break;

  case 128:
#line 927 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4956 "ds_parser.cpp"
    break;

  case 129:
#line 928 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4962 "ds_parser.cpp"
    break;

  case 130:
#line 929 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4968 "ds_parser.cpp"
    break;

  case 131:
#line 930 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4974 "ds_parser.cpp"
    break;

  case 132:
#line 931 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4980 "ds_parser.cpp"
    break;

  case 133:
#line 932 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4986 "ds_parser.cpp"
    break;

  case 134:
#line 933 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4992 "ds_parser.cpp"
    break;

  case 135:
#line 934 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4998 "ds_parser.cpp"
    break;

  case 136:
#line 935 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5004 "ds_parser.cpp"
    break;

  case 137:
#line 936 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5010 "ds_parser.cpp"
    break;

  case 138:
#line 937 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5016 "ds_parser.cpp"
    break;

  case 139:
#line 938 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5022 "ds_parser.cpp"
    break;

  case 140:
#line 942 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5032 "ds_parser.cpp"
    break;

  case 141:
#line 947 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5043 "ds_parser.cpp"
    break;

  case 142:
#line 953 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5051 "ds_parser.cpp"
    break;

  case 143:
#line 959 "ds_parser.ypp"
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
#line 5067 "ds_parser.cpp"
    break;

  case 144:
#line 970 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5075 "ds_parser.cpp"
    break;

  case 145:
#line 976 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5081 "ds_parser.cpp"
    break;

  case 146:
#line 977 "ds_parser.ypp"
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
#line 5097 "ds_parser.cpp"
    break;

  case 147:
#line 988 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5103 "ds_parser.cpp"
    break;

  case 148:
#line 992 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5111 "ds_parser.cpp"
    break;

  case 149:
#line 998 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5119 "ds_parser.cpp"
    break;

  case 150:
#line 1001 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5127 "ds_parser.cpp"
    break;

  case 151:
#line 1004 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5136 "ds_parser.cpp"
    break;

  case 152:
#line 1008 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5144 "ds_parser.cpp"
    break;

  case 153:
#line 1014 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5150 "ds_parser.cpp"
    break;

  case 154:
#line 1018 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5156 "ds_parser.cpp"
    break;

  case 155:
#line 1022 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5164 "ds_parser.cpp"
    break;

  case 156:
#line 1025 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5172 "ds_parser.cpp"
    break;

  case 157:
#line 1028 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5182 "ds_parser.cpp"
    break;

  case 158:
#line 1033 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5190 "ds_parser.cpp"
    break;

  case 159:
#line 1036 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5200 "ds_parser.cpp"
    break;

  case 160:
#line 1044 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5208 "ds_parser.cpp"
    break;

  case 161:
#line 1047 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5218 "ds_parser.cpp"
    break;

  case 162:
#line 1052 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5226 "ds_parser.cpp"
    break;

  case 163:
#line 1055 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5236 "ds_parser.cpp"
    break;

  case 164:
#line 1063 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5244 "ds_parser.cpp"
    break;

  case 165:
#line 1069 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5250 "ds_parser.cpp"
    break;

  case 166:
#line 1070 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5256 "ds_parser.cpp"
    break;

  case 167:
#line 1074 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5262 "ds_parser.cpp"
    break;

  case 168:
#line 1075 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5268 "ds_parser.cpp"
    break;

  case 169:
#line 1079 "ds_parser.ypp"
    {
        auto pLet = new ExprLet();
        pLet->at = tokAt((yylsp[-2]));
        pLet->inScope = (yyvsp[-1].b);
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
                    if ( (yyvsp[-2].b) ) {
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
#line 5304 "ds_parser.cpp"
    break;

  case 170:
#line 1113 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5310 "ds_parser.cpp"
    break;

  case 171:
#line 1113 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5316 "ds_parser.cpp"
    break;

  case 172:
#line 1113 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5324 "ds_parser.cpp"
    break;

  case 173:
#line 1116 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5330 "ds_parser.cpp"
    break;

  case 174:
#line 1116 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5336 "ds_parser.cpp"
    break;

  case 175:
#line 1116 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5346 "ds_parser.cpp"
    break;

  case 176:
#line 1121 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5352 "ds_parser.cpp"
    break;

  case 177:
#line 1121 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5358 "ds_parser.cpp"
    break;

  case 178:
#line 1121 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5368 "ds_parser.cpp"
    break;

  case 179:
#line 1129 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5377 "ds_parser.cpp"
    break;

  case 180:
#line 1133 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5387 "ds_parser.cpp"
    break;

  case 181:
#line 1138 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5398 "ds_parser.cpp"
    break;

  case 182:
#line 1144 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 5407 "ds_parser.cpp"
    break;

  case 183:
#line 1148 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5417 "ds_parser.cpp"
    break;

  case 184:
#line 1153 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5428 "ds_parser.cpp"
    break;

  case 185:
#line 1162 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5436 "ds_parser.cpp"
    break;

  case 186:
#line 1165 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5444 "ds_parser.cpp"
    break;

  case 187:
#line 1171 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5450 "ds_parser.cpp"
    break;

  case 188:
#line 1172 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5462 "ds_parser.cpp"
    break;

  case 189:
#line 1179 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5475 "ds_parser.cpp"
    break;

  case 190:
#line 1190 "ds_parser.ypp"
    { (yyval.b) = false;   /* block */  }
#line 5481 "ds_parser.cpp"
    break;

  case 191:
#line 1191 "ds_parser.ypp"
    { (yyval.b) = true;    /* lambda */ }
#line 5487 "ds_parser.cpp"
    break;

  case 192:
#line 1195 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5497 "ds_parser.cpp"
    break;

  case 193:
#line 1201 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)), (yyvsp[-4].b));
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
#line 5553 "ds_parser.cpp"
    break;

  case 194:
#line 1255 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5559 "ds_parser.cpp"
    break;

  case 195:
#line 1256 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5565 "ds_parser.cpp"
    break;

  case 196:
#line 1257 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5571 "ds_parser.cpp"
    break;

  case 197:
#line 1258 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5577 "ds_parser.cpp"
    break;

  case 198:
#line 1259 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5583 "ds_parser.cpp"
    break;

  case 199:
#line 1260 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5589 "ds_parser.cpp"
    break;

  case 200:
#line 1264 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5595 "ds_parser.cpp"
    break;

  case 201:
#line 1265 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5601 "ds_parser.cpp"
    break;

  case 202:
#line 1266 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5607 "ds_parser.cpp"
    break;

  case 203:
#line 1267 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5613 "ds_parser.cpp"
    break;

  case 204:
#line 1268 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5619 "ds_parser.cpp"
    break;

  case 205:
#line 1269 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5625 "ds_parser.cpp"
    break;

  case 206:
#line 1270 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5631 "ds_parser.cpp"
    break;

  case 207:
#line 1271 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5637 "ds_parser.cpp"
    break;

  case 208:
#line 1272 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5643 "ds_parser.cpp"
    break;

  case 209:
#line 1273 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5649 "ds_parser.cpp"
    break;

  case 210:
#line 1274 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5655 "ds_parser.cpp"
    break;

  case 211:
#line 1275 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5661 "ds_parser.cpp"
    break;

  case 212:
#line 1276 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5667 "ds_parser.cpp"
    break;

  case 213:
#line 1277 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5673 "ds_parser.cpp"
    break;

  case 214:
#line 1278 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5679 "ds_parser.cpp"
    break;

  case 215:
#line 1279 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5685 "ds_parser.cpp"
    break;

  case 216:
#line 1283 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5691 "ds_parser.cpp"
    break;

  case 217:
#line 1284 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5697 "ds_parser.cpp"
    break;

  case 218:
#line 1288 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5709 "ds_parser.cpp"
    break;

  case 219:
#line 1299 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5719 "ds_parser.cpp"
    break;

  case 220:
#line 1304 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5731 "ds_parser.cpp"
    break;

  case 221:
#line 1314 "ds_parser.ypp"
    {
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
	}
#line 5740 "ds_parser.cpp"
    break;

  case 222:
#line 1318 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5746 "ds_parser.cpp"
    break;

  case 223:
#line 1318 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5752 "ds_parser.cpp"
    break;

  case 224:
#line 1318 "ds_parser.ypp"
    {
		auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5764 "ds_parser.cpp"
    break;

  case 225:
#line 1325 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5770 "ds_parser.cpp"
    break;

  case 226:
#line 1325 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5776 "ds_parser.cpp"
    break;

  case 227:
#line 1325 "ds_parser.ypp"
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
#line 5792 "ds_parser.cpp"
    break;

  case 228:
#line 1339 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5798 "ds_parser.cpp"
    break;

  case 229:
#line 1340 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5804 "ds_parser.cpp"
    break;

  case 230:
#line 1341 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5810 "ds_parser.cpp"
    break;

  case 231:
#line 1342 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5816 "ds_parser.cpp"
    break;

  case 232:
#line 1343 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5822 "ds_parser.cpp"
    break;

  case 233:
#line 1344 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5828 "ds_parser.cpp"
    break;

  case 234:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5834 "ds_parser.cpp"
    break;

  case 235:
#line 1346 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5840 "ds_parser.cpp"
    break;

  case 236:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5846 "ds_parser.cpp"
    break;

  case 237:
#line 1348 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5852 "ds_parser.cpp"
    break;

  case 238:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5858 "ds_parser.cpp"
    break;

  case 239:
#line 1350 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5864 "ds_parser.cpp"
    break;

  case 240:
#line 1351 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5870 "ds_parser.cpp"
    break;

  case 241:
#line 1352 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5876 "ds_parser.cpp"
    break;

  case 242:
#line 1353 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5882 "ds_parser.cpp"
    break;

  case 243:
#line 1354 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5888 "ds_parser.cpp"
    break;

  case 244:
#line 1355 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5894 "ds_parser.cpp"
    break;

  case 245:
#line 1356 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5900 "ds_parser.cpp"
    break;

  case 246:
#line 1357 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5906 "ds_parser.cpp"
    break;

  case 247:
#line 1358 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5912 "ds_parser.cpp"
    break;

  case 248:
#line 1359 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5918 "ds_parser.cpp"
    break;

  case 249:
#line 1360 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5924 "ds_parser.cpp"
    break;

  case 250:
#line 1361 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5930 "ds_parser.cpp"
    break;

  case 251:
#line 1362 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5936 "ds_parser.cpp"
    break;

  case 252:
#line 1363 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5942 "ds_parser.cpp"
    break;

  case 253:
#line 1364 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5948 "ds_parser.cpp"
    break;

  case 254:
#line 1365 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5954 "ds_parser.cpp"
    break;

  case 255:
#line 1366 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5960 "ds_parser.cpp"
    break;

  case 256:
#line 1367 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5966 "ds_parser.cpp"
    break;

  case 257:
#line 1368 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5972 "ds_parser.cpp"
    break;

  case 258:
#line 1369 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5978 "ds_parser.cpp"
    break;

  case 259:
#line 1370 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5984 "ds_parser.cpp"
    break;

  case 260:
#line 1371 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5990 "ds_parser.cpp"
    break;

  case 261:
#line 1372 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5996 "ds_parser.cpp"
    break;

  case 262:
#line 1373 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6002 "ds_parser.cpp"
    break;

  case 263:
#line 1374 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6008 "ds_parser.cpp"
    break;

  case 264:
#line 1375 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6014 "ds_parser.cpp"
    break;

  case 265:
#line 1376 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6020 "ds_parser.cpp"
    break;

  case 266:
#line 1377 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6026 "ds_parser.cpp"
    break;

  case 267:
#line 1378 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6035 "ds_parser.cpp"
    break;

  case 268:
#line 1382 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6044 "ds_parser.cpp"
    break;

  case 269:
#line 1386 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6052 "ds_parser.cpp"
    break;

  case 270:
#line 1389 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6060 "ds_parser.cpp"
    break;

  case 271:
#line 1392 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6066 "ds_parser.cpp"
    break;

  case 272:
#line 1393 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6072 "ds_parser.cpp"
    break;

  case 273:
#line 1394 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6078 "ds_parser.cpp"
    break;

  case 274:
#line 1395 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6088 "ds_parser.cpp"
    break;

  case 275:
#line 1400 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6098 "ds_parser.cpp"
    break;

  case 276:
#line 1405 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6104 "ds_parser.cpp"
    break;

  case 277:
#line 1406 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6112 "ds_parser.cpp"
    break;

  case 278:
#line 1409 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 6120 "ds_parser.cpp"
    break;

  case 279:
#line 1412 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 6129 "ds_parser.cpp"
    break;

  case 280:
#line 1416 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 6138 "ds_parser.cpp"
    break;

  case 281:
#line 1420 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6147 "ds_parser.cpp"
    break;

  case 282:
#line 1424 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6153 "ds_parser.cpp"
    break;

  case 283:
#line 1425 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6159 "ds_parser.cpp"
    break;

  case 284:
#line 1426 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6165 "ds_parser.cpp"
    break;

  case 285:
#line 1427 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6171 "ds_parser.cpp"
    break;

  case 286:
#line 1428 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6177 "ds_parser.cpp"
    break;

  case 287:
#line 1429 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6183 "ds_parser.cpp"
    break;

  case 288:
#line 1430 "ds_parser.ypp"
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
#line 6198 "ds_parser.cpp"
    break;

  case 289:
#line 1440 "ds_parser.ypp"
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
#line 6213 "ds_parser.cpp"
    break;

  case 290:
#line 1450 "ds_parser.ypp"
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
			(yyval.pExpression) = new ExprConstInt(0);	// dummy
		}
        delete (yyvsp[-1].s);
        delete (yyvsp[0].s);
    }
#line 6270 "ds_parser.cpp"
    break;

  case 291:
#line 1505 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6276 "ds_parser.cpp"
    break;

  case 292:
#line 1506 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 6282 "ds_parser.cpp"
    break;

  case 293:
#line 1510 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6288 "ds_parser.cpp"
    break;

  case 294:
#line 1511 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6294 "ds_parser.cpp"
    break;

  case 295:
#line 1515 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6304 "ds_parser.cpp"
    break;

  case 296:
#line 1523 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6312 "ds_parser.cpp"
    break;

  case 297:
#line 1526 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6321 "ds_parser.cpp"
    break;

  case 298:
#line 1533 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
		}
#line 6331 "ds_parser.cpp"
    break;

  case 299:
#line 1538 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
		}
#line 6341 "ds_parser.cpp"
    break;

  case 300:
#line 1546 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6347 "ds_parser.cpp"
    break;

  case 301:
#line 1547 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6353 "ds_parser.cpp"
    break;

  case 302:
#line 1551 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6361 "ds_parser.cpp"
    break;

  case 303:
#line 1554 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6372 "ds_parser.cpp"
    break;

  case 304:
#line 1563 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6378 "ds_parser.cpp"
    break;

  case 305:
#line 1564 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6384 "ds_parser.cpp"
    break;

  case 306:
#line 1568 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6395 "ds_parser.cpp"
    break;

  case 307:
#line 1577 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6401 "ds_parser.cpp"
    break;

  case 308:
#line 1578 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6407 "ds_parser.cpp"
    break;

  case 309:
#line 1583 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6413 "ds_parser.cpp"
    break;

  case 310:
#line 1584 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6419 "ds_parser.cpp"
    break;

  case 311:
#line 1588 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 6430 "ds_parser.cpp"
    break;

  case 312:
#line 1594 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 6441 "ds_parser.cpp"
    break;

  case 313:
#line 1600 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6449 "ds_parser.cpp"
    break;

  case 314:
#line 1603 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6458 "ds_parser.cpp"
    break;

  case 315:
#line 1607 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6469 "ds_parser.cpp"
    break;

  case 316:
#line 1613 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6480 "ds_parser.cpp"
    break;

  case 317:
#line 1622 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6486 "ds_parser.cpp"
    break;

  case 318:
#line 1623 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6492 "ds_parser.cpp"
    break;

  case 319:
#line 1624 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6498 "ds_parser.cpp"
    break;

  case 320:
#line 1628 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6504 "ds_parser.cpp"
    break;

  case 321:
#line 1629 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6510 "ds_parser.cpp"
    break;

  case 322:
#line 1633 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6518 "ds_parser.cpp"
    break;

  case 323:
#line 1636 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6528 "ds_parser.cpp"
    break;

  case 324:
#line 1641 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6538 "ds_parser.cpp"
    break;

  case 325:
#line 1646 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6551 "ds_parser.cpp"
    break;

  case 326:
#line 1654 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6564 "ds_parser.cpp"
    break;

  case 327:
#line 1665 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6572 "ds_parser.cpp"
    break;

  case 328:
#line 1668 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6581 "ds_parser.cpp"
    break;

  case 329:
#line 1675 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6587 "ds_parser.cpp"
    break;

  case 330:
#line 1676 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6593 "ds_parser.cpp"
    break;

  case 331:
#line 1680 "ds_parser.ypp"
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
                    if ( (yyvsp[-4].b) ) {
                        pVar->type->constant = true;
                    } else {
                        pVar->type->removeConstant = true;
                    }
                    pVar->global_shared = (yyvsp[-3].b);
                    if ( !g_Program->addVariable(pVar) )
                        das_yyerror("global variable is already declared " + name_at.first,name_at.second,
                            CompilationError::global_variable_already_declared);
                }
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6625 "ds_parser.cpp"
    break;

  case 332:
#line 1707 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 6634 "ds_parser.cpp"
    break;

  case 333:
#line 1710 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 6645 "ds_parser.cpp"
    break;

  case 334:
#line 1719 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6653 "ds_parser.cpp"
    break;

  case 335:
#line 1722 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6667 "ds_parser.cpp"
    break;

  case 336:
#line 1731 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6681 "ds_parser.cpp"
    break;

  case 337:
#line 1744 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 6695 "ds_parser.cpp"
    break;

  case 341:
#line 1765 "ds_parser.ypp"
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
#line 6711 "ds_parser.cpp"
    break;

  case 342:
#line 1776 "ds_parser.ypp"
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
#line 6728 "ds_parser.cpp"
    break;

  case 343:
#line 1791 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 6734 "ds_parser.cpp"
    break;

  case 344:
#line 1792 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 6740 "ds_parser.cpp"
    break;

  case 345:
#line 1796 "ds_parser.ypp"
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
#line 6781 "ds_parser.cpp"
    break;

  case 346:
#line 1835 "ds_parser.ypp"
    {
        if ( (yyvsp[-3].pStructure) ) {
            auto pStruct = (yyvsp[-3].pStructure);
            pStruct->at = tokAt((yylsp[-4]));
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
#line 6849 "ds_parser.cpp"
    break;

  case 347:
#line 1901 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6861 "ds_parser.cpp"
    break;

  case 348:
#line 1908 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 6872 "ds_parser.cpp"
    break;

  case 349:
#line 1917 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 6878 "ds_parser.cpp"
    break;

  case 350:
#line 1918 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 6884 "ds_parser.cpp"
    break;

  case 351:
#line 1919 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6890 "ds_parser.cpp"
    break;

  case 352:
#line 1920 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6896 "ds_parser.cpp"
    break;

  case 353:
#line 1921 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6902 "ds_parser.cpp"
    break;

  case 354:
#line 1922 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 6908 "ds_parser.cpp"
    break;

  case 355:
#line 1923 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 6914 "ds_parser.cpp"
    break;

  case 356:
#line 1924 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 6920 "ds_parser.cpp"
    break;

  case 357:
#line 1925 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 6926 "ds_parser.cpp"
    break;

  case 358:
#line 1926 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6932 "ds_parser.cpp"
    break;

  case 359:
#line 1927 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6938 "ds_parser.cpp"
    break;

  case 360:
#line 1928 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6944 "ds_parser.cpp"
    break;

  case 361:
#line 1929 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 6950 "ds_parser.cpp"
    break;

  case 362:
#line 1930 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 6956 "ds_parser.cpp"
    break;

  case 363:
#line 1931 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 6962 "ds_parser.cpp"
    break;

  case 364:
#line 1932 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 6968 "ds_parser.cpp"
    break;

  case 365:
#line 1933 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 6974 "ds_parser.cpp"
    break;

  case 366:
#line 1934 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 6980 "ds_parser.cpp"
    break;

  case 367:
#line 1935 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 6986 "ds_parser.cpp"
    break;

  case 368:
#line 1936 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 6992 "ds_parser.cpp"
    break;

  case 369:
#line 1937 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 6998 "ds_parser.cpp"
    break;

  case 370:
#line 1938 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7004 "ds_parser.cpp"
    break;

  case 371:
#line 1939 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7010 "ds_parser.cpp"
    break;

  case 372:
#line 1940 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7016 "ds_parser.cpp"
    break;

  case 373:
#line 1941 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7022 "ds_parser.cpp"
    break;

  case 374:
#line 1945 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7028 "ds_parser.cpp"
    break;

  case 375:
#line 1946 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7034 "ds_parser.cpp"
    break;

  case 376:
#line 1947 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7040 "ds_parser.cpp"
    break;

  case 377:
#line 1948 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7046 "ds_parser.cpp"
    break;

  case 378:
#line 1949 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7052 "ds_parser.cpp"
    break;

  case 379:
#line 1950 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7058 "ds_parser.cpp"
    break;

  case 380:
#line 1954 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7071 "ds_parser.cpp"
    break;

  case 381:
#line 1965 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7080 "ds_parser.cpp"
    break;

  case 382:
#line 1969 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7092 "ds_parser.cpp"
    break;

  case 383:
#line 1979 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7104 "ds_parser.cpp"
    break;

  case 384:
#line 1986 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7115 "ds_parser.cpp"
    break;

  case 385:
#line 1995 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7121 "ds_parser.cpp"
    break;

  case 386:
#line 1995 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7127 "ds_parser.cpp"
    break;

  case 387:
#line 1995 "ds_parser.ypp"
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
#line 7142 "ds_parser.cpp"
    break;

  case 388:
#line 2008 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7148 "ds_parser.cpp"
    break;

  case 389:
#line 2009 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7154 "ds_parser.cpp"
    break;

  case 390:
#line 2010 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7160 "ds_parser.cpp"
    break;

  case 391:
#line 2011 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7166 "ds_parser.cpp"
    break;

  case 392:
#line 2012 "ds_parser.ypp"
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
#line 7185 "ds_parser.cpp"
    break;

  case 393:
#line 2026 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7196 "ds_parser.cpp"
    break;

  case 394:
#line 2032 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7205 "ds_parser.cpp"
    break;

  case 395:
#line 2036 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7215 "ds_parser.cpp"
    break;

  case 396:
#line 2041 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7225 "ds_parser.cpp"
    break;

  case 397:
#line 2046 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7235 "ds_parser.cpp"
    break;

  case 398:
#line 2051 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7247 "ds_parser.cpp"
    break;

  case 399:
#line 2058 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7257 "ds_parser.cpp"
    break;

  case 400:
#line 2063 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7266 "ds_parser.cpp"
    break;

  case 401:
#line 2067 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7275 "ds_parser.cpp"
    break;

  case 402:
#line 2071 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7285 "ds_parser.cpp"
    break;

  case 403:
#line 2076 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7294 "ds_parser.cpp"
    break;

  case 404:
#line 2080 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7304 "ds_parser.cpp"
    break;

  case 405:
#line 2085 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7310 "ds_parser.cpp"
    break;

  case 406:
#line 2085 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7316 "ds_parser.cpp"
    break;

  case 407:
#line 2085 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7327 "ds_parser.cpp"
    break;

  case 408:
#line 2091 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7339 "ds_parser.cpp"
    break;

  case 409:
#line 2098 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7345 "ds_parser.cpp"
    break;

  case 410:
#line 2098 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7351 "ds_parser.cpp"
    break;

  case 411:
#line 2098 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7361 "ds_parser.cpp"
    break;

  case 412:
#line 2103 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7367 "ds_parser.cpp"
    break;

  case 413:
#line 2103 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7373 "ds_parser.cpp"
    break;

  case 414:
#line 2103 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7384 "ds_parser.cpp"
    break;

  case 415:
#line 2109 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7390 "ds_parser.cpp"
    break;

  case 416:
#line 2109 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7396 "ds_parser.cpp"
    break;

  case 417:
#line 2109 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7406 "ds_parser.cpp"
    break;

  case 418:
#line 2114 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7415 "ds_parser.cpp"
    break;

  case 419:
#line 2118 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7421 "ds_parser.cpp"
    break;

  case 420:
#line 2118 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7427 "ds_parser.cpp"
    break;

  case 421:
#line 2118 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7437 "ds_parser.cpp"
    break;

  case 422:
#line 2123 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7443 "ds_parser.cpp"
    break;

  case 423:
#line 2123 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7449 "ds_parser.cpp"
    break;

  case 424:
#line 2123 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7463 "ds_parser.cpp"
    break;

  case 425:
#line 2132 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7472 "ds_parser.cpp"
    break;

  case 426:
#line 2136 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7478 "ds_parser.cpp"
    break;

  case 427:
#line 2136 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7484 "ds_parser.cpp"
    break;

  case 428:
#line 2136 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7494 "ds_parser.cpp"
    break;

  case 429:
#line 2141 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7500 "ds_parser.cpp"
    break;

  case 430:
#line 2141 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7506 "ds_parser.cpp"
    break;

  case 431:
#line 2141 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7520 "ds_parser.cpp"
    break;

  case 432:
#line 2150 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7529 "ds_parser.cpp"
    break;

  case 433:
#line 2154 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7535 "ds_parser.cpp"
    break;

  case 434:
#line 2154 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7541 "ds_parser.cpp"
    break;

  case 435:
#line 2154 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7551 "ds_parser.cpp"
    break;

  case 436:
#line 2159 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7557 "ds_parser.cpp"
    break;

  case 437:
#line 2159 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7563 "ds_parser.cpp"
    break;

  case 438:
#line 2159 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7577 "ds_parser.cpp"
    break;

  case 439:
#line 2168 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7583 "ds_parser.cpp"
    break;

  case 440:
#line 2168 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7589 "ds_parser.cpp"
    break;

  case 441:
#line 2168 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 7600 "ds_parser.cpp"
    break;

  case 442:
#line 2174 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7606 "ds_parser.cpp"
    break;

  case 443:
#line 2174 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7612 "ds_parser.cpp"
    break;

  case 444:
#line 2174 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 7623 "ds_parser.cpp"
    break;

  case 445:
#line 2183 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7629 "ds_parser.cpp"
    break;

  case 446:
#line 2183 "ds_parser.ypp"
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
#line 7646 "ds_parser.cpp"
    break;

  case 447:
#line 2198 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7652 "ds_parser.cpp"
    break;

  case 448:
#line 2198 "ds_parser.ypp"
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
#line 7673 "ds_parser.cpp"
    break;

  case 449:
#line 2218 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7679 "ds_parser.cpp"
    break;

  case 450:
#line 2219 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7685 "ds_parser.cpp"
    break;

  case 451:
#line 2220 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7691 "ds_parser.cpp"
    break;

  case 452:
#line 2221 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7697 "ds_parser.cpp"
    break;

  case 453:
#line 2225 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7709 "ds_parser.cpp"
    break;

  case 454:
#line 2232 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7720 "ds_parser.cpp"
    break;

  case 455:
#line 2241 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 7730 "ds_parser.cpp"
    break;

  case 456:
#line 2246 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7739 "ds_parser.cpp"
    break;

  case 457:
#line 2253 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7749 "ds_parser.cpp"
    break;

  case 458:
#line 2258 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7760 "ds_parser.cpp"
    break;

  case 459:
#line 2264 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7771 "ds_parser.cpp"
    break;

  case 460:
#line 2270 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7783 "ds_parser.cpp"
    break;

  case 461:
#line 2277 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7796 "ds_parser.cpp"
    break;

  case 462:
#line 2288 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7804 "ds_parser.cpp"
    break;

  case 463:
#line 2291 "ds_parser.ypp"
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
#line 7820 "ds_parser.cpp"
    break;

  case 464:
#line 2305 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7831 "ds_parser.cpp"
    break;

  case 465:
#line 2314 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7837 "ds_parser.cpp"
    break;

  case 466:
#line 2315 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7843 "ds_parser.cpp"
    break;

  case 467:
#line 2319 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7853 "ds_parser.cpp"
    break;

  case 468:
#line 2324 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7862 "ds_parser.cpp"
    break;

  case 469:
#line 2331 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7872 "ds_parser.cpp"
    break;

  case 470:
#line 2336 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7885 "ds_parser.cpp"
    break;

  case 471:
#line 2347 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7895 "ds_parser.cpp"
    break;

  case 472:
#line 2352 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7904 "ds_parser.cpp"
    break;

  case 473:
#line 2359 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7918 "ds_parser.cpp"
    break;

  case 474:
#line 2371 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 7924 "ds_parser.cpp"
    break;

  case 475:
#line 2372 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7930 "ds_parser.cpp"
    break;

  case 476:
#line 2376 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
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
#line 7952 "ds_parser.cpp"
    break;

  case 477:
#line 2393 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
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
#line 7974 "ds_parser.cpp"
    break;


#line 7978 "ds_parser.cpp"

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
#line 2412 "ds_parser.ypp"


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


