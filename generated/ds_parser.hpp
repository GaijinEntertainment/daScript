/* A Bison parser, made by GNU Bison 3.2.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_C_USERS_BORIS_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED
# define YY_YY_C_USERS_BORIS_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "src/parser/ds_parser.ypp" /* yacc.c:1906  */

	#include "daScript/misc/platform.h"
	#include "daScript/ast/ast.h"
    
    namespace das { extern ProgramPtr g_Program; }
    using namespace das;
    
    struct VariableDeclaration {
        VariableDeclaration ( const LineInfo & a, vector<string> * n, TypeDecl * t, Expression * i )
            : at(a), pNameList(n), pTypeDecl(t), pInit(i) {}
        ~VariableDeclaration () { delete pNameList; delete pTypeDecl; delete pInit; }
        LineInfo        at;
        vector<string>  *pNameList;
        TypeDecl        *pTypeDecl;
        Expression      *pInit;
        bool            move_to_init = false;
    };
    
    ExprLooksLikeCall * parseFunctionArguments ( ExprLooksLikeCall * pCall, Expression * arguments );
    vector<ExpressionPtr> sequenceToList ( Expression * arguments );
    void deleteVariableDeclarationList ( vector<VariableDeclaration *> * list );
    
    LineInfo tokAt ( const struct YYLTYPE & li );
    
    Annotation * findAnnotation ( const string & name, const LineInfo & at );

#line 74 "C:/Users/Boris/Work/yzg/generated/ds_parser.hpp" /* yacc.c:1906  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LEXER_ERROR = 258,
    _STRUCT = 259,
    _LET = 260,
    _DEF = 261,
    _WHILE = 262,
    _IF = 263,
    _ELSE = 264,
    _FOR = 265,
    _CATCH = 266,
    _TRUE = 267,
    _FALSE = 268,
    _SIZEOF = 269,
    _NEWT = 270,
    _TYPE = 271,
    _IN = 272,
    _ELIF = 273,
    _ARRAY = 274,
    _RETURN = 275,
    _NULL = 276,
    _BREAK = 277,
    _TRY = 278,
    _OPTIONS = 279,
    _TABLE = 280,
    _EXPECT = 281,
    _CONST = 282,
    _REQUIRE = 283,
    _OPERATOR = 284,
    _ENUM = 285,
    _FINALLY = 286,
    _DELETE = 287,
    _TBOOL = 288,
    _TVOID = 289,
    _TSTRING = 290,
    _TAUTO = 291,
    _TINT = 292,
    _TINT2 = 293,
    _TINT3 = 294,
    _TINT4 = 295,
    _TUINT = 296,
    _TUINT2 = 297,
    _TUINT3 = 298,
    _TUINT4 = 299,
    _TFLOAT = 300,
    _TFLOAT2 = 301,
    _TFLOAT3 = 302,
    _TFLOAT4 = 303,
    _TRANGE = 304,
    _TURANGE = 305,
    _TBLOCK = 306,
    _TINT64 = 307,
    _TUINT64 = 308,
    _TYPENAME = 309,
    _TDOUBLE = 310,
    ADDEQU = 311,
    SUBEQU = 312,
    DIVEQU = 313,
    MULEQU = 314,
    MODEQU = 315,
    ANDEQU = 316,
    OREQU = 317,
    XOREQU = 318,
    ADDADD = 319,
    SUBSUB = 320,
    LEEQU = 321,
    GREQU = 322,
    EQUEQU = 323,
    NOTEQU = 324,
    RARROW = 325,
    LARROW = 326,
    QQ = 327,
    QDOT = 328,
    LPIPE = 329,
    LBPIPE = 330,
    RPIPE = 331,
    INTEGER = 332,
    LONG_INTEGER = 333,
    UNSIGNED_INTEGER = 334,
    UNSIGNED_LONG_INTEGER = 335,
    FLOAT = 336,
    DOUBLE = 337,
    NAME = 338,
    BEGIN_STRING = 339,
    STRING_CHARACTER = 340,
    END_STRING = 341,
    BEGIN_STRING_EXPR = 342,
    END_STRING_EXPR = 343,
    UNARY_MINUS = 344,
    UNARY_PLUS = 345,
    PRE_INC = 346,
    PRE_DEC = 347,
    POST_INC = 348,
    POST_DEC = 349,
    COLCOL = 350
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 51 "src/parser/ds_parser.ypp" /* yacc.c:1906  */

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

#line 206 "C:/Users/Boris/Work/yzg/generated/ds_parser.hpp" /* yacc.c:1906  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_C_USERS_BORIS_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED  */
