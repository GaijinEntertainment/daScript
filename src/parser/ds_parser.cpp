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
#define YYLAST   6786

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  174
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  116
/* YYNRULES -- Number of rules.  */
#define YYNRULES  439
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  807

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
     403,   404,   405,   406,   407,   411,   423,   424,   428,   432,
     437,   446,   454,   470,   492,   496,   499,   505,   514,   535,
     558,   559,   563,   567,   568,   572,   575,   581,   587,   590,
     596,   597,   601,   602,   603,   612,   613,   617,   626,   638,
     648,   657,   658,   659,   660,   661,   662,   666,   671,   679,
     680,   681,   682,   683,   684,   685,   686,   692,   705,   721,
     722,   726,   733,   745,   749,   756,   757,   761,   762,   763,
     767,   772,   779,   783,   784,   785,   786,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   806,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,   817,   818,   822,
     896,   899,   909,   910,   911,   912,   913,   914,   915,   916,
     917,   918,   919,   920,   921,   922,   923,   924,   925,   926,
     930,   935,   941,   947,   958,   964,   965,   976,   980,   986,
     989,   992,   996,  1002,  1006,  1010,  1013,  1016,  1021,  1024,
    1032,  1035,  1040,  1043,  1051,  1057,  1058,  1062,  1063,  1067,
    1101,  1104,  1109,  1117,  1121,  1126,  1132,  1136,  1141,  1150,
    1153,  1159,  1160,  1167,  1178,  1179,  1183,  1188,  1243,  1244,
    1245,  1246,  1247,  1248,  1252,  1253,  1254,  1255,  1256,  1257,
    1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,
    1271,  1272,  1276,  1287,  1292,  1302,  1306,  1313,  1327,  1328,
    1329,  1330,  1331,  1332,  1333,  1334,  1335,  1336,  1337,  1338,
    1339,  1340,  1341,  1342,  1343,  1344,  1345,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,  1353,  1354,  1355,  1356,  1357,  1358,
    1359,  1360,  1361,  1362,  1363,  1364,  1365,  1366,  1370,  1374,
    1377,  1380,  1381,  1382,  1383,  1388,  1393,  1394,  1397,  1400,
    1404,  1408,  1412,  1413,  1414,  1415,  1416,  1417,  1418,  1428,
    1438,  1493,  1494,  1498,  1499,  1503,  1511,  1514,  1521,  1526,
    1534,  1535,  1539,  1542,  1551,  1552,  1556,  1565,  1566,  1571,
    1572,  1576,  1581,  1584,  1588,  1594,  1603,  1604,  1605,  1609,
    1610,  1614,  1617,  1622,  1627,  1635,  1646,  1649,  1656,  1657,
    1661,  1688,  1688,  1700,  1703,  1712,  1725,  1737,  1738,  1742,
    1746,  1757,  1772,  1773,  1777,  1816,  1882,  1889,  1898,  1899,
    1900,  1901,  1902,  1903,  1904,  1905,  1906,  1907,  1908,  1909,
    1910,  1911,  1912,  1913,  1914,  1915,  1916,  1917,  1918,  1919,
    1920,  1921,  1922,  1926,  1927,  1928,  1929,  1930,  1931,  1935,
    1946,  1950,  1960,  1967,  1976,  1985,  1986,  1987,  1988,  1989,
    2003,  2009,  2013,  2018,  2023,  2028,  2035,  2040,  2044,  2048,
    2053,  2058,  2064,  2071,  2076,  2082,  2087,  2091,  2096,  2105,
    2109,  2114,  2123,  2127,  2132,  2141,  2147,  2156,  2156,  2171,
    2171,  2187,  2188,  2189,  2190,  2194,  2201,  2210,  2215,  2222,
    2227,  2233,  2239,  2246,  2257,  2260,  2274,  2283,  2284,  2288,
    2293,  2300,  2305,  2316,  2321,  2328,  2340,  2341,  2345,  2359
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
  "type_declaration", "variant_alias_declaration", "$@2",
  "bitfield_alias_declaration", "$@3", "make_decl", "make_struct_fields",
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

#define YYPACT_NINF -510

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-510)))

