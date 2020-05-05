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
#define YYLAST   7159

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  174
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  117
/* YYNRULES -- Number of rules.  */
#define YYNRULES  442
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  813

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
       0,   398,   398,   399,   400,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   414,   426,   427,   431,   435,
     440,   449,   457,   473,   495,   499,   502,   508,   517,   538,
     561,   562,   566,   570,   571,   575,   578,   584,   590,   593,
     599,   600,   604,   605,   606,   615,   616,   620,   629,   644,
     654,   663,   664,   665,   666,   667,   668,   672,   677,   685,
     686,   687,   688,   689,   690,   691,   692,   698,   711,   727,
     728,   732,   739,   751,   755,   762,   763,   767,   768,   769,
     773,   778,   785,   789,   790,   791,   792,   793,   794,   795,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,   810,   811,   812,   813,   814,   815,
     816,   817,   818,   819,   820,   821,   822,   823,   824,   828,
     909,   912,   922,   923,   924,   925,   926,   927,   928,   929,
     930,   931,   932,   933,   934,   935,   936,   937,   938,   939,
     943,   948,   954,   960,   971,   977,   978,   989,   993,   999,
    1002,  1005,  1009,  1015,  1019,  1023,  1026,  1029,  1034,  1037,
    1045,  1048,  1053,  1056,  1064,  1070,  1071,  1075,  1076,  1080,
    1114,  1117,  1122,  1130,  1134,  1139,  1145,  1149,  1154,  1163,
    1166,  1172,  1173,  1180,  1191,  1192,  1196,  1201,  1256,  1257,
    1258,  1259,  1260,  1261,  1265,  1266,  1267,  1268,  1269,  1270,
    1271,  1272,  1273,  1274,  1275,  1276,  1277,  1278,  1279,  1280,
    1284,  1285,  1289,  1300,  1305,  1315,  1319,  1326,  1340,  1341,
    1342,  1343,  1344,  1345,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,  1353,  1354,  1355,  1356,  1357,  1358,  1359,  1360,  1361,
    1362,  1363,  1364,  1365,  1366,  1367,  1368,  1369,  1370,  1371,
    1372,  1373,  1374,  1375,  1376,  1377,  1378,  1379,  1383,  1387,
    1390,  1393,  1394,  1395,  1396,  1401,  1406,  1407,  1410,  1413,
    1417,  1421,  1425,  1426,  1427,  1428,  1429,  1430,  1431,  1441,
    1451,  1506,  1507,  1511,  1512,  1516,  1524,  1527,  1534,  1539,
    1547,  1548,  1552,  1555,  1564,  1565,  1569,  1578,  1579,  1584,
    1585,  1589,  1595,  1598,  1602,  1608,  1617,  1618,  1619,  1623,
    1624,  1628,  1631,  1636,  1641,  1649,  1660,  1663,  1670,  1671,
    1675,  1702,  1702,  1714,  1717,  1726,  1739,  1751,  1752,  1756,
    1760,  1771,  1786,  1787,  1791,  1830,  1896,  1903,  1912,  1919,
    1928,  1929,  1930,  1931,  1932,  1933,  1934,  1935,  1936,  1937,
    1938,  1939,  1940,  1941,  1942,  1943,  1944,  1945,  1946,  1947,
    1948,  1949,  1950,  1951,  1952,  1956,  1957,  1958,  1959,  1960,
    1961,  1965,  1976,  1980,  1990,  1997,  2006,  2019,  2020,  2021,
    2022,  2023,  2037,  2043,  2047,  2052,  2057,  2062,  2069,  2074,
    2078,  2082,  2087,  2091,  2096,  2102,  2109,  2114,  2120,  2125,
    2129,  2134,  2143,  2147,  2152,  2161,  2165,  2170,  2179,  2185,
    2194,  2194,  2209,  2209,  2229,  2230,  2231,  2232,  2236,  2243,
    2252,  2257,  2264,  2269,  2275,  2281,  2288,  2299,  2302,  2316,
    2325,  2326,  2330,  2335,  2342,  2347,  2358,  2363,  2370,  2382,
    2383,  2387,  2404
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
  "variable_name_list", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "type_declaration",
  "variant_alias_declaration", "$@2", "bitfield_alias_declaration", "$@3",
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

#define YYPACT_NINF -519

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-519)))

