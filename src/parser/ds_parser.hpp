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
    DAS_TYPEDECL = 295,            /* "typedecl"  */
    DAS_WITH = 296,                /* "with"  */
    DAS_AKA = 297,                 /* "aka"  */
    DAS_ASSUME = 298,              /* "assume"  */
    DAS_CAST = 299,                /* "cast"  */
    DAS_OVERRIDE = 300,            /* "override"  */
    DAS_ABSTRACT = 301,            /* "abstract"  */
    DAS_UPCAST = 302,              /* "upcast"  */
    DAS_ITERATOR = 303,            /* "iterator"  */
    DAS_VAR = 304,                 /* "var"  */
    DAS_ADDR = 305,                /* "addr"  */
    DAS_CONTINUE = 306,            /* "continue"  */
    DAS_WHERE = 307,               /* "where"  */
    DAS_PASS = 308,                /* "pass"  */
    DAS_REINTERPRET = 309,         /* "reinterpret"  */
    DAS_MODULE = 310,              /* "module"  */
    DAS_PUBLIC = 311,              /* "public"  */
    DAS_LABEL = 312,               /* "label"  */
    DAS_GOTO = 313,                /* "goto"  */
    DAS_IMPLICIT = 314,            /* "implicit"  */
    DAS_EXPLICIT = 315,            /* "explicit"  */
    DAS_SHARED = 316,              /* "shared"  */
    DAS_PRIVATE = 317,             /* "private"  */
    DAS_SMART_PTR = 318,           /* "smart_ptr"  */
    DAS_UNSAFE = 319,              /* "unsafe"  */
    DAS_INSCOPE = 320,             /* "inscope"  */
    DAS_STATIC = 321,              /* "static"  */
    DAS_FIXED_ARRAY = 322,         /* "fixed_array"  */
    DAS_DEFAULT = 323,             /* "default"  */
    DAS_TBOOL = 324,               /* "bool"  */
    DAS_TVOID = 325,               /* "void"  */
    DAS_TSTRING = 326,             /* "string"  */
    DAS_TAUTO = 327,               /* "auto"  */
    DAS_TINT = 328,                /* "int"  */
    DAS_TINT2 = 329,               /* "int2"  */
    DAS_TINT3 = 330,               /* "int3"  */
    DAS_TINT4 = 331,               /* "int4"  */
    DAS_TUINT = 332,               /* "uint"  */
    DAS_TBITFIELD = 333,           /* "bitfield"  */
    DAS_TUINT2 = 334,              /* "uint2"  */
    DAS_TUINT3 = 335,              /* "uint3"  */
    DAS_TUINT4 = 336,              /* "uint4"  */
    DAS_TFLOAT = 337,              /* "float"  */
    DAS_TFLOAT2 = 338,             /* "float2"  */
    DAS_TFLOAT3 = 339,             /* "float3"  */
    DAS_TFLOAT4 = 340,             /* "float4"  */
    DAS_TRANGE = 341,              /* "range"  */
    DAS_TURANGE = 342,             /* "urange"  */
    DAS_TRANGE64 = 343,            /* "range64"  */
    DAS_TURANGE64 = 344,           /* "urange64"  */
    DAS_TBLOCK = 345,              /* "block"  */
    DAS_TINT64 = 346,              /* "int64"  */
    DAS_TUINT64 = 347,             /* "uint64"  */
    DAS_TDOUBLE = 348,             /* "double"  */
    DAS_TFUNCTION = 349,           /* "function"  */
    DAS_TLAMBDA = 350,             /* "lambda"  */
    DAS_TINT8 = 351,               /* "int8"  */
    DAS_TUINT8 = 352,              /* "uint8"  */
    DAS_TINT16 = 353,              /* "int16"  */
    DAS_TUINT16 = 354,             /* "uint16"  */
    DAS_TTUPLE = 355,              /* "tuple"  */
    DAS_TVARIANT = 356,            /* "variant"  */
    DAS_GENERATOR = 357,           /* "generator"  */
    DAS_YIELD = 358,               /* "yield"  */
    DAS_SEALED = 359,              /* "sealed"  */
    ADDEQU = 360,                  /* "+="  */
    SUBEQU = 361,                  /* "-="  */
    DIVEQU = 362,                  /* "/="  */
    MULEQU = 363,                  /* "*="  */
    MODEQU = 364,                  /* "%="  */
    ANDEQU = 365,                  /* "&="  */
    OREQU = 366,                   /* "|="  */
    XOREQU = 367,                  /* "^="  */
    SHL = 368,                     /* "<<"  */
    SHR = 369,                     /* ">>"  */
    ADDADD = 370,                  /* "++"  */
    SUBSUB = 371,                  /* "--"  */
    LEEQU = 372,                   /* "<="  */
    SHLEQU = 373,                  /* "<<="  */
    SHREQU = 374,                  /* ">>="  */
    GREQU = 375,                   /* ">="  */
    EQUEQU = 376,                  /* "=="  */
    NOTEQU = 377,                  /* "!="  */
    RARROW = 378,                  /* "->"  */
    LARROW = 379,                  /* "<-"  */
    QQ = 380,                      /* "??"  */
    QDOT = 381,                    /* "?."  */
    QBRA = 382,                    /* "?["  */
    LPIPE = 383,                   /* "<|"  */
    LBPIPE = 384,                  /* " <|"  */
    LLPIPE = 385,                  /* "$ <|"  */
    LAPIPE = 386,                  /* "@ <|"  */
    LFPIPE = 387,                  /* "@@ <|"  */
    RPIPE = 388,                   /* "|>"  */
    CLONEEQU = 389,                /* ":="  */
    ROTL = 390,                    /* "<<<"  */
    ROTR = 391,                    /* ">>>"  */
    ROTLEQU = 392,                 /* "<<<="  */
    ROTREQU = 393,                 /* ">>>="  */
    MAPTO = 394,                   /* "=>"  */
    COLCOL = 395,                  /* "::"  */
    ANDAND = 396,                  /* "&&"  */
    OROR = 397,                    /* "||"  */
    XORXOR = 398,                  /* "^^"  */
    ANDANDEQU = 399,               /* "&&="  */
    OROREQU = 400,                 /* "||="  */
    XORXOREQU = 401,               /* "^^="  */
    DOTDOT = 402,                  /* ".."  */
    MTAG_E = 403,                  /* "$$"  */
    MTAG_I = 404,                  /* "$i"  */
    MTAG_V = 405,                  /* "$v"  */
    MTAG_B = 406,                  /* "$b"  */
    MTAG_A = 407,                  /* "$a"  */
    MTAG_T = 408,                  /* "$t"  */
    MTAG_C = 409,                  /* "$c"  */
    MTAG_F = 410,                  /* "$f"  */
    MTAG_DOTDOTDOT = 411,          /* "..."  */
    BRABRAB = 412,                 /* "[["  */
    BRACBRB = 413,                 /* "[{"  */
    CBRCBRB = 414,                 /* "{{"  */
    INTEGER = 415,                 /* "integer constant"  */
    LONG_INTEGER = 416,            /* "long integer constant"  */
    UNSIGNED_INTEGER = 417,        /* "unsigned integer constant"  */
    UNSIGNED_LONG_INTEGER = 418,   /* "unsigned long integer constant"  */
    UNSIGNED_INT8 = 419,           /* "unsigned int8 constant"  */
    FLOAT = 420,                   /* "floating point constant"  */
    DOUBLE = 421,                  /* "double constant"  */
    NAME = 422,                    /* "name"  */
    KEYWORD = 423,                 /* "keyword"  */
    TYPE_FUNCTION = 424,           /* "type function"  */
    BEGIN_STRING = 425,            /* "start of the string"  */
    STRING_CHARACTER = 426,        /* STRING_CHARACTER  */
    STRING_CHARACTER_ESC = 427,    /* STRING_CHARACTER_ESC  */
    END_STRING = 428,              /* "end of the string"  */
    BEGIN_STRING_EXPR = 429,       /* "{"  */
    END_STRING_EXPR = 430,         /* "}"  */
    END_OF_READ = 431,             /* "end of failed eader macro"  */
    SEMICOLON_CUR_CUR = 432,       /* ";}}"  */
    SEMICOLON_CUR_SQR = 433,       /* ";}]"  */
    SEMICOLON_SQR_SQR = 434,       /* ";]]"  */
    COMMA_SQR_SQR = 435,           /* ",]]"  */
    COMMA_CUR_SQR = 436,           /* ",}]"  */
    UNARY_MINUS = 437,             /* UNARY_MINUS  */
    UNARY_PLUS = 438,              /* UNARY_PLUS  */
    PRE_INC = 439,                 /* PRE_INC  */
    PRE_DEC = 440,                 /* PRE_DEC  */
    POST_INC = 441,                /* POST_INC  */
    POST_DEC = 442,                /* POST_DEC  */
    DEREF = 443                    /* DEREF  */
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
