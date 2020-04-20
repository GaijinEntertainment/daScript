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
#line 53 "ds_parser.ypp"

	#include "daScript/misc/platform.h"
	#include "daScript/simulate/debug_info.h"
	#include "daScript/ast/compilation_errors.h"

	#ifdef _MSC_VER
	#pragma warning(disable:4262)
	#pragma warning(disable:4127)
	#pragma warning(disable:4702)
	#endif
    
    using namespace das;
    
    void das_yyerror (const string & error, das::CompilationError cerr = das::CompilationError::syntax_error);
    void das_yyerror (const string & error, const das::LineInfo & at, das::CompilationError cerr = das::CompilationError::unspecified);
	void das_checkName(const string & name, const LineInfo &at);
    int yylex();
    void yybegin(const char * str);

#line 101 "ds_parser.cpp"

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
    
    Annotation * findAnnotation ( const string & name, const LineInfo & at );

    extern bool das_need_oxford_comma;

#line 194 "ds_parser.cpp"

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
    DAS_TBOOL = 312,
    DAS_TVOID = 313,
    DAS_TSTRING = 314,
    DAS_TAUTO = 315,
    DAS_TINT = 316,
    DAS_TINT2 = 317,
    DAS_TINT3 = 318,
    DAS_TINT4 = 319,
    DAS_TUINT = 320,
    DAS_TUINT2 = 321,
    DAS_TUINT3 = 322,
    DAS_TUINT4 = 323,
    DAS_TFLOAT = 324,
    DAS_TFLOAT2 = 325,
    DAS_TFLOAT3 = 326,
    DAS_TFLOAT4 = 327,
    DAS_TRANGE = 328,
    DAS_TURANGE = 329,
    DAS_TBLOCK = 330,
    DAS_TINT64 = 331,
    DAS_TUINT64 = 332,
    DAS_TDOUBLE = 333,
    DAS_TFUNCTION = 334,
    DAS_TLAMBDA = 335,
    DAS_TINT8 = 336,
    DAS_TUINT8 = 337,
    DAS_TINT16 = 338,
    DAS_TUINT16 = 339,
    DAS_TTUPLE = 340,
    DAS_TVARIANT = 341,
    DAS_GENERATOR = 342,
    DAS_YIELD = 343,
    ADDEQU = 344,
    SUBEQU = 345,
    DIVEQU = 346,
    MULEQU = 347,
    MODEQU = 348,
    ANDEQU = 349,
    OREQU = 350,
    XOREQU = 351,
    SHL = 352,
    SHR = 353,
    ADDADD = 354,
    SUBSUB = 355,
    LEEQU = 356,
    SHLEQU = 357,
    SHREQU = 358,
    GREQU = 359,
    EQUEQU = 360,
    NOTEQU = 361,
    RARROW = 362,
    LARROW = 363,
    QQ = 364,
    QDOT = 365,
    QBRA = 366,
    LPIPE = 367,
    LBPIPE = 368,
    LAPIPE = 369,
    RPIPE = 370,
    CLONEEQU = 371,
    ROTL = 372,
    ROTR = 373,
    ROTLEQU = 374,
    ROTREQU = 375,
    MAPTO = 376,
    BRABRAB = 377,
    BRACBRB = 378,
    CBRCBRB = 379,
    INTEGER = 380,
    LONG_INTEGER = 381,
    UNSIGNED_INTEGER = 382,
    UNSIGNED_LONG_INTEGER = 383,
    FLOAT = 384,
    DOUBLE = 385,
    NAME = 386,
    BEGIN_STRING = 387,
    STRING_CHARACTER = 388,
    END_STRING = 389,
    BEGIN_STRING_EXPR = 390,
    END_STRING_EXPR = 391,
    UNARY_MINUS = 392,
    UNARY_PLUS = 393,
    PRE_INC = 394,
    PRE_DEC = 395,
    POST_INC = 396,
    POST_DEC = 397,
    DEREF = 398,
    COLCOL = 399
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED
union DAS_YYSTYPE
{
#line 79 "ds_parser.ypp"

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

#line 375 "ds_parser.cpp"

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
#define YYLAST   6750

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  172
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  111
/* YYNRULES -- Number of rules.  */
#define YYNRULES  428
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  783

#define YYUNDEFTOK  2
#define YYMAXUTOK   399

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
       2,     2,     2,   154,     2,   171,   169,   150,   143,     2,
     162,   163,   148,   147,   137,   146,   159,   149,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   140,   166,
     144,   138,   145,   139,   170,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   160,     2,   161,   142,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   167,   141,   168,   153,     2,     2,     2,
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
     135,   136,   151,   152,   155,   156,   157,   158,   164,   165
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   385,   385,   386,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,   400,   412,   413,   417,   421,   425,
     434,   442,   458,   479,   483,   486,   492,   501,   522,   545,
     546,   550,   554,   555,   559,   562,   568,   574,   577,   583,
     584,   588,   589,   590,   599,   600,   604,   613,   625,   635,
     644,   645,   646,   647,   648,   649,   653,   658,   666,   667,
     668,   669,   670,   671,   672,   673,   679,   692,   708,   709,
     713,   720,   732,   736,   743,   744,   748,   749,   750,   754,
     758,   765,   769,   770,   771,   772,   773,   774,   775,   776,
     777,   778,   779,   780,   781,   782,   783,   784,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,   795,   796,
     797,   798,   799,   800,   801,   802,   803,   804,   808,   881,
     884,   894,   895,   896,   897,   898,   899,   900,   901,   902,
     903,   904,   905,   906,   907,   908,   909,   910,   911,   915,
     919,   925,   931,   942,   948,   949,   960,   964,   970,   973,
     976,   980,   986,   990,   994,   997,  1000,  1005,  1008,  1016,
    1019,  1024,  1027,  1035,  1041,  1042,  1046,  1047,  1051,  1085,
    1088,  1093,  1101,  1105,  1110,  1116,  1120,  1125,  1134,  1137,
    1143,  1144,  1151,  1162,  1163,  1167,  1172,  1227,  1228,  1229,
    1230,  1231,  1232,  1236,  1237,  1238,  1239,  1240,  1241,  1242,
    1243,  1244,  1245,  1246,  1247,  1248,  1249,  1250,  1251,  1255,
    1256,  1260,  1271,  1276,  1286,  1290,  1297,  1311,  1312,  1313,
    1314,  1315,  1316,  1317,  1318,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,  1326,  1327,  1328,  1329,  1330,  1331,  1332,  1333,
    1334,  1335,  1336,  1337,  1338,  1339,  1340,  1341,  1342,  1343,
    1344,  1345,  1346,  1347,  1348,  1349,  1350,  1354,  1358,  1361,
    1364,  1365,  1366,  1367,  1372,  1377,  1378,  1381,  1384,  1388,
    1392,  1396,  1397,  1398,  1399,  1400,  1401,  1402,  1412,  1422,
    1456,  1457,  1461,  1462,  1466,  1474,  1477,  1484,  1489,  1497,
    1498,  1502,  1505,  1514,  1515,  1519,  1528,  1529,  1534,  1535,
    1539,  1544,  1547,  1551,  1557,  1566,  1567,  1568,  1572,  1573,
    1577,  1580,  1585,  1590,  1598,  1609,  1612,  1619,  1620,  1624,
    1654,  1657,  1666,  1679,  1691,  1692,  1696,  1700,  1711,  1726,
    1727,  1731,  1770,  1836,  1843,  1852,  1853,  1854,  1855,  1856,
    1857,  1858,  1859,  1860,  1861,  1862,  1863,  1864,  1865,  1866,
    1867,  1868,  1869,  1870,  1871,  1872,  1873,  1874,  1875,  1879,
    1880,  1881,  1882,  1883,  1884,  1888,  1899,  1903,  1913,  1914,
    1915,  1916,  1930,  1936,  1940,  1945,  1950,  1955,  1962,  1967,
    1971,  1975,  1980,  1985,  1992,  1997,  2003,  2008,  2012,  2017,
    2026,  2030,  2035,  2044,  2048,  2053,  2062,  2068,  2077,  2083,
    2098,  2099,  2100,  2101,  2105,  2112,  2121,  2126,  2133,  2138,
    2144,  2150,  2157,  2168,  2171,  2185,  2194,  2195,  2199,  2204,
    2211,  2216,  2227,  2232,  2239,  2251,  2252,  2256,  2270
};
#endif

#if DAS_YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"lexer error\"", "\"struct\"",
  "\"let\"", "\"def\"", "\"while\"", "\"if\"", "\"static_if\"", "\"else\"",
  "\"for\"", "\"catch\"", "\"true\"", "\"false\"", "\"new\"",
  "\"typeinfo\"", "\"type\"", "\"in\"", "\"is\"", "\"as\"", "\"elif\"",
  "\"static_elif\"", "\"array\"", "\"return\"", "\"null\"", "\"break\"",
  "\"try\"", "\"options\"", "\"table\"", "\"expect\"", "\"const\"",
  "\"require\"", "\"operator\"", "\"enum\"", "\"finally\"", "\"delete\"",
  "\"deref\"", "\"scope\"", "\"typedef\"", "\"with\"", "\"cast\"",
  "\"override\"", "\"upcast\"", "\"iterator\"", "\"var\"", "\"addr\"",
  "\"continue\"", "\"where\"", "\"pass\"", "\"reinterpret\"", "\"module\"",
  "\"public\"", "\"label\"", "\"goto\"", "\"implicit\"", "\"shared\"",
  "\"bool\"", "\"void\"", "\"string\"", "\"auto\"", "\"int\"", "\"int2\"",
  "\"int3\"", "\"int4\"", "\"uint\"", "\"uint2\"", "\"uint3\"",
  "\"uint4\"", "\"float\"", "\"float2\"", "\"float3\"", "\"float4\"",
  "\"range\"", "\"urange\"", "\"block\"", "\"int64\"", "\"uint64\"",
  "\"double\"", "\"function\"", "\"lambda\"", "\"int8\"", "\"uint8\"",
  "\"int16\"", "\"uint16\"", "\"tuple\"", "\"variant\"", "\"generator\"",
  "\"yield\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"",
  "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"",
  "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"",
  "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"", "\"@ <|\"", "\"|>\"",
  "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"",
  "\"[[\"", "\"[{\"", "\"{{\"", "\"integer constant\"",
  "\"long integer constant\"", "\"unsigned integer constant\"",
  "\"unsigned long integer constant\"", "\"floating point constant\"",
  "\"double constant\"", "\"name\"", "\"start of the string\"",
  "STRING_CHARACTER", "\"end of the string\"", "\"{\"", "\"}\"", "','",
  "'='", "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'-'", "'+'",
  "'*'", "'/'", "'%'", "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'",
  "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC", "'.'", "'['", "']'", "'('",
  "')'", "DEREF", "COLCOL", "';'", "'{'", "'}'", "'$'", "'@'", "'#'",
  "$accept", "program", "module_declaration", "character_sequence",
  "string_constant", "string_builder_body", "string_builder",
  "options_declaration", "require_declaration", "require_module_name",
  "require_module", "is_public_module", "expect_declaration",
  "expect_list", "expect_error", "expression_label", "expression_goto",
  "elif_or_static_elif", "expression_else", "if_or_static_if",
  "expression_if_then_else", "expression_for_loop",
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
  "optional_shared", "global_let", "enum_list", "single_alias",
  "alias_list", "alias_declaration", "enum_declaration",
  "optional_structure_parent", "structure_name", "structure_declaration",
  "variable_name_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "type_declaration", "variant_keyword",
  "variant_alias_declaration", "make_decl", "make_struct_fields",
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
     385,   386,   387,   388,   389,   390,   391,    44,    61,    63,
      58,   124,    94,    38,    60,    62,    45,    43,    42,    47,
      37,   392,   393,   126,    33,   394,   395,   396,   397,    46,
      91,    93,    40,    41,   398,   399,    59,   123,   125,    36,
      64,    35
};
# endif

#define YYPACT_NINF -495

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-495)))

