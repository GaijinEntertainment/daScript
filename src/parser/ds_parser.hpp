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
#line 2 "ds_parser.ypp" /* yacc.c:1912  */

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
        extern ProgramPtr			g_Program;
        extern FileAccessPtr        g_Access;
        extern vector<FileInfo *>	g_FileAccessStack;
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
    void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list );
    
    LineInfo tokAt ( const struct YYLTYPE & li );
    
    Annotation * findAnnotation ( const string & name, const LineInfo & at );

#line 103 "ds_parser.hpp" /* yacc.c:1912  */

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
    DAS_ELSE = 264,
    DAS_FOR = 265,
    DAS_CATCH = 266,
    DAS_TRUE = 267,
    DAS_FALSE = 268,
    DAS_NEWT = 269,
    DAS_TYPEINFO = 270,
    DAS_TYPE = 271,
    DAS_IN = 272,
    DAS_IS = 273,
    DAS_ELIF = 274,
    DAS_ARRAY = 275,
    DAS_RETURN = 276,
    DAS_NULL = 277,
    DAS_BREAK = 278,
    DAS_TRY = 279,
    DAS_OPTIONS = 280,
    DAS_TABLE = 281,
    DAS_EXPECT = 282,
    DAS_CONST = 283,
    DAS_REQUIRE = 284,
    DAS_OPERATOR = 285,
    DAS_ENUM = 286,
    DAS_FINALLY = 287,
    DAS_DELETE = 288,
    DAS_DEREF = 289,
    DAS_SCOPE = 290,
    DAS_TYPEDEF = 291,
    DAS_WITH = 292,
    DAS_CAST = 293,
    DAS_OVERRIDE = 294,
    DAS_UPCAST = 295,
    DAS_ITERATOR = 296,
    DAS_VAR = 297,
    DAS_ADDR = 298,
    DAS_CONTINUE = 299,
    DAS_WHERE = 300,
    DAS_PASS = 301,
    DAS_REINTERPRET = 302,
    DAS_MODULE = 303,
    DAS_PUBLIC = 304,
    DAS_LABEL = 305,
    DAS_GOTO = 306,
    DAS_TBOOL = 307,
    DAS_TVOID = 308,
    DAS_TSTRING = 309,
    DAS_TAUTO = 310,
    DAS_TINT = 311,
    DAS_TINT2 = 312,
    DAS_TINT3 = 313,
    DAS_TINT4 = 314,
    DAS_TUINT = 315,
    DAS_TUINT2 = 316,
    DAS_TUINT3 = 317,
    DAS_TUINT4 = 318,
    DAS_TFLOAT = 319,
    DAS_TFLOAT2 = 320,
    DAS_TFLOAT3 = 321,
    DAS_TFLOAT4 = 322,
    DAS_TRANGE = 323,
    DAS_TURANGE = 324,
    DAS_TBLOCK = 325,
    DAS_TINT64 = 326,
    DAS_TUINT64 = 327,
    DAS_TDOUBLE = 328,
    DAS_TFUNCTION = 329,
    DAS_TLAMBDA = 330,
    DAS_TINT8 = 331,
    DAS_TUINT8 = 332,
    DAS_TINT16 = 333,
    DAS_TUINT16 = 334,
    DAS_TTUPLE = 335,
    ADDEQU = 336,
    SUBEQU = 337,
    DIVEQU = 338,
    MULEQU = 339,
    MODEQU = 340,
    ANDEQU = 341,
    OREQU = 342,
    XOREQU = 343,
    SHL = 344,
    SHR = 345,
    ADDADD = 346,
    SUBSUB = 347,
    LEEQU = 348,
    SHLEQU = 349,
    SHREQU = 350,
    GREQU = 351,
    EQUEQU = 352,
    NOTEQU = 353,
    RARROW = 354,
    LARROW = 355,
    QQ = 356,
    QDOT = 357,
    LPIPE = 358,
    LBPIPE = 359,
    RPIPE = 360,
    CLONEEQU = 361,
    ROTL = 362,
    ROTR = 363,
    ROTLEQU = 364,
    ROTREQU = 365,
    MAPTO = 366,
    BRABRAB = 367,
    BRACBRB = 368,
    CBRCBRB = 369,
    INTEGER = 370,
    LONG_INTEGER = 371,
    UNSIGNED_INTEGER = 372,
    UNSIGNED_LONG_INTEGER = 373,
    FLOAT = 374,
    DOUBLE = 375,
    NAME = 376,
    BEGIN_STRING = 377,
    STRING_CHARACTER = 378,
    END_STRING = 379,
    BEGIN_STRING_EXPR = 380,
    END_STRING_EXPR = 381,
    UNARY_MINUS = 382,
    UNARY_PLUS = 383,
    PRE_INC = 384,
    PRE_DEC = 385,
    POST_INC = 386,
    POST_DEC = 387,
    DEREF = 388,
    COLCOL = 389
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED

union DAS_YYSTYPE
{
#line 76 "ds_parser.ypp" /* yacc.c:1912  */

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

#line 275 "ds_parser.hpp" /* yacc.c:1912  */
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
