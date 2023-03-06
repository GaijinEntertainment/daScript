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
        // #pragma optimize( "s", on )
        #pragma optimize( "", off )
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
    DAS_INSCOPE = 319,             /* "inscope"  */
    DAS_TBOOL = 320,               /* "bool"  */
    DAS_TVOID = 321,               /* "void"  */
    DAS_TSTRING = 322,             /* "string"  */
    DAS_TAUTO = 323,               /* "auto"  */
    DAS_TINT = 324,                /* "int"  */
    DAS_TINT2 = 325,               /* "int2"  */
    DAS_TINT3 = 326,               /* "int3"  */
    DAS_TINT4 = 327,               /* "int4"  */
    DAS_TUINT = 328,               /* "uint"  */
    DAS_TBITFIELD = 329,           /* "bitfield"  */
    DAS_TUINT2 = 330,              /* "uint2"  */
    DAS_TUINT3 = 331,              /* "uint3"  */
    DAS_TUINT4 = 332,              /* "uint4"  */
    DAS_TFLOAT = 333,              /* "float"  */
    DAS_TFLOAT2 = 334,             /* "float2"  */
    DAS_TFLOAT3 = 335,             /* "float3"  */
    DAS_TFLOAT4 = 336,             /* "float4"  */
    DAS_TRANGE = 337,              /* "range"  */
    DAS_TURANGE = 338,             /* "urange"  */
    DAS_TBLOCK = 339,              /* "block"  */
    DAS_TINT64 = 340,              /* "int64"  */
    DAS_TUINT64 = 341,             /* "uint64"  */
    DAS_TDOUBLE = 342,             /* "double"  */
    DAS_TFUNCTION = 343,           /* "function"  */
    DAS_TLAMBDA = 344,             /* "lambda"  */
    DAS_TINT8 = 345,               /* "int8"  */
    DAS_TUINT8 = 346,              /* "uint8"  */
    DAS_TINT16 = 347,              /* "int16"  */
    DAS_TUINT16 = 348,             /* "uint16"  */
    DAS_TTUPLE = 349,              /* "tuple"  */
    DAS_TVARIANT = 350,            /* "variant"  */
    DAS_GENERATOR = 351,           /* "generator"  */
    DAS_YIELD = 352,               /* "yield"  */
    DAS_SEALED = 353,              /* "sealed"  */
    ADDEQU = 354,                  /* "+="  */
    SUBEQU = 355,                  /* "-="  */
    DIVEQU = 356,                  /* "/="  */
    MULEQU = 357,                  /* "*="  */
    MODEQU = 358,                  /* "%="  */
    ANDEQU = 359,                  /* "&="  */
    OREQU = 360,                   /* "|="  */
    XOREQU = 361,                  /* "^="  */
    SHL = 362,                     /* "<<"  */
    SHR = 363,                     /* ">>"  */
    ADDADD = 364,                  /* "++"  */
    SUBSUB = 365,                  /* "--"  */
    LEEQU = 366,                   /* "<="  */
    SHLEQU = 367,                  /* "<<="  */
    SHREQU = 368,                  /* ">>="  */
    GREQU = 369,                   /* ">="  */
    EQUEQU = 370,                  /* "=="  */
    NOTEQU = 371,                  /* "!="  */
    RARROW = 372,                  /* "->"  */
    LARROW = 373,                  /* "<-"  */
    QQ = 374,                      /* "??"  */
    QDOT = 375,                    /* "?."  */
    QBRA = 376,                    /* "?["  */
    LPIPE = 377,                   /* "<|"  */
    LBPIPE = 378,                  /* " <|"  */
    LAPIPE = 379,                  /* "@ <|"  */
    LFPIPE = 380,                  /* "@@ <|"  */
    RPIPE = 381,                   /* "|>"  */
    CLONEEQU = 382,                /* ":="  */
    ROTL = 383,                    /* "<<<"  */
    ROTR = 384,                    /* ">>>"  */
    ROTLEQU = 385,                 /* "<<<="  */
    ROTREQU = 386,                 /* ">>>="  */
    MAPTO = 387,                   /* "=>"  */
    COLCOL = 388,                  /* "::"  */
    ANDAND = 389,                  /* "&&"  */
    OROR = 390,                    /* "||"  */
    XORXOR = 391,                  /* "^^"  */
    ANDANDEQU = 392,               /* "&&="  */
    OROREQU = 393,                 /* "||="  */
    XORXOREQU = 394,               /* "^^="  */
    DOTDOT = 395,                  /* ".."  */
    MTAG_E = 396,                  /* "$$"  */
    MTAG_I = 397,                  /* "$i"  */
    MTAG_V = 398,                  /* "$v"  */
    MTAG_B = 399,                  /* "$b"  */
    MTAG_A = 400,                  /* "$a"  */
    MTAG_T = 401,                  /* "$t"  */
    MTAG_C = 402,                  /* "$c"  */
    MTAG_F = 403,                  /* "$f"  */
    MTAG_DOTDOTDOT = 404,          /* "..."  */
    BRABRAB = 405,                 /* "[["  */
    BRACBRB = 406,                 /* "[{"  */
    CBRCBRB = 407,                 /* "{{"  */
    INTEGER = 408,                 /* "integer constant"  */
    LONG_INTEGER = 409,            /* "long integer constant"  */
    UNSIGNED_INTEGER = 410,        /* "unsigned integer constant"  */
    UNSIGNED_LONG_INTEGER = 411,   /* "unsigned long integer constant"  */
    FLOAT = 412,                   /* "floating point constant"  */
    DOUBLE = 413,                  /* "double constant"  */
    NAME = 414,                    /* "name"  */
    KEYWORD = 415,                 /* "keyword"  */
    BEGIN_STRING = 416,            /* "start of the string"  */
    STRING_CHARACTER = 417,        /* STRING_CHARACTER  */
    STRING_CHARACTER_ESC = 418,    /* STRING_CHARACTER_ESC  */
    END_STRING = 419,              /* "end of the string"  */
    BEGIN_STRING_EXPR = 420,       /* "{"  */
    END_STRING_EXPR = 421,         /* "}"  */
    END_OF_READ = 422,             /* "end of failed eader macro"  */
    UNARY_MINUS = 423,             /* UNARY_MINUS  */
    UNARY_PLUS = 424,              /* UNARY_PLUS  */
    PRE_INC = 425,                 /* PRE_INC  */
    PRE_DEC = 426,                 /* PRE_DEC  */
    POST_INC = 427,                /* POST_INC  */
    POST_DEC = 428,                /* POST_DEC  */
    DEREF = 429                    /* DEREF  */
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