#define YYTABLE_NINF -195

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -519,    13,  -519,  -519,   -83,   -15,    30,    40,   -44,  -519,
      59,  -519,  -519,     3,  -519,  -519,  -519,  -519,  -519,   176,
    -519,   145,  -519,  -519,  -519,  -519,  -519,  -519,    83,  -519,
     -19,   101,   113,  -519,  -519,    24,  -519,   -69,   141,  -519,
     160,   164,  -519,    77,   167,   144,  -519,   -51,  -519,   178,
     -12,  -519,   152,    35,   -83,   200,   -15,   202,  -519,   203,
     204,  -519,   175,  -519,   193,  -519,   -97,   163,   168,   212,
    -519,   -83,     3,  -519,   205,   179,  7003,   318,   319,  -519,
     190,  -519,   356,  -519,  -519,  -519,  -519,  -519,   224,    57,
    -519,  -519,  -519,  -519,   308,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,   192,   -95,  5709,  -519,  -519,   228,   232,
    -519,   -20,  -519,   -82,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,    31,   225,   -94,   213,  -519,   148,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,   -10,  -519,  -519,   -47,  -519,
     227,   236,   238,   239,  -519,  -519,  -519,   209,  -519,  -519,
    -519,  -519,  -519,   241,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,   242,  -519,  -519,  -519,   245,   247,  -519,
    -519,  -519,  -519,   251,   252,  -519,  -519,  -519,  -519,  -519,
     374,  -519,   230,   233,  -519,   231,  -519,  -519,   -46,   -83,
    -519,   -25,  -519,  -101,  5709,   199,  -519,  -519,  -519,   162,
    -519,  -519,  -519,    57,  -519,   -91,  4063,  -519,  5709,  5709,
    5709,  5709,   266,   228,  5596,  5596,  5596,  5786,   232,  -519,
      -6,  -519,  -519,  -519,  -519,  -519,  2065,  -519,  -519,   -76,
    5709,   -73,  -519,   360,   234,   -34,   270,  -519,   177,  -519,
     246,  1315,  -519,  -519,   278,  5709,  -519,    -3,  -519,  -519,
    -519,  -519,  5519,   248,  -519,   256,   276,   280,   259,   282,
    -519,   284,  4063,  4063,   762,  4320,  4063,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  4063,  4063,  4063,  4063,  4063,  4063,
    -519,   244,  -519,  -519,   -70,  -519,  -519,  -519,   253,  -519,
    -519,  -519,  -519,  -519,  4863,   269,  -519,  -519,  -519,  -519,
    -519,  4189,  1170,  6024,  6611,   265,   -36,   225,  6612,   225,
    6653,   225,  6661,    36,  -519,    34,  1315,   103,   290,  -519,
     273,  -519,  -519,  5780,  -519,  -519,  1315,  -519,  -519,  -519,
     270,  -519,  -519,  -519,  -519,  -519,  5709,  2194,  -519,  -519,
     914,  -519,   634,  -519,  -519,  -519,  2194,  6734,  -519,   305,
    4063,  5709,  5709,  4063,  5709,  5709,   375,   375,   307,  1234,
     307,  1379,  6172,  -519,    66,   155,   375,   375,   -59,   375,
     375,  5000,   -72,  -519,  1662,   190,     0,   309,  4063,  4063,
    -519,  -519,  4063,  4063,  4063,  4063,   321,  4063,   322,  4063,
    4063,  4063,  4063,  4063,  2332,  4063,  4063,  4063,  4063,  4063,
    4063,  4063,  4063,  4063,  4063,   326,  4063,  -519,  2461,  -519,
    5709,  -519,  -519,  -519,  -519,   328,   304,  -519,   316,  -519,
     317,  -519,  5709,  -519,  5786,  -519,   232,  -519,  -519,  -519,
    -519,   747,   153,  -519,   337,  6003,  -519,  4063,  -519,  -519,
     307,  1524,  -519,   199,  4063,  4063,  -519,   295,   339,  2599,
    1807,  -519,   433,   300,   301,  4063,  -519,  -519,  -519,  -519,
    -519,  -519,   302,   310,   311,  -519,  -519,  -519,   456,  -519,
     -84,  -519,  6107,  -519,  2194,  -519,  4318,  2728,  1936,  5069,
    6735,  6774,  5137,  6782,  6794,  -519,     8,   -78,  4192,   314,
    2866,  6172,   340,   -48,   343,  -519,  -519,    23,    22,  3004,
      93,   107,  4063,  4063,   323,  -519,  4063,   348,  -519,  5596,
    -519,   353,  -519,    63,  6375,   225,  5709,  -519,  -519,   105,
     105,   229,   229,  6630,  6630,   325,   201,  -519,  5857,   -79,
     -79,   105,   105,   358,  6241,  5595,  6440,  6509,   229,   229,
     306,   306,   201,   201,   201,  -519,  5926,  -519,    64,  6807,
    -519,  -519,  -519,  1315,  -519,  4063,  -519,  -519,   153,  4063,
    4063,  4063,  4063,  4063,  4063,  4063,  4063,  4063,  4063,  4063,
    4063,  4063,  4063,  4063,   486,    38,  2194,  -519,  -519,  4414,
     481,  6375,   486,  -519,   352,   369,  6375,  2194,  -519,  4510,
     327,  -519,  -519,   486,  -519,  -519,  -519,   459,   270,  -519,
    3142,  3271,  -519,  4606,  -519,  -519,    74,  5709,   366,  5205,
    -519,  4063,  4063,  -519,  4063,   335,  4063,   368,  4063,  -519,
     353,   370,   338,   353,  4063,   344,  4063,  4063,   353,   345,
     347,  6375,  -519,  -519,  6306,   225,  6832,   -42,   -33,  4063,
    -519,   -77,  1315,  3400,  -519,  -519,  4063,  -519,  -519,  -519,
    6375,  -519,  6375,  6375,  6375,  6375,  6375,  6375,  6375,  6375,
    6375,  6375,  6375,  6375,  6375,  6375,  6375,  -519,  4063,  -519,
    4702,  -519,   199,  -519,  -519,  -519,  -519,  4798,  -519,  -519,
      45,  -519,  -519,  4063,  4063,  -519,  -519,  6833,   124,  -519,
     229,   229,   229,  3538,   -67,  -519,  6375,    75,   -42,  -519,
     340,  6375,  -519,  -519,   -60,   136,  -519,  -519,  -519,   365,
     -82,   351,  6375,  3676,  -519,  -519,  -519,    76,  6375,   -56,
    -519,  -519,  -519,  1086,   199,  -519,  -519,  4063,  -519,  6574,
    6574,  -519,  3805,   380,  -519,  5273,  4063,   350,  4063,  4063,
     354,   -82,  -519,  -519,  4063,  6375,  -519,   199,  -519,  -519,
     486,  5709,  5341,   377,  -519,  4935,  -519,  6375,  4935,  -519,
    -519,  6375,  -519,    45,  6870,  -519,  3934,   473,   361,   355,
    -519,  -519,  5709,  5409,  4063,   364,   371,  6920,  -519,  6375,
    -519,  -519,  -519
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    75,     1,   167,     0,     0,     0,     0,     0,   168,
       0,   412,   410,     0,    14,     3,    10,     9,     8,     0,
       7,   318,     6,    11,     5,     4,    12,    13,    65,    67,
      23,    35,    32,    33,    25,    30,    24,     0,     0,    15,
       0,     0,    70,   145,     0,    71,    73,     0,    69,     0,
       0,   319,   321,     0,     0,     0,     0,     0,    31,     0,
       0,    28,     0,   323,     0,   327,     0,     0,     0,     0,
     147,     0,     0,    76,   332,     0,     0,     0,     0,    82,
      77,   316,     0,    63,    64,    61,    62,    60,     0,     0,
      59,    68,    36,    34,    30,    27,    26,   365,   368,   366,
     369,   367,   370,     0,     0,     0,   329,   328,     0,     0,
     146,     0,    74,     0,   334,   286,    85,    86,    88,    87,
      89,    90,    91,    92,   111,   112,   109,   110,   102,   113,
     114,   103,   100,   101,   115,   116,   117,   118,   105,   106,
     104,    98,    99,    94,    93,    95,    96,    97,    84,    83,
     107,   108,   281,    80,     0,     0,    16,     0,    55,    56,
      53,    54,    52,    51,    57,     0,    29,   323,     0,   330,
       0,     0,     0,     0,   340,   360,   341,   372,   342,   346,
     347,   348,   349,   364,   353,   354,   355,   356,   357,   358,
     359,   361,   362,   399,   345,   352,   363,   402,   405,   343,
     350,   344,   351,     0,     0,   371,   377,   380,   378,   379,
       0,   374,     0,     0,   297,     0,    72,   333,   281,     0,
      78,     0,   290,     0,     0,     0,   336,   320,   317,   309,
     322,    17,    18,     0,    66,     0,     0,   324,     0,     0,
       0,     0,     0,     0,    77,    77,    77,     0,     0,   384,
       0,   390,   395,   393,   386,   392,     0,   326,   389,     0,
       0,     0,   335,   283,     0,     0,     0,   288,   309,    79,
     281,    81,   140,   119,     0,     0,   310,     0,    58,   331,
     223,   224,     0,     0,   218,     0,     0,     0,     0,     0,
     364,     0,     0,     0,     0,     0,     0,   188,   190,   189,
     191,   192,   193,    19,     0,     0,     0,     0,     0,     0,
     184,   185,   221,   186,   219,   274,   273,   272,    75,   277,
     220,   276,   275,   256,     0,     0,   222,   414,   415,   416,
     417,     0,     0,     0,     0,     0,     0,    80,     0,    80,
       0,    80,     0,   145,   294,     0,   292,     0,   385,   388,
       0,   391,   382,     0,   375,   413,   296,   411,   298,   284,
       0,   287,   282,   289,   300,   299,     0,     0,   301,   291,
       0,   337,     0,   307,   308,   306,     0,   149,   152,     0,
       0,     0,     0,     0,     0,     0,   247,   248,     0,     0,
       0,     0,     0,   436,     0,     0,   228,   227,   261,   226,
     225,     0,     0,   280,     0,    77,     0,     0,     0,     0,
     249,   250,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   325,     0,   396,
       0,   398,   394,   373,   376,     0,     0,   400,     0,   403,
       0,   406,     0,   408,     0,   409,     0,   387,   383,   381,
     285,   302,     0,   305,     0,   304,   142,     0,    45,    46,
       0,     0,   153,     0,     0,     0,   154,     0,     0,     0,
       0,   122,   120,     0,     0,     0,   135,   130,   128,   129,
     141,   123,     0,     0,     0,   133,   134,   136,   165,   127,
       0,   124,   194,   311,     0,   315,   194,     0,     0,     0,
       0,     0,     0,     0,     0,   338,     0,   145,   392,     0,
       0,   427,   420,     0,   430,   431,   432,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,    20,   251,    77,
     215,     0,   257,     0,   179,    80,     0,   269,   270,   229,
     230,   242,   243,   240,   241,     0,   266,   255,     0,   278,
     279,   231,   232,     0,     0,   245,   246,   244,   238,   239,
     234,   233,   235,   236,   237,   254,     0,   259,     0,     0,
     401,   404,   407,   293,   295,     0,   185,   144,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   155,   158,   194,
       0,   148,     0,   139,     0,     0,    39,     0,   162,   194,
       0,   137,   138,     0,   126,   131,   132,     0,     0,   125,
       0,     0,   313,   194,   314,   150,     0,     0,     0,     0,
     262,     0,     0,   263,     0,     0,     0,     0,     0,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   429,   437,   438,     0,    80,     0,     0,     0,     0,
     258,     0,   268,     0,   253,   271,     0,   252,   260,   397,
     303,   143,   201,   202,   204,   203,   205,   198,   199,   200,
     206,   207,   196,   197,   208,   209,   195,    49,     0,   159,
     194,   156,     0,    50,    37,    38,   163,   194,   160,   140,
      42,   166,   169,     0,     0,   312,   151,     0,     0,   173,
     170,   171,   172,     0,     0,   339,   418,     0,     0,   422,
     421,   428,   434,   433,     0,     0,   425,   435,    21,     0,
       0,     0,   180,     0,   181,   187,   213,     0,   267,     0,
     157,   164,   161,     0,     0,    40,    41,     0,    47,   211,
     210,   176,     0,     0,   264,     0,     0,     0,     0,     0,
       0,     0,   216,   212,     0,   182,   214,     0,   121,    43,
       0,     0,     0,     0,   265,   439,   424,   419,   439,   426,
     217,   183,    48,    42,     0,   174,     0,     0,     0,     0,
      44,   177,     0,     0,     0,     0,     0,     0,   175,   440,
     441,   442,   178
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -519,  -519,  -519,   127,   479,  -519,  -519,  -519,  -519,  -519,
    -519,   440,  -519,  -519,   483,  -519,  -519,  -519,  -253,  -519,
    -519,  -519,  -519,  -519,   312,  -519,   488,   -49,  -519,   471,
    -519,   240,  -234,  -333,  -519,  -519,  -198,  -519,  -163,  -339,
     -13,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,   549,
    -519,  -519,  -519,  -422,  -519,  -519,  -448,  -519,  -367,  -519,
    -519,  -519,  -519,   140,   357,  -519,  -519,  -519,   313,  -519,
     123,  -519,  -246,   330,  -266,  -242,   208,   333,   -38,  -519,
    -519,  -519,  -519,   414,   518,  -519,  -519,  -519,  -519,  -519,
    -519,  -153,  -361,   -96,  -519,  -519,  -519,   359,  -519,   -86,
    -519,  -519,  -519,  -519,   331,  -518,  -386,  -519,  -519,  -289,
     -65,   217,  -519,  -519,  -519,  -192,  -519
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   157,   163,   395,   312,    16,    17,    35,
      36,    61,    18,    32,    33,   483,   484,   757,   758,   485,
     486,   487,   488,   489,   164,   165,    29,    30,    45,    46,
      47,    19,   153,   225,    80,    20,   313,   490,   370,   491,
     314,   492,   315,   493,   494,   495,   496,   497,   628,   498,
     499,   316,   317,   543,   745,   318,   319,   320,   464,   501,
     321,   322,   323,   544,   221,   360,   264,   218,   222,   223,
     344,   345,   214,   215,   648,   267,   376,   277,   228,   154,
      52,    22,    82,   104,    65,    66,    23,    24,   114,    75,
      25,   268,   516,   325,   103,   207,   208,   212,   209,   346,
      26,    41,    27,    40,   326,   522,   523,   327,   524,   525,
     526,   527,   328,   394,   329,   798,   330
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   229,   367,   500,   446,   530,   448,   393,   450,   206,
     337,   339,   341,     2,   587,   358,   578,   546,     3,   210,
     266,    76,   111,   668,   363,   348,   646,   273,   463,   528,
     416,   588,   364,   418,   419,    42,    64,   505,   168,   226,
     657,     4,   168,     5,    57,     6,   743,     7,    83,    84,
      28,    43,     8,   418,   419,   754,   698,   354,     9,    48,
     213,    43,   365,   403,    10,   269,   755,   756,   364,   270,
     158,   159,   669,    62,   539,   106,    58,   169,   227,   669,
      11,   279,   435,   669,   436,   636,   629,    44,    72,    77,
      78,    69,   205,   236,   272,   404,   355,    44,   365,   357,
     217,    12,    63,   766,   436,    54,   651,   373,   226,   605,
     769,   444,    31,    73,   777,   374,   652,   219,   460,    54,
      54,    79,   653,   237,   406,   407,   262,    38,   206,   233,
     362,   741,   608,   547,   445,   730,    43,   375,   271,   349,
     681,   618,   206,   206,   206,   206,   216,   647,   206,   206,
     206,   206,   331,   332,   333,   334,   234,   350,   338,   340,
     342,   647,    85,    34,   206,   632,    86,   351,    87,    88,
     265,   545,    44,    37,   356,    59,    13,   647,   452,   206,
      49,   453,    50,    14,   160,    60,   206,   655,   161,   372,
     162,    88,    39,   656,   219,   585,   377,   220,   206,   206,
      89,    51,   669,   669,   454,    69,   410,   411,   389,   391,
     358,   205,   671,   669,   416,   669,   417,   418,   419,   420,
     406,   407,   421,    53,   724,   205,   205,   205,   205,   670,
     678,   205,   205,   205,   205,   734,   533,    97,   534,   767,
     716,    98,   776,    55,   662,   653,    69,   205,   406,   407,
     455,   747,    56,   430,   431,   432,   433,   434,    99,   100,
     101,   102,   205,   653,   727,   659,   435,   699,   436,   205,
     206,   762,   735,   456,    64,   610,   749,   656,   706,   660,
     461,   205,   205,   231,   232,   206,   206,   364,   206,   206,
     156,   535,   536,    67,   763,   510,   511,    68,   513,   514,
      70,   274,   410,   411,   275,   665,   653,   276,   770,    71,
     416,    74,   417,   418,   419,   420,   274,   365,   421,   366,
     310,   586,   276,    81,   272,   406,   407,    92,   408,   409,
     410,   411,   739,   105,   108,    94,    95,    96,   416,   109,
     417,   418,   419,   420,   206,   110,   421,   113,   422,   423,
     115,   150,   151,   205,   579,   152,   206,   155,   206,   156,
      58,   211,   435,   167,   436,   213,   583,   224,   205,   205,
     272,   205,   205,   238,   242,   260,   324,   430,   431,   432,
     433,   434,   239,   230,   240,   241,   500,   243,   244,   540,
     435,   245,   436,   246,   406,   407,   353,   247,   248,   335,
     259,   261,   359,   226,   361,   249,   697,   410,   411,   219,
     250,   371,   402,   379,   703,   416,    13,   417,   418,   419,
     420,   380,   381,   421,   383,   710,   382,   205,   384,   251,
     385,   443,   386,   387,   438,   457,   392,   458,   508,   205,
     515,   205,   548,   206,   396,   397,   398,   399,   400,   401,
     206,   580,   588,   666,   555,   557,   432,   433,   434,   575,
     672,   354,   768,   581,   582,   613,   614,   435,   620,   436,
     621,   622,   624,   744,   627,   229,   410,   411,   649,   651,
     625,   626,   654,   231,   416,   252,   667,   418,   419,   420,
     673,   675,   421,   702,   704,   663,   705,   711,   709,   718,
     723,   725,   729,   728,   751,   406,   407,   465,   732,   736,
     502,   737,   771,   783,   786,   253,   506,   773,   789,   254,
     509,   804,   537,   512,   796,   805,   205,   806,   810,   521,
     255,   521,    90,   205,   166,   811,   435,   256,   436,    93,
     800,   206,    91,   112,   257,   278,   753,   258,   549,   550,
      21,   717,   551,   552,   553,   554,   779,   556,   405,   558,
     559,   560,   561,   562,   564,   565,   566,   567,   568,   569,
     570,   571,   572,   573,   574,   263,   576,   584,   347,   792,
     504,   235,   793,   369,   107,   408,   409,   410,   411,   412,
     712,   733,   413,   414,   415,   416,   799,   417,   418,   419,
     420,   368,   336,   421,     0,   422,   423,   604,   531,     0,
       0,   609,     0,   378,   611,   612,     0,     0,     0,   616,
     619,     0,     0,     0,   205,   623,     0,   424,     0,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,     0,
       0,     0,     0,     0,   633,     0,     0,   435,   639,   436,
       0,     0,     0,     0,     0,     0,     0,   272,   400,     0,
     401,     0,     0,     0,     0,   249,     0,     0,     0,   401,
     250,     0,   661,   392,     0,     0,   664,     0,     0,     0,
       0,     0,     0,     0,     0,   206,     0,     0,     0,   251,
       0,     0,     0,     0,     0,   794,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   206,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   807,     0,     0,     0,
       0,     0,     0,     0,     0,   680,     0,   772,     0,   682,
     683,   684,   685,   686,   687,   688,   689,   690,   691,   692,
     693,   694,   695,   696,   373,   252,   700,     0,     0,     0,
       0,     0,   374,     0,     0,     0,     0,   707,   790,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   205,     0,
     692,   696,     0,   388,   375,   253,     0,     0,   249,   254,
       0,   720,   721,   250,   722,   170,     0,     0,   726,   205,
     255,   171,     0,     0,   731,     0,   521,   256,     0,     0,
       0,     0,   251,     0,   503,     0,   172,   258,     0,   742,
       0,     0,     0,     0,     0,     0,   748,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,     0,     0,   759,   760,     0,     0,   364,   252,     0,
       0,     0,     0,   765,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   775,     0,     0,     0,   365,   253,     0,
       0,     0,   254,   502,     0,    43,     0,   780,     0,     0,
       0,     0,   782,   255,     0,     0,   785,     0,   787,   788,
     256,     0,     0,     0,   791,   466,     0,     0,     0,     3,
     258,   467,   468,   469,     0,   470,     0,   280,   281,   282,
     283,    44,     0,     0,     0,     0,   803,     0,   471,   284,
     472,   473,     0,     0,   809,     0,     0,     0,     0,     0,
     474,   285,     0,     0,   475,   286,     0,   287,     0,     9,
     288,   476,     0,   477,   289,     0,     0,   478,   479,     0,
       0,     0,   174,   175,   176,     0,   178,   179,   180,   181,
     182,   290,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,   194,   195,   196,     0,     0,   199,   200,   201,
     202,     0,     0,   291,   480,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   292,   293,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     462,     0,     0,     0,     0,     0,     0,     0,   294,   295,
     296,   297,   298,   299,   300,   301,   302,    43,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,   305,   306,     0,     0,     0,     0,   307,
     308,     0,     0,     0,     0,     0,     0,     0,     0,   309,
       0,   310,   311,    44,   481,   272,   482,   466,     0,     0,
       0,     3,     0,   467,   468,   469,     0,   470,     0,   280,
     281,   282,   283,     0,     0,     0,     0,     0,     0,     0,
     471,   284,   472,   473,     0,     0,     0,     0,     0,     0,
       0,     0,   474,   285,     0,     0,   475,   286,     0,   287,
       0,     9,   288,   476,     0,   477,   289,     0,     0,   478,
     479,     0,     0,     0,   174,   175,   176,     0,   178,   179,
     180,   181,   182,   290,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     0,   194,   195,   196,     0,     0,   199,
     200,   201,   202,     0,     0,   291,   480,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   292,   293,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   249,   462,     0,     0,     0,   250,     0,     0,     0,
     294,   295,   296,   297,   298,   299,   300,   301,   302,    43,
     303,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   304,   305,   306,     0,     0,     0,
       0,   307,   308,     0,     0,     0,     0,   280,   281,   282,
     283,   309,     0,   310,   311,    44,   481,   272,   778,   284,
       0,     0,     0,     0,     0,   249,     0,     0,     0,     0,
     250,   285,     0,     0,     0,   286,     0,   287,     0,     0,
     288,   252,     0,     0,   289,     0,     0,     0,     0,   251,
       0,     0,   174,   175,   176,     0,   178,   179,   180,   181,
     182,   290,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   253,   194,   195,   196,   254,     0,   199,   200,   201,
     202,     0,     0,   291,     0,     0,   255,     0,     0,     0,
       0,     0,     0,   256,     0,   292,   293,     0,     0,     0,
     440,     0,     0,   258,     0,   252,   249,     0,     0,     0,
       0,   250,     0,     0,     0,     0,     0,     0,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   517,   303,     0,
     251,     0,     0,     0,     0,   253,     0,     0,     0,   254,
       0,     0,   304,   305,   306,     0,     0,     0,     0,   307,
     518,     0,   280,   281,   282,   283,     0,   256,   519,   520,
       0,   310,   311,    44,   284,   272,     0,   258,     0,     0,
     249,     0,     0,     0,     0,   250,   285,     0,     0,     0,
     286,     0,   287,     0,     0,   288,   252,     0,     0,   289,
       0,     0,     0,     0,   251,     0,     0,   174,   175,   176,
       0,   178,   179,   180,   181,   182,   290,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   253,   194,   195,   196,
     254,     0,   199,   200,   201,   202,     0,     0,   291,     0,
       0,   255,     0,     0,     0,     0,     0,     0,   256,     0,
     292,   293,     0,     0,     0,     0,     0,     0,   258,     0,
     252,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   517,   303,     0,     0,     0,     0,     0,     0,
     253,     0,     0,     0,   254,     0,     0,   304,   305,   306,
       0,     0,     0,     0,   307,   518,     0,   280,   281,   282,
     283,     0,   256,     0,   529,     0,   310,   311,    44,   284,
     272,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,   285,     0,     0,     0,   286,     0,   287,     0,     0,
     288,     0,     0,     0,   289,     0,     0,     0,     0,     0,
       0,     0,   174,   175,   176,     0,   178,   179,   180,   181,
     182,   290,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,   194,   195,   196,     0,     0,   199,   200,   201,
     202,     0,     0,   291,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   292,   293,     0,     0,     0,
       0,     0,     0,     0,   606,     0,     0,     0,     0,     0,
     462,     0,     0,     0,     0,     0,     0,     0,   294,   295,
     296,   297,   298,   299,   300,   301,   302,    43,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,   305,   306,   280,   281,   282,   283,   307,
     308,     0,     0,     0,     0,     0,     0,   284,     0,   309,
       0,   310,   311,    44,   607,   272,     0,     0,     0,   285,
       0,     0,     0,   286,     0,   287,     0,     0,   288,     0,
       0,     0,   289,     0,     0,     0,     0,     0,     0,     0,
     174,   175,   176,     0,   178,   179,   180,   181,   182,   290,
     184,   185,   186,   187,   188,   189,   190,   191,   192,     0,
     194,   195,   196,     0,     0,   199,   200,   201,   202,     0,
       0,   291,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   292,   293,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   294,   295,   296,   297,
     298,   299,   300,   301,   302,    43,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     304,   305,   306,     0,     0,     0,     0,   307,   308,     0,
     280,   281,   282,   283,     0,   541,     0,   309,   542,   310,
     311,    44,   284,   272,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   285,     0,     0,     0,   286,     0,
     287,     0,     0,   288,     0,     0,     0,   289,     0,     0,
       0,     0,     0,     0,     0,   174,   175,   176,     0,   178,
     179,   180,   181,   182,   290,   184,   185,   186,   187,   188,
     189,   190,   191,   192,     0,   194,   195,   196,     0,     0,
     199,   200,   201,   202,     0,     0,   291,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   292,   293,
       0,     0,     0,     0,     0,     0,     0,   617,     0,     0,
       0,     0,     0,   462,     0,     0,     0,     0,     0,     0,
       0,   294,   295,   296,   297,   298,   299,   300,   301,   302,
      43,   303,     0,     0,     0,     0,     0,     0,     0,   280,
     281,   282,   283,   637,     0,   304,   305,   306,     0,     0,
       0,   284,   307,   308,     0,     0,     0,     0,     0,     0,
       0,     0,   309,   285,   310,   311,    44,   286,   272,   287,
       0,     0,   288,     0,     0,     0,   289,     0,     0,     0,
       0,     0,     0,     0,   174,   175,   176,     0,   178,   179,
     180,   181,   182,   290,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     0,   194,   195,   196,     0,     0,   199,
     200,   201,   202,     0,     0,   291,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   292,   293,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     294,   295,   296,   297,   298,   299,   300,   301,   302,    43,
     303,     0,     0,     0,     0,     0,     0,     0,   280,   281,
     282,   283,   638,     0,   304,   305,   306,     0,     0,     0,
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
     283,     0,     0,   304,   305,   306,     0,     0,     0,   284,
     307,   308,     0,     0,     0,     0,     0,     0,     0,   352,
     309,   285,   310,   311,    44,   286,   272,   287,     0,     0,
     288,     0,     0,     0,   289,     0,     0,     0,     0,     0,
       0,     0,   174,   175,   176,     0,   178,   179,   180,   181,
     182,   290,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,   194,   195,   196,     0,     0,   199,   200,   201,
     202,     0,     0,   291,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   292,   293,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     462,     0,     0,     0,     0,     0,     0,     0,   294,   295,
     296,   297,   298,   299,   300,   301,   302,    43,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,   305,   306,   280,   281,   282,   283,   307,
     308,     0,   563,     0,     0,     0,     0,   284,     0,   309,
       0,   310,   311,    44,     0,   272,     0,     0,     0,   285,
       0,     0,     0,   286,     0,   287,     0,     0,   288,     0,
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
     304,   305,   306,     0,     0,     0,   284,   307,   308,     0,
       0,     0,     0,     0,     0,     0,     0,   309,   285,   310,
     311,    44,   286,   272,   287,     0,     0,   288,     0,     0,
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
     305,   306,   280,   281,   282,   283,   307,   308,     0,     0,
       0,     0,     0,     0,   284,     0,   309,   577,   310,   311,
      44,     0,   272,     0,     0,     0,   285,     0,     0,     0,
     286,     0,   287,     0,     0,   288,     0,     0,     0,   289,
       0,     0,   615,     0,     0,     0,     0,   174,   175,   176,
       0,   178,   179,   180,   181,   182,   290,   184,   185,   186,
     187,   188,   189,   190,   191,   192,     0,   194,   195,   196,
       0,     0,   199,   200,   201,   202,     0,     0,   291,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     292,   293,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   294,   295,   296,   297,   298,   299,   300,
     301,   302,    43,   303,     0,     0,     0,     0,     0,     0,
       0,   280,   281,   282,   283,     0,     0,   304,   305,   306,
       0,     0,     0,   284,   307,   308,     0,     0,     0,     0,
       0,     0,     0,     0,   309,   285,   310,   311,    44,   286,
     272,   287,     0,     0,   288,     0,     0,     0,   289,     0,
       0,     0,     0,     0,     0,     0,   174,   175,   176,     0,
     178,   179,   180,   181,   182,   290,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     0,   194,   195,   196,     0,
       0,   199,   200,   201,   202,     0,     0,   291,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   292,
     293,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   294,   295,   296,   297,   298,   299,   300,   301,
     302,    43,   303,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   304,   305,   306,   280,
     281,   282,   283,   307,   308,     0,     0,     0,     0,     0,
       0,   284,     0,   309,   635,   310,   311,    44,     0,   272,
       0,     0,     0,   285,     0,     0,     0,   286,     0,   287,
       0,     0,   288,     0,     0,     0,   289,     0,     0,     0,
       0,     0,     0,     0,   174,   175,   176,     0,   178,   179,
     180,   181,   182,   290,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     0,   194,   195,   196,     0,     0,   199,
     200,   201,   202,     0,     0,   291,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   292,   293,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     294,   295,   296,   297,   298,   299,   300,   301,   302,    43,
     303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,   305,   306,   280,   281,   282,
     283,   307,   308,     0,     0,     0,     0,     0,     0,   284,
       0,   309,   650,   310,   311,    44,     0,   272,     0,     0,
       0,   285,     0,     0,     0,   286,     0,   287,     0,     0,
     288,     0,     0,     0,   289,     0,     0,     0,     0,     0,
       0,     0,   174,   175,   176,     0,   178,   179,   180,   181,
     182,   290,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,   194,   195,   196,     0,     0,   199,   200,   201,
     202,     0,     0,   291,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   292,   293,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   294,   295,
     296,   297,   298,   299,   300,   301,   302,    43,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,   305,   306,   280,   281,   282,   283,   307,
     308,     0,     0,     0,     0,     0,     0,   284,     0,   309,
     658,   310,   311,    44,     0,   272,     0,     0,     0,   285,
       0,     0,     0,   286,     0,   287,     0,     0,   288,     0,
       0,     0,   289,     0,     0,     0,     0,     0,     0,     0,
     174,   175,   176,     0,   178,   179,   180,   181,   182,   290,
     184,   185,   186,   187,   188,   189,   190,   191,   192,     0,
     194,   195,   196,     0,     0,   199,   200,   201,   202,     0,
       0,   291,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   292,   293,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   713,     0,
       0,     0,     0,     0,     0,     0,   294,   295,   296,   297,
     298,   299,   300,   301,   302,    43,   303,     0,     0,     0,
       0,     0,     0,     0,   280,   281,   282,   283,     0,     0,
     304,   305,   306,     0,     0,     0,   284,   307,   308,     0,
       0,     0,     0,     0,     0,     0,     0,   309,   285,   310,
     311,    44,   286,   272,   287,     0,     0,   288,     0,     0,
       0,   289,     0,     0,     0,     0,     0,     0,     0,   174,
     175,   176,     0,   178,   179,   180,   181,   182,   290,   184,
     185,   186,   187,   188,   189,   190,   191,   192,     0,   194,
     195,   196,     0,     0,   199,   200,   201,   202,     0,     0,
     291,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   292,   293,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   714,     0,     0,
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
       0,     0,     0,   284,     0,   309,   746,   310,   311,    44,
       0,   272,     0,     0,     0,   285,     0,     0,     0,   286,
       0,   287,     0,     0,   288,     0,     0,     0,   289,     0,
       0,     0,     0,     0,     0,     0,   174,   175,   176,     0,
     178,   179,   180,   181,   182,   290,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     0,   194,   195,   196,     0,
       0,   199,   200,   201,   202,     0,     0,   291,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   292,
     293,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   294,   295,   296,   297,   298,   299,   300,   301,
     302,    43,   303,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   304,   305,   306,   280,
     281,   282,   283,   307,   308,     0,     0,     0,     0,     0,
       0,   284,     0,   309,   764,   310,   311,    44,     0,   272,
       0,     0,     0,   285,     0,     0,     0,   286,     0,   287,
       0,     0,   288,     0,     0,     0,   289,     0,     0,     0,
       0,     0,     0,     0,   174,   175,   176,     0,   178,   179,
     180,   181,   182,   290,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     0,   194,   195,   196,     0,     0,   199,
     200,   201,   202,     0,     0,   291,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   292,   293,     0,
       0,     0,     0,     0,     0,     0,   774,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     294,   295,   296,   297,   298,   299,   300,   301,   302,    43,
     303,     0,     0,     0,     0,     0,     0,     0,   280,   281,
     282,   283,   781,     0,   304,   305,   306,     0,     0,     0,
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
     283,   802,     0,   304,   305,   306,     0,     0,     0,   284,
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
     249,     0,     0,     0,     0,   250,     0,     0,   309,   285,
     310,   311,    44,   286,   272,   287,     0,     0,   288,     0,
       0,     0,   289,     0,   251,     0,     0,     0,     0,     0,
     174,   175,   176,     0,   178,   179,   180,   181,   182,   290,
     184,   185,   186,   187,   188,   189,   190,   191,   192,     0,
     194,   195,   196,     0,     0,   199,   200,   201,   202,     0,
       0,   291,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   292,   293,     0,     0,     0,     0,     0,
     252,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   294,   295,   296,   297,
     298,   299,   300,   301,   302,    43,   303,     0,     0,     0,
     253,   390,     0,     0,   254,     0,   439,   406,   407,     0,
       0,     0,     0,   170,     0,   255,     0,   307,   308,   171,
       0,     0,   256,     0,     0,     0,     0,   309,     0,   310,
     311,    44,   258,   272,   172,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   589,
     590,   591,   592,   593,   594,   595,   596,   408,   409,   410,
     411,   412,   597,   598,   413,   414,   415,   416,   599,   417,
     418,   419,   420,   406,   407,   421,   600,   422,   423,   601,
     602,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,     0,     0,   603,   424,
       0,   425,   426,   427,   428,   429,   430,   431,   432,   433,
     434,     0,     0,     0,     0,     0,     0,     0,     0,   435,
       0,   436,     0,     0,     0,     0,     0,     0,   634,    44,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   589,   590,   591,   592,   593,
     594,   595,   596,   408,   409,   410,   411,   412,   597,   598,
     413,   414,   415,   416,   599,   417,   418,   419,   420,   406,
     407,   421,   600,   422,   423,   601,   602,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   603,   424,     0,   425,   426,   427,
     428,   429,   430,   431,   432,   433,   434,     0,     0,     0,
       0,     0,     0,     0,     0,   435,     0,   436,     0,     0,
       0,     0,     0,     0,   701,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   589,   590,   591,   592,   593,   594,   595,   596,   408,
     409,   410,   411,   412,   597,   598,   413,   414,   415,   416,
     599,   417,   418,   419,   420,   406,   407,   421,   600,   422,
     423,   601,   602,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     603,   424,     0,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,     0,     0,     0,     0,     0,     0,     0,
       0,   435,     0,   436,     0,     0,     0,     0,     0,     0,
     708,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   589,   590,   591,
     592,   593,   594,   595,   596,   408,   409,   410,   411,   412,
     597,   598,   413,   414,   415,   416,   599,   417,   418,   419,
     420,   406,   407,   421,   600,   422,   423,   601,   602,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   603,   424,     0,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,     0,
       0,     0,     0,     0,     0,     0,     0,   435,     0,   436,
       0,     0,     0,     0,     0,     0,   715,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   589,   590,   591,   592,   593,   594,   595,
     596,   408,   409,   410,   411,   412,   597,   598,   413,   414,
     415,   416,   599,   417,   418,   419,   420,   406,   407,   421,
     600,   422,   423,   601,   602,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   603,   424,     0,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,     0,     0,     0,     0,     0,
       0,     0,     0,   435,     0,   436,     0,     0,     0,     0,
       0,     0,   750,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   406,   407,     0,     0,     0,     0,     0,   589,
     590,   591,   592,   593,   594,   595,   596,   408,   409,   410,
     411,   412,   597,   598,   413,   414,   415,   416,   599,   417,
     418,   419,   420,     0,     0,   421,   600,   422,   423,   601,
     602,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   603,   424,
       0,   425,   426,   427,   428,   429,   430,   431,   432,   433,
     434,     0,     0,     0,   406,   407,     0,     0,     0,   435,
       0,   436,   408,   409,   410,   411,   412,     0,   752,   413,
     414,   415,   416,     0,   417,   418,   419,   420,     0,     0,
     421,     0,   422,   423,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   424,     0,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,     0,     0,     0,   406,
     407,     0,     0,     0,   435,     0,   436,     0,     0,     0,
       0,     0,     0,   437,   408,   409,   410,   411,   412,     0,
       0,   413,   414,   415,   416,     0,   417,   418,   419,   420,
       0,     0,   421,     0,   422,   423,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   424,     0,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   406,   407,
       0,     0,     0,     0,     0,     0,   435,     0,   436,   408,
     409,   410,   411,   412,     0,   797,   413,   414,   415,   416,
       0,   417,   418,   419,   420,     0,     0,   421,     0,   422,
     423,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   424,     0,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,     0,     0,     0,   406,   407,     0,     0,
       0,   435,     0,   436,     0,     0,   538,     0,   408,   409,
     410,   411,   412,     0,     0,   413,   414,   415,   416,     0,
     417,   418,   419,   420,     0,     0,   421,     0,   422,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     424,     0,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,     0,     0,   406,   407,     0,     0,     0,     0,
     435,     0,   436,     0,     0,   640,   408,   409,   410,   411,
     412,     0,     0,   413,   414,   415,   416,     0,   417,   418,
     419,   420,     0,     0,   421,     0,   422,   423,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
       0,     0,   406,   407,     0,     0,     0,     0,   435,     0,
     436,     0,     0,   643,   408,   409,   410,   411,   412,     0,
       0,   413,   414,   415,   416,     0,   417,   418,   419,   420,
       0,     0,   421,     0,   422,   423,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   424,     0,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,     0,     0,
     406,   407,     0,     0,     0,     0,   435,     0,   436,     0,
       0,   719,   408,   409,   410,   411,   412,     0,     0,   413,
     414,   415,   416,     0,   417,   418,   419,   420,     0,     0,
     421,     0,   422,   423,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   424,     0,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,     0,     0,   406,   407,
       0,     0,     0,     0,   435,     0,   436,     0,     0,   784,
     408,   409,   410,   411,   412,     0,     0,   413,   414,   415,
     416,     0,   417,   418,   419,   420,     0,     0,   421,     0,
     422,   423,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   424,     0,   425,   426,   427,   428,   429,   430,
     431,   432,   433,   434,     0,     0,     0,     0,     0,     0,
       0,     0,   435,     0,   436,     0,     0,   795,   408,   409,
     410,   411,   412,     0,     0,   413,   414,   415,   416,     0,
     417,   418,   419,   420,     0,     0,   421,     0,   422,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   170,     0,     0,     0,     0,     0,   171,     0,
     424,     0,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,     0,   172,     0,     0,     0,     0,     0,     0,
     435,     0,   436,     0,     0,   808,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,     0,     0,
       0,     0,     0,     0,   406,   407,     0,     0,     0,   170,
       0,     0,     0,     0,     0,   171,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     172,     0,     0,   294,   295,   296,     0,     0,     0,     0,
       0,     0,    43,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,     0,     0,     0,    44,     0,
       0,     0,     0,     0,   408,   409,   410,   411,   412,     0,
       0,   413,   414,   415,   416,     0,   417,   418,   419,   420,
       0,     0,   421,     0,   422,   423,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,   170,     0,     0,     0,     0,     0,   171,   426,
     427,   428,   429,   430,   431,   432,   433,   434,     0,     0,
       0,     0,     0,   172,     0,     0,   435,     0,   436,     0,
       0,   152,     0,     0,     0,    44,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,     0,   406,
     407,     0,     0,     0,     0,     0,     0,     0,     0,   170,
       0,     0,     0,     0,     0,   171,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     172,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,     0,   406,   407,    44,   408,
     409,   410,   411,   412,     0,     0,   413,   414,   415,   416,
       0,   417,   418,   419,   420,     0,     0,   421,     0,   422,
     423,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,   424,     0,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,     0,     0,     0,     0,     0,     0,     0,
       0,   435,     0,   436,   459,   406,   407,     0,     0,     0,
       0,     0,     0,     0,     0,    44,   408,   409,   410,   411,
     412,     0,     0,   413,   414,   415,   416,     0,   417,   418,
     419,   420,     0,     0,   421,     0,   422,   423,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
       0,     0,     0,     0,     0,     0,     0,     0,   435,     0,
     436,   674,   406,   407,     0,   408,   409,   410,   411,   412,
       0,     0,   413,   414,   415,   416,     0,   417,   418,   419,
     420,     0,     0,   421,     0,   422,   423,     0,     0,     0,
       0,     0,     0,     0,     0,   249,     0,     0,     0,     0,
     250,     0,     0,     0,     0,     0,     0,   424,     0,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,   251,
       0,     0,     0,     0,     0,     0,     0,   435,     0,   436,
     677,     0,     0,     0,   589,   590,   591,   592,   593,   594,
     595,   596,   408,   409,   410,   411,   412,   597,   598,   413,
     414,   415,   416,   599,   417,   418,   419,   420,  -194,     0,
     421,   600,   422,   423,   601,   602,   406,   407,     0,     0,
       0,     0,     0,     0,     0,   252,     0,     0,     0,     0,
       0,     0,     0,   603,   424,     0,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,     0,     0,     0,     0,
       0,     0,     0,     0,   435,   253,   436,     0,     0,   254,
       0,   441,     0,     0,     0,     0,     0,     0,     0,     0,
     255,     0,     0,     0,     0,     0,     0,   256,     0,     0,
       0,   406,   407,     0,     0,     0,     0,   258,   589,   590,
     591,   592,   593,   594,   595,   596,   408,   409,   410,   411,
     412,   597,   598,   413,   414,   415,   416,   630,   417,   418,
     419,   420,     0,     0,   421,   600,   422,   423,   601,   602,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   631,   424,     0,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     406,   407,     0,     0,     0,     0,     0,     0,   435,     0,
     436,   408,   409,   410,   411,   412,     0,     0,   413,   414,
     415,   416,     0,   417,   418,   419,   420,     0,     0,   421,
       0,   422,   423,     0,     0,   532,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   424,     0,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   406,   407,     0,     0,     0,
       0,     0,     0,   435,     0,   436,     0,     0,     0,     0,
     408,   409,   410,   411,   412,     0,     0,   413,   414,   415,
     416,     0,   417,   418,   419,   420,     0,     0,   421,     0,
     422,   423,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   424,   676,   425,   426,   427,   428,   429,   430,
     431,   432,   433,   434,   406,   407,     0,     0,     0,     0,
       0,     0,   435,     0,   436,   408,   409,   410,   411,   412,
       0,     0,   413,   414,   415,   416,     0,   417,   418,   419,
     420,     0,     0,   421,     0,   422,   423,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   738,     0,     0,   424,     0,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,   406,
     407,     0,     0,     0,     0,     0,     0,   435,     0,   436,
       0,     0,     0,     0,   408,   409,   410,   411,   412,     0,
       0,   413,   414,   415,   416,     0,   417,   418,   419,   420,
       0,     0,   421,     0,   422,   423,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   424,     0,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   406,   407,
       0,     0,     0,     0,     0,     0,   435,     0,   436,   408,
     409,   410,   411,   412,     0,     0,   413,   414,   415,   416,
       0,   417,   418,   419,   420,     0,     0,   421,     0,   422,
     423,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   427,   428,   429,   430,   431,
     432,   433,   434,   406,   407,     0,     0,     0,     0,     0,
       0,   435,     0,   436,     0,     0,     0,     0,   408,   409,
     410,   411,   412,     0,     0,   413,   414,   415,   416,     0,
     417,   418,   419,   420,     0,     0,   421,     0,   422,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   249,   249,     0,     0,     0,   250,   250,   406,
     407,     0,     0,     0,     0,   428,   429,   430,   431,   432,
     433,   434,     0,     0,     0,     0,   251,   251,     0,     0,
     435,     0,   436,   408,   409,     0,     0,   412,     0,     0,
     413,   414,   415,   416,   249,   417,   418,   419,   420,   250,
       0,   421,   249,   422,   423,     0,     0,   250,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   251,     0,
       0,     0,     0,     0,     0,   424,   251,   425,   426,   427,
     428,   429,   252,   252,     0,   433,   434,     0,     0,   408,
     409,   410,   411,   412,     0,   435,   413,   436,     0,   416,
       0,   417,   418,   419,   420,     0,     0,   421,     0,   422,
     423,     0,   253,   253,     0,     0,   254,   254,   442,   447,
       0,     0,     0,     0,   252,   249,   249,   255,   255,     0,
     250,   250,   252,     0,   256,   256,   428,   429,   430,   431,
     432,   433,   434,     0,   258,   258,     0,     0,     0,   251,
     251,   435,     0,   436,   253,     0,     0,     0,   254,     0,
     449,     0,   253,     0,     0,   249,   254,     0,   451,   255,
     250,     0,     0,   249,     0,     0,   256,   255,   250,     0,
       0,     0,     0,     0,   256,   249,   258,     0,     0,   251,
     250,     0,     0,     0,   258,     0,     0,   251,   249,     0,
       0,     0,     0,   250,     0,   252,   252,     0,     0,   251,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   251,   249,   249,     0,     0,     0,   250,   250,
       0,     0,     0,     0,     0,   253,   253,     0,     0,   254,
     254,     0,   641,     0,     0,   252,     0,   251,   251,     0,
     255,   255,     0,   252,     0,     0,     0,   256,   256,   507,
       0,   249,     0,     0,     0,   252,   250,   258,   258,     0,
       0,     0,     0,     0,     0,   253,     0,     0,   252,   254,
       0,   642,     0,   253,     0,   251,     0,   254,     0,   644,
     255,     0,     0,     0,     0,   253,     0,   256,   255,   254,
       0,   645,     0,   252,   252,   256,     0,   258,   253,     0,
     255,   249,   254,     0,   679,   258,   250,   256,     0,     0,
       0,     0,     0,   255,     0,     0,     0,   258,     0,     0,
     256,     0,     0,   253,   253,   251,     0,   254,   254,   740,
     258,   252,     0,     0,     0,     0,     0,     0,   255,   255,
       0,     0,     0,     0,     0,   256,   256,     0,     0,   761,
       0,     0,     0,     0,     0,   258,   258,     0,     0,     0,
       0,   253,     0,     0,     0,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   255,     0,     0,     0,
       0,   252,     0,   256,     0,     0,   801,     0,     0,     0,
       0,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   253,     0,     0,     0,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   255,     0,     0,     0,
       0,     0,     0,   256,     0,     0,   812,     0,     0,     0,
       0,     0,     0,   258,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   134,   135,   136,   137,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,     0,     0,   148,   149
};

