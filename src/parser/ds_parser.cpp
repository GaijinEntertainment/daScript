/* A Bison parser, made by GNU Bison 3.2.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.2.1"

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
#line 51 "ds_parser.ypp" /* yacc.c:338  */

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

#line 100 "ds_parser.cpp" /* yacc.c:338  */
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

/* In a future release of Bison, this section will be replaced
   by #include "ds_parser.hpp".  */
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
#line 2 "ds_parser.ypp" /* yacc.c:353  */

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

#line 190 "ds_parser.cpp" /* yacc.c:353  */

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
    DAS_GENERATOR = 341,
    DAS_YIELD = 342,
    ADDEQU = 343,
    SUBEQU = 344,
    DIVEQU = 345,
    MULEQU = 346,
    MODEQU = 347,
    ANDEQU = 348,
    OREQU = 349,
    XOREQU = 350,
    SHL = 351,
    SHR = 352,
    ADDADD = 353,
    SUBSUB = 354,
    LEEQU = 355,
    SHLEQU = 356,
    SHREQU = 357,
    GREQU = 358,
    EQUEQU = 359,
    NOTEQU = 360,
    RARROW = 361,
    LARROW = 362,
    QQ = 363,
    QDOT = 364,
    LPIPE = 365,
    LBPIPE = 366,
    LAPIPE = 367,
    RPIPE = 368,
    CLONEEQU = 369,
    ROTL = 370,
    ROTR = 371,
    ROTLEQU = 372,
    ROTREQU = 373,
    MAPTO = 374,
    BRABRAB = 375,
    BRACBRB = 376,
    CBRCBRB = 377,
    INTEGER = 378,
    LONG_INTEGER = 379,
    UNSIGNED_INTEGER = 380,
    UNSIGNED_LONG_INTEGER = 381,
    FLOAT = 382,
    DOUBLE = 383,
    NAME = 384,
    BEGIN_STRING = 385,
    STRING_CHARACTER = 386,
    END_STRING = 387,
    BEGIN_STRING_EXPR = 388,
    END_STRING_EXPR = 389,
    UNARY_MINUS = 390,
    UNARY_PLUS = 391,
    PRE_INC = 392,
    PRE_DEC = 393,
    POST_INC = 394,
    POST_DEC = 395,
    DEREF = 396,
    COLCOL = 397
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED

union DAS_YYSTYPE
{
#line 77 "ds_parser.ypp" /* yacc.c:353  */

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

#line 370 "ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   6347

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  170
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  105
/* YYNRULES -- Number of rules.  */
#define YYNRULES  414
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  750

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   397

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   152,     2,   169,   167,   148,   141,     2,
     160,   161,   146,   145,   135,   144,   157,   147,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   138,   164,
     142,   136,   143,   137,   168,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   158,     2,   159,   140,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   165,   139,   166,   151,     2,     2,     2,
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
     149,   150,   153,   154,   155,   156,   162,   163
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   378,   378,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   393,   405,   406,   410,   414,   418,   427,
     435,   451,   472,   476,   479,   485,   494,   515,   538,   539,
     543,   547,   548,   552,   555,   561,   567,   570,   576,   577,
     581,   582,   583,   592,   593,   597,   606,   618,   628,   637,
     638,   639,   640,   641,   642,   646,   651,   659,   660,   661,
     662,   663,   664,   665,   666,   672,   685,   701,   702,   706,
     713,   725,   729,   736,   737,   741,   742,   743,   747,   751,
     758,   762,   763,   764,   765,   766,   767,   768,   769,   770,
     771,   772,   773,   774,   775,   776,   777,   778,   779,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,   797,   801,   871,   874,
     884,   885,   886,   887,   888,   889,   890,   891,   892,   893,
     894,   895,   896,   897,   898,   899,   900,   901,   905,   909,
     915,   921,   932,   938,   939,   950,   954,   960,   963,   966,
     970,   976,   980,   984,   987,   990,   995,   998,  1006,  1009,
    1014,  1017,  1025,  1031,  1032,  1036,  1037,  1041,  1075,  1078,
    1083,  1091,  1095,  1100,  1106,  1110,  1115,  1124,  1127,  1133,
    1134,  1141,  1152,  1153,  1157,  1162,  1214,  1215,  1216,  1217,
    1218,  1219,  1223,  1224,  1225,  1226,  1227,  1228,  1229,  1230,
    1231,  1232,  1233,  1234,  1235,  1236,  1237,  1238,  1242,  1243,
    1247,  1258,  1263,  1273,  1277,  1284,  1298,  1299,  1300,  1301,
    1302,  1303,  1304,  1305,  1306,  1307,  1308,  1309,  1310,  1311,
    1312,  1313,  1314,  1315,  1316,  1317,  1318,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,  1326,  1327,  1328,  1329,  1330,  1331,
    1332,  1333,  1334,  1335,  1336,  1340,  1344,  1347,  1350,  1351,
    1352,  1353,  1358,  1363,  1364,  1367,  1370,  1371,  1372,  1373,
    1374,  1375,  1376,  1386,  1396,  1430,  1431,  1435,  1436,  1440,
    1448,  1451,  1458,  1459,  1463,  1464,  1468,  1471,  1480,  1481,
    1485,  1486,  1490,  1495,  1498,  1502,  1508,  1517,  1518,  1519,
    1523,  1524,  1528,  1531,  1536,  1541,  1549,  1560,  1563,  1570,
    1571,  1575,  1605,  1608,  1617,  1629,  1641,  1652,  1667,  1668,
    1672,  1711,  1777,  1784,  1793,  1794,  1795,  1796,  1797,  1798,
    1799,  1800,  1801,  1802,  1803,  1804,  1805,  1806,  1807,  1808,
    1809,  1810,  1811,  1812,  1813,  1814,  1815,  1816,  1820,  1821,
    1822,  1823,  1824,  1825,  1829,  1840,  1844,  1854,  1855,  1856,
    1857,  1871,  1877,  1881,  1886,  1891,  1896,  1903,  1908,  1912,
    1916,  1921,  1926,  1933,  1938,  1944,  1949,  1953,  1958,  1967,
    1971,  1976,  1985,  1989,  1994,  2003,  2012,  2013,  2014,  2015,
    2019,  2026,  2035,  2040,  2047,  2052,  2058,  2064,  2071,  2082,
    2085,  2099,  2108,  2109,  2113,  2118,  2125,  2130,  2141,  2146,
    2153,  2165,  2166,  2170,  2184
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
  "\"int16\"", "\"uint16\"", "\"tuple\"", "\"generator\"", "\"yield\"",
  "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"",
  "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"",
  "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"",
  "\"?.\"", "\"<|\"", "\" <|\"", "\"@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"",
  "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"[[\"", "\"[{\"",
  "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"start of the string\"", "STRING_CHARACTER", "\"end of the string\"",
  "\"{\"", "\"}\"", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'", "'<'",
  "'>'", "'-'", "'+'", "'*'", "'/'", "'%'", "UNARY_MINUS", "UNARY_PLUS",
  "'~'", "'!'", "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC", "'.'", "'['",
  "']'", "'('", "')'", "DEREF", "COLCOL", "';'", "'{'", "'}'", "'$'",
  "'@'", "'#'", "$accept", "program", "module_declaration",
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
  "copy_or_move", "variable_declaration", "copy_or_move_or_clone",
  "optional_ref", "let_variable_declaration",
  "global_variable_declaration_list", "optional_shared", "global_let",
  "enum_list", "alias_declaration", "enum_declaration",
  "optional_structure_parent", "structure_name", "structure_declaration",
  "variable_name_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "type_declaration", "make_decl",
  "make_struct_fields", "make_struct_dim", "make_struct_decl",
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
     385,   386,   387,   388,   389,    44,    61,    63,    58,   124,
      94,    38,    60,    62,    45,    43,    42,    47,    37,   390,
     391,   126,    33,   392,   393,   394,   395,    46,    91,    93,
      40,    41,   396,   397,    59,   123,   125,    36,    64,    35
};
# endif

#define YYPACT_NINF -454

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-454)))

