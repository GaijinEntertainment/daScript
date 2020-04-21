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
#define YYLAST   6724

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
       2,     2,     2,   154,     2,   171,   165,   150,   143,     2,
     163,   164,   148,   147,   137,   146,   159,   149,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   140,   168,
     144,   138,   145,   139,   166,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   161,     2,   162,   142,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   169,   141,   170,   153,     2,     2,     2,
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
     135,   136,   151,   152,   155,   156,   157,   158,   160,   167
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   387,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   402,   414,   415,   419,   423,   427,
     436,   444,   460,   481,   485,   488,   494,   503,   524,   547,
     548,   552,   556,   557,   561,   564,   570,   576,   579,   585,
     586,   590,   591,   592,   601,   602,   606,   615,   627,   637,
     646,   647,   648,   649,   650,   651,   655,   660,   668,   669,
     670,   671,   672,   673,   674,   675,   681,   694,   710,   711,
     715,   722,   734,   738,   745,   746,   750,   751,   752,   756,
     760,   767,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
     789,   790,   791,   792,   793,   794,   795,   796,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,   810,   883,
     886,   896,   897,   898,   899,   900,   901,   902,   903,   904,
     905,   906,   907,   908,   909,   910,   911,   912,   913,   917,
     921,   927,   933,   944,   950,   951,   962,   966,   972,   975,
     978,   982,   988,   992,   996,   999,  1002,  1007,  1010,  1018,
    1021,  1026,  1029,  1037,  1043,  1044,  1048,  1049,  1053,  1087,
    1090,  1095,  1103,  1107,  1112,  1118,  1122,  1127,  1136,  1139,
    1145,  1146,  1153,  1164,  1165,  1169,  1174,  1229,  1230,  1231,
    1232,  1233,  1234,  1238,  1239,  1240,  1241,  1242,  1243,  1244,
    1245,  1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,  1257,
    1258,  1262,  1273,  1278,  1288,  1292,  1299,  1313,  1314,  1315,
    1316,  1317,  1318,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1326,  1327,  1328,  1329,  1330,  1331,  1332,  1333,  1334,  1335,
    1336,  1337,  1338,  1339,  1340,  1341,  1342,  1343,  1344,  1345,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,  1356,  1360,  1363,
    1366,  1367,  1368,  1369,  1374,  1379,  1380,  1383,  1386,  1390,
    1394,  1398,  1399,  1400,  1401,  1402,  1403,  1404,  1414,  1424,
    1458,  1459,  1463,  1464,  1468,  1476,  1479,  1486,  1491,  1499,
    1500,  1504,  1507,  1516,  1517,  1521,  1530,  1531,  1536,  1537,
    1541,  1546,  1549,  1553,  1559,  1568,  1569,  1570,  1574,  1575,
    1579,  1582,  1587,  1592,  1600,  1611,  1614,  1621,  1622,  1626,
    1656,  1659,  1668,  1681,  1693,  1694,  1698,  1702,  1713,  1728,
    1729,  1733,  1772,  1838,  1845,  1854,  1855,  1856,  1857,  1858,
    1859,  1860,  1861,  1862,  1863,  1864,  1865,  1866,  1867,  1868,
    1869,  1870,  1871,  1872,  1873,  1874,  1875,  1876,  1877,  1881,
    1882,  1883,  1884,  1885,  1886,  1890,  1901,  1905,  1915,  1916,
    1917,  1918,  1932,  1938,  1942,  1947,  1952,  1957,  1964,  1969,
    1973,  1977,  1982,  1987,  1994,  1999,  2005,  2010,  2014,  2019,
    2028,  2032,  2037,  2046,  2050,  2055,  2064,  2070,  2079,  2085,
    2100,  2101,  2102,  2103,  2107,  2114,  2123,  2128,  2135,  2140,
    2146,  2152,  2159,  2170,  2173,  2187,  2196,  2197,  2201,  2206,
    2213,  2218,  2229,  2234,  2241,  2253,  2254,  2258,  2272
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
  "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC", "'.'", "DEREF", "'['",
  "']'", "'('", "')'", "'$'", "'@'", "COLCOL", "';'", "'{'", "'}'", "'#'",
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
     398,    91,    93,    40,    41,    36,    64,   399,    59,   123,
     125,    35
};
# endif

#define YYPACT_NINF -487

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-487)))

