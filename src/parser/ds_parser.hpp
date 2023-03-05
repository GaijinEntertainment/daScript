/* A Bison parser, made by GNU Bison 3.0.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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


    #include "daScript/misc/platform.h"
    #include "daScript/ast/ast.h"
    #include "daScript/ast/ast_generate.h"
    #include "daScript/ast/ast_expressions.h"

    #include "parser_state.h"

#if defined(_MSC_VER) && !defined(__clang__)
    #if defined(DAS_RELWITHDEBINFO)
        #pragma optimize( "s", on )
    #endif
#endif

    enum {
        CorM_COPY   = 0,
        CorM_MOVE   = (1<<0),
        CorM_CLONE  = (1<<1)
    };

    using namespace das;

    #include "parser_impl.h"

    LineInfo tokAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li );
    LineInfo tokRangeAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li, const struct DAS_YYLTYPE & lie );

    struct TypePair {
        TypeDecl * firstType;
        TypeDecl * secondType;
    };



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
    DAS_INSCOPE = 319,
    DAS_TBOOL = 320,
    DAS_TVOID = 321,
    DAS_TSTRING = 322,
    DAS_TAUTO = 323,
    DAS_TINT = 324,
    DAS_TINT2 = 325,
    DAS_TINT3 = 326,
    DAS_TINT4 = 327,
    DAS_TUINT = 328,
    DAS_TBITFIELD = 329,
    DAS_TUINT2 = 330,
    DAS_TUINT3 = 331,
    DAS_TUINT4 = 332,
    DAS_TFLOAT = 333,
    DAS_TFLOAT2 = 334,
    DAS_TFLOAT3 = 335,
    DAS_TFLOAT4 = 336,
    DAS_TRANGE = 337,
    DAS_TURANGE = 338,
    DAS_TBLOCK = 339,
    DAS_TINT64 = 340,
    DAS_TUINT64 = 341,
    DAS_TDOUBLE = 342,
    DAS_TFUNCTION = 343,
    DAS_TLAMBDA = 344,
    DAS_TINT8 = 345,
    DAS_TUINT8 = 346,
    DAS_TINT16 = 347,
    DAS_TUINT16 = 348,
    DAS_TTUPLE = 349,
    DAS_TVARIANT = 350,
    DAS_GENERATOR = 351,
    DAS_YIELD = 352,
    DAS_SEALED = 353,
    ADDEQU = 354,
    SUBEQU = 355,
    DIVEQU = 356,
    MULEQU = 357,
    MODEQU = 358,
    ANDEQU = 359,
    OREQU = 360,
    XOREQU = 361,
    SHL = 362,
    SHR = 363,
    ADDADD = 364,
    SUBSUB = 365,
    LEEQU = 366,
    SHLEQU = 367,
    SHREQU = 368,
    GREQU = 369,
    EQUEQU = 370,
    NOTEQU = 371,
    RARROW = 372,
    LARROW = 373,
    QQ = 374,
    QDOT = 375,
    QBRA = 376,
    LPIPE = 377,
    LBPIPE = 378,
    LAPIPE = 379,
    LFPIPE = 380,
    RPIPE = 381,
    CLONEEQU = 382,
    ROTL = 383,
    ROTR = 384,
    ROTLEQU = 385,
    ROTREQU = 386,
    MAPTO = 387,
    COLCOL = 388,
    ANDAND = 389,
    OROR = 390,
    XORXOR = 391,
    ANDANDEQU = 392,
    OROREQU = 393,
    XORXOREQU = 394,
    DOTDOT = 395,
    MTAG_E = 396,
    MTAG_I = 397,
    MTAG_V = 398,
    MTAG_B = 399,
    MTAG_A = 400,
    MTAG_T = 401,
    MTAG_C = 402,
    MTAG_F = 403,
    MTAG_DOTDOTDOT = 404,
    BRABRAB = 405,
    BRACBRB = 406,
    CBRCBRB = 407,
    INTEGER = 408,
    LONG_INTEGER = 409,
    UNSIGNED_INTEGER = 410,
    UNSIGNED_LONG_INTEGER = 411,
    FLOAT = 412,
    DOUBLE = 413,
    NAME = 414,
    KEYWORD = 415,
    BEGIN_STRING = 416,
    STRING_CHARACTER = 417,
    STRING_CHARACTER_ESC = 418,
    END_STRING = 419,
    BEGIN_STRING_EXPR = 420,
    END_STRING_EXPR = 421,
    END_OF_READ = 422,
    UNARY_MINUS = 423,
    UNARY_PLUS = 424,
    PRE_INC = 425,
    PRE_DEC = 426,
    POST_INC = 427,
    POST_DEC = 428,
    DEREF = 429
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED
typedef union DAS_YYSTYPE DAS_YYSTYPE;
union DAS_YYSTYPE
{


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
    TypePair                        aTypePair;


};
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
