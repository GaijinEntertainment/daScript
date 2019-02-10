/* A Bison parser, made by GNU Bison 3.2.1.  */

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

#ifndef YY_YY_USERS_BORISBATKIN_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED
# define YY_YY_USERS_BORISBATKIN_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "src/parser/ds_parser.ypp" /* yacc.c:1912  */

	#include "daScript/misc/platform.h"
	#include "daScript/ast/ast.h"
    
    namespace das {
        extern ProgramPtr           g_Program;
        extern vector<FileInfo *>   g_AccessStack;
    }
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

#line 77 "/Users/borisbatkin/work/yzg/generated/ds_parser.hpp" /* yacc.c:1912  */

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
    _DEREF = 288,
    _SCOPE = 289,
    _TYPEDEF = 290,
    _TBOOL = 291,
    _TVOID = 292,
    _TSTRING = 293,
    _TAUTO = 294,
    _TINT = 295,
    _TINT2 = 296,
    _TINT3 = 297,
    _TINT4 = 298,
    _TUINT = 299,
    _TUINT2 = 300,
    _TUINT3 = 301,
    _TUINT4 = 302,
    _TFLOAT = 303,
    _TFLOAT2 = 304,
    _TFLOAT3 = 305,
    _TFLOAT4 = 306,
    _TRANGE = 307,
    _TURANGE = 308,
    _TBLOCK = 309,
    _TINT64 = 310,
    _TUINT64 = 311,
    _TYPENAME = 312,
    _TDOUBLE = 313,
    ADDEQU = 314,
    SUBEQU = 315,
    DIVEQU = 316,
    MULEQU = 317,
    MODEQU = 318,
    ANDEQU = 319,
    OREQU = 320,
    XOREQU = 321,
    ADDADD = 322,
    SUBSUB = 323,
    LEEQU = 324,
    GREQU = 325,
    EQUEQU = 326,
    NOTEQU = 327,
    RARROW = 328,
    LARROW = 329,
    QQ = 330,
    QDOT = 331,
    LPIPE = 332,
    LBPIPE = 333,
    RPIPE = 334,
    INTEGER = 335,
    LONG_INTEGER = 336,
    UNSIGNED_INTEGER = 337,
    UNSIGNED_LONG_INTEGER = 338,
    FLOAT = 339,
    DOUBLE = 340,
    NAME = 341,
    BEGIN_STRING = 342,
    STRING_CHARACTER = 343,
    END_STRING = 344,
    BEGIN_STRING_EXPR = 345,
    END_STRING_EXPR = 346,
    UNARY_MINUS = 347,
    UNARY_PLUS = 348,
    PRE_INC = 349,
    PRE_DEC = 350,
    POST_INC = 351,
    POST_DEC = 352,
    COLCOL = 353
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 54 "src/parser/ds_parser.ypp" /* yacc.c:1912  */

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

#line 212 "/Users/borisbatkin/work/yzg/generated/ds_parser.hpp" /* yacc.c:1912  */
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

#endif /* !YY_YY_USERS_BORISBATKIN_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED  */