static const yytype_int16 yycheck[] =
{
      13,   154,   268,   370,   337,   391,   339,   296,   341,   105,
     244,   245,   246,     0,   462,   261,   438,    17,     5,   105,
      45,    33,    71,   541,   266,    31,    18,   225,   367,   390,
     109,   115,   110,   112,   113,    32,   133,   376,   133,   133,
      18,    28,   133,    30,    20,    32,   123,    34,    13,    14,
     133,   133,    39,   112,   113,    10,    18,   133,    45,    72,
     133,   133,   140,   133,    51,   166,    21,    22,   110,   170,
      13,    14,   139,   142,   146,   172,    52,   172,   172,   139,
      67,   172,   161,   139,   163,   507,   170,   169,   139,   101,
     102,   169,   105,   140,   171,   165,   172,   169,   140,   172,
     113,    88,   171,   170,   163,   139,   139,   110,   133,   470,
     170,   147,   127,   164,   170,   118,   164,   163,   360,   139,
     139,   133,   170,   170,    19,    20,   172,   171,   224,   139,
     164,   164,   471,   133,   170,   653,   133,   140,   224,   145,
     588,   480,   238,   239,   240,   241,   166,   139,   244,   245,
     246,   247,   238,   239,   240,   241,   166,   163,   244,   245,
     246,   139,   127,   133,   260,   504,   131,   173,   133,   134,
     219,   405,   169,   133,   260,   151,   163,   139,   142,   275,
       4,   147,     6,   170,   127,   161,   282,   164,   131,   275,
     133,   134,   133,   170,   163,   461,   282,   166,   294,   295,
     165,    56,   139,   139,   170,   169,   101,   102,   294,   295,
     456,   224,   545,   139,   109,   139,   111,   112,   113,   114,
      19,    20,   117,   140,   646,   238,   239,   240,   241,   166,
     166,   244,   245,   246,   247,   657,   170,    62,   172,   164,
     166,    66,   166,   142,   533,   170,   169,   260,    19,    20,
     147,   673,   139,   148,   149,   150,   151,   152,    83,    84,
      85,    86,   275,   170,   650,   172,   161,   606,   163,   282,
     366,   147,   658,   170,   133,   473,   698,   170,   617,   172,
     366,   294,   295,   135,   136,   381,   382,   110,   384,   385,
     135,   136,   137,   133,   170,   381,   382,   133,   384,   385,
     133,   139,   101,   102,   142,   539,   170,   145,   172,   165,
     109,   133,   111,   112,   113,   114,   139,   140,   117,   142,
     167,   168,   145,   171,   171,    19,    20,   127,    99,   100,
     101,   102,   665,   140,   171,   133,   133,   133,   109,   171,
     111,   112,   113,   114,   440,   133,   117,   142,   119,   120,
     171,    33,    33,   366,   440,   165,   452,     1,   454,   135,
      52,   133,   161,   171,   163,   133,   452,   142,   381,   382,
     171,   384,   385,   146,   165,   142,   236,   148,   149,   150,
     151,   152,   146,   170,   146,   146,   753,   146,   146,   402,
     161,   146,   163,   146,    19,    20,   256,   146,   146,   133,
     170,   170,    42,   133,   170,    31,   604,   101,   102,   163,
      36,   133,   168,   165,   612,   109,   163,   111,   112,   113,
     114,   165,   146,   117,   165,   623,   146,   440,   146,    55,
     146,   166,   292,   293,   165,   145,   296,   164,   133,   452,
     133,   454,   133,   539,   304,   305,   306,   307,   308,   309,
     546,   147,   115,   539,   133,   133,   150,   151,   152,   133,
     546,   133,   728,   147,   147,   170,   127,   161,    35,   163,
     170,   170,   170,   671,    18,   628,   101,   102,   164,   139,
     170,   170,   139,   135,   109,   111,   133,   112,   113,   114,
     165,   133,   117,    12,   142,   172,   127,    38,   171,   133,
     165,   133,   164,   133,   702,    19,    20,   367,   164,   164,
     370,   164,   147,   133,   164,   141,   376,   166,   164,   145,
     380,    48,   395,   383,   147,   164,   539,   172,   164,   389,
     156,   391,    53,   546,    94,   164,   161,   163,   163,    56,
     793,   637,    54,    72,   170,   233,   709,   173,   408,   409,
       1,   637,   412,   413,   414,   415,   754,   417,   318,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   218,   436,   454,   248,   777,
     372,   167,   780,   270,    66,    99,   100,   101,   102,   103,
     628,   656,   106,   107,   108,   109,   788,   111,   112,   113,
     114,   268,   243,   117,    -1,   119,   120,   467,   391,    -1,
      -1,   471,    -1,   282,   474,   475,    -1,    -1,    -1,   479,
     480,    -1,    -1,    -1,   637,   485,    -1,   141,    -1,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    -1,
      -1,    -1,    -1,    -1,   504,    -1,    -1,   161,   508,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,   518,    -1,
     520,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,   529,
      36,    -1,   532,   533,    -1,    -1,   536,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   781,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,   781,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   802,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   802,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   585,    -1,   740,    -1,   589,
     590,   591,   592,   593,   594,   595,   596,   597,   598,   599,
     600,   601,   602,   603,   110,   111,   606,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,   617,   771,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   781,    -1,
     630,   631,    -1,    11,   140,   141,    -1,    -1,    31,   145,
      -1,   641,   642,    36,   644,    23,    -1,    -1,   648,   802,
     156,    29,    -1,    -1,   654,    -1,   656,   163,    -1,    -1,
      -1,    -1,    55,    -1,   170,    -1,    44,   173,    -1,   669,
      -1,    -1,    -1,    -1,    -1,    -1,   676,    -1,    -1,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    -1,   713,   714,    -1,    -1,   110,   111,    -1,
      -1,    -1,    -1,   723,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   743,    -1,    -1,    -1,   140,   141,    -1,
      -1,    -1,   145,   753,    -1,   133,    -1,   757,    -1,    -1,
      -1,    -1,   762,   156,    -1,    -1,   766,    -1,   768,   769,
     163,    -1,    -1,    -1,   774,     1,    -1,    -1,    -1,     5,
     173,     7,     8,     9,    -1,    11,    -1,    13,    14,    15,
      16,   169,    -1,    -1,    -1,    -1,   796,    -1,    24,    25,
      26,    27,    -1,    -1,   804,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    47,    -1,    49,    50,    -1,    -1,    53,    54,    -1,
      -1,    -1,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    84,    85,
      86,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,   150,    -1,    -1,    -1,    -1,   155,
     156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,   167,   168,   169,   170,   171,   172,     1,    -1,    -1,
      -1,     5,    -1,     7,     8,     9,    -1,    11,    -1,    13,
      14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    47,    -1,    49,    50,    -1,    -1,    53,
      54,    -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    78,    79,    80,    -1,    -1,    83,
      84,    85,    86,    -1,    -1,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,   116,    -1,    -1,    -1,    36,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,    -1,    -1,    -1,
      -1,   155,   156,    -1,    -1,    -1,    -1,    13,    14,    15,
      16,   165,    -1,   167,   168,   169,   170,   171,   172,    25,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      36,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,   111,    -1,    -1,    50,    -1,    -1,    -1,    -1,    55,
      -1,    -1,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,   141,    78,    79,    80,   145,    -1,    83,    84,    85,
      86,    -1,    -1,    89,    -1,    -1,   156,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   101,   102,    -1,    -1,    -1,
     170,    -1,    -1,   173,    -1,   111,    31,    -1,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      55,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,   145,
      -1,    -1,   148,   149,   150,    -1,    -1,    -1,    -1,   155,
     156,    -1,    13,    14,    15,    16,    -1,   163,   164,   165,
      -1,   167,   168,   169,    25,   171,    -1,   173,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,   111,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    55,    -1,    -1,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,   141,    78,    79,    80,
     145,    -1,    83,    84,    85,    86,    -1,    -1,    89,    -1,
      -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,   145,    -1,    -1,   148,   149,   150,
      -1,    -1,    -1,    -1,   155,   156,    -1,    13,    14,    15,
      16,    -1,   163,    -1,   165,    -1,   167,   168,   169,    25,
     171,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    84,    85,
      86,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,   150,    13,    14,    15,    16,   155,
     156,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,   165,
      -1,   167,   168,   169,   170,   171,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    -1,
      78,    79,    80,    -1,    -1,    83,    84,    85,    86,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,    -1,    -1,    -1,    -1,   155,   156,    -1,
      13,    14,    15,    16,    -1,   163,    -1,   165,   166,   167,
     168,   169,    25,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    -1,    78,    79,    80,    -1,    -1,
      83,    84,    85,    86,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      14,    15,    16,    17,    -1,   148,   149,   150,    -1,    -1,
      -1,    25,   155,   156,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    37,   167,   168,   169,    41,   171,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    78,    79,    80,    -1,    -1,    83,
      84,    85,    86,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,
      15,    16,   146,    -1,   148,   149,   150,    -1,    -1,    -1,
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
      16,    -1,    -1,   148,   149,   150,    -1,    -1,    -1,    25,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,
     165,    37,   167,   168,   169,    41,   171,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    84,    85,
      86,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,   150,    13,    14,    15,    16,   155,
     156,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,   165,
      -1,   167,   168,   169,    -1,   171,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
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
     148,   149,   150,    -1,    -1,    -1,    25,   155,   156,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    37,   167,
     168,   169,    41,   171,    43,    -1,    -1,    46,    -1,    -1,
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
     149,   150,    13,    14,    15,    16,   155,   156,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,   165,   166,   167,   168,
     169,    -1,   171,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    -1,    78,    79,    80,
      -1,    -1,    83,    84,    85,    86,    -1,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    14,    15,    16,    -1,    -1,   148,   149,   150,
      -1,    -1,    -1,    25,   155,   156,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    37,   167,   168,   169,    41,
     171,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    79,    80,    -1,
      -1,    83,    84,    85,    86,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,    13,
      14,    15,    16,   155,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,   165,   166,   167,   168,   169,    -1,   171,
      -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    78,    79,    80,    -1,    -1,    83,
      84,    85,    86,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,    13,    14,    15,
      16,   155,   156,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,   165,   166,   167,   168,   169,    -1,   171,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    84,    85,
      86,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,   150,    13,    14,    15,    16,   155,
     156,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,   165,
     166,   167,   168,   169,    -1,   171,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    -1,
      78,    79,    80,    -1,    -1,    83,    84,    85,    86,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,
     148,   149,   150,    -1,    -1,    -1,    25,   155,   156,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    37,   167,
     168,   169,    41,   171,    43,    -1,    -1,    46,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,    13,    14,    15,    16,   155,   156,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,   165,   166,   167,   168,   169,
      -1,   171,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    79,    80,    -1,
      -1,    83,    84,    85,    86,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,    13,
      14,    15,    16,   155,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,   165,   166,   167,   168,   169,    -1,   171,
      -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    78,    79,    80,    -1,    -1,    83,
      84,    85,    86,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   148,   149,   150,    -1,    -1,    -1,    25,   155,
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
     141,    11,    -1,    -1,   145,    -1,   147,    19,    20,    -1,
      -1,    -1,    -1,    23,    -1,   156,    -1,   155,   156,    29,
      -1,    -1,   163,    -1,    -1,    -1,    -1,   165,    -1,   167,
     168,   169,   173,   171,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    19,    20,   117,   118,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,   170,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    19,
      20,   117,   118,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    19,    20,   117,   118,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    19,    20,   117,   118,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    19,    20,   117,
     118,   119,   120,   121,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,   163,    -1,    -1,    -1,    -1,
      -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    20,    -1,    -1,    -1,    -1,    -1,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,   161,
      -1,   163,    99,   100,   101,   102,   103,    -1,   170,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,    -1,    -1,
     117,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,    -1,    -1,    19,
      20,    -1,    -1,    -1,   161,    -1,   163,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    99,   100,   101,   102,   103,    -1,
      -1,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,    99,
     100,   101,   102,   103,    -1,   170,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,    -1,    -1,    19,    20,    -1,    -1,
      -1,   161,    -1,   163,    -1,    -1,   166,    -1,    99,   100,
     101,   102,   103,    -1,    -1,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,    -1,    -1,   117,    -1,   119,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,
     161,    -1,   163,    -1,    -1,   166,    99,   100,   101,   102,
     103,    -1,    -1,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      -1,    -1,    19,    20,    -1,    -1,    -1,    -1,   161,    -1,
     163,    -1,    -1,   166,    99,   100,   101,   102,   103,    -1,
      -1,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,    -1,
      19,    20,    -1,    -1,    -1,    -1,   161,    -1,   163,    -1,
      -1,   166,    99,   100,   101,   102,   103,    -1,    -1,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,    -1,    -1,
     117,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,    -1,    19,    20,
      -1,    -1,    -1,    -1,   161,    -1,   163,    -1,    -1,   166,
      99,   100,   101,   102,   103,    -1,    -1,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,   163,    -1,    -1,   166,    99,   100,
     101,   102,   103,    -1,    -1,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,    -1,    -1,   117,    -1,   119,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    29,    -1,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,   163,    -1,    -1,   166,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,   124,   125,   126,    -1,    -1,    -1,    -1,
      -1,    -1,   133,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    -1,    -1,   169,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,   103,    -1,
      -1,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    29,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,   161,    -1,   163,    -1,
      -1,   165,    -1,    -1,    -1,   169,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    19,    20,   169,    99,
     100,   101,   102,   103,    -1,    -1,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
      -1,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,   163,   164,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    99,   100,   101,   102,
     103,    -1,    -1,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
     163,   164,    19,    20,    -1,    99,   100,   101,   102,   103,
      -1,    -1,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,
     164,    -1,    -1,    -1,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,   141,   163,    -1,    -1,   145,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,
      -1,    19,    20,    -1,    -1,    -1,    -1,   173,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
     163,    99,   100,   101,   102,   103,    -1,    -1,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,    -1,    -1,   117,
      -1,   119,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,   163,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,   103,    -1,    -1,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,   163,    99,   100,   101,   102,   103,
      -1,    -1,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,   141,    -1,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,   103,    -1,
      -1,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,    99,
     100,   101,   102,   103,    -1,    -1,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,   163,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,   103,    -1,    -1,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,    -1,    -1,   117,    -1,   119,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    31,    -1,    -1,    -1,    36,    36,    19,
      20,    -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,
     151,   152,    -1,    -1,    -1,    -1,    55,    55,    -1,    -1,
     161,    -1,   163,    99,   100,    -1,    -1,   103,    -1,    -1,
     106,   107,   108,   109,    31,   111,   112,   113,   114,    36,
      -1,   117,    31,   119,   120,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    55,   143,   144,   145,
     146,   147,   111,   111,    -1,   151,   152,    -1,    -1,    99,
     100,   101,   102,   103,    -1,   161,   106,   163,    -1,   109,
      -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
     120,    -1,   141,   141,    -1,    -1,   145,   145,   147,   147,
      -1,    -1,    -1,    -1,   111,    31,    31,   156,   156,    -1,
      36,    36,   111,    -1,   163,   163,   146,   147,   148,   149,
     150,   151,   152,    -1,   173,   173,    -1,    -1,    -1,    55,
      55,   161,    -1,   163,   141,    -1,    -1,    -1,   145,    -1,
     147,    -1,   141,    -1,    -1,    31,   145,    -1,   147,   156,
      36,    -1,    -1,    31,    -1,    -1,   163,   156,    36,    -1,
      -1,    -1,    -1,    -1,   163,    31,   173,    -1,    -1,    55,
      36,    -1,    -1,    -1,   173,    -1,    -1,    55,    31,    -1,
      -1,    -1,    -1,    36,    -1,   111,   111,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    31,    31,    -1,    -1,    -1,    36,    36,
      -1,    -1,    -1,    -1,    -1,   141,   141,    -1,    -1,   145,
     145,    -1,   147,    -1,    -1,   111,    -1,    55,    55,    -1,
     156,   156,    -1,   111,    -1,    -1,    -1,   163,   163,   165,
      -1,    31,    -1,    -1,    -1,   111,    36,   173,   173,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,   111,   145,
      -1,   147,    -1,   141,    -1,    55,    -1,   145,    -1,   147,
     156,    -1,    -1,    -1,    -1,   141,    -1,   163,   156,   145,
      -1,   147,    -1,   111,   111,   163,    -1,   173,   141,    -1,
     156,    31,   145,    -1,   147,   173,    36,   163,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,    -1,   173,    -1,    -1,
     163,    -1,    -1,   141,   141,    55,    -1,   145,   145,   147,
     173,   111,    -1,    -1,    -1,    -1,    -1,    -1,   156,   156,
      -1,    -1,    -1,    -1,    -1,   163,   163,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,   173,   173,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,
      -1,   111,    -1,   163,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,    -1,   155,   156
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   175,     0,     5,    28,    30,    32,    34,    39,    45,
      51,    67,    88,   163,   170,   176,   181,   182,   186,   205,
     209,   223,   255,   260,   261,   264,   274,   276,   133,   200,
     201,   127,   187,   188,   133,   183,   184,   133,   171,   133,
     277,   275,    32,   133,   169,   202,   203,   204,   214,     4,
       6,    56,   254,   140,   139,   142,   139,    20,    52,   151,
     161,   185,   142,   171,   133,   258,   259,   133,   133,   169,
     133,   165,   139,   164,   133,   263,    33,   101,   102,   133,
     208,   171,   256,    13,    14,   127,   131,   133,   134,   165,
     178,   200,   127,   188,   133,   133,   133,    62,    66,    83,
      84,    85,    86,   268,   257,   140,   172,   258,   171,   171,
     133,   201,   203,   142,   262,   171,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   119,   120,   121,   122,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   155,   156,
      33,    33,   165,   206,   253,     1,   135,   177,    13,    14,
     127,   131,   133,   178,   198,   199,   185,   171,   133,   172,
      23,    29,    44,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,   214,   267,   269,   270,   272,
     273,   133,   271,   133,   246,   247,   166,   214,   241,   163,
     166,   238,   242,   243,   142,   207,   133,   172,   252,   265,
     170,   135,   136,   139,   166,   257,   140,   170,   146,   146,
     146,   146,   165,   146,   146,   146,   146,   146,   146,    31,
      36,    55,   111,   141,   145,   156,   163,   170,   173,   170,
     142,   170,   172,   238,   240,   201,    45,   249,   265,   166,
     170,   273,   171,   210,   139,   142,   145,   251,   198,   172,
      13,    14,    15,    16,    25,    37,    41,    43,    46,    50,
      67,    89,   101,   102,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   134,   148,   149,   150,   155,   156,   165,
     167,   168,   180,   210,   214,   216,   225,   226,   229,   230,
     231,   234,   235,   236,   237,   267,   278,   281,   286,   288,
     290,   273,   273,   273,   273,   133,   271,   206,   273,   206,
     273,   206,   273,   133,   244,   245,   273,   247,    31,   145,
     163,   173,   164,   237,   133,   172,   273,   172,   246,    42,
     239,   170,   164,   249,   110,   140,   142,   248,   251,   242,
     212,   133,   273,   110,   118,   140,   250,   273,   278,   165,
     165,   146,   146,   165,   146,   146,   237,   237,    11,   273,
      11,   273,   237,   283,   287,   179,   237,   237,   237,   237,
     237,   237,   168,   133,   165,   205,    19,    20,    99,   100,
     101,   102,   103,   106,   107,   108,   109,   111,   112,   113,
     114,   117,   119,   120,   141,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   161,   163,   170,   165,   147,
     170,   147,   147,   166,   147,   170,   207,   147,   207,   147,
     207,   147,   142,   147,   170,   147,   170,   145,   164,   164,
     249,   273,   116,   213,   232,   237,     1,     7,     8,     9,
      11,    24,    26,    27,    36,    40,    47,    49,    53,    54,
      90,   170,   172,   189,   190,   193,   194,   195,   196,   197,
     211,   213,   215,   217,   218,   219,   220,   221,   223,   224,
     232,   233,   237,   170,   250,   213,   237,   165,   133,   237,
     273,   273,   237,   273,   273,   133,   266,   133,   156,   164,
     165,   237,   279,   280,   282,   283,   284,   285,   266,   165,
     280,   285,   123,   170,   172,   136,   137,   177,   166,   146,
     214,   163,   166,   227,   237,   206,    17,   133,   133,   237,
     237,   237,   237,   237,   237,   133,   237,   133,   237,   237,
     237,   237,   237,    20,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   133,   237,   166,   227,   273,
     147,   147,   147,   273,   244,   248,   168,   230,   115,    91,
      92,    93,    94,    95,    96,    97,    98,   104,   105,   110,
     118,   121,   122,   140,   237,   266,   110,   170,   213,   237,
     210,   237,   237,   170,   127,    53,   237,   110,   213,   237,
      35,   170,   170,   237,   170,   170,   170,    18,   222,   170,
     110,   140,   213,   237,   170,   166,   227,    17,   146,   237,
     166,   147,   147,   166,   147,   147,    18,   139,   248,   164,
     166,   139,   164,   170,   139,   164,   170,    18,   166,   172,
     172,   237,   283,   172,   237,   206,   273,   133,   279,   139,
     166,   207,   273,   165,   164,   133,   142,   164,   166,   147,
     237,   230,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   210,    18,   213,
     237,   170,    12,   210,   142,   127,   213,   237,   170,   171,
     210,    38,   252,   116,   116,   170,   166,   273,   133,   166,
     237,   237,   237,   165,   227,   133,   237,   280,   133,   164,
     279,   237,   164,   284,   227,   280,   164,   164,   138,   207,
     147,   164,   237,   123,   210,   228,   166,   227,   237,   227,
     170,   210,   170,   212,    10,    21,    22,   191,   192,   237,
     237,   166,   147,   170,   166,   237,   170,   164,   248,   170,
     172,   147,   214,   166,   110,   237,   166,   170,   172,   210,
     237,    17,   237,   133,   166,   237,   164,   237,   237,   164,
     214,   237,   210,   210,   273,   166,   147,   170,   289,   289,
     192,   166,    17,   237,    48,   164,   172,   273,   166,   237,
     164,   164,   166
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
     225,   225,   225,   226,   226,   226,   226,   226,   226,   227,
     227,   228,   228,   228,   229,   229,   230,   230,   231,   231,
     231,   231,   231,   231,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     233,   233,   234,   235,   235,   236,   236,   236,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   238,   238,   239,   239,   240,   241,   241,   242,   242,
     243,   243,   244,   244,   245,   245,   246,   247,   247,   248,
     248,   249,   249,   249,   249,   249,   250,   250,   250,   251,
     251,   252,   252,   252,   252,   252,   253,   253,   254,   254,
     255,   256,   255,   257,   257,   257,   258,   259,   259,   260,
     261,   261,   262,   262,   263,   264,   265,   265,   266,   266,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   268,   268,   268,   268,   268,
     268,   269,   270,   270,   271,   271,   272,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     275,   274,   277,   276,   278,   278,   278,   278,   279,   279,
     280,   280,   281,   281,   281,   281,   281,   282,   282,   283,
     284,   284,   285,   285,   286,   286,   287,   287,   288,   289,
     289,   290,   290
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
       5,     5,     5,     5,     8,    10,     6,     9,    11,     1,
       3,     1,     2,     3,     1,     1,     1,     5,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     4,     6,     5,     6,     3,     6,     7,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     3,     4,     4,     3,     3,     1,     3,     4,     3,
       4,     2,     4,     4,     6,     7,     3,     5,     4,     3,
       3,     4,     1,     1,     1,     1,     1,     1,     3,     3,
       2,     0,     3,     0,     1,     3,     0,     3,     2,     3,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       1,     2,     3,     5,     3,     3,     1,     1,     1,     0,
       1,     4,     6,     5,     5,     4,     0,     2,     0,     1,
       5,     0,     5,     0,     3,     5,     4,     1,     2,     4,
       5,     7,     0,     2,     2,     6,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     1,     3,     4,     1,     1,     1,
       1,     4,     3,     4,     2,     3,     2,     4,     3,     2,
       2,     3,     2,     2,     4,     2,     4,     6,     4,     1,
       4,     5,     1,     4,     5,     1,     4,     5,     4,     4,
       0,     7,     0,     7,     1,     1,     1,     1,     3,     5,
       1,     3,     5,     4,     7,     5,     7,     1,     3,     3,
       1,     1,     1,     3,     5,     5,     1,     3,     4,     0,
       3,    10,    10
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
#line 3219 "ds_parser.cpp"
        break;

    case 177: /* character_sequence  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3225 "ds_parser.cpp"
        break;

    case 178: /* string_constant  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3231 "ds_parser.cpp"
        break;

    case 179: /* string_builder_body  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3237 "ds_parser.cpp"
        break;

    case 180: /* string_builder  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3243 "ds_parser.cpp"
        break;

    case 183: /* require_module_name  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3249 "ds_parser.cpp"
        break;

    case 189: /* expression_label  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3255 "ds_parser.cpp"
        break;

    case 190: /* expression_goto  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3261 "ds_parser.cpp"
        break;

    case 192: /* expression_else  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3267 "ds_parser.cpp"
        break;

    case 194: /* expression_if_then_else  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3273 "ds_parser.cpp"
        break;

    case 195: /* expression_for_loop  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3279 "ds_parser.cpp"
        break;

    case 196: /* expression_while_loop  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3285 "ds_parser.cpp"
        break;

    case 197: /* expression_with  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3291 "ds_parser.cpp"
        break;

    case 198: /* annotation_argument_value  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3297 "ds_parser.cpp"
        break;

    case 199: /* annotation_argument_value_list  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3303 "ds_parser.cpp"
        break;

    case 200: /* annotation_argument  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3309 "ds_parser.cpp"
        break;

    case 201: /* annotation_argument_list  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3315 "ds_parser.cpp"
        break;

    case 202: /* annotation_declaration_name  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3321 "ds_parser.cpp"
        break;

    case 203: /* annotation_declaration  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3327 "ds_parser.cpp"
        break;

    case 204: /* annotation_list  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3333 "ds_parser.cpp"
        break;

    case 205: /* optional_annotation_list  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3339 "ds_parser.cpp"
        break;

    case 206: /* optional_function_argument_list  */
#line 113 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3345 "ds_parser.cpp"
        break;

    case 207: /* optional_function_type  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3351 "ds_parser.cpp"
        break;

    case 208: /* function_name  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3357 "ds_parser.cpp"
        break;

    case 210: /* expression_block  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3363 "ds_parser.cpp"
        break;

    case 211: /* expression_any  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3369 "ds_parser.cpp"
        break;

    case 212: /* expressions  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3375 "ds_parser.cpp"
        break;

    case 213: /* expr_pipe  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3381 "ds_parser.cpp"
        break;

    case 214: /* name_in_namespace  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3387 "ds_parser.cpp"
        break;

    case 215: /* expression_delete  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3393 "ds_parser.cpp"
        break;

    case 216: /* expr_new  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3399 "ds_parser.cpp"
        break;

    case 217: /* expression_break  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3405 "ds_parser.cpp"
        break;

    case 218: /* expression_continue  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3411 "ds_parser.cpp"
        break;

    case 219: /* expression_return  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3417 "ds_parser.cpp"
        break;

    case 220: /* expression_yield  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3423 "ds_parser.cpp"
        break;

    case 221: /* expression_try_catch  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3429 "ds_parser.cpp"
        break;

    case 224: /* expression_let  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3435 "ds_parser.cpp"
        break;

    case 225: /* expr_cast  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3441 "ds_parser.cpp"
        break;

    case 226: /* expr_type_info  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3447 "ds_parser.cpp"
        break;

    case 227: /* expr_list  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3453 "ds_parser.cpp"
        break;

    case 228: /* block_or_simple_block  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3459 "ds_parser.cpp"
        break;

    case 230: /* expr_block  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3465 "ds_parser.cpp"
        break;

    case 231: /* expr_numeric_const  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3471 "ds_parser.cpp"
        break;

    case 232: /* expr_assign  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3477 "ds_parser.cpp"
        break;

    case 233: /* expr_assign_pipe  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3483 "ds_parser.cpp"
        break;

    case 234: /* expr_named_call  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3489 "ds_parser.cpp"
        break;

    case 235: /* expr_method_call  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3495 "ds_parser.cpp"
        break;

    case 236: /* func_addr_expr  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3501 "ds_parser.cpp"
        break;

    case 237: /* expr  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3507 "ds_parser.cpp"
        break;

    case 238: /* optional_field_annotation  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3513 "ds_parser.cpp"
        break;

    case 240: /* structure_variable_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3519 "ds_parser.cpp"
        break;

    case 241: /* struct_variable_declaration_list  */
#line 113 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3525 "ds_parser.cpp"
        break;

    case 242: /* function_argument_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3531 "ds_parser.cpp"
        break;

    case 243: /* function_argument_list  */
#line 113 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3537 "ds_parser.cpp"
        break;

    case 244: /* tuple_type  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3543 "ds_parser.cpp"
        break;

    case 245: /* tuple_type_list  */
#line 113 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3549 "ds_parser.cpp"
        break;

    case 246: /* variant_type  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3555 "ds_parser.cpp"
        break;

    case 247: /* variant_type_list  */
#line 113 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3561 "ds_parser.cpp"
        break;

    case 249: /* variable_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3567 "ds_parser.cpp"
        break;

    case 252: /* let_variable_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3573 "ds_parser.cpp"
        break;

    case 253: /* global_variable_declaration_list  */
#line 113 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3579 "ds_parser.cpp"
        break;

    case 257: /* enum_list  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3585 "ds_parser.cpp"
        break;

    case 262: /* optional_structure_parent  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3591 "ds_parser.cpp"
        break;

    case 265: /* variable_name_list  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3597 "ds_parser.cpp"
        break;

    case 266: /* variable_name_with_pos_list  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3603 "ds_parser.cpp"
        break;

    case 269: /* structure_type_declaration  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3609 "ds_parser.cpp"
        break;

    case 270: /* auto_type_declaration  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3615 "ds_parser.cpp"
        break;

    case 271: /* bitfield_bits  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3621 "ds_parser.cpp"
        break;

    case 272: /* bitfield_type_declaration  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3627 "ds_parser.cpp"
        break;

    case 273: /* type_declaration  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3633 "ds_parser.cpp"
        break;

    case 278: /* make_decl  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3639 "ds_parser.cpp"
        break;

    case 279: /* make_struct_fields  */
#line 120 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3645 "ds_parser.cpp"
        break;

    case 280: /* make_struct_dim  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3651 "ds_parser.cpp"
        break;

    case 281: /* make_struct_decl  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3657 "ds_parser.cpp"
        break;

    case 282: /* make_tuple  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3663 "ds_parser.cpp"
        break;

    case 283: /* make_map_tuple  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3669 "ds_parser.cpp"
        break;

    case 284: /* make_any_tuple  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3675 "ds_parser.cpp"
        break;

    case 285: /* make_dim  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3681 "ds_parser.cpp"
        break;

    case 286: /* make_dim_decl  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3687 "ds_parser.cpp"
        break;

    case 287: /* make_table  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3693 "ds_parser.cpp"
        break;

    case 288: /* make_table_decl  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3699 "ds_parser.cpp"
        break;

    case 289: /* array_comprehension_where  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3705 "ds_parser.cpp"
        break;

    case 290: /* array_comprehension  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3711 "ds_parser.cpp"
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
#line 414 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 4011 "ds_parser.cpp"
    break;

  case 16:
#line 426 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4017 "ds_parser.cpp"
    break;

  case 17:
#line 427 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4023 "ds_parser.cpp"
    break;

  case 18:
#line 431 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4029 "ds_parser.cpp"
    break;

  case 19:
#line 435 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4039 "ds_parser.cpp"
    break;

  case 20:
#line 440 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4053 "ds_parser.cpp"
    break;

  case 21:
#line 449 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4063 "ds_parser.cpp"
    break;

  case 22:
#line 457 "ds_parser.ypp"
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
#line 4081 "ds_parser.cpp"
    break;

  case 23:
#line 473 "ds_parser.ypp"
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
#line 4105 "ds_parser.cpp"
    break;

  case 25:
#line 499 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4113 "ds_parser.cpp"
    break;

  case 26:
#line 502 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4124 "ds_parser.cpp"
    break;

  case 27:
#line 508 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4135 "ds_parser.cpp"
    break;

  case 28:
#line 517 "ds_parser.ypp"
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
#line 4161 "ds_parser.cpp"
    break;

  case 29:
#line 538 "ds_parser.ypp"
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
#line 4186 "ds_parser.cpp"
    break;

  case 30:
#line 561 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4192 "ds_parser.cpp"
    break;

  case 31:
#line 562 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4198 "ds_parser.cpp"
    break;

  case 35:
#line 575 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4206 "ds_parser.cpp"
    break;

  case 36:
#line 578 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4214 "ds_parser.cpp"
    break;

  case 37:
#line 584 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4222 "ds_parser.cpp"
    break;

  case 38:
#line 590 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4230 "ds_parser.cpp"
    break;

  case 39:
#line 593 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4238 "ds_parser.cpp"
    break;

  case 40:
#line 599 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4244 "ds_parser.cpp"
    break;

  case 41:
#line 600 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4250 "ds_parser.cpp"
    break;

  case 42:
#line 604 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4256 "ds_parser.cpp"
    break;

  case 43:
#line 605 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4262 "ds_parser.cpp"
    break;

  case 44:
#line 606 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4273 "ds_parser.cpp"
    break;

  case 45:
#line 615 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4279 "ds_parser.cpp"
    break;

  case 46:
#line 616 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4285 "ds_parser.cpp"
    break;

  case 47:
#line 620 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4296 "ds_parser.cpp"
    break;

  case 48:
#line 629 "ds_parser.ypp"
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
#line 4313 "ds_parser.cpp"
    break;

  case 49:
#line 644 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4325 "ds_parser.cpp"
    break;

  case 50:
#line 654 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4336 "ds_parser.cpp"
    break;

  case 51:
#line 663 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4342 "ds_parser.cpp"
    break;

  case 52:
#line 664 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4348 "ds_parser.cpp"
    break;

  case 53:
#line 665 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4354 "ds_parser.cpp"
    break;

  case 54:
#line 666 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4360 "ds_parser.cpp"
    break;

  case 55:
#line 667 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4366 "ds_parser.cpp"
    break;

  case 56:
#line 668 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4372 "ds_parser.cpp"
    break;

  case 57:
#line 672 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4382 "ds_parser.cpp"
    break;

  case 58:
#line 677 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4392 "ds_parser.cpp"
    break;

  case 59:
#line 685 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4398 "ds_parser.cpp"
    break;

  case 60:
#line 686 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4404 "ds_parser.cpp"
    break;

  case 61:
#line 687 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4410 "ds_parser.cpp"
    break;

  case 62:
#line 688 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4416 "ds_parser.cpp"
    break;

  case 63:
#line 689 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4422 "ds_parser.cpp"
    break;

  case 64:
#line 690 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4428 "ds_parser.cpp"
    break;

  case 65:
#line 691 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4434 "ds_parser.cpp"
    break;

  case 66:
#line 692 "ds_parser.ypp"
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4442 "ds_parser.cpp"
    break;

  case 67:
#line 698 "ds_parser.ypp"
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
#line 4460 "ds_parser.cpp"
    break;

  case 68:
#line 711 "ds_parser.ypp"
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
#line 4478 "ds_parser.cpp"
    break;

  case 69:
#line 727 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4484 "ds_parser.cpp"
    break;

  case 70:
#line 728 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4490 "ds_parser.cpp"
    break;

  case 71:
#line 732 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4502 "ds_parser.cpp"
    break;

  case 72:
#line 739 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4516 "ds_parser.cpp"
    break;

  case 73:
#line 751 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4525 "ds_parser.cpp"
    break;

  case 74:
#line 755 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4534 "ds_parser.cpp"
    break;

  case 75:
#line 762 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4540 "ds_parser.cpp"
    break;

  case 76:
#line 763 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4546 "ds_parser.cpp"
    break;

  case 77:
#line 767 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4552 "ds_parser.cpp"
    break;

  case 78:
#line 768 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4558 "ds_parser.cpp"
    break;

  case 79:
#line 769 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4564 "ds_parser.cpp"
    break;

  case 80:
#line 773 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4574 "ds_parser.cpp"
    break;

  case 81:
#line 778 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4583 "ds_parser.cpp"
    break;

  case 82:
#line 785 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s);
	}
#line 4592 "ds_parser.cpp"
    break;

  case 83:
#line 789 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4598 "ds_parser.cpp"
    break;

  case 84:
#line 790 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4604 "ds_parser.cpp"
    break;

  case 85:
#line 791 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4610 "ds_parser.cpp"
    break;

  case 86:
#line 792 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4616 "ds_parser.cpp"
    break;

  case 87:
#line 793 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4622 "ds_parser.cpp"
    break;

  case 88:
#line 794 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4628 "ds_parser.cpp"
    break;

  case 89:
#line 795 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4634 "ds_parser.cpp"
    break;

  case 90:
#line 796 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4640 "ds_parser.cpp"
    break;

  case 91:
#line 797 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4646 "ds_parser.cpp"
    break;

  case 92:
#line 798 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4652 "ds_parser.cpp"
    break;

  case 93:
#line 799 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4658 "ds_parser.cpp"
    break;

  case 94:
#line 800 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4664 "ds_parser.cpp"
    break;

  case 95:
#line 801 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4670 "ds_parser.cpp"
    break;

  case 96:
#line 802 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4676 "ds_parser.cpp"
    break;

  case 97:
#line 803 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4682 "ds_parser.cpp"
    break;

  case 98:
#line 804 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4688 "ds_parser.cpp"
    break;

  case 99:
#line 805 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4694 "ds_parser.cpp"
    break;

  case 100:
#line 806 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4700 "ds_parser.cpp"
    break;

  case 101:
#line 807 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4706 "ds_parser.cpp"
    break;

  case 102:
#line 808 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4712 "ds_parser.cpp"
    break;

  case 103:
#line 809 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4718 "ds_parser.cpp"
    break;

  case 104:
#line 810 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4724 "ds_parser.cpp"
    break;

  case 105:
#line 811 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4730 "ds_parser.cpp"
    break;

  case 106:
#line 812 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4736 "ds_parser.cpp"
    break;

  case 107:
#line 813 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4742 "ds_parser.cpp"
    break;

  case 108:
#line 814 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4748 "ds_parser.cpp"
    break;

  case 109:
#line 815 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4754 "ds_parser.cpp"
    break;

  case 110:
#line 816 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4760 "ds_parser.cpp"
    break;

  case 111:
#line 817 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4766 "ds_parser.cpp"
    break;

  case 112:
#line 818 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4772 "ds_parser.cpp"
    break;

  case 113:
#line 819 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4778 "ds_parser.cpp"
    break;

  case 114:
#line 820 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4784 "ds_parser.cpp"
    break;

  case 115:
#line 821 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4790 "ds_parser.cpp"
    break;

  case 116:
#line 822 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4796 "ds_parser.cpp"
    break;

  case 117:
#line 823 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4802 "ds_parser.cpp"
    break;

  case 118:
#line 824 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4808 "ds_parser.cpp"
    break;

  case 119:
#line 829 "ds_parser.ypp"
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
#line 4890 "ds_parser.cpp"
    break;

  case 120:
#line 909 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4898 "ds_parser.cpp"
    break;

  case 121:
#line 912 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4910 "ds_parser.cpp"
    break;

  case 122:
#line 922 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4916 "ds_parser.cpp"
    break;

  case 123:
#line 923 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4922 "ds_parser.cpp"
    break;

  case 124:
#line 924 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4928 "ds_parser.cpp"
    break;

  case 125:
#line 925 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4934 "ds_parser.cpp"
    break;

  case 126:
#line 926 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4940 "ds_parser.cpp"
    break;

  case 127:
#line 927 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4946 "ds_parser.cpp"
    break;

  case 128:
#line 928 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4952 "ds_parser.cpp"
    break;

  case 129:
#line 929 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4958 "ds_parser.cpp"
    break;

  case 130:
#line 930 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4964 "ds_parser.cpp"
    break;

  case 131:
#line 931 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4970 "ds_parser.cpp"
    break;

  case 132:
#line 932 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4976 "ds_parser.cpp"
    break;

  case 133:
#line 933 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4982 "ds_parser.cpp"
    break;

  case 134:
#line 934 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4988 "ds_parser.cpp"
    break;

  case 135:
#line 935 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4994 "ds_parser.cpp"
    break;

  case 136:
#line 936 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5000 "ds_parser.cpp"
    break;

  case 137:
#line 937 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5006 "ds_parser.cpp"
    break;

  case 138:
#line 938 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5012 "ds_parser.cpp"
    break;

  case 139:
#line 939 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5018 "ds_parser.cpp"
    break;

  case 140:
#line 943 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5028 "ds_parser.cpp"
    break;

  case 141:
#line 948 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5039 "ds_parser.cpp"
    break;

  case 142:
#line 954 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5047 "ds_parser.cpp"
    break;

  case 143:
#line 960 "ds_parser.ypp"
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
#line 5063 "ds_parser.cpp"
    break;

  case 144:
#line 971 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5071 "ds_parser.cpp"
    break;

  case 145:
#line 977 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5077 "ds_parser.cpp"
    break;

  case 146:
#line 978 "ds_parser.ypp"
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
#line 5093 "ds_parser.cpp"
    break;

  case 147:
#line 989 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5099 "ds_parser.cpp"
    break;

  case 148:
#line 993 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5107 "ds_parser.cpp"
    break;

  case 149:
#line 999 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5115 "ds_parser.cpp"
    break;

  case 150:
#line 1002 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5123 "ds_parser.cpp"
    break;

  case 151:
#line 1005 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5132 "ds_parser.cpp"
    break;

  case 152:
#line 1009 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5140 "ds_parser.cpp"
    break;

  case 153:
#line 1015 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5146 "ds_parser.cpp"
    break;

  case 154:
#line 1019 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5152 "ds_parser.cpp"
    break;

  case 155:
#line 1023 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5160 "ds_parser.cpp"
    break;

  case 156:
#line 1026 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5168 "ds_parser.cpp"
    break;

  case 157:
#line 1029 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5178 "ds_parser.cpp"
    break;

  case 158:
#line 1034 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5186 "ds_parser.cpp"
    break;

  case 159:
#line 1037 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5196 "ds_parser.cpp"
    break;

  case 160:
#line 1045 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5204 "ds_parser.cpp"
    break;

  case 161:
#line 1048 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5214 "ds_parser.cpp"
    break;

  case 162:
#line 1053 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5222 "ds_parser.cpp"
    break;

  case 163:
#line 1056 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5232 "ds_parser.cpp"
    break;

  case 164:
#line 1064 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5240 "ds_parser.cpp"
    break;

  case 165:
#line 1070 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5246 "ds_parser.cpp"
    break;

  case 166:
#line 1071 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5252 "ds_parser.cpp"
    break;

  case 167:
#line 1075 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5258 "ds_parser.cpp"
    break;

  case 168:
#line 1076 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5264 "ds_parser.cpp"
    break;

  case 169:
#line 1080 "ds_parser.ypp"
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
#line 5300 "ds_parser.cpp"
    break;

  case 170:
#line 1114 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5308 "ds_parser.cpp"
    break;

  case 171:
#line 1117 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5318 "ds_parser.cpp"
    break;

  case 172:
#line 1122 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5328 "ds_parser.cpp"
    break;

  case 173:
#line 1130 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5337 "ds_parser.cpp"
    break;

  case 174:
#line 1134 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5347 "ds_parser.cpp"
    break;

  case 175:
#line 1139 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5358 "ds_parser.cpp"
    break;

  case 176:
#line 1145 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 5367 "ds_parser.cpp"
    break;

  case 177:
#line 1149 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5377 "ds_parser.cpp"
    break;

  case 178:
#line 1154 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5388 "ds_parser.cpp"
    break;

  case 179:
#line 1163 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5396 "ds_parser.cpp"
    break;

  case 180:
#line 1166 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5404 "ds_parser.cpp"
    break;

  case 181:
#line 1172 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5410 "ds_parser.cpp"
    break;

  case 182:
#line 1173 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5422 "ds_parser.cpp"
    break;

  case 183:
#line 1180 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5435 "ds_parser.cpp"
    break;

  case 184:
#line 1191 "ds_parser.ypp"
    { (yyval.b) = false;   /* block */  }