#define YYTABLE_NINF -195

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -510,    21,  -510,  -510,   -88,   -75,   -57,   -54,  -104,  -510,
      -5,  -510,  -510,   -10,  -510,  -510,  -510,  -510,  -510,   181,
    -510,    64,  -510,  -510,  -510,  -510,  -510,  -510,    -2,  -510,
      34,    50,    67,  -510,  -510,     5,  -510,   -67,    36,  -510,
      74,    88,  -510,    53,   108,    87,  -510,   103,  -510,   140,
      61,  -510,   118,     3,   -88,   151,   -75,   146,  -510,   153,
     172,  -510,   396,  -510,    98,  -510,   -98,   142,   149,   201,
    -510,   -88,   -10,  -510,   179,   164,  6630,   304,   306,  -510,
     171,  -510,   339,  -510,  -510,  -510,  -510,  -510,   207,    70,
    -510,  -510,  -510,  -510,   289,  -510,  -510,  -510,  -510,  -510,
    -510,  -510,  -510,   173,   -91,  5731,  -510,  -510,   215,   216,
    -510,   -52,  -510,   -72,  -510,  -510,  -510,  -510,  -510,  -510,
    -510,  -510,  -510,  -510,  -510,  -510,  -510,  -510,  -510,  -510,
    -510,  -510,  -510,  -510,  -510,  -510,  -510,  -510,  -510,  -510,
    -510,  -510,  -510,  -510,  -510,  -510,  -510,  -510,  -510,  -510,
    -510,  -510,    39,   208,   -90,   183,  -510,    -3,  -510,  -510,
    -510,  -510,  -510,  -510,  -510,   -12,  -510,  -510,    38,  -510,
     213,   214,   217,   218,  -510,  -510,  -510,   189,  -510,  -510,
    -510,  -510,  -510,   219,  -510,  -510,  -510,  -510,  -510,  -510,
    -510,  -510,  -510,   225,  -510,  -510,  -510,   229,   230,  -510,
    -510,  -510,  -510,   232,   233,  -510,  -510,  -510,  -510,  -510,
     227,  -510,   192,   231,  -510,   210,  -510,  -510,   -99,   -88,
    -510,    -7,  -510,  -101,  5731,   211,  -510,  -510,  -510,   115,
    -510,  -510,  -510,    70,  -510,   -87,  4123,  -510,  5731,  5731,
    5731,  5731,   248,   215,  5618,  5618,  5618,  5808,   216,  -510,
      27,  -510,  -510,  -510,  -510,  2125,  -510,  -510,   -79,  5731,
     -77,  -510,   342,   221,   105,   255,  -510,   -32,  -510,   226,
    1375,  -510,  -510,   259,  5731,  -510,    30,  -510,  -510,  -510,
    -510,  5541,   234,  -510,   236,   249,   252,   242,   262,  -510,
     267,  4123,  4123,   825,  4251,  4123,  -510,  -510,  -510,  -510,
    -510,  -510,  -510,  4123,  4123,  4123,  4123,  4123,  4123,  -510,
     250,  -510,  -510,   -62,  -510,  -510,  -510,   256,  -510,  -510,
    -510,  -510,  -510,  4885,   261,  -510,  -510,  -510,  -510,  -510,
     441,  1230,  1293,  1438,   258,   -35,   208,  1668,   208,  2251,
     208,  2338,    47,  -510,    89,  1375,   104,   276,  -510,   263,
    -510,  -510,  5802,  -510,  -510,  1375,  -510,  -510,  -510,   255,
    -510,  -510,  -510,  -510,  -510,  5731,  2254,  -510,   974,  -510,
     405,  -510,  -510,  -510,  2254,  2518,  -510,   295,  4123,  5731,
    5731,  4123,  5731,  5731,   374,   374,   255,  1294,   255,  1439,
    6194,  -510,   130,   156,   374,   374,   -73,   374,   374,  5022,
     -83,  -510,  1722,   171,    14,   296,  4123,  4123,  -510,  -510,
    4123,  4123,  4123,  4123,   297,  4123,   298,  4123,  4123,  4123,
    4123,  4123,  2392,  4123,  4123,  4123,  4123,  4123,  4123,  4123,
    4123,  4123,  4123,   299,  4123,  -510,  2521,  -510,  5731,  -510,
    -510,  -510,  -510,   300,   287,  -510,   288,  -510,   291,  -510,
    5731,  -510,  5808,  -510,   216,  -510,  -510,  -510,  -510,   188,
     143,  -510,   329,  6025,  -510,  4123,  -510,  -510,   255,  1584,
    -510,   211,  4123,  4123,  -510,   275,   319,  2659,  1867,  -510,
     422,   293,   314,  4123,  -510,  -510,  -510,  -510,  -510,  -510,
     315,   320,   322,  -510,  -510,  -510,   447,  -510,   -81,  -510,
    6129,  -510,  2254,  -510,  4340,  2788,  1996,  5091,  2605,  2785,
    5159,  2872,  3010,     2,   -78,   305,  2926,  6194,   328,   -45,
     331,  -510,  -510,    92,    18,  3064,   154,   155,  4123,  4123,
     317,  -510,  4123,   324,  -510,  5618,  -510,   335,  -510,    22,
    6397,   208,  5731,  -510,  -510,   662,   662,  6596,  6596,  6531,
    6531,   330,     4,  -510,  5879,   -65,   -65,   662,   662,   360,
    6263,   546,   303,  5615,  6596,  6596,   195,   195,     4,     4,
       4,  -510,  5948,  -510,    57,  3148,  -510,  -510,  -510,  1375,
    -510,  4123,  -510,  -510,   143,  4123,  4123,  4123,  4123,  4123,
    4123,  4123,  4123,  4123,  4123,  4123,  4123,  4123,  4123,  4123,
    4244,    19,  2254,  -510,  -510,  4436,   482,  6397,  4244,  -510,
     357,   373,  6397,  2254,  -510,  4532,   332,  -510,  -510,  4244,
    -510,  -510,  -510,   464,   255,  -510,  3202,  3331,  -510,  4628,
    -510,  -510,    79,  5731,   371,  5227,  -510,  4123,  4123,  -510,
    4123,   340,  4123,  4123,  -510,   335,   375,   343,   335,  4123,
     353,  4123,  4123,   335,   354,   355,  6397,  -510,  -510,  6328,
     208,  3457,    73,   111,  4123,  -510,   -94,  1375,  3460,  -510,
    -510,  4123,  -510,  -510,  -510,  6397,  -510,  6397,  6397,  6397,
    6397,  6397,  6397,  6397,  6397,  6397,  6397,  6397,  6397,  6397,
    6397,  6397,  -510,  4123,  -510,  4724,  -510,   211,  -510,  -510,
    -510,  -510,  4820,  -510,  -510,   273,  -510,  -510,  4123,  4123,
    -510,  -510,  3544,   106,  -510,  6596,  6596,  6596,  3598,   -71,
    6397,   107,    73,  -510,   328,  6397,  -510,  -510,   -69,   160,
    -510,  -510,  -510,   359,   -72,   358,  6397,  3736,  -510,  -510,
    -510,    81,  6397,   -59,  -510,  -510,  -510,  1146,   211,  -510,
    -510,  4123,  -510,  6462,  6462,  -510,  3865,   387,  -510,  5295,
    4123,   362,  4123,  4123,   363,   -72,  -510,  -510,  4123,  6397,
    -510,   211,  -510,  -510,  4244,  5731,  5363,   381,  -510,  4957,
    -510,  6397,  4957,  -510,  -510,  6397,  -510,   273,  3682,  -510,
    3994,   473,   366,   361,  -510,  -510,  5731,  5431,  4123,   367,
     368,  4120,  -510,  6397,  -510,  -510,  -510
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    75,     1,   167,     0,     0,     0,     0,     0,   168,
       0,   409,   407,     0,    14,     3,    10,     9,     8,     0,
       7,   318,     6,    11,     5,     4,    12,    13,    65,    67,
      23,    35,    32,    33,    25,    30,    24,     0,     0,    15,
       0,     0,    70,   145,     0,    71,    73,     0,    69,     0,
       0,   319,   321,     0,     0,     0,     0,     0,    31,     0,
       0,    28,     0,   323,     0,   327,     0,     0,     0,     0,
     147,     0,     0,    76,   332,     0,     0,     0,     0,    82,
      77,   316,     0,    63,    64,    61,    62,    60,     0,     0,
      59,    68,    36,    34,    30,    27,    26,   363,   366,   364,
     367,   365,   368,     0,     0,     0,   329,   328,     0,     0,
     146,     0,    74,     0,   334,   286,    85,    86,    88,    87,
      89,    90,    91,    92,   111,   112,   109,   110,   102,   113,
     114,   103,   100,   101,   115,   116,   117,   118,   105,   106,
     104,    98,    99,    94,    93,    95,    96,    97,    84,    83,
     107,   108,   281,    80,     0,     0,    16,     0,    55,    56,
      53,    54,    52,    51,    57,     0,    29,   323,     0,   330,
       0,     0,     0,     0,   338,   358,   339,   370,   340,   344,
     345,   346,   347,   362,   351,   352,   353,   354,   355,   356,
     357,   359,   360,   396,   343,   350,   361,   399,   402,   341,
     348,   342,   349,     0,     0,   369,   375,   378,   376,   377,
       0,   372,     0,     0,   297,     0,    72,   333,   281,     0,
      78,     0,   290,     0,     0,     0,   336,   320,   317,   309,
     322,    17,    18,     0,    66,     0,     0,   324,     0,     0,
       0,     0,     0,     0,    77,    77,    77,     0,     0,   382,
       0,   388,   392,   390,   384,     0,   326,   387,     0,     0,
       0,   335,   283,     0,     0,     0,   288,   301,    79,   281,
      81,   140,   119,     0,     0,   310,     0,    58,   331,   223,
     224,     0,     0,   218,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,   188,   190,   189,   191,
     192,   193,    19,     0,     0,     0,     0,     0,     0,   184,
     185,   221,   186,   219,   274,   273,   272,    75,   277,   220,
     276,   275,   256,     0,     0,   222,   411,   412,   413,   414,
       0,     0,     0,     0,     0,     0,    80,     0,    80,     0,
      80,     0,   145,   294,     0,   292,     0,   383,   386,     0,
     389,   380,     0,   373,   410,   296,   408,   298,   284,     0,
     287,   282,   289,   300,   299,     0,     0,   291,     0,   337,
       0,   307,   308,   306,     0,   149,   152,     0,     0,     0,
       0,     0,     0,     0,   247,   248,     0,     0,     0,     0,
       0,   433,     0,     0,   228,   227,   261,   226,   225,     0,
       0,   280,     0,    77,     0,     0,     0,     0,   249,   250,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   325,     0,   393,     0,   395,
     391,   371,   374,     0,     0,   397,     0,   400,     0,   403,
       0,   405,     0,   406,     0,   385,   381,   379,   285,   302,
       0,   305,     0,   304,   142,     0,    45,    46,     0,     0,
     153,     0,     0,     0,   154,     0,     0,     0,     0,   122,
     120,     0,     0,     0,   135,   130,   128,   129,   141,   123,
       0,     0,     0,   133,   134,   136,   165,   127,     0,   124,
     194,   311,     0,   315,   194,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,     0,     0,   424,   417,     0,
     427,   428,   429,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,    20,   251,    77,   215,     0,   257,     0,
     179,    80,     0,   269,   270,   229,   230,   242,   243,   240,
     241,     0,   266,   255,     0,   278,   279,   231,   232,     0,
       0,   245,   246,   244,   238,   239,   234,   233,   235,   236,
     237,   254,     0,   259,     0,     0,   398,   401,   404,   293,
     295,     0,   185,   144,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   155,   158,   194,     0,   148,     0,   139,
       0,     0,    39,     0,   162,   194,     0,   137,   138,     0,
     126,   131,   132,     0,     0,   125,     0,     0,   313,   194,
     314,   150,     0,     0,     0,     0,   262,     0,     0,   263,
       0,     0,     0,     0,   420,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   426,   434,   435,     0,
      80,     0,     0,     0,     0,   258,     0,   268,     0,   253,
     271,     0,   252,   260,   394,   303,   143,   201,   202,   204,
     203,   205,   198,   199,   200,   206,   207,   196,   197,   208,
     209,   195,    49,     0,   159,   194,   156,     0,    50,    37,
      38,   163,   194,   160,   140,    42,   166,   169,     0,     0,
     312,   151,     0,     0,   173,   170,   171,   172,     0,     0,
     415,     0,     0,   419,   418,   425,   431,   430,     0,     0,
     422,   432,    21,     0,     0,     0,   180,     0,   181,   187,
     213,     0,   267,     0,   157,   164,   161,     0,     0,    40,
      41,     0,    47,   211,   210,   176,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,   216,   212,     0,   182,
     214,     0,   121,    43,     0,     0,     0,     0,   265,   436,
     421,   416,   436,   423,   217,   183,    48,    42,     0,   174,
       0,     0,     0,     0,    44,   177,     0,     0,     0,     0,
       0,     0,   175,   437,   438,   439,   178
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -510,  -510,  -510,   141,   483,  -510,  -510,  -510,  -510,  -510,
    -510,   445,  -510,  -510,   484,  -510,  -510,  -510,  -246,  -510,
    -510,  -510,  -510,  -510,   309,  -510,   489,   -66,  -510,   472,
    -510,   237,  -232,  -329,  -510,  -510,  -223,  -510,  -157,  -347,
     -13,  -510,  -510,  -510,  -510,  -510,  -510,  -510,  -510,   548,
    -510,  -510,  -510,  -403,  -510,  -510,  -442,  -510,  -364,  -510,
    -510,  -510,  -510,   206,   337,  -510,  -510,  -510,   284,  -510,
     109,  -510,  -245,   308,  -266,  -235,   187,  -510,   -61,  -510,
    -510,  -510,  -510,   391,   493,  -510,  -510,  -510,  -510,  -510,
    -510,  -151,   -95,  -510,  -510,  -510,   321,  -510,   -64,  -510,
    -510,  -510,  -510,   279,  -509,  -381,  -510,  -510,  -289,   -89,
     178,  -510,  -510,  -510,  -212,  -510
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   157,   163,   393,   311,    16,    17,    35,
      36,    61,    18,    32,    33,   481,   482,   751,   752,   483,
     484,   485,   486,   487,   164,   165,    29,    30,    45,    46,
      47,    19,   153,   225,    80,    20,   312,   488,   368,   489,
     313,   490,   314,   491,   492,   493,   494,   495,   624,   496,
     497,   315,   316,   539,   739,   317,   318,   319,   462,   499,
     320,   321,   322,   540,   221,   359,   263,   218,   222,   223,
     343,   344,   214,   215,   643,   266,   374,   276,   228,   154,
      52,    22,    82,   104,    65,    66,    23,    24,   114,    75,
      25,   267,   324,   103,   207,   208,   212,   209,   345,    26,
      41,    27,    40,   325,   518,   519,   326,   520,   521,   522,
     523,   327,   392,   328,   792,   329
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   366,   272,   229,   498,   111,   391,   444,   526,   446,
     206,   448,   336,   338,   340,   357,    83,    84,   583,   461,
     642,     2,    42,   404,   405,    57,     3,   503,   663,   737,
     362,   542,   363,   574,   584,    64,   652,   693,   265,   416,
     417,   210,   168,   226,   414,    28,   168,   416,   417,     4,
      43,     5,    31,     6,   353,     7,   213,    58,   347,    48,
       8,    43,   364,   535,   219,   268,     9,    38,   664,   269,
     664,   401,    10,   261,   106,    62,    34,   271,   363,    37,
     664,   169,   227,   158,   159,   278,    44,    54,    11,   625,
     434,    69,   205,   354,    76,   356,   433,    44,   434,   760,
     217,   763,   632,   402,    63,   408,   409,   273,   364,    12,
     365,   771,   442,   414,   216,   415,   416,   417,   418,   647,
      51,   419,   604,    43,   458,   648,   226,   233,    39,   206,
      85,   614,   231,   232,    86,   443,    87,    88,    53,   724,
     371,   273,   676,   206,   206,   206,   206,   543,   372,   206,
     206,   206,   206,   264,   234,   628,    59,   273,   273,    44,
     270,   664,    77,    78,   206,   433,    60,   434,    89,    64,
     373,   541,   348,    54,   330,   331,   332,   333,   236,   206,
     337,   339,   341,   363,    13,    49,   206,    50,   665,   450,
     349,    14,    55,   581,    79,   355,   664,   160,   206,   206,
     350,   161,   219,   162,    88,   220,    56,    67,   237,   357,
     370,   205,   666,   364,   404,   405,    69,   375,   664,   249,
     664,    68,    69,   673,   250,   205,   205,   205,   205,   387,
     389,   205,   205,   205,   205,   513,   451,   524,   105,   719,
     657,    70,    72,   251,    54,   711,   205,   770,   606,   728,
     646,   453,    71,   756,   273,   694,   650,   274,   249,   452,
     275,   205,   651,   250,   721,   741,   701,    73,   205,   361,
     206,   761,   729,    74,   454,   735,   757,   648,    92,    94,
     205,   205,   251,   748,   206,   206,    95,   206,   206,    81,
     743,   156,   531,   532,   749,   750,   408,   409,   363,   252,
     529,   459,   530,   660,   414,    96,   415,   416,   417,   418,
     309,   582,   419,   108,   271,   508,   509,   601,   511,   512,
     109,   113,   404,   405,   648,   651,   654,   655,   364,   253,
     648,   733,   764,   254,   110,   115,   152,   150,   252,   151,
     155,    58,   156,   206,   167,   430,   431,   432,   211,   213,
     224,   255,   205,   230,   242,   206,   433,   206,   434,   238,
     239,   257,   258,   240,   241,   243,   205,   205,   253,   205,
     205,   244,   254,   259,   575,   245,   246,   692,   247,   248,
     260,   334,   271,   498,   358,   698,   579,   536,   226,   219,
     255,   360,   369,   404,   405,   379,   705,   256,   380,   377,
     257,   378,   406,   407,   408,   409,   410,   381,   382,   411,
     412,   413,   414,   383,   415,   416,   417,   418,   400,    13,
     419,   455,   420,   421,   441,   205,   436,   456,   506,   544,
     551,   553,   571,   353,   576,   577,   249,   205,   578,   205,
     206,   250,   323,   738,   584,   609,   610,   206,   425,   426,
     427,   428,   429,   430,   431,   432,   762,   616,    97,   231,
     251,   352,    98,   617,   433,   623,   434,   646,   662,   644,
     649,   661,   249,   229,   745,   408,   409,   250,   667,    99,
     100,   101,   102,   414,   618,   620,   416,   417,   418,   658,
     621,   419,   622,   670,   697,   668,   251,   384,   385,   699,
     700,   390,   706,   704,   713,   718,   765,   723,   722,   394,
     395,   396,   397,   398,   399,   371,   252,   726,   730,   731,
     777,   798,   205,   372,   767,   773,   780,   783,   790,   205,
     799,   804,   805,   800,   533,   433,    90,   434,   206,   166,
      93,   794,   277,    91,   112,   373,   253,   747,   786,    21,
     254,   787,   252,   367,   403,   262,   346,   502,   235,   107,
     376,   580,   727,   707,   335,   404,   405,   527,   255,   712,
     793,     0,   463,     0,   500,   501,     0,     0,   257,     0,
     504,     0,   253,     0,   507,     0,   254,   510,   437,     0,
       0,     0,     0,   517,     0,   517,     0,     0,     0,     0,
       0,     0,     0,     0,   255,     0,     0,     0,     0,     0,
       0,     0,   545,   546,   257,     0,   547,   548,   549,   550,
     205,   552,     0,   554,   555,   556,   557,   558,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,   570,     0,
     572,     0,     0,     0,     0,   406,   407,   408,   409,   410,
       0,     0,   411,   412,   413,   414,     0,   415,   416,   417,
     418,     0,     0,   419,     0,   420,   421,     0,     0,     0,
       0,   600,     0,     0,     0,   605,     0,     0,   607,   608,
     206,   404,   405,   612,   615,     0,     0,     0,     0,   619,
     424,   425,   426,   427,   428,   429,   430,   431,   432,     0,
       0,   206,     0,     0,     0,     0,     0,   433,   629,   434,
       0,   788,   635,     0,     0,     0,     0,     0,     0,     0,
       0,   766,   399,     0,     0,     0,     0,     0,     0,     0,
       0,   399,   801,     0,   656,   390,     0,     0,   659,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   784,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   408,   409,     0,     0,     0,     0,     0,
       0,   414,     0,   415,   416,   417,   418,     0,     0,   419,
       0,     0,     0,   205,     0,     0,     0,   675,     0,     0,
       0,   677,   678,   679,   680,   681,   682,   683,   684,   685,
     686,   687,   688,   689,   690,   691,     0,     0,   695,     0,
     428,   429,   430,   431,   432,     0,     0,     0,     0,   702,
       0,     0,     0,   433,     0,   434,     0,     0,     0,     0,
       0,     0,   687,   691,     0,     0,   386,     0,     0,     0,
       0,     0,     0,   715,   716,     0,   717,     0,   170,   720,
       0,     0,     0,     0,   171,   725,     0,   517,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   172,
     736,     0,     0,     0,     0,     0,     0,   742,     0,     0,
       0,     0,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   753,   754,     0,     0,     0,     0,
       0,     0,     0,     0,   759,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   769,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   500,     0,     0,     0,   774,    43,     0,
       0,     0,   776,     0,     0,     0,   779,     0,   781,   782,
       0,     0,     0,     0,   785,   464,     0,     0,     0,     3,
       0,   465,   466,   467,     0,   468,     0,   279,   280,   281,
     282,     0,     0,     0,    44,     0,   797,     0,   469,   283,
     470,   471,     0,     0,   803,     0,     0,     0,     0,     0,
     472,   284,     0,     0,   473,   285,     0,   286,     0,     9,
     287,   474,     0,   475,   288,     0,     0,   476,   477,     0,
       0,     0,   174,   175,   176,     0,   178,   179,   180,   181,
     182,   289,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,   194,   195,   196,     0,     0,   199,   200,   201,
     202,     0,     0,   290,   478,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   291,   292,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     460,     0,     0,     0,     0,     0,     0,     0,   293,   294,
     295,   296,   297,   298,   299,   300,   301,    43,   302,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   303,   304,   305,     0,     0,     0,     0,   306,
     307,     0,     0,     0,     0,     0,     0,     0,     0,   308,
       0,   309,   310,    44,   479,   271,   480,   464,     0,     0,
       0,     3,     0,   465,   466,   467,     0,   468,     0,   279,
     280,   281,   282,     0,     0,     0,     0,     0,     0,     0,
     469,   283,   470,   471,     0,     0,     0,     0,     0,     0,
       0,     0,   472,   284,     0,     0,   473,   285,     0,   286,
       0,     9,   287,   474,     0,   475,   288,     0,     0,   476,
     477,     0,     0,     0,   174,   175,   176,     0,   178,   179,
     180,   181,   182,   289,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     0,   194,   195,   196,     0,     0,   199,
     200,   201,   202,     0,     0,   290,   478,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   291,   292,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   249,   460,     0,     0,     0,   250,     0,     0,     0,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    43,
     302,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   303,   304,   305,     0,     0,     0,
       0,   306,   307,     0,     0,     0,     0,   279,   280,   281,
     282,   308,     0,   309,   310,    44,   479,   271,   772,   283,
       0,     0,     0,     0,   249,   249,     0,     0,     0,   250,
     250,   284,     0,     0,     0,   285,     0,   286,     0,     0,
     287,   252,     0,     0,   288,     0,     0,     0,   251,   251,
       0,     0,   174,   175,   176,     0,   178,   179,   180,   181,
     182,   289,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   253,   194,   195,   196,   254,     0,   199,   200,   201,
     202,     0,     0,   290,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   255,     0,   291,   292,     0,     0,     0,
     438,     0,     0,   257,   252,   252,   249,     0,     0,     0,
       0,   250,     0,     0,     0,     0,     0,     0,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   514,   302,     0,
     251,     0,     0,     0,   253,   253,     0,     0,   254,   254,
     439,     0,   303,   304,   305,     0,     0,     0,     0,   306,
     307,     0,   279,   280,   281,   282,   255,   255,   515,   516,
       0,   309,   310,    44,   283,   271,   257,   257,     0,   249,
     249,     0,     0,     0,   250,   250,   284,     0,     0,     0,
     285,     0,   286,     0,     0,   287,   252,     0,     0,   288,
       0,     0,     0,   251,   251,     0,     0,   174,   175,   176,
       0,   178,   179,   180,   181,   182,   289,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   253,   194,   195,   196,
     254,     0,   199,   200,   201,   202,     0,     0,   290,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   255,     0,
     291,   292,     0,     0,     0,     0,     0,     0,   257,   252,
     252,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   514,   302,     0,     0,     0,     0,     0,   253,
     253,     0,     0,   254,   254,   440,     0,   303,   304,   305,
       0,     0,     0,     0,   306,   307,     0,   279,   280,   281,
     282,   255,   255,     0,   525,     0,   309,   310,    44,   283,
     271,   257,   257,     0,     0,     0,     0,     0,     0,     0,
       0,   284,     0,     0,     0,   285,     0,   286,     0,     0,
     287,     0,     0,     0,   288,     0,     0,     0,     0,     0,
       0,     0,   174,   175,   176,     0,   178,   179,   180,   181,
     182,   289,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,   194,   195,   196,     0,     0,   199,   200,   201,
     202,     0,     0,   290,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   291,   292,     0,     0,     0,
       0,     0,     0,     0,   602,     0,     0,     0,     0,   249,
     460,     0,     0,     0,   250,     0,     0,     0,   293,   294,
     295,   296,   297,   298,   299,   300,   301,    43,   302,     0,
       0,     0,     0,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   303,   304,   305,   279,   280,   281,   282,   306,
     307,     0,     0,     0,     0,     0,     0,   283,     0,   308,
       0,   309,   310,    44,   603,   271,     0,     0,     0,   284,
       0,     0,     0,   285,     0,   286,     0,     0,   287,     0,
       0,     0,   288,     0,     0,     0,     0,     0,     0,   252,
     174,   175,   176,     0,   178,   179,   180,   181,   182,   289,
     184,   185,   186,   187,   188,   189,   190,   191,   192,     0,
     194,   195,   196,     0,     0,   199,   200,   201,   202,   253,
       0,   290,     0,   254,     0,   445,     0,     0,     0,     0,
       0,     0,     0,   291,   292,     0,     0,     0,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   257,     0,     0,     0,     0,   293,   294,   295,   296,
     297,   298,   299,   300,   301,    43,   302,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     303,   304,   305,     0,     0,     0,     0,   306,   307,     0,
     279,   280,   281,   282,     0,   537,     0,   308,   538,   309,
     310,    44,   283,   271,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   284,     0,     0,     0,   285,     0,
     286,     0,     0,   287,     0,     0,     0,   288,     0,     0,
       0,     0,     0,     0,     0,   174,   175,   176,     0,   178,
     179,   180,   181,   182,   289,   184,   185,   186,   187,   188,
     189,   190,   191,   192,     0,   194,   195,   196,     0,     0,
     199,   200,   201,   202,     0,     0,   290,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   291,   292,
       0,     0,     0,     0,     0,     0,     0,   613,     0,     0,
       0,     0,     0,   460,     0,     0,     0,     0,     0,     0,
       0,   293,   294,   295,   296,   297,   298,   299,   300,   301,
      43,   302,     0,     0,     0,     0,     0,     0,     0,   279,
     280,   281,   282,   633,     0,   303,   304,   305,     0,     0,
       0,   283,   306,   307,     0,     0,     0,     0,     0,     0,
       0,     0,   308,   284,   309,   310,    44,   285,   271,   286,
       0,     0,   287,     0,     0,     0,   288,     0,     0,     0,
       0,     0,     0,     0,   174,   175,   176,     0,   178,   179,
     180,   181,   182,   289,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     0,   194,   195,   196,     0,     0,   199,
     200,   201,   202,     0,     0,   290,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   291,   292,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    43,
     302,     0,     0,     0,     0,     0,     0,     0,   279,   280,
     281,   282,   634,     0,   303,   304,   305,     0,     0,     0,
     283,   306,   307,     0,     0,     0,     0,     0,     0,     0,
       0,   308,   284,   309,   310,    44,   285,   271,   286,     0,
       0,   287,     0,     0,     0,   288,     0,     0,     0,     0,
       0,     0,     0,   174,   175,   176,     0,   178,   179,   180,
     181,   182,   289,   184,   185,   186,   187,   188,   189,   190,
     191,   192,     0,   194,   195,   196,     0,     0,   199,   200,
     201,   202,     0,     0,   290,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   291,   292,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   293,
     294,   295,   296,   297,   298,   299,   300,   301,    43,   302,
       0,     0,     0,     0,     0,     0,     0,   279,   280,   281,
     282,     0,     0,   303,   304,   305,     0,     0,     0,   283,
     306,   307,   249,     0,     0,     0,     0,   250,     0,   351,
     308,   284,   309,   310,    44,   285,   271,   286,     0,     0,
     287,     0,     0,     0,   288,     0,   251,     0,     0,     0,
       0,     0,   174,   175,   176,     0,   178,   179,   180,   181,
     182,   289,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,   194,   195,   196,     0,     0,   199,   200,   201,
     202,     0,     0,   290,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   291,   292,     0,     0,     0,
       0,     0,   252,     0,     0,     0,     0,     0,     0,   249,
     460,     0,     0,     0,   250,     0,     0,     0,   293,   294,
     295,   296,   297,   298,   299,   300,   301,    43,   302,     0,
       0,     0,   253,   251,     0,     0,   254,     0,   447,     0,
       0,     0,   303,   304,   305,   279,   280,   281,   282,   306,
     307,     0,   559,     0,   255,     0,     0,   283,     0,   308,
       0,   309,   310,    44,   257,   271,     0,     0,     0,   284,
       0,     0,     0,   285,     0,   286,     0,     0,   287,     0,
       0,     0,   288,     0,     0,     0,     0,     0,     0,   252,
     174,   175,   176,     0,   178,   179,   180,   181,   182,   289,
     184,   185,   186,   187,   188,   189,   190,   191,   192,     0,
     194,   195,   196,     0,     0,   199,   200,   201,   202,   253,
       0,   290,     0,   254,     0,   449,     0,     0,     0,     0,
       0,     0,     0,   291,   292,     0,     0,     0,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   257,     0,     0,     0,     0,   293,   294,   295,   296,
     297,   298,   299,   300,   301,    43,   302,     0,     0,     0,
       0,     0,     0,     0,   279,   280,   281,   282,     0,     0,
     303,   304,   305,     0,     0,     0,   283,   306,   307,   249,
       0,     0,     0,     0,   250,     0,     0,   308,   284,   309,
     310,    44,   285,   271,   286,     0,     0,   287,     0,     0,
       0,   288,     0,   251,     0,     0,     0,     0,     0,   174,
     175,   176,     0,   178,   179,   180,   181,   182,   289,   184,
     185,   186,   187,   188,   189,   190,   191,   192,     0,   194,
     195,   196,     0,     0,   199,   200,   201,   202,     0,     0,
     290,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   291,   292,     0,     0,     0,     0,     0,   252,
       0,     0,     0,     0,     0,     0,   249,     0,     0,     0,
       0,   250,     0,     0,     0,   293,   294,   295,   296,   297,
     298,   299,   300,   301,    43,   302,     0,     0,     0,   253,
     251,     0,     0,   254,     0,     0,     0,     0,     0,   303,
     304,   305,   279,   280,   281,   282,   306,   307,     0,     0,
       0,   255,     0,   505,   283,     0,   308,   573,   309,   310,
      44,   257,   271,     0,     0,     0,   284,     0,     0,     0,
     285,     0,   286,     0,     0,   287,     0,     0,     0,   288,
       0,     0,   611,     0,     0,     0,   252,   174,   175,   176,
       0,   178,   179,   180,   181,   182,   289,   184,   185,   186,
     187,   188,   189,   190,   191,   192,     0,   194,   195,   196,
       0,     0,   199,   200,   201,   202,   253,     0,   290,     0,
     254,     0,   637,     0,     0,     0,     0,     0,     0,     0,
     291,   292,     0,     0,     0,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   257,     0,
       0,     0,     0,   293,   294,   295,   296,   297,   298,   299,
     300,   301,    43,   302,     0,     0,     0,     0,     0,     0,
       0,   279,   280,   281,   282,     0,     0,   303,   304,   305,
       0,     0,     0,   283,   306,   307,   249,     0,     0,     0,
       0,   250,     0,     0,   308,   284,   309,   310,    44,   285,
     271,   286,     0,     0,   287,     0,     0,     0,   288,     0,
     251,     0,     0,     0,     0,     0,   174,   175,   176,     0,
     178,   179,   180,   181,   182,   289,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     0,   194,   195,   196,     0,
       0,   199,   200,   201,   202,     0,     0,   290,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   291,
     292,     0,     0,     0,     0,     0,   252,     0,     0,     0,
       0,     0,     0,   249,     0,     0,     0,     0,   250,     0,
       0,     0,   293,   294,   295,   296,   297,   298,   299,   300,
     301,    43,   302,     0,     0,     0,   253,   251,     0,     0,
     254,     0,   638,     0,     0,     0,   303,   304,   305,   279,
     280,   281,   282,   306,   307,     0,     0,     0,   255,     0,
       0,   283,     0,   308,   631,   309,   310,    44,   257,   271,
       0,     0,     0,   284,     0,     0,     0,   285,     0,   286,
       0,     0,   287,     0,     0,     0,   288,     0,     0,     0,
       0,     0,     0,   252,   174,   175,   176,     0,   178,   179,
     180,   181,   182,   289,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     0,   194,   195,   196,     0,     0,   199,
     200,   201,   202,   253,     0,   290,     0,   254,     0,   640,
       0,     0,     0,     0,     0,     0,     0,   291,   292,     0,
       0,     0,     0,     0,     0,   255,     0,     0,     0,     0,
       0,   249,     0,     0,     0,   257,   250,     0,     0,     0,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    43,
     302,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   303,   304,   305,   279,   280,   281,
     282,   306,   307,     0,     0,     0,     0,     0,     0,   283,
       0,   308,   645,   309,   310,    44,     0,   271,     0,     0,
       0,   284,     0,     0,     0,   285,     0,   286,     0,     0,
     287,     0,     0,     0,   288,     0,     0,     0,     0,     0,
       0,   252,   174,   175,   176,     0,   178,   179,   180,   181,
     182,   289,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,   194,   195,   196,     0,     0,   199,   200,   201,
     202,   253,     0,   290,     0,   254,     0,   641,     0,     0,
       0,     0,     0,     0,     0,   291,   292,     0,     0,     0,
       0,     0,     0,   255,     0,     0,     0,     0,     0,   249,
       0,     0,     0,   257,   250,     0,     0,     0,   293,   294,
     295,   296,   297,   298,   299,   300,   301,    43,   302,     0,
       0,     0,     0,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   303,   304,   305,   279,   280,   281,   282,   306,
     307,     0,     0,     0,     0,     0,     0,   283,     0,   308,
     653,   309,   310,    44,     0,   271,     0,     0,     0,   284,
       0,     0,     0,   285,     0,   286,     0,     0,   287,     0,
       0,     0,   288,     0,     0,     0,     0,     0,     0,   252,
     174,   175,   176,     0,   178,   179,   180,   181,   182,   289,
     184,   185,   186,   187,   188,   189,   190,   191,   192,     0,
     194,   195,   196,     0,     0,   199,   200,   201,   202,   253,
       0,   290,     0,   254,     0,   674,     0,     0,     0,     0,
       0,     0,     0,   291,   292,     0,     0,     0,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,   708,     0,
       0,   257,     0,     0,     0,     0,   293,   294,   295,   296,
     297,   298,   299,   300,   301,    43,   302,     0,     0,     0,
       0,     0,     0,     0,   279,   280,   281,   282,     0,     0,
     303,   304,   305,     0,     0,     0,   283,   306,   307,     0,
       0,     0,     0,     0,     0,     0,     0,   308,   284,   309,
     310,    44,   285,   271,   286,     0,     0,   287,     0,     0,
       0,   288,     0,     0,     0,     0,     0,     0,     0,   174,
     175,   176,     0,   178,   179,   180,   181,   182,   289,   184,
     185,   186,   187,   188,   189,   190,   191,   192,     0,   194,
     195,   196,     0,     0,   199,   200,   201,   202,     0,     0,
     290,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   291,   292,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   709,     0,     0,
       0,     0,     0,     0,     0,   293,   294,   295,   296,   297,
     298,   299,   300,   301,    43,   302,     0,     0,     0,     0,
       0,     0,     0,   279,   280,   281,   282,     0,     0,   303,
     304,   305,     0,     0,     0,   283,   306,   307,   249,     0,
       0,     0,     0,   250,     0,     0,   308,   284,   309,   310,
      44,   285,   271,   286,     0,     0,   287,     0,     0,     0,
     288,     0,   251,     0,     0,     0,     0,     0,   174,   175,
     176,     0,   178,   179,   180,   181,   182,   289,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,   194,   195,
     196,     0,     0,   199,   200,   201,   202,     0,     0,   290,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   291,   292,     0,     0,     0,     0,     0,   252,     0,
       0,     0,     0,     0,     0,   249,     0,     0,     0,     0,
     250,     0,     0,     0,   293,   294,   295,   296,   297,   298,
     299,   300,   301,    43,   302,     0,     0,     0,   253,   251,
       0,     0,   254,     0,   734,     0,     0,     0,   303,   304,
     305,   279,   280,   281,   282,   306,   307,     0,     0,     0,
     255,     0,     0,   283,     0,   308,   740,   309,   310,    44,
     257,   271,     0,     0,     0,   284,     0,     0,     0,   285,
       0,   286,     0,     0,   287,     0,     0,     0,   288,     0,
       0,     0,     0,     0,     0,   252,   174,   175,   176,     0,
     178,   179,   180,   181,   182,   289,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     0,   194,   195,   196,     0,
       0,   199,   200,   201,   202,   253,     0,   290,     0,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   291,
     292,     0,     0,     0,     0,     0,     0,   255,     0,     0,
     755,     0,     0,   249,     0,     0,     0,   257,   250,     0,
       0,     0,   293,   294,   295,   296,   297,   298,   299,   300,
     301,    43,   302,     0,     0,     0,     0,   251,     0,     0,
       0,     0,     0,     0,     0,     0,   303,   304,   305,   279,
     280,   281,   282,   306,   307,     0,     0,     0,     0,     0,
       0,   283,     0,   308,   758,   309,   310,    44,     0,   271,
       0,     0,     0,   284,     0,     0,     0,   285,     0,   286,
       0,     0,   287,     0,     0,     0,   288,     0,     0,     0,
       0,     0,     0,   252,   174,   175,   176,     0,   178,   179,
     180,   181,   182,   289,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     0,   194,   195,   196,     0,     0,   199,
     200,   201,   202,   253,     0,   290,     0,   254,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   291,   292,     0,
       0,     0,     0,     0,     0,   255,   768,     0,   795,     0,
       0,     0,     0,     0,     0,   257,     0,     0,     0,     0,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    43,
     302,     0,     0,     0,     0,     0,     0,     0,   279,   280,
     281,   282,   775,     0,   303,   304,   305,     0,     0,     0,
     283,   306,   307,     0,     0,     0,     0,     0,     0,     0,
       0,   308,   284,   309,   310,    44,   285,   271,   286,     0,
       0,   287,     0,     0,     0,   288,     0,     0,     0,     0,
       0,     0,     0,   174,   175,   176,     0,   178,   179,   180,
     181,   182,   289,   184,   185,   186,   187,   188,   189,   190,
     191,   192,     0,   194,   195,   196,     0,     0,   199,   200,
     201,   202,     0,     0,   290,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   291,   292,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   293,
     294,   295,   296,   297,   298,   299,   300,   301,    43,   302,
       0,     0,     0,     0,     0,     0,     0,   279,   280,   281,
     282,   796,     0,   303,   304,   305,     0,     0,     0,   283,
     306,   307,     0,     0,     0,     0,     0,     0,     0,     0,
     308,   284,   309,   310,    44,   285,   271,   286,     0,     0,
     287,     0,     0,     0,   288,     0,     0,     0,     0,     0,
       0,     0,   174,   175,   176,     0,   178,   179,   180,   181,
     182,   289,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,   194,   195,   196,     0,     0,   199,   200,   201,
     202,     0,     0,   290,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   291,   292,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   293,   294,
     295,   296,   297,   298,   299,   300,   301,    43,   302,     0,
       0,     0,     0,     0,     0,     0,   279,   280,   281,   282,
       0,     0,   303,   304,   305,     0,     0,     0,   283,   306,
     307,   249,     0,     0,     0,     0,   250,     0,     0,   308,
     284,   309,   310,    44,   285,   271,   286,     0,     0,   287,
       0,     0,     0,   288,     0,   251,     0,     0,     0,     0,
       0,   174,   175,   176,     0,   178,   179,   180,   181,   182,
     289,   184,   185,   186,   187,   188,   189,   190,   191,   192,
       0,   194,   195,   196,     0,     0,   199,   200,   201,   202,
       0,     0,   290,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   291,   292,     0,     0,     0,     0,
       0,   252,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   293,   294,   295,
     296,   297,   298,   299,   300,   301,    43,   302,     0,     0,
       0,   253,   388,   404,   405,   254,     0,     0,     0,     0,
       0,   303,   304,   305,   170,     0,     0,     0,   306,   307,
     171,     0,     0,   255,     0,     0,   806,     0,   308,     0,
     309,   310,    44,   257,   271,   172,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,     0,     0,   406,   407,   408,   409,   410,     0,     0,
     411,   412,   413,   414,     0,   415,   416,   417,   418,   404,
     405,   419,     0,   420,   421,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,   422,     0,   423,   424,   425,
     426,   427,   428,   429,   430,   431,   432,     0,     0,     0,
       0,     0,     0,     0,     0,   433,     0,   434,     0,     0,
       0,     0,     0,     0,     0,   271,     0,     0,     0,     0,
      44,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   585,   586,   587,   588,   589,   590,   591,   592,   406,
     407,   408,   409,   410,   593,   594,   411,   412,   413,   414,
     595,   415,   416,   417,   418,   404,   405,   419,   596,   420,
     421,   597,   598,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     599,   422,     0,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,   434,     0,     0,     0,     0,     0,     0,
     630,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   585,   586,   587,
     588,   589,   590,   591,   592,   406,   407,   408,   409,   410,
     593,   594,   411,   412,   413,   414,   595,   415,   416,   417,
     418,   404,   405,   419,   596,   420,   421,   597,   598,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   599,   422,     0,   423,
     424,   425,   426,   427,   428,   429,   430,   431,   432,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,   434,
       0,     0,     0,     0,     0,     0,   696,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   585,   586,   587,   588,   589,   590,   591,
     592,   406,   407,   408,   409,   410,   593,   594,   411,   412,
     413,   414,   595,   415,   416,   417,   418,   404,   405,   419,
     596,   420,   421,   597,   598,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   599,   422,     0,   423,   424,   425,   426,   427,
     428,   429,   430,   431,   432,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,   434,     0,     0,     0,     0,
       0,     0,   703,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   585,
     586,   587,   588,   589,   590,   591,   592,   406,   407,   408,
     409,   410,   593,   594,   411,   412,   413,   414,   595,   415,
     416,   417,   418,   404,   405,   419,   596,   420,   421,   597,
     598,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   599,   422,
       0,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,   434,     0,     0,     0,     0,     0,     0,   710,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   585,   586,   587,   588,   589,
     590,   591,   592,   406,   407,   408,   409,   410,   593,   594,
     411,   412,   413,   414,   595,   415,   416,   417,   418,   404,
     405,   419,   596,   420,   421,   597,   598,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   599,   422,     0,   423,   424,   425,
     426,   427,   428,   429,   430,   431,   432,     0,     0,     0,
       0,     0,     0,     0,     0,   433,     0,   434,     0,     0,
       0,     0,     0,     0,   744,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   405,     0,     0,     0,     0,
       0,   585,   586,   587,   588,   589,   590,   591,   592,   406,
     407,   408,   409,   410,   593,   594,   411,   412,   413,   414,
     595,   415,   416,   417,   418,     0,     0,   419,   596,   420,
     421,   597,   598,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     599,   422,     0,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,     0,     0,     0,   404,   405,     0,     0,
       0,   433,     0,   434,   406,   407,   408,   409,   410,     0,
     746,   411,   412,   413,   414,     0,   415,   416,   417,   418,
       0,     0,   419,     0,   420,   421,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   422,     0,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,     0,     0,
       0,   404,   405,     0,     0,     0,   433,     0,   434,     0,
       0,     0,     0,     0,     0,   435,   406,   407,   408,   409,
     410,     0,     0,   411,   412,   413,   414,     0,   415,   416,
     417,   418,     0,     0,   419,     0,   420,   421,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   422,     0,
     423,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     404,   405,     0,     0,     0,     0,     0,     0,   433,     0,
     434,   406,   407,   408,   409,   410,     0,   791,   411,   412,
     413,   414,     0,   415,   416,   417,   418,     0,     0,   419,
       0,   420,   421,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   422,     0,   423,   424,   425,   426,   427,
     428,   429,   430,   431,   432,     0,     0,     0,   404,   405,
       0,     0,     0,   433,     0,   434,     0,     0,   534,     0,
     406,   407,   408,   409,   410,     0,     0,   411,   412,   413,
     414,     0,   415,   416,   417,   418,     0,     0,   419,     0,
     420,   421,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   422,     0,   423,   424,   425,   426,   427,   428,
     429,   430,   431,   432,     0,     0,   404,   405,     0,     0,
       0,     0,   433,     0,   434,     0,     0,   636,   406,   407,
     408,   409,   410,     0,     0,   411,   412,   413,   414,     0,
     415,   416,   417,   418,     0,     0,   419,     0,   420,   421,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     422,     0,   423,   424,   425,   426,   427,   428,   429,   430,
     431,   432,     0,     0,   404,   405,     0,     0,     0,     0,
     433,     0,   434,     0,     0,   639,   406,   407,   408,   409,
     410,     0,     0,   411,   412,   413,   414,     0,   415,   416,
     417,   418,     0,     0,   419,     0,   420,   421,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   422,     0,
     423,   424,   425,   426,   427,   428,   429,   430,   431,   432,
       0,     0,   404,   405,     0,     0,     0,     0,   433,     0,
     434,     0,     0,   714,   406,   407,   408,   409,   410,     0,
       0,   411,   412,   413,   414,     0,   415,   416,   417,   418,
       0,     0,   419,     0,   420,   421,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   422,     0,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,     0,     0,
     404,   405,     0,     0,     0,     0,   433,     0,   434,     0,
       0,   778,   406,   407,   408,   409,   410,     0,     0,   411,
     412,   413,   414,     0,   415,   416,   417,   418,     0,     0,
     419,     0,   420,   421,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   422,     0,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,   434,     0,     0,   789,
     406,   407,   408,   409,   410,     0,     0,   411,   412,   413,
     414,     0,   415,   416,   417,   418,     0,     0,   419,     0,
     420,   421,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   170,     0,     0,     0,     0,     0,
     171,     0,   422,     0,   423,   424,   425,   426,   427,   428,
     429,   430,   431,   432,     0,   172,     0,     0,     0,     0,
       0,     0,   433,     0,   434,     0,     0,   802,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,     0,     0,     0,   404,   405,     0,     0,     0,     0,
       0,   170,     0,     0,     0,     0,     0,   171,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   172,     0,     0,   293,   294,   295,     0,     0,
       0,     0,     0,     0,    43,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,     0,     0,     0,
      44,     0,     0,     0,   406,   407,   408,   409,   410,     0,
       0,   411,   412,   413,   414,     0,   415,   416,   417,   418,
       0,     0,   419,     0,   420,   421,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    43,     0,     0,   170,     0,     0,     0,     0,     0,
     171,   426,   427,   428,   429,   430,   431,   432,     0,     0,
       0,     0,     0,     0,     0,   172,   433,     0,   434,     0,
       0,     0,     0,   152,     0,     0,     0,    44,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,   404,   405,     0,     0,     0,     0,     0,     0,     0,
       0,   170,     0,     0,     0,     0,     0,   171,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   172,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,     0,   404,   405,
      44,   406,   407,   408,   409,   410,     0,     0,   411,   412,
     413,   414,     0,   415,   416,   417,   418,     0,     0,   419,
       0,   420,   421,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   342,     0,   422,     0,   423,   424,   425,   426,   427,
     428,   429,   430,   431,   432,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,   434,   457,   404,   405,     0,
       0,     0,     0,     0,     0,     0,     0,    44,   406,   407,
     408,   409,   410,     0,     0,   411,   412,   413,   414,     0,
     415,   416,   417,   418,     0,     0,   419,     0,   420,   421,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     422,     0,   423,   424,   425,   426,   427,   428,   429,   430,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
     433,     0,   434,   669,   404,   405,     0,   406,   407,   408,
     409,   410,     0,     0,   411,   412,   413,   414,     0,   415,
     416,   417,   418,     0,     0,   419,     0,   420,   421,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   422,
       0,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,   434,   672,     0,     0,     0,   585,   586,   587,   588,
     589,   590,   591,   592,   406,   407,   408,   409,   410,   593,
     594,   411,   412,   413,   414,   595,   415,   416,   417,   418,
    -194,     0,   419,   596,   420,   421,   597,   598,   404,   405,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   599,   422,     0,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,   434,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,     0,     0,     0,     0,     0,
     585,   586,   587,   588,   589,   590,   591,   592,   406,   407,
     408,   409,   410,   593,   594,   411,   412,   413,   414,   626,
     415,   416,   417,   418,     0,     0,   419,   596,   420,   421,
     597,   598,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   627,
     422,     0,   423,   424,   425,   426,   427,   428,   429,   430,
     431,   432,   404,   405,     0,     0,     0,     0,     0,     0,
     433,     0,   434,   406,   407,   408,   409,   410,     0,     0,
     411,   412,   413,   414,     0,   415,   416,   417,   418,     0,
       0,   419,     0,   420,   421,     0,     0,   528,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   422,     0,   423,   424,   425,
     426,   427,   428,   429,   430,   431,   432,   404,   405,     0,
       0,     0,     0,     0,     0,   433,     0,   434,     0,     0,
       0,     0,   406,   407,   408,   409,   410,     0,     0,   411,
     412,   413,   414,     0,   415,   416,   417,   418,     0,     0,
     419,     0,   420,   421,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   422,   671,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   404,   405,     0,     0,
       0,     0,     0,     0,   433,     0,   434,   406,   407,   408,
     409,   410,     0,     0,   411,   412,   413,   414,     0,   415,
     416,   417,   418,     0,     0,   419,     0,   420,   421,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   732,     0,     0,   422,
       0,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   404,   405,     0,     0,     0,     0,     0,     0,   433,
       0,   434,     0,     0,     0,     0,   406,   407,   408,   409,
     410,     0,     0,   411,   412,   413,   414,     0,   415,   416,
     417,   418,     0,     0,   419,     0,   420,   421,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   422,     0,
     423,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     404,   405,     0,     0,     0,     0,     0,     0,   433,     0,
     434,   406,   407,     0,     0,   410,     0,     0,   411,   412,
     413,   414,     0,   415,   416,   417,   418,     0,     0,   419,
       0,   420,   421,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   422,     0,   423,   424,   425,   426,   427,
       0,     0,     0,   431,   432,   404,   405,     0,     0,     0,
       0,     0,     0,   433,     0,   434,     0,     0,     0,     0,
     406,   407,   408,   409,   410,     0,     0,   411,     0,     0,
     414,     0,   415,   416,   417,   418,     0,     0,   419,     0,
     420,   421,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   426,   427,   428,
     429,   430,   431,   432,     0,     0,     0,     0,     0,     0,
       0,     0,   433,     0,   434,   406,   407,   408,   409,     0,
       0,     0,     0,     0,     0,   414,     0,   415,   416,   417,
     418,     0,     0,   419,     0,   420,   421,     0,     0,     0,
       0,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
       0,     0,     0,     0,   428,   429,   430,   431,   432,   134,
     135,   136,   137,     0,     0,     0,     0,   433,     0,   434,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,     0,     0,   148,   149
};