#define YYTABLE_NINF -194

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -495,    18,  -495,  -495,   -26,    38,    40,    94,  -101,  -495,
      97,  -495,   -15,  -495,  -495,  -495,  -495,  -495,    69,  -495,
      63,  -495,  -495,  -495,  -495,   107,  -495,    80,  -495,   117,
     120,   129,  -495,  -495,    13,  -495,    -1,   139,  -495,  -495,
     108,   151,   123,  -495,    76,  -495,   166,    -6,  -495,   149,
     152,    12,   -26,   180,    38,   183,  -495,   187,   189,  -495,
     227,  -495,   185,  -495,   -94,   190,  -495,   -26,   -15,  -495,
     182,   160,  6596,   296,   297,  -495,   171,  -495,   203,  -495,
    -495,  -495,  -495,  -495,   202,    70,  -495,  -495,  -495,  -495,
     284,  -495,  -495,  -495,  -495,  -495,  -495,  -495,  -495,   170,
     -92,  5679,  -495,  -495,  -495,   -65,  -495,   -84,  -495,  -495,
    -495,  -495,  -495,  -495,  -495,  -495,  -495,  -495,  -495,  -495,
    -495,  -495,  -495,  -495,  -495,  -495,  -495,  -495,  -495,  -495,
    -495,  -495,  -495,  -495,  -495,  -495,  -495,  -495,  -495,  -495,
    -495,  -495,  -495,  -495,  -495,  -495,  -105,   198,   -91,   199,
    -495,   174,  -495,    42,  -495,  -495,  -495,  -495,  -495,  -495,
    -495,   -11,  -495,  -495,   -42,  -495,   197,   204,   207,  -495,
    -495,  -495,   191,  -495,  -495,  -495,  -495,  -495,  -495,  -495,
    -495,  -495,  -495,  -495,  -495,  -495,  -495,   208,  -495,  -495,
    -495,   212,   213,  -495,  -495,  -495,  -495,   214,   218,  -495,
    -495,  -495,  -495,   658,  -495,  -495,  -107,   -26,  -495,   -14,
    -495,   -57,  5679,   175,  -495,  -495,  -495,    30,  5679,   -88,
    -495,  -495,    70,  -495,   -86,  4142,  -495,  5679,  5679,  5679,
     229,  5604,  5604,  5604,  5754,   203,  -495,   -22,  -495,  -495,
    -495,  -495,  2081,  -495,  -495,  -495,   321,   200,   102,   233,
    -495,   -30,  -495,   205,  1330,  -495,  -495,   237,  5679,  -495,
     -46,  1330,  -495,  -495,  -495,  -495,  -495,  -495,  5529,   209,
    -495,   210,   226,   230,   211,   231,   240,  4142,  4142,   776,
    4264,  4142,  -495,  -495,  -495,  -495,  -495,  -495,  -495,  4142,
    4142,  4142,  4142,  4142,  4142,  -495,   220,  -495,  -495,   -63,
    -495,  -495,  -495,   235,  -495,  -495,  -495,  -495,  -495,  4885,
     234,  -495,  -495,  -495,  -495,  -495,   216,  1198,  1476,   243,
     198,  1477,   198,  1748,   198,  1749,    25,  -495,    32,  1330,
      89,   255,  -495,   242,  -495,  -495,  5748,  -495,   233,  -495,
    -495,  -495,  -495,  -495,  5679,  2224,  -495,   923,  -495,   502,
    -495,  -495,  -495,  2224,   276,  -495,   277,  4142,  5679,  5679,
    4142,  5679,  5679,   184,   184,   233,  1251,   233,  1394,  6125,
    -495,   113,   143,   184,   184,   298,   184,   184,  5019,   -77,
    -495,  1666,   171,     4,   279,  4142,  4142,  -495,  -495,  4142,
    4142,  4142,  4142,   281,  4142,   286,  4142,  4142,  4142,  4142,
    4142,  2360,  4142,  4142,  4142,  4142,  4142,  4142,  4142,  4142,
    4142,  4142,   289,  4142,  -495,  2496,  -495,  5679,  -495,  -495,
     262,  -495,   268,  -495,   280,  -495,  5679,  -495,  5754,  -495,
     203,  -495,  -495,  -495,  -495,   595,   -47,  -495,   311,  5966,
    -495,  4142,  -495,  -495,   233,  1530,  -495,   175,  4142,  4142,
    -495,   267,   309,  2632,  1802,  -495,   402,   274,   278,  4142,
    -495,  -495,  -495,  -495,  -495,  -495,   283,   285,   287,  -495,
    -495,  -495,   425,  -495,   -81,  -495,  6068,  -495,  2224,  -495,
    4351,  2768,  1945,  5087,  1942,  2027,  5154,  2028,  2221,    10,
     -78,   291,  2904,  6125,   308,  -102,   317,  -495,  -495,    84,
      16,  3040,   132,   138,  4142,  4142,   290,  -495,  4142,   315,
    -495,  5604,  -495,   324,  -495,    48,  6296,   198,  5679,  -495,
    -495,   615,   615,   403,   403,   282,   282,   299,   382,  -495,
    5823,   -69,   -69,   615,   615,   331,  6182,  6353,  6410,  6467,
     403,   403,    82,    82,   382,   382,   382,  -495,  5891,  -495,
      51,  2306,  -495,  -495,  -495,  1330,  -495,  4142,  -495,  -495,
     -47,  4142,  4142,  4142,  4142,  4142,  4142,  4142,  4142,  4142,
    4142,  4142,  4142,  4142,  4142,  4142,  4257,    17,  2224,  -495,
    -495,  4445,   451,  6296,  4257,  -495,   325,   339,  6296,  2224,
    -495,  4539,   300,  -495,  -495,  4257,  -495,  -495,  -495,   436,
     233,  -495,  3176,  3312,  -495,  4633,  -495,  -495,    72,  5679,
     344,  5221,  -495,  4142,  4142,  -495,  4142,   314,  4142,  4142,
    -495,   324,   346,   318,   324,  4142,   319,  4142,  4142,   324,
     323,   326,  6296,  -495,  -495,  6239,   198,  2442,   -59,   114,
    4142,  -495,   -70,  1330,  3448,  -495,  -495,  4142,  -495,  -495,
    -495,  6296,  -495,  6296,  6296,  6296,  6296,  6296,  6296,  6296,
    6296,  6296,  6296,  6296,  6296,  6296,  6296,  6296,  -495,  4142,
    -495,  4727,  -495,   175,  -495,  -495,  -495,  -495,  4821,  -495,
    -495,    93,  -495,  -495,  4142,  4142,  -495,  -495,   452,    98,
    -495,   403,   403,   403,  3584,   -66,  6296,    87,   -59,  -495,
     308,  6296,  -495,  -495,   -24,   147,  -495,  -495,  -495,   333,
     -84,   322,  6296,  3720,  -495,  -495,  -495,    73,  6296,    46,
    -495,  -495,  -495,  1093,   175,  -495,  -495,  4142,  -495,  6524,
    6524,  -495,  3863,   355,  -495,  5288,  4142,   329,  4142,  4142,
     334,   -84,  -495,  -495,  4142,  6296,  -495,   175,  -495,  -495,
    4257,  5679,  5355,   351,  -495,  4955,  -495,  6296,  4955,  -495,
    -495,  6296,  -495,    93,  2443,  -495,  4006,   450,   343,   337,
    -495,  -495,  5679,  5422,  4142,   347,   350,  2579,  -495,  6296,
    -495,  -495,  -495
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    74,     1,   166,     0,     0,     0,     0,     0,   167,
       0,   398,     0,    13,     3,    10,     9,     8,     0,     7,
     317,     6,    11,     5,     4,     0,    12,    64,    66,    22,
      34,    31,    32,    24,    29,    23,     0,     0,    14,    69,
     144,     0,    70,    72,     0,    68,     0,     0,   318,     0,
       0,     0,     0,     0,     0,     0,    30,     0,     0,    27,
       0,   320,     0,   324,     0,     0,   146,     0,     0,    75,
     329,     0,     0,     0,     0,    81,    76,   315,     0,    62,
      63,    60,    61,    59,     0,     0,    58,    67,    35,    33,
      29,    26,    25,   359,   362,   360,   363,   361,   364,     0,
       0,     0,   326,   325,   145,     0,    73,     0,   331,   285,
      84,    85,    87,    86,    88,    89,    90,    91,   110,   111,
     108,   109,   101,   112,   113,   102,    99,   100,   114,   115,
     116,   117,   104,   105,   103,    97,    98,    93,    92,    94,
      95,    96,    83,    82,   106,   107,   280,    79,     0,     0,
     296,     0,    15,     0,    54,    55,    52,    53,    51,    50,
      56,     0,    28,   320,     0,   327,     0,     0,     0,   335,
     355,   336,   366,   337,   341,   342,   343,   344,   348,   349,
     350,   351,   352,   353,   354,   356,   357,   387,   340,   347,
     358,   390,   393,   338,   345,   339,   346,     0,     0,   365,
     368,   370,   369,     0,    71,   330,   280,     0,    77,     0,
     289,     0,     0,     0,   333,   319,   316,   308,     0,     0,
      16,    17,     0,    65,     0,     0,   321,     0,     0,     0,
       0,    76,    76,    76,     0,     0,   374,     0,   380,   383,
     382,   376,     0,   323,   379,   332,   282,     0,     0,     0,
     287,   300,    78,   280,    80,   139,   118,     0,     0,   309,
       0,   295,   399,   297,    57,   328,   222,   223,     0,     0,
     217,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   187,   189,   188,   190,   191,   192,    18,     0,
       0,     0,     0,     0,     0,   183,   184,   220,   185,   218,
     273,   272,   271,    74,   276,   219,   275,   274,   255,     0,
       0,   221,   400,   401,   402,   403,     0,     0,     0,     0,
      79,     0,    79,     0,    79,     0,   144,   293,     0,   291,
       0,   375,   378,     0,   381,   372,     0,   283,     0,   286,
     281,   288,   299,   298,     0,     0,   290,     0,   334,     0,
     306,   307,   305,     0,   148,   151,     0,     0,     0,     0,
       0,     0,     0,   246,   247,     0,     0,     0,     0,     0,
     422,     0,     0,   227,   226,   260,   225,   224,     0,     0,
     279,     0,    76,     0,     0,     0,     0,   248,   249,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   322,     0,   384,     0,   386,   367,
       0,   388,     0,   391,     0,   394,     0,   396,     0,   397,
       0,   377,   373,   371,   284,   301,     0,   304,     0,   303,
     141,     0,    44,    45,     0,     0,   152,     0,     0,     0,
     153,     0,     0,     0,     0,   121,   119,     0,     0,     0,
     134,   129,   127,   128,   140,   122,     0,     0,     0,   132,
     133,   135,   164,   126,     0,   123,   193,   310,     0,   314,
     193,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     144,     0,     0,   413,   406,     0,   416,   417,   418,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,    19,
     250,    76,   214,     0,   256,     0,   178,    79,     0,   268,
     269,   228,   229,   241,   242,   239,   240,     0,   265,   254,
       0,   277,   278,   230,   231,     0,     0,   244,   245,   243,
     237,   238,   233,   232,   234,   235,   236,   253,     0,   258,
       0,     0,   389,   392,   395,   292,   294,     0,   184,   143,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   154,
     157,   193,     0,   147,     0,   138,     0,     0,    38,     0,
     161,   193,     0,   136,   137,     0,   125,   130,   131,     0,
       0,   124,     0,     0,   312,   193,   313,   149,     0,     0,
       0,     0,   261,     0,     0,   262,     0,     0,     0,     0,
     409,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   415,   423,   424,     0,    79,     0,     0,     0,
       0,   257,     0,   267,     0,   252,   270,     0,   251,   259,
     385,   302,   142,   200,   201,   203,   202,   204,   197,   198,
     199,   205,   206,   195,   196,   207,   208,   194,    48,     0,
     158,   193,   155,     0,    49,    36,    37,   162,   193,   159,
     139,    41,   165,   168,     0,     0,   311,   150,     0,     0,
     172,   169,   170,   171,     0,     0,   404,     0,     0,   408,
     407,   414,   420,   419,     0,     0,   411,   421,    20,     0,
       0,     0,   179,     0,   180,   186,   212,     0,   266,     0,
     156,   163,   160,     0,     0,    39,    40,     0,    46,   210,
     209,   175,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,   215,   211,     0,   181,   213,     0,   120,    42,
       0,     0,     0,     0,   264,   425,   410,   405,   425,   412,
     216,   182,    47,    41,     0,   173,     0,     0,     0,     0,
      43,   176,     0,     0,     0,     0,     0,     0,   174,   426,
     427,   428,   177
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -495,  -495,  -495,   144,   468,  -495,  -495,  -495,  -495,  -495,
    -495,   432,  -495,  -495,   469,  -495,  -495,  -495,  -238,  -495,
    -495,  -495,  -495,  -495,   305,  -495,   476,   -56,  -495,   461,
    -495,   228,  -218,  -312,  -495,  -495,  -207,  -495,  -150,  -309,
     -12,  -495,  -495,  -495,  -495,  -495,  -495,  -495,  -495,   533,
    -495,  -495,  -495,  -395,  -495,  -495,  -412,  -495,  -345,  -495,
    -495,  -495,  -495,   179,   338,  -495,  -495,  -495,   293,  -495,
     126,  -495,  -203,   313,  -248,  -227,   186,  -495,   -45,  -495,
    -495,  -495,   393,   494,  -495,  -495,  -495,  -495,  -495,  -495,
    -141,  -100,  -495,  -495,  -495,   -72,  -495,  -495,   292,  -494,
    -364,  -495,  -495,  -276,   -68,   206,  -495,  -495,  -495,  -192,
    -495
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    14,   153,   159,   372,   297,    15,    16,    34,
      35,    59,    17,    31,    32,   457,   458,   727,   728,   459,
     460,   461,   462,   463,   160,   161,    28,    29,    42,    43,
      44,    18,   147,   213,    76,    19,   298,   464,   347,   465,
     299,   466,   300,   467,   468,   469,   470,   471,   600,   472,
     473,   301,   302,   515,   715,   303,   304,   305,   438,   475,
     306,   307,   308,   516,   209,   338,   247,   206,   210,   211,
     327,   328,   150,   151,   619,   250,   353,   260,   216,   148,
      49,    21,   100,    63,    64,    22,    23,   108,    71,    24,
     251,   310,    99,   201,   202,   329,    25,    26,   311,   494,
     495,   312,   496,   497,   498,   499,   313,   371,   314,   768,
     315
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      45,   200,   474,   345,   502,   370,   256,   217,   420,   331,
     422,   105,   424,   320,   322,   324,   263,    39,     2,   639,
     550,   518,   341,     3,   559,    79,    80,    72,   618,   203,
     342,   249,   560,    55,   628,   669,   437,    62,   393,   164,
     214,   395,   396,   149,   479,   164,     4,    40,     5,   342,
       6,   713,     7,   207,    40,   207,    45,     8,   208,   623,
     343,   245,   350,     9,   624,    56,    37,   511,   380,    10,
     351,   640,    52,    46,   102,    47,   165,   215,   342,   343,
     262,    41,   265,   154,   155,   601,   608,    65,    41,   199,
     412,   413,   352,    73,    74,   205,   225,   255,   204,   381,
     736,   383,   384,   724,    11,    27,   252,   257,   343,   253,
     344,   434,   200,   640,   725,   726,    40,   214,   200,    48,
     255,   332,   295,   558,   226,    75,   222,   200,   200,   200,
     700,   200,   200,   200,   200,   519,   580,    81,   333,    60,
     254,    82,   739,    83,    84,   590,   261,   257,   652,   334,
      41,   248,   223,   257,   257,   316,   317,   318,   200,   321,
     323,   325,    57,    30,   517,   426,    61,   257,   200,   604,
     258,    33,    58,   259,    85,   220,   221,   427,    12,   200,
     200,   387,   388,   640,    13,   640,   349,   557,   640,   393,
      65,   394,   395,   396,   397,   156,   354,   398,   428,   157,
     199,   158,    84,   383,   384,   642,   199,   366,   368,   640,
     640,   641,   747,    68,   649,   199,   199,   199,    51,   199,
     199,   199,   199,   695,   489,    36,   500,   263,    38,   633,
     409,   410,   411,   704,   429,   687,   746,    69,    50,    52,
     582,   412,   413,   732,   200,   626,   199,   236,   737,   717,
     627,   622,   237,   624,    52,   430,   199,   697,   200,   200,
      53,   200,   200,   340,   733,   705,    54,   199,   199,   670,
      62,   238,   435,    65,   719,   711,   152,   507,   508,   505,
     677,   506,    66,   387,   388,    67,   484,   485,    93,   487,
     488,   393,    94,   636,   395,   396,   397,    70,   624,   398,
     630,   383,   384,   577,   627,    88,   631,   236,    95,    96,
      97,    98,   237,   624,    90,   740,    77,   200,    91,    78,
      92,   104,   107,   101,   709,   239,   200,   109,   200,   144,
     145,   238,   199,   146,   149,   152,    56,   163,   212,   218,
     219,   227,   255,   412,   413,   551,   199,   199,   228,   199,
     199,   229,   231,   230,   555,   240,   232,   233,   234,   241,
     319,   416,   235,   337,   214,   207,   339,   512,   348,   668,
     358,   356,   357,   360,   359,   361,   242,   674,   474,   385,
     386,   387,   388,   389,   362,   239,   390,   244,   681,   393,
     379,   394,   395,   396,   397,    12,   415,   398,   431,   399,
     400,   383,   384,   432,   309,   199,   419,   552,   482,   396,
     520,   200,   527,   553,   199,   240,   199,   529,   200,   241,
     547,   336,   383,   384,   560,   554,   405,   406,   407,   408,
     409,   410,   411,   585,   586,   714,   242,   592,   481,   637,
     593,   412,   413,   599,   594,   622,   643,   244,   220,   596,
     738,   597,   620,   598,   625,   638,   363,   364,   634,   217,
     369,   644,   646,   673,   676,   675,   721,   680,   373,   374,
     375,   376,   377,   378,   682,   689,   694,   698,   741,   699,
     702,   387,   388,   236,   706,   743,   753,   707,   237,   393,
     756,   394,   395,   396,   397,   759,   766,   398,   774,   199,
     385,   386,   387,   388,   775,   776,   199,   238,   780,   200,
     393,   781,   394,   395,   396,   397,   509,   749,   398,    86,
     399,   400,   162,    89,   439,   770,   476,   264,    87,   106,
     723,   382,   480,   236,    20,   478,   483,   688,   237,   486,
     762,   412,   413,   763,   246,   493,   346,   493,   330,   407,
     408,   409,   410,   411,   556,   683,   224,   238,   103,   703,
     355,   239,   412,   413,   521,   522,   769,     0,   523,   524,
     525,   526,     0,   528,   503,   530,   531,   532,   533,   534,
     536,   537,   538,   539,   540,   541,   542,   543,   544,   545,
     546,   240,   548,     0,     0,   241,     0,   199,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     350,   239,   242,     0,     0,   731,     0,     0,   351,     0,
     576,     0,     0,   244,   581,     0,   236,   583,   584,     0,
       0,   237,   588,   591,   383,   384,     0,     0,   595,     0,
     352,   240,     0,     0,     0,   241,     0,     0,     0,     0,
     238,   200,     0,     0,     0,     0,     0,   605,     0,     0,
       0,   611,   242,     0,     0,     0,     0,     0,   477,     0,
       0,   378,   200,   244,     0,     0,     0,     0,     0,   764,
     378,     0,     0,   632,   369,     0,     0,   635,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,   742,     0,
     777,     0,     0,   342,   239,     0,     0,     0,     0,     0,
       0,     0,     0,   238,   387,   388,     0,     0,     0,     0,
       0,     0,   393,     0,   394,   395,   396,   397,     0,   760,
     398,     0,     0,   343,   240,     0,   651,     0,   241,   199,
     653,   654,   655,   656,   657,   658,   659,   660,   661,   662,
     663,   664,   665,   666,   667,   242,     0,   671,     0,     0,
     199,   407,   408,   409,   410,   411,   244,   239,   678,     0,
       0,     0,     0,     0,   412,   413,     0,     0,     0,     0,
       0,   663,   667,     0,     0,     0,     0,   365,     0,     0,
       0,     0,   691,   692,     0,   693,     0,   240,   696,   166,
       0,   241,     0,     0,   701,   167,   493,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,   712,
     168,     0,     0,     0,   243,     0,   718,     0,     0,   244,
       0,     0,     0,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,   735,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   745,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   476,     0,     0,     0,   750,    40,     0,     0,
       0,   752,     0,     0,     0,   755,     0,   757,   758,     0,
       0,     0,     0,   761,   440,     0,     0,     0,     3,     0,
     441,   442,   443,     0,   444,     0,   266,   267,   268,   269,
       0,    41,     0,     0,     0,   773,     0,   445,   270,   446,
     447,     0,     0,   779,     0,     0,     0,     0,     0,   448,
     271,     0,     0,   449,   272,     0,   273,     0,     9,   274,
     450,     0,   451,   275,     0,     0,   452,   453,     0,     0,
     169,   170,   171,     0,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,   188,
     189,   190,     0,     0,   193,   194,   195,   196,     0,     0,
     276,   454,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,   278,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,   279,   280,   281,   282,   283,
     284,   285,   286,   287,    40,   288,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   289,
     290,   291,     0,     0,     0,     0,   292,   293,     0,     0,
       0,     0,     0,     0,     0,   294,     0,     0,    41,   455,
     255,   456,   295,   296,   440,     0,     0,     0,     3,     0,
     441,   442,   443,     0,   444,     0,   266,   267,   268,   269,
       0,     0,     0,     0,     0,     0,     0,   445,   270,   446,
     447,     0,     0,     0,     0,     0,     0,     0,     0,   448,
     271,     0,     0,   449,   272,     0,   273,     0,     9,   274,
     450,     0,   451,   275,     0,     0,   452,   453,     0,     0,
     169,   170,   171,     0,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,   188,
     189,   190,     0,     0,   193,   194,   195,   196,     0,     0,
     276,   454,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,   278,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,   279,   280,   281,   282,   283,
     284,   285,   286,   287,    40,   288,     0,     0,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,   289,
     290,   291,     0,     0,     0,     0,   292,   293,     0,     0,
       0,     0,     0,   238,     0,   294,     0,     0,    41,   455,
     255,   748,   295,   296,   266,   267,   268,   269,     0,     0,
       0,     0,     0,     0,     0,     0,   270,     0,     0,     0,
       0,     0,   236,     0,     0,     0,     0,   237,   271,     0,
       0,     0,   272,     0,   273,     0,     0,   274,     0,     0,
       0,   275,     0,     0,     0,     0,   238,   239,   169,   170,
     171,     0,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,     0,   188,   189,   190,
       0,     0,   193,   194,   195,   196,     0,   240,   276,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
     277,   278,     0,     0,     0,     0,     0,     0,   242,     0,
     239,   236,     0,     0,   417,     0,   237,     0,     0,   244,
       0,     0,     0,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   490,   288,     0,   238,     0,     0,     0,     0,
     240,     0,     0,     0,   241,     0,     0,   289,   290,   291,
       0,     0,     0,     0,   292,   293,     0,   266,   267,   268,
     269,   242,   491,   492,     0,     0,    41,     0,   255,   270,
     295,   296,   244,     0,     0,   236,     0,     0,     0,     0,
     237,   271,     0,     0,     0,   272,     0,   273,     0,   239,
     274,     0,     0,     0,   275,     0,     0,     0,     0,   238,
       0,   169,   170,   171,     0,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   240,
     188,   189,   190,   241,     0,   193,   194,   195,   196,     0,
       0,   276,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,   277,   278,     0,     0,     0,     0,     0,
       0,   244,     0,   239,     0,     0,     0,   236,   236,     0,
       0,     0,   237,   237,     0,     0,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   490,   288,     0,     0,     0,
       0,   238,   238,   240,     0,     0,     0,   241,     0,     0,
     289,   290,   291,   266,   267,   268,   269,   292,   293,     0,
       0,     0,     0,     0,   242,   270,   501,     0,     0,    41,
       0,   255,     0,   295,   296,   244,     0,   271,     0,     0,
       0,   272,     0,   273,     0,     0,   274,     0,     0,     0,
     275,     0,     0,     0,     0,   239,   239,   169,   170,   171,
       0,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,     0,   188,   189,   190,     0,
       0,   193,   194,   195,   196,   240,   240,   276,     0,   241,
     241,   418,   421,     0,     0,     0,     0,     0,     0,   277,
     278,     0,     0,     0,     0,     0,   242,   242,   578,     0,
       0,     0,     0,     0,   436,     0,     0,   244,   244,     0,
       0,     0,   279,   280,   281,   282,   283,   284,   285,   286,
     287,    40,   288,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   289,   290,   291,   266,
     267,   268,   269,   292,   293,     0,     0,     0,     0,     0,
       0,   270,   294,     0,     0,    41,   579,   255,     0,   295,
     296,     0,     0,   271,     0,     0,     0,   272,     0,   273,
       0,     0,   274,     0,     0,     0,   275,     0,     0,     0,
       0,     0,     0,   169,   170,   171,     0,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,     0,   188,   189,   190,     0,     0,   193,   194,   195,
     196,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   236,
     236,     0,     0,     0,   237,   237,     0,     0,   279,   280,
     281,   282,   283,   284,   285,   286,   287,    40,   288,     0,
       0,     0,     0,   238,   238,     0,     0,     0,     0,     0,
       0,     0,   289,   290,   291,   266,   267,   268,   269,   292,
     293,     0,     0,     0,     0,     0,   513,   270,   294,   514,
       0,    41,     0,   255,     0,   295,   296,     0,     0,   271,
       0,     0,     0,   272,     0,   273,     0,     0,   274,     0,
       0,     0,   275,     0,     0,     0,     0,   239,   239,   169,
     170,   171,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,   188,   189,
     190,     0,     0,   193,   194,   195,   196,   240,   240,   276,
       0,   241,   241,   423,   425,     0,     0,     0,     0,     0,
       0,   277,   278,     0,     0,     0,     0,     0,   242,   242,
     589,     0,     0,     0,     0,     0,   436,     0,     0,   244,
     244,     0,     0,     0,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    40,   288,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   289,   290,
     291,     0,     0,     0,     0,   292,   293,     0,   266,   267,
     268,   269,   609,     0,   294,     0,     0,    41,     0,   255,
     270,   295,   296,   236,     0,     0,     0,     0,   237,     0,
       0,     0,   271,     0,     0,     0,   272,     0,   273,     0,
       0,   274,     0,     0,     0,   275,     0,   238,     0,     0,
       0,     0,   169,   170,   171,     0,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
       0,   188,   189,   190,     0,     0,   193,   194,   195,   196,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,   278,     0,     0,     0,     0,
       0,   239,     0,     0,     0,     0,     0,     0,   236,   236,
       0,     0,     0,   237,   237,     0,     0,   279,   280,   281,
     282,   283,   284,   285,   286,   287,    40,   288,     0,     0,
       0,   240,   238,   238,     0,   241,     0,   613,     0,   610,
       0,   289,   290,   291,   266,   267,   268,   269,   292,   293,
       0,     0,   242,     0,     0,     0,   270,   294,     0,     0,
      41,     0,   255,   244,   295,   296,     0,     0,   271,     0,
       0,     0,   272,     0,   273,     0,     0,   274,     0,     0,
       0,   275,     0,     0,     0,     0,   239,   239,   169,   170,
     171,     0,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,     0,   188,   189,   190,
       0,     0,   193,   194,   195,   196,   240,   240,   276,     0,
     241,   241,   614,   616,     0,     0,     0,     0,     0,     0,
     277,   278,     0,     0,     0,     0,     0,   242,   242,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   244,   244,
       0,     0,     0,   279,   280,   281,   282,   283,   284,   285,
     286,   287,    40,   288,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   289,   290,   291,
       0,     0,     0,     0,   292,   293,     0,   266,   267,   268,
     269,     0,   335,   294,     0,     0,    41,     0,   255,   270,
     295,   296,   236,     0,     0,     0,     0,   237,     0,     0,
       0,   271,     0,     0,     0,   272,     0,   273,     0,     0,
     274,     0,     0,     0,   275,     0,   238,     0,     0,     0,
       0,   169,   170,   171,     0,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,     0,
     188,   189,   190,     0,     0,   193,   194,   195,   196,     0,
       0,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   277,   278,     0,     0,     0,     0,     0,
     239,     0,     0,     0,     0,     0,     0,   236,   436,     0,
       0,     0,   237,     0,     0,     0,   279,   280,   281,   282,
     283,   284,   285,   286,   287,    40,   288,     0,     0,     0,
     240,   238,     0,     0,   241,     0,   617,     0,     0,     0,
     289,   290,   291,   266,   267,   268,   269,   292,   293,     0,
     535,   242,     0,     0,     0,   270,   294,     0,     0,    41,
       0,   255,   244,   295,   296,     0,     0,   271,     0,     0,
       0,   272,     0,   273,     0,     0,   274,     0,     0,     0,
     275,     0,     0,     0,     0,   239,     0,   169,   170,   171,
       0,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,     0,   188,   189,   190,     0,
       0,   193,   194,   195,   196,   240,     0,   276,     0,   241,
       0,   650,     0,     0,     0,     0,     0,     0,     0,   277,
     278,     0,     0,     0,     0,     0,   242,     0,     0,     0,
       0,     0,     0,   236,   236,     0,     0,   244,   237,   237,
       0,     0,   279,   280,   281,   282,   283,   284,   285,   286,
     287,    40,   288,     0,     0,     0,     0,   238,   238,     0,
       0,     0,     0,     0,     0,     0,   289,   290,   291,   266,
     267,   268,   269,   292,   293,     0,     0,     0,     0,     0,
       0,   270,   294,     0,     0,    41,     0,   255,     0,   295,
     296,     0,     0,   271,     0,     0,     0,   272,     0,   273,
       0,     0,   274,     0,     0,     0,   275,     0,     0,     0,
       0,   239,   239,   169,   170,   171,     0,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,     0,   188,   189,   190,     0,     0,   193,   194,   195,
     196,   240,   240,   276,     0,   241,   241,   710,     0,     0,
       0,     0,     0,     0,     0,   277,   278,     0,     0,     0,
       0,     0,   242,   242,     0,     0,   771,     0,     0,     0,
     236,     0,     0,   244,   244,   237,     0,     0,   279,   280,
     281,   282,   283,   284,   285,   286,   287,    40,   288,     0,
       0,     0,     0,     0,   238,     0,     0,     0,     0,     0,
       0,     0,   289,   290,   291,   266,   267,   268,   269,   292,
     293,     0,     0,     0,     0,     0,     0,   270,   294,   549,
       0,    41,     0,   255,     0,   295,   296,     0,     0,   271,
       0,     0,     0,   272,     0,   273,     0,     0,   274,     0,
       0,     0,   275,     0,     0,   587,     0,     0,   239,   169,
     170,   171,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,   188,   189,
     190,     0,     0,   193,   194,   195,   196,     0,   240,   276,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,   277,   278,     0,     0,     0,     0,     0,     0,   242,
       0,     0,   782,     0,     0,     0,     0,     0,     0,     0,
     244,     0,     0,     0,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    40,   288,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   289,   290,
     291,   266,   267,   268,   269,   292,   293,     0,     0,     0,
       0,     0,     0,   270,   294,     0,     0,    41,     0,   255,
       0,   295,   296,     0,     0,   271,     0,     0,     0,   272,
       0,   273,     0,     0,   274,     0,     0,     0,   275,     0,
       0,     0,     0,     0,     0,   169,   170,   171,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,   188,   189,   190,     0,     0,   193,
     194,   195,   196,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,   278,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     279,   280,   281,   282,   283,   284,   285,   286,   287,    40,
     288,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   289,   290,   291,   266,   267,   268,
     269,   292,   293,     0,     0,     0,     0,     0,     0,   270,
     294,   607,     0,    41,     0,   255,     0,   295,   296,     0,
       0,   271,     0,     0,     0,   272,     0,   273,     0,     0,
     274,     0,     0,     0,   275,     0,     0,     0,     0,     0,
       0,   169,   170,   171,     0,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,     0,
     188,   189,   190,     0,     0,   193,   194,   195,   196,     0,
       0,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   277,   278,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   279,   280,   281,   282,
     283,   284,   285,   286,   287,    40,   288,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     289,   290,   291,   266,   267,   268,   269,   292,   293,     0,
       0,     0,     0,     0,     0,   270,   294,   621,     0,    41,
       0,   255,     0,   295,   296,     0,     0,   271,     0,     0,
       0,   272,     0,   273,     0,     0,   274,     0,     0,     0,
     275,     0,     0,     0,     0,     0,     0,   169,   170,   171,
       0,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,     0,   188,   189,   190,     0,
       0,   193,   194,   195,   196,     0,     0,   276,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
     278,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   279,   280,   281,   282,   283,   284,   285,   286,
     287,    40,   288,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   289,   290,   291,   266,
     267,   268,   269,   292,   293,     0,     0,     0,     0,     0,
       0,   270,   294,   629,     0,    41,     0,   255,     0,   295,
     296,     0,     0,   271,     0,     0,     0,   272,     0,   273,
       0,     0,   274,     0,     0,     0,   275,     0,     0,     0,
       0,     0,     0,   169,   170,   171,     0,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,     0,   188,   189,   190,     0,     0,   193,   194,   195,
     196,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     684,     0,     0,     0,     0,     0,     0,     0,   279,   280,
     281,   282,   283,   284,   285,   286,   287,    40,   288,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   289,   290,   291,   266,   267,   268,   269,   292,
     293,     0,     0,     0,     0,     0,     0,   270,   294,     0,
       0,    41,     0,   255,     0,   295,   296,     0,     0,   271,
       0,     0,     0,   272,     0,   273,     0,     0,   274,     0,
       0,     0,   275,     0,     0,     0,     0,     0,     0,   169,
     170,   171,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,   188,   189,
     190,     0,     0,   193,   194,   195,   196,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,   278,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   685,     0,     0,     0,
       0,     0,     0,     0,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    40,   288,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   289,   290,
     291,   266,   267,   268,   269,   292,   293,     0,     0,     0,
       0,     0,     0,   270,   294,     0,     0,    41,     0,   255,
       0,   295,   296,     0,     0,   271,     0,     0,     0,   272,
       0,   273,     0,     0,   274,     0,     0,     0,   275,     0,
       0,     0,     0,     0,     0,   169,   170,   171,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,   188,   189,   190,     0,     0,   193,
     194,   195,   196,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,   278,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     279,   280,   281,   282,   283,   284,   285,   286,   287,    40,
     288,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   289,   290,   291,   266,   267,   268,
     269,   292,   293,     0,     0,     0,     0,     0,     0,   270,
     294,   716,     0,    41,     0,   255,     0,   295,   296,     0,
       0,   271,     0,     0,     0,   272,     0,   273,     0,     0,
     274,     0,     0,     0,   275,     0,     0,     0,     0,     0,
       0,   169,   170,   171,     0,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,     0,
     188,   189,   190,     0,     0,   193,   194,   195,   196,     0,
       0,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   277,   278,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   279,   280,   281,   282,
     283,   284,   285,   286,   287,    40,   288,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     289,   290,   291,   266,   267,   268,   269,   292,   293,     0,
       0,     0,     0,     0,     0,   270,   294,   734,     0,    41,
       0,   255,     0,   295,   296,     0,     0,   271,     0,     0,
       0,   272,     0,   273,     0,     0,   274,     0,     0,     0,
     275,     0,     0,     0,     0,     0,     0,   169,   170,   171,
       0,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,     0,   188,   189,   190,     0,
       0,   193,   194,   195,   196,     0,     0,   276,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
     278,     0,     0,     0,     0,     0,     0,     0,   744,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   279,   280,   281,   282,   283,   284,   285,   286,
     287,    40,   288,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   289,   290,   291,     0,
       0,     0,     0,   292,   293,     0,   266,   267,   268,   269,
     751,     0,   294,     0,     0,    41,     0,   255,   270,   295,
     296,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     271,     0,     0,     0,   272,     0,   273,     0,     0,   274,
       0,     0,     0,   275,     0,     0,     0,     0,     0,     0,
     169,   170,   171,     0,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,   188,
     189,   190,     0,     0,   193,   194,   195,   196,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,   278,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   279,   280,   281,   282,   283,
     284,   285,   286,   287,    40,   288,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   289,
     290,   291,     0,     0,     0,     0,   292,   293,     0,   266,
     267,   268,   269,   772,     0,   294,     0,     0,    41,     0,
     255,   270,   295,   296,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   271,     0,     0,     0,   272,     0,   273,
       0,     0,   274,     0,     0,     0,   275,     0,     0,     0,
       0,     0,     0,   169,   170,   171,     0,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,     0,   188,   189,   190,     0,     0,   193,   194,   195,
     196,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   279,   280,
     281,   282,   283,   284,   285,   286,   287,    40,   288,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   289,   290,   291,   266,   267,   268,   269,   292,
     293,     0,     0,     0,     0,     0,     0,   270,   294,     0,
       0,    41,     0,   255,     0,   295,   296,     0,     0,   271,
       0,     0,     0,   272,     0,   273,     0,     0,   274,     0,
       0,     0,   275,     0,     0,     0,     0,     0,     0,   169,
     170,   171,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,   188,   189,
     190,     0,     0,   193,   194,   195,   196,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,   278,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    40,   288,   367,   383,   384,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   166,   289,   290,
     291,     0,     0,   167,     0,   292,   293,     0,     0,     0,
       0,     0,     0,     0,   294,     0,     0,    41,   168,   255,
       0,   295,   296,     0,     0,     0,     0,     0,     0,     0,
       0,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,     0,     0,   385,   386,   387,   388,   389,     0,
       0,   390,   391,   392,   393,     0,   394,   395,   396,   397,
     383,   384,   398,     0,   399,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    40,   401,     0,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,     0,     0,
       0,     0,     0,     0,     0,     0,   412,   413,     0,     0,
       0,     0,     0,     0,   255,     0,     0,     0,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,   563,   564,   565,   566,   567,   568,   385,   386,
     387,   388,   389,   569,   570,   390,   391,   392,   393,   571,
     394,   395,   396,   397,   383,   384,   398,   572,   399,   400,
     573,   574,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   575,
     401,     0,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,     0,     0,     0,     0,     0,     0,     0,     0,
     412,   413,     0,     0,     0,     0,     0,   606,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,   563,   564,   565,   566,
     567,   568,   385,   386,   387,   388,   389,   569,   570,   390,
     391,   392,   393,   571,   394,   395,   396,   397,   383,   384,
     398,   572,   399,   400,   573,   574,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   575,   401,     0,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,     0,     0,     0,     0,
       0,     0,     0,     0,   412,   413,     0,     0,     0,     0,
       0,   672,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   562,
     563,   564,   565,   566,   567,   568,   385,   386,   387,   388,
     389,   569,   570,   390,   391,   392,   393,   571,   394,   395,
     396,   397,   383,   384,   398,   572,   399,   400,   573,   574,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   575,   401,     0,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
       0,     0,     0,     0,     0,     0,     0,     0,   412,   413,
       0,     0,     0,     0,     0,   679,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   562,   563,   564,   565,   566,   567,   568,
     385,   386,   387,   388,   389,   569,   570,   390,   391,   392,
     393,   571,   394,   395,   396,   397,   383,   384,   398,   572,
     399,   400,   573,   574,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   575,   401,     0,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,     0,     0,     0,     0,     0,     0,
       0,     0,   412,   413,     0,     0,     0,     0,     0,   686,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,   563,   564,
     565,   566,   567,   568,   385,   386,   387,   388,   389,   569,
     570,   390,   391,   392,   393,   571,   394,   395,   396,   397,
     383,   384,   398,   572,   399,   400,   573,   574,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   575,   401,     0,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,     0,     0,
       0,     0,     0,     0,     0,     0,   412,   413,     0,     0,
       0,     0,     0,   720,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   383,   384,     0,     0,     0,     0,
     561,   562,   563,   564,   565,   566,   567,   568,   385,   386,
     387,   388,   389,   569,   570,   390,   391,   392,   393,   571,
     394,   395,   396,   397,     0,     0,   398,   572,   399,   400,
     573,   574,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   575,
     401,     0,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,     0,     0,   383,   384,     0,     0,     0,     0,
     412,   413,   385,   386,   387,   388,   389,   722,     0,   390,
     391,   392,   393,     0,   394,   395,   396,   397,     0,     0,
     398,     0,   399,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   401,     0,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,     0,     0,   383,   384,
       0,     0,     0,     0,   412,   413,     0,     0,     0,     0,
       0,   414,   385,   386,   387,   388,   389,     0,     0,   390,
     391,   392,   393,     0,   394,   395,   396,   397,     0,     0,
     398,     0,   399,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   401,     0,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   383,   384,     0,     0,
       0,     0,     0,     0,   412,   413,   385,   386,   387,   388,
     389,   767,     0,   390,   391,   392,   393,     0,   394,   395,
     396,   397,     0,     0,   398,     0,   399,   400,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   401,     0,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
       0,     0,     0,   383,   384,     0,     0,     0,   412,   413,
       0,     0,   510,     0,   385,   386,   387,   388,   389,     0,
       0,   390,   391,   392,   393,     0,   394,   395,   396,   397,
       0,     0,   398,     0,   399,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   401,     0,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,     0,     0,
     383,   384,     0,     0,     0,     0,   412,   413,     0,     0,
     612,   385,   386,   387,   388,   389,     0,     0,   390,   391,
     392,   393,     0,   394,   395,   396,   397,     0,     0,   398,
       0,   399,   400,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   401,     0,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,     0,     0,   383,   384,     0,
       0,     0,     0,   412,   413,     0,     0,   615,   385,   386,
     387,   388,   389,     0,     0,   390,   391,   392,   393,     0,
     394,   395,   396,   397,     0,     0,   398,     0,   399,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     401,     0,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,     0,     0,   383,   384,     0,     0,     0,     0,
     412,   413,     0,     0,   690,   385,   386,   387,   388,   389,
       0,     0,   390,   391,   392,   393,     0,   394,   395,   396,
     397,     0,     0,   398,     0,   399,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   411,     0,
       0,   383,   384,     0,     0,     0,     0,   412,   413,     0,
       0,   754,   385,   386,   387,   388,   389,     0,     0,   390,
     391,   392,   393,     0,   394,   395,   396,   397,     0,     0,
     398,     0,   399,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   401,     0,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,     0,     0,     0,     0,
       0,     0,     0,     0,   412,   413,     0,     0,   765,   385,
     386,   387,   388,   389,     0,     0,   390,   391,   392,   393,
       0,   394,   395,   396,   397,     0,     0,   398,     0,   399,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   166,     0,     0,     0,     0,     0,   167,     0,
       0,   401,     0,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   411,   168,     0,     0,     0,     0,     0,     0,
       0,   412,   413,     0,     0,   778,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   166,     0,     0,
       0,     0,     0,   167,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   168,     0,
       0,   279,   280,   281,     0,     0,     0,     0,     0,     0,
      40,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,     0,     0,    41,     0,     0,     0,     0,     0,
       0,     0,   166,     0,     0,     0,     0,     0,   167,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   168,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    40,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   146,   383,   384,    41,
       0,     0,     0,     0,     0,     0,     0,   166,     0,     0,
       0,     0,     0,   167,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   168,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      40,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   383,   384,    41,   385,   386,   387,   388,   389,
       0,     0,   390,   391,   392,   393,     0,   394,   395,   396,
     397,     0,     0,   398,     0,   399,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   326,     0,   401,     0,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   411,     0,
       0,     0,     0,     0,     0,     0,     0,   412,   413,   433,
     383,   384,     0,     0,     0,     0,     0,     0,     0,    41,
     385,   386,   387,   388,   389,     0,     0,   390,   391,   392,
     393,     0,   394,   395,   396,   397,     0,     0,   398,     0,
     399,   400,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   401,     0,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,     0,     0,     0,     0,     0,     0,
       0,     0,   412,   413,   645,   383,   384,     0,   385,   386,
     387,   388,   389,     0,     0,   390,   391,   392,   393,     0,
     394,   395,   396,   397,     0,     0,   398,     0,   399,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     401,     0,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,     0,     0,     0,     0,     0,     0,     0,     0,
     412,   413,   648,     0,     0,   561,   562,   563,   564,   565,
     566,   567,   568,   385,   386,   387,   388,   389,   569,   570,
     390,   391,   392,   393,   571,   394,   395,   396,   397,  -193,
       0,   398,   572,   399,   400,   573,   574,   383,   384,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   575,   401,     0,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,     0,     0,     0,
       0,     0,     0,     0,     0,   412,   413,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   383,   384,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   561,   562,   563,
     564,   565,   566,   567,   568,   385,   386,   387,   388,   389,
     569,   570,   390,   391,   392,   393,   602,   394,   395,   396,
     397,     0,     0,   398,   572,   399,   400,   573,   574,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   383,   384,     0,     0,     0,   603,   401,     0,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   411,     0,
       0,     0,   385,   386,   387,   388,   389,   412,   413,   390,
     391,   392,   393,     0,   394,   395,   396,   397,     0,     0,
     398,     0,   399,   400,     0,     0,   504,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   383,   384,
       0,     0,     0,     0,   401,     0,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,     0,     0,     0,   385,
     386,   387,   388,   389,   412,   413,   390,   391,   392,   393,
       0,   394,   395,   396,   397,     0,     0,   398,     0,   399,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   383,   384,     0,     0,     0,
       0,   401,   647,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   411,     0,     0,     0,   385,   386,   387,   388,
     389,   412,   413,   390,   391,   392,   393,     0,   394,   395,
     396,   397,     0,     0,   398,     0,   399,   400,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   383,   384,     0,   708,     0,     0,   401,     0,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
       0,     0,     0,   385,   386,   387,   388,   389,   412,   413,
     390,   391,   392,   393,     0,   394,   395,   396,   397,     0,
       0,   398,     0,   399,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   383,
     384,     0,     0,     0,     0,   401,     0,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,     0,     0,     0,
     385,   386,   387,   388,   389,   412,   413,   390,   391,   392,
     393,     0,   394,   395,   396,   397,     0,     0,   398,     0,
     399,   400,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   383,   384,     0,     0,
       0,     0,     0,     0,     0,   403,   404,   405,   406,   407,
     408,   409,   410,   411,     0,     0,     0,   385,   386,   387,
     388,   389,   412,   413,   390,   391,   392,   393,     0,   394,
     395,   396,   397,     0,     0,   398,     0,   399,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   383,   384,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,     0,     0,     0,   385,   386,   387,   388,   389,   412,
     413,   390,   391,   392,   393,     0,   394,   395,   396,   397,
       0,     0,   398,     0,   399,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   405,   406,   407,   408,   409,   410,   411,     0,     0,
       0,   385,   386,     0,     0,   389,   412,   413,   390,   391,
     392,   393,     0,   394,   395,   396,   397,     0,     0,   398,
       0,   399,   400,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   401,     0,   402,   403,   404,   405,   406,
       0,     0,     0,   410,   411,     0,     0,     0,     0,     0,
       0,     0,     0,   412,   413,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   128,   129,   130,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,     0,     0,   142,
     143
};