#line 5441 "ds_parser.cpp"
    break;

  case 185:
#line 1192 "ds_parser.ypp"
    { (yyval.b) = true;    /* lambda */ }
#line 5447 "ds_parser.cpp"
    break;

  case 186:
#line 1196 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5457 "ds_parser.cpp"
    break;

  case 187:
#line 1202 "ds_parser.ypp"
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
#line 5513 "ds_parser.cpp"
    break;

  case 188:
#line 1256 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5519 "ds_parser.cpp"
    break;

  case 189:
#line 1257 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5525 "ds_parser.cpp"
    break;

  case 190:
#line 1258 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5531 "ds_parser.cpp"
    break;

  case 191:
#line 1259 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5537 "ds_parser.cpp"
    break;

  case 192:
#line 1260 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5543 "ds_parser.cpp"
    break;

  case 193:
#line 1261 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5549 "ds_parser.cpp"
    break;

  case 194:
#line 1265 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5555 "ds_parser.cpp"
    break;

  case 195:
#line 1266 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5561 "ds_parser.cpp"
    break;

  case 196:
#line 1267 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5567 "ds_parser.cpp"
    break;

  case 197:
#line 1268 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5573 "ds_parser.cpp"
    break;

  case 198:
#line 1269 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5579 "ds_parser.cpp"
    break;

  case 199:
#line 1270 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5585 "ds_parser.cpp"
    break;

  case 200:
#line 1271 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5591 "ds_parser.cpp"
    break;

  case 201:
#line 1272 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5597 "ds_parser.cpp"
    break;

  case 202:
#line 1273 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5603 "ds_parser.cpp"
    break;

  case 203:
#line 1274 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5609 "ds_parser.cpp"
    break;

  case 204:
#line 1275 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5615 "ds_parser.cpp"
    break;

  case 205:
#line 1276 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5621 "ds_parser.cpp"
    break;

  case 206:
#line 1277 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5627 "ds_parser.cpp"
    break;

  case 207:
#line 1278 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5633 "ds_parser.cpp"
    break;

  case 208:
#line 1279 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5639 "ds_parser.cpp"
    break;

  case 209:
#line 1280 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5645 "ds_parser.cpp"
    break;

  case 210:
#line 1284 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5651 "ds_parser.cpp"
    break;

  case 211:
#line 1285 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5657 "ds_parser.cpp"
    break;

  case 212:
#line 1289 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5669 "ds_parser.cpp"
    break;

  case 213:
#line 1300 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5679 "ds_parser.cpp"
    break;

  case 214:
#line 1305 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5691 "ds_parser.cpp"
    break;

  case 215:
#line 1315 "ds_parser.ypp"
    {
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
	}
#line 5700 "ds_parser.cpp"
    break;

  case 216:
#line 1319 "ds_parser.ypp"
    {
		auto expr = new ExprAddr(tokAt((yylsp[-4])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5712 "ds_parser.cpp"
    break;

  case 217:
#line 1326 "ds_parser.ypp"
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
#line 5728 "ds_parser.cpp"
    break;

  case 218:
#line 1340 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5734 "ds_parser.cpp"
    break;

  case 219:
#line 1341 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5740 "ds_parser.cpp"
    break;

  case 220:
#line 1342 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5746 "ds_parser.cpp"
    break;

  case 221:
#line 1343 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5752 "ds_parser.cpp"
    break;

  case 222:
#line 1344 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5758 "ds_parser.cpp"
    break;

  case 223:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5764 "ds_parser.cpp"
    break;

  case 224:
#line 1346 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5770 "ds_parser.cpp"
    break;

  case 225:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5776 "ds_parser.cpp"
    break;

  case 226:
#line 1348 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5782 "ds_parser.cpp"
    break;

  case 227:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5788 "ds_parser.cpp"
    break;

  case 228:
#line 1350 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5794 "ds_parser.cpp"
    break;

  case 229:
#line 1351 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5800 "ds_parser.cpp"
    break;

  case 230:
#line 1352 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5806 "ds_parser.cpp"
    break;

  case 231:
#line 1353 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5812 "ds_parser.cpp"
    break;

  case 232:
#line 1354 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5818 "ds_parser.cpp"
    break;

  case 233:
#line 1355 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5824 "ds_parser.cpp"
    break;

  case 234:
#line 1356 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5830 "ds_parser.cpp"
    break;

  case 235:
#line 1357 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5836 "ds_parser.cpp"
    break;

  case 236:
#line 1358 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5842 "ds_parser.cpp"
    break;

  case 237:
#line 1359 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5848 "ds_parser.cpp"
    break;

  case 238:
#line 1360 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5854 "ds_parser.cpp"
    break;

  case 239:
#line 1361 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5860 "ds_parser.cpp"
    break;

  case 240:
#line 1362 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5866 "ds_parser.cpp"
    break;

  case 241:
#line 1363 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5872 "ds_parser.cpp"
    break;

  case 242:
#line 1364 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5878 "ds_parser.cpp"
    break;

  case 243:
#line 1365 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5884 "ds_parser.cpp"
    break;

  case 244:
#line 1366 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5890 "ds_parser.cpp"
    break;

  case 245:
#line 1367 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5896 "ds_parser.cpp"
    break;

  case 246:
#line 1368 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5902 "ds_parser.cpp"
    break;

  case 247:
#line 1369 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5908 "ds_parser.cpp"
    break;

  case 248:
#line 1370 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5914 "ds_parser.cpp"
    break;

  case 249:
#line 1371 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5920 "ds_parser.cpp"
    break;

  case 250:
#line 1372 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5926 "ds_parser.cpp"
    break;

  case 251:
#line 1373 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5932 "ds_parser.cpp"
    break;

  case 252:
#line 1374 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5938 "ds_parser.cpp"
    break;

  case 253:
#line 1375 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5944 "ds_parser.cpp"
    break;

  case 254:
#line 1376 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5950 "ds_parser.cpp"
    break;

  case 255:
#line 1377 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5956 "ds_parser.cpp"
    break;

  case 256:
#line 1378 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5962 "ds_parser.cpp"
    break;

  case 257:
#line 1379 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5971 "ds_parser.cpp"
    break;

  case 258:
#line 1383 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5980 "ds_parser.cpp"
    break;

  case 259:
#line 1387 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5988 "ds_parser.cpp"
    break;

  case 260:
#line 1390 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5996 "ds_parser.cpp"
    break;

  case 261:
#line 1393 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6002 "ds_parser.cpp"
    break;

  case 262:
#line 1394 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6008 "ds_parser.cpp"
    break;

  case 263:
#line 1395 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6014 "ds_parser.cpp"
    break;

  case 264:
#line 1396 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6024 "ds_parser.cpp"
    break;

  case 265:
#line 1401 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6034 "ds_parser.cpp"
    break;

  case 266:
#line 1406 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6040 "ds_parser.cpp"
    break;

  case 267:
#line 1407 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6048 "ds_parser.cpp"
    break;

  case 268:
#line 1410 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 6056 "ds_parser.cpp"
    break;

  case 269:
#line 1413 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 6065 "ds_parser.cpp"
    break;

  case 270:
#line 1417 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 6074 "ds_parser.cpp"
    break;

  case 271:
#line 1421 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6083 "ds_parser.cpp"
    break;

  case 272:
#line 1425 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6089 "ds_parser.cpp"
    break;

  case 273:
#line 1426 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6095 "ds_parser.cpp"
    break;

  case 274:
#line 1427 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6101 "ds_parser.cpp"
    break;

  case 275:
#line 1428 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6107 "ds_parser.cpp"
    break;

  case 276:
#line 1429 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6113 "ds_parser.cpp"
    break;

  case 277:
#line 1430 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6119 "ds_parser.cpp"
    break;

  case 278:
#line 1431 "ds_parser.ypp"
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
#line 6134 "ds_parser.cpp"
    break;

  case 279:
#line 1441 "ds_parser.ypp"
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
#line 6149 "ds_parser.cpp"
    break;

  case 280:
#line 1451 "ds_parser.ypp"
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
#line 6206 "ds_parser.cpp"
    break;

  case 281:
#line 1506 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6212 "ds_parser.cpp"
    break;

  case 282:
#line 1507 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 6218 "ds_parser.cpp"
    break;

  case 283:
#line 1511 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6224 "ds_parser.cpp"
    break;

  case 284:
#line 1512 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6230 "ds_parser.cpp"
    break;

  case 285:
#line 1516 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6240 "ds_parser.cpp"
    break;

  case 286:
#line 1524 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6248 "ds_parser.cpp"
    break;

  case 287:
#line 1527 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6257 "ds_parser.cpp"
    break;

  case 288:
#line 1534 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
		}
#line 6267 "ds_parser.cpp"
    break;

  case 289:
#line 1539 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
		}
#line 6277 "ds_parser.cpp"
    break;

  case 290:
#line 1547 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6283 "ds_parser.cpp"
    break;

  case 291:
#line 1548 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6289 "ds_parser.cpp"
    break;

  case 292:
#line 1552 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6297 "ds_parser.cpp"
    break;

  case 293:
#line 1555 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6308 "ds_parser.cpp"
    break;

  case 294:
#line 1564 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6314 "ds_parser.cpp"
    break;

  case 295:
#line 1565 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6320 "ds_parser.cpp"
    break;

  case 296:
#line 1569 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6331 "ds_parser.cpp"
    break;

  case 297:
#line 1578 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6337 "ds_parser.cpp"
    break;

  case 298:
#line 1579 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6343 "ds_parser.cpp"
    break;

  case 299:
#line 1584 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6349 "ds_parser.cpp"
    break;

  case 300:
#line 1585 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6355 "ds_parser.cpp"
    break;

  case 301:
#line 1589 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = (yyvsp[0].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-1])),(yyvsp[-1].pNameList),autoT,nullptr);
    }
