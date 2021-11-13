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

    namespace das {
        extern ProgramPtr               g_Program;
        extern FileAccessPtr            g_Access;
        extern vector<FileInfo *>       g_FileAccessStack;
        extern das_map<string,string>   das_module_alias;

        extern ReaderMacro *            g_ReaderMacro;
        extern ExprReader *             g_ReaderExpr;
    }
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
    void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list, bool needNames = true );
    void runFunctionAnnotations ( Function * func, AnnotationList * annL, const LineInfo & at );

    LineInfo tokAt ( const struct YYLTYPE & li );
    LineInfo tokRangeAt ( const struct YYLTYPE & li, const struct YYLTYPE & lie );

    Annotation * findAnnotation ( const string & name, const LineInfo & at );

    extern bool das_need_oxford_comma;
    extern bool das_force_oxford_comma;
    extern bool das_supress_errors;
    extern int das_arrow_depth;

    extern Structure * g_thisStructure;

#line 130 "ds_parser.hpp"

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
    DAS_CAST = 297,
    DAS_OVERRIDE = 298,
    DAS_ABSTRACT = 299,
    DAS_UPCAST = 300,
    DAS_ITERATOR = 301,
    DAS_VAR = 302,
    DAS_ADDR = 303,
    DAS_CONTINUE = 304,
    DAS_WHERE = 305,
    DAS_PASS = 306,
    DAS_REINTERPRET = 307,
    DAS_MODULE = 308,
    DAS_PUBLIC = 309,
    DAS_LABEL = 310,
    DAS_GOTO = 311,
    DAS_IMPLICIT = 312,
    DAS_EXPLICIT = 313,
    DAS_SHARED = 314,
    DAS_PRIVATE = 315,
    DAS_SMART_PTR = 316,
    DAS_UNSAFE = 317,
    DAS_TBOOL = 318,
    DAS_TVOID = 319,
    DAS_TSTRING = 320,
    DAS_TAUTO = 321,
    DAS_TINT = 322,
    DAS_TINT2 = 323,
    DAS_TINT3 = 324,
    DAS_TINT4 = 325,
    DAS_TUINT = 326,
    DAS_TBITFIELD = 327,
    DAS_TUINT2 = 328,
    DAS_TUINT3 = 329,
    DAS_TUINT4 = 330,
    DAS_TFLOAT = 331,
    DAS_TFLOAT2 = 332,
    DAS_TFLOAT3 = 333,
    DAS_TFLOAT4 = 334,
    DAS_TRANGE = 335,
    DAS_TURANGE = 336,
    DAS_TBLOCK = 337,
    DAS_TINT64 = 338,
    DAS_TUINT64 = 339,
    DAS_TDOUBLE = 340,
    DAS_TFUNCTION = 341,
    DAS_TLAMBDA = 342,
    DAS_TINT8 = 343,
    DAS_TUINT8 = 344,
    DAS_TINT16 = 345,
    DAS_TUINT16 = 346,
    DAS_TTUPLE = 347,
    DAS_TVARIANT = 348,
    DAS_GENERATOR = 349,
    DAS_YIELD = 350,
    DAS_SEALED = 351,
    ADDEQU = 352,
    SUBEQU = 353,
    DIVEQU = 354,
    MULEQU = 355,
    MODEQU = 356,
    ANDEQU = 357,
    OREQU = 358,
    XOREQU = 359,
    SHL = 360,
    SHR = 361,
    ADDADD = 362,
    SUBSUB = 363,
    LEEQU = 364,
    SHLEQU = 365,
    SHREQU = 366,
    GREQU = 367,
    EQUEQU = 368,
    NOTEQU = 369,
    RARROW = 370,
    LARROW = 371,
    QQ = 372,
    QDOT = 373,
    QBRA = 374,
    LPIPE = 375,
    LBPIPE = 376,
    LAPIPE = 377,
    LFPIPE = 378,
    RPIPE = 379,
    CLONEEQU = 380,
    ROTL = 381,
    ROTR = 382,
    ROTLEQU = 383,
    ROTREQU = 384,
    MAPTO = 385,
    COLCOL = 386,
    ANDAND = 387,
    OROR = 388,
    XORXOR = 389,
    ANDANDEQU = 390,
    OROREQU = 391,
    XORXOREQU = 392,
    BRABRAB = 393,
    BRACBRB = 394,
    CBRCBRB = 395,
    INTEGER = 396,
    LONG_INTEGER = 397,
    UNSIGNED_INTEGER = 398,
    UNSIGNED_LONG_INTEGER = 399,
    FLOAT = 400,
    DOUBLE = 401,
    NAME = 402,
    BEGIN_STRING = 403,
    STRING_CHARACTER = 404,
    STRING_CHARACTER_ESC = 405,
    END_STRING = 406,
    BEGIN_STRING_EXPR = 407,
    END_STRING_EXPR = 408,
    END_OF_READ = 409,
    UNARY_MINUS = 410,
    UNARY_PLUS = 411,
    PRE_INC = 412,
    PRE_DEC = 413,
    POST_INC = 414,
    POST_DEC = 415,
    DEREF = 416
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED
union DAS_YYSTYPE
{
#line 119 "ds_parser.ypp"

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

#line 332 "ds_parser.hpp"

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