#define YYTABLE_NINF -193

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -454,    14,  -454,  -454,   -20,    -8,    -4,    18,    28,  -454,
      48,   -21,  -454,  -454,  -454,  -454,  -454,   227,  -454,   137,
    -454,  -454,  -454,  -454,    74,  -454,    88,   103,   134,  -454,
    -454,    47,  -454,   -34,   139,  -454,  -454,   113,   171,   146,
    -454,    23,  -454,   178,   110,  -454,   153,    41,   -20,   192,
      -8,   193,  -454,   203,   206,  -454,   255,  -454,  5296,   221,
    -454,   -20,   -21,  -454,   219,   204,  6195,   337,   338,  -454,
     217,  -454,  -454,  -454,  -454,  -454,  -454,   244,    99,  -454,
    -454,  -454,  -454,   326,  -454,  -454,  -454,  -454,  -454,  -454,
    -454,  -454,   214,  -100,   238,   239,   240,  -454,  -454,  -454,
     223,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,
    -454,  -454,  -454,  -454,  -454,   242,  -454,  -454,  -454,   243,
     246,  -454,  -454,  -454,  -454,   247,  -454,  -454,  -454,  -454,
    3236,  -454,   -15,  -454,    -1,  -454,  -454,  -454,  -454,  -454,
    -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,
    -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,
    -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,
    -454,  -454,  -454,     0,   248,   -98,  -454,   -71,  -454,  -454,
    -454,  -454,  -454,  -454,  -454,    55,  -454,  -454,  -102,  -454,
    5296,  5296,  5296,   261,  5223,  5223,  5223,  5369,  -454,   -14,
    -454,  -454,  -454,  -454,  1941,  -454,  -454,  -454,   -47,   262,
    -454,  -454,  -454,   104,  -454,   100,  5296,   228,  -454,  -454,
     150,  -454,  -454,    99,  -454,   -66,  3838,  -454,   638,  1056,
    1465,   233,   248,  1621,   248,  1670,   248,  1755,   -37,  -454,
    -107,  3236,   254,  -454,   237,  -454,  -454,  -454,  5150,   241,
    -454,   245,   256,   257,   249,   258,   264,  3838,  3838,    12,
     503,  3838,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  3838,
    3838,  3838,  3838,  3838,  -454,  3838,  -454,  -454,   229,  -454,
    -454,   -24,  -454,  -454,  -454,   250,  -454,  -454,  -454,  -454,
    -454,  5365,   251,  -454,  -454,  -454,  -454,  -454,   -20,  -454,
     361,   252,  -454,  -454,   -27,  -454,   275,  -454,  5296,  2082,
    3236,  -454,  5296,  -454,   106,  -454,  -454,  4516,  -454,  5296,
    -454,  -454,   267,  -454,   270,  -454,   271,  -454,  5296,  -454,
    5369,  -454,  -454,  1889,  -454,   286,  3838,  5296,  5296,  3838,
    5296,  5296,   154,   154,   262,  1108,   262,  1249,  5658,  -454,
     179,   -93,   154,   154,  -454,   154,   154,  4648,   784,   -99,
    -454,  1517,   217,   400,  3838,  3838,  -454,  -454,  3838,  3838,
    3838,  3838,   289,  3838,   291,  3838,  3838,  3838,  3838,  3838,
    3838,  3838,  3838,  3838,  3838,  3838,  3838,  3838,  3838,  3838,
     292,  3838,  -454,  2216,    65,  -454,   262,  -454,  -454,  -454,
     603,   166,  -454,   312,  5509,   376,  -454,  -454,  -454,  2082,
    -454,  2079,  -454,  -454,  -454,  3236,  -454,  2350,  1673,  4715,
    2164,  2298,  4781,  2432,  2566,    -5,    62,   265,  2484,  5658,
     290,   115,   294,  -454,  -454,   130,    -3,  2618,   185,   200,
    3838,  3838,   260,  -454,  3838,   301,  -454,  -454,  3838,  -454,
    -454,   262,  1383,  -454,   228,  3838,  3838,  -454,   272,   314,
    2752,  1807,  -454,   403,   276,   279,  3838,  -454,  -454,  -454,
    -454,  -454,  -454,   280,   281,   282,  -454,  -454,  -454,   417,
    -454,   -61,  -454,  5602,  5223,  -454,   310,  -454,    79,  5826,
     248,  5296,   215,   215,  6124,  6124,  6103,  6103,   297,     8,
    -454,    54,    54,   215,   215,  5714,  5882,  5938,  5994,  6124,
    6124,   607,   607,     8,     8,     8,  -454,  5437,  -454,    97,
    -454,  -454,  3838,  -454,  -454,   166,  3838,  3838,  3838,  3838,
    3838,  3838,  3838,  3838,  3838,  3838,  3838,  3838,  3838,  3838,
    3838,  -454,  2082,  -454,  3993,  -454,  -454,   111,  5296,   346,
    4847,  -454,  3838,  3838,  -454,  3838,   316,  3838,  3838,  -454,
     310,   348,   319,   310,  3838,   320,  3838,  3838,   310,   322,
     323,  5826,  -454,  -454,  5770,  3911,    10,  2082,  -454,  -454,
    4085,   473,  5826,  3911,  -454,   349,   363,  5826,  2082,  -454,
    4177,   324,  -454,  -454,  3911,  -454,  -454,  -454,   450,   262,
    -454,  2886,  3020,   248,  2700,    15,   119,  3838,  -454,   -67,
    3236,  3154,  3838,  -454,  -454,  5826,  -454,  5826,  5826,  5826,
    5826,  5826,  5826,  5826,  5826,  5826,  5826,  5826,  5826,  5826,
    5826,  5826,  -454,  4269,  -454,  -454,  2834,  -106,  -454,  6124,
    6124,  6124,  3288,    80,  5826,   183,    15,  -454,   290,  5826,
    -454,  -454,    86,   201,  -454,  -454,  -454,  -454,  3838,  -454,
    4361,  -454,   228,  -454,  -454,  -454,  -454,  4453,  -454,  -454,
     283,  -454,  -454,  3838,  3838,   350,    -1,   334,  5826,  3422,
    -454,  -454,  -454,   121,  5826,  -454,  -454,  3563,   367,  -454,
    4913,  3838,   339,  3838,  3838,   340,    95,  -454,  -454,  -454,
     952,   228,  -454,  -454,  3838,  -454,  6050,  6050,    -1,  -454,
    -454,  3838,  5826,  -454,  5296,  4979,   354,  -454,  4585,  -454,
    5826,  4585,  -454,   228,  -454,  -454,  3911,  -454,  5826,  2968,
    -454,  3704,   452,   343,   341,  -454,   283,  -454,  5296,  5045,
    3838,   344,   345,  -454,  3102,  -454,  5826,  -454,  -454,  -454
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    73,     1,   165,     0,     0,     0,     0,     0,   166,
       0,     0,    12,     3,    10,     9,     8,     0,     7,   309,
       6,    11,     5,     4,    63,    65,    21,    33,    30,    31,
      23,    28,    22,     0,     0,    13,    68,   143,     0,    69,
      71,     0,    67,     0,     0,   310,     0,     0,     0,     0,
       0,     0,    29,     0,     0,    26,     0,   312,     0,     0,
     145,     0,     0,    74,   318,     0,     0,     0,     0,    80,
      75,   307,    61,    62,    59,    60,    58,     0,     0,    57,
      66,    34,    32,    28,    25,    24,   348,   351,   349,   352,
     350,   353,     0,     0,     0,     0,     0,   324,   344,   325,
     355,   326,   330,   331,   332,   333,   337,   338,   339,   340,
     341,   342,   343,   345,   346,   376,   329,   336,   347,   379,
     382,   327,   334,   328,   335,     0,   354,   357,   359,   358,
     315,   144,     0,    72,     0,   320,   280,    83,    84,    86,
      85,    87,    88,    89,    90,   109,   110,   107,   108,   100,
     111,   112,   101,    98,    99,   113,   114,   115,   116,   103,
     104,   102,    96,    97,    92,    91,    93,    94,    95,    82,
      81,   105,   106,     0,    78,     0,    14,     0,    53,    54,
      51,    52,    50,    49,    55,     0,    27,   312,     0,   316,
       0,     0,     0,     0,    75,    75,    75,     0,   363,     0,
     369,   372,   371,   365,     0,   368,    70,   319,   275,     0,
     322,    76,   284,     0,   282,   292,     0,     0,   311,   308,
     300,    15,    16,     0,    64,     0,     0,   313,     0,     0,
       0,     0,    78,     0,    78,     0,    78,     0,   143,   288,
       0,   286,   364,   367,     0,   370,   221,   222,     0,     0,
     216,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   186,   188,   187,   189,   190,   191,    17,     0,
       0,     0,     0,     0,   361,     0,   138,   182,   183,   219,
     184,   217,   268,   267,   266,    73,   271,   218,   270,   269,
     253,     0,     0,   220,   386,   387,   388,   389,     0,   321,
     277,     0,   283,    77,     0,   291,     0,   290,     0,     0,
      79,   117,     0,   301,     0,    56,   317,     0,   373,     0,
     375,   356,     0,   377,     0,   380,     0,   383,     0,   385,
       0,   366,   362,   147,   150,     0,     0,     0,     0,     0,
       0,     0,   245,   246,     0,     0,     0,     0,     0,   408,
       0,     0,   226,   225,   258,   224,   223,     0,     0,     0,
     274,     0,    75,     0,     0,     0,   247,   248,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   360,     0,     0,   278,     0,   281,   285,   323,
     293,     0,   296,     0,   295,     0,   298,   299,   297,     0,
     314,     0,   378,   381,   384,   287,   289,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   143,     0,     0,   399,
     392,     0,   402,   403,   404,     0,     0,     0,     0,     0,
       0,     0,     0,    20,     0,    18,   249,   140,     0,    43,
      44,     0,     0,   151,     0,     0,     0,   152,     0,     0,
       0,     0,   120,   118,     0,     0,     0,   133,   128,   126,
     127,   139,   121,     0,     0,     0,   131,   132,   134,   163,
     125,     0,   122,   192,    75,   213,     0,   254,     0,   177,
      78,     0,   227,   228,   240,   241,   238,   239,     0,   263,
     252,   272,   273,   229,   230,     0,   243,   244,   242,   236,
     237,   232,   231,   233,   234,   235,   251,     0,   256,     0,
     276,   279,     0,   183,   142,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   302,     0,   306,   192,   374,   148,     0,     0,     0,
       0,   259,     0,     0,   260,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   401,   409,   410,     0,     0,     0,     0,   153,   156,
     192,     0,   146,     0,   137,     0,     0,    37,     0,   160,
     192,     0,   135,   136,     0,   124,   129,   130,     0,     0,
     123,     0,     0,    78,     0,     0,     0,     0,   255,     0,
     265,     0,     0,   250,   257,   294,   141,   199,   200,   202,
     201,   203,   196,   197,   198,   204,   205,   194,   195,   206,
     207,   193,   304,   192,   305,   149,     0,     0,   171,   168,
     169,   170,     0,     0,   390,     0,     0,   394,   393,   400,
     406,   405,     0,     0,   397,   407,    19,    47,     0,   157,
     192,   154,     0,    48,    35,    36,   161,   192,   158,   138,
      40,   164,   167,     0,     0,     0,     0,     0,   178,     0,
     179,   185,   211,     0,   264,   303,   174,     0,     0,   261,
       0,     0,     0,     0,     0,     0,     0,   155,   162,   159,
       0,     0,    38,    39,     0,    45,   209,   208,     0,   214,
     210,     0,   180,   212,     0,     0,     0,   262,   411,   396,
     391,   411,   398,     0,   119,    41,     0,   215,   181,     0,
     172,     0,     0,     0,     0,    46,    40,   175,     0,     0,
       0,     0,     0,    42,     0,   173,   412,   413,   414,   176
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -454,  -454,  -454,   157,   458,  -454,  -454,  -454,  -454,  -454,
    -454,   426,  -454,  -454,   460,  -454,  -454,  -454,  -221,  -454,
    -454,  -454,  -454,  -454,   295,  -454,   474,   -53,  -454,   459,
    -454,   263,  -174,  -229,  -454,  -454,  -207,  -454,  -144,  -285,
     -11,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,   527,
    -454,  -454,  -454,  -368,  -454,  -454,  -369,  -454,  -354,  -454,
    -454,  -454,  -454,    83,  -454,  -454,  -454,  -454,   225,  -454,
     211,  -454,  -213,  -200,   125,  -454,   -63,  -454,  -454,  -454,
     355,  -454,  -454,  -454,  -454,  -454,  -149,   -57,  -454,  -454,
    -454,   -42,   285,  -453,  -341,  -454,  -454,  -249,   -16,   199,
    -454,  -454,  -454,  -170,  -454
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   177,   183,   351,   279,    14,    15,    31,
      32,    55,    16,    28,    29,   464,   465,   704,   705,   466,
     467,   468,   469,   470,   184,   185,    25,    26,    39,    40,
      41,    17,   174,   217,    70,    18,   280,   471,   358,   472,
     281,   473,   282,   474,   475,   476,   477,   478,   599,   479,
     480,   283,   284,   488,   681,   285,   286,   287,   403,   482,
     288,   289,   290,   489,   300,   396,   301,   208,   212,   213,
     239,   240,   558,   214,   409,   314,   219,   175,    46,    20,
      93,    21,    22,   135,    65,    23,   215,   292,    92,   128,
     129,   241,   293,   430,   431,   294,   432,   433,   434,   435,
     295,   350,   296,   733,   297
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   127,   309,   322,   481,   324,   438,   326,   132,   302,
     311,    36,   349,   557,     2,   567,   130,   242,   209,     3,
     232,   234,   236,   344,   402,   519,   220,   363,   658,   188,
      37,   210,   524,   606,   226,    94,   329,   687,   176,   443,
     444,    95,     4,   484,     5,   209,     6,   126,     7,   547,
     525,    42,   679,     8,    72,    73,    96,   330,   688,     9,
     221,   222,   227,   188,    38,    10,   189,    51,   218,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   276,    52,
     316,   328,   210,   600,    56,   360,   366,   367,    37,    24,
     648,   298,   178,   179,   372,    27,   373,   374,   375,   299,
      48,   376,   305,   207,   543,    30,    59,   243,    37,   210,
     306,    57,   306,   127,   127,   127,   361,   127,   127,   127,
     127,    37,    38,    66,   244,   306,   206,    33,   228,   229,
     230,   307,   233,   235,   237,   245,   616,    34,    62,   127,
     372,   211,    38,   374,    74,   390,   391,   579,    75,   305,
      76,    77,    11,   363,   310,    38,   589,    35,    12,   126,
     126,   126,    63,   126,   126,   126,   126,   522,   490,   643,
     223,   127,   572,    45,    53,   425,   521,   436,   307,   652,
      48,    78,   127,   127,    54,   126,   333,   305,    67,    68,
      47,   390,   391,   406,   607,   607,   224,   345,   347,   645,
     407,   607,   180,    48,   520,    59,   181,   653,   182,    77,
     607,    43,   607,    44,   363,   306,   307,   126,   308,    69,
     608,    49,   408,   683,   691,   394,   607,   581,   126,   126,
     694,   127,   366,   367,   561,   127,   607,   632,   614,   723,
     372,   609,   127,   374,   375,   303,   400,   376,   304,    50,
     405,   127,   635,   127,   562,    58,    59,   411,   677,   563,
     127,   127,   713,   127,   127,   306,   415,   291,   312,   565,
     696,   313,   659,   701,   566,   420,   421,   126,   423,   424,
      60,   126,   576,   666,   702,   703,    61,    64,   126,   317,
     603,   390,   391,   366,   367,    81,    86,   126,    71,   126,
      87,   372,    83,   373,   374,   375,   126,   126,   376,   126,
     126,   276,    84,   277,   523,    85,    88,    89,    90,    91,
     342,   343,   692,   441,   348,   442,   481,   563,   485,   563,
     131,   569,   352,   353,   354,   355,   356,   134,   357,   385,
     386,   387,   388,   389,   566,   563,   570,   695,   657,   136,
     171,   172,   390,   391,   675,   176,   663,   173,    52,   187,
     190,   191,   192,   193,   194,   195,   216,   670,   196,   197,
     231,   210,   404,   276,   321,   331,   332,   359,   337,   338,
     340,   335,   680,   395,   399,   336,   341,   198,    11,   339,
     412,   393,   199,   413,   414,   418,   397,   491,   498,   419,
     500,   516,   422,   525,   559,   561,   573,   127,   429,   564,
     429,   200,   221,   693,   127,   598,   584,   585,   591,   605,
     592,   483,   604,   593,   595,   596,   597,   492,   493,   610,
     220,   494,   495,   496,   497,   698,   499,   611,   501,   502,
     503,   504,   505,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   126,   517,   637,   642,   646,   647,   650,
     126,   654,   655,   406,   201,   662,   665,   664,   671,   669,
     407,   127,   544,   708,   725,   710,   716,   731,   719,   722,
     740,   550,   741,   747,   748,    79,   636,   742,   445,   186,
      82,   357,   408,   202,   346,   743,   735,   203,   315,   736,
     357,   133,    80,   571,   348,   700,    94,   574,    19,   398,
     542,   575,    95,   334,   204,   580,   672,   126,   582,   583,
     541,   416,   225,   587,   590,   205,   439,    96,   362,   594,
     651,   734,     0,     0,     0,     0,     0,     0,     0,     0,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   615,     0,     0,     0,   617,
     618,   619,   620,   621,   622,   623,   624,   625,   626,   627,
     628,   629,   630,   631,     0,   633,   363,     0,     0,     0,
       0,     0,    37,     0,   198,   639,   640,     0,   641,   199,
       0,   644,     0,     0,     0,     0,     0,   649,     0,   429,
       0,     0,     0,     0,     0,     0,     0,   127,   200,     0,
     660,     0,     0,     0,     0,   709,    38,     0,     0,   198,
       0,   667,   729,     0,   199,     0,     0,     0,     0,     0,
       0,   127,     0,     0,   627,   631,     0,     0,     0,     0,
     678,     0,     0,   200,     0,   684,   744,   727,     0,     0,
       0,     0,     0,   126,     0,   366,   367,     0,     0,     0,
     305,   201,     0,   372,     0,   373,   374,   375,     0,     0,
     376,     0,     0,     0,     0,   690,     0,   126,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   307,
     202,     0,     0,     0,   203,     0,   201,     0,     0,     0,
       0,     0,     0,   387,   388,   389,   706,   707,     0,     0,
       0,   204,   712,     0,   390,   391,     0,     0,     0,     0,
     715,     0,   205,     0,   718,   202,   720,   721,     0,   203,
       0,   318,     0,   483,     0,   447,     0,   726,     0,     3,
       0,   448,   449,   450,   728,   451,   204,   246,   247,   248,
     249,     0,     0,     0,     0,     0,     0,   205,   452,   250,
     453,   454,     0,     0,   739,     0,     0,     0,     0,     0,
     455,   251,     0,   746,   456,   252,     0,   253,     0,     9,
     254,   457,     0,   458,   255,     0,     0,   459,   460,     0,
       0,    97,    98,    99,     0,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,     0,
     116,   117,   118,     0,     0,   121,   122,   123,   124,     0,
     256,   461,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   257,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   401,     0,     0,     0,
       0,     0,     0,     0,   259,   260,   261,   262,   263,   264,
     265,   266,   267,    37,   268,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   269,   270,
     271,     0,     0,     0,     0,   272,   273,     0,     0,     0,
       0,     0,     0,     0,   275,     0,     0,    38,   462,   276,
     463,   277,   278,   447,     0,     0,     0,     3,     0,   448,
     449,   450,     0,   451,     0,   246,   247,   248,   249,     0,
       0,     0,     0,     0,     0,     0,   452,   250,   453,   454,
       0,     0,     0,     0,     0,     0,     0,     0,   455,   251,
       0,     0,   456,   252,     0,   253,     0,     9,   254,   457,
       0,   458,   255,     0,     0,   459,   460,     0,     0,    97,
      98,    99,     0,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,     0,   116,   117,
     118,     0,     0,   121,   122,   123,   124,     0,   256,   461,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     257,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   401,     0,     0,     0,     0,     0,
       0,     0,   259,   260,   261,   262,   263,   264,   265,   266,
     267,    37,   268,     0,     0,     0,     0,   198,     0,     0,
       0,     0,   199,     0,     0,     0,   269,   270,   271,     0,
       0,     0,     0,   272,   273,     0,     0,     0,     0,     0,
       0,   200,   275,     0,     0,    38,   462,   276,   724,   277,
     278,   246,   247,   248,   249,     0,     0,     0,     0,     0,
       0,     0,     0,   250,     0,     0,     0,     0,     0,   198,
       0,     0,     0,     0,   199,   251,     0,     0,     0,   252,
       0,   253,     0,     0,   254,     0,     0,     0,   255,     0,
       0,     0,     0,   200,   201,    97,    98,    99,     0,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,     0,   116,   117,   118,     0,     0,   121,
     122,   123,   124,   202,   256,     0,     0,   203,     0,     0,
       0,     0,     0,     0,     0,     0,   257,   258,     0,     0,
       0,     0,     0,     0,   204,     0,   201,     0,     0,     0,
     319,     0,     0,     0,     0,   205,     0,     0,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   426,   268,     0,
       0,     0,     0,     0,     0,   202,     0,     0,     0,   203,
       0,     0,   269,   270,   271,     0,     0,     0,     0,   272,
     273,     0,   246,   247,   248,   249,   204,   427,   428,     0,
       0,    38,     0,   276,   250,   277,   278,   205,     0,     0,
     198,     0,     0,     0,     0,   199,   251,     0,     0,     0,
     252,     0,   253,     0,     0,   254,     0,     0,     0,   255,
       0,     0,     0,     0,   200,     0,    97,    98,    99,     0,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,     0,   116,   117,   118,     0,     0,
     121,   122,   123,   124,     0,   256,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   257,   258,     0,
       0,     0,     0,     0,     0,     0,     0,   201,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   426,   268,
       0,     0,     0,     0,     0,     0,   202,     0,     0,     0,
     203,     0,     0,   269,   270,   271,   246,   247,   248,   249,
     272,   273,     0,     0,     0,     0,     0,   204,   250,   437,
       0,     0,    38,     0,   276,     0,   277,   278,   205,     0,
     251,     0,     0,     0,   252,     0,   253,     0,     0,   254,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
      97,    98,    99,     0,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,     0,   116,
     117,   118,     0,     0,   121,   122,   123,   124,     0,   256,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   257,   258,     0,     0,     0,     0,     0,     0,     0,
     577,     0,     0,     0,     0,   401,   198,     0,     0,     0,
       0,   199,     0,   259,   260,   261,   262,   263,   264,   265,
     266,   267,    37,   268,     0,     0,     0,     0,     0,     0,
     200,     0,     0,     0,     0,     0,     0,   269,   270,   271,
     246,   247,   248,   249,   272,   273,     0,     0,     0,     0,
       0,     0,   250,   275,     0,     0,    38,   578,   276,     0,
     277,   278,     0,     0,   251,     0,     0,     0,   252,     0,
     253,     0,     0,   254,     0,     0,     0,   255,     0,     0,
       0,     0,     0,   201,    97,    98,    99,     0,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,     0,   116,   117,   118,     0,     0,   121,   122,
     123,   124,   202,   256,     0,     0,   203,     0,   320,     0,
       0,     0,     0,     0,     0,   257,   258,     0,     0,     0,
       0,     0,     0,   204,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   205,     0,     0,   259,   260,   261,
     262,   263,   264,   265,   266,   267,    37,   268,     0,     0,
       0,     0,   198,     0,     0,     0,     0,   199,     0,     0,
       0,   269,   270,   271,     0,     0,     0,     0,   272,   273,
       0,     0,     0,     0,     0,   486,   200,   275,   487,     0,
      38,     0,   276,     0,   277,   278,   246,   247,   248,   249,
     548,     0,     0,     0,     0,     0,     0,     0,   250,     0,
       0,   198,     0,     0,     0,     0,   199,     0,     0,     0,
     251,     0,     0,     0,   252,     0,   253,     0,     0,   254,
       0,     0,     0,   255,     0,   200,     0,     0,     0,   201,
      97,    98,    99,     0,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,     0,   116,
     117,   118,     0,     0,   121,   122,   123,   124,   202,   256,
       0,     0,   203,     0,   323,     0,     0,     0,     0,     0,
       0,   257,   258,     0,     0,     0,     0,     0,   201,   204,
       0,     0,     0,     0,     0,     0,   198,     0,     0,     0,
     205,   199,     0,   259,   260,   261,   262,   263,   264,   265,
     266,   267,    37,   268,     0,     0,     0,   202,     0,     0,
     200,   203,     0,   325,     0,   549,     0,   269,   270,   271,
     246,   247,   248,   249,   272,   273,     0,     0,   204,     0,
       0,     0,   250,   275,     0,     0,    38,     0,   276,   205,
     277,   278,     0,     0,   251,     0,     0,     0,   252,     0,
     253,     0,     0,   254,     0,     0,     0,   255,     0,     0,
       0,     0,     0,   201,    97,    98,    99,     0,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,     0,   116,   117,   118,     0,     0,   121,   122,
     123,   124,   202,   256,     0,     0,   203,     0,   327,     0,
       0,     0,     0,     0,     0,   257,   258,     0,     0,     0,
       0,     0,     0,   204,   588,     0,     0,     0,     0,   401,
     198,     0,     0,     0,   205,   199,     0,   259,   260,   261,
     262,   263,   264,   265,   266,   267,    37,   268,     0,     0,
       0,     0,     0,     0,   200,     0,     0,     0,     0,     0,
       0,   269,   270,   271,   246,   247,   248,   249,   272,   273,
       0,     0,     0,     0,     0,     0,   250,   275,     0,     0,
      38,     0,   276,     0,   277,   278,     0,     0,   251,     0,
       0,     0,   252,     0,   253,     0,     0,   254,     0,     0,
       0,   255,     0,     0,     0,     0,     0,   201,    97,    98,
      99,     0,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,     0,   116,   117,   118,
       0,     0,   121,   122,   123,   124,   202,   256,     0,     0,
     203,     0,     0,     0,     0,     0,     0,     0,     0,   257,
     258,     0,     0,     0,     0,     0,     0,   204,     0,   417,
       0,     0,     0,     0,     0,     0,     0,     0,   205,     0,
       0,   259,   260,   261,   262,   263,   264,   265,   266,   267,
      37,   268,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   269,   270,   271,     0,     0,
       0,     0,   272,   273,     0,   246,   247,   248,   249,     0,
     274,   275,     0,     0,    38,     0,   276,   250,   277,   278,
     198,     0,     0,     0,     0,   199,     0,     0,     0,   251,
       0,     0,     0,   252,     0,   253,     0,     0,   254,     0,
       0,     0,   255,     0,   200,     0,     0,     0,     0,    97,
      98,    99,     0,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,     0,   116,   117,
     118,     0,     0,   121,   122,   123,   124,     0,   256,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     257,   258,     0,     0,     0,     0,     0,   201,     0,     0,
       0,     0,     0,     0,   401,   198,     0,     0,     0,     0,
     199,     0,   259,   260,   261,   262,   263,   264,   265,   266,
     267,    37,   268,     0,     0,     0,   202,     0,     0,   200,
     203,     0,   545,     0,     0,     0,   269,   270,   271,   246,
     247,   248,   249,   272,   273,     0,     0,   204,     0,     0,
       0,   250,   275,     0,     0,    38,     0,   276,   205,   277,
     278,     0,     0,   251,     0,     0,     0,   252,     0,   253,
       0,     0,   254,     0,     0,     0,   255,     0,     0,     0,
       0,     0,   201,    97,    98,    99,     0,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,     0,   116,   117,   118,     0,     0,   121,   122,   123,
     124,   202,   256,     0,     0,   203,     0,   552,     0,     0,
       0,     0,     0,     0,   257,   258,     0,     0,     0,     0,
       0,     0,   204,     0,     0,     0,     0,     0,     0,   198,
       0,     0,     0,   205,   199,     0,   259,   260,   261,   262,
     263,   264,   265,   266,   267,    37,   268,     0,     0,     0,
       0,     0,     0,   200,     0,     0,     0,     0,     0,     0,
     269,   270,   271,   246,   247,   248,   249,   272,   273,     0,
       0,     0,     0,     0,     0,   250,   275,   518,     0,    38,
       0,   276,     0,   277,   278,     0,     0,   251,     0,     0,
       0,   252,     0,   253,     0,     0,   254,     0,     0,     0,
     255,     0,     0,     0,     0,     0,   201,    97,    98,    99,
       0,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,   116,   117,   118,     0,
       0,   121,   122,   123,   124,   202,   256,     0,     0,   203,
       0,   553,     0,     0,     0,     0,     0,     0,   257,   258,
       0,     0,     0,     0,     0,     0,   204,     0,     0,     0,
       0,     0,     0,   198,     0,     0,     0,   205,   199,     0,
     259,   260,   261,   262,   263,   264,   265,   266,   267,    37,
     268,     0,     0,     0,     0,     0,     0,   200,     0,     0,
       0,     0,     0,     0,   269,   270,   271,   246,   247,   248,
     249,   272,   273,     0,     0,     0,     0,     0,     0,   250,
     275,   546,     0,    38,     0,   276,     0,   277,   278,     0,
       0,   251,     0,     0,     0,   252,     0,   253,     0,     0,
     254,     0,     0,     0,   255,     0,     0,     0,     0,     0,
     201,    97,    98,    99,     0,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,     0,
     116,   117,   118,     0,     0,   121,   122,   123,   124,   202,
     256,     0,     0,   203,     0,   555,     0,     0,     0,     0,
       0,     0,   257,   258,     0,     0,     0,     0,     0,     0,
     204,     0,     0,     0,     0,     0,     0,   198,     0,     0,
       0,   205,   199,     0,   259,   260,   261,   262,   263,   264,
     265,   266,   267,    37,   268,     0,     0,     0,     0,     0,
       0,   200,     0,     0,     0,     0,     0,     0,   269,   270,
     271,   246,   247,   248,   249,   272,   273,     0,     0,     0,
       0,     0,     0,   250,   275,   560,     0,    38,     0,   276,
       0,   277,   278,     0,     0,   251,     0,     0,     0,   252,
       0,   253,     0,     0,   254,     0,     0,     0,   255,     0,
       0,     0,     0,     0,   201,    97,    98,    99,     0,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,     0,   116,   117,   118,     0,     0,   121,
     122,   123,   124,   202,   256,     0,     0,   203,     0,   556,
       0,     0,     0,     0,     0,     0,   257,   258,     0,     0,
       0,     0,     0,     0,   204,     0,     0,     0,     0,     0,
       0,   198,     0,     0,     0,   205,   199,     0,   259,   260,
     261,   262,   263,   264,   265,   266,   267,    37,   268,     0,
       0,     0,     0,     0,     0,   200,     0,     0,     0,     0,
       0,     0,   269,   270,   271,   246,   247,   248,   249,   272,
     273,     0,     0,     0,     0,     0,     0,   250,   275,   568,
       0,    38,     0,   276,     0,   277,   278,     0,     0,   251,
       0,     0,     0,   252,     0,   253,     0,     0,   254,     0,
       0,     0,   255,     0,     0,   586,     0,     0,   201,    97,
      98,    99,     0,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,     0,   116,   117,
     118,     0,     0,   121,   122,   123,   124,   202,   256,     0,
       0,   203,     0,   676,     0,     0,     0,     0,     0,     0,
     257,   258,     0,     0,     0,     0,     0,     0,   204,     0,
       0,     0,     0,     0,     0,   198,     0,     0,     0,   205,
     199,     0,   259,   260,   261,   262,   263,   264,   265,   266,
     267,    37,   268,     0,     0,     0,     0,     0,     0,   200,
       0,     0,     0,     0,     0,     0,   269,   270,   271,   246,
     247,   248,   249,   272,   273,     0,     0,     0,     0,     0,
       0,   250,   275,     0,     0,    38,     0,   276,     0,   277,
     278,     0,     0,   251,     0,     0,     0,   252,     0,   253,
       0,     0,   254,     0,     0,     0,   255,     0,     0,     0,
       0,     0,   201,    97,    98,    99,     0,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,     0,   116,   117,   118,     0,     0,   121,   122,   123,
     124,   202,   256,     0,     0,   203,     0,     0,     0,     0,
       0,     0,     0,     0,   257,   258,     0,     0,     0,     0,
       0,     0,   204,     0,     0,   686,     0,     0,   673,   198,
       0,     0,     0,   205,   199,     0,   259,   260,   261,   262,
     263,   264,   265,   266,   267,    37,   268,     0,     0,     0,
       0,     0,     0,   200,     0,     0,     0,     0,     0,     0,
     269,   270,   271,   246,   247,   248,   249,   272,   273,     0,
       0,     0,     0,     0,     0,   250,   275,     0,     0,    38,
       0,   276,     0,   277,   278,     0,     0,   251,     0,     0,
       0,   252,     0,   253,     0,     0,   254,     0,     0,     0,
     255,     0,     0,     0,     0,     0,   201,    97,    98,    99,
       0,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,   116,   117,   118,     0,
       0,   121,   122,   123,   124,   202,   256,     0,     0,   203,
       0,     0,     0,     0,     0,     0,     0,     0,   257,   258,
       0,     0,     0,     0,     0,     0,   204,     0,     0,   737,
       0,     0,   674,   198,     0,     0,     0,   205,   199,     0,
     259,   260,   261,   262,   263,   264,   265,   266,   267,    37,
     268,     0,     0,     0,     0,     0,     0,   200,     0,     0,
       0,     0,     0,     0,   269,   270,   271,   246,   247,   248,
     249,   272,   273,     0,     0,     0,     0,     0,     0,   250,
     275,     0,     0,    38,     0,   276,     0,   277,   278,     0,
       0,   251,     0,     0,     0,   252,     0,   253,     0,     0,
     254,     0,     0,     0,   255,     0,     0,     0,     0,     0,
     201,    97,    98,    99,     0,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,     0,
     116,   117,   118,     0,     0,   121,   122,   123,   124,   202,
     256,     0,     0,   203,     0,     0,     0,     0,     0,     0,
       0,     0,   257,   258,     0,     0,     0,     0,     0,     0,
     204,     0,     0,   749,     0,     0,     0,   198,     0,     0,
       0,   205,   199,     0,   259,   260,   261,   262,   263,   264,
     265,   266,   267,    37,   268,     0,     0,     0,     0,     0,
       0,   200,     0,     0,     0,     0,     0,     0,   269,   270,
     271,   246,   247,   248,   249,   272,   273,     0,     0,     0,
       0,     0,     0,   250,   275,   682,     0,    38,     0,   276,
       0,   277,   278,     0,     0,   251,     0,     0,     0,   252,
       0,   253,     0,     0,   254,     0,     0,     0,   255,     0,
       0,     0,     0,     0,   201,    97,    98,    99,     0,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,     0,   116,   117,   118,     0,     0,   121,
     122,   123,   124,   202,   256,     0,     0,   203,     0,     0,
       0,     0,     0,     0,     0,     0,   257,   258,     0,     0,
       0,     0,     0,     0,   204,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   205,     0,     0,   259,   260,
     261,   262,   263,   264,   265,   266,   267,    37,   268,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   269,   270,   271,   246,   247,   248,   249,   272,
     273,     0,     0,     0,     0,     0,     0,   250,   275,   689,
       0,    38,     0,   276,     0,   277,   278,     0,     0,   251,
       0,     0,     0,   252,     0,   253,     0,     0,   254,     0,
       0,     0,   255,     0,     0,     0,     0,     0,     0,    97,
      98,    99,     0,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,     0,   116,   117,
     118,     0,     0,   121,   122,   123,   124,     0,   256,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     257,   258,     0,     0,     0,     0,     0,     0,     0,   711,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,   260,   261,   262,   263,   264,   265,   266,
     267,    37,   268,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   269,   270,   271,     0,
       0,     0,     0,   272,   273,     0,   246,   247,   248,   249,
     714,     0,   275,     0,     0,    38,     0,   276,   250,   277,
     278,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     251,     0,     0,     0,   252,     0,   253,     0,     0,   254,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
      97,    98,    99,     0,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,     0,   116,
     117,   118,     0,     0,   121,   122,   123,   124,     0,   256,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   257,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,   260,   261,   262,   263,   264,   265,
     266,   267,    37,   268,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   269,   270,   271,
       0,     0,     0,     0,   272,   273,     0,   246,   247,   248,
     249,   738,     0,   275,     0,     0,    38,     0,   276,   250,
     277,   278,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   251,     0,     0,     0,   252,     0,   253,     0,     0,
     254,     0,     0,     0,   255,     0,     0,     0,     0,     0,
       0,    97,    98,    99,     0,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,     0,
     116,   117,   118,     0,     0,   121,   122,   123,   124,     0,
     256,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   257,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,   260,   261,   262,   263,   264,
     265,   266,   267,    37,   268,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   269,   270,
     271,   246,   247,   248,   249,   272,   273,     0,     0,     0,
       0,     0,     0,   250,   275,     0,     0,    38,     0,   276,
       0,   277,   278,     0,     0,   251,     0,     0,     0,   252,
       0,   253,     0,     0,   254,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,    97,    98,    99,     0,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,     0,   116,   117,   118,     0,     0,   121,
     122,   123,   124,     0,   256,     0,     0,     0,     0,     0,
     363,     0,     0,     0,     0,     0,   257,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,   260,
     261,   262,   263,   264,   265,   266,   267,    37,   268,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   269,   270,   271,     0,     0,     0,     0,   272,
     273,     0,     0,     0,     0,     0,     0,     0,   275,     0,
       0,    38,     0,   276,     0,   277,   278,   364,   365,   366,
     367,   368,   363,     0,   369,   370,   371,   372,     0,   373,
     374,   375,     0,     0,   376,     0,   377,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,     0,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
       0,     0,     0,     0,     0,     0,     0,     0,   390,   391,
       0,     0,     0,     0,     0,     0,   276,     0,     0,     0,
       0,   526,   527,   528,   529,   530,   531,   532,   533,   364,
     365,   366,   367,   368,   534,   535,   369,   370,   371,   372,
     536,   373,   374,   375,   363,     0,   376,   537,   377,   378,
     538,   539,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   540,
     379,     0,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,     0,     0,     0,     0,     0,     0,     0,     0,
     390,   391,     0,     0,     0,     0,     0,   634,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,   529,   530,   531,   532,
     533,   364,   365,   366,   367,   368,   534,   535,   369,   370,
     371,   372,   536,   373,   374,   375,   363,     0,   376,   537,
     377,   378,   538,   539,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   540,   379,     0,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,     0,     0,     0,     0,     0,     0,
       0,     0,   390,   391,     0,     0,     0,     0,     0,   661,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   526,   527,   528,   529,   530,
     531,   532,   533,   364,   365,   366,   367,   368,   534,   535,
     369,   370,   371,   372,   536,   373,   374,   375,   363,     0,
     376,   537,   377,   378,   538,   539,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   540,   379,     0,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,     0,     0,     0,     0,
       0,     0,     0,     0,   390,   391,     0,     0,     0,     0,
       0,   668,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   526,   527,   528,
     529,   530,   531,   532,   533,   364,   365,   366,   367,   368,
     534,   535,   369,   370,   371,   372,   536,   373,   374,   375,
     363,     0,   376,   537,   377,   378,   538,   539,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   379,     0,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,     0,     0,
       0,     0,     0,     0,     0,     0,   390,   391,     0,     0,
       0,     0,     0,   685,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   526,
     527,   528,   529,   530,   531,   532,   533,   364,   365,   366,
     367,   368,   534,   535,   369,   370,   371,   372,   536,   373,
     374,   375,   363,     0,   376,   537,   377,   378,   538,   539,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   540,   379,     0,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
       0,     0,     0,     0,     0,     0,     0,     0,   390,   391,
       0,     0,     0,     0,     0,   697,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,     0,     0,     0,     0,
       0,   526,   527,   528,   529,   530,   531,   532,   533,   364,
     365,   366,   367,   368,   534,   535,   369,   370,   371,   372,
     536,   373,   374,   375,     0,     0,   376,   537,   377,   378,
     538,   539,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   540,
     379,     0,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,     0,     0,   363,     0,     0,     0,     0,     0,
     390,   391,   364,   365,   366,   367,   368,   699,     0,   369,
     370,   371,   372,     0,   373,   374,   375,     0,     0,   376,
       0,   377,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,     0,     0,   363,     0,     0,
       0,     0,     0,   390,   391,     0,     0,     0,     0,     0,
     410,   364,   365,   366,   367,   368,     0,     0,   369,   370,
     371,   372,     0,   373,   374,   375,     0,     0,   376,     0,
     377,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,     0,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   363,     0,     0,     0,     0,     0,
       0,     0,   390,   391,   364,   365,   366,   367,   368,   732,
       0,   369,   370,   371,   372,     0,   373,   374,   375,     0,
       0,   376,     0,   377,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,     0,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,     0,     0,     0,
     363,     0,     0,     0,     0,   390,   391,     0,     0,   446,
       0,   364,   365,   366,   367,   368,     0,     0,   369,   370,
     371,   372,     0,   373,   374,   375,     0,     0,   376,     0,
     377,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,     0,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,     0,     0,   363,     0,     0,     0,
       0,     0,   390,   391,     0,     0,   551,   364,   365,   366,
     367,   368,     0,     0,   369,   370,   371,   372,     0,   373,
     374,   375,     0,     0,   376,     0,   377,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,     0,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
       0,     0,   363,     0,     0,     0,     0,     0,   390,   391,
       0,     0,   554,   364,   365,   366,   367,   368,     0,     0,
     369,   370,   371,   372,     0,   373,   374,   375,     0,     0,
     376,     0,   377,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,     0,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,     0,     0,   363,     0,
       0,     0,     0,     0,   390,   391,     0,     0,   638,   364,
     365,   366,   367,   368,     0,     0,   369,   370,   371,   372,
       0,   373,   374,   375,     0,     0,   376,     0,   377,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,     0,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,     0,     0,   363,     0,     0,     0,     0,     0,
     390,   391,     0,     0,   717,   364,   365,   366,   367,   368,
       0,     0,   369,   370,   371,   372,     0,   373,   374,   375,
       0,     0,   376,     0,   377,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,     0,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,     0,     0,
       0,     0,     0,     0,     0,     0,   390,   391,     0,     0,
     730,   364,   365,   366,   367,   368,     0,     0,   369,   370,
     371,   372,     0,   373,   374,   375,     0,     0,   376,     0,
     377,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,    95,
       0,     0,   379,     0,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,    96,     0,     0,     0,     0,     0,
       0,     0,   390,   391,     0,     0,   745,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
     259,   260,   261,     0,     0,     0,     0,     0,     0,    37,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,     0,
       0,     0,     0,    38,     0,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,     0,   173,   363,     0,    38,     0,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,     0,   363,     0,     0,    38,
       0,   364,   365,   366,   367,   368,     0,     0,   369,   370,
     371,   372,     0,   373,   374,   375,     0,     0,   376,     0,
     377,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
       0,     0,   379,     0,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,     0,     0,     0,     0,     0,     0,
       0,     0,   390,   391,   392,     0,     0,     0,   363,     0,
       0,     0,    38,   364,   365,   366,   367,   368,     0,     0,
     369,   370,   371,   372,     0,   373,   374,   375,     0,     0,
     376,     0,   377,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,     0,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,     0,     0,     0,     0,
       0,     0,     0,     0,   390,   391,   613,   526,   527,   528,
     529,   530,   531,   532,   533,   364,   365,   366,   367,   368,
     534,   535,   369,   370,   371,   372,   536,   373,   374,   375,
    -192,   363,   376,   537,   377,   378,   538,   539,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   379,     0,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,     0,     0,
       0,     0,     0,     0,     0,     0,   390,   391,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     526,   527,   528,   529,   530,   531,   532,   533,   364,   365,
     366,   367,   368,   534,   535,   369,   370,   371,   372,   601,
     373,   374,   375,     0,     0,   376,   537,   377,   378,   538,
     539,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,     0,     0,     0,     0,   602,   379,
       0,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,     0,     0,     0,   364,   365,   366,   367,   368,   390,
     391,   369,   370,   371,   372,     0,   373,   374,   375,     0,
       0,   376,     0,   377,   378,     0,     0,   440,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
       0,     0,     0,     0,     0,   379,     0,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,     0,     0,     0,
     364,   365,   366,   367,   368,   390,   391,   369,   370,   371,
     372,     0,   373,   374,   375,     0,     0,   376,     0,   377,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,     0,     0,     0,     0,
       0,   379,   612,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,     0,     0,     0,   364,   365,   366,   367,
     368,   390,   391,   369,   370,   371,   372,     0,   373,   374,
     375,     0,     0,   376,     0,   377,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,     0,     0,   656,     0,     0,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,     0,
       0,     0,   364,   365,   366,   367,   368,   390,   391,   369,
     370,   371,   372,     0,   373,   374,   375,     0,     0,   376,
       0,   377,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,     0,     0,
       0,     0,     0,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,     0,     0,     0,   364,   365,
     366,   367,   368,   390,   391,   369,   370,   371,   372,     0,
     373,   374,   375,     0,     0,   376,     0,   377,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,     0,     0,     0,     0,     0,     0,
       0,     0,   381,   382,   383,   384,   385,   386,   387,   388,
     389,     0,     0,     0,   364,   365,   366,   367,   368,   390,
     391,   369,   370,   371,   372,     0,   373,   374,   375,     0,
       0,   376,     0,   377,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   382,
     383,   384,   385,   386,   387,   388,   389,     0,     0,     0,
     364,   365,   366,   367,   368,   390,   391,   369,   370,   371,
     372,     0,   373,   374,   375,     0,     0,   376,     0,   377,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   363,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   383,   384,   385,   386,
     387,   388,   389,   363,     0,     0,   364,   365,     0,     0,
     368,   390,   391,   369,   370,   371,   372,     0,   373,   374,
     375,     0,     0,   376,     0,   377,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,     0,   380,
     381,   382,   383,   384,     0,     0,     0,   388,   389,   364,
     365,   366,   367,   368,     0,     0,   369,   390,   391,   372,
       0,   373,   374,   375,     0,     0,   376,     0,   377,   378,
     364,   365,   366,   367,     0,     0,     0,     0,     0,     0,
     372,     0,   373,   374,   375,     0,     0,   376,     0,   377,
     378,     0,     0,     0,     0,   383,   384,   385,   386,   387,
     388,   389,     0,     0,     0,     0,     0,     0,     0,     0,
     390,   391,     0,     0,     0,     0,     0,     0,   385,   386,
     387,   388,   389,     0,     0,     0,     0,     0,     0,     0,
       0,   390,   391,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     155,   156,   157,   158,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,     0,     0,   169,   170
};