#line 6366 "ds_parser.cpp"
    break;

  case 302:
#line 1595 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6374 "ds_parser.cpp"
    break;

  case 303:
#line 1598 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6383 "ds_parser.cpp"
    break;

  case 304:
#line 1602 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6394 "ds_parser.cpp"
    break;

  case 305:
#line 1608 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6405 "ds_parser.cpp"
    break;

  case 306:
#line 1617 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6411 "ds_parser.cpp"
    break;

  case 307:
#line 1618 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6417 "ds_parser.cpp"
    break;

  case 308:
#line 1619 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6423 "ds_parser.cpp"
    break;

  case 309:
#line 1623 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6429 "ds_parser.cpp"
    break;

  case 310:
#line 1624 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6435 "ds_parser.cpp"
    break;

  case 311:
#line 1628 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6443 "ds_parser.cpp"
    break;

  case 312:
#line 1631 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6453 "ds_parser.cpp"
    break;

  case 313:
#line 1636 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6463 "ds_parser.cpp"
    break;

  case 314:
#line 1641 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6476 "ds_parser.cpp"
    break;

  case 315:
#line 1649 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6489 "ds_parser.cpp"
    break;

  case 316:
#line 1660 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6497 "ds_parser.cpp"
    break;

  case 317:
#line 1663 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6506 "ds_parser.cpp"
    break;

  case 318:
#line 1670 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6512 "ds_parser.cpp"
    break;

  case 319:
#line 1671 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6518 "ds_parser.cpp"
    break;

  case 320:
#line 1675 "ds_parser.ypp"
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
#line 6550 "ds_parser.cpp"
    break;

  case 321:
#line 1702 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 6559 "ds_parser.cpp"
    break;

  case 322:
#line 1705 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 6570 "ds_parser.cpp"
    break;

  case 323:
#line 1714 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6578 "ds_parser.cpp"
    break;

  case 324:
#line 1717 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6592 "ds_parser.cpp"
    break;

  case 325:
#line 1726 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6606 "ds_parser.cpp"
    break;

  case 326:
#line 1739 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 6620 "ds_parser.cpp"
    break;

  case 330:
#line 1760 "ds_parser.ypp"
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
#line 6636 "ds_parser.cpp"
    break;

  case 331:
#line 1771 "ds_parser.ypp"
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
#line 6653 "ds_parser.cpp"
    break;

  case 332:
#line 1786 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 6659 "ds_parser.cpp"
    break;

  case 333:
#line 1787 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 6665 "ds_parser.cpp"
    break;

  case 334:
#line 1791 "ds_parser.ypp"
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
#line 6706 "ds_parser.cpp"
    break;

  case 335:
#line 1830 "ds_parser.ypp"
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
#line 6774 "ds_parser.cpp"
    break;

  case 336:
#line 1896 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6786 "ds_parser.cpp"
    break;

  case 337:
#line 1903 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6797 "ds_parser.cpp"
    break;

  case 338:
#line 1912 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(pair<string,LineInfo>(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6809 "ds_parser.cpp"
    break;

  case 339:
#line 1919 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(pair<string,LineInfo>(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 6820 "ds_parser.cpp"
    break;

  case 340:
#line 1928 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 6826 "ds_parser.cpp"
    break;

  case 341:
#line 1929 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 6832 "ds_parser.cpp"
    break;

  case 342:
#line 1930 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6838 "ds_parser.cpp"
    break;

  case 343:
#line 1931 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6844 "ds_parser.cpp"
    break;

  case 344:
#line 1932 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6850 "ds_parser.cpp"
    break;

  case 345:
#line 1933 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 6856 "ds_parser.cpp"
    break;

  case 346:
#line 1934 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 6862 "ds_parser.cpp"
    break;

  case 347:
#line 1935 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 6868 "ds_parser.cpp"
    break;

  case 348:
#line 1936 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 6874 "ds_parser.cpp"
    break;

  case 349:
#line 1937 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6880 "ds_parser.cpp"
    break;

  case 350:
#line 1938 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6886 "ds_parser.cpp"
    break;

  case 351:
#line 1939 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6892 "ds_parser.cpp"
    break;

  case 352:
#line 1940 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 6898 "ds_parser.cpp"
    break;

  case 353:
#line 1941 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 6904 "ds_parser.cpp"
    break;

  case 354:
#line 1942 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 6910 "ds_parser.cpp"
    break;

  case 355:
#line 1943 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 6916 "ds_parser.cpp"
    break;

  case 356:
#line 1944 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 6922 "ds_parser.cpp"
    break;

  case 357:
#line 1945 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 6928 "ds_parser.cpp"
    break;

  case 358:
#line 1946 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 6934 "ds_parser.cpp"
    break;

  case 359:
#line 1947 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 6940 "ds_parser.cpp"
    break;

  case 360:
#line 1948 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 6946 "ds_parser.cpp"
    break;

  case 361:
#line 1949 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 6952 "ds_parser.cpp"
    break;

  case 362:
#line 1950 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 6958 "ds_parser.cpp"
    break;

  case 363:
#line 1951 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 6964 "ds_parser.cpp"
    break;

  case 364:
#line 1952 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 6970 "ds_parser.cpp"
    break;

  case 365:
#line 1956 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6976 "ds_parser.cpp"
    break;

  case 366:
#line 1957 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6982 "ds_parser.cpp"
    break;

  case 367:
#line 1958 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6988 "ds_parser.cpp"
    break;

  case 368:
#line 1959 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6994 "ds_parser.cpp"
    break;

  case 369:
#line 1960 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7000 "ds_parser.cpp"
    break;

  case 370:
#line 1961 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7006 "ds_parser.cpp"
    break;

  case 371:
#line 1965 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7019 "ds_parser.cpp"
    break;

  case 372:
#line 1976 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7028 "ds_parser.cpp"
    break;

  case 373:
#line 1980 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7040 "ds_parser.cpp"
    break;

  case 374:
#line 1990 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7052 "ds_parser.cpp"
    break;

  case 375:
#line 1997 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7063 "ds_parser.cpp"
    break;

  case 376:
#line 2006 "ds_parser.ypp"
    {
            (yyval.pTypeDecl) = new TypeDecl(Type::tBitfield);
            (yyval.pTypeDecl)->argNames = *(yyvsp[-1].pNameList);
            if ( (yyval.pTypeDecl)->argNames.size()>32 ) {
                das_yyerror("only 32 different bits allowed",tokAt((yylsp[-3])),
                    CompilationError::invalid_type);
            }
            (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
            delete (yyvsp[-1].pNameList);
    }
#line 7078 "ds_parser.cpp"
    break;

  case 377:
#line 2019 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7084 "ds_parser.cpp"
    break;

  case 378:
#line 2020 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7090 "ds_parser.cpp"
    break;

  case 379:
#line 2021 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7096 "ds_parser.cpp"
    break;

  case 380:
#line 2022 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7102 "ds_parser.cpp"
    break;

  case 381:
#line 2023 "ds_parser.ypp"
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
#line 7121 "ds_parser.cpp"
    break;

  case 382:
#line 2037 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7132 "ds_parser.cpp"
    break;

  case 383:
#line 2043 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7141 "ds_parser.cpp"
    break;

  case 384:
#line 2047 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7151 "ds_parser.cpp"
    break;

  case 385:
#line 2052 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7161 "ds_parser.cpp"
    break;

  case 386:
#line 2057 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7171 "ds_parser.cpp"
    break;

  case 387:
#line 2062 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7183 "ds_parser.cpp"
    break;

  case 388:
#line 2069 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7193 "ds_parser.cpp"
    break;

  case 389:
#line 2074 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7202 "ds_parser.cpp"
    break;

  case 390:
#line 2078 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7211 "ds_parser.cpp"
    break;

  case 391:
#line 2082 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7221 "ds_parser.cpp"
    break;

  case 392:
#line 2087 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7230 "ds_parser.cpp"
    break;

  case 393:
#line 2091 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7240 "ds_parser.cpp"
    break;

  case 394:
#line 2096 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7251 "ds_parser.cpp"
    break;

  case 395:
#line 2102 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7263 "ds_parser.cpp"
    break;

  case 396:
#line 2109 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7273 "ds_parser.cpp"
    break;

  case 397:
#line 2114 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7284 "ds_parser.cpp"
    break;

  case 398:
#line 2120 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7294 "ds_parser.cpp"
    break;

  case 399:
#line 2125 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7303 "ds_parser.cpp"
    break;

  case 400:
#line 2129 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7313 "ds_parser.cpp"
    break;

  case 401:
#line 2134 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7327 "ds_parser.cpp"
    break;

  case 402:
#line 2143 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7336 "ds_parser.cpp"
    break;

  case 403:
#line 2147 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7346 "ds_parser.cpp"
    break;

  case 404:
#line 2152 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7360 "ds_parser.cpp"
    break;

  case 405:
#line 2161 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7369 "ds_parser.cpp"
    break;

  case 406:
#line 2165 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7379 "ds_parser.cpp"
    break;

  case 407:
#line 2170 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7393 "ds_parser.cpp"
    break;

  case 408:
#line 2179 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7404 "ds_parser.cpp"
    break;

  case 409:
#line 2185 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7415 "ds_parser.cpp"
    break;

  case 410:
#line 2194 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7421 "ds_parser.cpp"
    break;

  case 411:
#line 2194 "ds_parser.ypp"
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
#line 7438 "ds_parser.cpp"
    break;

  case 412:
#line 2209 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7444 "ds_parser.cpp"
    break;

  case 413:
#line 2209 "ds_parser.ypp"
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
#line 7465 "ds_parser.cpp"
    break;

  case 414:
#line 2229 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7471 "ds_parser.cpp"
    break;

  case 415:
#line 2230 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7477 "ds_parser.cpp"
    break;

  case 416:
#line 2231 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7483 "ds_parser.cpp"
    break;

  case 417:
#line 2232 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7489 "ds_parser.cpp"
    break;

  case 418:
#line 2236 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7501 "ds_parser.cpp"
    break;

  case 419:
#line 2243 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7512 "ds_parser.cpp"
    break;

  case 420:
#line 2252 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 7522 "ds_parser.cpp"
    break;

  case 421:
#line 2257 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7531 "ds_parser.cpp"
    break;

  case 422:
#line 2264 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7541 "ds_parser.cpp"
    break;

  case 423:
#line 2269 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7552 "ds_parser.cpp"
    break;

  case 424:
#line 2275 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7563 "ds_parser.cpp"
    break;

  case 425:
#line 2281 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7575 "ds_parser.cpp"
    break;

  case 426:
#line 2288 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7588 "ds_parser.cpp"
    break;

  case 427:
#line 2299 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7596 "ds_parser.cpp"
    break;

  case 428:
#line 2302 "ds_parser.ypp"
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
#line 7612 "ds_parser.cpp"
    break;

  case 429:
#line 2316 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7623 "ds_parser.cpp"
    break;

  case 430:
#line 2325 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7629 "ds_parser.cpp"
    break;

  case 431:
#line 2326 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7635 "ds_parser.cpp"
    break;

  case 432:
#line 2330 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7645 "ds_parser.cpp"
    break;

  case 433:
#line 2335 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7654 "ds_parser.cpp"
    break;

  case 434:
#line 2342 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7664 "ds_parser.cpp"
    break;

  case 435:
#line 2347 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7677 "ds_parser.cpp"
    break;

  case 436:
#line 2358 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7687 "ds_parser.cpp"
    break;

  case 437:
#line 2363 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7696 "ds_parser.cpp"
    break;

  case 438:
#line 2370 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7710 "ds_parser.cpp"
    break;

  case 439:
#line 2382 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 7716 "ds_parser.cpp"
    break;

  case 440:
#line 2383 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7722 "ds_parser.cpp"
    break;

  case 441:
#line 2387 "ds_parser.ypp"
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
#line 7744 "ds_parser.cpp"
    break;

  case 442:
#line 2404 "ds_parser.ypp"
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
#line 7766 "ds_parser.cpp"
    break;


#line 7770 "ds_parser.cpp"

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
#line 2423 "ds_parser.ypp"


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