static const yytype_int16 yycheck[] =
{
      13,   267,   225,   154,   368,    71,   295,   336,   389,   338,
     105,   340,   244,   245,   246,   260,    13,    14,   460,   366,
      18,     0,    32,    19,    20,    20,     5,   374,   537,   123,
     265,    17,   110,   436,   115,   133,    18,    18,    45,   112,
     113,   105,   133,   133,   109,   133,   133,   112,   113,    28,
     133,    30,   127,    32,   133,    34,   133,    52,    31,    72,
      39,   133,   140,   146,   163,   166,    45,   171,   139,   170,
     139,   133,    51,   172,   172,   142,   133,   171,   110,   133,
     139,   172,   172,    13,    14,   172,   169,   139,    67,   170,
     163,   169,   105,   172,    33,   172,   161,   169,   163,   170,
     113,   170,   505,   165,   171,   101,   102,   139,   140,    88,
     142,   170,   147,   109,   166,   111,   112,   113,   114,   164,
      56,   117,   469,   133,   359,   170,   133,   139,   133,   224,
     127,   478,   135,   136,   131,   170,   133,   134,   140,   648,
     110,   139,   584,   238,   239,   240,   241,   133,   118,   244,
     245,   246,   247,   219,   166,   502,   151,   139,   139,   169,
     224,   139,   101,   102,   259,   161,   161,   163,   165,   133,
     140,   403,   145,   139,   238,   239,   240,   241,   140,   274,
     244,   245,   246,   110,   163,     4,   281,     6,   166,   142,
     163,   170,   142,   459,   133,   259,   139,   127,   293,   294,
     173,   131,   163,   133,   134,   166,   139,   133,   170,   454,
     274,   224,   541,   140,    19,    20,   169,   281,   139,    31,
     139,   133,   169,   166,    36,   238,   239,   240,   241,   293,
     294,   244,   245,   246,   247,   386,   147,   388,   140,   642,
     529,   133,   139,    55,   139,   166,   259,   166,   471,   652,
     139,   147,   165,   147,   139,   602,   164,   142,    31,   170,
     145,   274,   170,    36,   645,   668,   613,   164,   281,   164,
     365,   164,   653,   133,   170,   164,   170,   170,   127,   133,
     293,   294,    55,    10,   379,   380,   133,   382,   383,   171,
     693,   135,   136,   137,    21,    22,   101,   102,   110,   111,
     170,   365,   172,   535,   109,   133,   111,   112,   113,   114,
     167,   168,   117,   171,   171,   379,   380,   468,   382,   383,
     171,   142,    19,    20,   170,   170,   172,   172,   140,   141,
     170,   660,   172,   145,   133,   171,   165,    33,   111,    33,
       1,    52,   135,   438,   171,   150,   151,   152,   133,   133,
     142,   163,   365,   170,   165,   450,   161,   452,   163,   146,
     146,   173,   170,   146,   146,   146,   379,   380,   141,   382,
     383,   146,   145,   142,   438,   146,   146,   600,   146,   146,
     170,   133,   171,   747,    42,   608,   450,   400,   133,   163,
     163,   170,   133,    19,    20,   146,   619,   170,   146,   165,
     173,   165,    99,   100,   101,   102,   103,   165,   146,   106,
     107,   108,   109,   146,   111,   112,   113,   114,   168,   163,
     117,   145,   119,   120,   166,   438,   165,   164,   133,   133,
     133,   133,   133,   133,   147,   147,    31,   450,   147,   452,
     535,    36,   236,   666,   115,   170,   127,   542,   145,   146,
     147,   148,   149,   150,   151,   152,   722,    35,    62,   135,
      55,   255,    66,   170,   161,    18,   163,   139,   133,   164,
     139,   535,    31,   624,   697,   101,   102,    36,   542,    83,
      84,    85,    86,   109,   170,   170,   112,   113,   114,   172,
     170,   117,   170,   133,    12,   165,    55,   291,   292,   142,
     127,   295,    38,   171,   133,   165,   147,   164,   133,   303,
     304,   305,   306,   307,   308,   110,   111,   164,   164,   164,
     133,    48,   535,   118,   166,   748,   164,   164,   147,   542,
     164,   164,   164,   172,   393,   161,    53,   163,   633,    94,
      56,   787,   233,    54,    72,   140,   141,   704,   771,     1,
     145,   774,   111,   269,   317,   218,   248,   370,   167,    66,
     281,   452,   651,   624,   243,    19,    20,   389,   163,   633,
     782,    -1,   366,    -1,   368,   170,    -1,    -1,   173,    -1,
     374,    -1,   141,    -1,   378,    -1,   145,   381,   147,    -1,
      -1,    -1,    -1,   387,    -1,   389,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   406,   407,   173,    -1,   410,   411,   412,   413,
     633,   415,    -1,   417,   418,   419,   420,   421,   422,   423,
     424,   425,   426,   427,   428,   429,   430,   431,   432,    -1,
     434,    -1,    -1,    -1,    -1,    99,   100,   101,   102,   103,
      -1,    -1,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,   120,    -1,    -1,    -1,
      -1,   465,    -1,    -1,    -1,   469,    -1,    -1,   472,   473,
     775,    19,    20,   477,   478,    -1,    -1,    -1,    -1,   483,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    -1,
      -1,   796,    -1,    -1,    -1,    -1,    -1,   161,   502,   163,
      -1,   775,   506,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   734,   516,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   525,   796,    -1,   528,   529,    -1,    -1,   532,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   765,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   775,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,   111,   112,   113,   114,    -1,    -1,   117,
      -1,    -1,    -1,   796,    -1,    -1,    -1,   581,    -1,    -1,
      -1,   585,   586,   587,   588,   589,   590,   591,   592,   593,
     594,   595,   596,   597,   598,   599,    -1,    -1,   602,    -1,
     148,   149,   150,   151,   152,    -1,    -1,    -1,    -1,   613,
      -1,    -1,    -1,   161,    -1,   163,    -1,    -1,    -1,    -1,
      -1,    -1,   626,   627,    -1,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    -1,   637,   638,    -1,   640,    -1,    23,   643,
      -1,    -1,    -1,    -1,    29,   649,    -1,   651,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
     664,    -1,    -1,    -1,    -1,    -1,    -1,   671,    -1,    -1,
      -1,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,   708,   709,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   718,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   737,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   747,    -1,    -1,    -1,   751,   133,    -1,
      -1,    -1,   756,    -1,    -1,    -1,   760,    -1,   762,   763,
      -1,    -1,    -1,    -1,   768,     1,    -1,    -1,    -1,     5,
      -1,     7,     8,     9,    -1,    11,    -1,    13,    14,    15,
      16,    -1,    -1,    -1,   169,    -1,   790,    -1,    24,    25,
      26,    27,    -1,    -1,   798,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    31,    31,    -1,    -1,    -1,    36,
      36,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,   111,    -1,    -1,    50,    -1,    -1,    -1,    55,    55,
      -1,    -1,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,   141,    78,    79,    80,   145,    -1,    83,    84,    85,
      86,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   101,   102,    -1,    -1,    -1,
     170,    -1,    -1,   173,   111,   111,    31,    -1,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      55,    -1,    -1,    -1,   141,   141,    -1,    -1,   145,   145,
     147,    -1,   148,   149,   150,    -1,    -1,    -1,    -1,   155,
     156,    -1,    13,    14,    15,    16,   163,   163,   164,   165,
      -1,   167,   168,   169,    25,   171,   173,   173,    -1,    31,
      31,    -1,    -1,    -1,    36,    36,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,   111,    -1,    -1,    50,
      -1,    -1,    -1,    55,    55,    -1,    -1,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,   141,    78,    79,    80,
     145,    -1,    83,    84,    85,    86,    -1,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,   173,   111,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,   141,
     141,    -1,    -1,   145,   145,   147,    -1,   148,   149,   150,
      -1,    -1,    -1,    -1,   155,   156,    -1,    13,    14,    15,
      16,   163,   163,    -1,   165,    -1,   167,   168,   169,    25,
     171,   173,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    84,    85,
      86,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    31,
     116,    -1,    -1,    -1,    36,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,   150,    13,    14,    15,    16,   155,
     156,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,   165,
      -1,   167,   168,   169,   170,   171,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    -1,
      78,    79,    80,    -1,    -1,    83,    84,    85,    86,   141,
      -1,    89,    -1,   145,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   173,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
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
     155,   156,    31,    -1,    -1,    -1,    -1,    36,    -1,   164,
     165,    37,   167,   168,   169,    41,   171,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    84,    85,
      86,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    31,
     116,    -1,    -1,    -1,    36,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,   141,    55,    -1,    -1,   145,    -1,   147,    -1,
      -1,    -1,   148,   149,   150,    13,    14,    15,    16,   155,
     156,    -1,    20,    -1,   163,    -1,    -1,    25,    -1,   165,
      -1,   167,   168,   169,   173,   171,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    -1,
      78,    79,    80,    -1,    -1,    83,    84,    85,    86,   141,
      -1,    89,    -1,   145,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   173,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
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
      -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,   141,
      55,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,   148,
     149,   150,    13,    14,    15,    16,   155,   156,    -1,    -1,
      -1,   163,    -1,   165,    25,    -1,   165,   166,   167,   168,
     169,   173,   171,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,   111,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    -1,    78,    79,    80,
      -1,    -1,    83,    84,    85,    86,   141,    -1,    89,    -1,
     145,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    14,    15,    16,    -1,    -1,   148,   149,   150,
      -1,    -1,    -1,    25,   155,   156,    31,    -1,    -1,    -1,
      -1,    36,    -1,    -1,   165,    37,   167,   168,   169,    41,
     171,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    79,    80,    -1,
      -1,    83,    84,    85,    86,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,   141,    55,    -1,    -1,
     145,    -1,   147,    -1,    -1,    -1,   148,   149,   150,    13,
      14,    15,    16,   155,   156,    -1,    -1,    -1,   163,    -1,
      -1,    25,    -1,   165,   166,   167,   168,   169,   173,   171,
      -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    78,    79,    80,    -1,    -1,    83,
      84,    85,    86,   141,    -1,    89,    -1,   145,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,   173,    36,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,    13,    14,    15,
      16,   155,   156,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,   165,   166,   167,   168,   169,    -1,   171,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    84,    85,
      86,   141,    -1,    89,    -1,   145,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,   173,    36,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,   150,    13,    14,    15,    16,   155,
     156,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,   165,
     166,   167,   168,   169,    -1,   171,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    -1,
      78,    79,    80,    -1,    -1,    83,    84,    85,    86,   141,
      -1,    89,    -1,   145,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      -1,   173,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
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
      -1,    -1,   145,    -1,   147,    -1,    -1,    -1,   148,   149,
     150,    13,    14,    15,    16,   155,   156,    -1,    -1,    -1,
     163,    -1,    -1,    25,    -1,   165,   166,   167,   168,   169,
     173,   171,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    79,    80,    -1,
      -1,    83,    84,    85,    86,   141,    -1,    89,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,
     166,    -1,    -1,    31,    -1,    -1,    -1,   173,    36,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,    13,
      14,    15,    16,   155,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,   165,   166,   167,   168,   169,    -1,   171,
      -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    78,    79,    80,    -1,    -1,    83,
      84,    85,    86,   141,    -1,    89,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,   163,   110,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
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
     156,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,   165,
      37,   167,   168,   169,    41,   171,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    79,    80,    -1,    -1,    83,    84,    85,    86,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,   141,    11,    19,    20,   145,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,    23,    -1,    -1,    -1,   155,   156,
      29,    -1,    -1,   163,    -1,    -1,   166,    -1,   165,    -1,
     167,   168,   169,   173,   171,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    -1,    -1,    99,   100,   101,   102,   103,    -1,    -1,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,    19,
      20,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    19,    20,   117,   118,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,
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
      -1,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,
      -1,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    -1,    -1,   117,   118,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,    -1,    -1,    19,    20,    -1,    -1,
      -1,   161,    -1,   163,    99,   100,   101,   102,   103,    -1,
     170,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,    -1,
      -1,    19,    20,    -1,    -1,    -1,   161,    -1,   163,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    99,   100,   101,   102,
     103,    -1,    -1,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
     163,    99,   100,   101,   102,   103,    -1,   170,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,    -1,    -1,   117,
      -1,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,    -1,    -1,    19,    20,
      -1,    -1,    -1,   161,    -1,   163,    -1,    -1,   166,    -1,
      99,   100,   101,   102,   103,    -1,    -1,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,    -1,    19,    20,    -1,    -1,
      -1,    -1,   161,    -1,   163,    -1,    -1,   166,    99,   100,
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
     147,   148,   149,   150,   151,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,   163,    -1,    -1,   166,
      99,   100,   101,   102,   103,    -1,    -1,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      29,    -1,   141,    -1,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,   163,    -1,    -1,   166,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    -1,    -1,
     169,    -1,    -1,    -1,    99,   100,   101,   102,   103,    -1,
      -1,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      29,   146,   147,   148,   149,   150,   151,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,   161,    -1,   163,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,   169,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    19,    20,
     169,    99,   100,   101,   102,   103,    -1,    -1,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,    -1,    -1,   117,
      -1,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,    -1,   141,    -1,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,   163,   164,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    99,   100,
     101,   102,   103,    -1,    -1,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,    -1,    -1,   117,    -1,   119,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,   163,   164,    19,    20,    -1,    99,   100,   101,
     102,   103,    -1,    -1,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,   163,   164,    -1,    -1,    -1,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,    -1,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    -1,    -1,   117,   118,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,   163,    99,   100,   101,   102,   103,    -1,    -1,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,    -1,
      -1,   117,    -1,   119,   120,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,   163,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,   103,    -1,    -1,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,    -1,    -1,
     117,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,   163,    99,   100,   101,
     102,   103,    -1,    -1,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,   141,
      -1,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,   163,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
     103,    -1,    -1,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
     163,    99,   100,    -1,    -1,   103,    -1,    -1,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,    -1,    -1,   117,
      -1,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,   143,   144,   145,   146,   147,
      -1,    -1,    -1,   151,   152,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,   163,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,   163,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,   120,    -1,    -1,    -1,
      -1,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   161,    -1,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,    -1,   155,   156
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   175,     0,     5,    28,    30,    32,    34,    39,    45,
      51,    67,    88,   163,   170,   176,   181,   182,   186,   205,
     209,   223,   255,   260,   261,   264,   273,   275,   133,   200,
     201,   127,   187,   188,   133,   183,   184,   133,   171,   133,
     276,   274,    32,   133,   169,   202,   203,   204,   214,     4,
       6,    56,   254,   140,   139,   142,   139,    20,    52,   151,
     161,   185,   142,   171,   133,   258,   259,   133,   133,   169,
     133,   165,   139,   164,   133,   263,    33,   101,   102,   133,
     208,   171,   256,    13,    14,   127,   131,   133,   134,   165,
     178,   200,   127,   188,   133,   133,   133,    62,    66,    83,
      84,    85,    86,   267,   257,   140,   172,   258,   171,   171,
     133,   201,   203,   142,   262,   171,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   119,   120,   121,   122,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   155,   156,
      33,    33,   165,   206,   253,     1,   135,   177,    13,    14,
     127,   131,   133,   178,   198,   199,   185,   171,   133,   172,
      23,    29,    44,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,   214,   266,   268,   269,   271,
     272,   133,   270,   133,   246,   247,   166,   214,   241,   163,
     166,   238,   242,   243,   142,   207,   133,   172,   252,   265,
     170,   135,   136,   139,   166,   257,   140,   170,   146,   146,
     146,   146,   165,   146,   146,   146,   146,   146,   146,    31,
      36,    55,   111,   141,   145,   163,   170,   173,   170,   142,
     170,   172,   238,   240,   201,    45,   249,   265,   166,   170,
     272,   171,   210,   139,   142,   145,   251,   198,   172,    13,
      14,    15,    16,    25,    37,    41,    43,    46,    50,    67,
      89,   101,   102,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   134,   148,   149,   150,   155,   156,   165,   167,
     168,   180,   210,   214,   216,   225,   226,   229,   230,   231,
     234,   235,   236,   237,   266,   277,   280,   285,   287,   289,
     272,   272,   272,   272,   133,   270,   206,   272,   206,   272,
     206,   272,   133,   244,   245,   272,   247,    31,   145,   163,
     173,   164,   237,   133,   172,   272,   172,   246,    42,   239,
     170,   164,   249,   110,   140,   142,   248,   242,   212,   133,
     272,   110,   118,   140,   250,   272,   277,   165,   165,   146,
     146,   165,   146,   146,   237,   237,    11,   272,    11,   272,
     237,   282,   286,   179,   237,   237,   237,   237,   237,   237,
     168,   133,   165,   205,    19,    20,    99,   100,   101,   102,
     103,   106,   107,   108,   109,   111,   112,   113,   114,   117,
     119,   120,   141,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   161,   163,   170,   165,   147,   170,   147,
     147,   166,   147,   170,   207,   147,   207,   147,   207,   147,
     142,   147,   170,   147,   170,   145,   164,   164,   249,   272,
     116,   213,   232,   237,     1,     7,     8,     9,    11,    24,
      26,    27,    36,    40,    47,    49,    53,    54,    90,   170,
     172,   189,   190,   193,   194,   195,   196,   197,   211,   213,
     215,   217,   218,   219,   220,   221,   223,   224,   232,   233,
     237,   170,   250,   213,   237,   165,   133,   237,   272,   272,
     237,   272,   272,   265,   133,   164,   165,   237,   278,   279,
     281,   282,   283,   284,   265,   165,   279,   284,   123,   170,
     172,   136,   137,   177,   166,   146,   214,   163,   166,   227,
     237,   206,    17,   133,   133,   237,   237,   237,   237,   237,
     237,   133,   237,   133,   237,   237,   237,   237,   237,    20,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   133,   237,   166,   227,   272,   147,   147,   147,   272,
     244,   248,   168,   230,   115,    91,    92,    93,    94,    95,
      96,    97,    98,   104,   105,   110,   118,   121,   122,   140,
     237,   265,   110,   170,   213,   237,   210,   237,   237,   170,
     127,    53,   237,   110,   213,   237,    35,   170,   170,   237,
     170,   170,   170,    18,   222,   170,   110,   140,   213,   237,
     170,   166,   227,    17,   146,   237,   166,   147,   147,   166,
     147,   147,    18,   248,   164,   166,   139,   164,   170,   139,
     164,   170,    18,   166,   172,   172,   237,   282,   172,   237,
     206,   272,   133,   278,   139,   166,   207,   272,   165,   164,
     133,   142,   164,   166,   147,   237,   230,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   210,    18,   213,   237,   170,    12,   210,   142,
     127,   213,   237,   170,   171,   210,    38,   252,   116,   116,
     170,   166,   272,   133,   166,   237,   237,   237,   165,   227,
     237,   279,   133,   164,   278,   237,   164,   283,   227,   279,
     164,   164,   138,   207,   147,   164,   237,   123,   210,   228,
     166,   227,   237,   227,   170,   210,   170,   212,    10,    21,
      22,   191,   192,   237,   237,   166,   147,   170,   166,   237,
     170,   164,   248,   170,   172,   147,   214,   166,   110,   237,
     166,   170,   172,   210,   237,    17,   237,   133,   166,   237,
     164,   237,   237,   164,   214,   237,   210,   210,   272,   166,
     147,   170,   288,   288,   192,   166,    17,   237,    48,   164,
     172,   272,   166,   237,   164,   164,   166
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
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   267,   267,   267,   267,   267,   267,   268,
     269,   269,   270,   270,   271,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   274,   273,   276,
     275,   277,   277,   277,   277,   278,   278,   279,   279,   280,
     280,   280,   280,   280,   281,   281,   282,   283,   283,   284,
     284,   285,   285,   286,   286,   287,   288,   288,   289,   289
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
       1,     1,     3,     5,     3,     3,     1,     1,     1,     0,
       1,     4,     6,     5,     5,     4,     0,     2,     0,     1,
       5,     0,     5,     0,     3,     5,     4,     1,     2,     4,
       5,     7,     0,     2,     2,     6,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     1,     3,     4,     1,     1,     1,     1,     4,
       3,     4,     2,     3,     2,     4,     3,     2,     2,     3,
       2,     4,     2,     4,     6,     4,     1,     4,     5,     1,
       4,     5,     1,     4,     5,     4,     4,     0,     7,     0,
       7,     1,     1,     1,     1,     3,     5,     1,     3,     5,
       4,     7,     5,     7,     1,     3,     3,     1,     1,     1,
       3,     5,     5,     1,     3,     4,     0,     3,    10,    10
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
#line 3137 "ds_parser.cpp"
        break;

    case 177: /* character_sequence  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3143 "ds_parser.cpp"
        break;

    case 178: /* string_constant  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3149 "ds_parser.cpp"
        break;

    case 179: /* string_builder_body  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3155 "ds_parser.cpp"
        break;

    case 180: /* string_builder  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3161 "ds_parser.cpp"
        break;

    case 183: /* require_module_name  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3167 "ds_parser.cpp"
        break;

    case 189: /* expression_label  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3173 "ds_parser.cpp"
        break;

    case 190: /* expression_goto  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3179 "ds_parser.cpp"
        break;

    case 192: /* expression_else  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3185 "ds_parser.cpp"
        break;

    case 194: /* expression_if_then_else  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3191 "ds_parser.cpp"
        break;

    case 195: /* expression_for_loop  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3197 "ds_parser.cpp"
        break;

    case 196: /* expression_while_loop  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3203 "ds_parser.cpp"
        break;

    case 197: /* expression_with  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3209 "ds_parser.cpp"
        break;

    case 198: /* annotation_argument_value  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3215 "ds_parser.cpp"
        break;

    case 199: /* annotation_argument_value_list  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3221 "ds_parser.cpp"
        break;

    case 200: /* annotation_argument  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3227 "ds_parser.cpp"
        break;

    case 201: /* annotation_argument_list  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3233 "ds_parser.cpp"
        break;

    case 202: /* annotation_declaration_name  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3239 "ds_parser.cpp"
        break;

    case 203: /* annotation_declaration  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3245 "ds_parser.cpp"
        break;

    case 204: /* annotation_list  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3251 "ds_parser.cpp"
        break;

    case 205: /* optional_annotation_list  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3257 "ds_parser.cpp"
        break;

    case 206: /* optional_function_argument_list  */
#line 111 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3263 "ds_parser.cpp"
        break;

    case 207: /* optional_function_type  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3269 "ds_parser.cpp"
        break;

    case 208: /* function_name  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3275 "ds_parser.cpp"
        break;

    case 210: /* expression_block  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3281 "ds_parser.cpp"
        break;

    case 211: /* expression_any  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3287 "ds_parser.cpp"
        break;

    case 212: /* expressions  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3293 "ds_parser.cpp"
        break;

    case 213: /* expr_pipe  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3299 "ds_parser.cpp"
        break;

    case 214: /* name_in_namespace  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3305 "ds_parser.cpp"
        break;

    case 215: /* expression_delete  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3311 "ds_parser.cpp"
        break;

    case 216: /* expr_new  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3317 "ds_parser.cpp"
        break;

    case 217: /* expression_break  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3323 "ds_parser.cpp"
        break;

    case 218: /* expression_continue  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3329 "ds_parser.cpp"
        break;

    case 219: /* expression_return  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3335 "ds_parser.cpp"
        break;

    case 220: /* expression_yield  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3341 "ds_parser.cpp"
        break;

    case 221: /* expression_try_catch  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3347 "ds_parser.cpp"
        break;

    case 224: /* expression_let  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3353 "ds_parser.cpp"
        break;

    case 225: /* expr_cast  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3359 "ds_parser.cpp"
        break;

    case 226: /* expr_type_info  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3365 "ds_parser.cpp"
        break;

    case 227: /* expr_list  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3371 "ds_parser.cpp"
        break;

    case 228: /* block_or_simple_block  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3377 "ds_parser.cpp"
        break;

    case 230: /* expr_block  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3383 "ds_parser.cpp"
        break;

    case 231: /* expr_numeric_const  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3389 "ds_parser.cpp"
        break;

    case 232: /* expr_assign  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3395 "ds_parser.cpp"
        break;

    case 233: /* expr_assign_pipe  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3401 "ds_parser.cpp"
        break;

    case 234: /* expr_named_call  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3407 "ds_parser.cpp"
        break;

    case 235: /* expr_method_call  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3413 "ds_parser.cpp"
        break;

    case 236: /* func_addr_expr  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3419 "ds_parser.cpp"
        break;

    case 237: /* expr  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3425 "ds_parser.cpp"
        break;

    case 238: /* optional_field_annotation  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3431 "ds_parser.cpp"
        break;

    case 240: /* structure_variable_declaration  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3437 "ds_parser.cpp"
        break;

    case 241: /* struct_variable_declaration_list  */
#line 111 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3443 "ds_parser.cpp"
        break;

    case 242: /* function_argument_declaration  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3449 "ds_parser.cpp"
        break;

    case 243: /* function_argument_list  */
#line 111 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3455 "ds_parser.cpp"
        break;

    case 244: /* tuple_type  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3461 "ds_parser.cpp"
        break;

    case 245: /* tuple_type_list  */
#line 111 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3467 "ds_parser.cpp"
        break;

    case 246: /* variant_type  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3473 "ds_parser.cpp"
        break;

    case 247: /* variant_type_list  */
#line 111 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3479 "ds_parser.cpp"
        break;

    case 249: /* variable_declaration  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3485 "ds_parser.cpp"
        break;

    case 252: /* let_variable_declaration  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3491 "ds_parser.cpp"
        break;

    case 253: /* global_variable_declaration_list  */
#line 111 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3497 "ds_parser.cpp"
        break;

    case 257: /* enum_list  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3503 "ds_parser.cpp"
        break;

    case 262: /* optional_structure_parent  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3509 "ds_parser.cpp"
        break;

    case 265: /* variable_name_list  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3515 "ds_parser.cpp"
        break;

    case 268: /* structure_type_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3521 "ds_parser.cpp"
        break;

    case 269: /* auto_type_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3527 "ds_parser.cpp"
        break;

    case 270: /* bitfield_bits  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3533 "ds_parser.cpp"
        break;

    case 271: /* bitfield_type_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3539 "ds_parser.cpp"
        break;

    case 272: /* type_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3545 "ds_parser.cpp"
        break;

    case 277: /* make_decl  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3551 "ds_parser.cpp"
        break;

    case 278: /* make_struct_fields  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3557 "ds_parser.cpp"
        break;

    case 279: /* make_struct_dim  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3563 "ds_parser.cpp"
        break;

    case 280: /* make_struct_decl  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3569 "ds_parser.cpp"
        break;

    case 281: /* make_tuple  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3575 "ds_parser.cpp"
        break;

    case 282: /* make_map_tuple  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3581 "ds_parser.cpp"
        break;

    case 283: /* make_any_tuple  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3587 "ds_parser.cpp"
        break;

    case 284: /* make_dim  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3593 "ds_parser.cpp"
        break;

    case 285: /* make_dim_decl  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3599 "ds_parser.cpp"
        break;

    case 286: /* make_table  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3605 "ds_parser.cpp"
        break;

    case 287: /* make_table_decl  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3611 "ds_parser.cpp"
        break;

    case 288: /* array_comprehension_where  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3617 "ds_parser.cpp"
        break;

    case 289: /* array_comprehension  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3623 "ds_parser.cpp"
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
#line 411 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3923 "ds_parser.cpp"
    break;

  case 16:
#line 423 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3929 "ds_parser.cpp"
    break;

  case 17:
#line 424 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3935 "ds_parser.cpp"
    break;

  case 18:
#line 428 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 3941 "ds_parser.cpp"
    break;

  case 19:
#line 432 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 3951 "ds_parser.cpp"
    break;

  case 20:
#line 437 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3965 "ds_parser.cpp"
    break;

  case 21:
#line 446 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3975 "ds_parser.cpp"
    break;

  case 22:
#line 454 "ds_parser.ypp"
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
#line 3993 "ds_parser.cpp"
    break;

  case 23:
#line 470 "ds_parser.ypp"
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
#line 4017 "ds_parser.cpp"
    break;

  case 25:
#line 496 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4025 "ds_parser.cpp"
    break;

  case 26:
#line 499 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4036 "ds_parser.cpp"
    break;

  case 27:
#line 505 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4047 "ds_parser.cpp"
    break;

  case 28:
#line 514 "ds_parser.ypp"
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
#line 4073 "ds_parser.cpp"
    break;

  case 29:
#line 535 "ds_parser.ypp"
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
#line 4098 "ds_parser.cpp"
    break;

  case 30:
#line 558 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4104 "ds_parser.cpp"
    break;

  case 31:
#line 559 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4110 "ds_parser.cpp"
    break;

  case 35:
#line 572 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4118 "ds_parser.cpp"
    break;

  case 36:
#line 575 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4126 "ds_parser.cpp"
    break;

  case 37:
#line 581 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4134 "ds_parser.cpp"
    break;

  case 38:
#line 587 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4142 "ds_parser.cpp"
    break;

  case 39:
#line 590 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4150 "ds_parser.cpp"
    break;

  case 40:
#line 596 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4156 "ds_parser.cpp"
    break;

  case 41:
#line 597 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4162 "ds_parser.cpp"
    break;

  case 42:
#line 601 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4168 "ds_parser.cpp"
    break;

  case 43:
#line 602 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4174 "ds_parser.cpp"
    break;

  case 44:
#line 603 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4185 "ds_parser.cpp"
    break;

  case 45:
#line 612 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4191 "ds_parser.cpp"
    break;

  case 46:
#line 613 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4197 "ds_parser.cpp"
    break;

  case 47:
#line 617 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4208 "ds_parser.cpp"
    break;

  case 48:
#line 626 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 4222 "ds_parser.cpp"
    break;

  case 49:
#line 638 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4234 "ds_parser.cpp"
    break;

  case 50:
#line 648 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4245 "ds_parser.cpp"
    break;

  case 51:
#line 657 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4251 "ds_parser.cpp"
    break;

  case 52:
#line 658 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4257 "ds_parser.cpp"
    break;

  case 53:
#line 659 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4263 "ds_parser.cpp"
    break;

  case 54:
#line 660 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4269 "ds_parser.cpp"
    break;

  case 55:
#line 661 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4275 "ds_parser.cpp"
    break;

  case 56:
#line 662 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4281 "ds_parser.cpp"
    break;

  case 57:
#line 666 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4291 "ds_parser.cpp"
    break;

  case 58:
#line 671 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4301 "ds_parser.cpp"
    break;

  case 59:
#line 679 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4307 "ds_parser.cpp"
    break;

  case 60:
#line 680 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4313 "ds_parser.cpp"
    break;

  case 61:
#line 681 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4319 "ds_parser.cpp"
    break;

  case 62:
#line 682 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4325 "ds_parser.cpp"
    break;

  case 63:
#line 683 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4331 "ds_parser.cpp"
    break;

  case 64:
#line 684 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4337 "ds_parser.cpp"
    break;

  case 65:
#line 685 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4343 "ds_parser.cpp"
    break;

  case 66:
#line 686 "ds_parser.ypp"
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4351 "ds_parser.cpp"
    break;

  case 67:
#line 692 "ds_parser.ypp"
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
#line 4369 "ds_parser.cpp"
    break;

  case 68:
#line 705 "ds_parser.ypp"
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
#line 4387 "ds_parser.cpp"
    break;

  case 69:
#line 721 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4393 "ds_parser.cpp"
    break;

  case 70:
#line 722 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4399 "ds_parser.cpp"
    break;

  case 71:
#line 726 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4411 "ds_parser.cpp"
    break;

  case 72:
#line 733 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4425 "ds_parser.cpp"
    break;

  case 73:
#line 745 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4434 "ds_parser.cpp"
    break;

  case 74:
#line 749 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4443 "ds_parser.cpp"
    break;

  case 75:
#line 756 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4449 "ds_parser.cpp"
    break;

  case 76:
#line 757 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4455 "ds_parser.cpp"
    break;

  case 77:
#line 761 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4461 "ds_parser.cpp"
    break;

  case 78:
#line 762 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4467 "ds_parser.cpp"
    break;

  case 79:
#line 763 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4473 "ds_parser.cpp"
    break;

  case 80:
#line 767 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4483 "ds_parser.cpp"
    break;

  case 81:
#line 772 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4492 "ds_parser.cpp"
    break;

  case 82:
#line 779 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s);
	}
#line 4501 "ds_parser.cpp"
    break;

  case 83:
#line 783 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4507 "ds_parser.cpp"
    break;

  case 84:
#line 784 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4513 "ds_parser.cpp"
    break;

  case 85:
#line 785 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4519 "ds_parser.cpp"
    break;

  case 86:
#line 786 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4525 "ds_parser.cpp"
    break;

  case 87:
#line 787 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4531 "ds_parser.cpp"
    break;

  case 88:
#line 788 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4537 "ds_parser.cpp"
    break;

  case 89:
#line 789 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4543 "ds_parser.cpp"
    break;

  case 90:
#line 790 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4549 "ds_parser.cpp"
    break;

  case 91:
#line 791 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4555 "ds_parser.cpp"
    break;

  case 92:
#line 792 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4561 "ds_parser.cpp"
    break;

  case 93:
#line 793 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4567 "ds_parser.cpp"
    break;

  case 94:
#line 794 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4573 "ds_parser.cpp"
    break;

  case 95:
#line 795 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4579 "ds_parser.cpp"
    break;

  case 96:
#line 796 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4585 "ds_parser.cpp"
    break;

  case 97:
#line 797 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4591 "ds_parser.cpp"
    break;

  case 98:
#line 798 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4597 "ds_parser.cpp"
    break;

  case 99:
#line 799 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4603 "ds_parser.cpp"
    break;

  case 100:
#line 800 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4609 "ds_parser.cpp"
    break;

  case 101:
#line 801 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4615 "ds_parser.cpp"
    break;

  case 102:
#line 802 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4621 "ds_parser.cpp"
    break;

  case 103:
#line 803 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4627 "ds_parser.cpp"
    break;

  case 104:
#line 804 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4633 "ds_parser.cpp"
    break;

  case 105:
#line 805 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4639 "ds_parser.cpp"
    break;

  case 106:
#line 806 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4645 "ds_parser.cpp"
    break;

  case 107:
#line 807 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4651 "ds_parser.cpp"
    break;

  case 108:
#line 808 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4657 "ds_parser.cpp"
    break;

  case 109:
#line 809 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4663 "ds_parser.cpp"
    break;

  case 110:
#line 810 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4669 "ds_parser.cpp"
    break;

  case 111:
#line 811 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4675 "ds_parser.cpp"
    break;

  case 112:
#line 812 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4681 "ds_parser.cpp"
    break;

  case 113:
#line 813 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4687 "ds_parser.cpp"
    break;

  case 114:
#line 814 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4693 "ds_parser.cpp"
    break;

  case 115:
#line 815 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4699 "ds_parser.cpp"
    break;

  case 116:
#line 816 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4705 "ds_parser.cpp"
    break;

  case 117:
#line 817 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4711 "ds_parser.cpp"
    break;

  case 118:
#line 818 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4717 "ds_parser.cpp"
    break;

  case 119:
#line 823 "ds_parser.ypp"
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
#line 4792 "ds_parser.cpp"
    break;

  case 120:
#line 896 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4800 "ds_parser.cpp"
    break;

  case 121:
#line 899 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4812 "ds_parser.cpp"
    break;

  case 122:
#line 909 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4818 "ds_parser.cpp"
    break;

  case 123:
#line 910 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4824 "ds_parser.cpp"
    break;

  case 124:
#line 911 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4830 "ds_parser.cpp"
    break;

  case 125:
#line 912 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4836 "ds_parser.cpp"
    break;

  case 126:
#line 913 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4842 "ds_parser.cpp"
    break;

  case 127:
#line 914 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4848 "ds_parser.cpp"
    break;

  case 128:
#line 915 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4854 "ds_parser.cpp"
    break;

  case 129:
#line 916 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4860 "ds_parser.cpp"
    break;

  case 130:
#line 917 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4866 "ds_parser.cpp"
    break;

  case 131:
#line 918 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4872 "ds_parser.cpp"
    break;

  case 132:
#line 919 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4878 "ds_parser.cpp"
    break;

  case 133:
#line 920 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4884 "ds_parser.cpp"
    break;

  case 134:
#line 921 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4890 "ds_parser.cpp"
    break;

  case 135:
#line 922 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4896 "ds_parser.cpp"
    break;

  case 136:
#line 923 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4902 "ds_parser.cpp"
    break;

  case 137:
#line 924 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4908 "ds_parser.cpp"
    break;

  case 138:
#line 925 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4914 "ds_parser.cpp"
    break;

  case 139:
#line 926 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4920 "ds_parser.cpp"
    break;

  case 140:
#line 930 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4930 "ds_parser.cpp"
    break;

  case 141:
#line 935 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4941 "ds_parser.cpp"
    break;

  case 142:
#line 941 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4949 "ds_parser.cpp"
    break;

  case 143:
#line 947 "ds_parser.ypp"
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
#line 4965 "ds_parser.cpp"
    break;

  case 144:
#line 958 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4973 "ds_parser.cpp"
    break;

  case 145:
#line 964 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4979 "ds_parser.cpp"
    break;

  case 146:
#line 965 "ds_parser.ypp"
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
#line 4995 "ds_parser.cpp"
    break;

  case 147:
#line 976 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5001 "ds_parser.cpp"
    break;

  case 148:
#line 980 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5009 "ds_parser.cpp"
    break;

  case 149:
#line 986 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5017 "ds_parser.cpp"
    break;

  case 150:
#line 989 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5025 "ds_parser.cpp"
    break;

  case 151:
#line 992 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5034 "ds_parser.cpp"
    break;

  case 152:
#line 996 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5042 "ds_parser.cpp"
    break;

  case 153:
#line 1002 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5048 "ds_parser.cpp"
    break;

  case 154:
#line 1006 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5054 "ds_parser.cpp"
    break;

  case 155:
#line 1010 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5062 "ds_parser.cpp"
    break;

  case 156:
#line 1013 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5070 "ds_parser.cpp"
    break;

  case 157:
#line 1016 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5080 "ds_parser.cpp"
    break;

  case 158:
#line 1021 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5088 "ds_parser.cpp"
    break;

  case 159:
#line 1024 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5098 "ds_parser.cpp"
    break;

  case 160:
#line 1032 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5106 "ds_parser.cpp"
    break;

  case 161:
#line 1035 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5116 "ds_parser.cpp"
    break;

  case 162:
#line 1040 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5124 "ds_parser.cpp"
    break;

  case 163:
#line 1043 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5134 "ds_parser.cpp"
    break;

  case 164:
#line 1051 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5142 "ds_parser.cpp"
    break;

  case 165:
#line 1057 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5148 "ds_parser.cpp"
    break;

  case 166:
#line 1058 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5154 "ds_parser.cpp"
    break;

  case 167:
#line 1062 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5160 "ds_parser.cpp"
    break;

  case 168:
#line 1063 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5166 "ds_parser.cpp"
    break;

  case 169:
#line 1067 "ds_parser.ypp"
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
#line 5202 "ds_parser.cpp"
    break;

  case 170:
#line 1101 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5210 "ds_parser.cpp"
    break;

  case 171:
#line 1104 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5220 "ds_parser.cpp"
    break;

  case 172:
#line 1109 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5230 "ds_parser.cpp"
    break;

  case 173:
#line 1117 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5239 "ds_parser.cpp"
    break;

  case 174:
#line 1121 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5249 "ds_parser.cpp"
    break;

  case 175:
#line 1126 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5260 "ds_parser.cpp"
    break;

  case 176:
#line 1132 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 5269 "ds_parser.cpp"
    break;

  case 177:
#line 1136 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5279 "ds_parser.cpp"
    break;

  case 178:
#line 1141 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5290 "ds_parser.cpp"
    break;

  case 179:
#line 1150 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5298 "ds_parser.cpp"
    break;

  case 180:
#line 1153 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5306 "ds_parser.cpp"
    break;

  case 181:
#line 1159 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5312 "ds_parser.cpp"
    break;

  case 182:
#line 1160 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5324 "ds_parser.cpp"
    break;

  case 183:
#line 1167 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5337 "ds_parser.cpp"
    break;

  case 184:
#line 1178 "ds_parser.ypp"
    { (yyval.b) = false;   /* block */  }
