/* A Bison parser, made by GNU Bison 3.8.2.  */

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
    DAS_INSCOPE = 319,             /* "inscope"  */
    DAS_STATIC = 320,              /* "static"  */
    DAS_FIXED_ARRAY = 321,         /* "fixed_array"  */
    DAS_TBOOL = 322,               /* "bool"  */
    DAS_TVOID = 323,               /* "void"  */
    DAS_TSTRING = 324,             /* "string"  */
    DAS_TAUTO = 325,               /* "auto"  */
    DAS_TINT = 326,                /* "int"  */
    DAS_TINT2 = 327,               /* "int2"  */
    DAS_TINT3 = 328,               /* "int3"  */
    DAS_TINT4 = 329,               /* "int4"  */
    DAS_TUINT = 330,               /* "uint"  */
    DAS_TBITFIELD = 331,           /* "bitfield"  */
    DAS_TUINT2 = 332,              /* "uint2"  */
    DAS_TUINT3 = 333,              /* "uint3"  */
    DAS_TUINT4 = 334,              /* "uint4"  */
    DAS_TFLOAT = 335,              /* "float"  */
    DAS_TFLOAT2 = 336,             /* "float2"  */
    DAS_TFLOAT3 = 337,             /* "float3"  */
    DAS_TFLOAT4 = 338,             /* "float4"  */
    DAS_TRANGE = 339,              /* "range"  */
    DAS_TURANGE = 340,             /* "urange"  */
    DAS_TRANGE64 = 341,            /* "range64"  */
    DAS_TURANGE64 = 342,           /* "urange64"  */
    DAS_TBLOCK = 343,              /* "block"  */
    DAS_TINT64 = 344,              /* "int64"  */
    DAS_TUINT64 = 345,             /* "uint64"  */
    DAS_TDOUBLE = 346,             /* "double"  */
    DAS_TFUNCTION = 347,           /* "function"  */
    DAS_TLAMBDA = 348,             /* "lambda"  */
    DAS_TINT8 = 349,               /* "int8"  */
    DAS_TUINT8 = 350,              /* "uint8"  */
    DAS_TINT16 = 351,              /* "int16"  */
    DAS_TUINT16 = 352,             /* "uint16"  */
    DAS_TTUPLE = 353,              /* "tuple"  */
    DAS_TVARIANT = 354,            /* "variant"  */
    DAS_GENERATOR = 355,           /* "generator"  */
    DAS_YIELD = 356,               /* "yield"  */
    DAS_SEALED = 357,              /* "sealed"  */
    ADDEQU = 358,                  /* "+="  */
    SUBEQU = 359,                  /* "-="  */
    DIVEQU = 360,                  /* "/="  */
    MULEQU = 361,                  /* "*="  */
    MODEQU = 362,                  /* "%="  */
    ANDEQU = 363,                  /* "&="  */
    OREQU = 364,                   /* "|="  */
    XOREQU = 365,                  /* "^="  */
    SHL = 366,                     /* "<<"  */
    SHR = 367,                     /* ">>"  */
    ADDADD = 368,                  /* "++"  */
    SUBSUB = 369,                  /* "--"  */
    LEEQU = 370,                   /* "<="  */
    SHLEQU = 371,                  /* "<<="  */
    SHREQU = 372,                  /* ">>="  */
    GREQU = 373,                   /* ">="  */
    EQUEQU = 374,                  /* "=="  */
    NOTEQU = 375,                  /* "!="  */
    RARROW = 376,                  /* "->"  */
    LARROW = 377,                  /* "<-"  */
    QQ = 378,                      /* "??"  */
    QDOT = 379,                    /* "?."  */
    QBRA = 380,                    /* "?["  */
    LPIPE = 381,                   /* "<|"  */
    LBPIPE = 382,                  /* " <|"  */
    LLPIPE = 383,                  /* "$ <|"  */
    LAPIPE = 384,                  /* "@ <|"  */
    LFPIPE = 385,                  /* "@@ <|"  */
    RPIPE = 386,                   /* "|>"  */
    CLONEEQU = 387,                /* ":="  */
    ROTL = 388,                    /* "<<<"  */
    ROTR = 389,                    /* ">>>"  */
    ROTLEQU = 390,                 /* "<<<="  */
    ROTREQU = 391,                 /* ">>>="  */
    MAPTO = 392,                   /* "=>"  */
    COLCOL = 393,                  /* "::"  */
    ANDAND = 394,                  /* "&&"  */
    OROR = 395,                    /* "||"  */
    XORXOR = 396,                  /* "^^"  */
    ANDANDEQU = 397,               /* "&&="  */
    OROREQU = 398,                 /* "||="  */
    XORXOREQU = 399,               /* "^^="  */
    DOTDOT = 400,                  /* ".."  */
    MTAG_E = 401,                  /* "$$"  */
    MTAG_I = 402,                  /* "$i"  */
    MTAG_V = 403,                  /* "$v"  */
    MTAG_B = 404,                  /* "$b"  */
    MTAG_A = 405,                  /* "$a"  */
    MTAG_T = 406,                  /* "$t"  */
    MTAG_C = 407,                  /* "$c"  */
    MTAG_F = 408,                  /* "$f"  */
    MTAG_DOTDOTDOT = 409,          /* "..."  */
    BRABRAB = 410,                 /* "[["  */
    BRACBRB = 411,                 /* "[{"  */
    CBRCBRB = 412,                 /* "{{"  */
    INTEGER = 413,                 /* "integer constant"  */
    LONG_INTEGER = 414,            /* "long integer constant"  */
    UNSIGNED_INTEGER = 415,        /* "unsigned integer constant"  */
    UNSIGNED_LONG_INTEGER = 416,   /* "unsigned long integer constant"  */
    UNSIGNED_INT8 = 417,           /* "unsigned int8 constant"  */
    FLOAT = 418,                   /* "floating point constant"  */
    DOUBLE = 419,                  /* "double constant"  */
    NAME = 420,                    /* "name"  */
    KEYWORD = 421,                 /* "keyword"  */
    BEGIN_STRING = 422,            /* "start of the string"  */
    STRING_CHARACTER = 423,        /* STRING_CHARACTER  */
    STRING_CHARACTER_ESC = 424,    /* STRING_CHARACTER_ESC  */
    END_STRING = 425,              /* "end of the string"  */
    BEGIN_STRING_EXPR = 426,       /* "{"  */
    END_STRING_EXPR = 427,         /* "}"  */
    END_OF_READ = 428,             /* "end of failed eader macro"  */
    SEMICOLON_CUR_CUR = 429,       /* ";}}"  */
    SEMICOLON_CUR_SQR = 430,       /* ";}]"  */
    SEMICOLON_SQR_SQR = 431,       /* ";]]"  */
    COMMA_SQR_SQR = 432,           /* ",]]"  */
    COMMA_CUR_SQR = 433,           /* ",}]"  */
    UNARY_MINUS = 434,             /* UNARY_MINUS  */
    UNARY_PLUS = 435,              /* UNARY_PLUS  */
    PRE_INC = 436,                 /* PRE_INC  */
    PRE_DEC = 437,                 /* PRE_DEC  */
    POST_INC = 438,                /* POST_INC  */
    POST_DEC = 439,                /* POST_DEC  */
    DEREF = 440                    /* DEREF  */
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