static const yytype_int16 yycheck[] =
{
      12,   101,   347,   251,   368,   281,   213,   148,   320,    31,
     322,    67,   324,   231,   232,   233,   219,    32,     0,   513,
     415,    17,   249,     5,   436,    13,    14,    33,    18,   101,
     108,    45,   113,    20,    18,    18,   345,   131,   107,   131,
     131,   110,   111,   131,   353,   131,    28,   131,    30,   108,
      32,   121,    34,   160,   131,   160,    68,    39,   163,   161,
     138,   168,   108,    45,   166,    52,   167,   144,   131,    51,
     116,   137,   137,     4,   168,     6,   168,   168,   108,   138,
     168,   165,   168,    13,    14,   166,   481,   165,   165,   101,
     159,   160,   138,    99,   100,   107,   138,   167,   163,   162,
     166,    19,    20,    10,    86,   131,   163,   137,   138,   166,
     140,   338,   212,   137,    21,    22,   131,   131,   218,    56,
     167,   143,   169,   170,   166,   131,   137,   227,   228,   229,
     624,   231,   232,   233,   234,   131,   445,   125,   160,   140,
     212,   129,   166,   131,   132,   454,   218,   137,   560,   171,
     165,   207,   163,   137,   137,   227,   228,   229,   258,   231,
     232,   233,   149,   125,   382,   140,   167,   137,   268,   478,
     140,   131,   159,   143,   162,   133,   134,   145,   160,   279,
     280,    99,   100,   137,   166,   137,   258,   435,   137,   107,
     165,   109,   110,   111,   112,   125,   268,   115,   166,   129,
     212,   131,   132,    19,    20,   517,   218,   279,   280,   137,
     137,   163,   166,   137,   163,   227,   228,   229,   138,   231,
     232,   233,   234,   618,   365,   131,   367,   430,   131,   505,
     148,   149,   150,   628,   145,   163,   163,   161,   131,   137,
     447,   159,   160,   145,   344,   161,   258,    31,   161,   644,
     166,   137,    36,   166,   137,   166,   268,   621,   358,   359,
     140,   361,   362,   161,   166,   629,   137,   279,   280,   578,
     131,    55,   344,   165,   669,   161,   133,   134,   135,   166,
     589,   168,   131,    99,   100,   162,   358,   359,    61,   361,
     362,   107,    65,   511,   110,   111,   112,   131,   166,   115,
     168,    19,    20,   444,   166,   125,   168,    31,    81,    82,
      83,    84,    36,   166,   131,   168,   167,   417,   131,   167,
     131,   131,   140,   138,   636,   109,   426,   167,   428,    33,
      33,    55,   344,   162,   131,   133,    52,   167,   140,   140,
     166,   144,   167,   159,   160,   417,   358,   359,   144,   361,
     362,   144,   144,   162,   426,   139,   144,   144,   144,   143,
     131,   145,   144,    42,   131,   160,   166,   379,   131,   576,
     144,   162,   162,   162,   144,   144,   160,   584,   723,    97,
      98,    99,   100,   101,   144,   109,   104,   171,   595,   107,
     170,   109,   110,   111,   112,   160,   162,   115,   143,   117,
     118,    19,    20,   161,   225,   417,   163,   145,   131,   111,
     131,   511,   131,   145,   426,   139,   428,   131,   518,   143,
     131,   242,    19,    20,   113,   145,   144,   145,   146,   147,
     148,   149,   150,   166,   125,   642,   160,    35,   162,   511,
     166,   159,   160,    18,   166,   137,   518,   171,   133,   166,
     698,   166,   161,   166,   137,   131,   277,   278,   168,   600,
     281,   162,   131,    12,   125,   140,   673,   167,   289,   290,
     291,   292,   293,   294,    38,   131,   162,   131,   145,   161,
     161,    99,   100,    31,   161,   163,   131,   161,    36,   107,
     161,   109,   110,   111,   112,   161,   145,   115,    48,   511,
      97,    98,    99,   100,   161,   168,   518,    55,   161,   609,
     107,   161,   109,   110,   111,   112,   372,   724,   115,    51,
     117,   118,    90,    54,   345,   763,   347,   222,    52,    68,
     680,   303,   353,    31,     1,   349,   357,   609,    36,   360,
     747,   159,   160,   750,   206,   366,   253,   368,   235,   146,
     147,   148,   149,   150,   428,   600,   163,    55,    64,   627,
     268,   109,   159,   160,   385,   386,   758,    -1,   389,   390,
     391,   392,    -1,   394,   368,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   139,   413,    -1,    -1,   143,    -1,   609,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,   160,    -1,    -1,   163,    -1,    -1,   116,    -1,
     441,    -1,    -1,   171,   445,    -1,    31,   448,   449,    -1,
      -1,    36,   453,   454,    19,    20,    -1,    -1,   459,    -1,
     138,   139,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      55,   751,    -1,    -1,    -1,    -1,    -1,   478,    -1,    -1,
      -1,   482,   160,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      -1,   492,   772,   171,    -1,    -1,    -1,    -1,    -1,   751,
     501,    -1,    -1,   504,   505,    -1,    -1,   508,    -1,    31,
      -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,   710,    -1,
     772,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    99,   100,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,   112,    -1,   741,
     115,    -1,    -1,   138,   139,    -1,   557,    -1,   143,   751,
     561,   562,   563,   564,   565,   566,   567,   568,   569,   570,
     571,   572,   573,   574,   575,   160,    -1,   578,    -1,    -1,
     772,   146,   147,   148,   149,   150,   171,   109,   589,    -1,
      -1,    -1,    -1,    -1,   159,   160,    -1,    -1,    -1,    -1,
      -1,   602,   603,    -1,    -1,    -1,    -1,    11,    -1,    -1,
      -1,    -1,   613,   614,    -1,   616,    -1,   139,   619,    23,
      -1,   143,    -1,    -1,   625,    29,   627,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,   640,
      44,    -1,    -1,    -1,   166,    -1,   647,    -1,    -1,   171,
      -1,    -1,    -1,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   684,   685,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   694,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   713,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   723,    -1,    -1,    -1,   727,   131,    -1,    -1,
      -1,   732,    -1,    -1,    -1,   736,    -1,   738,   739,    -1,
      -1,    -1,    -1,   744,     1,    -1,    -1,    -1,     5,    -1,
       7,     8,     9,    -1,    11,    -1,    13,    14,    15,    16,
      -1,   165,    -1,    -1,    -1,   766,    -1,    24,    25,    26,
      27,    -1,    -1,   774,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      47,    -1,    49,    50,    -1,    -1,    53,    54,    -1,    -1,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,    -1,    -1,    -1,    -1,   153,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,   165,   166,
     167,   168,   169,   170,     1,    -1,    -1,    -1,     5,    -1,
       7,     8,     9,    -1,    11,    -1,    13,    14,    15,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      47,    -1,    49,    50,    -1,    -1,    53,    54,    -1,    -1,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,   146,
     147,   148,    -1,    -1,    -1,    -1,   153,   154,    -1,    -1,
      -1,    -1,    -1,    55,    -1,   162,    -1,    -1,   165,   166,
     167,   168,   169,   170,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    55,   109,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    82,    83,    84,    -1,   139,    87,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
     109,    31,    -1,    -1,   166,    -1,    36,    -1,    -1,   171,
      -1,    -1,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,    -1,    55,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,   143,    -1,    -1,   146,   147,   148,
      -1,    -1,    -1,    -1,   153,   154,    -1,    13,    14,    15,
      16,   160,   161,   162,    -1,    -1,   165,    -1,   167,    25,
     169,   170,   171,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      36,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,   109,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    55,
      -1,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,   139,
      76,    77,    78,   143,    -1,    81,    82,    83,    84,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      -1,   171,    -1,   109,    -1,    -1,    -1,    31,    31,    -1,
      -1,    -1,    36,    36,    -1,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,    -1,    -1,    -1,
      -1,    55,    55,   139,    -1,    -1,    -1,   143,    -1,    -1,
     146,   147,   148,    13,    14,    15,    16,   153,   154,    -1,
      -1,    -1,    -1,    -1,   160,    25,   162,    -1,    -1,   165,
      -1,   167,    -1,   169,   170,   171,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,   109,   109,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      -1,    81,    82,    83,    84,   139,   139,    87,    -1,   143,
     143,   145,   145,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,   160,   160,   108,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,   171,   171,    -1,
      -1,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,    13,
      14,    15,    16,   153,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    25,   162,    -1,    -1,   165,   166,   167,    -1,   169,
     170,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    77,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      31,    -1,    -1,    -1,    36,    36,    -1,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,    -1,
      -1,    -1,    -1,    55,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,    13,    14,    15,    16,   153,
     154,    -1,    -1,    -1,    -1,    -1,   160,    25,   162,   163,
      -1,   165,    -1,   167,    -1,   169,   170,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,   109,   109,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    -1,    81,    82,    83,    84,   139,   139,    87,
      -1,   143,   143,   145,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,   160,   160,
     108,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,   171,
     171,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,    -1,    -1,    -1,    -1,   153,   154,    -1,    13,    14,
      15,    16,    17,    -1,   162,    -1,    -1,   165,    -1,   167,
      25,   169,   170,    31,    -1,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    55,    -1,    -1,
      -1,    -1,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    31,    31,
      -1,    -1,    -1,    36,    36,    -1,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,    -1,    -1,
      -1,   139,    55,    55,    -1,   143,    -1,   145,    -1,   144,
      -1,   146,   147,   148,    13,    14,    15,    16,   153,   154,
      -1,    -1,   160,    -1,    -1,    -1,    25,   162,    -1,    -1,
     165,    -1,   167,   171,   169,   170,    -1,    -1,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,   109,   109,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    82,    83,    84,   139,   139,    87,    -1,
     143,   143,   145,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,   160,   160,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,   171,
      -1,    -1,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
      -1,    -1,    -1,    -1,   153,   154,    -1,    13,    14,    15,
      16,    -1,   161,   162,    -1,    -1,   165,    -1,   167,    25,
     169,   170,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    55,    -1,    -1,    -1,
      -1,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    31,   114,    -1,
      -1,    -1,    36,    -1,    -1,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,    -1,    -1,    -1,
     139,    55,    -1,    -1,   143,    -1,   145,    -1,    -1,    -1,
     146,   147,   148,    13,    14,    15,    16,   153,   154,    -1,
      20,   160,    -1,    -1,    -1,    25,   162,    -1,    -1,   165,
      -1,   167,   171,   169,   170,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,   109,    -1,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      -1,    81,    82,    83,    84,   139,    -1,    87,    -1,   143,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    31,    -1,    -1,   171,    36,    36,
      -1,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,    -1,    -1,    -1,    -1,    55,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,    13,
      14,    15,    16,   153,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    25,   162,    -1,    -1,   165,    -1,   167,    -1,   169,
     170,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,   109,   109,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    77,    78,    -1,    -1,    81,    82,    83,
      84,   139,   139,    87,    -1,   143,   143,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,
      -1,    -1,   160,   160,    -1,    -1,   163,    -1,    -1,    -1,
      31,    -1,    -1,   171,   171,    36,    -1,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,    13,    14,    15,    16,   153,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    25,   162,   163,
      -1,   165,    -1,   167,    -1,   169,   170,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,   109,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    -1,    81,    82,    83,    84,    -1,   139,    87,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,    13,    14,    15,    16,   153,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    25,   162,    -1,    -1,   165,    -1,   167,
      -1,   169,   170,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   146,   147,   148,    13,    14,    15,
      16,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,    25,
     162,   163,    -1,   165,    -1,   167,    -1,   169,   170,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,    13,    14,    15,    16,   153,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    25,   162,   163,    -1,   165,
      -1,   167,    -1,   169,   170,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,    13,
      14,    15,    16,   153,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    25,   162,   163,    -1,   165,    -1,   167,    -1,   169,
     170,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    77,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,    13,    14,    15,    16,   153,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    25,   162,    -1,
      -1,   165,    -1,   167,    -1,   169,   170,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,    13,    14,    15,    16,   153,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    25,   162,    -1,    -1,   165,    -1,   167,
      -1,   169,   170,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   146,   147,   148,    13,    14,    15,
      16,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,    25,
     162,   163,    -1,   165,    -1,   167,    -1,   169,   170,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,    13,    14,    15,    16,   153,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    25,   162,   163,    -1,   165,
      -1,   167,    -1,   169,   170,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,    -1,
      -1,    -1,    -1,   153,   154,    -1,    13,    14,    15,    16,
      17,    -1,   162,    -1,    -1,   165,    -1,   167,    25,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,    -1,    -1,    -1,    -1,   153,   154,    -1,    13,
      14,    15,    16,    17,    -1,   162,    -1,    -1,   165,    -1,
     167,    25,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    77,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,    13,    14,    15,    16,   153,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    25,   162,    -1,
      -1,   165,    -1,   167,    -1,   169,   170,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,    11,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,   146,   147,
     148,    -1,    -1,    29,    -1,   153,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   162,    -1,    -1,   165,    44,   167,
      -1,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    -1,    -1,    97,    98,    99,   100,   101,    -1,
      -1,   104,   105,   106,   107,    -1,   109,   110,   111,   112,
      19,    20,   115,    -1,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   139,    -1,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,   160,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    19,    20,   115,   116,   117,   118,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,    -1,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,   160,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,    19,    20,
     115,   116,   117,   118,   119,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,   160,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,    19,    20,   115,   116,   117,   118,   119,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    19,    20,   115,   116,
     117,   118,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,    -1,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,   160,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      19,    20,   115,   116,   117,   118,   119,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,   160,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    -1,    -1,   115,   116,   117,   118,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,    -1,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,
     159,   160,    97,    98,    99,   100,   101,   166,    -1,   104,
     105,   106,   107,    -1,   109,   110,   111,   112,    -1,    -1,
     115,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,    -1,    -1,    19,    20,
      -1,    -1,    -1,    -1,   159,   160,    -1,    -1,    -1,    -1,
      -1,   166,    97,    98,    99,   100,   101,    -1,    -1,   104,
     105,   106,   107,    -1,   109,   110,   111,   112,    -1,    -1,
     115,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,   159,   160,    97,    98,    99,   100,
     101,   166,    -1,   104,   105,   106,   107,    -1,   109,   110,
     111,   112,    -1,    -1,   115,    -1,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
      -1,    -1,    -1,    19,    20,    -1,    -1,    -1,   159,   160,
      -1,    -1,   163,    -1,    97,    98,    99,   100,   101,    -1,
      -1,   104,   105,   106,   107,    -1,   109,   110,   111,   112,
      -1,    -1,   115,    -1,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,    -1,    -1,
      19,    20,    -1,    -1,    -1,    -1,   159,   160,    -1,    -1,
     163,    97,    98,    99,   100,   101,    -1,    -1,   104,   105,
     106,   107,    -1,   109,   110,   111,   112,    -1,    -1,   115,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,    -1,    -1,    19,    20,    -1,
      -1,    -1,    -1,   159,   160,    -1,    -1,   163,    97,    98,
      99,   100,   101,    -1,    -1,   104,   105,   106,   107,    -1,
     109,   110,   111,   112,    -1,    -1,   115,    -1,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,
     159,   160,    -1,    -1,   163,    97,    98,    99,   100,   101,
      -1,    -1,   104,   105,   106,   107,    -1,   109,   110,   111,
     112,    -1,    -1,   115,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,    -1,
      -1,    19,    20,    -1,    -1,    -1,    -1,   159,   160,    -1,
      -1,   163,    97,    98,    99,   100,   101,    -1,    -1,   104,
     105,   106,   107,    -1,   109,   110,   111,   112,    -1,    -1,
     115,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,   160,    -1,    -1,   163,    97,
      98,    99,   100,   101,    -1,    -1,   104,   105,   106,   107,
      -1,   109,   110,   111,   112,    -1,    -1,   115,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    29,    -1,
      -1,   139,    -1,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,   160,    -1,    -1,   163,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,   122,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
     131,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,   162,    19,    20,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    19,    20,   165,    97,    98,    99,   100,   101,
      -1,    -1,   104,   105,   106,   107,    -1,   109,   110,   111,
     112,    -1,    -1,   115,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,    -1,   139,    -1,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      97,    98,    99,   100,   101,    -1,    -1,   104,   105,   106,
     107,    -1,   109,   110,   111,   112,    -1,    -1,   115,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,   160,   161,    19,    20,    -1,    97,    98,
      99,   100,   101,    -1,    -1,   104,   105,   106,   107,    -1,
     109,   110,   111,   112,    -1,    -1,   115,    -1,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,   160,   161,    -1,    -1,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,   119,   120,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,   160,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,    -1,    -1,   115,   116,   117,   118,   119,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    20,    -1,    -1,    -1,   138,   139,    -1,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   159,   160,   104,
     105,   106,   107,    -1,   109,   110,   111,   112,    -1,    -1,
     115,    -1,   117,   118,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      -1,    -1,    -1,    -1,   139,    -1,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   159,   160,   104,   105,   106,   107,
      -1,   109,   110,   111,   112,    -1,    -1,   115,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   159,   160,   104,   105,   106,   107,    -1,   109,   110,
     111,   112,    -1,    -1,   115,    -1,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    20,    -1,   136,    -1,    -1,   139,    -1,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   159,   160,
     104,   105,   106,   107,    -1,   109,   110,   111,   112,    -1,
      -1,   115,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    -1,    -1,    -1,    -1,   139,    -1,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   159,   160,   104,   105,   106,
     107,    -1,   109,   110,   111,   112,    -1,    -1,   115,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   159,   160,   104,   105,   106,   107,    -1,   109,
     110,   111,   112,    -1,    -1,   115,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,   144,   145,   146,   147,   148,   149,
     150,    -1,    -1,    -1,    97,    98,    99,   100,   101,   159,
     160,   104,   105,   106,   107,    -1,   109,   110,   111,   112,
      -1,    -1,   115,    -1,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,   146,   147,   148,   149,   150,    -1,    -1,
      -1,    97,    98,    -1,    -1,   101,   159,   160,   104,   105,
     106,   107,    -1,   109,   110,   111,   112,    -1,    -1,   115,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,   149,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,   160,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,   119,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,    -1,    -1,   153,
     154
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   173,     0,     5,    28,    30,    32,    34,    39,    45,
      51,    86,   160,   166,   174,   179,   180,   184,   203,   207,
     221,   253,   257,   258,   261,   268,   269,   131,   198,   199,
     125,   185,   186,   131,   181,   182,   131,   167,   131,    32,
     131,   165,   200,   201,   202,   212,     4,     6,    56,   252,
     131,   138,   137,   140,   137,    20,    52,   149,   159,   183,
     140,   167,   131,   255,   256,   165,   131,   162,   137,   161,
     131,   260,    33,    99,   100,   131,   206,   167,   167,    13,
      14,   125,   129,   131,   132,   162,   176,   198,   125,   186,
     131,   131,   131,    61,    65,    81,    82,    83,    84,   264,
     254,   138,   168,   255,   131,   199,   201,   140,   259,   167,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   117,   118,
     119,   120,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   153,   154,    33,    33,   162,   204,   251,   131,
     244,   245,   133,   175,    13,    14,   125,   129,   131,   176,
     196,   197,   183,   167,   131,   168,    23,    29,    44,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,   212,
     263,   265,   266,   267,   163,   212,   239,   160,   163,   236,
     240,   241,   140,   205,   131,   168,   250,   262,   140,   166,
     133,   134,   137,   163,   254,   138,   166,   144,   144,   144,
     162,   144,   144,   144,   144,   144,    31,    36,    55,   109,
     139,   143,   160,   166,   171,   168,   236,   238,   199,    45,
     247,   262,   163,   166,   267,   167,   208,   137,   140,   143,
     249,   267,   168,   244,   196,   168,    13,    14,    15,    16,
      25,    37,    41,    43,    46,    50,    87,    99,   100,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   132,   146,
     147,   148,   153,   154,   162,   169,   170,   178,   208,   212,
     214,   223,   224,   227,   228,   229,   232,   233,   234,   235,
     263,   270,   273,   278,   280,   282,   267,   267,   267,   131,
     204,   267,   204,   267,   204,   267,   131,   242,   243,   267,
     245,    31,   143,   160,   171,   161,   235,    42,   237,   166,
     161,   247,   108,   138,   140,   246,   240,   210,   131,   267,
     108,   116,   138,   248,   267,   270,   162,   162,   144,   144,
     162,   144,   144,   235,   235,    11,   267,    11,   267,   235,
     275,   279,   177,   235,   235,   235,   235,   235,   235,   170,
     131,   162,   203,    19,    20,    97,    98,    99,   100,   101,
     104,   105,   106,   107,   109,   110,   111,   112,   115,   117,
     118,   139,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   159,   160,   166,   162,   145,   166,   145,   163,
     205,   145,   205,   145,   205,   145,   140,   145,   166,   145,
     166,   143,   161,   161,   247,   267,   114,   211,   230,   235,
       1,     7,     8,     9,    11,    24,    26,    27,    36,    40,
      47,    49,    53,    54,    88,   166,   168,   187,   188,   191,
     192,   193,   194,   195,   209,   211,   213,   215,   216,   217,
     218,   219,   221,   222,   230,   231,   235,   166,   248,   211,
     235,   162,   131,   235,   267,   267,   235,   267,   267,   262,
     131,   161,   162,   235,   271,   272,   274,   275,   276,   277,
     262,   162,   272,   277,   121,   166,   168,   134,   135,   175,
     163,   144,   212,   160,   163,   225,   235,   204,    17,   131,
     131,   235,   235,   235,   235,   235,   235,   131,   235,   131,
     235,   235,   235,   235,   235,    20,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   131,   235,   163,
     225,   267,   145,   145,   145,   267,   242,   246,   170,   228,
     113,    89,    90,    91,    92,    93,    94,    95,    96,   102,
     103,   108,   116,   119,   120,   138,   235,   262,   108,   166,
     211,   235,   208,   235,   235,   166,   125,    53,   235,   108,
     211,   235,    35,   166,   166,   235,   166,   166,   166,    18,
     220,   166,   108,   138,   211,   235,   166,   163,   225,    17,
     144,   235,   163,   145,   145,   163,   145,   145,    18,   246,
     161,   163,   137,   161,   166,   137,   161,   166,    18,   163,
     168,   168,   235,   275,   168,   235,   204,   267,   131,   271,
     137,   163,   205,   267,   162,   161,   131,   140,   161,   163,
     145,   235,   228,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   208,    18,
     211,   235,   166,    12,   208,   140,   125,   211,   235,   166,
     167,   208,    38,   250,   114,   114,   166,   163,   267,   131,
     163,   235,   235,   235,   162,   225,   235,   272,   131,   161,
     271,   235,   161,   276,   225,   272,   161,   161,   136,   205,
     145,   161,   235,   121,   208,   226,   163,   225,   235,   225,
     166,   208,   166,   210,    10,    21,    22,   189,   190,   235,
     235,   163,   145,   166,   163,   235,   166,   161,   246,   166,
     168,   145,   212,   163,   108,   235,   163,   166,   168,   208,
     235,    17,   235,   131,   163,   235,   161,   235,   235,   161,
     212,   235,   208,   208,   267,   163,   145,   166,   281,   281,
     190,   163,    17,   235,    48,   161,   168,   267,   163,   235,
     161,   161,   163
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   172,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   174,   175,   175,   176,   177,   177,
     177,   178,   179,   180,   181,   181,   181,   182,   182,   183,
     183,   184,   185,   185,   186,   186,   187,   188,   188,   189,
     189,   190,   190,   190,   191,   191,   192,   193,   194,   195,
     196,   196,   196,   196,   196,   196,   197,   197,   198,   198,
     198,   198,   198,   198,   198,   198,   199,   199,   200,   200,
     201,   201,   202,   202,   203,   203,   204,   204,   204,   205,
     205,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   207,   208,
     208,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   210,
     210,   210,   211,   211,   212,   212,   212,   213,   214,   214,
     214,   214,   215,   216,   217,   217,   217,   217,   217,   218,
     218,   218,   218,   219,   220,   220,   221,   221,   222,   223,
     223,   223,   224,   224,   224,   224,   224,   224,   225,   225,
     226,   226,   226,   227,   227,   228,   228,   229,   229,   229,
     229,   229,   229,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   231,
     231,   232,   233,   233,   234,   234,   234,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     236,   236,   237,   237,   238,   239,   239,   240,   240,   241,
     241,   242,   242,   243,   243,   244,   245,   245,   246,   246,
     247,   247,   247,   247,   247,   248,   248,   248,   249,   249,
     250,   250,   250,   250,   250,   251,   251,   252,   252,   253,
     254,   254,   254,   255,   256,   256,   257,   258,   258,   259,
     259,   260,   261,   262,   262,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   264,
     264,   264,   264,   264,   264,   265,   266,   266,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   268,   269,
     270,   270,   270,   270,   271,   271,   272,   272,   273,   273,
     273,   273,   273,   274,   274,   275,   276,   276,   277,   277,
     278,   278,   279,   279,   280,   281,   281,   282,   282
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
       0,     3,     5,     4,     1,     2,     4,     5,     7,     0,
       2,     2,     6,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     1,     1,
       1,     4,     3,     4,     2,     3,     2,     4,     3,     2,
       2,     3,     2,     2,     4,     6,     4,     1,     4,     5,
       1,     4,     5,     1,     4,     5,     4,     4,     1,     6,
       1,     1,     1,     1,     3,     5,     1,     3,     5,     4,
       7,     5,     7,     1,     3,     3,     1,     1,     1,     3,
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
    case 131: /* "name"  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3114 "ds_parser.cpp"
        break;

    case 175: /* character_sequence  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3120 "ds_parser.cpp"
        break;

    case 176: /* string_constant  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3126 "ds_parser.cpp"
        break;

    case 177: /* string_builder_body  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3132 "ds_parser.cpp"
        break;

    case 178: /* string_builder  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3138 "ds_parser.cpp"
        break;

    case 181: /* require_module_name  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3144 "ds_parser.cpp"
        break;

    case 187: /* expression_label  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3150 "ds_parser.cpp"
        break;

    case 188: /* expression_goto  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3156 "ds_parser.cpp"
        break;

    case 190: /* expression_else  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3162 "ds_parser.cpp"
        break;

    case 192: /* expression_if_then_else  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3168 "ds_parser.cpp"
        break;

    case 193: /* expression_for_loop  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3174 "ds_parser.cpp"
        break;

    case 194: /* expression_while_loop  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3180 "ds_parser.cpp"
        break;

    case 195: /* expression_with  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3186 "ds_parser.cpp"
        break;

    case 196: /* annotation_argument_value  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3192 "ds_parser.cpp"
        break;

    case 197: /* annotation_argument_value_list  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3198 "ds_parser.cpp"
        break;

    case 198: /* annotation_argument  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3204 "ds_parser.cpp"
        break;

    case 199: /* annotation_argument_list  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3210 "ds_parser.cpp"
        break;

    case 200: /* annotation_declaration_name  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3216 "ds_parser.cpp"
        break;

    case 201: /* annotation_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3222 "ds_parser.cpp"
        break;

    case 202: /* annotation_list  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3228 "ds_parser.cpp"
        break;

    case 203: /* optional_annotation_list  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3234 "ds_parser.cpp"
        break;

    case 204: /* optional_function_argument_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3240 "ds_parser.cpp"
        break;

    case 205: /* optional_function_type  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3246 "ds_parser.cpp"
        break;

    case 206: /* function_name  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3252 "ds_parser.cpp"
        break;

    case 208: /* expression_block  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3258 "ds_parser.cpp"
        break;

    case 209: /* expression_any  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3264 "ds_parser.cpp"
        break;

    case 210: /* expressions  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3270 "ds_parser.cpp"
        break;

    case 211: /* expr_pipe  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3276 "ds_parser.cpp"
        break;

    case 212: /* name_in_namespace  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3282 "ds_parser.cpp"
        break;

    case 213: /* expression_delete  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3288 "ds_parser.cpp"
        break;

    case 214: /* expr_new  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3294 "ds_parser.cpp"
        break;

    case 215: /* expression_break  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3300 "ds_parser.cpp"
        break;

    case 216: /* expression_continue  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3306 "ds_parser.cpp"
        break;

    case 217: /* expression_return  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3312 "ds_parser.cpp"
        break;

    case 218: /* expression_yield  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3318 "ds_parser.cpp"
        break;

    case 219: /* expression_try_catch  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3324 "ds_parser.cpp"
        break;

    case 222: /* expression_let  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3330 "ds_parser.cpp"
        break;

    case 223: /* expr_cast  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3336 "ds_parser.cpp"
        break;

    case 224: /* expr_type_info  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3342 "ds_parser.cpp"
        break;

    case 225: /* expr_list  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3348 "ds_parser.cpp"
        break;

    case 226: /* block_or_simple_block  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3354 "ds_parser.cpp"
        break;

    case 228: /* expr_block  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3360 "ds_parser.cpp"
        break;

    case 229: /* expr_numeric_const  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3366 "ds_parser.cpp"
        break;

    case 230: /* expr_assign  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3372 "ds_parser.cpp"
        break;

    case 231: /* expr_assign_pipe  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3378 "ds_parser.cpp"
        break;

    case 232: /* expr_named_call  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3384 "ds_parser.cpp"
        break;

    case 233: /* expr_method_call  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3390 "ds_parser.cpp"
        break;

    case 234: /* func_addr_expr  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3396 "ds_parser.cpp"
        break;

    case 235: /* expr  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3402 "ds_parser.cpp"
        break;

    case 236: /* optional_field_annotation  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3408 "ds_parser.cpp"
        break;

    case 238: /* structure_variable_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3414 "ds_parser.cpp"
        break;

    case 239: /* struct_variable_declaration_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3420 "ds_parser.cpp"
        break;

    case 240: /* function_argument_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3426 "ds_parser.cpp"
        break;

    case 241: /* function_argument_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3432 "ds_parser.cpp"
        break;

    case 242: /* tuple_type  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3438 "ds_parser.cpp"
        break;

    case 243: /* tuple_type_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3444 "ds_parser.cpp"
        break;

    case 244: /* variant_type  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3450 "ds_parser.cpp"
        break;

    case 245: /* variant_type_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3456 "ds_parser.cpp"
        break;

    case 247: /* variable_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3462 "ds_parser.cpp"
        break;

    case 250: /* let_variable_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3468 "ds_parser.cpp"
        break;

    case 251: /* global_variable_declaration_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3474 "ds_parser.cpp"
        break;

    case 254: /* enum_list  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3480 "ds_parser.cpp"
        break;

    case 259: /* optional_structure_parent  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3486 "ds_parser.cpp"
        break;

    case 262: /* variable_name_list  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3492 "ds_parser.cpp"
        break;

    case 265: /* structure_type_declaration  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3498 "ds_parser.cpp"
        break;

    case 266: /* auto_type_declaration  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3504 "ds_parser.cpp"
        break;

    case 267: /* type_declaration  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3510 "ds_parser.cpp"
        break;

    case 270: /* make_decl  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3516 "ds_parser.cpp"
        break;

    case 271: /* make_struct_fields  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3522 "ds_parser.cpp"
        break;

    case 272: /* make_struct_dim  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3528 "ds_parser.cpp"
        break;

    case 273: /* make_struct_decl  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3534 "ds_parser.cpp"
        break;

    case 274: /* make_tuple  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3540 "ds_parser.cpp"
        break;

    case 275: /* make_map_tuple  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3546 "ds_parser.cpp"
        break;

    case 276: /* make_any_tuple  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3552 "ds_parser.cpp"
        break;

    case 277: /* make_dim  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3558 "ds_parser.cpp"
        break;

    case 278: /* make_dim_decl  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3564 "ds_parser.cpp"
        break;

    case 279: /* make_table  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3570 "ds_parser.cpp"
        break;

    case 280: /* make_table_decl  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3576 "ds_parser.cpp"
        break;

    case 281: /* array_comprehension_where  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3582 "ds_parser.cpp"
        break;

    case 282: /* array_comprehension  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3588 "ds_parser.cpp"
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
#line 400 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3888 "ds_parser.cpp"
    break;

  case 15:
#line 412 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3894 "ds_parser.cpp"
    break;

  case 16:
#line 413 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3900 "ds_parser.cpp"
    break;

  case 17:
#line 417 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 3906 "ds_parser.cpp"
    break;

  case 18:
#line 421 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3915 "ds_parser.cpp"
    break;

  case 19:
#line 425 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3929 "ds_parser.cpp"
    break;

  case 20:
#line 434 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3939 "ds_parser.cpp"
    break;

  case 21:
#line 442 "ds_parser.ypp"
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
#line 3957 "ds_parser.cpp"
    break;

  case 22:
#line 458 "ds_parser.ypp"
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
		}
		delete (yyvsp[0].aaList);
	}
#line 3980 "ds_parser.cpp"
    break;

  case 24:
#line 483 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3988 "ds_parser.cpp"
    break;

  case 25:
#line 486 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3999 "ds_parser.cpp"
    break;

  case 26:
#line 492 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4010 "ds_parser.cpp"
    break;

  case 27:
#line 501 "ds_parser.ypp"
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
#line 4036 "ds_parser.cpp"
    break;

  case 28:
#line 522 "ds_parser.ypp"
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
#line 4061 "ds_parser.cpp"
    break;

  case 29:
#line 545 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4067 "ds_parser.cpp"
    break;

  case 30:
#line 546 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4073 "ds_parser.cpp"
    break;

  case 34:
#line 559 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4081 "ds_parser.cpp"
    break;

  case 35:
#line 562 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4089 "ds_parser.cpp"
    break;

  case 36:
#line 568 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4097 "ds_parser.cpp"
    break;

  case 37:
#line 574 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4105 "ds_parser.cpp"
    break;

  case 38:
#line 577 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4113 "ds_parser.cpp"
    break;

  case 39:
#line 583 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4119 "ds_parser.cpp"
    break;

  case 40:
#line 584 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4125 "ds_parser.cpp"
    break;

  case 41:
#line 588 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4131 "ds_parser.cpp"
    break;

  case 42:
#line 589 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4137 "ds_parser.cpp"
    break;

  case 43:
#line 590 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4148 "ds_parser.cpp"
    break;

  case 44:
#line 599 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4154 "ds_parser.cpp"
    break;

  case 45:
#line 600 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4160 "ds_parser.cpp"
    break;

  case 46:
#line 604 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4171 "ds_parser.cpp"
    break;

  case 47:
#line 613 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 4185 "ds_parser.cpp"
    break;

  case 48:
#line 625 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4197 "ds_parser.cpp"
    break;

  case 49:
#line 635 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4208 "ds_parser.cpp"
    break;

  case 50:
#line 644 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4214 "ds_parser.cpp"
    break;

  case 51:
#line 645 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4220 "ds_parser.cpp"
    break;

  case 52:
#line 646 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4226 "ds_parser.cpp"
    break;

  case 53:
#line 647 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4232 "ds_parser.cpp"
    break;

  case 54:
#line 648 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4238 "ds_parser.cpp"
    break;

  case 55:
#line 649 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4244 "ds_parser.cpp"
    break;

  case 56:
#line 653 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4254 "ds_parser.cpp"
    break;

  case 57:
#line 658 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4264 "ds_parser.cpp"
    break;

  case 58:
#line 666 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4270 "ds_parser.cpp"
    break;

  case 59:
#line 667 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4276 "ds_parser.cpp"
    break;

  case 60:
#line 668 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4282 "ds_parser.cpp"
    break;

  case 61:
#line 669 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4288 "ds_parser.cpp"
    break;

  case 62:
#line 670 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4294 "ds_parser.cpp"
    break;

  case 63:
#line 671 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4300 "ds_parser.cpp"
    break;

  case 64:
#line 672 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4306 "ds_parser.cpp"
    break;

  case 65:
#line 673 "ds_parser.ypp"
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4314 "ds_parser.cpp"
    break;

  case 66:
#line 679 "ds_parser.ypp"
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
#line 4332 "ds_parser.cpp"
    break;

  case 67:
#line 692 "ds_parser.ypp"
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
#line 4350 "ds_parser.cpp"
    break;

  case 68:
#line 708 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4356 "ds_parser.cpp"
    break;

  case 69:
#line 709 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4362 "ds_parser.cpp"
    break;

  case 70:
#line 713 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 4374 "ds_parser.cpp"
    break;

  case 71:
#line 720 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4388 "ds_parser.cpp"
    break;

  case 72:
#line 732 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4397 "ds_parser.cpp"
    break;

  case 73:
#line 736 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4406 "ds_parser.cpp"
    break;

  case 74:
#line 743 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4412 "ds_parser.cpp"
    break;

  case 75:
#line 744 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4418 "ds_parser.cpp"
    break;

  case 76:
#line 748 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4424 "ds_parser.cpp"
    break;

  case 77:
#line 749 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4430 "ds_parser.cpp"
    break;

  case 78:
#line 750 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4436 "ds_parser.cpp"
    break;

  case 79:
#line 754 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4445 "ds_parser.cpp"
    break;

  case 80:
#line 758 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4454 "ds_parser.cpp"
    break;

  case 81:
#line 765 "ds_parser.ypp"
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 4463 "ds_parser.cpp"
    break;

  case 82:
#line 769 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4469 "ds_parser.cpp"
    break;

  case 83:
#line 770 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4475 "ds_parser.cpp"
    break;

  case 84:
#line 771 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4481 "ds_parser.cpp"
    break;

  case 85:
#line 772 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4487 "ds_parser.cpp"
    break;

  case 86:
#line 773 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4493 "ds_parser.cpp"
    break;

  case 87:
#line 774 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4499 "ds_parser.cpp"
    break;

  case 88:
#line 775 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4505 "ds_parser.cpp"
    break;

  case 89:
#line 776 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4511 "ds_parser.cpp"
    break;

  case 90:
#line 777 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4517 "ds_parser.cpp"
    break;

  case 91:
#line 778 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4523 "ds_parser.cpp"
    break;

  case 92:
#line 779 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4529 "ds_parser.cpp"
    break;

  case 93:
#line 780 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4535 "ds_parser.cpp"
    break;

  case 94:
#line 781 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4541 "ds_parser.cpp"
    break;

  case 95:
#line 782 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4547 "ds_parser.cpp"
    break;

  case 96:
#line 783 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4553 "ds_parser.cpp"
    break;

  case 97:
#line 784 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4559 "ds_parser.cpp"
    break;

  case 98:
#line 785 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4565 "ds_parser.cpp"
    break;

  case 99:
#line 786 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4571 "ds_parser.cpp"
    break;

  case 100:
#line 787 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4577 "ds_parser.cpp"
    break;

  case 101:
#line 788 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4583 "ds_parser.cpp"
    break;

  case 102:
#line 789 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4589 "ds_parser.cpp"
    break;

  case 103:
#line 790 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4595 "ds_parser.cpp"
    break;

  case 104:
#line 791 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4601 "ds_parser.cpp"
    break;

  case 105:
#line 792 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4607 "ds_parser.cpp"
    break;

  case 106:
#line 793 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4613 "ds_parser.cpp"
    break;

  case 107:
#line 794 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4619 "ds_parser.cpp"
    break;

  case 108:
#line 795 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4625 "ds_parser.cpp"
    break;

  case 109:
#line 796 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4631 "ds_parser.cpp"
    break;

  case 110:
#line 797 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4637 "ds_parser.cpp"
    break;

  case 111:
#line 798 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4643 "ds_parser.cpp"
    break;

  case 112:
#line 799 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4649 "ds_parser.cpp"
    break;

  case 113:
#line 800 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4655 "ds_parser.cpp"
    break;

  case 114:
#line 801 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4661 "ds_parser.cpp"
    break;

  case 115:
#line 802 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4667 "ds_parser.cpp"
    break;

  case 116:
#line 803 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4673 "ds_parser.cpp"
    break;

  case 117:
#line 804 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4679 "ds_parser.cpp"
    break;

  case 118:
#line 809 "ds_parser.ypp"
    {
        auto pFunction = make_shared<Function>();
        pFunction->at = tokAt((yylsp[-4]));
        pFunction->name = *(yyvsp[-3].s);
        pFunction->body = ExpressionPtr((yyvsp[0].pExpression));
        pFunction->result = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name : *pDecl->pNameList ) {
                        if ( !pFunction->findArgument(name) ) {
                            VariablePtr pVar = make_shared<Variable>();
                            pVar->name = name;
                            pVar->at = pDecl->at;
                            pVar->type = make_shared<TypeDecl>(*pDecl->pTypeDecl);
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
                                tokAt((yylsp[-4])), CompilationError::invalid_annotation);
                        }
                    } else {
                        das_yyerror("functions are only allowed function annotations",
                            tokAt((yylsp[-4])), CompilationError::invalid_annotation);
                    }
                }
            }
            swap ( pFunction->annotations, *(yyvsp[-5].faList) );
            delete (yyvsp[-5].faList);
        }
        if ( pFunction->isGeneric() ) {
            if ( !g_Program->addGeneric(pFunction) ) {
                das_yyerror("generic function is already defined " +
                    pFunction->getMangledName(),tokAt((yylsp[-4])),
                        CompilationError::function_already_declared);
                pFunction.reset();
            }
        } else {
            if ( !g_Program->addFunction(pFunction) ) {
                das_yyerror("function is already defined " +
                    pFunction->getMangledName(),tokAt((yylsp[-4])),
                        CompilationError::function_already_declared);
                pFunction.reset();
            }
        }
        delete (yyvsp[-3].s);
    }
#line 4753 "ds_parser.cpp"
    break;

  case 119:
#line 881 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4761 "ds_parser.cpp"
    break;

  case 120:
#line 884 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4773 "ds_parser.cpp"
    break;

  case 121:
#line 894 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4779 "ds_parser.cpp"
    break;

  case 122:
#line 895 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4785 "ds_parser.cpp"
    break;

  case 123:
#line 896 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4791 "ds_parser.cpp"
    break;

  case 124:
#line 897 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4797 "ds_parser.cpp"
    break;

  case 125:
#line 898 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4803 "ds_parser.cpp"
    break;

  case 126:
#line 899 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4809 "ds_parser.cpp"
    break;

  case 127:
#line 900 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4815 "ds_parser.cpp"
    break;

  case 128:
#line 901 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4821 "ds_parser.cpp"
    break;

  case 129:
#line 902 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4827 "ds_parser.cpp"
    break;

  case 130:
#line 903 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4833 "ds_parser.cpp"
    break;

  case 131:
#line 904 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4839 "ds_parser.cpp"
    break;

  case 132:
#line 905 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4845 "ds_parser.cpp"
    break;

  case 133:
#line 906 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4851 "ds_parser.cpp"
    break;

  case 134:
#line 907 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4857 "ds_parser.cpp"
    break;

  case 135:
#line 908 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4863 "ds_parser.cpp"
    break;

  case 136:
#line 909 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4869 "ds_parser.cpp"
    break;

  case 137:
#line 910 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4875 "ds_parser.cpp"
    break;

  case 138:
#line 911 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4881 "ds_parser.cpp"
    break;

  case 139:
#line 915 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4890 "ds_parser.cpp"
    break;

  case 140:
#line 919 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4901 "ds_parser.cpp"
    break;

  case 141:
#line 925 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4909 "ds_parser.cpp"
    break;

  case 142:
#line 931 "ds_parser.ypp"
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
#line 4925 "ds_parser.cpp"
    break;

  case 143:
#line 942 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4933 "ds_parser.cpp"
    break;

  case 144:
#line 948 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4939 "ds_parser.cpp"
    break;

  case 145:
#line 949 "ds_parser.ypp"
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
#line 4955 "ds_parser.cpp"
    break;

  case 146:
#line 960 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4961 "ds_parser.cpp"
    break;

  case 147:
#line 964 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4969 "ds_parser.cpp"
    break;

  case 148:
#line 970 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4977 "ds_parser.cpp"
    break;

  case 149:
#line 973 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4985 "ds_parser.cpp"
    break;

  case 150:
#line 976 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4994 "ds_parser.cpp"
    break;

  case 151:
#line 980 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5002 "ds_parser.cpp"
    break;

  case 152:
#line 986 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5008 "ds_parser.cpp"
    break;

  case 153:
#line 990 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5014 "ds_parser.cpp"
    break;

  case 154:
#line 994 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5022 "ds_parser.cpp"
    break;

  case 155:
#line 997 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5030 "ds_parser.cpp"
    break;

  case 156:
#line 1000 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5040 "ds_parser.cpp"
    break;

  case 157:
#line 1005 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5048 "ds_parser.cpp"
    break;

  case 158:
#line 1008 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5058 "ds_parser.cpp"
    break;

  case 159:
#line 1016 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5066 "ds_parser.cpp"
    break;

  case 160:
#line 1019 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5076 "ds_parser.cpp"
    break;

  case 161:
#line 1024 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5084 "ds_parser.cpp"
    break;

  case 162:
#line 1027 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5094 "ds_parser.cpp"
    break;

  case 163:
#line 1035 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5102 "ds_parser.cpp"
    break;

  case 164:
#line 1041 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5108 "ds_parser.cpp"
    break;

  case 165:
#line 1042 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5114 "ds_parser.cpp"
    break;

  case 166:
#line 1046 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5120 "ds_parser.cpp"
    break;

  case 167:
#line 1047 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5126 "ds_parser.cpp"
    break;

  case 168:
#line 1051 "ds_parser.ypp"
    {
        auto pLet = new ExprLet();
        pLet->at = tokAt((yylsp[-2]));
        pLet->inScope = (yyvsp[-1].b);
        if ( (yyvsp[0].pVarDecl)->pTypeDecl ) {
            for ( const auto & name : *(yyvsp[0].pVarDecl)->pNameList ) {
                if ( !pLet->find(name) ) {
                    VariablePtr pVar = make_shared<Variable>();
                    pVar->name = name;
                    pVar->at = (yyvsp[0].pVarDecl)->at;
                    pVar->type = make_shared<TypeDecl>(*(yyvsp[0].pVarDecl)->pTypeDecl);
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
#line 5162 "ds_parser.cpp"
    break;

  case 169:
#line 1085 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5170 "ds_parser.cpp"
    break;

  case 170:
#line 1088 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5180 "ds_parser.cpp"
    break;

  case 171:
#line 1093 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5190 "ds_parser.cpp"
    break;

  case 172:
#line 1101 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5199 "ds_parser.cpp"
    break;

  case 173:
#line 1105 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5209 "ds_parser.cpp"
    break;

  case 174:
#line 1110 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5220 "ds_parser.cpp"
    break;

  case 175:
#line 1116 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 5229 "ds_parser.cpp"
    break;

  case 176:
#line 1120 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5239 "ds_parser.cpp"
    break;

  case 177:
#line 1125 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5250 "ds_parser.cpp"
    break;

  case 178:
#line 1134 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5258 "ds_parser.cpp"
    break;

  case 179:
#line 1137 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5266 "ds_parser.cpp"
    break;

  case 180:
#line 1143 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5272 "ds_parser.cpp"
    break;

  case 181:
#line 1144 "ds_parser.ypp"
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5284 "ds_parser.cpp"
    break;

  case 182:
#line 1151 "ds_parser.ypp"
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5297 "ds_parser.cpp"
    break;

  case 183:
#line 1162 "ds_parser.ypp"
    { (yyval.b) = false;   /* block */  }
