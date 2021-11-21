/* A Bison parser, made by GNU Bison 3.4.1.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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

    #include "parser_state.h"

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
    void varDeclToTypeDecl ( yyscan_t scanner, TypeDecl * pType, vector<VariableDeclaration*> * list, bool needNames = true );
    void runFunctionAnnotations ( yyscan_t scanner, Function * func, AnnotationList * annL, const LineInfo & at );
    LineInfo tokAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li );
    LineInfo tokRangeAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li, const struct DAS_YYLTYPE & lie );
    Annotation * findAnnotation ( yyscan_t scanner, const string & name, const LineInfo & at );

#line 114 "ds_parser.hpp"

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
#line 117 "ds_parser.ypp"

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

#line 317 "ds_parser.hpp"

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