#define YYTABLE_NINF -194

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -487,    21,  -487,  -487,   -63,   -55,   -27,   -21,   -47,  -487,
      23,  -487,   -15,  -487,  -487,  -487,  -487,  -487,   293,  -487,
      63,  -487,  -487,  -487,  -487,    36,  -487,    45,  -487,    41,
      59,    74,  -487,  -487,    17,  -487,   -16,   140,  -487,  -487,
     112,   152,    67,  -487,    56,  -487,   160,    -8,  -487,   123,
     129,    10,   -63,   180,   -55,   177,  -487,   181,   184,  -487,
     170,  -487,   182,  -487,   -93,   191,  -487,   -63,   -15,  -487,
     183,   156,  6570,   294,   296,  -487,   167,  -487,   200,  -487,
    -487,  -487,  -487,  -487,   201,    66,  -487,  -487,  -487,  -487,
     281,  -487,  -487,  -487,  -487,  -487,  -487,  -487,  -487,   166,
     -88,  5577,  -487,  -487,  -487,   -90,  -487,   -64,  -487,  -487,
    -487,  -487,  -487,  -487,  -487,  -487,  -487,  -487,  -487,  -487,
    -487,  -487,  -487,  -487,  -487,  -487,  -487,  -487,  -487,  -487,
    -487,  -487,  -487,  -487,  -487,  -487,  -487,  -487,  -487,  -487,
    -487,  -487,  -487,  -487,  -487,  -487,    64,   196,   -70,   198,
    -487,   171,  -487,   130,  -487,  -487,  -487,  -487,  -487,  -487,
    -487,   -89,  -487,  -487,   -97,  -487,   197,   207,   208,  -487,
    -487,  -487,   190,  -487,  -487,  -487,  -487,  -487,  -487,  -487,
    -487,  -487,  -487,  -487,  -487,  -487,  -487,   211,  -487,  -487,
    -487,   212,   213,  -487,  -487,  -487,  -487,   214,   215,  -487,
    -487,  -487,  -487,    42,  -487,  -487,  -111,   -63,  -487,   -17,
    -487,  -110,  5577,   193,  -487,  -487,  -487,   102,  5577,   -69,
    -487,  -487,    66,  -487,   -68,  4026,  -487,  5577,  5577,  5577,
     209,  5502,  5502,  5502,  5652,   200,  -487,     1,  -487,  -487,
    -487,  -487,  2058,  -487,  -487,  -487,   318,   195,    72,   233,
    -487,   -32,  -487,   204,  1175,  -487,  -487,   237,  5577,  -487,
      76,  1175,  -487,  -487,  -487,  -487,  -487,  -487,  5427,   210,
    -487,   221,   226,   227,   222,   228,   230,  4026,  4026,   773,
    4152,  4026,  -487,  -487,  -487,  -487,  -487,  -487,  -487,  4026,
    4026,  4026,  4026,  4026,  4026,  -487,   224,  -487,  -487,   -50,
    -487,  -487,  -487,   234,  -487,  -487,  -487,  -487,  -487,  4774,
     235,  -487,  -487,  -487,  -487,  -487,   306,   517,  1238,   223,
     196,  1381,   196,  1607,   196,  2182,   -42,  -487,    20,  1175,
      98,   232,  -487,   239,  -487,  -487,  5648,  -487,   233,  -487,
    -487,  -487,  -487,  -487,  5577,  2185,  -487,   923,  -487,   139,
    -487,  -487,  -487,  2185,  2267,  -487,   265,  4026,  5577,  5577,
    4026,  5577,  5577,   701,   701,   233,  1239,   233,  1382,  6034,
    -487,   136,   161,   701,   701,   -71,   701,   701,  4911,   -79,
    -487,  1661,   167,    16,   275,  4026,  4026,  -487,  -487,  4026,
    4026,  4026,  4026,   276,  4026,   277,  4026,  4026,  4026,  4026,
    4026,  2321,  4026,  4026,  4026,  4026,  4026,  4026,  4026,  4026,
    4026,  4026,   278,  4026,  -487,  2448,  -487,  5577,  -487,  -487,
     267,  -487,   268,  -487,   272,  -487,  5577,  -487,  5652,  -487,
     200,  -487,  -487,  -487,  -487,   205,   119,  -487,   297,  5867,
    -487,  4026,  -487,  -487,   233,  1525,  -487,   193,  4026,  4026,
    -487,   251,   295,  2584,  1804,  -487,   387,   255,   256,  4026,
    -487,  -487,  -487,  -487,  -487,  -487,   257,   266,   269,  -487,
    -487,  -487,   415,  -487,   -83,  -487,  5969,  -487,  2185,  -487,
    4239,  2711,  1931,  4979,  2445,  2530,  5047,  2531,  2708,    -7,
     -74,   274,  2847,  6034,   301,    70,   303,  -487,  -487,   108,
       0,  2983,   141,   148,  4026,  4026,   283,  -487,  4026,   309,
    -487,  5502,  -487,   313,  -487,   -38,  6233,   196,  5577,  -487,
    -487,   494,   494,   615,   615,   282,   282,   284,   383,  -487,
    5723,   -65,   -65,   494,   494,   317,  6101,  6298,  6365,  6430,
     615,   615,  4306,  4306,   383,   383,   383,  -487,  5792,  -487,
      26,  2793,  -487,  -487,  -487,  1175,  -487,  4026,  -487,  -487,
     119,  4026,  4026,  4026,  4026,  4026,  4026,  4026,  4026,  4026,
    4026,  4026,  4026,  4026,  4026,  4026,  4145,    13,  2185,  -487,
    -487,  4333,   440,  6233,  4145,  -487,   314,   330,  6233,  2185,
    -487,  4427,   289,  -487,  -487,  4145,  -487,  -487,  -487,   423,
     233,  -487,  3119,  3246,  -487,  4521,  -487,  -487,    37,  5577,
     331,  5115,  -487,  4026,  4026,  -487,  4026,   300,  4026,  4026,
    -487,   313,   333,   312,   313,  4026,   316,  4026,  4026,   313,
     319,   322,  6233,  -487,  -487,  6166,   196,  2794,   -51,    75,
    4026,  -487,   -86,  1175,  3373,  -487,  -487,  4026,  -487,  -487,
    -487,  6233,  -487,  6233,  6233,  6233,  6233,  6233,  6233,  6233,
    6233,  6233,  6233,  6233,  6233,  6233,  6233,  6233,  -487,  4026,
    -487,  4615,  -487,   193,  -487,  -487,  -487,  -487,  4709,  -487,
    -487,    99,  -487,  -487,  4026,  4026,  -487,  -487,  2930,   105,
    -487,   615,   615,   615,  3509,   -53,  6233,   113,   -51,  -487,
     301,  6233,  -487,  -487,   -20,   151,  -487,  -487,  -487,   320,
     -64,   311,  6233,  3645,  -487,  -487,  -487,    40,  6233,    34,
    -487,  -487,  -487,  1093,   193,  -487,  -487,  4026,  -487,  6497,
    6497,  -487,  3772,   345,  -487,  5183,  4026,   323,  4026,  4026,
     324,   -64,  -487,  -487,  4026,  6233,  -487,   193,  -487,  -487,
    4145,  5577,  5251,   334,  -487,  4846,  -487,  6233,  4846,  -487,
    -487,  6233,  -487,    99,  3066,  -487,  3899,   432,   325,   321,
    -487,  -487,  5577,  5319,  4026,   326,   327,  3455,  -487,  6233,
    -487,  -487,  -487
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
    -487,  -487,  -487,   124,   446,  -487,  -487,  -487,  -487,  -487,
    -487,   410,  -487,  -487,   447,  -487,  -487,  -487,  -261,  -487,
    -487,  -487,  -487,  -487,   285,  -487,   451,   -58,  -487,   436,
    -487,   202,  -218,  -312,  -487,  -487,  -207,  -487,  -172,  -309,
     -12,  -487,  -487,  -487,  -487,  -487,  -487,  -487,  -487,   509,
    -487,  -487,  -487,  -395,  -487,  -487,  -417,  -487,  -345,  -487,
    -487,  -487,  -487,   179,   305,  -487,  -487,  -487,   259,  -487,
      87,  -487,  -203,   286,  -248,  -227,   169,  -487,   -84,  -487,
    -487,  -487,   356,   456,  -487,  -487,  -487,  -487,  -487,  -487,
    -141,  -100,  -487,  -487,  -487,   -72,  -487,  -487,   254,  -486,
    -364,  -487,  -487,  -276,  -104,   157,  -487,  -487,  -487,  -231,
    -487
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
      45,   200,   474,   345,   502,   370,   256,   217,   420,   105,
     422,   618,   424,   320,   322,   324,   263,    39,   628,   559,
     550,     2,   341,    79,    80,    72,     3,   639,   249,   203,
     560,   669,   331,   518,   342,   713,   437,    55,    62,   395,
     396,   225,   393,   164,   479,   395,   396,    52,   222,     4,
     207,     5,    40,     6,   252,     7,    45,   342,   253,   245,
       8,   214,   149,   164,   343,   511,     9,    40,    27,    56,
      30,   226,    10,   236,   204,   223,   342,   102,   237,   154,
     155,   380,   165,   255,   640,   601,   608,   343,    41,   199,
     413,    73,    74,    65,   412,   205,   413,   238,   426,   640,
     215,   262,   265,    41,    33,   257,   343,    11,   344,   724,
      36,   434,   200,   381,   214,   736,    40,   640,   200,    48,
     725,   726,    37,    75,    60,    65,   641,   200,   200,   200,
     257,   200,   200,   200,   200,    81,   580,   257,   700,    82,
     254,    83,    84,   652,   332,   590,   261,   519,   739,   248,
     257,   239,    41,    61,    38,   316,   317,   318,   200,   321,
     323,   325,   333,   640,   517,   427,    57,    50,   200,   604,
     236,   640,   334,    85,   640,   237,    58,   640,    52,   200,
     200,   240,    12,    51,   350,   241,   349,   557,   428,    13,
     649,   156,   351,    68,   238,   157,   354,   158,    84,    53,
     199,   687,   747,   242,   746,   642,   199,   366,   368,    52,
     243,    54,   622,   244,   352,   199,   199,   199,    69,   199,
     199,   199,   199,   695,   489,   207,   500,   263,   208,   633,
      67,    93,   623,   704,   340,    94,   236,   711,   624,   257,
     582,   237,   258,   429,   200,   259,   199,   350,   239,   717,
     732,    95,    96,    97,    98,   351,   199,   697,   200,   200,
     238,   200,   200,   220,   221,   705,   430,   199,   199,   670,
     626,    62,   435,   733,   719,   737,   627,   352,   240,    65,
     677,   624,   241,    66,   295,   558,   484,   485,   255,   487,
     488,    70,    77,   636,   152,   507,   508,    46,    78,    47,
     242,   383,   384,   577,   505,    88,   506,   477,    90,   624,
     244,   630,    91,   342,   239,    92,   627,   200,   631,   624,
     101,   740,   104,   107,   709,   109,   200,   144,   200,   145,
     146,   149,   199,    56,   152,   163,   212,   236,   218,   219,
     319,   227,   237,   343,   240,   551,   199,   199,   241,   199,
     199,   228,   229,   230,   555,   231,   232,   233,   234,   235,
     337,   238,   255,   339,   214,   207,   242,   512,   348,   668,
     358,   359,   361,   356,   362,   431,   244,   674,   474,   385,
     386,   387,   388,   389,   357,   360,   390,   419,   681,   393,
     379,   394,   395,   396,   397,    12,   482,   398,   415,   399,
     400,   432,   383,   384,   309,   199,   520,   527,   529,   547,
     560,   200,   552,   553,   199,   239,   199,   554,   200,   585,
     586,   336,   592,   593,   594,   596,   405,   406,   407,   408,
     409,   410,   411,   599,   597,   714,   620,   598,   622,   637,
     625,   412,   220,   413,   638,   240,   643,   644,   646,   241,
     738,   416,   673,   634,   675,   676,   363,   364,   680,   217,
     369,   682,   689,   694,   698,   741,   721,   242,   373,   374,
     375,   376,   377,   378,   699,   743,   753,   244,   702,   766,
     774,   706,   387,   388,   707,   756,   759,   775,   780,   781,
     393,   776,   394,   395,   396,   397,   509,    86,   398,   199,
     162,    89,   770,    87,   106,   382,   199,   264,   723,   200,
      20,   246,   346,   383,   384,   556,   683,   749,   478,   224,
     103,   330,   355,   703,   439,   503,   476,   769,     0,     0,
       0,     0,   480,     0,     0,     0,   483,   688,     0,   486,
     762,     0,   412,   763,   413,   493,     0,   493,   236,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,   522,     0,     0,   523,   524,
     525,   526,   238,   528,     0,   530,   531,   532,   533,   534,
     536,   537,   538,   539,   540,   541,   542,   543,   544,   545,
     546,     0,   548,   387,   388,     0,     0,   199,     0,     0,
       0,   393,     0,   394,   395,   396,   397,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     576,     0,     0,     0,   581,     0,   239,   583,   584,     0,
       0,     0,   588,   591,   383,   384,     0,     0,   595,     0,
     407,   408,   409,   410,   411,     0,     0,     0,     0,     0,
       0,   200,     0,   412,     0,   413,   240,   605,     0,     0,
     241,   611,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,   200,     0,     0,     0,     0,     0,   242,   764,
     378,     0,     0,   632,   369,   417,     0,   635,   244,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   742,     0,
     777,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   385,   386,   387,   388,     0,     0,     0,     0,
     383,   384,   393,     0,   394,   395,   396,   397,     0,   760,
     398,     0,   399,   400,     0,     0,   651,     0,     0,   199,
     653,   654,   655,   656,   657,   658,   659,   660,   661,   662,
     663,   664,   665,   666,   667,     0,     0,   671,     0,     0,
     199,   407,   408,   409,   410,   411,     0,     0,   678,     0,
       0,     0,     0,     0,   412,     0,   413,     0,     0,     0,
       0,   663,   667,     0,   365,     0,     0,     0,     0,     0,
       0,     0,   691,   692,     0,   693,   166,     0,   696,     0,
     387,   388,   167,     0,   701,     0,   493,     0,   393,     0,
       0,   395,   396,   397,     0,     0,   398,   168,     0,   712,
       0,     0,     0,     0,     0,     0,   718,     0,     0,     0,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     412,     0,   413,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,   735,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   745,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   476,     0,    40,     0,   750,     0,     0,     0,
       0,   752,     0,     0,     0,   755,     0,   757,   758,     0,
       0,     0,     0,   761,   440,     0,     0,     0,     3,     0,
     441,   442,   443,     0,   444,     0,   266,   267,   268,   269,
      41,     0,     0,     0,     0,   773,     0,   445,   270,   446,
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
       0,     0,     0,     0,     0,     0,   294,     0,   295,   296,
      41,   455,   255,   456,   440,     0,     0,     0,     3,     0,
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
       0,     0,     0,     0,     0,     0,   236,   436,     0,     0,
       0,   237,     0,     0,     0,   279,   280,   281,   282,   283,
     284,   285,   286,   287,    40,   288,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,     0,     0,     0,   289,
     290,   291,     0,     0,     0,     0,   292,   293,     0,     0,
       0,     0,   266,   267,   268,   269,   294,     0,   295,   296,
      41,   455,   255,   748,   270,     0,     0,     0,     0,   236,
     236,     0,     0,     0,   237,   237,   271,     0,     0,     0,
     272,     0,   273,     0,   239,   274,     0,     0,     0,   275,
       0,     0,     0,   238,   238,     0,   169,   170,   171,     0,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   240,   188,   189,   190,   241,     0,
     193,   194,   195,   196,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   242,     0,   277,   278,
       0,     0,     0,     0,     0,     0,   244,   239,   239,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     490,   288,     0,     0,     0,     0,     0,   240,   240,     0,
       0,   241,   241,   418,     0,   289,   290,   291,     0,     0,
       0,     0,   292,   293,     0,   266,   267,   268,   269,   242,
     242,   491,   492,     0,   295,   296,    41,   270,   255,   244,
     244,     0,   236,   236,     0,     0,     0,   237,   237,   271,
       0,     0,     0,   272,     0,   273,     0,     0,   274,     0,
       0,     0,   275,     0,     0,     0,   238,   238,     0,   169,
     170,   171,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,   188,   189,
     190,     0,     0,   193,   194,   195,   196,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,   278,     0,     0,     0,     0,     0,     0,     0,
     239,   239,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   490,   288,     0,     0,     0,     0,     0,
     240,   240,     0,     0,   241,   241,   421,     0,   289,   290,
     291,     0,     0,     0,     0,   292,   293,     0,   266,   267,
     268,   269,   242,   242,     0,   501,     0,   295,   296,    41,
     270,   255,   244,   244,     0,     0,     0,     0,     0,     0,
       0,     0,   271,     0,     0,     0,   272,     0,   273,     0,
       0,   274,     0,     0,     0,   275,     0,     0,     0,     0,
       0,     0,   169,   170,   171,     0,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
       0,   188,   189,   190,     0,     0,   193,   194,   195,   196,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,   278,     0,     0,     0,     0,
       0,     0,     0,   578,     0,     0,     0,     0,   236,   436,
       0,     0,     0,   237,     0,     0,     0,   279,   280,   281,
     282,   283,   284,   285,   286,   287,    40,   288,     0,     0,
       0,     0,   238,     0,     0,     0,     0,     0,     0,     0,
       0,   289,   290,   291,   266,   267,   268,   269,   292,   293,
       0,     0,     0,     0,     0,     0,   270,     0,   294,     0,
     295,   296,    41,   579,   255,     0,     0,     0,   271,     0,
       0,     0,   272,     0,   273,     0,     0,   274,     0,     0,
       0,   275,     0,     0,     0,     0,   239,     0,   169,   170,
     171,     0,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,     0,   188,   189,   190,
       0,     0,   193,   194,   195,   196,   240,     0,   276,     0,
     241,     0,   423,     0,     0,     0,     0,     0,     0,     0,
     277,   278,     0,     0,     0,     0,     0,     0,   242,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   244,     0,
       0,     0,     0,   279,   280,   281,   282,   283,   284,   285,
     286,   287,    40,   288,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   289,   290,   291,
       0,     0,     0,     0,   292,   293,     0,   266,   267,   268,
     269,     0,   513,     0,   294,   514,   295,   296,    41,   270,
     255,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   271,     0,     0,     0,   272,     0,   273,     0,     0,
     274,     0,     0,     0,   275,     0,     0,     0,     0,     0,
       0,   169,   170,   171,     0,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,     0,
     188,   189,   190,     0,     0,   193,   194,   195,   196,     0,
       0,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   277,   278,     0,     0,     0,     0,     0,
       0,     0,   589,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,   279,   280,   281,   282,
     283,   284,   285,   286,   287,    40,   288,     0,     0,     0,
       0,     0,     0,     0,   266,   267,   268,   269,   609,     0,
     289,   290,   291,     0,     0,     0,   270,   292,   293,     0,
       0,     0,     0,     0,     0,     0,     0,   294,   271,   295,
     296,    41,   272,   255,   273,     0,     0,   274,     0,     0,
       0,   275,     0,     0,     0,     0,     0,     0,   169,   170,
     171,     0,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,     0,   188,   189,   190,
       0,     0,   193,   194,   195,   196,     0,     0,   276,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,   278,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   279,   280,   281,   282,   283,   284,   285,
     286,   287,    40,   288,     0,     0,     0,     0,     0,     0,
       0,   266,   267,   268,   269,   610,     0,   289,   290,   291,
       0,     0,     0,   270,   292,   293,     0,     0,     0,     0,
       0,     0,     0,     0,   294,   271,   295,   296,    41,   272,
     255,   273,     0,     0,   274,     0,     0,     0,   275,     0,
       0,     0,     0,     0,     0,   169,   170,   171,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,   188,   189,   190,     0,     0,   193,
     194,   195,   196,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,   278,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     279,   280,   281,   282,   283,   284,   285,   286,   287,    40,
     288,     0,     0,     0,     0,     0,     0,     0,   266,   267,
     268,   269,     0,     0,   289,   290,   291,     0,     0,     0,
     270,   292,   293,   236,     0,     0,     0,     0,   237,     0,
     335,   294,   271,   295,   296,    41,   272,   255,   273,     0,
       0,   274,     0,     0,     0,   275,     0,   238,     0,     0,
       0,     0,   169,   170,   171,     0,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
       0,   188,   189,   190,     0,     0,   193,   194,   195,   196,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,   278,     0,     0,     0,     0,
       0,   239,     0,     0,     0,     0,     0,     0,   236,   436,
       0,     0,     0,   237,     0,     0,     0,   279,   280,   281,
     282,   283,   284,   285,   286,   287,    40,   288,     0,     0,
       0,   240,   238,     0,     0,   241,     0,   425,     0,     0,
       0,   289,   290,   291,   266,   267,   268,   269,   292,   293,
       0,   535,     0,   242,     0,     0,   270,     0,   294,     0,
     295,   296,    41,   244,   255,     0,     0,     0,   271,     0,
       0,     0,   272,     0,   273,     0,     0,   274,     0,     0,
       0,   275,     0,     0,     0,     0,   239,     0,   169,   170,
     171,     0,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,     0,   188,   189,   190,
       0,     0,   193,   194,   195,   196,   240,     0,   276,     0,
     241,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,   278,     0,     0,     0,     0,     0,     0,   242,     0,
     481,     0,     0,     0,     0,     0,     0,     0,   244,     0,
       0,     0,     0,   279,   280,   281,   282,   283,   284,   285,
     286,   287,    40,   288,     0,     0,     0,     0,     0,     0,
       0,   266,   267,   268,   269,     0,     0,   289,   290,   291,
       0,     0,     0,   270,   292,   293,   236,     0,     0,     0,
       0,   237,     0,     0,   294,   271,   295,   296,    41,   272,
     255,   273,     0,     0,   274,     0,     0,     0,   275,     0,
     238,     0,     0,     0,     0,   169,   170,   171,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,   188,   189,   190,     0,     0,   193,
     194,   195,   196,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,   278,     0,
       0,     0,     0,     0,   239,     0,     0,     0,     0,     0,
       0,   236,   236,     0,     0,     0,   237,   237,     0,     0,
     279,   280,   281,   282,   283,   284,   285,   286,   287,    40,
     288,     0,     0,     0,   240,   238,   238,     0,   241,     0,
     613,     0,     0,     0,   289,   290,   291,   266,   267,   268,
     269,   292,   293,     0,     0,     0,   242,     0,     0,   270,
       0,   294,   549,   295,   296,    41,   244,   255,     0,     0,
       0,   271,     0,     0,     0,   272,     0,   273,     0,     0,
     274,     0,     0,     0,   275,     0,     0,   587,     0,   239,
     239,   169,   170,   171,     0,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,     0,
     188,   189,   190,     0,     0,   193,   194,   195,   196,   240,
     240,   276,     0,   241,   241,   614,   616,     0,     0,     0,
       0,     0,     0,   277,   278,     0,     0,     0,     0,     0,
       0,   242,   242,     0,     0,     0,     0,     0,     0,     0,
       0,   244,   244,     0,     0,     0,   279,   280,   281,   282,
     283,   284,   285,   286,   287,    40,   288,     0,     0,     0,
       0,     0,     0,     0,   266,   267,   268,   269,     0,     0,
     289,   290,   291,     0,     0,     0,   270,   292,   293,   236,
       0,     0,     0,     0,   237,     0,     0,   294,   271,   295,
     296,    41,   272,   255,   273,     0,     0,   274,     0,     0,
       0,   275,     0,   238,     0,     0,     0,     0,   169,   170,
     171,     0,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,     0,   188,   189,   190,
       0,     0,   193,   194,   195,   196,     0,     0,   276,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,   278,     0,     0,     0,     0,     0,   239,     0,     0,
       0,     0,     0,     0,   236,   236,     0,     0,     0,   237,
     237,     0,     0,   279,   280,   281,   282,   283,   284,   285,
     286,   287,    40,   288,     0,     0,     0,   240,   238,   238,
       0,   241,     0,   617,     0,     0,     0,   289,   290,   291,
     266,   267,   268,   269,   292,   293,     0,     0,     0,   242,
       0,     0,   270,     0,   294,   607,   295,   296,    41,   244,
     255,     0,     0,     0,   271,     0,     0,     0,   272,     0,
     273,     0,     0,   274,     0,     0,     0,   275,     0,     0,
       0,     0,   239,   239,   169,   170,   171,     0,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,     0,   188,   189,   190,     0,     0,   193,   194,
     195,   196,   240,   240,   276,     0,   241,   241,   650,   710,
       0,     0,     0,     0,     0,     0,   277,   278,     0,     0,
       0,     0,     0,     0,   242,   242,     0,     0,     0,     0,
       0,   236,     0,     0,   244,   244,   237,     0,     0,   279,
     280,   281,   282,   283,   284,   285,   286,   287,    40,   288,
       0,     0,     0,     0,     0,   238,     0,     0,     0,     0,
       0,     0,     0,   289,   290,   291,   266,   267,   268,   269,
     292,   293,     0,     0,     0,     0,     0,     0,   270,     0,
     294,   621,   295,   296,    41,     0,   255,     0,     0,     0,
     271,     0,     0,     0,   272,     0,   273,     0,     0,   274,
       0,     0,     0,   275,     0,     0,     0,     0,     0,   239,
     169,   170,   171,     0,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,   188,
     189,   190,     0,     0,   193,   194,   195,   196,     0,   240,
     276,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,   277,   278,     0,     0,     0,     0,     0,     0,
       0,   242,     0,     0,   731,     0,     0,   236,     0,     0,
       0,   244,   237,     0,     0,   279,   280,   281,   282,   283,
     284,   285,   286,   287,    40,   288,     0,     0,     0,     0,
       0,   238,     0,     0,     0,     0,     0,     0,     0,   289,
     290,   291,   266,   267,   268,   269,   292,   293,     0,     0,
       0,     0,     0,     0,   270,     0,   294,   629,   295,   296,
      41,     0,   255,     0,     0,     0,   271,     0,     0,     0,
     272,     0,   273,     0,     0,   274,     0,     0,     0,   275,
       0,     0,     0,     0,     0,   239,   169,   170,   171,     0,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,     0,   188,   189,   190,     0,     0,
     193,   194,   195,   196,     0,   240,   276,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,   277,   278,
       0,     0,     0,     0,     0,     0,     0,   242,     0,     0,
     771,     0,     0,   684,     0,     0,     0,   244,     0,     0,
       0,   279,   280,   281,   282,   283,   284,   285,   286,   287,
      40,   288,     0,     0,     0,     0,     0,     0,     0,   266,
     267,   268,   269,     0,     0,   289,   290,   291,     0,     0,
       0,   270,   292,   293,     0,     0,     0,     0,     0,     0,
       0,     0,   294,   271,   295,   296,    41,   272,   255,   273,
       0,     0,   274,     0,     0,     0,   275,     0,     0,     0,
       0,     0,     0,   169,   170,   171,     0,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,     0,   188,   189,   190,     0,     0,   193,   194,   195,
     196,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     685,     0,     0,     0,     0,     0,     0,     0,   279,   280,
     281,   282,   283,   284,   285,   286,   287,    40,   288,     0,
       0,     0,     0,     0,     0,     0,   266,   267,   268,   269,
       0,     0,   289,   290,   291,     0,     0,     0,   270,   292,
     293,     0,     0,     0,     0,     0,     0,     0,     0,   294,
     271,   295,   296,    41,   272,   255,   273,     0,     0,   274,
       0,     0,     0,   275,     0,     0,     0,     0,     0,     0,
     169,   170,   171,     0,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,   188,
     189,   190,     0,     0,   193,   194,   195,   196,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,   278,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   236,     0,     0,     0,
       0,   237,     0,     0,     0,   279,   280,   281,   282,   283,
     284,   285,   286,   287,    40,   288,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,     0,     0,     0,   289,
     290,   291,   266,   267,   268,   269,   292,   293,     0,     0,
       0,     0,     0,     0,   270,     0,   294,   716,   295,   296,
      41,     0,   255,     0,     0,     0,   271,     0,     0,     0,
     272,     0,   273,     0,     0,   274,     0,     0,     0,   275,
       0,     0,     0,     0,   239,     0,   169,   170,   171,     0,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,     0,   188,   189,   190,     0,     0,
     193,   194,   195,   196,   240,     0,   276,     0,   241,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,   278,
       0,     0,     0,     0,     0,     0,   242,     0,     0,   782,
       0,     0,     0,     0,     0,     0,   244,     0,     0,     0,
       0,   279,   280,   281,   282,   283,   284,   285,   286,   287,
      40,   288,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   289,   290,   291,   266,   267,
     268,   269,   292,   293,     0,     0,     0,     0,     0,     0,
     270,     0,   294,   734,   295,   296,    41,     0,   255,     0,
       0,     0,   271,     0,     0,     0,   272,     0,   273,     0,
       0,   274,     0,     0,     0,   275,     0,     0,     0,     0,
       0,     0,   169,   170,   171,     0,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
       0,   188,   189,   190,     0,     0,   193,   194,   195,   196,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,   278,     0,     0,     0,     0,
       0,     0,     0,   744,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   279,   280,   281,
     282,   283,   284,   285,   286,   287,    40,   288,     0,     0,
       0,     0,     0,     0,     0,   266,   267,   268,   269,   751,
       0,   289,   290,   291,     0,     0,     0,   270,   292,   293,
       0,     0,     0,     0,     0,     0,     0,     0,   294,   271,
     295,   296,    41,   272,   255,   273,     0,     0,   274,     0,
       0,     0,   275,     0,     0,     0,     0,     0,     0,   169,
     170,   171,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,   188,   189,
     190,     0,     0,   193,   194,   195,   196,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,   278,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    40,   288,     0,     0,     0,     0,     0,
       0,     0,   266,   267,   268,   269,   772,     0,   289,   290,
     291,     0,     0,     0,   270,   292,   293,     0,     0,     0,
       0,     0,     0,     0,     0,   294,   271,   295,   296,    41,
     272,   255,   273,     0,     0,   274,     0,     0,     0,   275,
       0,     0,     0,     0,     0,     0,   169,   170,   171,     0,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,     0,   188,   189,   190,     0,     0,
     193,   194,   195,   196,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,   278,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   279,   280,   281,   282,   283,   284,   285,   286,   287,
      40,   288,     0,     0,     0,     0,     0,     0,     0,   266,
     267,   268,   269,     0,     0,   289,   290,   291,     0,     0,
       0,   270,   292,   293,     0,     0,     0,     0,     0,     0,
       0,     0,   294,   271,   295,   296,    41,   272,   255,   273,
       0,     0,   274,     0,     0,     0,   275,     0,     0,     0,
       0,     0,     0,   169,   170,   171,     0,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,     0,   188,   189,   190,     0,     0,   193,   194,   195,
     196,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   279,   280,
     281,   282,   283,   284,   285,   286,   287,    40,   288,     0,
       0,     0,     0,   367,   383,   384,     0,     0,     0,     0,
       0,     0,   289,   290,   291,   166,     0,     0,     0,   292,
     293,   167,     0,     0,     0,     0,     0,     0,     0,   294,
       0,   295,   296,    41,     0,   255,   168,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
       0,     0,   385,   386,   387,   388,   389,     0,     0,   390,
     391,   392,   393,     0,   394,   395,   396,   397,   383,   384,
     398,     0,   399,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,   401,     0,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,     0,     0,     0,     0,
       0,     0,     0,     0,   412,     0,   413,     0,     0,     0,
       0,     0,     0,     0,   255,     0,     0,     0,     0,    41,
       0,     0,     0,     0,     0,   383,   384,     0,   561,   562,
     563,   564,   565,   566,   567,   568,   385,   386,   387,   388,
     389,   569,   570,   390,   391,   392,   393,   571,   394,   395,
     396,   397,   383,   384,   398,   572,   399,   400,   573,   574,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   575,   401,     0,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
       0,     0,     0,     0,     0,     0,     0,     0,   412,     0,
     413,     0,     0,     0,     0,   387,   388,   606,     0,     0,
       0,     0,     0,   393,     0,   394,   395,   396,   397,     0,
       0,   398,   561,   562,   563,   564,   565,   566,   567,   568,
     385,   386,   387,   388,   389,   569,   570,   390,   391,   392,
     393,   571,   394,   395,   396,   397,   383,   384,   398,   572,
     399,   400,   573,   574,   409,   410,   411,     0,     0,     0,
       0,     0,     0,     0,     0,   412,     0,   413,     0,     0,
       0,   575,   401,     0,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,     0,     0,     0,     0,     0,     0,
       0,     0,   412,     0,   413,     0,     0,     0,     0,     0,
       0,   672,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,   563,   564,
     565,   566,   567,   568,   385,   386,   387,   388,   389,   569,
     570,   390,   391,   392,   393,   571,   394,   395,   396,   397,
     383,   384,   398,   572,   399,   400,   573,   574,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   575,   401,     0,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,     0,     0,
       0,     0,     0,     0,     0,     0,   412,     0,   413,     0,
       0,     0,     0,     0,     0,   679,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,   563,   564,   565,   566,   567,   568,   385,   386,
     387,   388,   389,   569,   570,   390,   391,   392,   393,   571,
     394,   395,   396,   397,   383,   384,   398,   572,   399,   400,
     573,   574,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   575,
     401,     0,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,     0,     0,     0,     0,     0,     0,     0,     0,
     412,     0,   413,     0,     0,     0,     0,     0,     0,   686,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,   563,   564,   565,   566,
     567,   568,   385,   386,   387,   388,   389,   569,   570,   390,
     391,   392,   393,   571,   394,   395,   396,   397,   383,   384,
     398,   572,   399,   400,   573,   574,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   575,   401,     0,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,     0,     0,     0,     0,
       0,     0,     0,     0,   412,     0,   413,     0,     0,     0,
       0,     0,     0,   720,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   383,   384,     0,     0,     0,   561,   562,
     563,   564,   565,   566,   567,   568,   385,   386,   387,   388,
     389,   569,   570,   390,   391,   392,   393,   571,   394,   395,
     396,   397,     0,     0,   398,   572,   399,   400,   573,   574,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   575,   401,     0,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
       0,     0,     0,     0,     0,   383,   384,     0,   412,     0,
     413,   385,   386,   387,   388,   389,     0,   722,   390,   391,
     392,   393,     0,   394,   395,   396,   397,     0,     0,   398,
       0,   399,   400,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   401,     0,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,     0,     0,     0,     0,     0,
     383,   384,     0,   412,     0,   413,     0,     0,     0,     0,
       0,     0,   414,   385,   386,   387,   388,   389,     0,     0,
     390,   391,   392,   393,     0,   394,   395,   396,   397,     0,
       0,   398,     0,   399,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   401,     0,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,     0,   383,   384,
       0,     0,     0,     0,     0,   412,     0,   413,   385,   386,
     387,   388,   389,     0,   767,   390,   391,   392,   393,     0,
     394,   395,   396,   397,     0,     0,   398,     0,   399,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     401,     0,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,     0,     0,     0,     0,   383,   384,     0,     0,
     412,     0,   413,     0,     0,   510,   385,   386,   387,   388,
     389,     0,     0,   390,   391,   392,   393,     0,   394,   395,
     396,   397,     0,     0,   398,     0,   399,   400,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   401,     0,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
       0,     0,     0,     0,   383,   384,     0,     0,   412,     0,
     413,     0,     0,   612,   385,   386,   387,   388,   389,     0,
       0,   390,   391,   392,   393,     0,   394,   395,   396,   397,
       0,     0,   398,     0,   399,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   401,     0,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,     0,     0,
       0,     0,   383,   384,     0,     0,   412,     0,   413,     0,
       0,   615,   385,   386,   387,   388,   389,     0,     0,   390,
     391,   392,   393,     0,   394,   395,   396,   397,     0,     0,
     398,     0,   399,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   401,     0,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,     0,     0,     0,     0,
     383,   384,     0,     0,   412,     0,   413,     0,     0,   690,
     385,   386,   387,   388,   389,     0,     0,   390,   391,   392,
     393,     0,   394,   395,   396,   397,     0,     0,   398,     0,
     399,   400,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   401,     0,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,     0,     0,     0,     0,   383,   384,
       0,     0,   412,     0,   413,     0,     0,   754,   385,   386,
     387,   388,   389,     0,     0,   390,   391,   392,   393,     0,
     394,   395,   396,   397,     0,     0,   398,     0,   399,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     401,     0,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,     0,     0,     0,     0,     0,     0,     0,     0,
     412,     0,   413,     0,     0,   765,   385,   386,   387,   388,
     389,     0,     0,   390,   391,   392,   393,     0,   394,   395,
     396,   397,     0,     0,   398,     0,   399,   400,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     166,     0,     0,     0,     0,     0,   167,     0,   401,     0,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
       0,   168,     0,     0,     0,     0,     0,     0,   412,     0,
     413,     0,     0,   778,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   166,     0,     0,     0,     0,
       0,   167,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   168,     0,     0,   279,
     280,   281,     0,     0,     0,     0,     0,     0,    40,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
       0,     0,     0,     0,    41,     0,     0,     0,     0,     0,
     166,     0,     0,     0,     0,     0,   167,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   168,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   146,     0,   383,   384,    41,
       0,     0,     0,     0,     0,   166,     0,     0,     0,     0,
       0,   167,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   168,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    40,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
       0,     0,   383,   384,    41,   385,   386,   387,   388,   389,
       0,     0,   390,   391,   392,   393,     0,   394,   395,   396,
     397,     0,     0,   398,     0,   399,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   326,     0,     0,     0,   401,     0,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   411,     0,
       0,     0,     0,     0,     0,     0,     0,   412,     0,   413,
     433,   383,   384,     0,     0,     0,     0,     0,     0,    41,
     385,   386,   387,   388,   389,     0,     0,   390,   391,   392,
     393,     0,   394,   395,   396,   397,     0,     0,   398,     0,
     399,   400,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   401,     0,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,     0,     0,     0,     0,     0,     0,
       0,     0,   412,     0,   413,   645,   383,   384,     0,   385,
     386,   387,   388,   389,     0,     0,   390,   391,   392,   393,
       0,   394,   395,   396,   397,     0,     0,   398,     0,   399,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   401,     0,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   411,     0,     0,     0,     0,     0,     0,     0,
       0,   412,     0,   413,   648,     0,   561,   562,   563,   564,
     565,   566,   567,   568,   385,   386,   387,   388,   389,   569,
     570,   390,   391,   392,   393,   571,   394,   395,   396,   397,
    -193,     0,   398,   572,   399,   400,   573,   574,   383,   384,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   575,   401,     0,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,     0,     0,
       0,     0,     0,     0,     0,     0,   412,     0,   413,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   383,   384,     0,     0,     0,   561,   562,
     563,   564,   565,   566,   567,   568,   385,   386,   387,   388,
     389,   569,   570,   390,   391,   392,   393,   602,   394,   395,
     396,   397,     0,     0,   398,   572,   399,   400,   573,   574,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   603,   401,     0,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     383,   384,     0,     0,     0,     0,     0,     0,   412,     0,
     413,   385,   386,   387,   388,   389,     0,     0,   390,   391,
     392,   393,     0,   394,   395,   396,   397,     0,     0,   398,
       0,   399,   400,     0,     0,   504,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   401,     0,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   383,   384,     0,     0,     0,
       0,     0,     0,   412,     0,   413,     0,     0,   385,   386,
     387,   388,   389,     0,     0,   390,   391,   392,   393,     0,
     394,   395,   396,   397,     0,     0,   398,     0,   399,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     401,   647,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   383,   384,     0,     0,     0,     0,     0,     0,
     412,     0,   413,   385,   386,   387,   388,   389,     0,     0,
     390,   391,   392,   393,     0,   394,   395,   396,   397,     0,
       0,   398,     0,   399,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   708,     0,     0,   401,     0,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   383,   384,     0,
       0,     0,     0,     0,     0,   412,     0,   413,     0,     0,
     385,   386,   387,   388,   389,     0,     0,   390,   391,   392,
     393,     0,   394,   395,   396,   397,     0,     0,   398,     0,
     399,   400,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   401,     0,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,   383,   384,     0,     0,     0,     0,
       0,     0,   412,     0,   413,   385,   386,   387,   388,   389,
       0,     0,   390,   391,   392,   393,     0,   394,   395,   396,
     397,     0,     0,   398,     0,   399,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   383,
     384,     0,     0,     0,     0,     0,     0,   412,     0,   413,
       0,     0,   385,   386,   387,   388,   389,     0,     0,   390,
     391,   392,   393,     0,   394,   395,   396,   397,     0,     0,
     398,     0,   399,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   404,   405,
     406,   407,   408,   409,   410,   411,   383,   384,     0,     0,
       0,     0,     0,     0,   412,     0,   413,   385,   386,   387,
     388,   389,     0,     0,   390,   391,   392,   393,     0,   394,
     395,   396,   397,     0,     0,   398,     0,   399,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   405,   406,   407,   408,   409,   410,
     411,     0,     0,     0,     0,     0,     0,     0,     0,   412,
       0,   413,     0,     0,   385,   386,     0,     0,   389,     0,
       0,   390,   391,   392,   393,     0,   394,   395,   396,   397,
       0,     0,   398,     0,   399,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   401,     0,   402,   403,
     404,   405,   406,     0,     0,     0,   410,   411,     0,     0,
       0,     0,     0,     0,     0,     0,   412,     0,   413,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   128,   129,   130,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,     0,     0,   142,   143
};