#line 5303 "ds_parser.cpp"
    break;

  case 184:
#line 1163 "ds_parser.ypp"
    { (yyval.b) = true;    /* lambda */ }
#line 5309 "ds_parser.cpp"
    break;

  case 185:
#line 1167 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 5319 "ds_parser.cpp"
    break;

  case 186:
#line 1173 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)), (yyvsp[-4].b));
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        closure->returnType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name : *pDecl->pNameList ) {
                        if ( !closure->findArgument(name) ) {
                            VariablePtr pVar = make_shared<Variable>();
                            pVar->name = name;
                            pVar->at = pDecl->at;
                            pVar->type = make_shared<TypeDecl>(*pDecl->pTypeDecl);
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
#line 5375 "ds_parser.cpp"
    break;

  case 187:
#line 1227 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5381 "ds_parser.cpp"
    break;

  case 188:
#line 1228 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5387 "ds_parser.cpp"
    break;

  case 189:
#line 1229 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5393 "ds_parser.cpp"
    break;

  case 190:
#line 1230 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5399 "ds_parser.cpp"
    break;

  case 191:
#line 1231 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5405 "ds_parser.cpp"
    break;

  case 192:
#line 1232 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5411 "ds_parser.cpp"
    break;

  case 193:
#line 1236 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5417 "ds_parser.cpp"
    break;

  case 194:
#line 1237 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5423 "ds_parser.cpp"
    break;

  case 195:
#line 1238 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5429 "ds_parser.cpp"
    break;

  case 196:
#line 1239 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5435 "ds_parser.cpp"
    break;

  case 197:
#line 1240 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5441 "ds_parser.cpp"
    break;

  case 198:
#line 1241 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5447 "ds_parser.cpp"
    break;

  case 199:
#line 1242 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5453 "ds_parser.cpp"
    break;

  case 200:
#line 1243 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5459 "ds_parser.cpp"
    break;

  case 201:
#line 1244 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5465 "ds_parser.cpp"
    break;

  case 202:
#line 1245 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5471 "ds_parser.cpp"
    break;

  case 203:
#line 1246 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5477 "ds_parser.cpp"
    break;

  case 204:
#line 1247 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5483 "ds_parser.cpp"
    break;

  case 205:
#line 1248 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5489 "ds_parser.cpp"
    break;

  case 206:
#line 1249 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5495 "ds_parser.cpp"
    break;

  case 207:
#line 1250 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5501 "ds_parser.cpp"
    break;

  case 208:
#line 1251 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5507 "ds_parser.cpp"
    break;

  case 209:
#line 1255 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5513 "ds_parser.cpp"
    break;

  case 210:
#line 1256 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5519 "ds_parser.cpp"
    break;

  case 211:
#line 1260 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5531 "ds_parser.cpp"
    break;

  case 212:
#line 1271 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5541 "ds_parser.cpp"
    break;

  case 213:
#line 1276 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5553 "ds_parser.cpp"
    break;

  case 214:
#line 1286 "ds_parser.ypp"
    { 
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
	}
#line 5562 "ds_parser.cpp"
    break;

  case 215:
#line 1290 "ds_parser.ypp"
    {
		auto expr = new ExprAddr(tokAt((yylsp[-4])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
        expr->funcType = make_shared<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5574 "ds_parser.cpp"
    break;

  case 216:
#line 1297 "ds_parser.ypp"
    { 
		auto expr = new ExprAddr(tokAt((yylsp[-5])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
        expr->funcType = make_shared<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(expr->funcType.get(), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
		(yyval.pExpression) = expr;
    }
#line 5590 "ds_parser.cpp"
    break;

  case 217:
#line 1311 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5596 "ds_parser.cpp"
    break;

  case 218:
#line 1312 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5602 "ds_parser.cpp"
    break;

  case 219:
#line 1313 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5608 "ds_parser.cpp"
    break;

  case 220:
#line 1314 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5614 "ds_parser.cpp"
    break;

  case 221:
#line 1315 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5620 "ds_parser.cpp"
    break;

  case 222:
#line 1316 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5626 "ds_parser.cpp"
    break;

  case 223:
#line 1317 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5632 "ds_parser.cpp"
    break;

  case 224:
#line 1318 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5638 "ds_parser.cpp"
    break;

  case 225:
#line 1319 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5644 "ds_parser.cpp"
    break;

  case 226:
#line 1320 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5650 "ds_parser.cpp"
    break;

  case 227:
#line 1321 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5656 "ds_parser.cpp"
    break;

  case 228:
#line 1322 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5662 "ds_parser.cpp"
    break;

  case 229:
#line 1323 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5668 "ds_parser.cpp"
    break;

  case 230:
#line 1324 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5674 "ds_parser.cpp"
    break;

  case 231:
#line 1325 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5680 "ds_parser.cpp"
    break;

  case 232:
#line 1326 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5686 "ds_parser.cpp"
    break;

  case 233:
#line 1327 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5692 "ds_parser.cpp"
    break;

  case 234:
#line 1328 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5698 "ds_parser.cpp"
    break;

  case 235:
#line 1329 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5704 "ds_parser.cpp"
    break;

  case 236:
#line 1330 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5710 "ds_parser.cpp"
    break;

  case 237:
#line 1331 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5716 "ds_parser.cpp"
    break;

  case 238:
#line 1332 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5722 "ds_parser.cpp"
    break;

  case 239:
#line 1333 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5728 "ds_parser.cpp"
    break;

  case 240:
#line 1334 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5734 "ds_parser.cpp"
    break;

  case 241:
#line 1335 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5740 "ds_parser.cpp"
    break;

  case 242:
#line 1336 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5746 "ds_parser.cpp"
    break;

  case 243:
#line 1337 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5752 "ds_parser.cpp"
    break;

  case 244:
#line 1338 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5758 "ds_parser.cpp"
    break;

  case 245:
#line 1339 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5764 "ds_parser.cpp"
    break;

  case 246:
#line 1340 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5770 "ds_parser.cpp"
    break;

  case 247:
#line 1341 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5776 "ds_parser.cpp"
    break;

  case 248:
#line 1342 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5782 "ds_parser.cpp"
    break;

  case 249:
#line 1343 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5788 "ds_parser.cpp"
    break;

  case 250:
#line 1344 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5794 "ds_parser.cpp"
    break;

  case 251:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5800 "ds_parser.cpp"
    break;

  case 252:
#line 1346 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5806 "ds_parser.cpp"
    break;

  case 253:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5812 "ds_parser.cpp"
    break;

  case 254:
#line 1348 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5818 "ds_parser.cpp"
    break;

  case 255:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5824 "ds_parser.cpp"
    break;

  case 256:
#line 1350 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5833 "ds_parser.cpp"
    break;

  case 257:
#line 1354 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5842 "ds_parser.cpp"
    break;

  case 258:
#line 1358 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5850 "ds_parser.cpp"
    break;

  case 259:
#line 1361 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5858 "ds_parser.cpp"
    break;

  case 260:
#line 1364 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5864 "ds_parser.cpp"
    break;

  case 261:
#line 1365 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5870 "ds_parser.cpp"
    break;

  case 262:
#line 1366 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5876 "ds_parser.cpp"
    break;

  case 263:
#line 1367 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5886 "ds_parser.cpp"
    break;

  case 264:
#line 1372 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5896 "ds_parser.cpp"
    break;

  case 265:
#line 1377 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5902 "ds_parser.cpp"
    break;

  case 266:
#line 1378 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5910 "ds_parser.cpp"
    break;

  case 267:
#line 1381 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5918 "ds_parser.cpp"
    break;

  case 268:
#line 1384 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5927 "ds_parser.cpp"
    break;

  case 269:
#line 1388 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5936 "ds_parser.cpp"
    break;

  case 270:
#line 1392 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 5945 "ds_parser.cpp"
    break;

  case 271:
#line 1396 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5951 "ds_parser.cpp"
    break;

  case 272:
#line 1397 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5957 "ds_parser.cpp"
    break;

  case 273:
#line 1398 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5963 "ds_parser.cpp"
    break;

  case 274:
#line 1399 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5969 "ds_parser.cpp"
    break;

  case 275:
#line 1400 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5975 "ds_parser.cpp"
    break;

  case 276:
#line 1401 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5981 "ds_parser.cpp"
    break;

  case 277:
#line 1402 "ds_parser.ypp"
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
#line 5996 "ds_parser.cpp"
    break;

  case 278:
#line 1412 "ds_parser.ypp"
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
#line 6011 "ds_parser.cpp"
    break;

  case 279:
#line 1422 "ds_parser.ypp"
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
				auto td = make_shared<TypeDecl>(pEnum);
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
#line 6047 "ds_parser.cpp"
    break;

  case 280:
#line 1456 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6053 "ds_parser.cpp"
    break;

  case 281:
#line 1457 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 6059 "ds_parser.cpp"
    break;

  case 282:
#line 1461 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6065 "ds_parser.cpp"
    break;

  case 283:
#line 1462 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6071 "ds_parser.cpp"
    break;

  case 284:
#line 1466 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6081 "ds_parser.cpp"
    break;

  case 285:
#line 1474 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6089 "ds_parser.cpp"
    break;

  case 286:
#line 1477 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6098 "ds_parser.cpp"
    break;

  case 287:
#line 1484 "ds_parser.ypp"
    { 
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->constant = true; 
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
		}
#line 6108 "ds_parser.cpp"
    break;

  case 288:
#line 1489 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl); 
			(yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
		}
#line 6118 "ds_parser.cpp"
    break;

  case 289:
#line 1497 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6124 "ds_parser.cpp"
    break;

  case 290:
#line 1498 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6130 "ds_parser.cpp"
    break;

  case 291:
#line 1502 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6138 "ds_parser.cpp"
    break;

  case 292:
#line 1505 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6149 "ds_parser.cpp"
    break;

  case 293:
#line 1514 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6155 "ds_parser.cpp"
    break;

  case 294:
#line 1515 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6161 "ds_parser.cpp"
    break;

  case 295:
#line 1519 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6172 "ds_parser.cpp"
    break;

  case 296:
#line 1528 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6178 "ds_parser.cpp"
    break;

  case 297:
#line 1529 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6184 "ds_parser.cpp"
    break;

  case 298:
#line 1534 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6190 "ds_parser.cpp"
    break;

  case 299:
#line 1535 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6196 "ds_parser.cpp"
    break;

  case 300:
#line 1539 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 6206 "ds_parser.cpp"
    break;

  case 301:
#line 1544 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6214 "ds_parser.cpp"
    break;

  case 302:
#line 1547 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6223 "ds_parser.cpp"
    break;

  case 303:
#line 1551 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6234 "ds_parser.cpp"
    break;

  case 304:
#line 1557 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6245 "ds_parser.cpp"
    break;

  case 305:
#line 1566 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6251 "ds_parser.cpp"
    break;

  case 306:
#line 1567 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6257 "ds_parser.cpp"
    break;

  case 307:
#line 1568 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6263 "ds_parser.cpp"
    break;

  case 308:
#line 1572 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6269 "ds_parser.cpp"
    break;

  case 309:
#line 1573 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6275 "ds_parser.cpp"
    break;

  case 310:
#line 1577 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6283 "ds_parser.cpp"
    break;

  case 311:
#line 1580 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6293 "ds_parser.cpp"
    break;

  case 312:
#line 1585 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6303 "ds_parser.cpp"
    break;

  case 313:
#line 1590 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6316 "ds_parser.cpp"
    break;

  case 314:
#line 1598 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6329 "ds_parser.cpp"
    break;

  case 315:
#line 1609 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6337 "ds_parser.cpp"
    break;

  case 316:
#line 1612 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6346 "ds_parser.cpp"
    break;

  case 317:
#line 1619 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6352 "ds_parser.cpp"
    break;

  case 318:
#line 1620 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6358 "ds_parser.cpp"
    break;

  case 319:
#line 1624 "ds_parser.ypp"
    {
        for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
            if ( pDecl->pTypeDecl ) {
                for ( const auto & name : *pDecl->pNameList ) {
                    VariablePtr pVar = make_shared<Variable>();
                    pVar->name = name;
                    pVar->at = pDecl->at;
                    pVar->type = make_shared<TypeDecl>(*pDecl->pTypeDecl);
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
#line 6390 "ds_parser.cpp"
    break;

  case 320:
#line 1654 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6398 "ds_parser.cpp"
    break;

  case 321:
#line 1657 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6412 "ds_parser.cpp"
    break;

  case 322:
#line 1666 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6426 "ds_parser.cpp"
    break;

  case 323:
#line 1679 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 6440 "ds_parser.cpp"
    break;

  case 327:
#line 1700 "ds_parser.ypp"
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
#line 6456 "ds_parser.cpp"
    break;

  case 328:
#line 1711 "ds_parser.ypp"
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
#line 6473 "ds_parser.cpp"
    break;

  case 329:
#line 1726 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 6479 "ds_parser.cpp"
    break;

  case 330:
#line 1727 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 6485 "ds_parser.cpp"
    break;

  case 331:
#line 1731 "ds_parser.ypp"
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
            pStruct = make_shared<Structure>(*(yyvsp[-1].s));
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
#line 6526 "ds_parser.cpp"
    break;

  case 332:
#line 1770 "ds_parser.ypp"
    {
        if ( (yyvsp[-3].pStructure) ) {
            auto pStruct = (yyvsp[-3].pStructure)->shared_from_this();
            pStruct->at = tokAt((yylsp[-4]));
            for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
                for ( const auto & name : *pDecl->pNameList ) {
                    auto oldFd = (Structure::FieldDeclaration *) pStruct->findField(name);
                    if ( !oldFd ) {
                        if ( pDecl->override ) {
                            das_yyerror("structure field is not overriding anything, "+name,pDecl->at,
                                CompilationError::invalid_override);
                        } else {
                            auto td = make_shared<TypeDecl>(*pDecl->pTypeDecl);
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
#line 6594 "ds_parser.cpp"
    break;

  case 333:
#line 1836 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6606 "ds_parser.cpp"
    break;

  case 334:
#line 1843 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6617 "ds_parser.cpp"
    break;

  case 335:
#line 1852 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 6623 "ds_parser.cpp"
    break;

  case 336:
#line 1853 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 6629 "ds_parser.cpp"
    break;

  case 337:
#line 1854 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6635 "ds_parser.cpp"
    break;

  case 338:
#line 1855 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6641 "ds_parser.cpp"
    break;

  case 339:
#line 1856 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6647 "ds_parser.cpp"
    break;

  case 340:
#line 1857 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 6653 "ds_parser.cpp"
    break;

  case 341:
#line 1858 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 6659 "ds_parser.cpp"
    break;

  case 342:
#line 1859 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 6665 "ds_parser.cpp"
    break;

  case 343:
#line 1860 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 6671 "ds_parser.cpp"
    break;

  case 344:
#line 1861 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6677 "ds_parser.cpp"
    break;

  case 345:
#line 1862 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6683 "ds_parser.cpp"
    break;

  case 346:
#line 1863 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6689 "ds_parser.cpp"
    break;

  case 347:
#line 1864 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 6695 "ds_parser.cpp"
    break;

  case 348:
#line 1865 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 6701 "ds_parser.cpp"
    break;

  case 349:
#line 1866 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 6707 "ds_parser.cpp"
    break;

  case 350:
#line 1867 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 6713 "ds_parser.cpp"
    break;

  case 351:
#line 1868 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 6719 "ds_parser.cpp"
    break;

  case 352:
#line 1869 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 6725 "ds_parser.cpp"
    break;

  case 353:
#line 1870 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 6731 "ds_parser.cpp"
    break;

  case 354:
#line 1871 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 6737 "ds_parser.cpp"
    break;

  case 355:
#line 1872 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 6743 "ds_parser.cpp"
    break;

  case 356:
#line 1873 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 6749 "ds_parser.cpp"
    break;

  case 357:
#line 1874 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 6755 "ds_parser.cpp"
    break;

  case 358:
#line 1875 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 6761 "ds_parser.cpp"
    break;

  case 359:
#line 1879 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6767 "ds_parser.cpp"
    break;

  case 360:
#line 1880 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6773 "ds_parser.cpp"
    break;

  case 361:
#line 1881 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6779 "ds_parser.cpp"
    break;

  case 362:
#line 1882 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6785 "ds_parser.cpp"
    break;

  case 363:
#line 1883 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6791 "ds_parser.cpp"
    break;

  case 364:
#line 1884 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6797 "ds_parser.cpp"
    break;

  case 365:
#line 1888 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6810 "ds_parser.cpp"
    break;

  case 366:
#line 1899 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6819 "ds_parser.cpp"
    break;

  case 367:
#line 1903 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6831 "ds_parser.cpp"
    break;

  case 368:
#line 1913 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6837 "ds_parser.cpp"
    break;

  case 369:
#line 1914 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6843 "ds_parser.cpp"
    break;

  case 370:
#line 1915 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6849 "ds_parser.cpp"
    break;

  case 371:
#line 1916 "ds_parser.ypp"
    {
        int32_t dI = TypeDecl::dimConst;
        if ( (yyvsp[-1].pExpression)->rtti_isConstant() ) {                // note: this shortcut is here so we don't get extra infer pass on every array
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
#line 6868 "ds_parser.cpp"
    break;

  case 372:
#line 1930 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6879 "ds_parser.cpp"
    break;

  case 373:
#line 1936 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6888 "ds_parser.cpp"
    break;

  case 374:
#line 1940 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6898 "ds_parser.cpp"
    break;

  case 375:
#line 1945 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6908 "ds_parser.cpp"
    break;

  case 376:
#line 1950 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6918 "ds_parser.cpp"
    break;

  case 377:
#line 1955 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6930 "ds_parser.cpp"
    break;

  case 378:
#line 1962 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6940 "ds_parser.cpp"
    break;

  case 379:
#line 1967 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6949 "ds_parser.cpp"
    break;

  case 380:
#line 1971 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6958 "ds_parser.cpp"
    break;

  case 381:
#line 1975 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6968 "ds_parser.cpp"
    break;

  case 382:
#line 1980 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6978 "ds_parser.cpp"
    break;

  case 383:
#line 1985 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6990 "ds_parser.cpp"
    break;

  case 384:
#line 1992 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7000 "ds_parser.cpp"
    break;

  case 385:
#line 1997 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7011 "ds_parser.cpp"
    break;

  case 386:
#line 2003 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7021 "ds_parser.cpp"
    break;

  case 387:
#line 2008 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7030 "ds_parser.cpp"
    break;

  case 388:
#line 2012 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7040 "ds_parser.cpp"
    break;

  case 389:
#line 2017 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7054 "ds_parser.cpp"
    break;

  case 390:
#line 2026 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7063 "ds_parser.cpp"
    break;

  case 391:
#line 2030 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7073 "ds_parser.cpp"
    break;

  case 392:
#line 2035 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7087 "ds_parser.cpp"
    break;

  case 393:
#line 2044 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7096 "ds_parser.cpp"
    break;

  case 394:
#line 2048 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7106 "ds_parser.cpp"
    break;

  case 395:
#line 2053 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7120 "ds_parser.cpp"
    break;

  case 396:
#line 2062 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7131 "ds_parser.cpp"
    break;

  case 397:
#line 2068 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7142 "ds_parser.cpp"
    break;

  case 398:
#line 2077 "ds_parser.ypp"
    {
        das_need_oxford_comma=false;
    }
#line 7150 "ds_parser.cpp"
    break;

  case 399:
#line 2083 "ds_parser.ypp"
    {
        auto vtype = make_shared<TypeDecl>(Type::tVariant);
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
#line 7167 "ds_parser.cpp"
    break;

  case 400:
#line 2098 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7173 "ds_parser.cpp"
    break;

  case 401:
#line 2099 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7179 "ds_parser.cpp"
    break;

  case 402:
#line 2100 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7185 "ds_parser.cpp"
    break;

  case 403:
#line 2101 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7191 "ds_parser.cpp"
    break;

  case 404:
#line 2105 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7203 "ds_parser.cpp"
    break;

  case 405:
#line 2112 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7214 "ds_parser.cpp"
    break;

  case 406:
#line 2121 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 7224 "ds_parser.cpp"
    break;

  case 407:
#line 2126 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7233 "ds_parser.cpp"
    break;

  case 408:
#line 2133 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7243 "ds_parser.cpp"
    break;

  case 409:
#line 2138 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7254 "ds_parser.cpp"
    break;

  case 410:
#line 2144 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7265 "ds_parser.cpp"
    break;

  case 411:
#line 2150 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7277 "ds_parser.cpp"
    break;

  case 412:
#line 2157 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7290 "ds_parser.cpp"
    break;

  case 413:
#line 2168 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7298 "ds_parser.cpp"
    break;

  case 414:
#line 2171 "ds_parser.ypp"
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
#line 7314 "ds_parser.cpp"
    break;

  case 415:
#line 2185 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7325 "ds_parser.cpp"
    break;

  case 416:
#line 2194 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7331 "ds_parser.cpp"
    break;

  case 417:
#line 2195 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7337 "ds_parser.cpp"
    break;

  case 418:
#line 2199 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7347 "ds_parser.cpp"
    break;

  case 419:
#line 2204 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7356 "ds_parser.cpp"
    break;

  case 420:
#line 2211 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7366 "ds_parser.cpp"
    break;

  case 421:
#line 2216 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7379 "ds_parser.cpp"
    break;

  case 422:
#line 2227 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7389 "ds_parser.cpp"
    break;

  case 423:
#line 2232 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7398 "ds_parser.cpp"
    break;

  case 424:
#line 2239 "ds_parser.ypp"
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7412 "ds_parser.cpp"
    break;

  case 425:
#line 2251 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 7418 "ds_parser.cpp"
    break;

  case 426:
#line 2252 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7424 "ds_parser.cpp"
    break;

  case 427:
#line 2256 "ds_parser.ypp"
    {
        auto pFor = make_shared<ExprFor>(tokAt((yylsp[-8])));
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
#line 7443 "ds_parser.cpp"
    break;

  case 428:
#line 2270 "ds_parser.ypp"
    {
        auto pFor = make_shared<ExprFor>(tokAt((yylsp[-8])));
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
#line 7462 "ds_parser.cpp"
    break;


#line 7466 "ds_parser.cpp"

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
#line 2286 "ds_parser.ypp"


void das_checkName(const string & name, const LineInfo &at) {
	if ( name.length()>=2 && name[0]=='_' && name[1]=='_' ) {
		g_Program->error("names starting with __ are reserved, " + name,"","",at,CompilationError::invalid_name);
	}
}

void das_yyerror(const string & error, const LineInfo & at, CompilationError cerr) {
    g_Program->error(error,"","",at,cerr);
}

void das_yyerror(const string & error, CompilationError cerr) {
    g_Program->error(error,"","",LineInfo(g_FileAccessStack.back(),
        yylloc.first_column,yylloc.first_line),cerr);
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
        argList.push_back(arg->shared_from_this());
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
    return LineInfo(g_FileAccessStack.back(), li.first_column,li.first_line);
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
                auto pVarType = make_shared<TypeDecl>(*pDecl->pTypeDecl);
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


