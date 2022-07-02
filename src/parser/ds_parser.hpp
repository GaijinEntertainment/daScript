/* A Bison parser, made by GNU Bison 3.7.6.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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


/* Token kinds.  */
#ifndef DAS_YYTOKENTYPE
# define DAS_YYTOKENTYPE
  enum das_yytokentype
  {
    DAS_YYEMPTY = -2,
    DAS_YYEOF = 0,                 /* "end of file"  */
    DAS_YYerror = 256,             /* error  */
    DAS_YYUNDEF = 257,             /* "invalid token"  */
    LEXER_ERROR = 258,             /* "lexer error"  */
    DAS_STRUCT = 259,              /* "struct"  */
    DAS_CLASS = 260,               /* "class"  */
    DAS_LET = 261,                 /* "let"  */
    DAS_DEF = 262,                 /* "def"  */
    DAS_WHILE = 263,               /* "while"  */
    DAS_IF = 264,                  /* "if"  */
    DAS_STATIC_IF = 265,           /* "static_if"  */
    DAS_ELSE = 266,                /* "else"  */
    DAS_FOR = 267,                 /* "for"  */
    DAS_CATCH = 268,               /* "recover"  */
    DAS_TRUE = 269,                /* "true"  */
    DAS_FALSE = 270,               /* "false"  */
    DAS_NEWT = 271,                /* "new"  */
    DAS_TYPEINFO = 272,            /* "typeinfo"  */
    DAS_TYPE = 273,                /* "type"  */
    DAS_IN = 274,                  /* "in"  */
    DAS_IS = 275,                  /* "is"  */
    DAS_AS = 276,                  /* "as"  */
    DAS_ELIF = 277,                /* "elif"  */
    DAS_STATIC_ELIF = 278,         /* "static_elif"  */
    DAS_ARRAY = 279,               /* "array"  */
    DAS_RETURN = 280,              /* "return"  */
    DAS_NULL = 281,                /* "null"  */
    DAS_BREAK = 282,               /* "break"  */
    DAS_TRY = 283,                 /* "try"  */
    DAS_OPTIONS = 284,             /* "options"  */
    DAS_TABLE = 285,               /* "table"  */
    DAS_EXPECT = 286,              /* "expect"  */
    DAS_CONST = 287,               /* "const"  */
    DAS_REQUIRE = 288,             /* "require"  */
    DAS_OPERATOR = 289,            /* "operator"  */
    DAS_ENUM = 290,                /* "enum"  */
    DAS_FINALLY = 291,             /* "finally"  */
    DAS_DELETE = 292,              /* "delete"  */
    DAS_DEREF = 293,               /* "deref"  */
    DAS_TYPEDEF = 294,             /* "typedef"  */
    DAS_WITH = 295,                /* "with"  */
    DAS_AKA = 296,                 /* "aka"  */
    DAS_ASSUME = 297,              /* "assume"  */
    DAS_CAST = 298,                /* "cast"  */
    DAS_OVERRIDE = 299,            /* "override"  */
    DAS_ABSTRACT = 300,            /* "abstract"  */
    DAS_UPCAST = 301,              /* "upcast"  */
    DAS_ITERATOR = 302,            /* "iterator"  */
    DAS_VAR = 303,                 /* "var"  */
    DAS_ADDR = 304,                /* "addr"  */
    DAS_CONTINUE = 305,            /* "continue"  */
    DAS_WHERE = 306,               /* "where"  */
    DAS_PASS = 307,                /* "pass"  */
    DAS_REINTERPRET = 308,         /* "reinterpret"  */
    DAS_MODULE = 309,              /* "module"  */
    DAS_PUBLIC = 310,              /* "public"  */
    DAS_LABEL = 311,               /* "label"  */
    DAS_GOTO = 312,                /* "goto"  */
    DAS_IMPLICIT = 313,            /* "implicit"  */
    DAS_EXPLICIT = 314,            /* "explicit"  */
    DAS_SHARED = 315,              /* "shared"  */
    DAS_PRIVATE = 316,             /* "private"  */
    DAS_SMART_PTR = 317,           /* "smart_ptr"  */
    DAS_UNSAFE = 318,              /* "unsafe"  */
    DAS_TBOOL = 319,               /* "bool"  */
    DAS_TVOID = 320,               /* "void"  */
    DAS_TSTRING = 321,             /* "string"  */
    DAS_TAUTO = 322,               /* "auto"  */
    DAS_TINT = 323,                /* "int"  */
    DAS_TINT2 = 324,               /* "int2"  */
    DAS_TINT3 = 325,               /* "int3"  */
    DAS_TINT4 = 326,               /* "int4"  */
    DAS_TUINT = 327,               /* "uint"  */
    DAS_TBITFIELD = 328,           /* "bitfield"  */
    DAS_TUINT2 = 329,              /* "uint2"  */
    DAS_TUINT3 = 330,              /* "uint3"  */
    DAS_TUINT4 = 331,              /* "uint4"  */
    DAS_TFLOAT = 332,              /* "float"  */
    DAS_TFLOAT2 = 333,             /* "float2"  */
    DAS_TFLOAT3 = 334,             /* "float3"  */
    DAS_TFLOAT4 = 335,             /* "float4"  */
    DAS_TRANGE = 336,              /* "range"  */
    DAS_TURANGE = 337,             /* "urange"  */
    DAS_TBLOCK = 338,              /* "block"  */
    DAS_TINT64 = 339,              /* "int64"  */
    DAS_TUINT64 = 340,             /* "uint64"  */
    DAS_TDOUBLE = 341,             /* "double"  */
    DAS_TFUNCTION = 342,           /* "function"  */
    DAS_TLAMBDA = 343,             /* "lambda"  */
    DAS_TINT8 = 344,               /* "int8"  */
    DAS_TUINT8 = 345,              /* "uint8"  */
    DAS_TINT16 = 346,              /* "int16"  */
    DAS_TUINT16 = 347,             /* "uint16"  */
    DAS_TTUPLE = 348,              /* "tuple"  */
    DAS_TVARIANT = 349,            /* "variant"  */
    DAS_GENERATOR = 350,           /* "generator"  */
    DAS_YIELD = 351,               /* "yield"  */
    DAS_SEALED = 352,              /* "sealed"  */
    ADDEQU = 353,                  /* "+="  */
    SUBEQU = 354,                  /* "-="  */
    DIVEQU = 355,                  /* "/="  */
    MULEQU = 356,                  /* "*="  */
    MODEQU = 357,                  /* "%="  */
    ANDEQU = 358,                  /* "&="  */
    OREQU = 359,                   /* "|="  */
    XOREQU = 360,                  /* "^="  */
    SHL = 361,                     /* "<<"  */
    SHR = 362,                     /* ">>"  */
    ADDADD = 363,                  /* "++"  */
    SUBSUB = 364,                  /* "--"  */
    LEEQU = 365,                   /* "<="  */
    SHLEQU = 366,                  /* "<<="  */
    SHREQU = 367,                  /* ">>="  */
    GREQU = 368,                   /* ">="  */
    EQUEQU = 369,                  /* "=="  */
    NOTEQU = 370,                  /* "!="  */
    RARROW = 371,                  /* "->"  */
    LARROW = 372,                  /* "<-"  */
    QQ = 373,                      /* "??"  */
    QDOT = 374,                    /* "?."  */
    QBRA = 375,                    /* "?["  */
    LPIPE = 376,                   /* "<|"  */
    LBPIPE = 377,                  /* " <|"  */
    LAPIPE = 378,                  /* "@ <|"  */
    LFPIPE = 379,                  /* "@@ <|"  */
    RPIPE = 380,                   /* "|>"  */
    CLONEEQU = 381,                /* ":="  */
    ROTL = 382,                    /* "<<<"  */
    ROTR = 383,                    /* ">>>"  */
    ROTLEQU = 384,                 /* "<<<="  */
    ROTREQU = 385,                 /* ">>>="  */
    MAPTO = 386,                   /* "=>"  */
    COLCOL = 387,                  /* "::"  */
    ANDAND = 388,                  /* "&&"  */
    OROR = 389,                    /* "||"  */
    XORXOR = 390,                  /* "^^"  */
    ANDANDEQU = 391,               /* "&&="  */
    OROREQU = 392,                 /* "||="  */
    XORXOREQU = 393,               /* "^^="  */
    DOTDOT = 394,                  /* ".."  */
    MTAG_E = 395,                  /* "$$"  */
    MTAG_I = 396,                  /* "$i"  */
    MTAG_V = 397,                  /* "$v"  */
    MTAG_B = 398,                  /* "$b"  */
    MTAG_A = 399,                  /* "$a"  */
    MTAG_T = 400,                  /* "$t"  */
    MTAG_C = 401,                  /* "$c"  */
    MTAG_F = 402,                  /* "$f"  */
    BRABRAB = 403,                 /* "[["  */
    BRACBRB = 404,                 /* "[{"  */
    CBRCBRB = 405,                 /* "{{"  */
    INTEGER = 406,                 /* "integer constant"  */
    LONG_INTEGER = 407,            /* "long integer constant"  */
    UNSIGNED_INTEGER = 408,        /* "unsigned integer constant"  */
    UNSIGNED_LONG_INTEGER = 409,   /* "unsigned long integer constant"  */
    FLOAT = 410,                   /* "floating point constant"  */
    DOUBLE = 411,                  /* "double constant"  */
    NAME = 412,                    /* "name"  */
    BEGIN_STRING = 413,            /* "start of the string"  */
    STRING_CHARACTER = 414,        /* STRING_CHARACTER  */
    STRING_CHARACTER_ESC = 415,    /* STRING_CHARACTER_ESC  */
    END_STRING = 416,              /* "end of the string"  */
    BEGIN_STRING_EXPR = 417,       /* "{"  */
    END_STRING_EXPR = 418,         /* "}"  */
    END_OF_READ = 419,             /* "end of failed eader macro"  */
    UNARY_MINUS = 420,             /* UNARY_MINUS  */
    UNARY_PLUS = 421,              /* UNARY_PLUS  */
    PRE_INC = 422,                 /* PRE_INC  */
    PRE_DEC = 423,                 /* PRE_DEC  */
    POST_INC = 424,                /* POST_INC  */
    POST_DEC = 425,                /* POST_DEC  */
    DEREF = 426                    /* DEREF  */
  };
  typedef enum das_yytokentype das_yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED
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