static const yytype_int16 yycheck[] =
{
      12,   101,   347,   251,   368,   281,   213,   148,   320,    67,
     322,    18,   324,   231,   232,   233,   219,    32,    18,   436,
     415,     0,   249,    13,    14,    33,     5,   513,    45,   101,
     113,    18,    31,    17,   108,   121,   345,    20,   131,   110,
     111,   138,   107,   131,   353,   110,   111,   137,   137,    28,
     161,    30,   131,    32,   164,    34,    68,   108,   168,   170,
      39,   131,   131,   131,   138,   144,    45,   131,   131,    52,
     125,   168,    51,    31,   164,   164,   108,   170,    36,    13,
      14,   131,   170,   169,   137,   168,   481,   138,   167,   101,
     161,    99,   100,   167,   159,   107,   161,    55,   140,   137,
     170,   170,   170,   167,   131,   137,   138,    86,   140,    10,
     131,   338,   212,   163,   131,   168,   131,   137,   218,    56,
      21,    22,   169,   131,   140,   167,   164,   227,   228,   229,
     137,   231,   232,   233,   234,   125,   445,   137,   624,   129,
     212,   131,   132,   560,   143,   454,   218,   131,   168,   207,
     137,   109,   167,   169,   131,   227,   228,   229,   258,   231,
     232,   233,   161,   137,   382,   145,   149,   131,   268,   478,
      31,   137,   171,   163,   137,    36,   159,   137,   137,   279,
     280,   139,   161,   138,   108,   143,   258,   435,   168,   168,
     164,   125,   116,   137,    55,   129,   268,   131,   132,   140,
     212,   164,   168,   161,   164,   517,   218,   279,   280,   137,
     168,   137,   137,   171,   138,   227,   228,   229,   162,   231,
     232,   233,   234,   618,   365,   161,   367,   430,   164,   505,
     163,    61,   162,   628,   162,    65,    31,   162,   168,   137,
     447,    36,   140,   145,   344,   143,   258,   108,   109,   644,
     145,    81,    82,    83,    84,   116,   268,   621,   358,   359,
      55,   361,   362,   133,   134,   629,   168,   279,   280,   578,
     162,   131,   344,   168,   669,   162,   168,   138,   139,   167,
     589,   168,   143,   131,   165,   166,   358,   359,   169,   361,
     362,   131,   169,   511,   133,   134,   135,     4,   169,     6,
     161,    19,    20,   444,   168,   125,   170,   168,   131,   168,
     171,   170,   131,   108,   109,   131,   168,   417,   170,   168,
     138,   170,   131,   140,   636,   169,   426,    33,   428,    33,
     163,   131,   344,    52,   133,   169,   140,    31,   140,   168,
     131,   144,    36,   138,   139,   417,   358,   359,   143,   361,
     362,   144,   144,   163,   426,   144,   144,   144,   144,   144,
      42,    55,   169,   168,   131,   161,   161,   379,   131,   576,
     144,   144,   144,   163,   144,   143,   171,   584,   723,    97,
      98,    99,   100,   101,   163,   163,   104,   164,   595,   107,
     166,   109,   110,   111,   112,   161,   131,   115,   163,   117,
     118,   162,    19,    20,   225,   417,   131,   131,   131,   131,
     113,   511,   145,   145,   426,   109,   428,   145,   518,   168,
     125,   242,    35,   168,   168,   168,   144,   145,   146,   147,
     148,   149,   150,    18,   168,   642,   162,   168,   137,   511,
     137,   159,   133,   161,   131,   139,   518,   163,   131,   143,
     698,   145,    12,   170,   140,   125,   277,   278,   169,   600,
     281,    38,   131,   163,   131,   145,   673,   161,   289,   290,
     291,   292,   293,   294,   162,   164,   131,   171,   162,   145,
      48,   162,    99,   100,   162,   162,   162,   162,   162,   162,
     107,   170,   109,   110,   111,   112,   372,    51,   115,   511,
      90,    54,   763,    52,    68,   303,   518,   222,   680,   609,
       1,   206,   253,    19,    20,   428,   600,   724,   349,   163,
      64,   235,   268,   627,   345,   368,   347,   758,    -1,    -1,
      -1,    -1,   353,    -1,    -1,    -1,   357,   609,    -1,   360,
     747,    -1,   159,   750,   161,   366,    -1,   368,    31,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   385,   386,    -1,    -1,   389,   390,
     391,   392,    55,   394,    -1,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,    -1,   413,    99,   100,    -1,    -1,   609,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     441,    -1,    -1,    -1,   445,    -1,   109,   448,   449,    -1,
      -1,    -1,   453,   454,    19,    20,    -1,    -1,   459,    -1,
     146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,
      -1,   751,    -1,   159,    -1,   161,   139,   478,    -1,    -1,
     143,   482,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   492,   772,    -1,    -1,    -1,    -1,    -1,   161,   751,
     501,    -1,    -1,   504,   505,   168,    -1,   508,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   710,    -1,
     772,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,    -1,    -1,    -1,    -1,
      19,    20,   107,    -1,   109,   110,   111,   112,    -1,   741,
     115,    -1,   117,   118,    -1,    -1,   557,    -1,    -1,   751,
     561,   562,   563,   564,   565,   566,   567,   568,   569,   570,
     571,   572,   573,   574,   575,    -1,    -1,   578,    -1,    -1,
     772,   146,   147,   148,   149,   150,    -1,    -1,   589,    -1,
      -1,    -1,    -1,    -1,   159,    -1,   161,    -1,    -1,    -1,
      -1,   602,   603,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   613,   614,    -1,   616,    23,    -1,   619,    -1,
      99,   100,    29,    -1,   625,    -1,   627,    -1,   107,    -1,
      -1,   110,   111,   112,    -1,    -1,   115,    44,    -1,   640,
      -1,    -1,    -1,    -1,    -1,    -1,   647,    -1,    -1,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
     159,    -1,   161,   684,   685,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   694,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   713,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   723,    -1,   131,    -1,   727,    -1,    -1,    -1,
      -1,   732,    -1,    -1,    -1,   736,    -1,   738,   739,    -1,
      -1,    -1,    -1,   744,     1,    -1,    -1,    -1,     5,    -1,
       7,     8,     9,    -1,    11,    -1,    13,    14,    15,    16,
     167,    -1,    -1,    -1,    -1,   766,    -1,    24,    25,    26,
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
      -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,   166,
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
      -1,    -1,    -1,    -1,    -1,    -1,    31,   114,    -1,    -1,
      -1,    36,    -1,    -1,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,    -1,    -1,    -1,    -1,   153,   154,    -1,    -1,
      -1,    -1,    13,    14,    15,    16,   163,    -1,   165,   166,
     167,   168,   169,   170,    25,    -1,    -1,    -1,    -1,    31,
      31,    -1,    -1,    -1,    36,    36,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,   109,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    55,    55,    -1,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,   139,    76,    77,    78,   143,    -1,
      81,    82,    83,    84,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,   171,   109,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,   139,   139,    -1,
      -1,   143,   143,   145,    -1,   146,   147,   148,    -1,    -1,
      -1,    -1,   153,   154,    -1,    13,    14,    15,    16,   161,
     161,   162,   163,    -1,   165,   166,   167,    25,   169,   171,
     171,    -1,    31,    31,    -1,    -1,    -1,    36,    36,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    55,    55,    -1,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
     139,   139,    -1,    -1,   143,   143,   145,    -1,   146,   147,
     148,    -1,    -1,    -1,    -1,   153,   154,    -1,    13,    14,
      15,    16,   161,   161,    -1,   163,    -1,   165,   166,   167,
      25,   169,   171,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    31,   114,
      -1,    -1,    -1,    36,    -1,    -1,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,    -1,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,   148,    13,    14,    15,    16,   153,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,   163,    -1,
     165,   166,   167,   168,   169,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,   109,    -1,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    82,    83,    84,   139,    -1,    87,    -1,
     143,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
      -1,    -1,    -1,    -1,   153,   154,    -1,    13,    14,    15,
      16,    -1,   161,    -1,   163,   164,   165,   166,   167,    25,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    14,    15,    16,    17,    -1,
     146,   147,   148,    -1,    -1,    -1,    25,   153,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    37,   165,
     166,   167,    41,   169,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    82,    83,    84,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    14,    15,    16,   144,    -1,   146,   147,   148,
      -1,    -1,    -1,    25,   153,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   163,    37,   165,   166,   167,    41,
     169,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,
      15,    16,    -1,    -1,   146,   147,   148,    -1,    -1,    -1,
      25,   153,   154,    31,    -1,    -1,    -1,    -1,    36,    -1,
     162,   163,    37,   165,   166,   167,    41,   169,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    55,    -1,    -1,
      -1,    -1,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    31,   114,
      -1,    -1,    -1,    36,    -1,    -1,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,    -1,    -1,
      -1,   139,    55,    -1,    -1,   143,    -1,   145,    -1,    -1,
      -1,   146,   147,   148,    13,    14,    15,    16,   153,   154,
      -1,    20,    -1,   161,    -1,    -1,    25,    -1,   163,    -1,
     165,   166,   167,   171,   169,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,   109,    -1,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    82,    83,    84,   139,    -1,    87,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    14,    15,    16,    -1,    -1,   146,   147,   148,
      -1,    -1,    -1,    25,   153,   154,    31,    -1,    -1,    -1,
      -1,    36,    -1,    -1,   163,    37,   165,   166,   167,    41,
     169,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      55,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    31,    -1,    -1,    -1,    36,    36,    -1,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,    -1,    -1,    -1,   139,    55,    55,    -1,   143,    -1,
     145,    -1,    -1,    -1,   146,   147,   148,    13,    14,    15,
      16,   153,   154,    -1,    -1,    -1,   161,    -1,    -1,    25,
      -1,   163,   164,   165,   166,   167,   171,   169,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,   109,
     109,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    -1,    -1,    81,    82,    83,    84,   139,
     139,    87,    -1,   143,   143,   145,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      -1,   161,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   171,   171,    -1,    -1,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,
     146,   147,   148,    -1,    -1,    -1,    25,   153,   154,    31,
      -1,    -1,    -1,    -1,    36,    -1,    -1,   163,    37,   165,
     166,   167,    41,   169,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    55,    -1,    -1,    -1,    -1,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    82,    83,    84,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    31,    -1,    -1,    -1,    36,
      36,    -1,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,    -1,    -1,    -1,   139,    55,    55,
      -1,   143,    -1,   145,    -1,    -1,    -1,   146,   147,   148,
      13,    14,    15,    16,   153,   154,    -1,    -1,    -1,   161,
      -1,    -1,    25,    -1,   163,   164,   165,   166,   167,   171,
     169,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,   109,   109,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    76,    77,    78,    -1,    -1,    81,    82,
      83,    84,   139,   139,    87,    -1,   143,   143,   145,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,    -1,    -1,
      -1,    -1,    -1,    -1,   161,   161,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,   171,   171,    36,    -1,    -1,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,   148,    13,    14,    15,    16,
     153,   154,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
     163,   164,   165,   166,   167,    -1,   169,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,   109,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,    -1,   139,
      87,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,    -1,   164,    -1,    -1,    31,    -1,    -1,
      -1,   171,    36,    -1,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,    13,    14,    15,    16,   153,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,   163,   164,   165,   166,
     167,    -1,   169,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,   109,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    82,    83,    84,    -1,   139,    87,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,
     164,    -1,    -1,   114,    -1,    -1,    -1,   171,    -1,    -1,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      14,    15,    16,    -1,    -1,   146,   147,   148,    -1,    -1,
      -1,    25,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    37,   165,   166,   167,    41,   169,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    77,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,    14,    15,    16,
      -1,    -1,   146,   147,   148,    -1,    -1,    -1,    25,   153,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      37,   165,   166,   167,    41,   169,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,    -1,    -1,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,    13,    14,    15,    16,   153,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,   163,   164,   165,   166,
     167,    -1,   169,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,   109,    -1,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    82,    83,    84,   139,    -1,    87,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,   164,
      -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,   147,   148,    13,    14,
      15,    16,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,   163,   164,   165,   166,   167,    -1,   169,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    14,    15,    16,    17,
      -1,   146,   147,   148,    -1,    -1,    -1,    25,   153,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    37,
     165,   166,   167,    41,   169,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    13,    14,    15,    16,    17,    -1,   146,   147,
     148,    -1,    -1,    -1,    25,   153,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   163,    37,   165,   166,   167,
      41,   169,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      14,    15,    16,    -1,    -1,   146,   147,   148,    -1,    -1,
      -1,    25,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    37,   165,   166,   167,    41,   169,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    77,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,    -1,
      -1,    -1,    -1,    11,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,    23,    -1,    -1,    -1,   153,
     154,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      -1,   165,   166,   167,    -1,   169,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      -1,    -1,    97,    98,    99,   100,   101,    -1,    -1,   104,
     105,   106,   107,    -1,   109,   110,   111,   112,    19,    20,
     115,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   139,    -1,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,    19,    20,   115,   116,   117,   118,   119,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
     161,    -1,    -1,    -1,    -1,    99,   100,   168,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,   112,    -1,
      -1,   115,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    19,    20,   115,   116,
     117,   118,   119,   120,   148,   149,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,   161,    -1,    -1,
      -1,   138,   139,    -1,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,   161,    -1,    -1,    -1,    -1,    -1,
      -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      19,    20,   115,   116,   117,   118,   119,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,   161,    -1,
      -1,    -1,    -1,    -1,    -1,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    19,    20,   115,   116,   117,   118,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,    -1,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,    19,    20,
     115,   116,   117,   118,   119,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,    -1,    -1,   115,   116,   117,   118,   119,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
      -1,    -1,    -1,    -1,    -1,    19,    20,    -1,   159,    -1,
     161,    97,    98,    99,   100,   101,    -1,   168,   104,   105,
     106,   107,    -1,   109,   110,   111,   112,    -1,    -1,   115,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,
      19,    20,    -1,   159,    -1,   161,    -1,    -1,    -1,    -1,
      -1,    -1,   168,    97,    98,    99,   100,   101,    -1,    -1,
     104,   105,   106,   107,    -1,   109,   110,   111,   112,    -1,
      -1,   115,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,    -1,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,    -1,    19,    20,
      -1,    -1,    -1,    -1,    -1,   159,    -1,   161,    97,    98,
      99,   100,   101,    -1,   168,   104,   105,   106,   107,    -1,
     109,   110,   111,   112,    -1,    -1,   115,    -1,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    -1,    -1,    -1,    -1,    19,    20,    -1,    -1,
     159,    -1,   161,    -1,    -1,   164,    97,    98,    99,   100,
     101,    -1,    -1,   104,   105,   106,   107,    -1,   109,   110,
     111,   112,    -1,    -1,   115,    -1,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
      -1,    -1,    -1,    -1,    19,    20,    -1,    -1,   159,    -1,
     161,    -1,    -1,   164,    97,    98,    99,   100,   101,    -1,
      -1,   104,   105,   106,   107,    -1,   109,   110,   111,   112,
      -1,    -1,   115,    -1,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,    -1,    -1,
      -1,    -1,    19,    20,    -1,    -1,   159,    -1,   161,    -1,
      -1,   164,    97,    98,    99,   100,   101,    -1,    -1,   104,
     105,   106,   107,    -1,   109,   110,   111,   112,    -1,    -1,
     115,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,
      19,    20,    -1,    -1,   159,    -1,   161,    -1,    -1,   164,
      97,    98,    99,   100,   101,    -1,    -1,   104,   105,   106,
     107,    -1,   109,   110,   111,   112,    -1,    -1,   115,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,    -1,    -1,    -1,    -1,    19,    20,
      -1,    -1,   159,    -1,   161,    -1,    -1,   164,    97,    98,
      99,   100,   101,    -1,    -1,   104,   105,   106,   107,    -1,
     109,   110,   111,   112,    -1,    -1,   115,    -1,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,   161,    -1,    -1,   164,    97,    98,    99,   100,
     101,    -1,    -1,   104,   105,   106,   107,    -1,   109,   110,
     111,   112,    -1,    -1,   115,    -1,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    29,    -1,   139,    -1,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
     161,    -1,    -1,   164,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,   122,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,   131,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,   163,    -1,    19,    20,   167,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      -1,    -1,    19,    20,   167,    97,    98,    99,   100,   101,
      -1,    -1,   104,   105,   106,   107,    -1,   109,   110,   111,
     112,    -1,    -1,   115,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,    -1,    -1,    -1,   139,    -1,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,   161,
     162,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      97,    98,    99,   100,   101,    -1,    -1,   104,   105,   106,
     107,    -1,   109,   110,   111,   112,    -1,    -1,   115,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    19,    20,    -1,    97,
      98,    99,   100,   101,    -1,    -1,   104,   105,   106,   107,
      -1,   109,   110,   111,   112,    -1,    -1,   115,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,    -1,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,   161,   162,    -1,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,   161,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,    -1,    -1,   115,   116,   117,   118,   119,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
     161,    97,    98,    99,   100,   101,    -1,    -1,   104,   105,
     106,   107,    -1,   109,   110,   111,   112,    -1,    -1,   115,
      -1,   117,   118,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,   161,    -1,    -1,    97,    98,
      99,   100,   101,    -1,    -1,   104,   105,   106,   107,    -1,
     109,   110,   111,   112,    -1,    -1,   115,    -1,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,   161,    97,    98,    99,   100,   101,    -1,    -1,
     104,   105,   106,   107,    -1,   109,   110,   111,   112,    -1,
      -1,   115,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,    -1,   139,    -1,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,   161,    -1,    -1,
      97,    98,    99,   100,   101,    -1,    -1,   104,   105,   106,
     107,    -1,   109,   110,   111,   112,    -1,    -1,   115,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,   161,    97,    98,    99,   100,   101,
      -1,    -1,   104,   105,   106,   107,    -1,   109,   110,   111,
     112,    -1,    -1,   115,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,   147,   148,   149,   150,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,   161,
      -1,    -1,    97,    98,    99,   100,   101,    -1,    -1,   104,
     105,   106,   107,    -1,   109,   110,   111,   112,    -1,    -1,
     115,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,
     145,   146,   147,   148,   149,   150,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,   161,    97,    98,    99,
     100,   101,    -1,    -1,   104,   105,   106,   107,    -1,   109,
     110,   111,   112,    -1,    -1,   115,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,   149,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,   161,    -1,    -1,    97,    98,    -1,    -1,   101,    -1,
      -1,   104,   105,   106,   107,    -1,   109,   110,   111,   112,
      -1,    -1,   115,    -1,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,   149,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,   161,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,    -1,    -1,   153,   154
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   173,     0,     5,    28,    30,    32,    34,    39,    45,
      51,    86,   161,   168,   174,   179,   180,   184,   203,   207,
     221,   253,   257,   258,   261,   268,   269,   131,   198,   199,
     125,   185,   186,   131,   181,   182,   131,   169,   131,    32,
     131,   167,   200,   201,   202,   212,     4,     6,    56,   252,
     131,   138,   137,   140,   137,    20,    52,   149,   159,   183,
     140,   169,   131,   255,   256,   167,   131,   163,   137,   162,
     131,   260,    33,    99,   100,   131,   206,   169,   169,    13,
      14,   125,   129,   131,   132,   163,   176,   198,   125,   186,
     131,   131,   131,    61,    65,    81,    82,    83,    84,   264,
     254,   138,   170,   255,   131,   199,   201,   140,   259,   169,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   117,   118,
     119,   120,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   153,   154,    33,    33,   163,   204,   251,   131,
     244,   245,   133,   175,    13,    14,   125,   129,   131,   176,
     196,   197,   183,   169,   131,   170,    23,    29,    44,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,   212,
     263,   265,   266,   267,   164,   212,   239,   161,   164,   236,
     240,   241,   140,   205,   131,   170,   250,   262,   140,   168,
     133,   134,   137,   164,   254,   138,   168,   144,   144,   144,
     163,   144,   144,   144,   144,   144,    31,    36,    55,   109,
     139,   143,   161,   168,   171,   170,   236,   238,   199,    45,
     247,   262,   164,   168,   267,   169,   208,   137,   140,   143,
     249,   267,   170,   244,   196,   170,    13,    14,    15,    16,
      25,    37,    41,    43,    46,    50,    87,    99,   100,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   132,   146,
     147,   148,   153,   154,   163,   165,   166,   178,   208,   212,
     214,   223,   224,   227,   228,   229,   232,   233,   234,   235,
     263,   270,   273,   278,   280,   282,   267,   267,   267,   131,
     204,   267,   204,   267,   204,   267,   131,   242,   243,   267,
     245,    31,   143,   161,   171,   162,   235,    42,   237,   168,
     162,   247,   108,   138,   140,   246,   240,   210,   131,   267,
     108,   116,   138,   248,   267,   270,   163,   163,   144,   144,
     163,   144,   144,   235,   235,    11,   267,    11,   267,   235,
     275,   279,   177,   235,   235,   235,   235,   235,   235,   166,
     131,   163,   203,    19,    20,    97,    98,    99,   100,   101,
     104,   105,   106,   107,   109,   110,   111,   112,   115,   117,
     118,   139,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   159,   161,   168,   163,   145,   168,   145,   164,
     205,   145,   205,   145,   205,   145,   140,   145,   168,   145,
     168,   143,   162,   162,   247,   267,   114,   211,   230,   235,
       1,     7,     8,     9,    11,    24,    26,    27,    36,    40,
      47,    49,    53,    54,    88,   168,   170,   187,   188,   191,
     192,   193,   194,   195,   209,   211,   213,   215,   216,   217,
     218,   219,   221,   222,   230,   231,   235,   168,   248,   211,
     235,   163,   131,   235,   267,   267,   235,   267,   267,   262,
     131,   162,   163,   235,   271,   272,   274,   275,   276,   277,
     262,   163,   272,   277,   121,   168,   170,   134,   135,   175,
     164,   144,   212,   161,   164,   225,   235,   204,    17,   131,
     131,   235,   235,   235,   235,   235,   235,   131,   235,   131,
     235,   235,   235,   235,   235,    20,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   131,   235,   164,
     225,   267,   145,   145,   145,   267,   242,   246,   166,   228,
     113,    89,    90,    91,    92,    93,    94,    95,    96,   102,
     103,   108,   116,   119,   120,   138,   235,   262,   108,   168,
     211,   235,   208,   235,   235,   168,   125,    53,   235,   108,
     211,   235,    35,   168,   168,   235,   168,   168,   168,    18,
     220,   168,   108,   138,   211,   235,   168,   164,   225,    17,
     144,   235,   164,   145,   145,   164,   145,   145,    18,   246,
     162,   164,   137,   162,   168,   137,   162,   168,    18,   164,
     170,   170,   235,   275,   170,   235,   204,   267,   131,   271,
     137,   164,   205,   267,   163,   162,   131,   140,   162,   164,
     145,   235,   228,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   208,    18,
     211,   235,   168,    12,   208,   140,   125,   211,   235,   168,
     169,   208,    38,   250,   114,   114,   168,   164,   267,   131,
     164,   235,   235,   235,   163,   225,   235,   272,   131,   162,
     271,   235,   162,   276,   225,   272,   162,   162,   136,   205,
     145,   162,   235,   121,   208,   226,   164,   225,   235,   225,
     168,   208,   168,   210,    10,    21,    22,   189,   190,   235,
     235,   164,   145,   168,   164,   235,   168,   162,   246,   168,
     170,   145,   212,   164,   108,   235,   164,   168,   170,   208,
     235,    17,   235,   131,   164,   235,   162,   235,   235,   162,
     212,   235,   208,   208,   267,   164,   145,   168,   281,   281,
     190,   164,    17,   235,    48,   162,   170,   267,   164,   235,
     162,   162,   164
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
#line 3108 "ds_parser.cpp"
        break;

    case 175: /* character_sequence  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3114 "ds_parser.cpp"
        break;

    case 176: /* string_constant  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3120 "ds_parser.cpp"
        break;

    case 177: /* string_builder_body  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3126 "ds_parser.cpp"
        break;

    case 178: /* string_builder  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3132 "ds_parser.cpp"
        break;

    case 181: /* require_module_name  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3138 "ds_parser.cpp"
        break;

    case 187: /* expression_label  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3144 "ds_parser.cpp"
        break;

    case 188: /* expression_goto  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3150 "ds_parser.cpp"
        break;

    case 190: /* expression_else  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3156 "ds_parser.cpp"
        break;

    case 192: /* expression_if_then_else  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3162 "ds_parser.cpp"
        break;

    case 193: /* expression_for_loop  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3168 "ds_parser.cpp"
        break;

    case 194: /* expression_while_loop  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3174 "ds_parser.cpp"
        break;

    case 195: /* expression_with  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3180 "ds_parser.cpp"
        break;

    case 196: /* annotation_argument_value  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3186 "ds_parser.cpp"
        break;

    case 197: /* annotation_argument_value_list  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3192 "ds_parser.cpp"
        break;

    case 198: /* annotation_argument  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3198 "ds_parser.cpp"
        break;

    case 199: /* annotation_argument_list  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3204 "ds_parser.cpp"
        break;

    case 200: /* annotation_declaration_name  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3210 "ds_parser.cpp"
        break;

    case 201: /* annotation_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3216 "ds_parser.cpp"
        break;

    case 202: /* annotation_list  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3222 "ds_parser.cpp"
        break;

    case 203: /* optional_annotation_list  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3228 "ds_parser.cpp"
        break;

    case 204: /* optional_function_argument_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3234 "ds_parser.cpp"
        break;

    case 205: /* optional_function_type  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3240 "ds_parser.cpp"
        break;

    case 206: /* function_name  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3246 "ds_parser.cpp"
        break;

    case 208: /* expression_block  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3252 "ds_parser.cpp"
        break;

    case 209: /* expression_any  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3258 "ds_parser.cpp"
        break;

    case 210: /* expressions  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3264 "ds_parser.cpp"
        break;

    case 211: /* expr_pipe  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3270 "ds_parser.cpp"
        break;

    case 212: /* name_in_namespace  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3276 "ds_parser.cpp"
        break;

    case 213: /* expression_delete  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3282 "ds_parser.cpp"
        break;

    case 214: /* expr_new  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3288 "ds_parser.cpp"
        break;

    case 215: /* expression_break  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3294 "ds_parser.cpp"
        break;

    case 216: /* expression_continue  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3300 "ds_parser.cpp"
        break;

    case 217: /* expression_return  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3306 "ds_parser.cpp"
        break;

    case 218: /* expression_yield  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3312 "ds_parser.cpp"
        break;

    case 219: /* expression_try_catch  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3318 "ds_parser.cpp"
        break;

    case 222: /* expression_let  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3324 "ds_parser.cpp"
        break;

    case 223: /* expr_cast  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3330 "ds_parser.cpp"
        break;

    case 224: /* expr_type_info  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3336 "ds_parser.cpp"
        break;

    case 225: /* expr_list  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3342 "ds_parser.cpp"
        break;

    case 226: /* block_or_simple_block  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3348 "ds_parser.cpp"
        break;

    case 228: /* expr_block  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3354 "ds_parser.cpp"
        break;

    case 229: /* expr_numeric_const  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3360 "ds_parser.cpp"
        break;

    case 230: /* expr_assign  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3366 "ds_parser.cpp"
        break;

    case 231: /* expr_assign_pipe  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3372 "ds_parser.cpp"
        break;

    case 232: /* expr_named_call  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3378 "ds_parser.cpp"
        break;

    case 233: /* expr_method_call  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3384 "ds_parser.cpp"
        break;

    case 234: /* func_addr_expr  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3390 "ds_parser.cpp"
        break;

    case 235: /* expr  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3396 "ds_parser.cpp"
        break;

    case 236: /* optional_field_annotation  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3402 "ds_parser.cpp"
        break;

    case 238: /* structure_variable_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3408 "ds_parser.cpp"
        break;

    case 239: /* struct_variable_declaration_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3414 "ds_parser.cpp"
        break;

    case 240: /* function_argument_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3420 "ds_parser.cpp"
        break;

    case 241: /* function_argument_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3426 "ds_parser.cpp"
        break;

    case 242: /* tuple_type  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3432 "ds_parser.cpp"
        break;

    case 243: /* tuple_type_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3438 "ds_parser.cpp"
        break;

    case 244: /* variant_type  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3444 "ds_parser.cpp"
        break;

    case 245: /* variant_type_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3450 "ds_parser.cpp"
        break;

    case 247: /* variable_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3456 "ds_parser.cpp"
        break;

    case 250: /* let_variable_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3462 "ds_parser.cpp"
        break;

    case 251: /* global_variable_declaration_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3468 "ds_parser.cpp"
        break;

    case 254: /* enum_list  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3474 "ds_parser.cpp"
        break;

    case 259: /* optional_structure_parent  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3480 "ds_parser.cpp"
        break;

    case 262: /* variable_name_list  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3486 "ds_parser.cpp"
        break;

    case 265: /* structure_type_declaration  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3492 "ds_parser.cpp"
        break;

    case 266: /* auto_type_declaration  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3498 "ds_parser.cpp"
        break;

    case 267: /* type_declaration  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3504 "ds_parser.cpp"
        break;

    case 270: /* make_decl  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3510 "ds_parser.cpp"
        break;

    case 271: /* make_struct_fields  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3516 "ds_parser.cpp"
        break;

    case 272: /* make_struct_dim  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3522 "ds_parser.cpp"
        break;

    case 273: /* make_struct_decl  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3528 "ds_parser.cpp"
        break;

    case 274: /* make_tuple  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3534 "ds_parser.cpp"
        break;

    case 275: /* make_map_tuple  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3540 "ds_parser.cpp"
        break;

    case 276: /* make_any_tuple  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3546 "ds_parser.cpp"
        break;

    case 277: /* make_dim  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3552 "ds_parser.cpp"
        break;

    case 278: /* make_dim_decl  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3558 "ds_parser.cpp"
        break;

    case 279: /* make_table  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3564 "ds_parser.cpp"
        break;

    case 280: /* make_table_decl  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3570 "ds_parser.cpp"
        break;

    case 281: /* array_comprehension_where  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3576 "ds_parser.cpp"
        break;

    case 282: /* array_comprehension  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3582 "ds_parser.cpp"
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
#line 402 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3882 "ds_parser.cpp"
    break;

  case 15:
#line 414 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3888 "ds_parser.cpp"
    break;

  case 16:
#line 415 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3894 "ds_parser.cpp"
    break;

  case 17:
#line 419 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 3900 "ds_parser.cpp"
    break;

  case 18:
#line 423 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3909 "ds_parser.cpp"
    break;

  case 19:
#line 427 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3923 "ds_parser.cpp"
    break;

  case 20:
#line 436 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3933 "ds_parser.cpp"
    break;

  case 21:
#line 444 "ds_parser.ypp"
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
#line 3951 "ds_parser.cpp"
    break;

  case 22:
#line 460 "ds_parser.ypp"
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
#line 3974 "ds_parser.cpp"
    break;

  case 24:
#line 485 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3982 "ds_parser.cpp"
    break;

  case 25:
#line 488 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3993 "ds_parser.cpp"
    break;

  case 26:
#line 494 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4004 "ds_parser.cpp"
    break;

  case 27:
#line 503 "ds_parser.ypp"
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
#line 4030 "ds_parser.cpp"
    break;

  case 28:
#line 524 "ds_parser.ypp"
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
#line 4055 "ds_parser.cpp"
    break;

  case 29:
#line 547 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4061 "ds_parser.cpp"
    break;

  case 30:
#line 548 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4067 "ds_parser.cpp"
    break;

  case 34:
#line 561 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4075 "ds_parser.cpp"
    break;

  case 35:
#line 564 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4083 "ds_parser.cpp"
    break;

  case 36:
#line 570 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4091 "ds_parser.cpp"
    break;

  case 37:
#line 576 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4099 "ds_parser.cpp"
    break;

  case 38:
#line 579 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4107 "ds_parser.cpp"
    break;

  case 39:
#line 585 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4113 "ds_parser.cpp"
    break;

  case 40:
#line 586 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4119 "ds_parser.cpp"
    break;

  case 41:
#line 590 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4125 "ds_parser.cpp"
    break;

  case 42:
#line 591 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4131 "ds_parser.cpp"
    break;

  case 43:
#line 592 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4142 "ds_parser.cpp"
    break;

  case 44:
#line 601 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4148 "ds_parser.cpp"
    break;

  case 45:
#line 602 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4154 "ds_parser.cpp"
    break;

  case 46:
#line 606 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4165 "ds_parser.cpp"
    break;

  case 47:
#line 615 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 4179 "ds_parser.cpp"
    break;

  case 48:
#line 627 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4191 "ds_parser.cpp"
    break;

  case 49:
#line 637 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4202 "ds_parser.cpp"
    break;

  case 50:
#line 646 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4208 "ds_parser.cpp"
    break;

  case 51:
#line 647 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4214 "ds_parser.cpp"
    break;

  case 52:
#line 648 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4220 "ds_parser.cpp"
    break;

  case 53:
#line 649 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4226 "ds_parser.cpp"
    break;

  case 54:
#line 650 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4232 "ds_parser.cpp"
    break;

  case 55:
#line 651 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4238 "ds_parser.cpp"
    break;

  case 56:
#line 655 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4248 "ds_parser.cpp"
    break;

  case 57:
#line 660 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4258 "ds_parser.cpp"
    break;

  case 58:
#line 668 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4264 "ds_parser.cpp"
    break;

  case 59:
#line 669 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4270 "ds_parser.cpp"
    break;

  case 60:
#line 670 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4276 "ds_parser.cpp"
    break;

  case 61:
#line 671 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4282 "ds_parser.cpp"
    break;

  case 62:
#line 672 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4288 "ds_parser.cpp"
    break;

  case 63:
#line 673 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4294 "ds_parser.cpp"
    break;

  case 64:
#line 674 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4300 "ds_parser.cpp"
    break;

  case 65:
#line 675 "ds_parser.ypp"
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4308 "ds_parser.cpp"
    break;

  case 66:
#line 681 "ds_parser.ypp"
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
#line 4326 "ds_parser.cpp"
    break;

  case 67:
#line 694 "ds_parser.ypp"
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
#line 4344 "ds_parser.cpp"
    break;

  case 68:
#line 710 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4350 "ds_parser.cpp"
    break;

  case 69:
#line 711 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4356 "ds_parser.cpp"
    break;

  case 70:
#line 715 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 4368 "ds_parser.cpp"
    break;

  case 71:
#line 722 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4382 "ds_parser.cpp"
    break;

  case 72:
#line 734 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4391 "ds_parser.cpp"
    break;

  case 73:
#line 738 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4400 "ds_parser.cpp"
    break;

  case 74:
#line 745 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4406 "ds_parser.cpp"
    break;

  case 75:
#line 746 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4412 "ds_parser.cpp"
    break;

  case 76:
#line 750 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4418 "ds_parser.cpp"
    break;

  case 77:
#line 751 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4424 "ds_parser.cpp"
    break;

  case 78:
#line 752 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4430 "ds_parser.cpp"
    break;

  case 79:
#line 756 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4439 "ds_parser.cpp"
    break;

  case 80:
#line 760 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4448 "ds_parser.cpp"
    break;

  case 81:
#line 767 "ds_parser.ypp"
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 4457 "ds_parser.cpp"
    break;

  case 82:
#line 771 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4463 "ds_parser.cpp"
    break;

  case 83:
#line 772 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4469 "ds_parser.cpp"
    break;

  case 84:
#line 773 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4475 "ds_parser.cpp"
    break;

  case 85:
#line 774 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4481 "ds_parser.cpp"
    break;

  case 86:
#line 775 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4487 "ds_parser.cpp"
    break;

  case 87:
#line 776 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4493 "ds_parser.cpp"
    break;

  case 88:
#line 777 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4499 "ds_parser.cpp"
    break;

  case 89:
#line 778 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4505 "ds_parser.cpp"
    break;

  case 90:
#line 779 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4511 "ds_parser.cpp"
    break;

  case 91:
#line 780 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4517 "ds_parser.cpp"
    break;

  case 92:
#line 781 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4523 "ds_parser.cpp"
    break;

  case 93:
#line 782 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4529 "ds_parser.cpp"
    break;

  case 94:
#line 783 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4535 "ds_parser.cpp"
    break;

  case 95:
#line 784 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4541 "ds_parser.cpp"
    break;

  case 96:
#line 785 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4547 "ds_parser.cpp"
    break;

  case 97:
#line 786 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4553 "ds_parser.cpp"
    break;

  case 98:
#line 787 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4559 "ds_parser.cpp"
    break;

  case 99:
#line 788 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4565 "ds_parser.cpp"
    break;

  case 100:
#line 789 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4571 "ds_parser.cpp"
    break;

  case 101:
#line 790 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4577 "ds_parser.cpp"
    break;

  case 102:
#line 791 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4583 "ds_parser.cpp"
    break;

  case 103:
#line 792 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4589 "ds_parser.cpp"
    break;

  case 104:
#line 793 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4595 "ds_parser.cpp"
    break;

  case 105:
#line 794 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4601 "ds_parser.cpp"
    break;

  case 106:
#line 795 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4607 "ds_parser.cpp"
    break;

  case 107:
#line 796 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4613 "ds_parser.cpp"
    break;

  case 108:
#line 797 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4619 "ds_parser.cpp"
    break;

  case 109:
#line 798 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4625 "ds_parser.cpp"
    break;

  case 110:
#line 799 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4631 "ds_parser.cpp"
    break;

  case 111:
#line 800 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4637 "ds_parser.cpp"
    break;

  case 112:
#line 801 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4643 "ds_parser.cpp"
    break;

  case 113:
#line 802 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4649 "ds_parser.cpp"
    break;

  case 114:
#line 803 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4655 "ds_parser.cpp"
    break;

  case 115:
#line 804 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4661 "ds_parser.cpp"
    break;

  case 116:
#line 805 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4667 "ds_parser.cpp"
    break;

  case 117:
#line 806 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4673 "ds_parser.cpp"
    break;

  case 118:
#line 811 "ds_parser.ypp"
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
#line 4747 "ds_parser.cpp"
    break;

  case 119:
#line 883 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4755 "ds_parser.cpp"
    break;

  case 120:
#line 886 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4767 "ds_parser.cpp"
    break;

  case 121:
#line 896 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4773 "ds_parser.cpp"
    break;

  case 122:
#line 897 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4779 "ds_parser.cpp"
    break;

  case 123:
#line 898 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4785 "ds_parser.cpp"
    break;

  case 124:
#line 899 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4791 "ds_parser.cpp"
    break;

  case 125:
#line 900 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4797 "ds_parser.cpp"
    break;

  case 126:
#line 901 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4803 "ds_parser.cpp"
    break;

  case 127:
#line 902 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4809 "ds_parser.cpp"
    break;

  case 128:
#line 903 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4815 "ds_parser.cpp"
    break;

  case 129:
#line 904 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4821 "ds_parser.cpp"
    break;

  case 130:
#line 905 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4827 "ds_parser.cpp"
    break;

  case 131:
#line 906 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4833 "ds_parser.cpp"
    break;

  case 132:
#line 907 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4839 "ds_parser.cpp"
    break;

  case 133:
#line 908 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4845 "ds_parser.cpp"
    break;

  case 134:
#line 909 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4851 "ds_parser.cpp"
    break;

  case 135:
#line 910 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4857 "ds_parser.cpp"
    break;

  case 136:
#line 911 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4863 "ds_parser.cpp"
    break;

  case 137:
#line 912 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4869 "ds_parser.cpp"
    break;

  case 138:
#line 913 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4875 "ds_parser.cpp"
    break;

  case 139:
#line 917 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4884 "ds_parser.cpp"
    break;

  case 140:
#line 921 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4895 "ds_parser.cpp"
    break;

  case 141:
#line 927 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4903 "ds_parser.cpp"
    break;

  case 142:
#line 933 "ds_parser.ypp"
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
#line 4919 "ds_parser.cpp"
    break;

  case 143:
#line 944 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4927 "ds_parser.cpp"
    break;

  case 144:
#line 950 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4933 "ds_parser.cpp"
    break;

  case 145:
#line 951 "ds_parser.ypp"
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
#line 4949 "ds_parser.cpp"
    break;

  case 146:
#line 962 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4955 "ds_parser.cpp"
    break;

  case 147:
#line 966 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4963 "ds_parser.cpp"
    break;

  case 148:
#line 972 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4971 "ds_parser.cpp"
    break;

  case 149:
#line 975 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4979 "ds_parser.cpp"
    break;

  case 150:
#line 978 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4988 "ds_parser.cpp"
    break;

  case 151:
#line 982 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4996 "ds_parser.cpp"
    break;

  case 152:
#line 988 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5002 "ds_parser.cpp"
    break;

  case 153:
#line 992 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5008 "ds_parser.cpp"
    break;

  case 154:
#line 996 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5016 "ds_parser.cpp"
    break;

  case 155:
#line 999 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5024 "ds_parser.cpp"
    break;

  case 156:
#line 1002 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5034 "ds_parser.cpp"
    break;

  case 157:
#line 1007 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5042 "ds_parser.cpp"
    break;

  case 158:
#line 1010 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5052 "ds_parser.cpp"
    break;

  case 159:
#line 1018 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5060 "ds_parser.cpp"
    break;

  case 160:
#line 1021 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5070 "ds_parser.cpp"
    break;

  case 161:
#line 1026 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5078 "ds_parser.cpp"
    break;

  case 162:
#line 1029 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5088 "ds_parser.cpp"
    break;

  case 163:
#line 1037 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5096 "ds_parser.cpp"
    break;

  case 164:
#line 1043 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5102 "ds_parser.cpp"
    break;

  case 165:
#line 1044 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5108 "ds_parser.cpp"
    break;

  case 166:
#line 1048 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5114 "ds_parser.cpp"
    break;

  case 167:
#line 1049 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5120 "ds_parser.cpp"
    break;

  case 168:
#line 1053 "ds_parser.ypp"
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
#line 5156 "ds_parser.cpp"
    break;

  case 169:
#line 1087 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5164 "ds_parser.cpp"
    break;

  case 170:
#line 1090 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5174 "ds_parser.cpp"
    break;

  case 171:
#line 1095 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5184 "ds_parser.cpp"
    break;

  case 172:
#line 1103 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5193 "ds_parser.cpp"
    break;

  case 173:
#line 1107 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5203 "ds_parser.cpp"
    break;

  case 174:
#line 1112 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5214 "ds_parser.cpp"
    break;

  case 175:
#line 1118 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 5223 "ds_parser.cpp"
    break;

  case 176:
#line 1122 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5233 "ds_parser.cpp"
    break;

  case 177:
#line 1127 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5244 "ds_parser.cpp"
    break;

  case 178:
#line 1136 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5252 "ds_parser.cpp"
    break;

  case 179:
#line 1139 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5260 "ds_parser.cpp"
    break;

  case 180:
#line 1145 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5266 "ds_parser.cpp"
    break;

  case 181:
#line 1146 "ds_parser.ypp"
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5278 "ds_parser.cpp"
    break;

  case 182:
#line 1153 "ds_parser.ypp"
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5291 "ds_parser.cpp"
    break;

  case 183:
#line 1164 "ds_parser.ypp"
    { (yyval.b) = false;   /* block */  }