static const yytype_int16 yycheck[] =
{
      11,    58,   215,   232,   358,   234,   347,   236,    61,   209,
     217,    32,   261,    18,     0,    18,    58,    31,    45,     5,
     194,   195,   196,    11,   309,   393,   175,    19,    18,   129,
     129,   129,   401,   486,   136,    23,   143,   143,   131,   132,
     133,    29,    28,   142,    30,    45,    32,    58,    34,   417,
     111,    62,   119,    39,    13,    14,    44,   164,   164,    45,
     131,   132,   164,   129,   163,    51,   166,    20,   166,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,   165,    52,
     166,   138,   129,   164,   138,   129,    98,    99,   129,   129,
     563,   158,    13,    14,   106,   123,   108,   109,   110,   166,
     135,   113,   107,   134,   409,   129,   163,   141,   129,   129,
     135,   165,   135,   190,   191,   192,   160,   194,   195,   196,
     197,   129,   163,    33,   158,   135,   161,   129,   190,   191,
     192,   136,   194,   195,   196,   169,   525,   129,   135,   216,
     106,   161,   163,   109,   123,   157,   158,   452,   127,   107,
     129,   130,   158,    19,   216,   163,   461,   129,   164,   190,
     191,   192,   159,   194,   195,   196,   197,   400,   362,   557,
     135,   248,   441,    56,   147,   344,   396,   346,   136,   567,
     135,   160,   259,   260,   157,   216,   248,   107,    98,    99,
     136,   157,   158,   107,   135,   135,   161,   259,   260,   560,
     114,   135,   123,   135,   159,   163,   127,   568,   129,   130,
     135,     4,   135,     6,    19,   135,   136,   248,   138,   129,
     161,   138,   136,   611,   164,   298,   135,   454,   259,   260,
     164,   308,    98,    99,   135,   312,   135,   542,   161,   164,
     106,   490,   319,   109,   110,   161,   308,   113,   164,   135,
     312,   328,   161,   330,   159,   136,   163,   319,   159,   164,
     337,   338,   161,   340,   341,   135,   328,   204,   138,   159,
     658,   141,   577,    10,   164,   337,   338,   308,   340,   341,
     129,   312,   451,   588,    21,    22,   160,   129,   319,   226,
     484,   157,   158,    98,    99,   123,    61,   328,   165,   330,
      65,   106,   129,   108,   109,   110,   337,   338,   113,   340,
     341,   165,   129,   167,   168,   129,    81,    82,    83,    84,
     257,   258,   159,   164,   261,   166,   700,   164,   359,   164,
     129,   166,   269,   270,   271,   272,   273,   138,   275,   144,
     145,   146,   147,   148,   164,   164,   166,   166,   575,   165,
      33,    33,   157,   158,   603,   131,   583,   160,    52,   165,
     142,   142,   142,   160,   142,   142,   138,   594,   142,   142,
     129,   129,   309,   165,   161,   141,   159,   168,   142,   142,
     142,   160,   609,    42,   129,   160,   142,    31,   158,   160,
     143,   160,    36,   143,   143,   129,   164,    17,   129,   336,
     129,   129,   339,   111,   159,   135,   166,   484,   345,   135,
     347,    55,   131,   646,   491,    18,   164,   123,    35,   129,
     164,   358,   484,   164,   164,   164,   164,   364,   365,   491,
     599,   368,   369,   370,   371,   662,   373,   160,   375,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   484,   391,   129,   160,   129,   159,   159,
     491,   159,   159,   107,   108,    12,   123,   138,    38,   165,
     114,   548,   409,   143,   701,   161,   129,   143,   159,   159,
      48,   418,   159,   159,   159,    47,   548,   166,   351,    83,
      50,   428,   136,   137,    11,   736,   723,   141,   223,   726,
     437,    62,    48,   440,   441,   669,    23,   444,     1,   304,
     405,   448,    29,   248,   158,   452,   599,   548,   455,   456,
     164,   330,   187,   460,   461,   169,   347,    44,   285,   466,
     566,   721,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   522,    -1,    -1,    -1,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   540,    -1,   542,    19,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    31,   552,   553,    -1,   555,    36,
      -1,   558,    -1,    -1,    -1,    -1,    -1,   564,    -1,   566,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   714,    55,    -1,
     577,    -1,    -1,    -1,    -1,   676,   163,    -1,    -1,    31,
      -1,   588,   714,    -1,    36,    -1,    -1,    -1,    -1,    -1,
      -1,   738,    -1,    -1,   601,   602,    -1,    -1,    -1,    -1,
     607,    -1,    -1,    55,    -1,   612,   738,   708,    -1,    -1,
      -1,    -1,    -1,   714,    -1,    98,    99,    -1,    -1,    -1,
     107,   108,    -1,   106,    -1,   108,   109,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,   642,    -1,   738,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,    -1,    -1,   141,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,   148,   673,   674,    -1,    -1,
      -1,   158,   679,    -1,   157,   158,    -1,    -1,    -1,    -1,
     687,    -1,   169,    -1,   691,   137,   693,   694,    -1,   141,
      -1,   143,    -1,   700,    -1,     1,    -1,   704,    -1,     5,
      -1,     7,     8,     9,   711,    11,   158,    13,    14,    15,
      16,    -1,    -1,    -1,    -1,    -1,    -1,   169,    24,    25,
      26,    27,    -1,    -1,   731,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,   740,    40,    41,    -1,    43,    -1,    45,
      46,    47,    -1,    49,    50,    -1,    -1,    53,    54,    -1,
      -1,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
     166,   167,   168,     1,    -1,    -1,    -1,     5,    -1,     7,
       8,     9,    -1,    11,    -1,    13,    14,    15,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    47,
      -1,    49,    50,    -1,    -1,    53,    54,    -1,    -1,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    55,   160,    -1,    -1,   163,   164,   165,   166,   167,
     168,    13,    14,    15,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    55,   108,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,   137,    86,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,   108,    -1,    -1,    -1,
     164,    -1,    -1,    -1,    -1,   169,    -1,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,   141,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
     152,    -1,    13,    14,    15,    16,   158,   159,   160,    -1,
      -1,   163,    -1,   165,    25,   167,   168,   169,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    55,    -1,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
     141,    -1,    -1,   144,   145,   146,    13,    14,    15,    16,
     151,   152,    -1,    -1,    -1,    -1,    -1,   158,    25,   160,
      -1,    -1,   163,    -1,   165,    -1,   167,   168,   169,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,    -1,    -1,    -1,   112,    31,    -1,    -1,    -1,
      -1,    36,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,
      13,    14,    15,    16,   151,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    25,   160,    -1,    -1,   163,   164,   165,    -1,
     167,   168,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,   108,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    76,    77,    78,    -1,    -1,    81,    82,
      83,    84,   137,    86,    -1,    -1,   141,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,    -1,   151,   152,
      -1,    -1,    -1,    -1,    -1,   158,    55,   160,   161,    -1,
     163,    -1,   165,    -1,   167,   168,    13,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    55,    -1,    -1,    -1,   108,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,   137,    86,
      -1,    -1,   141,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    -1,   108,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
     169,    36,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,    -1,   137,    -1,    -1,
      55,   141,    -1,   143,    -1,   142,    -1,   144,   145,   146,
      13,    14,    15,    16,   151,   152,    -1,    -1,   158,    -1,
      -1,    -1,    25,   160,    -1,    -1,   163,    -1,   165,   169,
     167,   168,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,   108,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    76,    77,    78,    -1,    -1,    81,    82,
      83,    84,   137,    86,    -1,    -1,   141,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   107,    -1,    -1,    -1,    -1,   112,
      31,    -1,    -1,    -1,   169,    36,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,   146,    13,    14,    15,    16,   151,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    25,   160,    -1,    -1,
     163,    -1,   165,    -1,   167,   168,    -1,    -1,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,   108,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    82,    83,    84,   137,    86,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,   160,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,   152,    -1,    13,    14,    15,    16,    -1,
     159,   160,    -1,    -1,   163,    -1,   165,    25,   167,   168,
      31,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    55,    -1,    -1,    -1,    -1,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    31,    -1,    -1,    -1,    -1,
      36,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,    -1,   137,    -1,    -1,    55,
     141,    -1,   143,    -1,    -1,    -1,   144,   145,   146,    13,
      14,    15,    16,   151,   152,    -1,    -1,   158,    -1,    -1,
      -1,    25,   160,    -1,    -1,   163,    -1,   165,   169,   167,
     168,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,   108,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    77,    78,    -1,    -1,    81,    82,    83,
      84,   137,    86,    -1,    -1,   141,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,   169,    36,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,   146,    13,    14,    15,    16,   151,   152,    -1,
      -1,    -1,    -1,    -1,    -1,    25,   160,   161,    -1,   163,
      -1,   165,    -1,   167,   168,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,   108,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      -1,    81,    82,    83,    84,   137,    86,    -1,    -1,   141,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,   169,    36,    -1,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,   146,    13,    14,    15,
      16,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    25,
     160,   161,    -1,   163,    -1,   165,    -1,   167,   168,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
     108,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    -1,    -1,    81,    82,    83,    84,   137,
      86,    -1,    -1,   141,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,   169,    36,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,    13,    14,    15,    16,   151,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    25,   160,   161,    -1,   163,    -1,   165,
      -1,   167,   168,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,   108,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,   137,    86,    -1,    -1,   141,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,   169,    36,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,    13,    14,    15,    16,   151,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    25,   160,   161,
      -1,   163,    -1,   165,    -1,   167,   168,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,   108,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    -1,    81,    82,    83,    84,   137,    86,    -1,
      -1,   141,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,   169,
      36,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,    13,
      14,    15,    16,   151,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    25,   160,    -1,    -1,   163,    -1,   165,    -1,   167,
     168,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,   108,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    77,    78,    -1,    -1,    81,    82,    83,
      84,   137,    86,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,    -1,    -1,   112,    31,
      -1,    -1,    -1,   169,    36,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,   146,    13,    14,    15,    16,   151,   152,    -1,
      -1,    -1,    -1,    -1,    -1,    25,   160,    -1,    -1,   163,
      -1,   165,    -1,   167,   168,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,   108,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      -1,    81,    82,    83,    84,   137,    86,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
      -1,    -1,   112,    31,    -1,    -1,    -1,   169,    36,    -1,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,   146,    13,    14,    15,
      16,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    25,
     160,    -1,    -1,   163,    -1,   165,    -1,   167,   168,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
     108,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    -1,    -1,    81,    82,    83,    84,   137,
      86,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,    -1,    -1,    -1,    31,    -1,    -1,
      -1,   169,    36,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,    13,    14,    15,    16,   151,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    25,   160,   161,    -1,   163,    -1,   165,
      -1,   167,   168,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,   108,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,   137,    86,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,    13,    14,    15,    16,   151,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    25,   160,   161,
      -1,   163,    -1,   165,    -1,   167,   168,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,   152,    -1,    13,    14,    15,    16,
      17,    -1,   160,    -1,    -1,   163,    -1,   165,    25,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,   152,    -1,    13,    14,    15,
      16,    17,    -1,   160,    -1,    -1,   163,    -1,   165,    25,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,    13,    14,    15,    16,   151,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    25,   160,    -1,    -1,   163,    -1,   165,
      -1,   167,   168,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,    -1,   165,    -1,   167,   168,    96,    97,    98,
      99,   100,    19,    -1,   103,   104,   105,   106,    -1,   108,
     109,   110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    19,    -1,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,   158,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    19,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,   158,    -1,    -1,    -1,    -1,    -1,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    19,    -1,
     113,   114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,   158,    -1,    -1,    -1,    -1,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      19,    -1,   113,   114,   115,   116,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,   158,    -1,    -1,
      -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    19,    -1,   113,   114,   115,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,
      -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    -1,    -1,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     157,   158,    96,    97,    98,    99,   100,   164,    -1,   103,
     104,   105,   106,    -1,   108,   109,   110,    -1,    -1,   113,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   157,   158,    -1,    -1,    -1,    -1,    -1,
     164,    96,    97,    98,    99,   100,    -1,    -1,   103,   104,
     105,   106,    -1,   108,   109,   110,    -1,    -1,   113,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,   158,    96,    97,    98,    99,   100,   164,
      -1,   103,   104,   105,   106,    -1,   108,   109,   110,    -1,
      -1,   113,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,   157,   158,    -1,    -1,   161,
      -1,    96,    97,    98,    99,   100,    -1,    -1,   103,   104,
     105,   106,    -1,   108,   109,   110,    -1,    -1,   113,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   157,   158,    -1,    -1,   161,    96,    97,    98,
      99,   100,    -1,    -1,   103,   104,   105,   106,    -1,   108,
     109,   110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   157,   158,
      -1,    -1,   161,    96,    97,    98,    99,   100,    -1,    -1,
     103,   104,   105,   106,    -1,   108,   109,   110,    -1,    -1,
     113,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   157,   158,    -1,    -1,   161,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,   105,   106,
      -1,   108,   109,   110,    -1,    -1,   113,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     157,   158,    -1,    -1,   161,    96,    97,    98,    99,   100,
      -1,    -1,   103,   104,   105,   106,    -1,   108,   109,   110,
      -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,   158,    -1,    -1,
     161,    96,    97,    98,    99,   100,    -1,    -1,   103,   104,
     105,   106,    -1,   108,   109,   110,    -1,    -1,   113,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    29,
      -1,    -1,   137,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,   158,    -1,    -1,   161,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
     120,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,   129,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,   160,    19,    -1,   163,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    19,    -1,    -1,   163,
      -1,    96,    97,    98,    99,   100,    -1,    -1,   103,   104,
     105,   106,    -1,   108,   109,   110,    -1,    -1,   113,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,   137,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,   158,   159,    -1,    -1,    -1,    19,    -1,
      -1,    -1,   163,    96,    97,    98,    99,   100,    -1,    -1,
     103,   104,   105,   106,    -1,   108,   109,   110,    -1,    -1,
     113,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,   158,   159,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    19,   113,   114,   115,   116,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,   158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,   136,   137,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    -1,    96,    97,    98,    99,   100,   157,
     158,   103,   104,   105,   106,    -1,   108,   109,   110,    -1,
      -1,   113,    -1,   115,   116,    -1,    -1,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   137,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   157,   158,   103,   104,   105,
     106,    -1,   108,   109,   110,    -1,    -1,   113,    -1,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   157,   158,   103,   104,   105,   106,    -1,   108,   109,
     110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,   134,    -1,    -1,   137,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   157,   158,   103,
     104,   105,   106,    -1,   108,   109,   110,    -1,    -1,   113,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   137,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   157,   158,   103,   104,   105,   106,    -1,
     108,   109,   110,    -1,    -1,   113,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    -1,    96,    97,    98,    99,   100,   157,
     158,   103,   104,   105,   106,    -1,   108,   109,   110,    -1,
      -1,   113,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,   146,   147,   148,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   157,   158,   103,   104,   105,
     106,    -1,   108,   109,   110,    -1,    -1,   113,    -1,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,   147,   148,    19,    -1,    -1,    96,    97,    -1,    -1,
     100,   157,   158,   103,   104,   105,   106,    -1,   108,   109,
     110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,   147,   148,    96,
      97,    98,    99,   100,    -1,    -1,   103,   157,   158,   106,
      -1,   108,   109,   110,    -1,    -1,   113,    -1,   115,   116,
      96,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,    -1,   113,    -1,   115,
     116,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
     147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,   158,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,   158,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,    -1,    -1,   151,   152
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   171,     0,     5,    28,    30,    32,    34,    39,    45,
      51,   158,   164,   172,   177,   178,   182,   201,   205,   219,
     249,   251,   252,   255,   129,   196,   197,   123,   183,   184,
     129,   179,   180,   129,   129,   129,    32,   129,   163,   198,
     199,   200,   210,     4,     6,    56,   248,   136,   135,   138,
     135,    20,    52,   147,   157,   181,   138,   165,   136,   163,
     129,   160,   135,   159,   129,   254,    33,    98,    99,   129,
     204,   165,    13,    14,   123,   127,   129,   130,   160,   174,
     196,   123,   184,   129,   129,   129,    61,    65,    81,    82,
      83,    84,   258,   250,    23,    29,    44,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,   210,   257,   259,   260,
     261,   129,   197,   199,   138,   253,   165,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   115,   116,   117,   118,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   151,
     152,    33,    33,   160,   202,   247,   131,   173,    13,    14,
     123,   127,   129,   174,   194,   195,   181,   165,   129,   166,
     142,   142,   142,   160,   142,   142,   142,   142,    31,    36,
      55,   108,   137,   141,   158,   169,   161,   210,   237,    45,
     129,   161,   238,   239,   243,   256,   138,   203,   166,   246,
     256,   131,   132,   135,   161,   250,   136,   164,   261,   261,
     261,   129,   202,   261,   202,   261,   202,   261,   129,   240,
     241,   261,    31,   141,   158,   169,    13,    14,    15,    16,
      25,    37,    41,    43,    46,    50,    86,    98,    99,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   130,   144,
     145,   146,   151,   152,   159,   160,   165,   167,   168,   176,
     206,   210,   212,   221,   222,   225,   226,   227,   230,   231,
     232,   233,   257,   262,   265,   270,   272,   274,   158,   166,
     234,   236,   243,   161,   164,   107,   135,   136,   138,   242,
     261,   206,   138,   141,   245,   194,   166,   233,   143,   164,
     143,   161,   203,   143,   203,   143,   203,   143,   138,   143,
     164,   141,   159,   261,   262,   160,   160,   142,   142,   160,
     142,   142,   233,   233,    11,   261,    11,   261,   233,   267,
     271,   175,   233,   233,   233,   233,   233,   233,   208,   168,
     129,   160,   201,    19,    96,    97,    98,    99,   100,   103,
     104,   105,   106,   108,   109,   110,   113,   115,   116,   137,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     157,   158,   159,   160,   197,    42,   235,   164,   238,   129,
     261,   112,   209,   228,   233,   261,   107,   114,   136,   244,
     164,   261,   143,   143,   143,   261,   240,   160,   129,   233,
     261,   261,   233,   261,   261,   256,   129,   159,   160,   233,
     263,   264,   266,   267,   268,   269,   256,   160,   264,   269,
     119,   164,   166,   132,   133,   173,   161,     1,     7,     8,
       9,    11,    24,    26,    27,    36,    40,    47,    49,    53,
      54,    87,   164,   166,   185,   186,   189,   190,   191,   192,
     193,   207,   209,   211,   213,   214,   215,   216,   217,   219,
     220,   228,   229,   233,   142,   210,   158,   161,   223,   233,
     202,    17,   233,   233,   233,   233,   233,   233,   129,   233,
     129,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   129,   233,   161,   223,
     159,   243,   242,   168,   226,   111,    88,    89,    90,    91,
      92,    93,    94,    95,   101,   102,   107,   114,   117,   118,
     136,   164,   244,   209,   233,   143,   161,   223,    17,   142,
     233,   161,   143,   143,   161,   143,   143,    18,   242,   159,
     161,   135,   159,   164,   135,   159,   164,    18,   161,   166,
     166,   233,   267,   166,   233,   233,   256,   107,   164,   209,
     233,   206,   233,   233,   164,   123,    53,   233,   107,   209,
     233,    35,   164,   164,   233,   164,   164,   164,    18,   218,
     164,   107,   136,   202,   261,   129,   263,   135,   161,   203,
     261,   160,   138,   159,   161,   233,   226,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   209,   233,   164,   161,   261,   129,   161,   233,
     233,   233,   160,   223,   233,   264,   129,   159,   263,   233,
     159,   268,   223,   264,   159,   159,   134,   206,    18,   209,
     233,   164,    12,   206,   138,   123,   209,   233,   164,   165,
     206,    38,   246,   112,   112,   203,   143,   159,   233,   119,
     206,   224,   161,   223,   233,   164,   161,   143,   164,   161,
     233,   164,   159,   242,   164,   166,   223,   164,   206,   164,
     208,    10,    21,    22,   187,   188,   233,   233,   143,   210,
     161,   107,   233,   161,    17,   233,   129,   161,   233,   159,
     233,   233,   159,   164,   166,   206,   233,   210,   233,   261,
     161,   143,   164,   273,   273,   206,   206,   161,    17,   233,
      48,   159,   166,   188,   261,   161,   233,   159,   159,   161
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   170,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   172,   173,   173,   174,   175,   175,   175,
     176,   177,   178,   179,   179,   179,   180,   180,   181,   181,
     182,   183,   183,   184,   184,   185,   186,   186,   187,   187,
     188,   188,   188,   189,   189,   190,   191,   192,   193,   194,
     194,   194,   194,   194,   194,   195,   195,   196,   196,   196,
     196,   196,   196,   196,   196,   197,   197,   198,   198,   199,
     199,   200,   200,   201,   201,   202,   202,   202,   203,   203,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   205,   206,   206,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   208,   208,
     208,   209,   209,   210,   210,   210,   211,   212,   212,   212,
     212,   213,   214,   215,   215,   215,   215,   215,   216,   216,
     216,   216,   217,   218,   218,   219,   219,   220,   221,   221,
     221,   222,   222,   222,   222,   222,   222,   223,   223,   224,
     224,   224,   225,   225,   226,   226,   227,   227,   227,   227,
     227,   227,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   229,   229,
     230,   231,   231,   232,   232,   232,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   234,   234,   235,   235,   236,
     237,   237,   238,   238,   239,   239,   240,   240,   241,   241,
     242,   242,   243,   243,   243,   243,   243,   244,   244,   244,
     245,   245,   246,   246,   246,   246,   246,   247,   247,   248,
     248,   249,   250,   250,   250,   251,   252,   252,   253,   253,
     254,   255,   256,   256,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   258,   258,
     258,   258,   258,   258,   259,   260,   260,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   262,   262,   262,   262,
     263,   263,   264,   264,   265,   265,   265,   265,   265,   266,
     266,   267,   268,   268,   269,   269,   270,   270,   271,   271,
     272,   273,   273,   274,   274
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     2,     3,     0,     2,     4,
       3,     2,     2,     1,     3,     3,     2,     4,     0,     1,
       2,     1,     3,     1,     3,     3,     3,     2,     1,     1,
       0,     2,     4,     1,     1,     4,     6,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     1,     5,     1,     3,     1,     1,     1,
       4,     1,     3,     0,     3,     0,     2,     3,     0,     2,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     6,     3,     7,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     2,     2,     2,     0,     2,
       2,     3,     2,     1,     3,     2,     2,     2,     4,     5,
       2,     1,     1,     2,     3,     4,     2,     3,     3,     4,
       2,     3,     4,     0,     2,     1,     1,     3,     5,     5,
       5,     5,     8,    10,     6,     9,    11,     1,     3,     1,
       2,     3,     1,     1,     1,     5,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     4,
       6,     5,     6,     3,     6,     7,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     3,
       4,     3,     3,     1,     3,     4,     3,     4,     2,     4,
       4,     6,     7,     3,     5,     4,     1,     1,     1,     1,
       1,     1,     3,     3,     2,     0,     3,     0,     1,     3,
       0,     3,     1,     2,     1,     3,     1,     3,     1,     3,
       1,     1,     1,     3,     5,     3,     3,     1,     1,     1,
       0,     1,     4,     6,     5,     5,     4,     0,     2,     0,
       1,     5,     0,     3,     5,     4,     5,     7,     0,     2,
       2,     6,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     1,     1,     1,
       4,     3,     4,     2,     3,     2,     4,     3,     2,     2,
       3,     2,     2,     4,     6,     4,     1,     4,     5,     1,
       4,     5,     1,     4,     5,     4,     1,     1,     1,     1,
       3,     5,     1,     3,     5,     4,     7,     5,     7,     1,
       3,     3,     1,     1,     1,     3,     5,     5,     1,     3,
       4,     0,     3,    10,    10
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
                       &(yyvsp[(yyi + 1) - (yynrhs)])
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
            /* Fall through.  */
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
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          case 129: /* "name"  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3001 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* character_sequence  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3007 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* string_constant  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3013 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* string_builder_body  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3019 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* string_builder  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3025 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 179: /* require_module_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3031 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* expression_label  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3037 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expression_goto  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3043 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* expression_else  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3049 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* expression_if_then_else  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3055 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* expression_for_loop  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3061 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* expression_while_loop  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3067 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* expression_with  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3073 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* annotation_argument_value  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 3079 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* annotation_argument_value_list  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3085 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* annotation_argument  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 3091 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* annotation_argument_list  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3097 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* annotation_declaration_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3103 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* annotation_declaration  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 3109 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* annotation_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3115 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* optional_annotation_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3121 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* optional_function_argument_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3127 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* optional_function_type  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3133 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* function_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3139 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* expression_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3145 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* expression_any  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3151 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* expressions  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3157 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* expr_pipe  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3163 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* name_in_namespace  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3169 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* expression_delete  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3175 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* expr_new  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3181 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* expression_break  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3187 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 214: /* expression_continue  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3193 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 215: /* expression_return  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3199 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* expression_yield  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3205 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 217: /* expression_try_catch  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3211 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 220: /* expression_let  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3217 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 221: /* expr_cast  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3223 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 222: /* expr_type_info  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3229 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* expr_list  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3235 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* block_or_simple_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3241 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 226: /* expr_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3247 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 227: /* expr_numeric_const  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3253 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* expr_assign  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3259 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* expr_assign_pipe  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3265 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 230: /* expr_named_call  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3271 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* expr_method_call  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3277 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* func_addr_expr  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3283 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* expr  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3289 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 234: /* optional_field_annotation  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3295 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 236: /* structure_variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3301 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 237: /* struct_variable_declaration_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3307 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* function_argument_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3313 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 239: /* function_argument_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3319 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 240: /* tuple_type  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3325 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* tuple_type_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3331 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 243: /* variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3337 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 246: /* let_variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3343 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 247: /* global_variable_declaration_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3349 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 250: /* enum_list  */
#line 113 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 3355 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 253: /* optional_structure_parent  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3361 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 256: /* variable_name_list  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 3367 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 259: /* structure_type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3373 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 260: /* auto_type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3379 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 261: /* type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3385 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 262: /* make_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3391 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 263: /* make_struct_fields  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 3397 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 264: /* make_struct_dim  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3403 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 265: /* make_struct_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3409 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 266: /* make_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3415 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 267: /* make_map_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3421 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 268: /* make_any_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3427 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 269: /* make_dim  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3433 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 270: /* make_dim_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3439 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 271: /* make_table  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3445 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 272: /* make_table_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3451 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 273: /* array_comprehension_where  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3457 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 274: /* array_comprehension  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3463 "ds_parser.cpp" /* yacc.c:1254  */
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
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

#ifdef yyoverflow
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
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
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
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
| yyreduce -- Do a reduction.  |
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
        case 13:
#line 393 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3759 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 405 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3765 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 406 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3771 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 410 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3777 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 414 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3786 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 418 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3800 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 427 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3810 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 20:
#line 435 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3828 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 451 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3851 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 476 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3859 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 479 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3870 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 485 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3881 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 494 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3907 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 515 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3932 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 538 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3938 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 539 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3944 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 552 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3952 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 555 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3960 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 561 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3968 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 567 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 3976 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 570 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3984 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 576 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3990 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 577 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3996 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 581 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4002 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 582 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4008 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 583 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4019 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 592 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4025 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 593 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4031 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 597 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4042 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 606 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 4056 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 618 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4068 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 628 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4079 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 637 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4085 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 638 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4091 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 639 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4097 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 640 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4103 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 641 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4109 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 642 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4115 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 646 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4125 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 651 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4135 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 659 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4141 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 660 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4147 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 661 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4153 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 662 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4159 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 663 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4165 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 664 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4171 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 665 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4177 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 666 "ds_parser.ypp" /* yacc.c:1660  */
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4185 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 672 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4203 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 685 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4221 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 701 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4227 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 702 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 4233 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 706 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 4245 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 713 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4259 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 725 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4268 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 729 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4277 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 736 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 4283 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 737 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4289 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 741 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4295 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 742 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4301 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 743 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4307 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 747 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4316 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 751 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4325 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 758 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 4334 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 762 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 4340 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 763 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 4346 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 764 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 4352 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 765 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 4358 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 766 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 4364 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 767 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 4370 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 768 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 4376 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 769 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 4382 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 770 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 4388 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 771 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 4394 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 772 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 4400 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 773 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 4406 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 774 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 4412 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 775 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 4418 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 776 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 4424 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 777 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 4430 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 778 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 4436 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 779 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 4442 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 780 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 4448 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 781 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 4454 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 782 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 4460 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 783 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 4466 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 784 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 4472 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 785 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 4478 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 786 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 4484 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 787 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 4490 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 788 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 4496 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 789 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 4502 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 790 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 4508 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 791 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 4514 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 792 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 4520 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 793 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 4526 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 794 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 4532 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 795 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 4538 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 796 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 4544 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 797 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 4550 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 802 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4621 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 871 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4629 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 874 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4641 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 884 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4647 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 885 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4653 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 886 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4659 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 887 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4665 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 888 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4671 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 889 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4677 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 890 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4683 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 891 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4689 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 892 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4695 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 893 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4701 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 894 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4707 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 895 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4713 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 896 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4719 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 897 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4725 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 898 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4731 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 899 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4737 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 900 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4743 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 901 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4749 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 905 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4758 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 909 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4769 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 915 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4777 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 921 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4793 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 932 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4801 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 938 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4807 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 939 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4823 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 950 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4829 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 954 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4837 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 960 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4845 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 963 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4853 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 966 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4862 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 970 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4870 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 976 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4876 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 980 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4882 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 984 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4890 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 987 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4898 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 990 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4908 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 995 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4916 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 998 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4926 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 1006 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4934 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 1009 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4944 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 1014 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4952 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1017 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4962 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1025 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4970 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1031 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4976 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1032 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4982 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1036 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4988 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1037 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4994 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1041 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5030 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1075 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5038 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1078 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5048 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1083 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5058 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1091 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5067 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1095 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5077 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1100 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5088 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1106 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 5097 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1110 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5107 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1115 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5118 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1124 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5126 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1127 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5134 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1133 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5140 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1134 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5152 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1141 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5165 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1152 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false;   /* block */  }
#line 5171 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1153 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true;    /* lambda */ }
#line 5177 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1157 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 5187 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1163 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5240 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1214 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5246 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1215 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5252 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1216 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5258 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1217 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5264 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1218 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5270 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1219 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5276 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1223 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5282 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1224 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5288 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1225 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5294 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1226 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5300 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1227 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5306 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1228 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5312 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1229 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5318 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1230 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5324 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1231 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5330 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1232 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5336 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1233 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5342 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1234 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5348 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1235 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5354 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1236 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5360 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1237 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5366 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1238 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5372 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1242 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5378 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1243 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5384 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1247 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5396 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1258 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5406 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1263 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5418 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1273 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
	}