#line 5343 "ds_parser.cpp"
    break;

  case 185:
#line 1179 "ds_parser.ypp"
    { (yyval.b) = true;    /* lambda */ }
#line 5349 "ds_parser.cpp"
    break;

  case 186:
#line 1183 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5359 "ds_parser.cpp"
    break;

  case 187:
#line 1189 "ds_parser.ypp"
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
#line 5415 "ds_parser.cpp"
    break;

  case 188:
#line 1243 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5421 "ds_parser.cpp"
    break;

  case 189:
#line 1244 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5427 "ds_parser.cpp"
    break;

  case 190:
#line 1245 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5433 "ds_parser.cpp"
    break;

  case 191:
#line 1246 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5439 "ds_parser.cpp"
    break;

  case 192:
#line 1247 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5445 "ds_parser.cpp"
    break;

  case 193:
#line 1248 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5451 "ds_parser.cpp"
    break;

  case 194:
#line 1252 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5457 "ds_parser.cpp"
    break;

  case 195:
#line 1253 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5463 "ds_parser.cpp"
    break;

  case 196:
#line 1254 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5469 "ds_parser.cpp"
    break;

  case 197:
#line 1255 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5475 "ds_parser.cpp"
    break;

  case 198:
#line 1256 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5481 "ds_parser.cpp"
    break;

  case 199:
#line 1257 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5487 "ds_parser.cpp"
    break;

  case 200:
#line 1258 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5493 "ds_parser.cpp"
    break;

  case 201:
#line 1259 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5499 "ds_parser.cpp"
    break;

  case 202:
#line 1260 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5505 "ds_parser.cpp"
    break;

  case 203:
#line 1261 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5511 "ds_parser.cpp"
    break;

  case 204:
#line 1262 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5517 "ds_parser.cpp"
    break;

  case 205:
#line 1263 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5523 "ds_parser.cpp"
    break;

  case 206:
#line 1264 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5529 "ds_parser.cpp"
    break;

  case 207:
#line 1265 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5535 "ds_parser.cpp"
    break;

  case 208:
#line 1266 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5541 "ds_parser.cpp"
    break;

  case 209:
#line 1267 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5547 "ds_parser.cpp"
    break;

  case 210:
#line 1271 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5553 "ds_parser.cpp"
    break;

  case 211:
#line 1272 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5559 "ds_parser.cpp"
    break;

  case 212:
#line 1276 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5571 "ds_parser.cpp"
    break;

  case 213:
#line 1287 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5581 "ds_parser.cpp"
    break;

  case 214:
#line 1292 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5593 "ds_parser.cpp"
    break;

  case 215:
#line 1302 "ds_parser.ypp"
    {
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
	}
#line 5602 "ds_parser.cpp"
    break;

  case 216:
#line 1306 "ds_parser.ypp"
    {
		auto expr = new ExprAddr(tokAt((yylsp[-4])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5614 "ds_parser.cpp"
    break;

  case 217:
#line 1313 "ds_parser.ypp"
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
#line 5630 "ds_parser.cpp"
    break;

  case 218:
#line 1327 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5636 "ds_parser.cpp"
    break;

  case 219:
#line 1328 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5642 "ds_parser.cpp"
    break;

  case 220:
#line 1329 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5648 "ds_parser.cpp"
    break;

  case 221:
#line 1330 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5654 "ds_parser.cpp"
    break;

  case 222:
#line 1331 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5660 "ds_parser.cpp"
    break;

  case 223:
#line 1332 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5666 "ds_parser.cpp"
    break;

  case 224:
#line 1333 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5672 "ds_parser.cpp"
    break;

  case 225:
#line 1334 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5678 "ds_parser.cpp"
    break;

  case 226:
#line 1335 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5684 "ds_parser.cpp"
    break;

  case 227:
#line 1336 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5690 "ds_parser.cpp"
    break;

  case 228:
#line 1337 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5696 "ds_parser.cpp"
    break;

  case 229:
#line 1338 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5702 "ds_parser.cpp"
    break;

  case 230:
#line 1339 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5708 "ds_parser.cpp"
    break;

  case 231:
#line 1340 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5714 "ds_parser.cpp"
    break;

  case 232:
#line 1341 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5720 "ds_parser.cpp"
    break;

  case 233:
#line 1342 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5726 "ds_parser.cpp"
    break;

  case 234:
#line 1343 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5732 "ds_parser.cpp"
    break;

  case 235:
#line 1344 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5738 "ds_parser.cpp"
    break;

  case 236:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5744 "ds_parser.cpp"
    break;

  case 237:
#line 1346 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5750 "ds_parser.cpp"
    break;

  case 238:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5756 "ds_parser.cpp"
    break;

  case 239:
#line 1348 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5762 "ds_parser.cpp"
    break;

  case 240:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5768 "ds_parser.cpp"
    break;

  case 241:
#line 1350 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5774 "ds_parser.cpp"
    break;

  case 242:
#line 1351 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5780 "ds_parser.cpp"
    break;

  case 243:
#line 1352 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5786 "ds_parser.cpp"
    break;

  case 244:
#line 1353 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5792 "ds_parser.cpp"
    break;

  case 245:
#line 1354 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5798 "ds_parser.cpp"
    break;

  case 246:
#line 1355 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5804 "ds_parser.cpp"
    break;

  case 247:
#line 1356 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5810 "ds_parser.cpp"
    break;

  case 248:
#line 1357 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5816 "ds_parser.cpp"
    break;

  case 249:
#line 1358 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5822 "ds_parser.cpp"
    break;

  case 250:
#line 1359 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5828 "ds_parser.cpp"
    break;

  case 251:
#line 1360 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5834 "ds_parser.cpp"
    break;

  case 252:
#line 1361 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5840 "ds_parser.cpp"
    break;

  case 253:
#line 1362 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5846 "ds_parser.cpp"
    break;

  case 254:
#line 1363 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5852 "ds_parser.cpp"
    break;

  case 255:
#line 1364 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5858 "ds_parser.cpp"
    break;

  case 256:
#line 1365 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5864 "ds_parser.cpp"
    break;

  case 257:
#line 1366 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5873 "ds_parser.cpp"
    break;

  case 258:
#line 1370 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5882 "ds_parser.cpp"
    break;

  case 259:
#line 1374 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5890 "ds_parser.cpp"
    break;

  case 260:
#line 1377 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5898 "ds_parser.cpp"
    break;

  case 261:
#line 1380 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5904 "ds_parser.cpp"
    break;

  case 262:
#line 1381 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5910 "ds_parser.cpp"
    break;

  case 263:
#line 1382 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5916 "ds_parser.cpp"
    break;

  case 264:
#line 1383 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5926 "ds_parser.cpp"
    break;

  case 265:
#line 1388 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5936 "ds_parser.cpp"
    break;

  case 266:
#line 1393 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5942 "ds_parser.cpp"
    break;

  case 267:
#line 1394 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5950 "ds_parser.cpp"
    break;

  case 268:
#line 1397 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5958 "ds_parser.cpp"
    break;

  case 269:
#line 1400 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5967 "ds_parser.cpp"
    break;

  case 270:
#line 1404 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5976 "ds_parser.cpp"
    break;

  case 271:
#line 1408 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 5985 "ds_parser.cpp"
    break;

  case 272:
#line 1412 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5991 "ds_parser.cpp"
    break;

  case 273:
#line 1413 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5997 "ds_parser.cpp"
    break;

  case 274:
#line 1414 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6003 "ds_parser.cpp"
    break;

  case 275:
#line 1415 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6009 "ds_parser.cpp"
    break;

  case 276:
#line 1416 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6015 "ds_parser.cpp"
    break;

  case 277:
#line 1417 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6021 "ds_parser.cpp"
    break;

  case 278:
#line 1418 "ds_parser.ypp"
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
#line 6036 "ds_parser.cpp"
    break;

  case 279:
#line 1428 "ds_parser.ypp"
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
#line 6051 "ds_parser.cpp"
    break;

  case 280:
#line 1438 "ds_parser.ypp"
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
#line 6108 "ds_parser.cpp"
    break;

  case 281:
#line 1493 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6114 "ds_parser.cpp"
    break;

  case 282:
#line 1494 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 6120 "ds_parser.cpp"
    break;

  case 283:
#line 1498 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6126 "ds_parser.cpp"
    break;

  case 284:
#line 1499 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6132 "ds_parser.cpp"
    break;

  case 285:
#line 1503 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6142 "ds_parser.cpp"
    break;

  case 286:
#line 1511 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6150 "ds_parser.cpp"
    break;

  case 287:
#line 1514 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6159 "ds_parser.cpp"
    break;

  case 288:
#line 1521 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
		}
#line 6169 "ds_parser.cpp"
    break;

  case 289:
#line 1526 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
		}
#line 6179 "ds_parser.cpp"
    break;

  case 290:
#line 1534 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6185 "ds_parser.cpp"
    break;

  case 291:
#line 1535 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6191 "ds_parser.cpp"
    break;

  case 292:
#line 1539 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6199 "ds_parser.cpp"
    break;

  case 293:
#line 1542 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6210 "ds_parser.cpp"
    break;

  case 294:
#line 1551 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6216 "ds_parser.cpp"
    break;

  case 295:
#line 1552 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6222 "ds_parser.cpp"
    break;

  case 296:
#line 1556 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6233 "ds_parser.cpp"
    break;

  case 297:
#line 1565 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6239 "ds_parser.cpp"
    break;

  case 298:
#line 1566 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6245 "ds_parser.cpp"
    break;

  case 299:
#line 1571 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6251 "ds_parser.cpp"
    break;

  case 300:
#line 1572 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6257 "ds_parser.cpp"
    break;

  case 301:
#line 1576 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 6267 "ds_parser.cpp"
    break;

  case 302:
#line 1581 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6275 "ds_parser.cpp"
    break;

  case 303:
#line 1584 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6284 "ds_parser.cpp"
    break;

  case 304:
#line 1588 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6295 "ds_parser.cpp"
    break;

  case 305:
#line 1594 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6306 "ds_parser.cpp"
    break;

  case 306:
#line 1603 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6312 "ds_parser.cpp"
    break;

  case 307:
#line 1604 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6318 "ds_parser.cpp"
    break;

  case 308:
#line 1605 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6324 "ds_parser.cpp"
    break;

  case 309:
#line 1609 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6330 "ds_parser.cpp"
    break;

  case 310:
#line 1610 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6336 "ds_parser.cpp"
    break;

  case 311:
#line 1614 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6344 "ds_parser.cpp"
    break;

  case 312:
#line 1617 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6354 "ds_parser.cpp"
    break;

  case 313:
#line 1622 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6364 "ds_parser.cpp"
    break;

  case 314:
#line 1627 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6377 "ds_parser.cpp"
    break;

  case 315:
#line 1635 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6390 "ds_parser.cpp"
    break;

  case 316:
#line 1646 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6398 "ds_parser.cpp"
    break;

  case 317:
#line 1649 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6407 "ds_parser.cpp"
    break;

  case 318:
#line 1656 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6413 "ds_parser.cpp"
    break;

  case 319:
#line 1657 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6419 "ds_parser.cpp"
    break;

  case 320:
#line 1661 "ds_parser.ypp"
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
#line 6451 "ds_parser.cpp"
    break;

  case 321:
#line 1688 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 6460 "ds_parser.cpp"
    break;

  case 322:
#line 1691 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 6471 "ds_parser.cpp"
    break;

  case 323:
#line 1700 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6479 "ds_parser.cpp"
    break;

  case 324:
#line 1703 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6493 "ds_parser.cpp"
    break;

  case 325:
#line 1712 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6507 "ds_parser.cpp"
    break;

  case 326:
#line 1725 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 6521 "ds_parser.cpp"
    break;

  case 330:
#line 1746 "ds_parser.ypp"
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
#line 6537 "ds_parser.cpp"
    break;

  case 331:
#line 1757 "ds_parser.ypp"
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
#line 6554 "ds_parser.cpp"
    break;

  case 332:
#line 1772 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 6560 "ds_parser.cpp"
    break;

  case 333:
#line 1773 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 6566 "ds_parser.cpp"
    break;

  case 334:
#line 1777 "ds_parser.ypp"
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
#line 6607 "ds_parser.cpp"
    break;

  case 335:
#line 1816 "ds_parser.ypp"
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
#line 6675 "ds_parser.cpp"
    break;

  case 336:
#line 1882 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6687 "ds_parser.cpp"
    break;

  case 337:
#line 1889 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6698 "ds_parser.cpp"
    break;

  case 338:
#line 1898 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 6704 "ds_parser.cpp"
    break;

  case 339:
#line 1899 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 6710 "ds_parser.cpp"
    break;

  case 340:
#line 1900 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6716 "ds_parser.cpp"
    break;

  case 341:
#line 1901 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6722 "ds_parser.cpp"
    break;

  case 342:
#line 1902 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6728 "ds_parser.cpp"
    break;

  case 343:
#line 1903 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 6734 "ds_parser.cpp"
    break;

  case 344:
#line 1904 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 6740 "ds_parser.cpp"
    break;

  case 345:
#line 1905 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 6746 "ds_parser.cpp"
    break;

  case 346:
#line 1906 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 6752 "ds_parser.cpp"
    break;

  case 347:
#line 1907 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6758 "ds_parser.cpp"
    break;

  case 348:
#line 1908 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6764 "ds_parser.cpp"
    break;

  case 349:
#line 1909 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6770 "ds_parser.cpp"
    break;

  case 350:
#line 1910 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 6776 "ds_parser.cpp"
    break;

  case 351:
#line 1911 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 6782 "ds_parser.cpp"
    break;

  case 352:
#line 1912 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 6788 "ds_parser.cpp"
    break;

  case 353:
#line 1913 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 6794 "ds_parser.cpp"
    break;

  case 354:
#line 1914 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 6800 "ds_parser.cpp"
    break;

  case 355:
#line 1915 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 6806 "ds_parser.cpp"
    break;

  case 356:
#line 1916 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 6812 "ds_parser.cpp"
    break;

  case 357:
#line 1917 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 6818 "ds_parser.cpp"
    break;

  case 358:
#line 1918 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 6824 "ds_parser.cpp"
    break;

  case 359:
#line 1919 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 6830 "ds_parser.cpp"
    break;

  case 360:
#line 1920 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 6836 "ds_parser.cpp"
    break;

  case 361:
#line 1921 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 6842 "ds_parser.cpp"
    break;

  case 362:
#line 1922 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 6848 "ds_parser.cpp"
    break;

  case 363:
#line 1926 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6854 "ds_parser.cpp"
    break;

  case 364:
#line 1927 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6860 "ds_parser.cpp"
    break;

  case 365:
#line 1928 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6866 "ds_parser.cpp"
    break;

  case 366:
#line 1929 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6872 "ds_parser.cpp"
    break;

  case 367:
#line 1930 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6878 "ds_parser.cpp"
    break;

  case 368:
#line 1931 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6884 "ds_parser.cpp"
    break;

  case 369:
#line 1935 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6897 "ds_parser.cpp"
    break;

  case 370:
#line 1946 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6906 "ds_parser.cpp"
    break;

  case 371:
#line 1950 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6918 "ds_parser.cpp"
    break;

  case 372:
#line 1960 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6930 "ds_parser.cpp"
    break;

  case 373:
#line 1967 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6941 "ds_parser.cpp"
    break;

  case 374:
#line 1976 "ds_parser.ypp"
    {
            (yyval.pTypeDecl) = new TypeDecl(Type::tBitfield);
            (yyval.pTypeDecl)->argNames = *(yyvsp[-1].pNameList);
            (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
            delete (yyvsp[-1].pNameList);
    }
#line 6952 "ds_parser.cpp"
    break;

  case 375:
#line 1985 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6958 "ds_parser.cpp"
    break;

  case 376:
#line 1986 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6964 "ds_parser.cpp"
    break;

  case 377:
#line 1987 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6970 "ds_parser.cpp"
    break;

  case 378:
#line 1988 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6976 "ds_parser.cpp"
    break;

  case 379:
#line 1989 "ds_parser.ypp"
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
#line 6995 "ds_parser.cpp"
    break;

  case 380:
#line 2003 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7006 "ds_parser.cpp"
    break;

  case 381:
#line 2009 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7015 "ds_parser.cpp"
    break;

  case 382:
#line 2013 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7025 "ds_parser.cpp"
    break;

  case 383:
#line 2018 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7035 "ds_parser.cpp"
    break;

  case 384:
#line 2023 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7045 "ds_parser.cpp"
    break;

  case 385:
#line 2028 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7057 "ds_parser.cpp"
    break;

  case 386:
#line 2035 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7067 "ds_parser.cpp"
    break;

  case 387:
#line 2040 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7076 "ds_parser.cpp"
    break;

  case 388:
#line 2044 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7085 "ds_parser.cpp"
    break;

  case 389:
#line 2048 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7095 "ds_parser.cpp"
    break;

  case 390:
#line 2053 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7105 "ds_parser.cpp"
    break;

  case 391:
#line 2058 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7116 "ds_parser.cpp"
    break;

  case 392:
#line 2064 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7128 "ds_parser.cpp"
    break;

  case 393:
#line 2071 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7138 "ds_parser.cpp"
    break;

  case 394:
#line 2076 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7149 "ds_parser.cpp"
    break;

  case 395:
#line 2082 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7159 "ds_parser.cpp"
    break;

  case 396:
#line 2087 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7168 "ds_parser.cpp"
    break;

  case 397:
#line 2091 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7178 "ds_parser.cpp"
    break;

  case 398:
#line 2096 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7192 "ds_parser.cpp"
    break;

  case 399:
#line 2105 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7201 "ds_parser.cpp"
    break;

  case 400:
#line 2109 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7211 "ds_parser.cpp"
    break;

  case 401:
#line 2114 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7225 "ds_parser.cpp"
    break;

  case 402:
#line 2123 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7234 "ds_parser.cpp"
    break;

  case 403:
#line 2127 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7244 "ds_parser.cpp"
    break;

  case 404:
#line 2132 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7258 "ds_parser.cpp"
    break;

  case 405:
#line 2141 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7269 "ds_parser.cpp"
    break;

  case 406:
#line 2147 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7280 "ds_parser.cpp"
    break;

  case 407:
#line 2156 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7286 "ds_parser.cpp"
    break;

  case 408:
#line 2156 "ds_parser.ypp"
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
#line 7303 "ds_parser.cpp"
    break;

  case 409:
#line 2171 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7309 "ds_parser.cpp"
    break;

  case 410:
#line 2171 "ds_parser.ypp"
    {
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-4].s);
        btype->at = tokAt((yylsp[-4]));
        btype->argNames = *(yyvsp[-2].pNameList);
        if ( !g_Program->addAlias(btype) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-4].s),tokAt((yylsp[-4])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-4].s);
        delete (yyvsp[-2].pNameList);
    }