#line 5297 "ds_parser.cpp"
    break;

  case 184:
#line 1165 "ds_parser.ypp"
    { (yyval.b) = true;    /* lambda */ }
#line 5303 "ds_parser.cpp"
    break;

  case 185:
#line 1169 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 5313 "ds_parser.cpp"
    break;

  case 186:
#line 1175 "ds_parser.ypp"
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
#line 5369 "ds_parser.cpp"
    break;

  case 187:
#line 1229 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5375 "ds_parser.cpp"
    break;

  case 188:
#line 1230 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5381 "ds_parser.cpp"
    break;

  case 189:
#line 1231 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5387 "ds_parser.cpp"
    break;

  case 190:
#line 1232 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5393 "ds_parser.cpp"
    break;

  case 191:
#line 1233 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5399 "ds_parser.cpp"
    break;

  case 192:
#line 1234 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5405 "ds_parser.cpp"
    break;

  case 193:
#line 1238 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5411 "ds_parser.cpp"
    break;

  case 194:
#line 1239 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5417 "ds_parser.cpp"
    break;

  case 195:
#line 1240 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5423 "ds_parser.cpp"
    break;

  case 196:
#line 1241 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5429 "ds_parser.cpp"
    break;

  case 197:
#line 1242 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5435 "ds_parser.cpp"
    break;

  case 198:
#line 1243 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5441 "ds_parser.cpp"
    break;

  case 199:
#line 1244 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5447 "ds_parser.cpp"
    break;

  case 200:
#line 1245 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5453 "ds_parser.cpp"
    break;

  case 201:
#line 1246 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5459 "ds_parser.cpp"
    break;

  case 202:
#line 1247 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5465 "ds_parser.cpp"
    break;

  case 203:
#line 1248 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5471 "ds_parser.cpp"
    break;

  case 204:
#line 1249 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5477 "ds_parser.cpp"
    break;

  case 205:
#line 1250 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5483 "ds_parser.cpp"
    break;

  case 206:
#line 1251 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5489 "ds_parser.cpp"
    break;

  case 207:
#line 1252 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5495 "ds_parser.cpp"
    break;

  case 208:
#line 1253 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5501 "ds_parser.cpp"
    break;

  case 209:
#line 1257 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5507 "ds_parser.cpp"
    break;

  case 210:
#line 1258 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5513 "ds_parser.cpp"
    break;

  case 211:
#line 1262 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5525 "ds_parser.cpp"
    break;

  case 212:
#line 1273 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5535 "ds_parser.cpp"
    break;

  case 213:
#line 1278 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5547 "ds_parser.cpp"
    break;

  case 214:
#line 1288 "ds_parser.ypp"
    { 
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
	}
#line 5556 "ds_parser.cpp"
    break;

  case 215:
#line 1292 "ds_parser.ypp"
    {
		auto expr = new ExprAddr(tokAt((yylsp[-4])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
        expr->funcType = make_shared<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5568 "ds_parser.cpp"
    break;

  case 216:
#line 1299 "ds_parser.ypp"
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
#line 5584 "ds_parser.cpp"
    break;

  case 217:
#line 1313 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5590 "ds_parser.cpp"
    break;

  case 218:
#line 1314 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5596 "ds_parser.cpp"
    break;

  case 219:
#line 1315 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5602 "ds_parser.cpp"
    break;

  case 220:
#line 1316 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5608 "ds_parser.cpp"
    break;

  case 221:
#line 1317 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5614 "ds_parser.cpp"
    break;

  case 222:
#line 1318 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5620 "ds_parser.cpp"
    break;

  case 223:
#line 1319 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5626 "ds_parser.cpp"
    break;

  case 224:
#line 1320 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5632 "ds_parser.cpp"
    break;

  case 225:
#line 1321 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5638 "ds_parser.cpp"
    break;

  case 226:
#line 1322 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5644 "ds_parser.cpp"
    break;

  case 227:
#line 1323 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5650 "ds_parser.cpp"
    break;

  case 228:
#line 1324 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5656 "ds_parser.cpp"
    break;

  case 229:
#line 1325 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5662 "ds_parser.cpp"
    break;

  case 230:
#line 1326 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5668 "ds_parser.cpp"
    break;

  case 231:
#line 1327 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5674 "ds_parser.cpp"
    break;

  case 232:
#line 1328 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5680 "ds_parser.cpp"
    break;

  case 233:
#line 1329 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5686 "ds_parser.cpp"
    break;

  case 234:
#line 1330 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5692 "ds_parser.cpp"
    break;

  case 235:
#line 1331 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5698 "ds_parser.cpp"
    break;

  case 236:
#line 1332 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5704 "ds_parser.cpp"
    break;

  case 237:
#line 1333 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5710 "ds_parser.cpp"
    break;

  case 238:
#line 1334 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5716 "ds_parser.cpp"
    break;

  case 239:
#line 1335 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5722 "ds_parser.cpp"
    break;

  case 240:
#line 1336 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5728 "ds_parser.cpp"
    break;

  case 241:
#line 1337 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5734 "ds_parser.cpp"
    break;

  case 242:
#line 1338 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5740 "ds_parser.cpp"
    break;

  case 243:
#line 1339 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5746 "ds_parser.cpp"
    break;

  case 244:
#line 1340 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5752 "ds_parser.cpp"
    break;

  case 245:
#line 1341 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5758 "ds_parser.cpp"
    break;

  case 246:
#line 1342 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5764 "ds_parser.cpp"
    break;

  case 247:
#line 1343 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5770 "ds_parser.cpp"
    break;

  case 248:
#line 1344 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5776 "ds_parser.cpp"
    break;

  case 249:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5782 "ds_parser.cpp"
    break;

  case 250:
#line 1346 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5788 "ds_parser.cpp"
    break;

  case 251:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5794 "ds_parser.cpp"
    break;

  case 252:
#line 1348 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5800 "ds_parser.cpp"
    break;

  case 253:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5806 "ds_parser.cpp"
    break;

  case 254:
#line 1350 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5812 "ds_parser.cpp"
    break;

  case 255:
#line 1351 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5818 "ds_parser.cpp"
    break;

  case 256:
#line 1352 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5827 "ds_parser.cpp"
    break;

  case 257:
#line 1356 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5836 "ds_parser.cpp"
    break;

  case 258:
#line 1360 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5844 "ds_parser.cpp"
    break;

  case 259:
#line 1363 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5852 "ds_parser.cpp"
    break;

  case 260:
#line 1366 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5858 "ds_parser.cpp"
    break;

  case 261:
#line 1367 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5864 "ds_parser.cpp"
    break;

  case 262:
#line 1368 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5870 "ds_parser.cpp"
    break;

  case 263:
#line 1369 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5880 "ds_parser.cpp"
    break;

  case 264:
#line 1374 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5890 "ds_parser.cpp"
    break;

  case 265:
#line 1379 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5896 "ds_parser.cpp"
    break;

  case 266:
#line 1380 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5904 "ds_parser.cpp"
    break;

  case 267:
#line 1383 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5912 "ds_parser.cpp"
    break;

  case 268:
#line 1386 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5921 "ds_parser.cpp"
    break;

  case 269:
#line 1390 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5930 "ds_parser.cpp"
    break;

  case 270:
#line 1394 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 5939 "ds_parser.cpp"
    break;

  case 271:
#line 1398 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5945 "ds_parser.cpp"
    break;

  case 272:
#line 1399 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5951 "ds_parser.cpp"
    break;

  case 273:
#line 1400 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5957 "ds_parser.cpp"
    break;

  case 274:
#line 1401 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5963 "ds_parser.cpp"
    break;

  case 275:
#line 1402 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5969 "ds_parser.cpp"
    break;

  case 276:
#line 1403 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5975 "ds_parser.cpp"
    break;

  case 277:
#line 1404 "ds_parser.ypp"
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
#line 5990 "ds_parser.cpp"
    break;

  case 278:
#line 1414 "ds_parser.ypp"
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
#line 6005 "ds_parser.cpp"
    break;

  case 279:
#line 1424 "ds_parser.ypp"
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
#line 6041 "ds_parser.cpp"
    break;

  case 280:
#line 1458 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6047 "ds_parser.cpp"
    break;

  case 281:
#line 1459 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 6053 "ds_parser.cpp"
    break;

  case 282:
#line 1463 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6059 "ds_parser.cpp"
    break;

  case 283:
#line 1464 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6065 "ds_parser.cpp"
    break;

  case 284:
#line 1468 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6075 "ds_parser.cpp"
    break;

  case 285:
#line 1476 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6083 "ds_parser.cpp"
    break;

  case 286:
#line 1479 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6092 "ds_parser.cpp"
    break;

  case 287:
#line 1486 "ds_parser.ypp"
    { 
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->constant = true; 
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
		}
#line 6102 "ds_parser.cpp"
    break;

  case 288:
#line 1491 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl); 
			(yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
		}
#line 6112 "ds_parser.cpp"
    break;

  case 289:
#line 1499 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6118 "ds_parser.cpp"
    break;

  case 290:
#line 1500 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6124 "ds_parser.cpp"
    break;

  case 291:
#line 1504 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6132 "ds_parser.cpp"
    break;

  case 292:
#line 1507 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6143 "ds_parser.cpp"
    break;

  case 293:
#line 1516 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6149 "ds_parser.cpp"
    break;

  case 294:
#line 1517 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6155 "ds_parser.cpp"
    break;

  case 295:
#line 1521 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6166 "ds_parser.cpp"
    break;

  case 296:
#line 1530 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6172 "ds_parser.cpp"
    break;

  case 297:
#line 1531 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6178 "ds_parser.cpp"
    break;

  case 298:
#line 1536 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6184 "ds_parser.cpp"
    break;

  case 299:
#line 1537 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6190 "ds_parser.cpp"
    break;

  case 300:
#line 1541 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 6200 "ds_parser.cpp"
    break;

  case 301:
#line 1546 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6208 "ds_parser.cpp"
    break;

  case 302:
#line 1549 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6217 "ds_parser.cpp"
    break;

  case 303:
#line 1553 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6228 "ds_parser.cpp"
    break;

  case 304:
#line 1559 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6239 "ds_parser.cpp"
    break;

  case 305:
#line 1568 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6245 "ds_parser.cpp"
    break;

  case 306:
#line 1569 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6251 "ds_parser.cpp"
    break;

  case 307:
#line 1570 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6257 "ds_parser.cpp"
    break;

  case 308:
#line 1574 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6263 "ds_parser.cpp"
    break;

  case 309:
#line 1575 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6269 "ds_parser.cpp"
    break;

  case 310:
#line 1579 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6277 "ds_parser.cpp"
    break;

  case 311:
#line 1582 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6287 "ds_parser.cpp"
    break;

  case 312:
#line 1587 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6297 "ds_parser.cpp"
    break;

  case 313:
#line 1592 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6310 "ds_parser.cpp"
    break;

  case 314:
#line 1600 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6323 "ds_parser.cpp"
    break;

  case 315:
#line 1611 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6331 "ds_parser.cpp"
    break;

  case 316:
#line 1614 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6340 "ds_parser.cpp"
    break;

  case 317:
#line 1621 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6346 "ds_parser.cpp"
    break;

  case 318:
#line 1622 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6352 "ds_parser.cpp"
    break;

  case 319:
#line 1626 "ds_parser.ypp"
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
#line 6384 "ds_parser.cpp"
    break;

  case 320:
#line 1656 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6392 "ds_parser.cpp"
    break;

  case 321:
#line 1659 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6406 "ds_parser.cpp"
    break;

  case 322:
#line 1668 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6420 "ds_parser.cpp"
    break;

  case 323:
#line 1681 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 6434 "ds_parser.cpp"
    break;

  case 327:
#line 1702 "ds_parser.ypp"
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
#line 6450 "ds_parser.cpp"
    break;

  case 328:
#line 1713 "ds_parser.ypp"
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
#line 6467 "ds_parser.cpp"
    break;

  case 329:
#line 1728 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 6473 "ds_parser.cpp"
    break;

  case 330:
#line 1729 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 6479 "ds_parser.cpp"
    break;

  case 331:
#line 1733 "ds_parser.ypp"
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
#line 6520 "ds_parser.cpp"
    break;

  case 332:
#line 1772 "ds_parser.ypp"
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
#line 6588 "ds_parser.cpp"
    break;

  case 333:
#line 1838 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6600 "ds_parser.cpp"
    break;

  case 334:
#line 1845 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6611 "ds_parser.cpp"
    break;

  case 335:
#line 1854 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 6617 "ds_parser.cpp"
    break;

  case 336:
#line 1855 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 6623 "ds_parser.cpp"
    break;

  case 337:
#line 1856 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6629 "ds_parser.cpp"
    break;

  case 338:
#line 1857 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6635 "ds_parser.cpp"
    break;

  case 339:
#line 1858 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6641 "ds_parser.cpp"
    break;

  case 340:
#line 1859 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 6647 "ds_parser.cpp"
    break;

  case 341:
#line 1860 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 6653 "ds_parser.cpp"
    break;

  case 342:
#line 1861 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 6659 "ds_parser.cpp"
    break;

  case 343:
#line 1862 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 6665 "ds_parser.cpp"
    break;

  case 344:
#line 1863 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6671 "ds_parser.cpp"
    break;

  case 345:
#line 1864 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6677 "ds_parser.cpp"
    break;

  case 346:
#line 1865 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6683 "ds_parser.cpp"
    break;

  case 347:
#line 1866 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 6689 "ds_parser.cpp"
    break;

  case 348:
#line 1867 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 6695 "ds_parser.cpp"
    break;

  case 349:
#line 1868 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 6701 "ds_parser.cpp"
    break;

  case 350:
#line 1869 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 6707 "ds_parser.cpp"
    break;

  case 351:
#line 1870 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 6713 "ds_parser.cpp"
    break;

  case 352:
#line 1871 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 6719 "ds_parser.cpp"
    break;

  case 353:
#line 1872 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 6725 "ds_parser.cpp"
    break;

  case 354:
#line 1873 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 6731 "ds_parser.cpp"
    break;

  case 355:
#line 1874 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 6737 "ds_parser.cpp"
    break;

  case 356:
#line 1875 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 6743 "ds_parser.cpp"
    break;

  case 357:
#line 1876 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 6749 "ds_parser.cpp"
    break;

  case 358:
#line 1877 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 6755 "ds_parser.cpp"
    break;

  case 359:
#line 1881 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6761 "ds_parser.cpp"
    break;

  case 360:
#line 1882 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6767 "ds_parser.cpp"
    break;

  case 361:
#line 1883 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6773 "ds_parser.cpp"
    break;

  case 362:
#line 1884 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6779 "ds_parser.cpp"
    break;

  case 363:
#line 1885 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6785 "ds_parser.cpp"
    break;

  case 364:
#line 1886 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6791 "ds_parser.cpp"
    break;

  case 365:
#line 1890 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6804 "ds_parser.cpp"
    break;

  case 366:
#line 1901 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6813 "ds_parser.cpp"
    break;

  case 367:
#line 1905 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6825 "ds_parser.cpp"
    break;

  case 368:
#line 1915 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6831 "ds_parser.cpp"
    break;

  case 369:
#line 1916 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6837 "ds_parser.cpp"
    break;

  case 370:
#line 1917 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6843 "ds_parser.cpp"
    break;

  case 371:
#line 1918 "ds_parser.ypp"
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
#line 6862 "ds_parser.cpp"
    break;

  case 372:
#line 1932 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6873 "ds_parser.cpp"
    break;

  case 373:
#line 1938 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6882 "ds_parser.cpp"
    break;

  case 374:
#line 1942 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6892 "ds_parser.cpp"
    break;

  case 375:
#line 1947 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6902 "ds_parser.cpp"
    break;

  case 376:
#line 1952 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6912 "ds_parser.cpp"
    break;

  case 377:
#line 1957 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6924 "ds_parser.cpp"
    break;

  case 378:
#line 1964 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6934 "ds_parser.cpp"
    break;

  case 379:
#line 1969 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6943 "ds_parser.cpp"
    break;

  case 380:
#line 1973 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6952 "ds_parser.cpp"
    break;

  case 381:
#line 1977 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6962 "ds_parser.cpp"
    break;

  case 382:
#line 1982 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6972 "ds_parser.cpp"
    break;

  case 383:
#line 1987 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6984 "ds_parser.cpp"
    break;

  case 384:
#line 1994 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6994 "ds_parser.cpp"
    break;

  case 385:
#line 1999 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7005 "ds_parser.cpp"
    break;

  case 386:
#line 2005 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7015 "ds_parser.cpp"
    break;

  case 387:
#line 2010 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7024 "ds_parser.cpp"
    break;

  case 388:
#line 2014 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7034 "ds_parser.cpp"
    break;

  case 389:
#line 2019 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7048 "ds_parser.cpp"
    break;

  case 390:
#line 2028 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7057 "ds_parser.cpp"
    break;

  case 391:
#line 2032 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7067 "ds_parser.cpp"
    break;

  case 392:
#line 2037 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7081 "ds_parser.cpp"
    break;

  case 393:
#line 2046 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7090 "ds_parser.cpp"
    break;

  case 394:
#line 2050 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7100 "ds_parser.cpp"
    break;

  case 395:
#line 2055 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7114 "ds_parser.cpp"
    break;

  case 396:
#line 2064 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7125 "ds_parser.cpp"
    break;

  case 397:
#line 2070 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7136 "ds_parser.cpp"
    break;

  case 398:
#line 2079 "ds_parser.ypp"
    {
        das_need_oxford_comma=false;
    }
#line 7144 "ds_parser.cpp"
    break;

  case 399:
#line 2085 "ds_parser.ypp"
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
#line 7161 "ds_parser.cpp"
    break;

  case 400:
#line 2100 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7167 "ds_parser.cpp"
    break;

  case 401:
#line 2101 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7173 "ds_parser.cpp"
    break;

  case 402:
#line 2102 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7179 "ds_parser.cpp"
    break;

  case 403:
#line 2103 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7185 "ds_parser.cpp"
    break;

  case 404:
#line 2107 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7197 "ds_parser.cpp"
    break;

  case 405:
#line 2114 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7208 "ds_parser.cpp"
    break;

  case 406:
#line 2123 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 7218 "ds_parser.cpp"
    break;

  case 407:
#line 2128 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7227 "ds_parser.cpp"
    break;

  case 408:
#line 2135 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7237 "ds_parser.cpp"
    break;

  case 409:
#line 2140 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7248 "ds_parser.cpp"
    break;

  case 410:
#line 2146 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7259 "ds_parser.cpp"
    break;

  case 411:
#line 2152 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7271 "ds_parser.cpp"
    break;

  case 412:
#line 2159 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7284 "ds_parser.cpp"
    break;

  case 413:
#line 2170 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7292 "ds_parser.cpp"
    break;

  case 414:
#line 2173 "ds_parser.ypp"
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
#line 7308 "ds_parser.cpp"
    break;

  case 415:
#line 2187 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7319 "ds_parser.cpp"
    break;

  case 416:
#line 2196 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7325 "ds_parser.cpp"
    break;

  case 417:
#line 2197 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7331 "ds_parser.cpp"
    break;

  case 418:
#line 2201 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7341 "ds_parser.cpp"
    break;

  case 419:
#line 2206 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7350 "ds_parser.cpp"
    break;

  case 420:
#line 2213 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7360 "ds_parser.cpp"
    break;

  case 421:
#line 2218 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7373 "ds_parser.cpp"
    break;

  case 422:
#line 2229 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7383 "ds_parser.cpp"
    break;

  case 423:
#line 2234 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7392 "ds_parser.cpp"
    break;

  case 424:
#line 2241 "ds_parser.ypp"
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7406 "ds_parser.cpp"
    break;

  case 425:
#line 2253 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 7412 "ds_parser.cpp"
    break;

  case 426:
#line 2254 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7418 "ds_parser.cpp"
    break;

  case 427:
#line 2258 "ds_parser.ypp"
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
#line 7437 "ds_parser.cpp"
    break;

  case 428:
#line 2272 "ds_parser.ypp"
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
#line 7456 "ds_parser.cpp"
    break;


#line 7460 "ds_parser.cpp"

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
#line 2288 "ds_parser.ypp"


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