#line 5427 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1277 "ds_parser.ypp" /* yacc.c:1660  */
    {
		auto expr = new ExprAddr(tokAt((yylsp[-4])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
        expr->funcType = make_shared<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5439 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1284 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5455 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1298 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5461 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1299 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5467 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1300 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5473 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1301 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5479 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1302 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5485 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1303 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5491 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1304 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5497 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1305 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5503 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1306 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5509 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1307 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5515 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1308 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5521 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1309 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5527 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1310 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5533 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1311 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5539 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1312 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5545 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1313 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5551 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1314 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5557 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1315 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5563 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1316 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5569 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1317 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5575 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1318 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5581 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1319 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5587 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1320 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5593 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1321 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5599 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1322 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5605 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1323 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5611 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1324 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5617 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1325 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5623 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1326 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5629 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1327 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5635 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1328 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5641 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1329 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5647 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1330 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5653 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1331 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5659 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1332 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5665 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1333 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5671 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1334 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5677 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1335 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5683 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1336 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5692 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1340 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5701 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1344 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5709 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1347 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5717 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1350 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5723 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1351 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5729 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1352 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5735 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1353 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5745 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1358 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5755 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1363 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5761 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1364 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5769 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1367 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5777 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1370 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5783 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1371 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5789 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1372 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5795 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1373 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5801 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1374 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5807 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1375 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5813 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1376 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5828 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1386 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5843 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1396 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5879 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1430 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 5885 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1431 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5891 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1435 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5897 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1436 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5903 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1440 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5913 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1448 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5921 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1451 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5930 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1458 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5936 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1459 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5942 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1463 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5948 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1464 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5954 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1468 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5962 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1471 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 5973 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1480 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5979 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1481 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5985 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1485 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5991 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1486 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5997 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1490 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 6007 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1495 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6015 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1498 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6024 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1502 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6035 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1508 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6046 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1517 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 6052 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1518 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 6058 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1519 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 6064 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1523 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6070 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1524 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6076 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1528 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6084 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1531 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6094 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1536 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6104 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1541 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6117 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1549 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6130 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1560 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6138 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1563 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6147 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1570 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6153 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1571 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6159 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1575 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6191 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1605 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6199 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1608 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6213 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1617 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6227 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1629 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 6241 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1641 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6257 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1652 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6274 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1667 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 6280 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1668 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 6286 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1672 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6327 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1711 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6395 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1777 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6407 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1784 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6418 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1793 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 6424 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1794 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 6430 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1795 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6436 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1796 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6442 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1797 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6448 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1798 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 6454 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1799 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 6460 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1800 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 6466 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1801 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 6472 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1802 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6478 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1803 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6484 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1804 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6490 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1805 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 6496 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1806 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 6502 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1807 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 6508 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1808 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 6514 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1809 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 6520 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1810 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 6526 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1811 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 6532 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1812 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 6538 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1813 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 6544 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1814 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 6550 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1815 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 6556 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1816 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 6562 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1820 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6568 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1821 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6574 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1822 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6580 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 351:
#line 1823 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6586 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 352:
#line 1824 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6592 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 353:
#line 1825 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6598 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 354:
#line 1829 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6611 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 355:
#line 1840 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6620 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 356:
#line 1844 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6632 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 357:
#line 1854 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6638 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 358:
#line 1855 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6644 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 359:
#line 1856 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6650 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 360:
#line 1857 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6669 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 361:
#line 1871 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6680 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 362:
#line 1877 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6689 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 363:
#line 1881 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6699 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 364:
#line 1886 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6709 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 365:
#line 1891 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6719 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 366:
#line 1896 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6731 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 367:
#line 1903 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6741 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 368:
#line 1908 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6750 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 369:
#line 1912 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6759 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 370:
#line 1916 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6769 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 371:
#line 1921 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6779 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 372:
#line 1926 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6791 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 373:
#line 1933 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6801 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 374:
#line 1938 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6812 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 375:
#line 1944 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6822 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 376:
#line 1949 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6831 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 377:
#line 1953 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6841 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 378:
#line 1958 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6855 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 379:
#line 1967 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6864 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 380:
#line 1971 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6874 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 381:
#line 1976 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6888 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 382:
#line 1985 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6897 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 383:
#line 1989 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6907 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 384:
#line 1994 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6921 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 385:
#line 2003 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6932 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 386:
#line 2012 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6938 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 387:
#line 2013 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6944 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 388:
#line 2014 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6950 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 389:
#line 2015 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6956 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 390:
#line 2019 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 6968 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 391:
#line 2026 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 6979 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 392:
#line 2035 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 6989 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 393:
#line 2040 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6998 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 394:
#line 2047 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7008 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 395:
#line 2052 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7019 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 396:
#line 2058 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7030 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 397:
#line 2064 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7042 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 398:
#line 2071 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7055 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 399:
#line 2082 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7063 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 400:
#line 2085 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7079 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 401:
#line 2099 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7090 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 402:
#line 2108 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7096 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 403:
#line 2109 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7102 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 404:
#line 2113 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7112 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 405:
#line 2118 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7121 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 406:
#line 2125 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7131 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 407:
#line 2130 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7144 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 408:
#line 2141 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7154 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 409:
#line 2146 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7163 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 410:
#line 2153 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7177 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 411:
#line 2165 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 7183 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 412:
#line 2166 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7189 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 413:
#line 2170 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7208 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 414:
#line 2184 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7227 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 7231 "ds_parser.cpp" /* yacc.c:1660  */
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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
#line 2200 "ds_parser.ypp" /* yacc.c:1903  */


void das_checkName(const string & name, const LineInfo &at) {
	if ( name.length()>=2 && name[0]=='_' && name[1]=='_' ) {
		g_Program->error("names starting with __ are reserved, " + name,at,CompilationError::invalid_name);
	}
}

void das_yyerror(const string & error, const LineInfo & at, CompilationError cerr) {
    // printf("ERROR %s\n", error.c_str());
    g_Program->error(error,at,cerr);
}

void das_yyerror(const string & error, CompilationError cerr) {
    // printf("ERROR %s\n", error.c_str());
    g_Program->error(error,LineInfo(g_FileAccessStack.back(),
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