#line 7326 "ds_parser.cpp"
    break;

  case 411:
#line 2187 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7332 "ds_parser.cpp"
    break;

  case 412:
#line 2188 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7338 "ds_parser.cpp"
    break;

  case 413:
#line 2189 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7344 "ds_parser.cpp"
    break;

  case 414:
#line 2190 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7350 "ds_parser.cpp"
    break;

  case 415:
#line 2194 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7362 "ds_parser.cpp"
    break;

  case 416:
#line 2201 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7373 "ds_parser.cpp"
    break;

  case 417:
#line 2210 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 7383 "ds_parser.cpp"
    break;

  case 418:
#line 2215 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7392 "ds_parser.cpp"
    break;

  case 419:
#line 2222 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7402 "ds_parser.cpp"
    break;

  case 420:
#line 2227 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7413 "ds_parser.cpp"
    break;

  case 421:
#line 2233 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7424 "ds_parser.cpp"
    break;

  case 422:
#line 2239 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7436 "ds_parser.cpp"
    break;

  case 423:
#line 2246 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7449 "ds_parser.cpp"
    break;

  case 424:
#line 2257 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7457 "ds_parser.cpp"
    break;

  case 425:
#line 2260 "ds_parser.ypp"
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
#line 7473 "ds_parser.cpp"
    break;

  case 426:
#line 2274 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7484 "ds_parser.cpp"
    break;

  case 427:
#line 2283 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7490 "ds_parser.cpp"
    break;

  case 428:
#line 2284 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7496 "ds_parser.cpp"
    break;

  case 429:
#line 2288 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7506 "ds_parser.cpp"
    break;

  case 430:
#line 2293 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7515 "ds_parser.cpp"
    break;

  case 431:
#line 2300 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7525 "ds_parser.cpp"
    break;

  case 432:
#line 2305 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7538 "ds_parser.cpp"
    break;

  case 433:
#line 2316 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7548 "ds_parser.cpp"
    break;

  case 434:
#line 2321 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7557 "ds_parser.cpp"
    break;

  case 435:
#line 2328 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7571 "ds_parser.cpp"
    break;

  case 436:
#line 2340 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 7577 "ds_parser.cpp"
    break;

  case 437:
#line 2341 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7583 "ds_parser.cpp"
    break;

  case 438:
#line 2345 "ds_parser.ypp"
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
#line 7602 "ds_parser.cpp"
    break;

  case 439:
#line 2359 "ds_parser.ypp"
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
#line 7621 "ds_parser.cpp"
    break;


#line 7625 "ds_parser.cpp"

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
#line 2375 "ds_parser.ypp"


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


