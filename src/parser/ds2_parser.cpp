/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         DAS2_YYSTYPE
#define YYLTYPE         DAS2_YYLTYPE
/* Substitute the variable and function names.  */
#define yyparse         das2_yyparse
#define yylex           das2_yylex
#define yyerror         das2_yyerror
#define yydebug         das2_yydebug
#define yynerrs         das2_yynerrs

/* First part of user prologue.  */

    #include "daScript/misc/platform.h"
    #include "daScript/simulate/debug_info.h"
    #include "daScript/ast/compilation_errors.h"

    #ifdef _MSC_VER
    #pragma warning(disable:4262)
    #pragma warning(disable:4127)
    #pragma warning(disable:4702)
    #endif

    using namespace das;

    union DAS2_YYSTYPE;
    struct DAS2_YYLTYPE;

    #define YY_NO_UNISTD_H
    #include "lex2.yy.h"

    void das2_yyerror ( DAS2_YYLTYPE * lloc, yyscan_t scanner, const string & error );
    void das2_yyfatalerror ( DAS2_YYLTYPE * lloc, yyscan_t scanner, const string & error, CompilationError cerr );
    int yylex ( DAS2_YYSTYPE *lvalp, DAS2_YYLTYPE *llocp, yyscan_t scanner );
    void yybegin ( const char * str );

    void das2_yybegin_reader ( yyscan_t yyscanner );
    void das2_yyend_reader ( yyscan_t yyscanner );
    void das2_accept_sequence ( yyscan_t yyscanner, const char * seq, size_t seqLen, int lineNo, FileInfo * info, const LineInfo & macroAt );
    void das2_strfmt ( yyscan_t yyscanner );

    namespace das { class Module; }
    void das2_collect_keywords ( das::Module * mod, yyscan_t yyscanner );

    #undef yyextra
    #define yyextra (*((das::DasParserState **)(scanner)))

    #define DAS2_LOC_EMPTY(L) ((L).first_line == (L).last_line && (L).first_column == (L).last_column)
    #define YYLLOC_DEFAULT(Current, Rhs, N) do {                                        \
        int lo_ = 1, hi_ = int(N);                                                      \
        while ( lo_ < hi_ && DAS2_LOC_EMPTY(YYRHSLOC(Rhs, lo_)) ) lo_++;                \
        while ( hi_ > lo_ && DAS2_LOC_EMPTY(YYRHSLOC(Rhs, hi_)) ) hi_--;                \
        if ( N ) {                                                                      \
            (Current).first_line   = YYRHSLOC(Rhs, lo_).first_line;                     \
            (Current).first_column = YYRHSLOC(Rhs, lo_).first_column;                   \
            (Current).last_line    = YYRHSLOC(Rhs, hi_).last_line;                      \
            (Current).last_column  = YYRHSLOC(Rhs, hi_).last_column;                    \
        } else {                                                                        \
            (Current).first_line   = (Current).last_line   = YYRHSLOC(Rhs, 0).last_line;    \
            (Current).first_column = (Current).last_column = YYRHSLOC(Rhs, 0).last_column;  \
        }                                                                               \
    } while (0)


# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "ds2_parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_LEXER_ERROR = 3,                /* "lexer error"  */
  YYSYMBOL_DAS_CAPTURE = 4,                /* "capture"  */
  YYSYMBOL_DAS_STRUCT = 5,                 /* "struct"  */
  YYSYMBOL_DAS_CLASS = 6,                  /* "class"  */
  YYSYMBOL_DAS_LET = 7,                    /* "let"  */
  YYSYMBOL_DAS_DEF = 8,                    /* "def"  */
  YYSYMBOL_DAS_WHILE = 9,                  /* "while"  */
  YYSYMBOL_DAS_IF = 10,                    /* "if"  */
  YYSYMBOL_DAS_STATIC_IF = 11,             /* "static_if"  */
  YYSYMBOL_DAS_ELSE = 12,                  /* "else"  */
  YYSYMBOL_DAS_FOR = 13,                   /* "for"  */
  YYSYMBOL_DAS_CATCH = 14,                 /* "recover"  */
  YYSYMBOL_DAS_TRUE = 15,                  /* "true"  */
  YYSYMBOL_DAS_FALSE = 16,                 /* "false"  */
  YYSYMBOL_DAS_NEWT = 17,                  /* "new"  */
  YYSYMBOL_DAS_TYPEINFO = 18,              /* "typeinfo"  */
  YYSYMBOL_DAS_TYPE = 19,                  /* "type"  */
  YYSYMBOL_DAS_IN = 20,                    /* "in"  */
  YYSYMBOL_DAS_IS = 21,                    /* "is"  */
  YYSYMBOL_DAS_AS = 22,                    /* "as"  */
  YYSYMBOL_DAS_ELIF = 23,                  /* "elif"  */
  YYSYMBOL_DAS_STATIC_ELIF = 24,           /* "static_elif"  */
  YYSYMBOL_DAS_ARRAY = 25,                 /* "array"  */
  YYSYMBOL_DAS_RETURN = 26,                /* "return"  */
  YYSYMBOL_DAS_NULL = 27,                  /* "null"  */
  YYSYMBOL_DAS_BREAK = 28,                 /* "break"  */
  YYSYMBOL_DAS_TRY = 29,                   /* "try"  */
  YYSYMBOL_DAS_OPTIONS = 30,               /* "options"  */
  YYSYMBOL_DAS_TABLE = 31,                 /* "table"  */
  YYSYMBOL_DAS_EXPECT = 32,                /* "expect"  */
  YYSYMBOL_DAS_CONST = 33,                 /* "const"  */
  YYSYMBOL_DAS_REQUIRE = 34,               /* "require"  */
  YYSYMBOL_DAS_OPERATOR = 35,              /* "operator"  */
  YYSYMBOL_DAS_ENUM = 36,                  /* "enum"  */
  YYSYMBOL_DAS_FINALLY = 37,               /* "finally"  */
  YYSYMBOL_DAS_DELETE = 38,                /* "delete"  */
  YYSYMBOL_DAS_DEREF = 39,                 /* "deref"  */
  YYSYMBOL_DAS_TYPEDEF = 40,               /* "typedef"  */
  YYSYMBOL_DAS_TYPEDECL = 41,              /* "typedecl"  */
  YYSYMBOL_DAS_WITH = 42,                  /* "with"  */
  YYSYMBOL_DAS_AKA = 43,                   /* "aka"  */
  YYSYMBOL_DAS_ASSUME = 44,                /* "assume"  */
  YYSYMBOL_DAS_CAST = 45,                  /* "cast"  */
  YYSYMBOL_DAS_OVERRIDE = 46,              /* "override"  */
  YYSYMBOL_DAS_ABSTRACT = 47,              /* "abstract"  */
  YYSYMBOL_DAS_UPCAST = 48,                /* "upcast"  */
  YYSYMBOL_DAS_ITERATOR = 49,              /* "iterator"  */
  YYSYMBOL_DAS_VAR = 50,                   /* "var"  */
  YYSYMBOL_DAS_ADDR = 51,                  /* "addr"  */
  YYSYMBOL_DAS_CONTINUE = 52,              /* "continue"  */
  YYSYMBOL_DAS_WHERE = 53,                 /* "where"  */
  YYSYMBOL_DAS_PASS = 54,                  /* "pass"  */
  YYSYMBOL_DAS_REINTERPRET = 55,           /* "reinterpret"  */
  YYSYMBOL_DAS_MODULE = 56,                /* "module"  */
  YYSYMBOL_DAS_PUBLIC = 57,                /* "public"  */
  YYSYMBOL_DAS_LABEL = 58,                 /* "label"  */
  YYSYMBOL_DAS_GOTO = 59,                  /* "goto"  */
  YYSYMBOL_DAS_IMPLICIT = 60,              /* "implicit"  */
  YYSYMBOL_DAS_EXPLICIT = 61,              /* "explicit"  */
  YYSYMBOL_DAS_SHARED = 62,                /* "shared"  */
  YYSYMBOL_DAS_PRIVATE = 63,               /* "private"  */
  YYSYMBOL_DAS_SMART_PTR = 64,             /* "smart_ptr"  */
  YYSYMBOL_DAS_UNSAFE = 65,                /* "unsafe"  */
  YYSYMBOL_DAS_INSCOPE = 66,               /* "inscope"  */
  YYSYMBOL_DAS_STATIC = 67,                /* "static"  */
  YYSYMBOL_DAS_FIXED_ARRAY = 68,           /* "fixed_array"  */
  YYSYMBOL_DAS_DEFAULT = 69,               /* "default"  */
  YYSYMBOL_DAS_UNINITIALIZED = 70,         /* "uninitialized"  */
  YYSYMBOL_DAS_TBOOL = 71,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 72,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 73,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 74,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 75,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 76,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 77,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 78,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 79,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 80,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 81,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 82,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 83,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 84,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 85,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 86,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 87,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 88,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 89,               /* "urange"  */
  YYSYMBOL_DAS_TRANGE64 = 90,              /* "range64"  */
  YYSYMBOL_DAS_TURANGE64 = 91,             /* "urange64"  */
  YYSYMBOL_DAS_TBLOCK = 92,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 93,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 94,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 95,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 96,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 97,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 98,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 99,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 100,               /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 101,              /* "uint16"  */
  YYSYMBOL_DAS_TFLOAT16 = 102,             /* "float16"  */
  YYSYMBOL_DAS_THALF2 = 103,               /* "half2"  */
  YYSYMBOL_DAS_THALF3 = 104,               /* "half3"  */
  YYSYMBOL_DAS_THALF4 = 105,               /* "half4"  */
  YYSYMBOL_DAS_THALF8 = 106,               /* "half8"  */
  YYSYMBOL_DAS_TSHORT2 = 107,              /* "short2"  */
  YYSYMBOL_DAS_TSHORT3 = 108,              /* "short3"  */
  YYSYMBOL_DAS_TSHORT4 = 109,              /* "short4"  */
  YYSYMBOL_DAS_TSHORT8 = 110,              /* "short8"  */
  YYSYMBOL_DAS_TUSHORT2 = 111,             /* "ushort2"  */
  YYSYMBOL_DAS_TUSHORT3 = 112,             /* "ushort3"  */
  YYSYMBOL_DAS_TUSHORT4 = 113,             /* "ushort4"  */
  YYSYMBOL_DAS_TUSHORT8 = 114,             /* "ushort8"  */
  YYSYMBOL_DAS_TBYTE2 = 115,               /* "byte2"  */
  YYSYMBOL_DAS_TBYTE3 = 116,               /* "byte3"  */
  YYSYMBOL_DAS_TBYTE4 = 117,               /* "byte4"  */
  YYSYMBOL_DAS_TBYTE8 = 118,               /* "byte8"  */
  YYSYMBOL_DAS_TBYTE16 = 119,              /* "byte16"  */
  YYSYMBOL_DAS_TUBYTE2 = 120,              /* "ubyte2"  */
  YYSYMBOL_DAS_TUBYTE3 = 121,              /* "ubyte3"  */
  YYSYMBOL_DAS_TUBYTE4 = 122,              /* "ubyte4"  */
  YYSYMBOL_DAS_TUBYTE8 = 123,              /* "ubyte8"  */
  YYSYMBOL_DAS_TUBYTE16 = 124,             /* "ubyte16"  */
  YYSYMBOL_DAS_TTUPLE = 125,               /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 126,             /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 127,            /* "generator"  */
  YYSYMBOL_DAS_YIELD = 128,                /* "yield"  */
  YYSYMBOL_DAS_SEALED = 129,               /* "sealed"  */
  YYSYMBOL_DAS_TEMPLATE = 130,             /* "template"  */
  YYSYMBOL_ADDEQU = 131,                   /* "+="  */
  YYSYMBOL_SUBEQU = 132,                   /* "-="  */
  YYSYMBOL_DIVEQU = 133,                   /* "/="  */
  YYSYMBOL_MULEQU = 134,                   /* "*="  */
  YYSYMBOL_MODEQU = 135,                   /* "%="  */
  YYSYMBOL_ANDEQU = 136,                   /* "&="  */
  YYSYMBOL_OREQU = 137,                    /* "|="  */
  YYSYMBOL_XOREQU = 138,                   /* "^="  */
  YYSYMBOL_SHL = 139,                      /* "<<"  */
  YYSYMBOL_SHR = 140,                      /* ">>"  */
  YYSYMBOL_ADDADD = 141,                   /* "++"  */
  YYSYMBOL_SUBSUB = 142,                   /* "--"  */
  YYSYMBOL_LEEQU = 143,                    /* "<="  */
  YYSYMBOL_SHLEQU = 144,                   /* "<<="  */
  YYSYMBOL_SHREQU = 145,                   /* ">>="  */
  YYSYMBOL_GREQU = 146,                    /* ">="  */
  YYSYMBOL_EQUEQU = 147,                   /* "=="  */
  YYSYMBOL_NOTEQU = 148,                   /* "!="  */
  YYSYMBOL_RARROW = 149,                   /* "->"  */
  YYSYMBOL_LARROW = 150,                   /* "<-"  */
  YYSYMBOL_QQ = 151,                       /* "??"  */
  YYSYMBOL_QDOT = 152,                     /* "?."  */
  YYSYMBOL_QBRA = 153,                     /* "?["  */
  YYSYMBOL_NOTDOT = 154,                   /* "!."  */
  YYSYMBOL_NOTQDOT = 155,                  /* "!?."  */
  YYSYMBOL_NOTBRA = 156,                   /* "!["  */
  YYSYMBOL_NOTQBRA = 157,                  /* "!?["  */
  YYSYMBOL_NOTQQ = 158,                    /* "!??"  */
  YYSYMBOL_NOTEQUEQU = 159,                /* "!=="  */
  YYSYMBOL_NOTLARROW = 160,                /* "!<-"  */
  YYSYMBOL_NOTCLONEEQU = 161,              /* "!:="  */
  YYSYMBOL_NOTIS = 162,                    /* "!is"  */
  YYSYMBOL_NOTAS = 163,                    /* "!as"  */
  YYSYMBOL_NOTQAS = 164,                   /* "!?as"  */
  YYSYMBOL_LPIPE = 165,                    /* "<|"  */
  YYSYMBOL_RPIPE = 166,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 167,                 /* ":="  */
  YYSYMBOL_ROTL = 168,                     /* "<<<"  */
  YYSYMBOL_ROTR = 169,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 170,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 171,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 172,                    /* "=>"  */
  YYSYMBOL_DOUBLE_AT = 173,                /* "@@"  */
  YYSYMBOL_AT_FIELD = 174,                 /* "@field"  */
  YYSYMBOL_COLCOL = 175,                   /* "::"  */
  YYSYMBOL_ANDAND = 176,                   /* "&&"  */
  YYSYMBOL_OROR = 177,                     /* "||"  */
  YYSYMBOL_XORXOR = 178,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 179,                /* "&&="  */
  YYSYMBOL_OROREQU = 180,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 181,                /* "^^="  */
  YYSYMBOL_DOTDOT = 182,                   /* ".."  */
  YYSYMBOL_MTAG_E = 183,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 184,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 185,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 186,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 187,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 188,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 189,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 190,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 191,           /* "..."  */
  YYSYMBOL_INTEGER = 192,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 193,             /* "long integer constant"  */
  YYSYMBOL_LONG_INTEGER_MIN = 194,         /* "int64 minimum magnitude (requires minus)"  */
  YYSYMBOL_UNSIGNED_INTEGER = 195,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 196,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 197,            /* "unsigned int8 constant"  */
  YYSYMBOL_DAS_FLOAT = 198,                /* "floating point constant"  */
  YYSYMBOL_DAS_FLOAT16_CONST = 199,        /* "float16 constant"  */
  YYSYMBOL_DOUBLE = 200,                   /* "double constant"  */
  YYSYMBOL_NAME = 201,                     /* "name"  */
  YYSYMBOL_DAS_EMIT_COMMA = 202,           /* "new line, comma"  */
  YYSYMBOL_DAS_EMIT_SEMICOLON = 203,       /* "new line, semicolon"  */
  YYSYMBOL_BEGIN_STRING = 204,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 205,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 206,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 207,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 208,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 209,          /* "}"  */
  YYSYMBOL_END_OF_READ = 210,              /* "end of failed eader macro"  */
  YYSYMBOL_211_ = 211,                     /* ','  */
  YYSYMBOL_212_ = 212,                     /* '='  */
  YYSYMBOL_213_ = 213,                     /* '?'  */
  YYSYMBOL_214_ = 214,                     /* ':'  */
  YYSYMBOL_215_ = 215,                     /* '|'  */
  YYSYMBOL_216_ = 216,                     /* '^'  */
  YYSYMBOL_217_ = 217,                     /* '&'  */
  YYSYMBOL_218_ = 218,                     /* '<'  */
  YYSYMBOL_219_ = 219,                     /* '>'  */
  YYSYMBOL_220_ = 220,                     /* '-'  */
  YYSYMBOL_221_ = 221,                     /* '+'  */
  YYSYMBOL_222_ = 222,                     /* '*'  */
  YYSYMBOL_223_ = 223,                     /* '/'  */
  YYSYMBOL_224_ = 224,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 225,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 226,               /* UNARY_PLUS  */
  YYSYMBOL_227_ = 227,                     /* '~'  */
  YYSYMBOL_228_ = 228,                     /* '!'  */
  YYSYMBOL_PRE_INC = 229,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 230,                  /* PRE_DEC  */
  YYSYMBOL_LLPIPE = 231,                   /* LLPIPE  */
  YYSYMBOL_POST_INC = 232,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 233,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 234,                    /* DEREF  */
  YYSYMBOL_235_ = 235,                     /* '.'  */
  YYSYMBOL_236_ = 236,                     /* '['  */
  YYSYMBOL_237_ = 237,                     /* ']'  */
  YYSYMBOL_238_ = 238,                     /* '('  */
  YYSYMBOL_239_ = 239,                     /* ')'  */
  YYSYMBOL_240_ = 240,                     /* '$'  */
  YYSYMBOL_241_ = 241,                     /* '@'  */
  YYSYMBOL_242_ = 242,                     /* ';'  */
  YYSYMBOL_243_ = 243,                     /* '{'  */
  YYSYMBOL_244_ = 244,                     /* '}'  */
  YYSYMBOL_245_ = 245,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 246,                 /* $accept  */
  YYSYMBOL_program = 247,                  /* program  */
  YYSYMBOL_COMMA = 248,                    /* COMMA  */
  YYSYMBOL_SEMICOLON = 249,                /* SEMICOLON  */
  YYSYMBOL_top_level_reader_macro = 250,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 251, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 252,              /* module_name  */
  YYSYMBOL_optional_not_required = 253,    /* optional_not_required  */
  YYSYMBOL_module_declaration = 254,       /* module_declaration  */
  YYSYMBOL_character_sequence = 255,       /* character_sequence  */
  YYSYMBOL_string_constant = 256,          /* string_constant  */
  YYSYMBOL_format_string = 257,            /* format_string  */
  YYSYMBOL_optional_format_string = 258,   /* optional_format_string  */
  YYSYMBOL_259_1 = 259,                    /* $@1  */
  YYSYMBOL_string_builder_body = 260,      /* string_builder_body  */
  YYSYMBOL_string_builder = 261,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 262, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 263,              /* expr_reader  */
  YYSYMBOL_264_2 = 264,                    /* $@2  */
  YYSYMBOL_options_declaration = 265,      /* options_declaration  */
  YYSYMBOL_require_declaration = 266,      /* require_declaration  */
  YYSYMBOL_require_module_name = 267,      /* require_module_name  */
  YYSYMBOL_optional_require_guard = 268,   /* optional_require_guard  */
  YYSYMBOL_require_module = 269,           /* require_module  */
  YYSYMBOL_is_public_module = 270,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 271,       /* expect_declaration  */
  YYSYMBOL_expect_list = 272,              /* expect_list  */
  YYSYMBOL_expect_error = 273,             /* expect_error  */
  YYSYMBOL_expression_label = 274,         /* expression_label  */
  YYSYMBOL_expression_goto = 275,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 276,      /* elif_or_static_elif  */
  YYSYMBOL_emit_semis = 277,               /* emit_semis  */
  YYSYMBOL_optional_emit_semis = 278,      /* optional_emit_semis  */
  YYSYMBOL_expression_else = 279,          /* expression_else  */
  YYSYMBOL_280_3 = 280,                    /* $@3  */
  YYSYMBOL_281_4 = 281,                    /* $@4  */
  YYSYMBOL_if_or_static_if = 282,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 283, /* expression_else_one_liner  */
  YYSYMBOL_expression_if_one_liner = 284,  /* expression_if_one_liner  */
  YYSYMBOL_semis = 285,                    /* semis  */
  YYSYMBOL_optional_semis = 286,           /* optional_semis  */
  YYSYMBOL_expression_if_block = 287,      /* expression_if_block  */
  YYSYMBOL_288_5 = 288,                    /* $@5  */
  YYSYMBOL_289_6 = 289,                    /* $@6  */
  YYSYMBOL_290_7 = 290,                    /* $@7  */
  YYSYMBOL_expression_else_block = 291,    /* expression_else_block  */
  YYSYMBOL_292_8 = 292,                    /* $@8  */
  YYSYMBOL_293_9 = 293,                    /* $@9  */
  YYSYMBOL_294_10 = 294,                   /* $@10  */
  YYSYMBOL_expression_if_then_else = 295,  /* expression_if_then_else  */
  YYSYMBOL_296_11 = 296,                   /* $@11  */
  YYSYMBOL_297_12 = 297,                   /* $@12  */
  YYSYMBOL_expression_if_then_else_oneliner = 298, /* expression_if_then_else_oneliner  */
  YYSYMBOL_for_variable_name_with_pos_list = 299, /* for_variable_name_with_pos_list  */
  YYSYMBOL_expression_for_loop = 300,      /* expression_for_loop  */
  YYSYMBOL_301_13 = 301,                   /* $@13  */
  YYSYMBOL_expression_unsafe = 302,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 303,    /* expression_while_loop  */
  YYSYMBOL_304_14 = 304,                   /* $@14  */
  YYSYMBOL_with_keyword_on = 305,          /* with_keyword_on  */
  YYSYMBOL_expression_with = 306,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 307,    /* expression_with_alias  */
  YYSYMBOL_annotation_argument_value = 308, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 309, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 310, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 311,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 312, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 313,   /* metadata_argument_list  */
  YYSYMBOL_optional_for_annotations = 314, /* optional_for_annotations  */
  YYSYMBOL_annotation_declaration_name = 315, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 316, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 317,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 318,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 319, /* optional_annotation_list  */
  YYSYMBOL_optional_annotation_list_with_emit_semis = 320, /* optional_annotation_list_with_emit_semis  */
  YYSYMBOL_optional_function_argument_list = 321, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 322,   /* optional_function_type  */
  YYSYMBOL_function_name = 323,            /* function_name  */
  YYSYMBOL_das_type_name = 324,            /* das_type_name  */
  YYSYMBOL_optional_template = 325,        /* optional_template  */
  YYSYMBOL_global_function_declaration = 326, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 327, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 328, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 329,     /* function_declaration  */
  YYSYMBOL_330_15 = 330,                   /* $@15  */
  YYSYMBOL_expression_block_finally = 331, /* expression_block_finally  */
  YYSYMBOL_332_16 = 332,                   /* $@16  */
  YYSYMBOL_333_17 = 333,                   /* $@17  */
  YYSYMBOL_expression_block = 334,         /* expression_block  */
  YYSYMBOL_335_18 = 335,                   /* $@18  */
  YYSYMBOL_336_19 = 336,                   /* $@19  */
  YYSYMBOL_expr_call_pipe_no_bracket = 337, /* expr_call_pipe_no_bracket  */
  YYSYMBOL_expression_any = 338,           /* expression_any  */
  YYSYMBOL_339_20 = 339,                   /* $@20  */
  YYSYMBOL_340_21 = 340,                   /* $@21  */
  YYSYMBOL_expressions = 341,              /* expressions  */
  YYSYMBOL_optional_expr_list = 342,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_map_tuple_list = 343, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 344, /* type_declaration_no_options_list  */
  YYSYMBOL_name_in_namespace = 345,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 346,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 347,     /* new_type_declaration  */
  YYSYMBOL_348_22 = 348,                   /* $@22  */
  YYSYMBOL_349_23 = 349,                   /* $@23  */
  YYSYMBOL_expr_new = 350,                 /* expr_new  */
  YYSYMBOL_expression_break = 351,         /* expression_break  */
  YYSYMBOL_expression_continue = 352,      /* expression_continue  */
  YYSYMBOL_expression_return = 353,        /* expression_return  */
  YYSYMBOL_expression_yield = 354,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 355,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 356,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 357,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 358,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 359,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 360, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 361,           /* expression_let  */
  YYSYMBOL_expr_cast = 362,                /* expr_cast  */
  YYSYMBOL_363_24 = 363,                   /* $@24  */
  YYSYMBOL_364_25 = 364,                   /* $@25  */
  YYSYMBOL_365_26 = 365,                   /* $@26  */
  YYSYMBOL_366_27 = 366,                   /* $@27  */
  YYSYMBOL_367_28 = 367,                   /* $@28  */
  YYSYMBOL_368_29 = 368,                   /* $@29  */
  YYSYMBOL_expr_type_decl = 369,           /* expr_type_decl  */
  YYSYMBOL_370_30 = 370,                   /* $@30  */
  YYSYMBOL_371_31 = 371,                   /* $@31  */
  YYSYMBOL_expr_type_info = 372,           /* expr_type_info  */
  YYSYMBOL_expr_list = 373,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 374,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 375,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 376,            /* capture_entry  */
  YYSYMBOL_capture_list = 377,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 378,    /* optional_capture_list  */
  YYSYMBOL_expr_full_block = 379,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 380, /* expr_full_block_assumed_piped  */
  YYSYMBOL_expr_numeric_const = 381,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign_no_bracket = 382,   /* expr_assign_no_bracket  */
  YYSYMBOL_expr_named_call = 383,          /* expr_named_call  */
  YYSYMBOL_expr_method_call_no_bracket = 384, /* expr_method_call_no_bracket  */
  YYSYMBOL_func_addr_name = 385,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 386,           /* func_addr_expr  */
  YYSYMBOL_387_32 = 387,                   /* $@32  */
  YYSYMBOL_388_33 = 388,                   /* $@33  */
  YYSYMBOL_389_34 = 389,                   /* $@34  */
  YYSYMBOL_390_35 = 390,                   /* $@35  */
  YYSYMBOL_expr_field_no_bracket = 391,    /* expr_field_no_bracket  */
  YYSYMBOL_392_36 = 392,                   /* $@36  */
  YYSYMBOL_393_37 = 393,                   /* $@37  */
  YYSYMBOL_expr_call = 394,                /* expr_call  */
  YYSYMBOL_expr = 395,                     /* expr  */
  YYSYMBOL_expr_no_bracket = 396,          /* expr_no_bracket  */
  YYSYMBOL_397_38 = 397,                   /* $@38  */
  YYSYMBOL_398_39 = 398,                   /* $@39  */
  YYSYMBOL_399_40 = 399,                   /* $@40  */
  YYSYMBOL_400_41 = 400,                   /* $@41  */
  YYSYMBOL_401_42 = 401,                   /* $@42  */
  YYSYMBOL_402_43 = 402,                   /* $@43  */
  YYSYMBOL_403_44 = 403,                   /* $@44  */
  YYSYMBOL_404_45 = 404,                   /* $@45  */
  YYSYMBOL_405_46 = 405,                   /* $@46  */
  YYSYMBOL_406_47 = 406,                   /* $@47  */
  YYSYMBOL_407_48 = 407,                   /* $@48  */
  YYSYMBOL_408_49 = 408,                   /* $@49  */
  YYSYMBOL_409_50 = 409,                   /* $@50  */
  YYSYMBOL_410_51 = 410,                   /* $@51  */
  YYSYMBOL_expr_generator = 411,           /* expr_generator  */
  YYSYMBOL_expr_mtag_no_bracket = 412,     /* expr_mtag_no_bracket  */
  YYSYMBOL_optional_field_annotation = 413, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 414,        /* optional_override  */
  YYSYMBOL_optional_constant = 415,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 416, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 417, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 418, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 419, /* struct_variable_declaration_list  */
  YYSYMBOL_420_52 = 420,                   /* $@52  */
  YYSYMBOL_421_53 = 421,                   /* $@53  */
  YYSYMBOL_422_54 = 422,                   /* $@54  */
  YYSYMBOL_function_argument_declaration_no_type = 423, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 424, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 425,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 426,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 427,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 428,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 429,             /* variant_type  */
  YYSYMBOL_variant_type_list = 430,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 431,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 432,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 433, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 434, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 435,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 436,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 437,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 438, /* let_variable_name_with_pos_list  */
  YYSYMBOL_global_let_variable_name_with_pos_list = 439, /* global_let_variable_name_with_pos_list  */
  YYSYMBOL_variable_declaration_list = 440, /* variable_declaration_list  */
  YYSYMBOL_let_variable_declaration = 441, /* let_variable_declaration  */
  YYSYMBOL_global_let_variable_declaration = 442, /* global_let_variable_declaration  */
  YYSYMBOL_optional_shared = 443,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 444, /* optional_public_or_private_variable  */
  YYSYMBOL_global_variable_declaration_list = 445, /* global_variable_declaration_list  */
  YYSYMBOL_446_55 = 446,                   /* $@55  */
  YYSYMBOL_global_let = 447,               /* global_let  */
  YYSYMBOL_448_56 = 448,                   /* $@56  */
  YYSYMBOL_enum_expression = 449,          /* enum_expression  */
  YYSYMBOL_commas = 450,                   /* commas  */
  YYSYMBOL_enum_list = 451,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 452, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 453,             /* single_alias  */
  YYSYMBOL_454_57 = 454,                   /* $@57  */
  YYSYMBOL_alias_declaration = 455,        /* alias_declaration  */
  YYSYMBOL_distinct_alias = 456,           /* distinct_alias  */
  YYSYMBOL_457_58 = 457,                   /* $@58  */
  YYSYMBOL_optional_public_or_private_enum = 458, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 459,                /* enum_name  */
  YYSYMBOL_optional_enum_basic_type_declaration = 460, /* optional_enum_basic_type_declaration  */
  YYSYMBOL_optional_commas = 461,          /* optional_commas  */
  YYSYMBOL_emit_commas = 462,              /* emit_commas  */
  YYSYMBOL_optional_emit_commas = 463,     /* optional_emit_commas  */
  YYSYMBOL_enum_declaration = 464,         /* enum_declaration  */
  YYSYMBOL_465_59 = 465,                   /* $@59  */
  YYSYMBOL_466_60 = 466,                   /* $@60  */
  YYSYMBOL_467_61 = 467,                   /* $@61  */
  YYSYMBOL_optional_structure_parent = 468, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 469,          /* optional_sealed  */
  YYSYMBOL_structure_name = 470,           /* structure_name  */
  YYSYMBOL_class_or_struct = 471,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 472, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 473, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 474,    /* structure_declaration  */
  YYSYMBOL_475_62 = 475,                   /* $@62  */
  YYSYMBOL_476_63 = 476,                   /* $@63  */
  YYSYMBOL_477_64 = 477,                   /* $@64  */
  YYSYMBOL_variable_name_with_pos_list = 478, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 479,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 480, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 481, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 482,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 483,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 484,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_basic_type_declaration = 485, /* bitfield_basic_type_declaration  */
  YYSYMBOL_bitfield_type_declaration = 486, /* bitfield_type_declaration  */
  YYSYMBOL_487_65 = 487,                   /* $@65  */
  YYSYMBOL_488_66 = 488,                   /* $@66  */
  YYSYMBOL_c_or_s = 489,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 490,          /* table_type_pair  */
  YYSYMBOL_dim_list = 491,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 492, /* type_declaration_no_options  */
  YYSYMBOL_optional_expr_list_in_braces = 493, /* optional_expr_list_in_braces  */
  YYSYMBOL_type_declaration_no_options_no_dim = 494, /* type_declaration_no_options_no_dim  */
  YYSYMBOL_495_67 = 495,                   /* $@67  */
  YYSYMBOL_496_68 = 496,                   /* $@68  */
  YYSYMBOL_497_69 = 497,                   /* $@69  */
  YYSYMBOL_498_70 = 498,                   /* $@70  */
  YYSYMBOL_499_71 = 499,                   /* $@71  */
  YYSYMBOL_500_72 = 500,                   /* $@72  */
  YYSYMBOL_501_73 = 501,                   /* $@73  */
  YYSYMBOL_502_74 = 502,                   /* $@74  */
  YYSYMBOL_503_75 = 503,                   /* $@75  */
  YYSYMBOL_504_76 = 504,                   /* $@76  */
  YYSYMBOL_505_77 = 505,                   /* $@77  */
  YYSYMBOL_506_78 = 506,                   /* $@78  */
  YYSYMBOL_507_79 = 507,                   /* $@79  */
  YYSYMBOL_508_80 = 508,                   /* $@80  */
  YYSYMBOL_509_81 = 509,                   /* $@81  */
  YYSYMBOL_510_82 = 510,                   /* $@82  */
  YYSYMBOL_511_83 = 511,                   /* $@83  */
  YYSYMBOL_512_84 = 512,                   /* $@84  */
  YYSYMBOL_513_85 = 513,                   /* $@85  */
  YYSYMBOL_514_86 = 514,                   /* $@86  */
  YYSYMBOL_515_87 = 515,                   /* $@87  */
  YYSYMBOL_516_88 = 516,                   /* $@88  */
  YYSYMBOL_517_89 = 517,                   /* $@89  */
  YYSYMBOL_518_90 = 518,                   /* $@90  */
  YYSYMBOL_519_91 = 519,                   /* $@91  */
  YYSYMBOL_520_92 = 520,                   /* $@92  */
  YYSYMBOL_521_93 = 521,                   /* $@93  */
  YYSYMBOL_522_94 = 522,                   /* $@94  */
  YYSYMBOL_523_95 = 523,                   /* $@95  */
  YYSYMBOL_524_96 = 524,                   /* $@96  */
  YYSYMBOL_type_declaration = 525,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 526,  /* tuple_alias_declaration  */
  YYSYMBOL_527_97 = 527,                   /* $@97  */
  YYSYMBOL_528_98 = 528,                   /* $@98  */
  YYSYMBOL_529_99 = 529,                   /* $@99  */
  YYSYMBOL_530_100 = 530,                  /* $@100  */
  YYSYMBOL_variant_alias_declaration = 531, /* variant_alias_declaration  */
  YYSYMBOL_532_101 = 532,                  /* $@101  */
  YYSYMBOL_533_102 = 533,                  /* $@102  */
  YYSYMBOL_534_103 = 534,                  /* $@103  */
  YYSYMBOL_535_104 = 535,                  /* $@104  */
  YYSYMBOL_bitfield_alias_declaration = 536, /* bitfield_alias_declaration  */
  YYSYMBOL_537_105 = 537,                  /* $@105  */
  YYSYMBOL_538_106 = 538,                  /* $@106  */
  YYSYMBOL_539_107 = 539,                  /* $@107  */
  YYSYMBOL_540_108 = 540,                  /* $@108  */
  YYSYMBOL_make_decl = 541,                /* make_decl  */
  YYSYMBOL_make_decl_no_bracket = 542,     /* make_decl_no_bracket  */
  YYSYMBOL_make_struct_fields = 543,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 544,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 545,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim_list = 546,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 547,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 548, /* optional_make_struct_dim_decl  */
  YYSYMBOL_use_initializer = 549,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 550,         /* make_struct_decl  */
  YYSYMBOL_551_109 = 551,                  /* $@109  */
  YYSYMBOL_552_110 = 552,                  /* $@110  */
  YYSYMBOL_553_111 = 553,                  /* $@111  */
  YYSYMBOL_554_112 = 554,                  /* $@112  */
  YYSYMBOL_555_113 = 555,                  /* $@113  */
  YYSYMBOL_556_114 = 556,                  /* $@114  */
  YYSYMBOL_557_115 = 557,                  /* $@115  */
  YYSYMBOL_558_116 = 558,                  /* $@116  */
  YYSYMBOL_559_117 = 559,                  /* $@117  */
  YYSYMBOL_560_118 = 560,                  /* $@118  */
  YYSYMBOL_make_tuple_call = 561,          /* make_tuple_call  */
  YYSYMBOL_562_119 = 562,                  /* $@119  */
  YYSYMBOL_563_120 = 563,                  /* $@120  */
  YYSYMBOL_make_dim_decl = 564,            /* make_dim_decl  */
  YYSYMBOL_565_121 = 565,                  /* $@121  */
  YYSYMBOL_566_122 = 566,                  /* $@122  */
  YYSYMBOL_567_123 = 567,                  /* $@123  */
  YYSYMBOL_568_124 = 568,                  /* $@124  */
  YYSYMBOL_569_125 = 569,                  /* $@125  */
  YYSYMBOL_570_126 = 570,                  /* $@126  */
  YYSYMBOL_571_127 = 571,                  /* $@127  */
  YYSYMBOL_572_128 = 572,                  /* $@128  */
  YYSYMBOL_573_129 = 573,                  /* $@129  */
  YYSYMBOL_574_130 = 574,                  /* $@130  */
  YYSYMBOL_expr_map_tuple_list = 575,      /* expr_map_tuple_list  */
  YYSYMBOL_push_table_nesting = 576,       /* push_table_nesting  */
  YYSYMBOL_make_table_decl = 577,          /* make_table_decl  */
  YYSYMBOL_make_table_call = 578,          /* make_table_call  */
  YYSYMBOL_array_comprehension_where = 579, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 580,           /* optional_comma  */
  YYSYMBOL_table_comprehension = 581,      /* table_comprehension  */
  YYSYMBOL_array_comprehension = 582       /* array_comprehension  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined DAS2_YYLTYPE_IS_TRIVIAL && DAS2_YYLTYPE_IS_TRIVIAL \
             && defined DAS2_YYSTYPE_IS_TRIVIAL && DAS2_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   13090

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  246
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  337
/* YYNRULES -- Number of rules.  */
#define YYNRULES  1068
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1895

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   473


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   228,     2,   245,   240,   224,   217,     2,
     238,   239,   222,   221,   211,   220,   235,   223,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   214,   242,
     218,   212,   219,   213,   241,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   236,     2,   237,   216,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   243,   215,   244,   227,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   225,   226,   229,   230,
     231,   232,   233,   234
};

#if DAS2_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   622,   622,   623,   628,   629,   630,   631,   632,   633,
     634,   635,   636,   637,   638,   639,   640,   644,   645,   649,
     650,   654,   660,   661,   662,   666,   667,   671,   672,   676,
     695,   696,   697,   698,   702,   703,   707,   708,   712,   713,
     713,   717,   720,   729,   744,   761,   766,   774,   774,   819,
     837,   841,   844,   848,   852,   856,   860,   866,   875,   876,
     880,   883,   886,   892,   893,   897,   901,   902,   906,   909,
     915,   921,   924,   930,   931,   935,   936,   940,   941,   945,
     946,   946,   950,   950,   959,   960,   964,   965,   971,   972,
     973,   974,   975,   979,   980,   984,   985,   989,   991,   989,
    1003,  1003,  1012,  1014,  1012,  1026,  1026,  1035,  1037,  1035,
    1048,  1055,  1062,  1067,  1076,  1084,  1090,  1094,  1102,  1112,
    1112,  1121,  1129,  1129,  1145,  1151,  1158,  1177,  1181,  1188,
    1189,  1190,  1191,  1192,  1193,  1194,  1195,  1196,  1197,  1198,
    1199,  1203,  1208,  1216,  1217,  1218,  1219,  1223,  1224,  1225,
    1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,
    1236,  1237,  1243,  1246,  1252,  1255,  1261,  1264,  1267,  1273,
    1274,  1275,  1276,  1280,  1298,  1321,  1324,  1334,  1349,  1364,
    1379,  1382,  1389,  1393,  1400,  1401,  1405,  1406,  1410,  1411,
    1412,  1416,  1420,  1424,  1431,  1435,  1436,  1437,  1438,  1439,
    1440,  1441,  1442,  1443,  1444,  1445,  1446,  1447,  1448,  1449,
    1450,  1451,  1452,  1453,  1454,  1455,  1456,  1457,  1458,  1459,
    1460,  1461,  1462,  1463,  1464,  1465,  1466,  1467,  1468,  1469,
    1470,  1471,  1472,  1473,  1474,  1475,  1476,  1477,  1478,  1479,
    1480,  1481,  1482,  1483,  1484,  1485,  1486,  1487,  1488,  1489,
    1490,  1491,  1492,  1493,  1494,  1495,  1496,  1497,  1498,  1499,
    1500,  1501,  1502,  1503,  1504,  1505,  1506,  1507,  1508,  1509,
    1510,  1511,  1512,  1513,  1514,  1515,  1516,  1517,  1518,  1519,
    1520,  1521,  1522,  1523,  1524,  1528,  1529,  1530,  1531,  1532,
    1533,  1534,  1535,  1536,  1537,  1538,  1539,  1540,  1541,  1542,
    1543,  1544,  1545,  1546,  1547,  1548,  1549,  1550,  1551,  1552,
    1553,  1554,  1555,  1556,  1557,  1558,  1559,  1560,  1561,  1562,
    1563,  1564,  1565,  1566,  1567,  1568,  1569,  1570,  1571,  1572,
    1573,  1574,  1575,  1579,  1580,  1584,  1603,  1604,  1605,  1609,
    1615,  1615,  1632,  1635,  1637,  1635,  1649,  1651,  1649,  1666,
    1686,  1706,  1726,  1738,  1739,  1740,  1741,  1742,  1743,  1744,
    1745,  1746,  1747,  1748,  1749,  1750,  1751,  1752,  1753,  1754,
    1755,  1756,  1757,  1759,  1757,  1774,  1779,  1785,  1791,  1792,
    1796,  1797,  1801,  1805,  1812,  1813,  1824,  1828,  1831,  1839,
    1839,  1839,  1842,  1848,  1851,  1855,  1859,  1866,  1873,  1879,
    1883,  1887,  1890,  1893,  1901,  1904,  1912,  1918,  1919,  1920,
    1924,  1925,  1929,  1930,  1934,  1939,  1947,  1954,  1966,  1970,
    1973,  1983,  1983,  1983,  1986,  1986,  1986,  1991,  1991,  1991,
    1999,  1999,  1999,  2005,  2015,  2026,  2041,  2044,  2047,  2050,
    2056,  2057,  2065,  2077,  2078,  2079,  2083,  2084,  2085,  2086,
    2087,  2091,  2096,  2104,  2105,  2109,  2116,  2120,  2127,  2128,
    2129,  2130,  2131,  2132,  2133,  2134,  2135,  2139,  2140,  2141,
    2142,  2143,  2144,  2145,  2146,  2147,  2148,  2149,  2150,  2151,
    2152,  2153,  2154,  2155,  2156,  2157,  2158,  2159,  2160,  2161,
    2162,  2163,  2164,  2168,  2174,  2181,  2193,  2199,  2207,  2215,
    2226,  2238,  2242,  2249,  2252,  2252,  2252,  2257,  2257,  2257,
    2270,  2274,  2278,  2282,  2288,  2296,  2302,  2310,  2318,  2329,
    2338,  2344,  2352,  2352,  2352,  2359,  2363,  2371,  2379,  2387,
    2391,  2394,  2402,  2403,  2404,  2411,  2412,  2413,  2414,  2415,
    2416,  2417,  2418,  2419,  2420,  2421,  2422,  2423,  2424,  2425,
    2426,  2427,  2428,  2429,  2430,  2431,  2432,  2433,  2434,  2435,
    2436,  2437,  2438,  2439,  2440,  2441,  2442,  2443,  2444,  2445,
    2446,  2452,  2453,  2454,  2455,  2456,  2471,  2480,  2481,  2482,
    2483,  2484,  2485,  2486,  2487,  2488,  2489,  2490,  2491,  2492,
    2493,  2494,  2494,  2494,  2502,  2503,  2504,  2509,  2512,  2512,
    2512,  2515,  2520,  2524,  2524,  2524,  2529,  2536,  2542,  2546,
    2546,  2546,  2551,  2554,  2560,  2560,  2560,  2567,  2572,  2576,
    2576,  2576,  2581,  2584,  2590,  2590,  2590,  2597,  2602,  2603,
    2604,  2605,  2606,  2607,  2608,  2609,  2610,  2612,  2616,  2617,
    2622,  2628,  2634,  2643,  2646,  2649,  2658,  2659,  2660,  2661,
    2662,  2663,  2664,  2668,  2672,  2676,  2680,  2684,  2688,  2692,
    2696,  2700,  2705,  2709,  2714,  2718,  2723,  2730,  2731,  2735,
    2736,  2737,  2741,  2742,  2746,  2747,  2748,  2752,  2753,  2757,
    2769,  2772,  2773,  2777,  2777,  2796,  2795,  2810,  2809,  2826,
    2838,  2847,  2857,  2858,  2859,  2860,  2861,  2865,  2868,  2877,
    2878,  2882,  2885,  2889,  2902,  2908,  2917,  2918,  2922,  2925,
    2929,  2942,  2943,  2947,  2952,  2957,  2965,  2968,  2975,  2978,
    2984,  2985,  2986,  2990,  2991,  2995,  3002,  3007,  3016,  3022,
    3026,  3037,  3044,  3053,  3056,  3059,  3066,  3070,  3076,  3087,
    3090,  3095,  3106,  3107,  3111,  3112,  3113,  3117,  3120,  3123,
    3123,  3143,  3146,  3146,  3164,  3169,  3177,  3178,  3182,  3185,
    3198,  3215,  3216,  3217,  3222,  3222,  3248,  3249,  3256,  3256,
    3278,  3279,  3280,  3284,  3294,  3297,  3303,  3304,  3308,  3309,
    3313,  3314,  3318,  3320,  3325,  3318,  3341,  3342,  3346,  3347,
    3351,  3357,  3358,  3359,  3360,  3364,  3365,  3366,  3370,  3373,
    3379,  3381,  3386,  3379,  3407,  3414,  3419,  3428,  3434,  3438,
    3449,  3450,  3451,  3452,  3453,  3454,  3455,  3456,  3457,  3458,
    3459,  3460,  3461,  3462,  3463,  3464,  3465,  3466,  3467,  3468,
    3469,  3470,  3471,  3472,  3473,  3474,  3475,  3476,  3477,  3478,
    3479,  3480,  3481,  3482,  3483,  3484,  3485,  3486,  3487,  3488,
    3489,  3490,  3491,  3492,  3493,  3494,  3495,  3496,  3497,  3498,
    3502,  3503,  3504,  3505,  3506,  3507,  3508,  3509,  3513,  3524,
    3528,  3535,  3546,  3553,  3559,  3568,  3573,  3583,  3593,  3603,
    3616,  3617,  3618,  3619,  3620,  3624,  3628,  3628,  3628,  3642,
    3643,  3647,  3652,  3659,  3662,  3665,  3668,  3674,  3677,  3691,
    3692,  3696,  3697,  3698,  3699,  3700,  3700,  3700,  3704,  3709,
    3716,  3723,  3723,  3723,  3730,  3730,  3730,  3737,  3741,  3745,
    3750,  3755,  3760,  3765,  3769,  3773,  3778,  3782,  3786,  3791,
    3791,  3791,  3797,  3804,  3804,  3804,  3809,  3809,  3809,  3815,
    3815,  3815,  3820,  3826,  3826,  3826,  3831,  3831,  3831,  3840,
    3846,  3846,  3846,  3851,  3851,  3851,  3860,  3866,  3866,  3866,
    3871,  3871,  3871,  3880,  3880,  3880,  3886,  3886,  3886,  3895,
    3898,  3909,  3925,  3927,  3932,  3937,  3925,  3963,  3965,  3970,
    3976,  3963,  4002,  4004,  4009,  4014,  4002,  4055,  4056,  4057,
    4058,  4059,  4060,  4061,  4065,  4066,  4067,  4068,  4069,  4073,
    4080,  4087,  4093,  4099,  4106,  4113,  4119,  4128,  4131,  4137,
    4145,  4150,  4157,  4162,  4168,  4169,  4173,  4174,  4178,  4178,
    4178,  4186,  4186,  4186,  4193,  4193,  4193,  4203,  4203,  4203,
    4210,  4210,  4210,  4220,  4226,  4226,  4226,  4239,  4257,  4257,
    4257,  4267,  4267,  4267,  4280,  4280,  4280,  4293,  4302,  4302,
    4302,  4320,  4327,  4327,  4327,  4337,  4340,  4351,  4357,  4379,
    4387,  4405,  4428,  4429,  4433,  4434,  4439,  4442,  4452
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "\"lexer error\"",
  "\"capture\"", "\"struct\"", "\"class\"", "\"let\"", "\"def\"",
  "\"while\"", "\"if\"", "\"static_if\"", "\"else\"", "\"for\"",
  "\"recover\"", "\"true\"", "\"false\"", "\"new\"", "\"typeinfo\"",
  "\"type\"", "\"in\"", "\"is\"", "\"as\"", "\"elif\"", "\"static_elif\"",
  "\"array\"", "\"return\"", "\"null\"", "\"break\"", "\"try\"",
  "\"options\"", "\"table\"", "\"expect\"", "\"const\"", "\"require\"",
  "\"operator\"", "\"enum\"", "\"finally\"", "\"delete\"", "\"deref\"",
  "\"typedef\"", "\"typedecl\"", "\"with\"", "\"aka\"", "\"assume\"",
  "\"cast\"", "\"override\"", "\"abstract\"", "\"upcast\"", "\"iterator\"",
  "\"var\"", "\"addr\"", "\"continue\"", "\"where\"", "\"pass\"",
  "\"reinterpret\"", "\"module\"", "\"public\"", "\"label\"", "\"goto\"",
  "\"implicit\"", "\"explicit\"", "\"shared\"", "\"private\"",
  "\"smart_ptr\"", "\"unsafe\"", "\"inscope\"", "\"static\"",
  "\"fixed_array\"", "\"default\"", "\"uninitialized\"", "\"bool\"",
  "\"void\"", "\"string\"", "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"",
  "\"int4\"", "\"uint\"", "\"bitfield\"", "\"uint2\"", "\"uint3\"",
  "\"uint4\"", "\"float\"", "\"float2\"", "\"float3\"", "\"float4\"",
  "\"range\"", "\"urange\"", "\"range64\"", "\"urange64\"", "\"block\"",
  "\"int64\"", "\"uint64\"", "\"double\"", "\"function\"", "\"lambda\"",
  "\"int8\"", "\"uint8\"", "\"int16\"", "\"uint16\"", "\"float16\"",
  "\"half2\"", "\"half3\"", "\"half4\"", "\"half8\"", "\"short2\"",
  "\"short3\"", "\"short4\"", "\"short8\"", "\"ushort2\"", "\"ushort3\"",
  "\"ushort4\"", "\"ushort8\"", "\"byte2\"", "\"byte3\"", "\"byte4\"",
  "\"byte8\"", "\"byte16\"", "\"ubyte2\"", "\"ubyte3\"", "\"ubyte4\"",
  "\"ubyte8\"", "\"ubyte16\"", "\"tuple\"", "\"variant\"", "\"generator\"",
  "\"yield\"", "\"sealed\"", "\"template\"", "\"+=\"", "\"-=\"", "\"/=\"",
  "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"",
  "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"",
  "\"!=\"", "\"->\"", "\"<-\"", "\"??\"", "\"?.\"", "\"?[\"", "\"!.\"",
  "\"!?.\"", "\"![\"", "\"!?[\"", "\"!??\"", "\"!==\"", "\"!<-\"",
  "\"!:=\"", "\"!is\"", "\"!as\"", "\"!?as\"", "\"<|\"", "\"|>\"",
  "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"",
  "\"@@\"", "\"@field\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"",
  "\"&&=\"", "\"||=\"", "\"^^=\"", "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"",
  "\"$b\"", "\"$a\"", "\"$t\"", "\"$c\"", "\"$f\"", "\"...\"",
  "\"integer constant\"", "\"long integer constant\"",
  "\"int64 minimum magnitude (requires minus)\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"unsigned int8 constant\"", "\"floating point constant\"",
  "\"float16 constant\"", "\"double constant\"", "\"name\"",
  "\"new line, comma\"", "\"new line, semicolon\"",
  "\"start of the string\"", "STRING_CHARACTER", "STRING_CHARACTER_ESC",
  "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "','", "'='", "'?'", "':'", "'|'",
  "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'",
  "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
  "LLPIPE", "POST_INC", "POST_DEC", "DEREF", "'.'", "'['", "']'", "'('",
  "')'", "'$'", "'@'", "';'", "'{'", "'}'", "'#'", "$accept", "program",
  "COMMA", "SEMICOLON", "top_level_reader_macro",
  "optional_public_or_private_module", "module_name",
  "optional_not_required", "module_declaration", "character_sequence",
  "string_constant", "format_string", "optional_format_string", "$@1",
  "string_builder_body", "string_builder", "reader_character_sequence",
  "expr_reader", "$@2", "options_declaration", "require_declaration",
  "require_module_name", "optional_require_guard", "require_module",
  "is_public_module", "expect_declaration", "expect_list", "expect_error",
  "expression_label", "expression_goto", "elif_or_static_elif",
  "emit_semis", "optional_emit_semis", "expression_else", "$@3", "$@4",
  "if_or_static_if", "expression_else_one_liner",
  "expression_if_one_liner", "semis", "optional_semis",
  "expression_if_block", "$@5", "$@6", "$@7", "expression_else_block",
  "$@8", "$@9", "$@10", "expression_if_then_else", "$@11", "$@12",
  "expression_if_then_else_oneliner", "for_variable_name_with_pos_list",
  "expression_for_loop", "$@13", "expression_unsafe",
  "expression_while_loop", "$@14", "with_keyword_on", "expression_with",
  "expression_with_alias", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "metadata_argument_list", "optional_for_annotations",
  "annotation_declaration_name", "annotation_declaration_basic",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_annotation_list_with_emit_semis",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "das_type_name", "optional_template",
  "global_function_declaration", "optional_public_or_private_function",
  "function_declaration_header", "function_declaration", "$@15",
  "expression_block_finally", "$@16", "$@17", "expression_block", "$@18",
  "$@19", "expr_call_pipe_no_bracket", "expression_any", "$@20", "$@21",
  "expressions", "optional_expr_list", "optional_expr_map_tuple_list",
  "type_declaration_no_options_list", "name_in_namespace",
  "expression_delete", "new_type_declaration", "$@22", "$@23", "expr_new",
  "expression_break", "expression_continue", "expression_return",
  "expression_yield", "expression_try_catch", "kwd_let_var_or_nothing",
  "kwd_let", "optional_in_scope", "tuple_expansion",
  "tuple_expansion_variable_declaration", "expression_let", "expr_cast",
  "$@24", "$@25", "$@26", "$@27", "$@28", "$@29", "expr_type_decl", "$@30",
  "$@31", "expr_type_info", "expr_list", "block_or_simple_block",
  "block_or_lambda", "capture_entry", "capture_list",
  "optional_capture_list", "expr_full_block",
  "expr_full_block_assumed_piped", "expr_numeric_const",
  "expr_assign_no_bracket", "expr_named_call",
  "expr_method_call_no_bracket", "func_addr_name", "func_addr_expr",
  "$@32", "$@33", "$@34", "$@35", "expr_field_no_bracket", "$@36", "$@37",
  "expr_call", "expr", "expr_no_bracket", "$@38", "$@39", "$@40", "$@41",
  "$@42", "$@43", "$@44", "$@45", "$@46", "$@47", "$@48", "$@49", "$@50",
  "$@51", "expr_generator", "expr_mtag_no_bracket",
  "optional_field_annotation", "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@52", "$@53", "$@54",
  "function_argument_declaration_no_type",
  "function_argument_declaration_type", "function_argument_list",
  "tuple_type", "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration_no_type", "variable_declaration_type",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list",
  "global_let_variable_name_with_pos_list", "variable_declaration_list",
  "let_variable_declaration", "global_let_variable_declaration",
  "optional_shared", "optional_public_or_private_variable",
  "global_variable_declaration_list", "$@55", "global_let", "$@56",
  "enum_expression", "commas", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@57",
  "alias_declaration", "distinct_alias", "$@58",
  "optional_public_or_private_enum", "enum_name",
  "optional_enum_basic_type_declaration", "optional_commas", "emit_commas",
  "optional_emit_commas", "enum_declaration", "$@59", "$@60", "$@61",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@62", "$@63", "$@64", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_alias_bits", "bitfield_basic_type_declaration",
  "bitfield_type_declaration", "$@65", "$@66", "c_or_s", "table_type_pair",
  "dim_list", "type_declaration_no_options",
  "optional_expr_list_in_braces", "type_declaration_no_options_no_dim",
  "$@67", "$@68", "$@69", "$@70", "$@71", "$@72", "$@73", "$@74", "$@75",
  "$@76", "$@77", "$@78", "$@79", "$@80", "$@81", "$@82", "$@83", "$@84",
  "$@85", "$@86", "$@87", "$@88", "$@89", "$@90", "$@91", "$@92", "$@93",
  "$@94", "$@95", "$@96", "type_declaration", "tuple_alias_declaration",
  "$@97", "$@98", "$@99", "$@100", "variant_alias_declaration", "$@101",
  "$@102", "$@103", "$@104", "bitfield_alias_declaration", "$@105",
  "$@106", "$@107", "$@108", "make_decl", "make_decl_no_bracket",
  "make_struct_fields", "make_variant_dim", "make_struct_single",
  "make_struct_dim_list", "make_struct_dim_decl",
  "optional_make_struct_dim_decl", "use_initializer", "make_struct_decl",
  "$@109", "$@110", "$@111", "$@112", "$@113", "$@114", "$@115", "$@116",
  "$@117", "$@118", "make_tuple_call", "$@119", "$@120", "make_dim_decl",
  "$@121", "$@122", "$@123", "$@124", "$@125", "$@126", "$@127", "$@128",
  "$@129", "$@130", "expr_map_tuple_list", "push_table_nesting",
  "make_table_decl", "make_table_call", "array_comprehension_where",
  "optional_comma", "table_comprehension", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1719)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-961)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1719,    98, -1719, -1719,    92,    -9,    39,   525, -1719,  -118,
   -1719, -1719, -1719, -1719,   128,   298, -1719, -1719, -1719, -1719,
       7,     7,     7, -1719,    83, -1719,   195, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,   112, -1719,
     147,   203,   213, -1719,   340,   462, -1719, -1719, -1719,   232,
       7,     7, -1719, -1719,   195,   525,   525,   525,   271,   335,
   -1719, -1719, -1719, -1719,   298,   298,   298,   253, -1719,   913,
    -110, -1719, -1719, -1719, -1719,   419, -1719,   275, -1719,   549,
     347,    92,   342,    -9,   356, -1719,   414,   358,   431,   396,
      10,   412, -1719, -1719,   823,   466,   476,   503, -1719,   517,
     430, -1719, -1719,   -92,    92,   298,   298,   298,   298,   565,
   -1719,   872,   886,   624,   673,   888, -1719, -1719,   586, -1719,
   -1719,   655, -1719, -1719, -1719, -1719, -1719,   931,   783,   601,
   -1719, -1719, -1719, -1719,   340,   340,   626,   340,   671,   676,
     642,   683, -1719, -1719, -1719,   679, -1719, -1719,   660,   688,
     565,   565, -1719, -1719,   725, -1719,   -36, -1719,   262,   772,
     913, -1719,   753, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
     777, -1719, -1719, -1719, -1719, -1719, -1719,   792, -1719, -1719,
   -1719, -1719,   938, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719,   857, -1719, -1719,   126,   626,   626,
     626, -1719, -1719,   956,   956,   798, 11024,   960, -1719,   734,
     866, -1719, -1719, -1719, -1719, -1719, 12569, -1719,   869,   951,
      89,    92,   927,   901, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719,   601, -1719, -1719, -1719, 11024,   890,   891,   894,   876,
     905,   906, -1719, -1719, -1719,   893, -1719, -1719, -1719, -1719,
   -1719,   -18, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719,   916, -1719, -1719, -1719,   928,   933, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719,   935,   936,   919,   128,
     108, -1719, -1719, -1719, -1719,   258,   925,   948, -1719, -1719,
   -1719, -1719, -1719, -1719,   963, -1719,   937,   949, 11196, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719,  1143,  1156, -1719,
     955, -1719,   565,   838,   866, -1719,   997,   565, -1719, -1719,
     792,   565,    92, -1719,   643, -1719, -1719,   948, -1719, -1719,
   -1719,  9390, -1719, -1719,  1002,   986,   101,   122,   251, -1719,
   -1719,  9390,   328, -1719,  6871, -1719, -1719, -1719,     8, -1719,
   -1719, -1719,     1, -1719,  7100,   975,  1906, -1719,   969, -1719,
   -1719, 12780, 12834, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719,  1013,   978, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,  1194,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719,  1019,   985, -1719, -1719,   -66,   -81,   -83, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,   982,  1012,
   -1719,   278, -1719,   565,  1026, 11024, -1719,   -33, 11024, 11024,
   11024,  1020,  1021, -1719, -1719,    41,   128,  1022,    26, -1719,
     404,   995,  1023,  1024,   434,  1027,   999,   435,  1029, -1719,
     558,    20,  1030, 10527, 10527,   600,  1006,  1011,  1014,  1016,
    1017,  1018, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719,  9846, 10527, 10527, 10527, 10527,  4352,  5268, -1719,
   -1719, -1719, -1719, -1719, -1719,  1025, -1719, -1719, -1719, -1719,
    1028, -1719, -1719,   -42,   -42, -1719,   -42,   -42,  1031, 11301,
   -1719, -1719,  1033, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, 11024, 11024,  1035,  1032, 11024,   955, 11024,   955, 11024,
     955, 11195,  1049,  1036, -1719,  6871, -1719, 11024,  9390,  1037,
    1042, -1719, -1719, -1719, -1719, -1719,  1040, -1719, -1719,  1044,
    7329, -1719,   258, -1719, 11195,  1049, -1719, -1719, -1719, -1719,
   -1719, -1719, 12889,   790,  1592,  1041, -1719,   184,  1045,    47,
    1047, 11024, 11024, -1719, 10073, -1719, -1719, -1719, -1719,   128,
   -1719,   677,  1050,  1215,   693, -1719, -1719, -1719,   631, -1719,
   -1719, -1719,  9390,   316,   487,  1043,   234, -1719, -1719, -1719,
   -1719,  1051, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719,   588, -1719,  1048,  1065,  1072, -1719,  6871, 11024,  9390,
    9390, -1719, -1719, -1719,  9390, -1719,  9390, -1719,  6871, -1719,
   -1719,  6871,  1073, -1719, 11024,   573,   573,  1054,  1056,   290,
   -1719, -1719,  9390,  9390,  9390,  9390,  9390,  9390,   892, -1719,
     573,   573,   259,   573,   573,  1057,  1246,  1059,  1060,   178,
    1042,  1058,  1063,   565,  3894,   298,  1257, -1719, -1719,  1028,
   -1719, -1719, -1719, -1719, -1719, 12021, 12076, 10527, 10527, -1719,
   -1719, 10527, 10527, 10527, 10527,  1067, 10527,   468,  9390,   640,
     646,  9390,  9390, 10527, 12207, 12262, 12393, 10527, 10527, 10527,
   10527,  9390, 10527, 10527, 10527, 10527, 10300, 10527, 10527, 10527,
   10527, 10527, 10527, 10527, 10527, 10527, 10527, 12641,  9390,  5497,
     537,   696, -1719, -1719,  1098,   746,   -81,   765,   -81,   812,
     -81,   107, -1719,   608,   948,  1089, -1719,   613, -1719, 11024,
    1042,   620,   948, -1719, -1719,  7558, -1719, -1719, -1719, -1719,
    1069,  1108, -1719,     7, -1719,     7, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719,  9390, -1719, -1719,   343,   -67,   -67,
     -67, -1719,   948,   948, 10527, 11400, -1719,  1109, -1719, -1719,
   -1719, -1719,  9390,  1110,   389, 11024,   -33, -1719,  9390,     7,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, 11024, 11024, 11024,
   11024,  4581,  1111,  9390, 11024, -1719, -1719, -1719, 11024,  1042,
      85, -1719,  1102,  1075, 11024, 11024, 11024,  1076, 11024,  1078,
   11024,  1042, 11024, 11195,  1042, -1719,  1049,    55,  9390,  9390,
   11024,   955,  1079,  1080,  1082,  1083,  1093,  1094, -1719,  9390,
     758,   111,  1096, -1719,  9390, -1719,  9390, -1719,  9390,  1097,
     648, -1719, -1719,  7787,     4,  4123, -1719,   187,  1099,   319,
    1117,   955,  2434,  1257,  1129,  1118, -1719, -1719,  1130,  1119,
   -1719, -1719,  1188,  1188,  1446,  1446,   964,   964,  1120,  1598,
    1122, -1719,  1112,  1123,  1124,  1126, -1719,  1131,  1141,  1598,
    1149,  1142, -1719, -1719,  1161,  1154, -1719, -1719,  1163,  1159,
   -1719, -1719,   405,   405,  1033,  1188,  1188, 11400, -1719, -1719,
   11673,  2032, 11582, 11400, 12448,  2138, 11764, 11855, 11019,  1446,
    1446,  1411,  1411,  1598,  1598,  1598,   652,  9390,  1165,  1167,
     668,  9390,  1334,  1168,  1147, -1719,   209, -1719, -1719, -1719,
     211, -1719,  1176, -1719,  1182, -1719,  1196, 11024, -1719, 11195,
   11024, -1719,  1049,   731,  1148, -1719, 11024,  9390, -1719, -1719,
    1204,   -21, -1719, 10851, -1719,    75, -1719,  1178,  1181,  1377,
   -1719, -1719,   375, -1719, -1719, -1719, 11491,  2678,  1209, -1719,
     -21,    18,  1183, -1719,  1187,  1384,   631,  9390,     7, -1719,
   -1719, -1719, -1719,   948,   463,   727,   817,   669,   220,  1189,
    1191,   818,  1192,   819, 11024, 11195,  1049,   922,  1195,  1197,
   11024,  9390,  1199, -1719,  1015,  1146,  1157, -1719,  1169, -1719,
    1224,  1200,  1431,   822,  1201, 11024,   843,  1257,  1203,  1206,
    1442,   -81, -1719, -1719, -1719, -1719, -1719,  1198,  1232,  1210,
    1404,  1248,    16,   111,  1211, -1719, -1719, -1719,  1213,   123,
    1216,  1214,  1102,   256, -1719,  1218,   425,  5726, -1719, -1719,
   -1719,   433,   -81, -1719,  8016, -1719,  1217,  8245,  1252,  1260,
   -1719,     7,  1270,  8474,  -113,  8703, -1719, -1719, -1719,     7,
       7,  1453, -1719,  1008, -1719, -1719,  1457, -1719, -1719,  1462,
    1424, -1719,     7, -1719,     7,     7,     7,     7,     7, -1719,
    1408, -1719,     7, 10645,   955, -1719,  9390, -1719,  9390,  4810,
    9390, -1719,  9390,  5955,  9390, -1719, -1719, -1719,  9390, -1719,
    9390, -1719,  9390,  1258,  1239, -1719, -1719, 10527,  1240, -1719,
    1242,  9390,  5039,  1244, -1719,  1243, -1719,  6184, -1719,  7558,
   -1719, -1719, -1719,  1283, -1719,  1284, -1719, -1719, -1719, -1719,
   -1719, -1719,   948, -1719, -1719,   948, -1719, -1719, -1719, -1719,
    1249,   948, -1719,  9390, -1719,   509, -1719, -1719, -1719,  1251,
   -1719,  1253, -1719,  9390,  1285,   512, 11024, -1719,  9390,  1254,
    9390,   707, -1719,  1287, -1719, -1719,  1482,   792, -1719,  9390,
    1292, -1719,  9390,     7, -1719, -1719, -1719, -1719,  1262, -1719,
   -1719, -1719,  1261,  1295, -1719, -1719,  1498,   953,   994, -1719,
   -1719,  9390,  1631, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719,  1646, -1719,    45, -1719, -1719, -1719,
    1286,  6413, -1719,  1297,  9390,  1306, -1719,   263,  6871,   217,
      17,   294,  9390,  9390,  9390,   111, -1719, -1719, -1719,   648,
    1271,  4123,   277,  1308,  1310,  1274,  1312,  1313, -1719,   279,
     565,  9390, -1719,  1501,  9390, -1719,  1304,  1305, -1719,  1307,
    1326, -1719,  1217,  9390, -1719, -1719, -1719, -1719,  1281, -1719,
   -1719,  1282,   -87,   -87,  1289, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719,   -74, -1719, 10527, 10527, 10527, 10527, 10527, 10527,
   10527, 10527, 10527, 10527,  9390, 10527,  9390, 10527, 10527, 10527,
   10527, 10527, 10527, 10527, 10527,   -81, 11024,  1293, 11024,  1294,
    4123, -1719,   326,   331,  1296,  1298, -1719,   369,  1299, 11024,
    1300, 11024,  1302, 11024,  1303, -1719,  9390, 11491,  9390, -1719,
    1309,  4123, -1719,   391,   392,  9390, -1719, -1719, -1719,   400,
   -1719, -1719, -1719, -1719, -1719, -1719,  1249, -1719, -1719,  1311,
   -1719,  1278, -1719, -1719,  1316, -1719,  1290,  1481,   -27, -1719,
    1488, -1719, -1719,  1317,  1322,   895,  1469,     7,  1318, -1719,
       7, -1719,  1333,  1342, -1719, -1719,  9390,  1324, -1719, -1719,
   -1719, -1719,  1343,  1344,  1353,  1354,  1356,  1358,  1367,  1368,
    1474,  1369, -1719,  1375,  8932,  1217,   141, -1719, -1719,   409,
   -1719, -1719,  1378, -1719,  1345, -1719,   418,  1380,  1507,  1248,
    6871,  9390,  9390,  1382, -1719, -1719,    40, -1719,   548, -1719,
   -1719, -1719,  1350, -1719, -1719,   433, -1719,   -83, -1719,  1217,
   -1719, 11024,  9390, -1719, -1719, -1719, -1719,  2922,  9390,  9390,
      92,   927,  1386,  1388,  9161,  1248, -1719, -1719,   278, 11301,
   11301, 11301, 11301, 11301, 11301, 11301, 11301, 11301, 11301, 11301,
   -1719, -1719, 11301, 11301, -1719, -1719, 11301, 11301, 11301, 11301,
   11301, 11301, 11301, 11301,   565,  1889, -1719,   828, -1719,   555,
    6642, -1719, -1719, -1719, -1719, -1719, -1719,  1897, -1719,   842,
   -1719,   844, -1719, 11024,  1383,  1390, -1719,   563,  6642, -1719,
   -1719,  1391, -1719, -1719,  9390, -1719, -1719,  9390,  1355,  9390,
   -1719, -1719, -1719, 11024, -1719, -1719,   778, -1719,    15, -1719,
   -1719, -1719,  1474,  1474,  1392,  1389,  1398,  1399,  1400,  6871,
   -1719,  9390,  9390,  9390,  9390,  9390,  6871, -1719, -1719,  1474,
    1403,  1474, -1719,  1413, -1719, -1719,   141, -1719,  1437, -1719,
   -1719,  1330,  9390,  1415,   421,   422, -1719, -1719,   296,  6871,
    1414,  1417, -1719, -1719, -1719,   948, -1719,  1410,  1421,  1432,
     580,   111,  9390,   340,  1433,   432,   349, -1719,   -83, -1719,
   -1719,  1434,   445, -1719, -1719, -1719,   855, -1719, -1719,  1435,
     482, -1719, -1719,  1436, -1719, -1719,  1420,    96,  1610,    15,
   -1719, -1719,   895,   -61,   -61, -1719,  9390,  1474,  1474,   669,
    1438,  1439,  1441,  1444,  1445,  1450,  1042,   -61,  1474,   669,
   -1719, -1719, -1719,  9390,  1451, -1719, -1719,  1443,  9390,  9390,
     505, -1719, -1719,  1488,  1681,   565, -1719,    56,  1455,   484,
     565,   590, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719,  1610,   343,   669,  1484,  1485, -1719,  1459,  1460,  1461,
     -61,   -61,  1484,  1463, -1719, -1719, -1719, -1719, -1719, -1719,
    1464,  1466,   669,  1470,  1471, -1719,  9390, -1719, -1719,  1472,
   -1719,  9619,     7, -1719,  6871,   565,   565,  1217, 11024,   -33,
   -1719,  3166, 12569, -1719, -1719, -1719, -1719,   506,  1478, -1719,
   -1719, -1719, -1719,  1480,  1483, -1719, -1719, -1719,  1492, -1719,
    1648,  1495,  1471,  9390, -1719, -1719, -1719, -1719, -1719, 11301,
   -1719,  1465,   522,  1217,  1217, -1719,   538,  9390,  1476,     7,
   12569, -1719,   669, -1719, -1719, -1719,  9390, -1719,  1496,  1471,
   -1719,   457,  9619,   565, -1719, -1719,  9390,     7, -1719, -1719,
     565,   551, -1719, -1719,  1477, -1719,   565, -1719, -1719,  1497,
   -1719,     7,  1217,     7, -1719,   -83, -1719, -1719,  3410, -1719,
    9390, -1719, -1719, -1719, -1719,  1490,  1494,  1499,  1488, -1719,
   -1719,  9619,   565, -1719, -1719,     7, -1719,  3654, -1719,  1494,
    1500,   457,  1488, -1719, -1719
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   186,     1,   410,     0,     0,    58,   761,   411,     0,
     982,   972,   977,    20,     0,     0,    19,    16,    15,     3,
       0,     0,     0,     8,   800,     7,   742,     6,    11,     5,
       4,    13,    12,    14,   144,   145,   146,   143,   160,   162,
      49,    68,    65,    66,     0,     0,    50,   763,   762,     0,
       0,     0,    26,    25,   742,   761,   761,   761,     0,   384,
      47,   170,   171,   172,     0,     0,     0,   173,   175,   182,
       0,   169,    21,    10,     9,   333,   782,     0,   743,   744,
       0,     0,     0,     0,     0,    51,     0,     0,    59,     0,
      63,   764,   766,   767,    22,     0,     0,     0,   386,     0,
       0,   181,   176,     0,     0,     0,     0,     0,     0,    77,
     334,   336,   770,   792,   791,   795,   746,   745,   752,   158,
     159,     0,   150,   151,   154,   156,   148,     0,     0,     0,
     147,   163,    69,    67,     0,     0,    52,     0,     0,     0,
       0,     0,    64,    60,   768,     0,    23,    24,    27,   880,
      77,    77,   385,    45,    48,   180,     0,   177,   178,   179,
     183,    75,    78,   187,   338,   337,   340,   335,   772,   771,
       0,   794,   793,   797,   796,   801,   747,   667,   149,    30,
      31,    35,     0,   155,   152,   153,   157,   139,   140,   131,
     132,   135,   137,   130,     0,   129,   141,     0,    54,    55,
      53,    57,    56,    63,    63,     0,     0,     0,    29,     0,
     780,   973,   978,    46,   174,    76,     0,   773,   774,   788,
     749,     0,   668,     0,    32,    33,    34,   136,   133,   134,
     138,     0,   161,    62,    61,     0,     0,     0,     0,     0,
       0,     0,   810,   853,   811,   869,   812,   816,   817,   818,
     819,   859,   823,   824,   825,   826,   827,   828,   829,   854,
     855,   856,   857,   942,   815,   822,   858,   949,   956,   813,
     820,   814,   821,   830,   831,   832,   833,   834,   835,   836,
     837,   838,   839,   840,   841,   842,   843,   844,   845,   846,
     847,   848,   849,   850,   851,   852,     0,     0,     0,     0,
     868,   901,   904,   902,   903,   969,   897,   765,    28,   883,
     884,   881,   882,   778,   781,   983,     0,     0,     0,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,     0,     0,   194,
     188,   284,    77,     0,   780,   789,     0,    77,   751,   748,
     667,    77,     0,   731,   723,   753,   142,   769,   905,   933,
     936,     0,   939,   929,     0,     0,   943,   950,   957,   963,
     966,     0,   899,   911,   378,   919,   924,   918,     0,   932,
     928,   921,     0,   923,     0,   898,     0,   779,     0,   974,
     979,   275,   276,   273,   199,   200,   202,   201,   203,   204,
     205,   206,   232,   233,   230,   231,   223,   234,   235,   224,
     221,   222,   197,   274,   257,     0,   272,   236,   237,   238,
     239,   210,   211,   212,   207,   208,   209,   220,   196,     0,
     226,   227,   225,   218,   219,   214,   213,   215,   216,   217,
     198,   195,   256,     0,   228,   229,   667,   191,     0,   860,
     863,   866,   867,   861,   864,   862,   865,   775,     0,   786,
     802,     0,   164,    77,     0,     0,   724,     0,     0,     0,
       0,     0,     0,   541,   542,     0,     0,     0,     0,   535,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   859,
       0,     0,     0,     0,     0,   445,     0,     0,     0,     0,
       0,     0,   651,   458,   460,   459,   462,   463,   464,   465,
     466,    41,     0,     0,     0,     0,     0,   378,     0,   443,
     444,  1057,   539,   538,   638,   536,   631,   630,   629,   628,
     184,   634,   537,   633,   632,   586,   543,   587,     0,   532,
     594,   544,     0,   540,   994,   996,   995,   533,   998,   997,
     534,     0,     0,     0,   886,     0,   188,     0,   188,     0,
     188,     0,     0,     0,   914,     0,   910,     0,     0,     0,
    1064,   436,   926,   927,   920,   922,     0,   925,   894,     0,
       0,   971,   970,   984,   701,   708,   277,   279,   278,   280,
     271,   255,   281,   258,   240,     0,   189,   409,   692,   693,
       0,     0,     0,   339,     0,   346,   440,   341,   783,     0,
     790,     0,     0,   725,   723,   750,   165,   732,     0,   721,
     722,   720,     0,     0,     0,     0,   891,  1018,  1021,   389,
     868,   393,   392,   398,   987,   993,   988,   989,   990,   992,
     991,     0,   430,     0,     0,     0,  1048,     0,     0,     0,
       0,   421,   424,   591,     0,   427,     0,  1052,     0,  1030,
    1034,     0,     0,  1024,     0,   571,   572,     0,     0,   504,
     501,   503,     0,     0,     0,     0,     0,     0,     0,   461,
     548,   547,   588,   546,   545,     0,     0,     0,     0,   384,
    1064,  1064,     0,    77,     0,     0,   453,   445,   375,   184,
     352,   350,   351,   349,   908,     0,     0,     0,     0,   573,
     574,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   522,     0,     0,
       0,     0,   870,   885,     0,     0,   191,     0,   191,     0,
     191,   384,   699,     0,   697,   705,   706,     0,   871,     0,
    1064,     0,   382,   437,   909,  1065,   379,   917,   893,   896,
       0,   875,   702,    95,   709,    95,   282,   283,   260,   261,
     263,   262,   264,   265,   266,   267,   259,   268,   269,   270,
     244,   245,   247,   246,   248,   249,   250,   251,   242,   243,
     252,   253,   254,   241,     0,   407,   408,     0,   667,   667,
     667,   190,   193,   192,     0,   441,   375,   758,   787,   798,
     680,   803,     0,     0,     0,     0,     0,   739,     0,     0,
     906,   934,   937,    18,    17,   889,   890,     0,     0,     0,
       0,  1016,     0,     0,     0,  1038,  1041,  1044,     0,  1064,
       0,  1055,  1064,     0,     0,     0,     0,     0,     0,     0,
       0,  1064,     0,     0,  1064,  1027,     0,     0,     0,     0,
       0,   188,     0,     0,     0,     0,     0,     0,    44,     0,
      42,     0,     0,  1037,     0,   712,     0,   711,     0,     0,
    1065,  1009,   576,   380,     0,   378,   525,     0,     0,     0,
       0,   188,     0,   453,     0,     0,   602,   601,     0,     0,
     608,   612,   549,   550,   562,   563,   560,   561,     0,   595,
       0,   583,     0,     0,   512,     0,   585,     0,     0,   596,
       0,     0,   607,   606,     0,     0,   613,   617,     0,     0,
     623,   627,   635,   636,   637,   551,   552,   640,   641,   642,
     567,   568,   569,   570,     0,     0,   565,   566,   564,   558,
     559,   554,   553,   555,   556,   557,     0,     0,     0,   510,
       0,     0,     0,     0,     0,   530,     0,   940,   930,   872,
       0,   944,     0,   951,     0,   958,     0,     0,   964,     0,
       0,   967,     0,     0,     0,   912,     0,     0,   438,   895,
     876,   776,    93,    96,   975,    96,   980,     0,     0,   804,
     689,   690,   713,   694,   696,   695,   442,     0,   754,   759,
     776,   683,     0,   727,     0,   728,     0,     0,     0,   741,
     907,   935,   938,   892,     0,     0,     0,  1017,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1065,     0,   589,     0,     0,     0,   590,     0,   639,
       0,     0,     0,     0,     0,     0,     0,   453,     0,     0,
       0,   191,   646,   647,   648,   649,   650,     0,    38,     0,
     111,     0,     0,     0,     0,  1000,   999,   575,     0,     0,
       0,     0,  1064,     0,   526,     0,     0,     0,   529,   527,
     185,     0,   191,   377,   401,   399,     0,     0,     0,     0,
     400,     0,     0,     0,    77,     0,   372,   457,   353,     0,
       0,     0,   366,     0,   367,   361,     0,   358,   357,     0,
       0,   359,     0,   376,     0,    91,    92,    89,    90,   368,
     413,   356,     0,   467,   188,   598,     0,   609,     0,     0,
       0,   580,     0,     0,     0,   579,   582,   603,     0,   614,
       0,   624,     0,     0,     0,   618,   622,     0,     0,   584,
       0,     0,     0,     0,   511,     0,   523,     0,   577,     0,
     531,   941,   931,     0,   887,     0,   945,   947,   952,   954,
     959,   961,   698,   965,   700,   704,   968,   707,   915,   900,
     899,   383,   439,     0,   756,   777,   985,    94,   703,     0,
     710,     0,   691,     0,     0,     0,     0,   714,     0,     0,
       0,   777,   784,     0,   681,   799,     0,   667,   726,     0,
       0,   736,     0,     0,   740,  1019,  1022,   390,     0,   395,
     396,   394,     0,     0,   433,   431,     0,     0,     0,  1049,
    1047,   380,     0,  1056,  1059,   422,   425,   592,   428,  1053,
    1051,  1031,  1035,  1033,     0,  1025,    77,   502,   666,   505,
       0,     0,    39,     0,     0,     0,   414,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1058,   381,   528,     0,
       0,   378,     0,     0,     0,     0,     0,     0,   451,     0,
      77,     0,   402,     0,     0,   387,     0,     0,   371,     0,
       0,    72,     0,     0,   404,   375,   369,   370,     0,    84,
      85,     0,   166,   166,     0,   360,   355,   362,   363,   364,
     365,   412,   667,   354,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   191,     0,     0,     0,     0,
     378,   496,     0,     0,     0,     0,   515,     0,     0,     0,
       0,     0,     0,     0,     0,   619,     0,   597,     0,   581,
       0,   378,   513,     0,     0,     0,   578,   524,   520,     0,
     874,   888,   873,   948,   955,   962,   899,   913,   877,   878,
     757,     0,   976,   981,     0,   806,     0,   807,   716,   715,
     342,   755,   760,     0,     0,   674,   677,     0,     0,   730,
       0,   738,     0,     0,   391,   397,     0,     0,   432,  1039,
    1042,  1045,     0,     0,     0,     0,     0,     0,     0,     0,
    1016,     0,  1028,     0,     0,     0,     0,   508,   652,     0,
      36,    43,     0,   113,     0,   114,     0,     0,   115,     0,
       0,     0,     0,     0,  1002,  1001,     0,   493,     0,   495,
     448,   449,     0,   447,   446,     0,   454,     0,   403,     0,
     388,     0,     0,    70,    71,   121,   405,     0,     0,     0,
       0,   168,     0,     0,     0,     0,   733,   419,     0,   484,
     485,   487,   486,   488,   478,   479,   480,   489,   490,   469,
     470,   471,   473,   474,   475,   476,   477,   472,   491,   492,
     481,   482,   483,   468,    77,     0,   664,     0,   660,     0,
       0,   497,   500,   655,   657,   516,   659,     0,   665,     0,
     661,     0,   663,     0,     0,     0,   654,     0,     0,   514,
     519,     0,   521,   916,     0,   986,   805,     0,     0,     0,
     343,   348,   785,     0,   675,   676,   677,   678,   669,   684,
     729,   737,  1016,  1016,     0,     0,     0,     0,     0,   378,
    1060,   380,     0,     0,     0,     0,     0,  1017,  1032,  1016,
       0,  1016,   643,     0,   645,   506,     0,   653,    40,   112,
     415,     0,     0,     0,     0,     0,  1004,  1003,     0,     0,
       0,     0,   452,   455,   406,   128,   127,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   418,     0,   599,
     610,     0,     0,   604,   615,   625,     0,   662,   658,     0,
       0,   656,   879,     0,   809,   717,     0,     0,   672,   669,
     670,   671,   674,  1015,  1015,   434,     0,  1016,  1016,  1007,
       0,     0,     0,     0,     0,     0,  1064,  1015,  1016,  1007,
     644,   509,    37,     0,     0,   117,   118,     0,     0,     0,
       0,   494,   450,   342,    86,    77,   167,     0,     0,     0,
      77,   723,   420,   734,   735,   456,   600,   611,   498,   499,
     605,   616,   626,   620,   517,   518,   808,   375,   682,   673,
     685,   672,     0,     0,  1012,  1064,  1014,     0,     0,     0,
    1015,  1015,  1008,     0,  1050,  1061,   423,   426,   593,   429,
       0,     0,  1007,     0,  1062,   116,     0,  1006,  1005,     0,
     374,     0,     0,   108,     0,    77,    77,     0,     0,     0,
     621,     0,     0,   687,   719,   718,   679,     0,  1065,  1013,
    1020,  1023,   435,     0,     0,  1046,  1054,  1036,     0,  1026,
       0,     0,  1062,     0,    87,    91,    92,    89,    90,    88,
     110,   100,     0,     0,     0,   125,     0,     0,     0,     0,
       0,  1010,     0,  1040,  1043,  1029,     0,  1066,     0,  1062,
      97,    79,     0,    77,   123,   126,     0,     0,   345,   686,
      77,     0,  1063,  1067,     0,   375,    77,    73,    74,     0,
     109,     0,     0,     0,   417,     0,  1011,  1068,     0,    80,
       0,   101,   120,   416,   688,     0,   105,     0,   342,   102,
      81,     0,    77,    99,   375,     0,    82,     0,   106,   105,
       0,    79,   342,    83,   104
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1719, -1719,  -982,    -1, -1719, -1719, -1719, -1719, -1719,  1038,
    1661, -1719, -1719, -1719, -1719, -1719, -1719,  1744, -1719, -1719,
   -1719,   -38, -1719, -1719,   849, -1719, -1719,  1665, -1719, -1719,
   -1719, -1719,  -146,  -134, -1719, -1719, -1719, -1719, -1718,   952,
     954, -1719, -1719, -1719, -1719,  -123, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1108, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719,  1539, -1719, -1719,   -59,  -101,  -269,   416, -1719,
   -1719,   635,  1052,  1055,   719,  -509,  -717, -1719,  -392, -1719,
   -1719, -1719, -1478, -1719, -1719, -1681, -1719, -1719, -1092, -1719,
   -1719, -1719, -1719, -1719, -1719,  -844,  -395, -1273,   984,   -13,
   -1719, -1719, -1719, -1719, -1719, -1716, -1707, -1701, -1699, -1719,
   -1719,  1784, -1719, -1429, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719,  -538, -1455,   515,
     272, -1719,  -899, -1719,   520, -1719, -1719, -1719, -1719, -1422,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,   621,
    -387, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719,  -171,    97,    37,
      99,   183, -1719, -1719, -1719, -1719, -1719, -1719, -1719,   299,
    -586,  -870, -1719,  -598,  -866, -1719, -1027,    42,    43, -1719,
    -637,  -636, -1719, -1719, -1719, -1482, -1719,  1738, -1719, -1719,
   -1719, -1719, -1719,   535,   738, -1719,  1113, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719,   739, -1719,  1426, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719,  -131, -1719,  1301, -1719, -1719, -1719,  1550,
   -1719, -1719, -1719,  -629, -1719, -1719,  -381, -1197, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,  -190,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719,  -793, -1596,  -669,
   -1719, -1719, -1454, -1442,  1315, -1719, -1719, -1719, -1719, -1719,
   -1719, -1719, -1719, -1719, -1719,  1319, -1719, -1719,  1320, -1719,
   -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719, -1719,  1133,
   -1719,  -492,  1321, -1307,  -697,  1325,  -491
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   875,   876,    18,   148,    54,   208,    19,   182,
     195,  1638,  1313,  1490,   708,   552,   154,   553,   100,    21,
      22,    88,    45,    46,   143,    23,    42,    43,  1159,  1160,
    1859,   162,   163,  1860,  1876,  1889,  1361,  1782,  1161,  1043,
    1044,  1841,  1855,  1875,  1842,  1880,  1884,  1890,  1881,  1162,
    1163,  1821,  1164,  1122,  1165,  1166,  1167,  1168,  1169,  1170,
    1171,  1172,   196,   197,    38,    39,    40,   222,  1532,    67,
      68,    69,    70,   726,    24,   477,   633,   370,   371,   111,
      25,   166,   372,   167,   216,  1601,  1686,  1828,   636,   856,
    1259,   554,  1173,  1355,  1657,   942,   717,  1131,   801,   555,
    1174,   661,   880,  1464,   556,  1175,  1176,  1177,  1178,  1179,
     847,  1180,  1372,  1317,  1537,  1181,   557,   894,  1475,   895,
    1476,   898,  1478,   558,   884,  1468,   559,   600,   637,   560,
    1338,  1339,   941,   561,   730,   562,  1182,   563,   564,   701,
     565,   910,  1486,   911,  1636,   566,  1012,  1427,   567,   601,
     569,   896,  1477,  1396,  1736,  1409,  1740,  1398,  1737,  1411,
    1741,  1583,  1790,  1413,  1742,   570,   571,   627,  1692,  1750,
    1606,  1608,  1457,  1061,  1267,  1792,  1830,   628,   629,   630,
     792,   793,   813,   796,   797,   815,   928,  1050,  1051,  1796,
     652,   497,   644,   384,  1666,   645,   385,    79,   118,   220,
     380,    27,   177,  1059,  1245,  1060,    49,    50,   145,    28,
      51,   205,   170,   218,   374,  1246,   314,   315,    29,   112,
     857,  1453,   640,   376,   377,   115,   175,   861,    30,    77,
     219,   641,  1052,   572,   487,   302,   303,  1020,  1041,   210,
     304,   784,  1431,  1029,   655,   415,   305,   596,   306,   498,
    1070,   597,  1240,   799,  1436,   582,  1222,   499,  1071,   500,
    1072,   581,  1221,   585,  1226,   586,  1433,   587,  1228,   588,
    1434,   589,  1230,   590,  1435,   591,  1233,   592,  1236,   794,
      31,    56,   316,   614,  1249,    32,    57,   317,   615,  1251,
      33,    55,   418,   811,  1441,   663,   573,   721,  1763,   722,
    1755,  1756,  1757,  1080,   574,   878,  1462,   879,  1463,   906,
    1483,  1105,  1630,   902,  1480,   575,   903,  1481,   576,  1084,
    1616,  1085,  1617,  1086,  1618,   888,  1472,   900,  1479,  1132,
     723,   577,   578,  1811,   806,   579,   580
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,    60,    71,   156,   211,   212,   223,    90,   866,   599,
     720,   868,  1057,   667,   670,  1320,   307,   814,  1473,    72,
      73,    74,   131,   929,   931,  1258,  -186,   877,   812,   617,
     619,   673,   141,  1103,   604,   612,  1318,  1500,  1628,   692,
    1106,   602,  1780,  1437,  1184,   387,   501,   502,   136,    92,
      93,    71,    71,    71,  1343,   938,  1667,   800,  1263,  1244,
    1649,  1690,  1653,  1814,  1635,  1815,   508,   142,   631,  1022,
    1644,  1024,   510,  1026,  1816,   301,  1784,   786,  1244,   788,
    1817,   790,  1818,    52,   105,   106,   107,   221,   405,   634,
     161,    75,    71,    71,    71,    71,   198,   199,     2,   200,
     221,   108,  1324,  1034,   301,     3,  1665,   221,   221,   517,
     518,    34,    35,  1773,   648,   406,   407,   649,   405,    76,
     625,   625,    53,   925,  1861,   686,  1815,   109,     4,   718,
       5,   727,     6,   632,   650,  1816,   695,   696,     7,   889,
    1133,  1817,  1136,  1818,  1691,   406,   407,   155,     8,  1530,
     901,   674,   675,   904,     9,   710,   711,   712,   713,   714,
     635,    36,   381,  1885,  1535,  1815,   520,   521,  1032,  1536,
    1693,  1694,  1036,   626,  1816,    81,  1808,  1753,    10,   651,
    1817,   873,  1818,    41,  1734,   927,   937,  1707,   416,  1709,
     874,   845,  1088,   300,   718,  1092,   209,  1883,   549,   550,
    -880,   728,   408,   214,  1101,  1133,   409,  1104,  1306,   491,
      13,  1894,  1079,  1735,  1711,  1287,    58,  1506,   605,   379,
    1288,  1264,   300,    11,    12,   603,   478,  1319,  1319,  1067,
     817,   490,   408,   138,   846,   492,   409,   606,   693,  1593,
    1758,  1016,    59,  1134,   676,   139,   607,   855,   161,    16,
    -946,  1319,    44,  1771,    15,  1760,  1761,    78,   849,   659,
    1525,  1090,  1265,  1440,   677,  1135,  1772,  1319,   410,   988,
     989,  -953,   411,   925,  1107,   412,   795,   547,    13,  1440,
     113,   114,    99,  1484,   551,   301,   402,   873,    13,   850,
    1323,   405,    13,    37,  1502,  1119,   874,   890,   410,    13,
     413,    13,   411,    58,  1089,   412,  1803,  1804,   653,   654,
     656,   416,  1120,   907,  1829,  -946,    58,    16,   406,   407,
    -946,  1027,    14,   493,    80,   697,   403,    16,   925,    59,
     413,    16,    61,   378,    15,   927,  -953,   231,    16,  -946,
      16,  -953,    59,  1078,  1332,   926,   404,   646,  1701,  1121,
     952,   953,  1850,    99,   954,   955,   956,   957,    81,   959,
    -953,    62,   119,   120,   301,   232,   969,   301,   301,   301,
     982,   983,   985,   986,   987,   990,   991,   992,   993,   995,
     996,   997,   998,   999,  1000,  1001,  1002,  1003,  1004,  1005,
     927,   780,   781,  1634,  1310,   785,  1403,   787,  1137,   789,
    -960,  1497,  1111,   300,  1036,   408,   925,   802,  1278,   409,
    1874,   747,   748,   749,   750,   751,   752,    82,  1498,  1424,
    1219,  1599,  1223,   926,    83,  1340,  1138,  1654,    63,  1272,
    1224,  1219,  1142,    91,  1237,  1327,   873,    13,   105,  -507,
     107,   852,   853,  1234,   925,   874,   925,  1250,  1220,   416,
     301,   301,  1283,  1225,   301,  1499,   301,  1248,   301,  1279,
     301,  1501,   642,  1718,    64,  -960,   301,  1056,   927,  1856,
    -960,   410,    98,    58,  1494,   411,    16,  1032,   412,   643,
    1857,  1858,   300,   301,  1066,   300,   300,   300,  1329,  -960,
    1515,   104,   660,   671,   777,   778,   405,  1074,  1075,    59,
     301,   301,  1495,   413,  -507,  1838,   927,  1087,   927,  -507,
      99,  1527,   700,  1094,  1095,  1096,  1509,  1098,  1516,  1100,
     121,  1102,    84,   406,   407,   925,    65,  1048,  -507,  1110,
     108,   416,  1854,   642,   132,   870,    66,  1570,  1508,   122,
     123,    85,  1329,   124,  1049,   125,   594,   301,   126,   110,
     643,   127,    13,  1727,   745,  1183,  1140,   747,   748,   749,
     750,   751,   752,   301,    86,  1571,   595,   128,   300,   300,
    1572,   405,   300,  1064,   300,    87,   300,   933,   300,   134,
    1219,   137,    47,  1333,   300,   129,  1255,   927,    48,  1256,
    1065,    16,  1257,  1732,   735,   736,    84,   140,   406,   407,
    1334,   300,  1588,  1329,   947,   951,   116,  1569,  1575,   802,
     408,  1219,   117,   144,   409,    85,   187,   188,   300,   300,
    1219,  1719,   678,   973,   977,   981,   858,   984,  1587,  1219,
    1589,  1590,  1494,  1219,  1335,   153,  1329,   135,    86,  1592,
     777,   778,   679,  1494,    84,  1336,  1013,   867,  1637,    87,
    1337,  1402,   683,   687,   138,  1407,  1329,  1641,   960,  1032,
    1716,  1717,  1330,    85,   405,   300,   139,   149,   301,   961,
    1183,  1731,   684,   688,  1423,  1395,   410,   150,  1564,  1429,
     411,   300,  1275,   412,  1739,   408,    86,  1073,   649,   409,
    1076,   406,   407,  1329,  1083,  1825,  1446,    87,    89,   101,
     102,   103,   416,  1286,   151,   650,   871,   138,   413,  1292,
    1439,   873,    71,  1447,   739,   740,  1219,  1329,   152,   139,
     874,  1745,   745,  1786,  1304,   747,   748,   749,   750,   751,
     752,  1844,  1845,  1219,   301,   754,   755,   756,   757,   758,
     157,   158,   159,   160,  1779,  1831,   301,   301,   301,   301,
     651,   410,   416,   301,   171,   411,  1017,   301,   412,  1329,
     405,  1843,  1329,   301,   301,   301,  1329,   301,   161,   301,
    1872,   301,   301,  1489,  1329,    58,   690,  1672,   408,   301,
    1496,   649,   409,   413,   697,  1650,   300,   406,   407,   698,
    1866,    81,  1671,   189,   190,  1680,   691,   191,   650,   192,
    1679,    59,   193,   172,  1788,   127,   882,   496,   777,   778,
     873,    13,  1042,   309,  1042,   873,    13,  1726,   699,   874,
    1417,   194,   873,    13,   874,  1688,   883,  1028,   310,   176,
     963,   874,  1031,   311,    13,   312,   965,  1232,  1128,  1035,
    1235,   964,  1208,   651,   410,  1607,  1241,   966,   411,  1129,
      16,   412,   300,  1209,   494,    16,   178,   495,  1213,   718,
     496,   139,    16,  1206,   300,   300,   300,   300,  1069,  1214,
    1133,   300,   201,    16,   408,   300,   413,   202,   409,   203,
     146,   300,   300,   300,   204,   300,   147,   300,   207,   300,
     300,   206,  1550,  1551,  1554,  1555,   301,   300,   301,   301,
    1754,  1754,   209,  1791,   864,   301,  1762,   865,  1058,   873,
     496,   416,   301,   479,  1754,  1018,  1762,   480,   874,   859,
     860,   818,   819,   820,   821,   822,   823,   824,   825,   164,
     213,   481,   482,   873,    13,   165,   483,   484,   485,   486,
     410,  1158,   874,   168,   411,   173,  1276,   412,   105,   169,
    1238,   174,  1604,   301,   301,   405,   215,   826,  1605,   301,
    1797,   416,  1645,   224,   225,  1021,   221,  1754,  1754,   827,
     828,   829,   413,    16,   301,   183,   184,   185,   217,  1762,
     416,   186,   406,   407,  1023,   735,   736,  1539,  1540,  1541,
    1542,  1543,  1544,  1545,  1546,  1547,  1548,  1549,  1552,  1553,
    1556,  1557,  1558,  1559,  1560,  1561,  1562,  1563,  1352,  1770,
     235,  1868,   568,   142,   300,  1565,   300,   300,  1359,  1360,
     873,    13,   593,   300,   873,    13,   308,   416,  1577,   874,
     300,  1025,   416,   874,   416,   609,  1277,  1282,  1285,  1851,
    1887,  1302,  1247,   416,  1247,   873,    13,  1670,   405,   227,
     228,   229,   233,   234,   874,   230,  1158,   416,  1799,   416,
      16,  1674,  1305,  1675,    16,  1271,  1448,  1274,   313,   408,
     416,   300,   300,   409,  1743,   406,   407,   300,   729,   729,
     375,   729,   729,   373,   731,    16,   732,   733,  1706,   105,
     106,   107,   300,  1531,  1531,  1789,  1456,   179,   180,   918,
     919,   382,   383,   737,   738,   739,   740,   741,   388,   389,
     742,  1720,   390,   745,   391,   746,   747,   748,   749,   750,
     751,   752,   753,   392,   393,   301,   754,   755,   756,   757,
     758,   394,   759,   760,   396,   410,   179,   180,   181,   411,
    1183,  1289,   412,   224,   225,   226,   397,  1053,  1054,  1055,
    1348,   398,  1827,   399,   400,   873,    13,   401,  1356,  1357,
    1485,   414,   408,   416,   874,   417,   409,   413,    95,    96,
      97,  1365,  1470,  1366,  1367,  1368,  1369,  1370,   474,   405,
     419,  1373,   770,   771,   772,   773,   774,   775,   776,  1846,
     405,   475,   420,   476,  1517,    16,   873,    13,   489,   777,
     778,  1538,   405,   583,   584,   874,   406,   407,  1567,   735,
     736,   610,   613,  1471,   620,   621,   622,   406,   407,   803,
     623,  1579,   624,  1581,  1700,   638,   639,   647,   410,   406,
     407,   810,   411,   680,  1295,   412,    16,   686,   657,   658,
     672,   681,   682,   300,   702,   685,  1822,   689,   694,   703,
     795,   783,   704,   805,   705,   706,   707,   405,   863,   922,
     413,   940,   872,   724,   725,   301,   885,   301,   958,   930,
     734,   779,  1461,   869,   782,   798,   804,   807,   301,   844,
     301,   808,   301,   886,   406,   407,   851,   848,   862,   881,
     887,   905,   908,   408,   909,   921,   923,   409,   924,  1019,
     891,   893,   932,  1030,   408,   897,  1039,   899,   409,  1040,
    1058,  1063,  1081,  1091,  1093,  1097,   408,  1099,  1112,  1113,
     409,  1114,  1115,   912,   913,   914,   915,   916,   917,   739,
     740,  1655,  1116,  1117,  1123,  1216,  1127,   745,  1139,   746,
     747,   748,   749,   750,   751,   752,   753,  1185,  1187,  1191,
     754,   755,   756,   757,   758,  1141,  1186,  1188,  1189,   410,
    1190,  1192,  1193,   411,  1194,  1296,   412,  1197,  1195,   962,
     410,   408,   967,   968,   411,   409,  1297,   412,  1196,  1199,
    1198,  1201,   410,   300,  1218,   300,   411,  1239,  1298,   412,
     301,   413,  1200,  1676,  1819,  1227,   300,  1202,   300,  1014,
     300,  1229,   413,  1211,  1183,  1212,  1217,  1826,   772,   773,
     774,   775,   776,  1687,   413,  1231,  1243,  1252,  1668,  1253,
    1254,  1260,  1268,   777,   778,  1269,  1038,  1270,  1280,  1660,
    1281,  1284,   735,   736,  1290,  1291,  1311,   410,  1294,  1300,
    1303,   411,  1307,  1299,   412,  1308,  1312,  1315,  1314,  1316,
    1321,  1322,   301,  1346,  1325,  1819,  1609,  1328,  1326,  1611,
     635,  1347,  1349,  1358,   405,  1047,  1364,   735,   736,   413,
    1362,  1363,   301,   700,  1371,   405,  1415,  1416,  1418,  1419,
    1426,  1183,  1425,  1062,  1430,  1432,  1445,   595,  1454,  1068,
    1455,   406,   407,  1459,  1819,  1442,  1467,  1443,  1450,  1466,
    1183,  1465,   406,   407,  1082,  1487,  1491,  1493,   300,  1510,
    1507,  1511,  1512,  1513,  1514,  1519,  1521,  1522,  1524,  1528,
    1529,  1523,  1595,  1594,  1598,  1600,  1158,  1534,  1597,  1108,
    1109,   405,  1566,  1568,  1603,  1573,  1607,  1574,  1576,  1578,
    1118,  1580,  1582,  1615,  1627,  1124,  1640,  1125,  1586,  1126,
    1643,  1651,   739,   740,   891,  1596,  1684,  1610,   406,   407,
     745,  1602,   746,   747,   748,   749,   750,   751,   752,   753,
     300,  1612,  1713,   754,   755,   756,   757,   758,   408,  1783,
    1613,  1619,   409,  1620,  1787,   737,   738,   739,   740,   408,
     300,  1621,  1622,   409,  1623,   745,  1624,   746,   747,   748,
     749,   750,   751,   752,   753,  1625,  1626,  1629,   754,   755,
     756,   757,   758,  1631,   759,   760,  1715,  1639,  1642,   735,
     736,  1648,  1677,   700,  1661,  1729,  1662,  1696,  1210,  1678,
    1681,  1695,  1215,   774,   775,   776,  1697,  1698,  1699,  1823,
    1824,  1708,  1712,  1749,   410,   408,   777,   778,   411,   409,
    1301,   412,  1710,  1721,  1723,   410,  1722,   301,  1242,   411,
    1724,  1309,   412,  1747,   405,  1733,   772,   773,   774,   775,
     776,  1725,  1730,  1738,  1744,  1746,   413,  1764,  1765,   405,
    1766,   777,   778,  1767,  1768,  1776,  1748,   413,  1273,  1769,
    1775,   406,   407,  1781,  1785,  1329,  1798,  1862,  1800,  1801,
    1802,  1836,  1805,  1806,  1865,  1807,   406,   407,  1840,  1809,
    1869,   410,  1293,  1810,  1813,   411,  1832,  1469,   412,  1833,
    1848,  1867,  1834,   830,   831,   832,   833,   834,   835,   836,
     837,  1835,  1837,  1853,  1878,  1870,  1886,  1879,  1882,   739,
     740,   130,   838,   413,  1892,    20,   920,   745,   133,   746,
     747,   748,   749,   750,   751,   752,   753,  1893,  1038,   839,
     754,   755,   756,   757,   758,  1342,  1891,  1045,  1345,  1046,
     386,   840,   841,   842,  1351,   300,  1354,   939,   408,  1533,
    1266,  1820,   409,  1033,   943,    26,  1751,  1652,  1793,  1689,
    1158,  1752,    94,   408,  1794,  1795,  1452,   409,  1261,  1262,
     488,   395,     0,     0,   843,     0,   662,  1397,     0,  1399,
       0,  1404,   892,  1405,     0,  1408,     0,     0,     0,  1410,
     664,  1412,     0,  1414,   665,   666,   668,     0,  1849,     0,
     669,     0,  1420,   777,   778,     0,     0,     0,     0,     0,
    1038,     0,     0,     0,   410,     0,  1864,     0,   411,     0,
    1474,   412,     0,     0,     0,     0,     0,     0,     0,   410,
    1871,     0,  1873,   411,  1438,  1482,   412,  1158,     0,     0,
       0,     0,     0,     0,  1444,     0,   413,     0,     0,  1449,
       0,  1451,     0,     0,  1888,     0,  1158,     0,     0,     0,
    1458,   413,     0,  1460,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   891,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   405,     0,     0,   236,     0,     0,     0,     0,
     405,   237,     0,     0,     0,  1492,     0,   238,     0,     0,
       0,     0,     0,  1503,  1504,  1505,     0,   239,     0,   406,
     407,     0,     0,     0,     0,   240,     0,   406,   407,     0,
       0,     0,  1518,     0,     0,  1520,     0,     0,     0,     0,
     241,     0,     0,     0,  1526,     0,     0,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,     0,     0,     0,   408,  1584,     0,  1585,
     409,     0,     0,     0,   408,     0,  1591,     0,   409,     0,
       0,     0,     0,   735,   736,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,  1614,     0,     0,
       0,     0,     0,     0,   298,     0,     0,     0,     0,     0,
       0,     0,   410,     0,     0,  1633,   411,    59,  1669,   412,
     410,     0,     0,     0,   411,     0,  1673,   412,     0,     0,
       0,     0,  1646,  1647,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   413,     0,     0,     0,     0,     0,
       0,     0,   413,  1656,     0,     0,   299,     0,     0,  1658,
    1659,   611,     0,     0,     0,  1664,     0,     0,     0,   735,
     736,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   737,   738,   739,   740,   741,     0,     0,   742,   743,
     744,   745,     0,   746,   747,   748,   749,   750,   751,   752,
     753,  1038,     0,     0,   754,   755,   756,   757,   758,     0,
     759,   760,     0,     0,     0,     0,     0,     0,   762,  1038,
     764,     0,     0,     0,     0,  1682,     0,     0,  1683,     0,
    1685,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   891,  1702,  1703,  1704,  1705,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,     0,     0,     0,
       0,     0,     0,  1714,     0,     0,     0,   777,   778,     0,
       0,     0,     0,     0,     0,     0,     0,   737,   738,   739,
     740,   741,     0,  1728,   742,   743,   744,   745,     0,   746,
     747,   748,   749,   750,   751,   752,   753,     0,     0,     0,
     754,   755,   756,   757,   758,     0,   759,   760,     0,     0,
     761,     0,     0,     0,   762,   763,   764,  1759,     0,     0,
     765,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1774,     0,     0,     0,     0,  1777,
    1778,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   766,  1207,   767,   768,   769,   770,   771,   772,   773,
     774,   775,   776,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   777,   778,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1812,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1839,  1143,     0,     0,     0,   501,
     502,     3,     0,  -122,  -107,  -107,     0,  -119,  1847,   503,
     504,   505,   506,   507,     0,     0,     0,  1852,     0,   508,
    1144,   509,  1145,  1146,     0,   510,     0,  1863,     0,     0,
       0,     0,  1147,   511,  1148,     0,  -124,     0,  1149,   512,
       0,     0,   513,     0,     8,   514,  1150,     0,  1151,   515,
       0,  1877,  1152,  1153,     0,     0,     0,     0,     0,  1154,
       0,     0,   517,   518,     0,   242,   243,   244,     0,   246,
     247,   248,   249,   250,   519,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,     0,   264,   265,   266,
       0,     0,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   520,
     521,   522,  1155,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   526,   527,   528,
     529,   530,     0,   531,     0,   532,   533,   534,     0,   535,
     536,   537,   538,   539,   540,    59,     0,    13,   541,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   542,   543,   544,     0,    14,     0,
       0,   545,   546,     0,     0,     0,     0,     0,     0,     0,
     547,     0,   548,     0,   549,   550,    16,  1156,  1157,  1143,
       0,     0,     0,   501,   502,     3,     0,  -122,  -107,  -107,
       0,  -119,     0,   503,   504,   505,   506,   507,     0,     0,
       0,     0,     0,   508,  1144,   509,  1145,  1146,     0,   510,
       0,     0,     0,     0,     0,     0,  1147,   511,  1148,     0,
    -124,     0,  1149,   512,     0,     0,   513,     0,     8,   514,
    1150,     0,  1151,   515,     0,     0,  1152,  1153,     0,     0,
       0,     0,     0,  1154,     0,     0,   517,   518,     0,   242,
     243,   244,     0,   246,   247,   248,   249,   250,   519,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
       0,   264,   265,   266,     0,     0,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   520,   521,   522,  1155,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   526,   527,   528,   529,   530,     0,   531,     0,   532,
     533,   534,     0,   535,   536,   537,   538,   539,   540,    59,
       0,    13,   541,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   542,   543,
     544,     0,    14,     0,     0,   545,   546,     0,     0,     0,
       0,     0,     0,     0,   547,     0,   548,     0,   549,   550,
      16,  1156,  -347,  1143,     0,     0,     0,   501,   502,     3,
       0,  -122,  -107,  -107,     0,  -119,     0,   503,   504,   505,
     506,   507,     0,     0,     0,     0,     0,   508,  1144,   509,
    1145,  1146,     0,   510,     0,     0,     0,     0,     0,     0,
    1147,   511,  1148,     0,  -124,     0,  1149,   512,     0,     0,
     513,     0,     8,   514,  1150,     0,  1151,   515,     0,     0,
    1152,  1153,     0,     0,     0,     0,     0,  1154,     0,     0,
     517,   518,     0,   242,   243,   244,     0,   246,   247,   248,
     249,   250,   519,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,     0,   264,   265,   266,     0,     0,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   520,   521,   522,
    1155,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   526,   527,   528,   529,   530,
       0,   531,     0,   532,   533,   534,     0,   535,   536,   537,
     538,   539,   540,    59,     0,    13,   541,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   542,   543,   544,     0,    14,     0,     0,   545,
     546,     0,     0,     0,     0,     0,     0,     0,   547,     0,
     548,     0,   549,   550,    16,  1156,  -373,  1143,     0,     0,
       0,   501,   502,     3,     0,  -122,  -107,  -107,     0,  -119,
       0,   503,   504,   505,   506,   507,     0,     0,     0,     0,
       0,   508,  1144,   509,  1145,  1146,     0,   510,     0,     0,
       0,     0,     0,     0,  1147,   511,  1148,     0,  -124,     0,
    1149,   512,     0,     0,   513,     0,     8,   514,  1150,     0,
    1151,   515,     0,     0,  1152,  1153,     0,     0,     0,     0,
       0,  1154,     0,     0,   517,   518,     0,   242,   243,   244,
       0,   246,   247,   248,   249,   250,   519,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,     0,   264,
     265,   266,     0,     0,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   520,   521,   522,  1155,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   526,
     527,   528,   529,   530,     0,   531,     0,   532,   533,   534,
       0,   535,   536,   537,   538,   539,   540,    59,     0,    13,
     541,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   542,   543,   544,     0,
      14,     0,     0,   545,   546,     0,     0,     0,     0,     0,
       0,     0,   547,     0,   548,     0,   549,   550,    16,  1156,
    -344,  1143,     0,     0,     0,   501,   502,     3,     0,  -122,
    -107,  -107,     0,  -119,     0,   503,   504,   505,   506,   507,
       0,     0,     0,     0,     0,   508,  1144,   509,  1145,  1146,
       0,   510,     0,     0,     0,     0,     0,     0,  1147,   511,
    1148,     0,  -124,     0,  1149,   512,     0,     0,   513,     0,
       8,   514,  1150,     0,  1151,   515,     0,     0,  1152,  1153,
       0,     0,     0,     0,     0,  1154,     0,     0,   517,   518,
       0,   242,   243,   244,     0,   246,   247,   248,   249,   250,
     519,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,     0,   264,   265,   266,     0,     0,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   520,   521,   522,  1155,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,   529,   530,     0,   531,
       0,   532,   533,   534,     0,   535,   536,   537,   538,   539,
     540,    59,     0,    13,   541,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,   543,   544,     0,    14,     0,     0,   545,   546,     0,
       0,     0,     0,     0,     0,     0,   547,     0,   548,     0,
     549,   550,    16,  1156,   -98,  1143,     0,     0,     0,   501,
     502,     3,     0,  -122,  -107,  -107,     0,  -119,     0,   503,
     504,   505,   506,   507,     0,     0,     0,     0,     0,   508,
    1144,   509,  1145,  1146,     0,   510,     0,     0,     0,     0,
       0,     0,  1147,   511,  1148,     0,  -124,     0,  1149,   512,
       0,     0,   513,     0,     8,   514,  1150,     0,  1151,   515,
       0,     0,  1152,  1153,     0,     0,     0,     0,     0,  1154,
       0,     0,   517,   518,     0,   242,   243,   244,     0,   246,
     247,   248,   249,   250,   519,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,     0,   264,   265,   266,
       0,     0,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   520,
     521,   522,  1155,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   526,   527,   528,
     529,   530,     0,   531,     0,   532,   533,   534,     0,   535,
     536,   537,   538,   539,   540,    59,     0,    13,   541,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   542,   543,   544,     0,    14,     0,
       0,   545,   546,     0,     0,     0,     0,     0,     0,     0,
     547,     0,   548,     0,   549,   550,    16,  1156,  -103,   501,
     502,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     504,   505,   506,   507,     0,     0,     0,     0,     0,   508,
       0,   509,     0,     0,     0,   510,     0,     0,     0,     0,
       0,     0,     0,   511,     0,     0,     0,     0,     0,   512,
       0,     0,   513,     0,     0,   514,     0,     0,     0,   515,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   516,
       0,     0,   517,   518,   934,   242,   243,   244,     0,   246,
     247,   248,   249,   250,   519,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,     0,   264,   265,   266,
       0,     0,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   520,
     521,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,     0,     0,   598,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   526,   527,   528,
     529,   530,     0,   531,   718,   532,   533,   534,     0,   535,
     536,   537,   538,   539,   540,   719,     0,     0,   541,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   542,   543,   544,     0,    14,     0,
       0,   545,   546,     0,     0,     0,     0,     0,   501,   502,
     935,     0,   548,   936,   549,   550,   715,   551,   503,   504,
     505,   506,   507,     0,     0,     0,     0,     0,   508,     0,
     509,     0,     0,     0,   510,     0,     0,     0,     0,     0,
       0,     0,   511,     0,     0,     0,     0,     0,   512,     0,
       0,   513,   716,     0,   514,     0,     0,     0,   515,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   516,     0,
       0,   517,   518,     0,   242,   243,   244,     0,   246,   247,
     248,   249,   250,   519,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,     0,   264,   265,   266,     0,
       0,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   520,   521,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,     0,   598,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   525,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   526,   527,   528,   529,
     530,     0,   531,   718,   532,   533,   534,     0,   535,   536,
     537,   538,   539,   540,   719,     0,     0,   541,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,   543,   544,     0,    14,     0,     0,
     545,   546,     0,     0,     0,     0,     0,   501,   502,   547,
       0,   548,     0,   549,   550,   715,   551,   503,   504,   505,
     506,   507,     0,     0,     0,     0,     0,   508,     0,   509,
       0,     0,     0,   510,     0,     0,     0,     0,     0,     0,
       0,   511,     0,     0,     0,     0,     0,   512,     0,     0,
     513,   716,     0,   514,     0,     0,     0,   515,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   516,     0,     0,
     517,   518,     0,   242,   243,   244,     0,   246,   247,   248,
     249,   250,   519,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,     0,   264,   265,   266,     0,     0,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   520,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,     0,     0,
       0,     0,   598,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   526,   527,   528,   529,   530,
       0,   531,     0,   532,   533,   534,     0,   535,   536,   537,
     538,   539,   540,    59,     0,     0,   541,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   542,   543,   544,     0,    14,     0,     0,   545,
     546,     0,     0,     0,     0,     0,   501,   502,   547,     0,
     548,     0,   549,   550,     0,   551,   503,   504,   505,   506,
     507,     0,     0,     0,     0,     0,   508,     0,   509,     0,
       0,     0,   510,     0,     0,     0,     0,     0,     0,     0,
     511,     0,     0,     0,     0,     0,   512,     0,     0,   513,
       0,     0,   514,     0,     0,     0,   515,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,     0,     0,   517,
     518,  1077,   242,   243,   244,     0,   246,   247,   248,   249,
     250,   519,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,     0,   264,   265,   266,     0,     0,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   520,   521,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,     0,
       0,   598,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   525,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   526,   527,   528,   529,   530,     0,
     531,   718,   532,   533,   534,     0,   535,   536,   537,   538,
     539,   540,   719,     0,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,   543,   544,     0,    14,     0,     0,   545,   546,
       0,     0,     0,     0,     0,   501,   502,   547,     0,   548,
       0,   549,   550,     0,   551,   503,   504,   505,   506,   507,
       0,     0,     0,     0,     0,   508,     0,   509,     0,     0,
       0,   510,     0,     0,     0,     0,     0,     0,     0,   511,
       0,     0,     0,     0,     0,   512,     0,     0,   513,     0,
       0,   514,     0,     0,     0,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   516,     0,     0,   517,   518,
       0,   242,   243,   244,     0,   246,   247,   248,   249,   250,
     519,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,     0,   264,   265,   266,     0,     0,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   520,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,     0,     0,     0,     0,
     598,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,   529,   530,     0,   531,
     718,   532,   533,   534,     0,   535,   536,   537,   538,   539,
     540,   719,     0,     0,   541,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,   543,   544,     0,    14,     0,     0,   545,   546,     0,
       0,     0,     0,     0,   501,   502,  1400,     0,   548,  1401,
     549,   550,     0,   551,   503,   504,   505,   506,   507,     0,
       0,     0,     0,     0,   508,     0,   509,     0,     0,     0,
     510,     0,     0,     0,     0,     0,     0,     0,   511,     0,
       0,     0,     0,     0,   512,     0,     0,   513,     0,     0,
     514,     0,     0,     0,   515,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   516,     0,     0,   517,   518,     0,
     242,   243,   244,     0,   246,   247,   248,   249,   250,   519,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,     0,   264,   265,   266,     0,     0,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   520,   521,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,     0,     0,   598,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   526,   527,   528,   529,   530,     0,   531,   718,
     532,   533,   534,     0,   535,   536,   537,   538,   539,   540,
     719,     0,     0,   541,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   542,
     543,   544,     0,    14,     0,     0,   545,   546,     0,     0,
       0,     0,     0,   501,   502,  1421,     0,   548,  1422,   549,
     550,     0,   551,   503,   504,   505,   506,   507,     0,     0,
       0,     0,     0,   508,     0,   509,     0,     0,     0,   510,
       0,     0,     0,     0,     0,     0,     0,   511,     0,     0,
       0,     0,     0,   512,     0,     0,   513,     0,     0,   514,
       0,     0,     0,   515,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   516,     0,     0,   517,   518,     0,   242,
     243,   244,     0,   246,   247,   248,   249,   250,   519,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
       0,   264,   265,   266,     0,     0,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   520,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,     0,     0,     0,     0,   598,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   526,   527,   528,   529,   530,     0,   531,   718,   532,
     533,   534,     0,   535,   536,   537,   538,   539,   540,   719,
       0,     0,   541,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   542,   543,
     544,     0,    14,     0,     0,   545,   546,     0,     0,     0,
       0,     0,   501,   502,   547,     0,   548,     0,   549,   550,
       0,   551,   503,   504,   505,   506,   507,     0,     0,     0,
       0,     0,   508,     0,   509,     0,     0,     0,   510,     0,
       0,     0,     0,     0,     0,     0,   511,     0,     0,     0,
       0,     0,   512,     0,     0,   513,     0,     0,   514,     0,
       0,     0,   515,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   516,     0,     0,   517,   518,     0,   242,   243,
     244,     0,   246,   247,   248,   249,   250,   519,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,     0,
     264,   265,   266,     0,     0,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   520,   521,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,     0,     0,   598,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     526,   527,   528,   529,   530,     0,   531,     0,   532,   533,
     534,     0,   535,   536,   537,   538,   539,   540,    59,     0,
       0,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   542,   543,   544,
       0,    14,     0,     0,   545,   546,     0,     0,     0,     0,
       0,   501,   502,   547,     0,   548,  1015,   549,   550,     0,
     551,   503,   504,   505,   506,   507,     0,     0,     0,     0,
       0,   508,     0,   509,     0,     0,     0,   510,     0,     0,
       0,     0,     0,     0,     0,   511,     0,     0,     0,     0,
       0,   512,     0,     0,   513,     0,     0,   514,     0,     0,
       0,   515,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,     0,     0,   517,   518,     0,   242,   243,   244,
       0,   246,   247,   248,   249,   250,   519,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,     0,   264,
     265,   266,     0,     0,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   520,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,     0,     0,     0,     0,  1037,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   526,
     527,   528,   529,   530,     0,   531,   718,   532,   533,   534,
       0,   535,   536,   537,   538,   539,   540,   719,     0,     0,
     541,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   542,   543,   544,     0,
      14,     0,     0,   545,   546,     0,     0,     0,     0,     0,
     501,   502,  1331,     0,   548,     0,   549,   550,     0,   551,
     503,   504,   505,   506,   507,     0,     0,     0,     0,     0,
     508,     0,   509,     0,     0,     0,   510,     0,     0,     0,
       0,     0,     0,     0,   511,     0,     0,     0,     0,     0,
     512,     0,     0,   513,     0,     0,   514,     0,     0,     0,
     515,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     516,     0,     0,   517,   518,     0,   242,   243,   244,     0,
     246,   247,   248,   249,   250,   519,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,     0,   264,   265,
     266,     0,     0,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     520,   521,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,     0,     0,   598,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   525,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,   529,   530,     0,   531,     0,   532,   533,   534,     0,
     535,   536,   537,   538,   539,   540,    59,     0,     0,   541,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   542,   543,   544,     0,    14,
       0,     0,   545,   546,     0,     0,     0,     0,     0,   501,
     502,   547,     0,   548,  1406,   549,   550,     0,   551,   503,
     504,   505,   506,   507,     0,     0,     0,     0,     0,   508,
       0,   509,     0,     0,     0,   510,     0,     0,     0,     0,
       0,     0,     0,   511,     0,     0,     0,     0,     0,   512,
       0,     0,   513,     0,     0,   514,     0,     0,     0,   515,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   516,
       0,     0,   517,   518,     0,   242,   243,   244,     0,   246,
     247,   248,   249,   250,   519,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,     0,   264,   265,   266,
       0,     0,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   520,
     521,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,     0,     0,   598,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   526,   527,   528,
     529,   530,     0,   531,     0,   532,   533,   534,     0,   535,
     536,   537,   538,   539,   540,    59,     0,     0,   541,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   542,   543,   544,     0,    14,     0,
       0,   545,   546,     0,     0,     0,     0,     0,   501,   502,
     547,     0,   548,  1428,   549,   550,     0,   551,   503,   504,
     505,   506,   507,     0,     0,     0,     0,     0,   508,     0,
     509,     0,     0,     0,   510,     0,     0,     0,     0,     0,
       0,     0,   511,     0,     0,     0,     0,     0,   512,     0,
       0,   513,     0,     0,   514,     0,     0,     0,   515,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   516,     0,
       0,   517,   518,     0,   242,   243,   244,     0,   246,   247,
     248,   249,   250,   519,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,     0,   264,   265,   266,     0,
       0,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   520,   521,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,     0,   598,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   525,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   526,   527,   528,   529,
     530,     0,   531,     0,   532,   533,   534,     0,   535,   536,
     537,   538,   539,   540,    59,     0,     0,   541,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,   543,   544,     0,    14,     0,     0,
     545,   546,     0,     0,     0,     0,     0,   501,   502,   547,
       0,   548,  1488,   549,   550,     0,   551,   503,   504,   505,
     506,   507,     0,     0,     0,     0,     0,   508,     0,   509,
       0,     0,     0,   510,     0,     0,     0,     0,     0,     0,
       0,   511,     0,     0,     0,     0,     0,   512,     0,     0,
     513,     0,     0,   514,     0,     0,     0,   515,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   516,     0,     0,
     517,   518,     0,   242,   243,   244,     0,   246,   247,   248,
     249,   250,   519,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,     0,   264,   265,   266,     0,     0,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   520,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,     0,     0,
       0,     0,  1037,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   526,   527,   528,   529,   530,
       0,   531,   718,   532,   533,   534,     0,   535,   536,   537,
     538,   539,   540,   719,     0,     0,   541,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   542,   543,   544,     0,    14,     0,     0,   545,
     546,     0,     0,     0,     0,     0,   501,   502,   547,     0,
     548,     0,   549,   550,     0,   551,   503,   504,   505,   506,
     507,     0,     0,     0,     0,     0,   508,     0,   509,     0,
       0,     0,   510,     0,     0,     0,     0,     0,     0,     0,
     511,     0,     0,     0,     0,     0,   512,     0,     0,   513,
       0,     0,   514,     0,     0,     0,   515,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,     0,     0,   517,
     518,     0,   242,   243,   244,     0,   246,   247,   248,   249,
     250,   519,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,     0,   264,   265,   266,     0,     0,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   520,   521,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,     0,
       0,   598,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   525,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   526,   527,   528,   529,   530,     0,
     531,     0,   532,   533,   534,     0,   535,   536,   537,   538,
     539,   540,    59,     0,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,   543,   544,     0,    14,     0,     0,   545,   546,
       0,     0,     0,     0,     0,   501,   502,   547,     0,   548,
       0,   549,   550,     0,   551,   503,   504,   505,   506,   507,
       0,     0,     0,     0,     0,   508,     0,   509,     0,     0,
       0,   510,     0,     0,     0,     0,     0,     0,     0,   511,
       0,     0,     0,     0,     0,   512,     0,     0,   513,     0,
       0,   514,     0,     0,     0,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   516,     0,     0,   517,   518,
       0,   242,   243,   244,     0,   246,   247,   248,   249,   250,
     519,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,     0,   264,   265,   266,     0,     0,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   520,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,   529,   530,     0,   531,
       0,   532,   533,   534,     0,   535,   536,   537,   538,   539,
     540,    59,     0,     0,   541,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,   543,   544,     0,    14,     0,     0,   545,   546,     0,
       0,     0,     0,     0,   501,   502,   547,   608,   548,     0,
     549,   550,     0,   551,   503,   504,   505,   506,   507,     0,
       0,     0,     0,     0,   508,     0,   509,     0,     0,     0,
     510,     0,     0,     0,     0,     0,     0,     0,   511,     0,
       0,     0,     0,     0,   512,     0,     0,   513,     0,     0,
     514,     0,     0,     0,   515,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   516,     0,     0,   517,   518,     0,
     242,   243,   244,     0,   246,   247,   248,   249,   250,   519,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,     0,   264,   265,   266,     0,     0,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   520,   521,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   526,   527,   528,   529,   530,     0,   531,     0,
     532,   533,   534,     0,   535,   536,   537,   538,   539,   540,
      59,     0,     0,   541,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   542,
     543,   544,     0,    14,     0,     0,   545,   546,     0,     0,
       0,     0,     0,   501,   502,   547,   809,   548,     0,   549,
     550,     0,   551,   503,   504,   505,   506,   507,     0,     0,
       0,     0,     0,   508,     0,   509,     0,     0,     0,   510,
       0,     0,     0,     0,     0,     0,     0,   511,     0,     0,
       0,     0,     0,   512,     0,     0,   513,     0,     0,   514,
       0,     0,     0,   515,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   516,     0,     0,   517,   518,     0,   242,
     243,   244,     0,   246,   247,   248,   249,   250,   519,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
       0,   264,   265,   266,     0,     0,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   520,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,     0,     0,     0,     0,  1037,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   526,   527,   528,   529,   530,     0,   531,     0,   532,
     533,   534,     0,   535,   536,   537,   538,   539,   540,    59,
       0,     0,   541,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   542,   543,
     544,     0,    14,     0,     0,   545,   546,     0,     0,     0,
       0,     0,   501,   502,   547,     0,   548,     0,   549,   550,
    1130,   551,   503,   504,   505,   506,   507,     0,     0,     0,
       0,     0,   508,     0,   509,     0,     0,     0,   510,     0,
       0,     0,     0,     0,     0,     0,   511,     0,     0,     0,
       0,     0,   512,     0,     0,   513,     0,     0,   514,     0,
       0,     0,   515,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   516,     0,     0,   517,   518,     0,   242,   243,
     244,     0,   246,   247,   248,   249,   250,   519,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,     0,
     264,   265,   266,     0,     0,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   520,   521,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     526,   527,   528,   529,   530,     0,   531,     0,   532,   533,
     534,     0,   535,   536,   537,   538,   539,   540,    59,     0,
       0,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   542,   543,   544,
       0,    14,     0,     0,   545,   546,     0,     0,     0,     0,
       0,   501,   502,   547,     0,   548,     0,   549,   550,     0,
     551,   503,   504,   505,   506,   507,     0,     0,     0,     0,
       0,   508,     0,   509,     0,     0,     0,   510,     0,     0,
       0,     0,     0,     0,     0,   511,     0,     0,     0,     0,
       0,   512,     0,     0,   513,     0,     0,   514,     0,     0,
       0,   515,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,     0,     0,   517,   518,     0,   242,   243,   244,
       0,   246,   247,   248,   249,   250,   519,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,     0,   264,
     265,   266,     0,     0,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   520,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,     0,     0,     0,     0,  1341,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   526,
     527,   528,   529,   530,     0,   531,     0,   532,   533,   534,
       0,   535,   536,   537,   538,   539,   540,    59,     0,     0,
     541,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   542,   543,   544,     0,
      14,     0,     0,   545,   546,     0,     0,     0,     0,     0,
     501,   502,   547,     0,   548,     0,   549,   550,     0,   551,
     503,   504,   505,   506,   507,     0,     0,     0,     0,     0,
     508,     0,   509,     0,     0,     0,   510,     0,     0,     0,
       0,     0,     0,     0,   511,     0,     0,     0,     0,     0,
     512,     0,     0,   513,     0,     0,   514,     0,     0,     0,
     515,     0,     0,     0,     0,     0,  1344,     0,     0,     0,
     516,     0,     0,   517,   518,     0,   242,   243,   244,     0,
     246,   247,   248,   249,   250,   519,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,     0,   264,   265,
     266,     0,     0,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     520,   521,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   525,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,   529,   530,     0,   531,     0,   532,   533,   534,     0,
     535,   536,   537,   538,   539,   540,    59,     0,     0,   541,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   542,   543,   544,     0,    14,
       0,     0,   545,   546,     0,     0,     0,     0,     0,   501,
     502,   547,     0,   548,     0,   549,   550,     0,   551,   503,
     504,   505,   506,   507,     0,     0,     0,     0,     0,   508,
       0,   509,     0,     0,     0,   510,     0,     0,     0,     0,
       0,     0,     0,   511,     0,     0,     0,     0,     0,   512,
       0,     0,   513,     0,     0,   514,     0,     0,     0,   515,
       0,     0,  1350,     0,     0,     0,     0,     0,     0,   516,
       0,     0,   517,   518,     0,   242,   243,   244,     0,   246,
     247,   248,   249,   250,   519,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,     0,   264,   265,   266,
       0,     0,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   520,
     521,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   526,   527,   528,
     529,   530,     0,   531,     0,   532,   533,   534,     0,   535,
     536,   537,   538,   539,   540,    59,     0,     0,   541,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   542,   543,   544,     0,    14,     0,
       0,   545,   546,     0,     0,     0,     0,     0,   501,   502,
     547,     0,   548,     0,   549,   550,     0,   551,   503,   504,
     505,   506,   507,     0,     0,     0,     0,     0,   508,     0,
     509,     0,     0,     0,   510,     0,     0,     0,     0,     0,
       0,     0,   511,     0,     0,     0,     0,     0,   512,     0,
       0,   513,     0,     0,   514,     0,     0,     0,   515,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   516,     0,
       0,   517,   518,     0,   242,   243,   244,     0,   246,   247,
     248,   249,   250,   519,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,     0,   264,   265,   266,     0,
       0,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   520,   521,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,     0,  1353,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   525,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   526,   527,   528,   529,
     530,     0,   531,     0,   532,   533,   534,     0,   535,   536,
     537,   538,   539,   540,    59,     0,     0,   541,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,   543,   544,     0,    14,     0,     0,
     545,   546,     0,     0,     0,     0,     0,   501,   502,   547,
       0,   548,     0,   549,   550,     0,   551,   503,   504,   505,
     506,   507,     0,     0,     0,     0,     0,   508,     0,   509,
       0,     0,     0,   510,     0,     0,     0,     0,     0,     0,
       0,   511,     0,     0,     0,     0,     0,   512,     0,     0,
     513,     0,     0,   514,     0,     0,     0,   515,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   516,     0,     0,
     517,   518,     0,   242,   243,   244,     0,   246,   247,   248,
     249,   250,   519,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,     0,   264,   265,   266,     0,     0,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   520,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   526,   527,   528,   529,   530,
       0,   531,     0,   532,   533,   534,     0,   535,   536,   537,
     538,   539,   540,    59,     0,     0,   541,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   542,   543,   544,     0,    14,     0,     0,   545,
     546,     0,     0,     0,     0,     0,   501,   502,   547,     0,
     548,  1632,   549,   550,     0,   551,   503,   504,   505,   506,
     507,     0,     0,     0,     0,     0,   508,     0,   509,     0,
       0,     0,   510,     0,     0,     0,     0,     0,     0,     0,
     511,     0,     0,     0,     0,     0,   512,     0,     0,   513,
       0,     0,   514,     0,     0,     0,   515,  1663,     0,     0,
       0,     0,     0,     0,     0,     0,   516,     0,     0,   517,
     518,     0,   242,   243,   244,     0,   246,   247,   248,   249,
     250,   519,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,     0,   264,   265,   266,     0,     0,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   520,   521,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   525,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   526,   527,   528,   529,   530,     0,
     531,     0,   532,   533,   534,     0,   535,   536,   537,   538,
     539,   540,    59,     0,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,   543,   544,     0,    14,     0,     0,   545,   546,
       0,     0,     0,     0,     0,   501,   502,   547,     0,   548,
       0,   549,   550,     0,   551,   503,   504,   505,   506,   507,
       0,     0,     0,     0,     0,   508,     0,   509,     0,     0,
       0,   510,     0,     0,     0,     0,     0,     0,     0,   511,
       0,     0,     0,     0,     0,   512,     0,     0,   513,     0,
       0,   514,     0,     0,     0,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   516,     0,     0,   517,   518,
       0,   242,   243,   244,     0,   246,   247,   248,   249,   250,
     519,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,     0,   264,   265,   266,     0,     0,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   520,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,   529,   530,     0,   531,
       0,   532,   533,   534,     0,   535,   536,   537,   538,   539,
     540,    59,     0,     0,   541,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,   543,   544,     0,    14,     0,     0,   545,   546,     0,
       0,     0,     0,     0,   501,   502,   547,     0,   548,     0,
     549,   550,     0,   551,   503,   504,   505,   506,   507,     0,
       0,     0,     0,     0,   508,  1144,   509,  1145,     0,     0,
     510,     0,     0,     0,     0,     0,     0,     0,   511,     0,
       0,     0,     0,     0,   512,     0,     0,   513,     0,     0,
     514,  1150,     0,     0,   515,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   516,     0,     0,   517,   518,     0,
     242,   243,   244,     0,   246,   247,   248,   249,   250,   519,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,     0,   264,   265,   266,     0,     0,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   520,   521,   522,  1155,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   526,   527,   528,   529,   530,     0,   531,     0,
     532,   533,   534,     0,   535,   536,   537,   538,   539,   540,
      59,     0,     0,   541,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   542,
     543,   544,     0,    14,     0,     0,   545,   546,     0,     0,
       0,   501,   502,     0,     0,   547,     0,   548,     0,   549,
     550,   503,   504,   505,   506,   507,     0,     0,     0,     0,
       0,   508,     0,   509,     0,     0,     0,   510,     0,     0,
       0,     0,     0,     0,     0,   511,     0,     0,     0,     0,
       0,   512,     0,     0,   513,     0,     0,   514,     0,     0,
       0,   515,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,     0,     0,   517,   518,     0,   242,   243,   244,
       0,   246,   247,   248,   249,   250,   519,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,     0,   264,
     265,   266,     0,     0,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   520,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   526,
     527,   528,   529,   530,     0,   531,     0,   532,   533,   534,
     709,   535,   536,   537,   538,   539,   540,    59,     0,     0,
     541,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   542,   543,   544,     0,
      14,     0,     0,   545,   546,     0,     0,     0,   501,   502,
       0,     0,   547,     0,   548,     0,   549,   550,   503,   504,
     505,   506,   507,     0,     0,     0,     0,     0,   508,     0,
     509,     0,     0,     0,   510,     0,     0,     0,     0,     0,
       0,     0,   511,     0,     0,     0,     0,     0,   512,     0,
       0,   513,     0,     0,   514,     0,     0,     0,   515,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   516,     0,
       0,   517,   518,     0,   242,   243,   244,     0,   246,   247,
     248,   249,   250,   519,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,     0,   264,   265,   266,     0,
       0,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   520,   521,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,     0,   854,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   525,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   526,   527,   528,   529,
     530,     0,   531,     0,   532,   533,   534,     0,   535,   536,
     537,   538,   539,   540,    59,     0,     0,   541,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,   543,   544,     0,    14,     0,     0,
     545,   546,     0,     0,     0,   501,   502,     0,     0,   547,
       0,   548,     0,   549,   550,   503,   504,   505,   506,   507,
       0,     0,   994,     0,     0,   508,     0,   509,     0,     0,
       0,   510,     0,     0,     0,     0,     0,     0,     0,   511,
       0,     0,     0,     0,     0,   512,     0,     0,   513,     0,
       0,   514,     0,     0,     0,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   516,     0,     0,   517,   518,
       0,   242,   243,   244,     0,   246,   247,   248,   249,   250,
     519,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,     0,   264,   265,   266,     0,     0,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   520,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,   529,   530,     0,   531,
       0,   532,   533,   534,     0,   535,   536,   537,   538,   539,
     540,    59,     0,     0,   541,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,   543,   544,     0,    14,     0,     0,   545,   546,     0,
       0,     0,   501,   502,     0,     0,   547,     0,   548,     0,
     549,   550,   503,   504,   505,   506,   507,     0,     0,     0,
       0,     0,   508,     0,   509,     0,     0,     0,   510,     0,
       0,     0,     0,     0,     0,     0,   511,     0,     0,     0,
       0,     0,   512,     0,     0,   513,     0,     0,   514,     0,
       0,     0,   515,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   516,     0,     0,   517,   518,     0,   242,   243,
     244,     0,   246,   247,   248,   249,   250,   519,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,     0,
     264,   265,   266,     0,     0,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   520,   521,   522,   -88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   735,   736,   523,   524,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     526,   527,   528,   529,   530,     0,   531,     0,   532,   533,
     534,     0,   535,   536,   537,   538,   539,   540,    59,     0,
       0,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   542,   543,   544,
       0,    14,     0,     0,   545,   546,     0,     0,     0,     0,
       0,     0,     0,   547,     0,   548,     0,   549,   550,     0,
       0,     0,     0,     0,     0,     0,  1374,  1375,  1376,  1377,
    1378,  1379,  1380,  1381,   737,   738,   739,   740,   741,  1382,
    1383,   742,   743,   744,   745,  1384,   746,   747,   748,   749,
     750,   751,   752,   753,  1385,  1386,  1387,   754,   755,   756,
     757,   758,  1388,   759,   760,  1389,  1390,   761,     0,     0,
       0,   762,   763,   764,  1391,  1392,  1393,   765,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1394,   766,     0,
     767,   768,   769,   770,   771,   772,   773,   774,   775,   776,
     236,     0,     0,     0,     0,     0,   237,     0,     0,     0,
     777,   778,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   239,     0,     0,     0,     0,     0,     0,     0,
     240,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
     735,   736,     0,   236,     0,     0,     0,     0,     0,   237,
       0,     0,   791,     0,    13,   238,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   239,     0,     0,     0,     0,
       0,     0,     0,   240,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   241,     0,
       0,   299,     0,    16,     0,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,     0,     0,     0,     0,     0,     0,     0,   737,   738,
     739,   740,   741,     0,     0,   742,   743,   744,   745,     0,
     746,   747,   748,   749,   750,   751,   752,   753,     0,     0,
       0,   754,   755,   756,   757,   758,     0,   759,   760,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   298,     0,   236,     0,     0,   421,   422,     0,
     237,     0,     0,     0,     0,    59,   238,     0,     0,     0,
       0,     0,     0,     0,   423,     0,   239,   770,   771,   772,
     773,   774,   775,   776,   240,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   777,   778,     0,     0,     0,   241,
       0,     0,     0,     0,   299,     0,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   735,   736,     0,     0,     0,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,     0,   442,   443,   444,   445,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,   447,   448,   449,   450,     0,     0,
      58,     0,   451,   452,   453,   454,   455,   456,   457,     0,
       0,     0,     0,   298,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   791,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   458,   459,
       0,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   735,   736,   470,   471,     0,     0,     0,     0,     0,
       0,   472,   473,     0,     0,   299,     0,     0,     0,     0,
     737,   738,   739,   740,   741,     0,     0,   742,   743,   744,
     745,     0,   746,   747,   748,   749,   750,   751,   752,   753,
       0,     0,     0,   754,   755,   756,   757,   758,     0,   759,
     760,     0,     0,   761,     0,     0,     0,   762,   763,   764,
       0,     0,     0,   765,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   735,   736,   766,     0,   767,   768,   769,   770,
     771,   772,   773,   774,   775,   776,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   777,   778,     0,   737,
     738,   739,   740,   741,     0,     0,   742,   743,   744,   745,
       0,   746,   747,   748,   749,   750,   751,   752,   753,     0,
       0,     0,   754,   755,   756,   757,   758,     0,   759,   760,
       0,     0,  -961,     0,     0,     0,   762,   763,   764,     0,
       0,     0,  -961,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   735,   736,     0,     0,     0,     0,     0,
       0,     0,     0,   766,     0,   767,   768,   769,   770,   771,
     772,   773,   774,   775,   776,     0,     0,     0,     0,     0,
     737,   738,   739,   740,   741,   777,   778,   742,   743,   744,
     745,     0,   746,   747,   748,   749,   750,   751,   752,   753,
       0,     0,     0,   754,   755,   756,   757,   758,     0,   759,
     760,     0,     0,     0,     0,     0,     0,   762,   763,   764,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   735,   736,     0,     0,     0,     0,
       0,     0,     0,     0,   766,     0,   767,   768,   769,   770,
     771,   772,   773,   774,   775,   776,     0,     0,     0,     0,
       0,   737,   738,   739,   740,   741,   777,   778,   742,   743,
     744,   745,     0,   746,   747,   748,   749,   750,   751,   752,
     753,     0,     0,     0,   754,   755,   756,   757,   758,     0,
     759,   760,     0,     0,     0,     0,     0,     0,   762,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   735,   736,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,     0,     0,     0,
       0,     0,   737,   738,   739,   740,   741,   777,   778,   742,
     743,   744,   745,     0,   746,   747,   748,   749,   750,   751,
     752,   753,     0,     0,     0,   754,   755,   756,   757,   758,
       0,   759,   760,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   735,   736,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   767,   768,
     769,   770,   771,   772,   773,   774,   775,   776,     0,     0,
       0,     0,     0,   737,   738,   739,   740,   741,   777,   778,
     742,   743,   744,   745,     0,   746,   747,   748,   749,   750,
     751,   752,   753,     0,     0,     0,   754,   755,   756,   757,
     758,     0,   759,   760,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     768,   769,   770,   771,   772,   773,   774,   775,   776,     0,
       0,     0,     0,     0,   737,   738,   739,   740,   741,   777,
     778,   742,   743,   744,   745,     0,   746,   747,   748,   749,
     750,   751,   752,   753,     0,     0,     0,   754,   755,   756,
     757,   758,     0,   759,   760,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     944,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   769,   770,   771,   772,   773,   774,   775,   776,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     777,   778,   242,   243,   244,   948,   246,   247,   248,   249,
     250,   519,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,     0,   264,   265,   266,     0,     0,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,     0,   242,   243,   244,
       0,   246,   247,   248,   249,   250,   519,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,     0,   264,
     265,   266,     0,     0,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   945,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   946,     0,     0,     0,   970,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   949,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   950,   242,   243,
     244,   974,   246,   247,   248,   249,   250,   519,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,     0,
     264,   265,   266,     0,     0,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,     0,   242,   243,   244,     0,   246,   247,   248,
     249,   250,   519,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,     0,   264,   265,   266,     0,     0,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   971,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   972,     0,
       0,     0,   978,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   975,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   976,   242,   243,   244,  1203,   246,   247,
     248,   249,   250,   519,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,     0,   264,   265,   266,     0,
       0,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,     0,   242,
     243,   244,     0,   246,   247,   248,   249,   250,   519,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
       0,   264,   265,   266,     0,     0,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   979,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   980,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1204,     0,
     319,     0,   320,     0,   321,   322,   323,   324,   325,  1205,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,     0,   337,   338,   339,     0,     0,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     367,   368,   242,   243,   244,     0,   246,   247,   248,   249,
     250,   519,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,     0,   264,   265,   266,     0,     0,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,     0,     0,     0,     0,
     369,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1006,  1007,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1008,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1009,     0,     0,     0,     0,     0,     0,     0,
       0,   319,     0,   320,     0,   321,   322,   323,   324,   325,
       0,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,     0,   337,   338,   339,  1010,  1011,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   319,     0,   320,     0,   321,
     322,   323,   324,   325,     0,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,     0,   337,   338,   339,
       0,     0,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,     0,
     319,     0,   320,     0,   321,   322,   323,   324,   325,     0,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   616,   337,   338,   339,     0,     0,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   618,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     816
};

static const yytype_int16 yycheck[] =
{
       1,    14,    15,   104,   150,   151,   177,    45,   644,   404,
     548,   648,   856,   505,   505,  1123,   206,   615,  1291,    20,
      21,    22,    81,   720,   721,  1052,     8,   656,   614,   421,
     422,     5,    22,   903,    33,   416,    20,    20,  1480,    19,
     906,    33,  1723,  1240,   943,   235,     5,     6,    86,    50,
      51,    64,    65,    66,  1146,   724,  1538,   595,    40,  1041,
      20,    46,  1517,  1781,  1486,  1781,    25,    57,   149,   786,
    1499,   788,    31,   790,  1781,   206,    20,   586,  1060,   588,
    1781,   590,  1781,   201,   176,   177,   178,   174,    33,   172,
     203,     8,   105,   106,   107,   108,   134,   135,     0,   137,
     174,   211,  1129,   800,   235,     7,  1535,   174,   174,    68,
      69,    19,    20,  1709,   495,    60,    61,   150,    33,    36,
     187,   187,   240,   150,  1842,   238,  1842,   237,    30,   190,
      32,   173,    34,   214,   167,  1842,   523,   524,    40,   677,
     201,  1842,   935,  1842,   129,    60,    61,   239,    50,   236,
     688,   125,   126,   691,    56,   542,   543,   544,   545,   546,
     243,    69,   221,  1881,   238,  1881,   125,   126,   797,   243,
    1612,  1613,   801,   239,  1881,   211,  1772,   238,    80,   212,
    1881,   202,  1881,   192,  1666,   212,   724,  1629,   215,  1631,
     211,     7,   889,   206,   190,   892,   214,  1878,   240,   241,
     218,   243,   147,   239,   901,   201,   151,   904,  1107,   380,
     203,  1892,   881,  1668,  1636,  1085,   175,  1325,   217,   220,
    1086,   203,   235,   125,   126,   217,   372,   211,   211,   866,
     622,   377,   147,   223,    50,   381,   151,   236,   218,  1436,
    1694,   779,   201,   239,   218,   235,   245,   634,   203,   242,
     149,   211,   213,  1707,   236,  1697,  1698,    62,   211,   218,
    1352,   890,   244,  1245,   238,   934,  1708,   211,   213,   761,
     761,   149,   217,   150,   219,   220,   201,   236,   203,  1261,
       5,     6,   175,   238,   243,   416,   299,   202,   203,   242,
     167,    33,   203,   201,  1321,   184,   211,   678,   213,   203,
     245,   203,   217,   175,   219,   220,  1760,  1761,   498,   499,
     500,   215,   201,   694,  1792,   214,   175,   242,    60,    61,
     219,   214,   224,   382,   212,   184,   218,   242,   150,   201,
     245,   242,    34,   244,   236,   212,   214,   211,   242,   238,
     242,   219,   201,   881,  1137,   167,   238,   493,  1621,   238,
     737,   738,  1830,   175,   741,   742,   743,   744,   211,   746,
     238,    63,    15,    16,   495,   239,   753,   498,   499,   500,
     757,   758,   759,   760,   761,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,   772,   773,   774,   775,   776,
     212,   581,   582,  1485,  1111,   585,  1189,   587,   211,   589,
     149,   184,   911,   416,  1033,   147,   150,   597,  1077,   151,
    1865,   152,   153,   154,   155,   156,   157,   214,   201,  1212,
     211,  1448,   211,   167,   211,  1142,   239,  1519,   130,  1066,
     219,   211,   941,   201,  1032,  1132,   202,   203,   176,   149,
     178,   631,   632,  1029,   150,   211,   150,  1045,   239,   215,
     581,   582,  1081,   242,   585,   238,   587,  1043,   589,   239,
     591,   167,   184,   167,   166,   214,   597,   854,   212,    12,
     219,   213,   201,   175,   211,   217,   242,  1106,   220,   201,
      23,    24,   495,   614,   865,   498,   499,   500,   211,   238,
     211,   238,   505,   506,   235,   236,    33,   878,   879,   201,
     631,   632,   239,   245,   214,  1812,   212,   888,   212,   219,
     175,  1355,   525,   894,   895,   896,   239,   898,   239,   900,
     173,   902,   182,    60,    61,   150,   228,   184,   238,   910,
     211,   215,  1839,   184,   192,   219,   238,   211,  1331,   192,
     193,   201,   211,   196,   201,   198,   218,   678,   201,   130,
     201,   204,   203,  1661,   149,   942,   237,   152,   153,   154,
     155,   156,   157,   694,   224,   239,   238,   220,   581,   582,
     239,    33,   585,   184,   587,   235,   589,   723,   591,   223,
     211,   223,    57,   150,   597,   238,   211,   212,    63,   214,
     201,   242,   217,   244,    21,    22,   182,   201,    60,    61,
     167,   614,   211,   211,   735,   736,    57,  1400,   239,   799,
     147,   211,    63,   201,   151,   201,    15,    16,   631,   632,
     211,  1648,   218,   754,   755,   756,   639,   758,  1421,   211,
     239,   239,   211,   211,   201,   205,   211,   223,   224,   239,
     235,   236,   238,   211,   182,   212,   777,   648,   239,   235,
     217,  1189,   218,   218,   223,  1193,   211,   239,   190,  1288,
     239,   239,   237,   201,    33,   678,   235,   201,   799,   201,
    1057,   239,   238,   238,  1212,  1184,   213,   201,  1395,  1217,
     217,   694,   219,   220,   239,   147,   224,   877,   150,   151,
     880,    60,    61,   211,   884,  1787,   184,   235,   236,    64,
      65,    66,   215,  1084,   201,   167,   219,   223,   245,  1090,
     201,   202,   725,   201,   141,   142,   211,   211,   201,   235,
     211,   239,   149,   239,  1105,   152,   153,   154,   155,   156,
     157,  1823,  1824,   211,   865,   162,   163,   164,   165,   166,
     105,   106,   107,   108,   239,   239,   877,   878,   879,   880,
     212,   213,   215,   884,   130,   217,   219,   888,   220,   211,
      33,   239,   211,   894,   895,   896,   211,   898,   203,   900,
    1862,   902,   903,  1311,   211,   175,   218,  1570,   147,   910,
    1318,   150,   151,   245,   184,   237,   799,    60,    61,   189,
     239,   211,   237,   192,   193,  1588,   238,   196,   167,   198,
     237,   201,   201,   130,   214,   204,   218,   217,   235,   236,
     202,   203,   813,    79,   815,   202,   203,   237,   218,   211,
    1207,   220,   202,   203,   211,    47,   238,   219,    94,   243,
     190,   211,   219,    99,   203,   101,   190,  1027,   190,   219,
    1030,   201,   190,   212,   213,    67,  1036,   201,   217,   201,
     242,   220,   865,   201,   211,   242,   201,   214,   190,   190,
     217,   235,   242,   994,   877,   878,   879,   880,   869,   201,
     201,   884,   201,   242,   147,   888,   245,   201,   151,   237,
      57,   894,   895,   896,   201,   898,    63,   900,   228,   902,
     903,   212,  1384,  1384,  1386,  1386,  1027,   910,  1029,  1030,
    1693,  1694,   214,  1747,   211,  1036,  1699,   214,   201,   202,
     217,   215,  1043,    75,  1707,   219,  1709,    79,   211,   242,
     243,   131,   132,   133,   134,   135,   136,   137,   138,    57,
     205,    93,    94,   202,   203,    63,    98,    99,   100,   101,
     213,   942,   211,    57,   217,    57,   219,   220,   176,    63,
     219,    63,    57,  1084,  1085,    33,   203,   167,    63,  1090,
    1753,   215,  1500,   205,   206,   219,   174,  1760,  1761,   179,
     180,   181,   245,   242,  1105,   192,   193,   194,   201,  1772,
     215,   198,    60,    61,   219,    21,    22,  1374,  1375,  1376,
    1377,  1378,  1379,  1380,  1381,  1382,  1383,  1384,  1385,  1386,
    1387,  1388,  1389,  1390,  1391,  1392,  1393,  1394,  1154,  1706,
     212,  1855,   391,    57,  1027,  1396,  1029,  1030,    10,    11,
     202,   203,   401,  1036,   202,   203,    66,   215,  1409,   211,
    1043,   219,   215,   211,   215,   414,   219,   219,   219,  1832,
    1884,   219,  1043,   215,  1045,   202,   203,   219,    33,   192,
     193,   194,   203,   204,   211,   198,  1057,   215,  1755,   215,
     242,   219,   219,   219,   242,  1066,  1256,  1068,   202,   147,
     215,  1084,  1085,   151,   219,    60,    61,  1090,   563,   564,
     129,   566,   567,   214,   564,   242,   566,   567,  1626,   176,
     177,   178,  1105,  1362,  1363,  1731,  1267,   205,   206,   207,
     208,   174,   201,   139,   140,   141,   142,   143,   218,   218,
     146,  1649,   218,   149,   238,   151,   152,   153,   154,   155,
     156,   157,   158,   218,   218,  1256,   162,   163,   164,   165,
     166,   238,   168,   169,   218,   213,   205,   206,   207,   217,
    1527,   219,   220,   205,   206,   207,   218,   848,   849,   850,
    1151,   218,  1789,   218,   218,   202,   203,   238,  1159,  1160,
    1306,   236,   147,   215,   211,   202,   151,   245,    55,    56,
      57,  1172,   219,  1174,  1175,  1176,  1177,  1178,    35,    33,
     243,  1182,   218,   219,   220,   221,   222,   223,   224,  1826,
      33,    35,   243,   238,  1340,   242,   202,   203,   201,   235,
     236,  1372,    33,   201,   218,   211,    60,    61,  1398,    21,
      22,   236,   243,   219,   201,   237,    22,    60,    61,   598,
     201,  1411,   237,  1413,  1619,   243,   214,   201,   213,    60,
      61,   610,   217,   238,   219,   220,   242,   238,   218,   218,
     218,   218,   218,  1256,   238,   218,  1784,   218,   218,   238,
     201,   219,   238,   211,   238,   238,   238,    33,    43,    13,
     245,     4,   219,   238,   236,  1396,   218,  1398,   201,   211,
     239,   238,  1273,   652,   239,   239,   239,   237,  1409,   238,
    1411,   237,  1413,   218,    60,    61,   239,   242,   238,   238,
     218,   218,   238,   147,   238,   238,   237,   151,   238,   201,
     679,   680,   239,   214,   147,   684,   237,   686,   151,   201,
     201,   201,   201,   211,   239,   239,   147,   239,   239,   239,
     151,   239,   239,   702,   703,   704,   705,   706,   707,   141,
     142,  1521,   239,   239,   238,     1,   239,   149,   239,   151,
     152,   153,   154,   155,   156,   157,   158,   218,   218,   237,
     162,   163,   164,   165,   166,   238,   238,   238,   238,   213,
     238,   238,   238,   217,   238,   219,   220,   218,   237,   748,
     213,   147,   751,   752,   217,   151,   219,   220,   237,   218,
     238,   218,   213,  1396,   237,  1398,   217,   239,   219,   220,
    1521,   245,   238,  1583,  1781,   219,  1409,   238,  1411,   778,
    1413,   219,   245,   238,  1791,   238,   238,  1788,   220,   221,
     222,   223,   224,  1603,   245,   219,   212,   239,  1564,   238,
      43,   212,   239,   235,   236,   238,   805,    43,   239,  1530,
     239,   239,    21,    22,   239,   238,   238,   213,   239,   239,
     239,   217,   239,   219,   220,   239,   214,    43,   238,   201,
     239,   238,  1583,   201,   238,  1842,  1457,   239,   244,  1460,
     243,   201,   192,    10,    33,   844,    42,    21,    22,   245,
      13,     9,  1603,  1486,    66,    33,   218,   238,   238,   237,
     237,  1868,   238,   862,   201,   201,   201,   238,   201,   868,
       8,    60,    61,   201,  1881,   244,   201,   244,   244,   238,
    1887,   239,    60,    61,   883,   219,   209,   201,  1521,   201,
     239,   201,   238,   201,   201,    14,   212,   212,   192,   238,
     238,   214,   244,   212,    43,    37,  1527,   238,   238,   908,
     909,    33,   239,   239,   212,   239,    67,   239,   239,   239,
     919,   239,   239,   219,    70,   924,   201,   926,   239,   928,
      43,   201,   141,   142,   933,   239,   201,   239,    60,    61,
     149,   244,   151,   152,   153,   154,   155,   156,   157,   158,
    1583,   238,   242,   162,   163,   164,   165,   166,   147,  1725,
     238,   238,   151,   239,  1730,   139,   140,   141,   142,   147,
    1603,   238,   238,   151,   238,   149,   238,   151,   152,   153,
     154,   155,   156,   157,   158,   238,   238,   238,   162,   163,
     164,   165,   166,   238,   168,   169,   201,   239,   238,    21,
      22,   239,   239,  1636,   238,  1663,   238,   238,  1007,   239,
     239,   239,  1011,   222,   223,   224,   238,   238,   238,  1785,
    1786,   238,   205,    33,   213,   147,   235,   236,   217,   151,
     219,   220,   239,   239,   244,   213,   239,  1788,  1037,   217,
     239,   219,   220,   243,    33,  1666,   220,   221,   222,   223,
     224,   239,   239,   239,   239,   239,   245,   239,   239,    33,
     239,   235,   236,   239,   239,   242,  1687,   245,  1067,   239,
     239,    60,    61,    12,   239,   211,   211,  1843,   239,   239,
     239,    53,   239,   239,  1850,   239,    60,    61,   243,   239,
    1856,   213,  1091,   242,   242,   217,   238,   219,   220,   239,
     244,   244,   239,   131,   132,   133,   134,   135,   136,   137,
     138,   239,   237,   237,   244,   238,  1882,   243,   239,   141,
     142,    80,   150,   245,   244,     1,   708,   149,    83,   151,
     152,   153,   154,   155,   156,   157,   158,  1891,  1137,   167,
     162,   163,   164,   165,   166,  1144,  1889,   815,  1147,   815,
     231,   179,   180,   181,  1153,  1788,  1155,   725,   147,  1363,
    1061,  1782,   151,   799,   729,     1,  1689,  1515,  1751,  1606,
    1791,  1692,    54,   147,  1752,  1752,  1261,   151,  1060,  1060,
     374,   251,    -1,    -1,   212,    -1,   505,  1186,    -1,  1188,
      -1,  1190,   679,  1192,    -1,  1194,    -1,    -1,    -1,  1198,
     505,  1200,    -1,  1202,   505,   505,   505,    -1,  1829,    -1,
     505,    -1,  1211,   235,   236,    -1,    -1,    -1,    -1,    -1,
    1219,    -1,    -1,    -1,   213,    -1,  1847,    -1,   217,    -1,
     219,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,
    1861,    -1,  1863,   217,  1243,   219,   220,  1868,    -1,    -1,
      -1,    -1,    -1,    -1,  1253,    -1,   245,    -1,    -1,  1258,
      -1,  1260,    -1,    -1,  1885,    -1,  1887,    -1,    -1,    -1,
    1269,   245,    -1,  1272,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      33,    25,    -1,    -1,    -1,  1314,    -1,    31,    -1,    -1,
      -1,    -1,    -1,  1322,  1323,  1324,    -1,    41,    -1,    60,
      61,    -1,    -1,    -1,    -1,    49,    -1,    60,    61,    -1,
      -1,    -1,  1341,    -1,    -1,  1344,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,  1353,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,    -1,    -1,    -1,   147,  1416,    -1,  1418,
     151,    -1,    -1,    -1,   147,    -1,  1425,    -1,   151,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,  1466,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   213,    -1,    -1,  1484,   217,   201,   219,   220,
     213,    -1,    -1,    -1,   217,    -1,   219,   220,    -1,    -1,
      -1,    -1,  1501,  1502,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   245,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   245,  1522,    -1,    -1,   240,    -1,    -1,  1528,
    1529,   245,    -1,    -1,    -1,  1534,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,  1570,    -1,    -1,   162,   163,   164,   165,   166,    -1,
     168,   169,    -1,    -1,    -1,    -1,    -1,    -1,   176,  1588,
     178,    -1,    -1,    -1,    -1,  1594,    -1,    -1,  1597,    -1,
    1599,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1621,  1622,  1623,  1624,  1625,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,    -1,    -1,    -1,
      -1,    -1,    -1,  1642,    -1,    -1,    -1,   235,   236,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,  1662,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,    -1,    -1,    -1,
     162,   163,   164,   165,   166,    -1,   168,   169,    -1,    -1,
     172,    -1,    -1,    -1,   176,   177,   178,  1696,    -1,    -1,
     182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1713,    -1,    -1,    -1,    -1,  1718,
    1719,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   235,   236,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1776,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1813,     1,    -1,    -1,    -1,     5,
       6,     7,    -1,     9,    10,    11,    -1,    13,  1827,    15,
      16,    17,    18,    19,    -1,    -1,    -1,  1836,    -1,    25,
      26,    27,    28,    29,    -1,    31,    -1,  1846,    -1,    -1,
      -1,    -1,    38,    39,    40,    -1,    42,    -1,    44,    45,
      -1,    -1,    48,    -1,    50,    51,    52,    -1,    54,    55,
      -1,  1870,    58,    59,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,
     186,   187,    -1,   189,    -1,   191,   192,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   220,   221,   222,    -1,   224,    -1,
      -1,   227,   228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     236,    -1,   238,    -1,   240,   241,   242,   243,   244,     1,
      -1,    -1,    -1,     5,     6,     7,    -1,     9,    10,    11,
      -1,    13,    -1,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,    -1,
      42,    -1,    44,    45,    -1,    -1,    48,    -1,    50,    51,
      52,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   173,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   183,   184,   185,   186,   187,    -1,   189,    -1,   191,
     192,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,   221,
     222,    -1,   224,    -1,    -1,   227,   228,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   236,    -1,   238,    -1,   240,   241,
     242,   243,   244,     1,    -1,    -1,    -1,     5,     6,     7,
      -1,     9,    10,    11,    -1,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    40,    -1,    42,    -1,    44,    45,    -1,    -1,
      48,    -1,    50,    51,    52,    -1,    54,    55,    -1,    -1,
      58,    59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,   184,   185,   186,   187,
      -1,   189,    -1,   191,   192,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   220,   221,   222,    -1,   224,    -1,    -1,   227,
     228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   236,    -1,
     238,    -1,   240,   241,   242,   243,   244,     1,    -1,    -1,
      -1,     5,     6,     7,    -1,     9,    10,    11,    -1,    13,
      -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    29,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    40,    -1,    42,    -1,
      44,    45,    -1,    -1,    48,    -1,    50,    51,    52,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,
     184,   185,   186,   187,    -1,   189,    -1,   191,   192,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   220,   221,   222,    -1,
     224,    -1,    -1,   227,   228,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   236,    -1,   238,    -1,   240,   241,   242,   243,
     244,     1,    -1,    -1,    -1,     5,     6,     7,    -1,     9,
      10,    11,    -1,    13,    -1,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,
      40,    -1,    42,    -1,    44,    45,    -1,    -1,    48,    -1,
      50,    51,    52,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,   184,   185,   186,   187,    -1,   189,
      -1,   191,   192,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     220,   221,   222,    -1,   224,    -1,    -1,   227,   228,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   236,    -1,   238,    -1,
     240,   241,   242,   243,   244,     1,    -1,    -1,    -1,     5,
       6,     7,    -1,     9,    10,    11,    -1,    13,    -1,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    39,    40,    -1,    42,    -1,    44,    45,
      -1,    -1,    48,    -1,    50,    51,    52,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,
     186,   187,    -1,   189,    -1,   191,   192,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   220,   221,   222,    -1,   224,    -1,
      -1,   227,   228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     236,    -1,   238,    -1,   240,   241,   242,   243,   244,     5,
       6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    70,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,
     186,   187,    -1,   189,   190,   191,   192,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   220,   221,   222,    -1,   224,    -1,
      -1,   227,   228,    -1,    -1,    -1,    -1,    -1,     5,     6,
     236,    -1,   238,   239,   240,   241,    13,   243,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,   186,
     187,    -1,   189,   190,   191,   192,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   220,   221,   222,    -1,   224,    -1,    -1,
     227,   228,    -1,    -1,    -1,    -1,    -1,     5,     6,   236,
      -1,   238,    -1,   240,   241,    13,   243,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,   184,   185,   186,   187,
      -1,   189,    -1,   191,   192,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   220,   221,   222,    -1,   224,    -1,    -1,   227,
     228,    -1,    -1,    -1,    -1,    -1,     5,     6,   236,    -1,
     238,    -1,   240,   241,    -1,   243,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    70,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,   184,   185,   186,   187,    -1,
     189,   190,   191,   192,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   220,   221,   222,    -1,   224,    -1,    -1,   227,   228,
      -1,    -1,    -1,    -1,    -1,     5,     6,   236,    -1,   238,
      -1,   240,   241,    -1,   243,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,   184,   185,   186,   187,    -1,   189,
     190,   191,   192,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     220,   221,   222,    -1,   224,    -1,    -1,   227,   228,    -1,
      -1,    -1,    -1,    -1,     5,     6,   236,    -1,   238,   239,
     240,   241,    -1,   243,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,   184,   185,   186,   187,    -1,   189,   190,
     191,   192,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,
     221,   222,    -1,   224,    -1,    -1,   227,   228,    -1,    -1,
      -1,    -1,    -1,     5,     6,   236,    -1,   238,   239,   240,
     241,    -1,   243,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   173,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   183,   184,   185,   186,   187,    -1,   189,   190,   191,
     192,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,   221,
     222,    -1,   224,    -1,    -1,   227,   228,    -1,    -1,    -1,
      -1,    -1,     5,     6,   236,    -1,   238,    -1,   240,   241,
      -1,   243,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,   184,   185,   186,   187,    -1,   189,    -1,   191,   192,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,   221,   222,
      -1,   224,    -1,    -1,   227,   228,    -1,    -1,    -1,    -1,
      -1,     5,     6,   236,    -1,   238,   239,   240,   241,    -1,
     243,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,
     184,   185,   186,   187,    -1,   189,   190,   191,   192,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   220,   221,   222,    -1,
     224,    -1,    -1,   227,   228,    -1,    -1,    -1,    -1,    -1,
       5,     6,   236,    -1,   238,    -1,   240,   241,    -1,   243,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,
     185,   186,   187,    -1,   189,    -1,   191,   192,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   220,   221,   222,    -1,   224,
      -1,    -1,   227,   228,    -1,    -1,    -1,    -1,    -1,     5,
       6,   236,    -1,   238,   239,   240,   241,    -1,   243,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,
     186,   187,    -1,   189,    -1,   191,   192,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   220,   221,   222,    -1,   224,    -1,
      -1,   227,   228,    -1,    -1,    -1,    -1,    -1,     5,     6,
     236,    -1,   238,   239,   240,   241,    -1,   243,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,   186,
     187,    -1,   189,    -1,   191,   192,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   220,   221,   222,    -1,   224,    -1,    -1,
     227,   228,    -1,    -1,    -1,    -1,    -1,     5,     6,   236,
      -1,   238,   239,   240,   241,    -1,   243,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,   184,   185,   186,   187,
      -1,   189,   190,   191,   192,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   220,   221,   222,    -1,   224,    -1,    -1,   227,
     228,    -1,    -1,    -1,    -1,    -1,     5,     6,   236,    -1,
     238,    -1,   240,   241,    -1,   243,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,   184,   185,   186,   187,    -1,
     189,    -1,   191,   192,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   220,   221,   222,    -1,   224,    -1,    -1,   227,   228,
      -1,    -1,    -1,    -1,    -1,     5,     6,   236,    -1,   238,
      -1,   240,   241,    -1,   243,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,   184,   185,   186,   187,    -1,   189,
      -1,   191,   192,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     220,   221,   222,    -1,   224,    -1,    -1,   227,   228,    -1,
      -1,    -1,    -1,    -1,     5,     6,   236,   237,   238,    -1,
     240,   241,    -1,   243,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,   184,   185,   186,   187,    -1,   189,    -1,
     191,   192,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,
     221,   222,    -1,   224,    -1,    -1,   227,   228,    -1,    -1,
      -1,    -1,    -1,     5,     6,   236,   237,   238,    -1,   240,
     241,    -1,   243,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   173,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   183,   184,   185,   186,   187,    -1,   189,    -1,   191,
     192,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,   221,
     222,    -1,   224,    -1,    -1,   227,   228,    -1,    -1,    -1,
      -1,    -1,     5,     6,   236,    -1,   238,    -1,   240,   241,
      13,   243,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,   184,   185,   186,   187,    -1,   189,    -1,   191,   192,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,   221,   222,
      -1,   224,    -1,    -1,   227,   228,    -1,    -1,    -1,    -1,
      -1,     5,     6,   236,    -1,   238,    -1,   240,   241,    -1,
     243,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,
     184,   185,   186,   187,    -1,   189,    -1,   191,   192,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   220,   221,   222,    -1,
     224,    -1,    -1,   227,   228,    -1,    -1,    -1,    -1,    -1,
       5,     6,   236,    -1,   238,    -1,   240,   241,    -1,   243,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,
     185,   186,   187,    -1,   189,    -1,   191,   192,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   220,   221,   222,    -1,   224,
      -1,    -1,   227,   228,    -1,    -1,    -1,    -1,    -1,     5,
       6,   236,    -1,   238,    -1,   240,   241,    -1,   243,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,
     186,   187,    -1,   189,    -1,   191,   192,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   220,   221,   222,    -1,   224,    -1,
      -1,   227,   228,    -1,    -1,    -1,    -1,    -1,     5,     6,
     236,    -1,   238,    -1,   240,   241,    -1,   243,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,   186,
     187,    -1,   189,    -1,   191,   192,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   220,   221,   222,    -1,   224,    -1,    -1,
     227,   228,    -1,    -1,    -1,    -1,    -1,     5,     6,   236,
      -1,   238,    -1,   240,   241,    -1,   243,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,   184,   185,   186,   187,
      -1,   189,    -1,   191,   192,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   220,   221,   222,    -1,   224,    -1,    -1,   227,
     228,    -1,    -1,    -1,    -1,    -1,     5,     6,   236,    -1,
     238,   239,   240,   241,    -1,   243,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,   184,   185,   186,   187,    -1,
     189,    -1,   191,   192,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   220,   221,   222,    -1,   224,    -1,    -1,   227,   228,
      -1,    -1,    -1,    -1,    -1,     5,     6,   236,    -1,   238,
      -1,   240,   241,    -1,   243,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,   184,   185,   186,   187,    -1,   189,
      -1,   191,   192,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     220,   221,   222,    -1,   224,    -1,    -1,   227,   228,    -1,
      -1,    -1,    -1,    -1,     5,     6,   236,    -1,   238,    -1,
     240,   241,    -1,   243,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    52,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,   184,   185,   186,   187,    -1,   189,    -1,
     191,   192,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,
     221,   222,    -1,   224,    -1,    -1,   227,   228,    -1,    -1,
      -1,     5,     6,    -1,    -1,   236,    -1,   238,    -1,   240,
     241,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,
     184,   185,   186,   187,    -1,   189,    -1,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   220,   221,   222,    -1,
     224,    -1,    -1,   227,   228,    -1,    -1,    -1,     5,     6,
      -1,    -1,   236,    -1,   238,    -1,   240,   241,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,   186,
     187,    -1,   189,    -1,   191,   192,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   220,   221,   222,    -1,   224,    -1,    -1,
     227,   228,    -1,    -1,    -1,     5,     6,    -1,    -1,   236,
      -1,   238,    -1,   240,   241,    15,    16,    17,    18,    19,
      -1,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,   184,   185,   186,   187,    -1,   189,
      -1,   191,   192,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     220,   221,   222,    -1,   224,    -1,    -1,   227,   228,    -1,
      -1,    -1,     5,     6,    -1,    -1,   236,    -1,   238,    -1,
     240,   241,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,   141,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,   184,   185,   186,   187,    -1,   189,    -1,   191,   192,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,   221,   222,
      -1,   224,    -1,    -1,   227,   228,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   236,    -1,   238,    -1,   240,   241,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,    -1,    -1,
      -1,   176,   177,   178,   179,   180,   181,   182,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,   213,    -1,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
     235,   236,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      21,    22,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,   201,    -1,   203,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,   240,    -1,   242,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,    -1,    -1,
      -1,   162,   163,   164,   165,   166,    -1,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,    19,    -1,    -1,    21,    22,    -1,
      25,    -1,    -1,    -1,    -1,   201,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    41,   218,   219,   220,
     221,   222,   223,   224,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   235,   236,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,   240,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,    21,    22,    -1,    -1,    -1,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,   168,   169,   170,   171,    -1,    -1,
     175,    -1,   176,   177,   178,   179,   180,   181,   182,    -1,
      -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,   213,
      -1,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,    21,    22,   227,   228,    -1,    -1,    -1,    -1,    -1,
      -1,   235,   236,    -1,    -1,   240,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
      -1,    -1,    -1,   162,   163,   164,   165,   166,    -1,   168,
     169,    -1,    -1,   172,    -1,    -1,    -1,   176,   177,   178,
      -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,   213,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    -1,   139,
     140,   141,   142,   143,    -1,    -1,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      -1,    -1,   162,   163,   164,   165,   166,    -1,   168,   169,
      -1,    -1,   172,    -1,    -1,    -1,   176,   177,   178,    -1,
      -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   213,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,   235,   236,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
      -1,    -1,    -1,   162,   163,   164,   165,   166,    -1,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   213,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   235,   236,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,    -1,    -1,    -1,   162,   163,   164,   165,   166,    -1,
     168,   169,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   235,   236,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,    -1,    -1,   162,   163,   164,   165,   166,
      -1,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,   235,   236,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,    -1,    -1,    -1,   162,   163,   164,   165,
     166,    -1,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,   235,
     236,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,    -1,    -1,    -1,   162,   163,   164,
     165,   166,    -1,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   217,   218,   219,   220,   221,   222,   223,   224,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,   236,    71,    72,    73,    19,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,    71,    72,
      73,    19,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,    71,    72,    73,    19,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,
      71,    -1,    73,    -1,    75,    76,    77,    78,    79,   201,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    -1,    -1,    -1,    -1,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,   235,   236,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    71,    -1,    73,    -1,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    -1,
      71,    -1,    73,    -1,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,   201,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     201
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   247,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   125,   126,   203,   224,   236,   242,   249,   250,   254,
     263,   265,   266,   271,   320,   326,   357,   447,   455,   464,
     474,   526,   531,   536,    19,    20,    69,   201,   310,   311,
     312,   192,   272,   273,   213,   268,   269,    57,    63,   452,
     453,   456,   201,   240,   252,   537,   527,   532,   175,   201,
     345,    34,    63,   130,   166,   228,   238,   315,   316,   317,
     318,   345,   249,   249,   249,     8,    36,   475,    62,   443,
     212,   211,   214,   211,   182,   201,   224,   235,   267,   236,
     267,   201,   249,   249,   443,   452,   452,   452,   201,   175,
     264,   317,   317,   317,   238,   176,   177,   178,   211,   237,
     130,   325,   465,     5,     6,   471,    57,    63,   444,    15,
      16,   173,   192,   193,   196,   198,   201,   204,   220,   238,
     256,   311,   192,   273,   223,   223,   267,   223,   223,   235,
     201,    22,    57,   270,   201,   454,    57,    63,   251,   201,
     201,   201,   201,   205,   262,   239,   312,   317,   317,   317,
     317,   203,   277,   278,    57,    63,   327,   329,    57,    63,
     458,   130,   130,    57,    63,   472,   243,   448,   201,   205,
     206,   207,   255,   192,   193,   194,   198,    15,    16,   192,
     193,   196,   198,   201,   220,   256,   308,   309,   267,   267,
     267,   201,   201,   237,   201,   457,   212,   228,   253,   214,
     485,   278,   278,   205,   239,   203,   330,   201,   459,   476,
     445,   174,   313,   413,   205,   206,   207,   192,   193,   194,
     198,   211,   239,   270,   270,   212,    19,    25,    31,    41,
      49,    64,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   188,   240,
     345,   479,   481,   482,   486,   492,   494,   525,    66,    79,
      94,    99,   101,   202,   462,   463,   528,   533,    35,    71,
      73,    75,    76,    77,    78,    79,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    93,    94,    95,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   141,   142,   201,
     323,   324,   328,   214,   460,   129,   469,   470,   244,   249,
     446,   311,   174,   201,   439,   442,   308,   525,   218,   218,
     218,   238,   218,   218,   238,   485,   218,   218,   218,   218,
     218,   238,   345,   218,   238,    33,    60,    61,   147,   151,
     213,   217,   220,   245,   236,   491,   215,   202,   538,   243,
     243,    21,    22,    38,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   150,   151,   152,   153,   167,   168,   169,   170,
     171,   176,   177,   178,   179,   180,   181,   182,   212,   213,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     227,   228,   235,   236,    35,    35,   238,   321,   278,    75,
      79,    93,    94,    98,    99,   100,   101,   480,   463,   201,
     278,   413,   278,   311,   211,   214,   217,   437,   495,   503,
     505,     5,     6,    15,    16,    17,    18,    19,    25,    27,
      31,    39,    45,    48,    51,    55,    65,    68,    69,    80,
     125,   126,   127,   141,   142,   173,   183,   184,   185,   186,
     187,   189,   191,   192,   193,   195,   196,   197,   198,   199,
     200,   204,   220,   221,   222,   227,   228,   236,   238,   240,
     241,   243,   261,   263,   337,   345,   350,   362,   369,   372,
     375,   379,   381,   383,   384,   386,   391,   394,   395,   396,
     411,   412,   479,   542,   550,   561,   564,   577,   578,   581,
     582,   507,   501,   201,   218,   509,   511,   513,   515,   517,
     519,   521,   523,   395,   218,   238,   493,   497,   150,   342,
     373,   395,    33,   217,    33,   217,   236,   245,   237,   395,
     236,   245,   492,   243,   529,   534,   201,   324,   201,   324,
     201,   237,    22,   201,   237,   187,   239,   413,   423,   424,
     425,   149,   214,   322,   172,   243,   334,   374,   243,   214,
     468,   477,   184,   201,   438,   441,   278,   201,   492,   150,
     167,   212,   436,   525,   525,   490,   525,   218,   218,   218,
     345,   347,   481,   541,   550,   561,   564,   577,   578,   581,
     582,   345,   218,     5,   125,   126,   218,   238,   218,   238,
     238,   218,   218,   218,   238,   218,   238,   218,   238,   218,
     218,   238,    19,   218,   218,   396,   396,   184,   189,   218,
     345,   385,   238,   238,   238,   238,   238,   238,   260,   194,
     396,   396,   396,   396,   396,    13,    49,   342,   190,   201,
     373,   543,   545,   576,   238,   236,   319,   173,   243,   375,
     380,   380,   380,   380,   239,    21,    22,   139,   140,   141,
     142,   143,   146,   147,   148,   149,   151,   152,   153,   154,
     155,   156,   157,   158,   162,   163,   164,   165,   166,   168,
     169,   172,   176,   177,   178,   182,   213,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   235,   236,   238,
     525,   525,   239,   219,   487,   525,   321,   525,   321,   525,
     321,   201,   426,   427,   525,   201,   429,   430,   239,   499,
     373,   344,   525,   395,   239,   211,   580,   237,   237,   237,
     395,   539,   426,   428,   429,   431,   201,   324,   131,   132,
     133,   134,   135,   136,   137,   138,   167,   179,   180,   181,
     131,   132,   133,   134,   135,   136,   137,   138,   150,   167,
     179,   180,   181,   212,   238,     7,    50,   356,   242,   211,
     242,   239,   525,   525,   150,   396,   335,   466,   345,   242,
     243,   473,   238,    43,   211,   214,   437,   249,   436,   395,
     219,   219,   219,   202,   211,   248,   249,   489,   551,   553,
     348,   238,   218,   238,   370,   218,   218,   218,   571,   373,
     492,   395,   575,   395,   363,   365,   397,   395,   367,   395,
     573,   373,   559,   562,   373,   218,   555,   492,   238,   238,
     387,   389,   395,   395,   395,   395,   395,   395,   207,   208,
     255,   238,    13,   237,   238,   150,   167,   212,   432,   580,
     211,   580,   239,   278,    70,   236,   239,   373,   545,   318,
       4,   378,   341,   319,    19,   190,   201,   479,    19,   190,
     201,   479,   396,   396,   396,   396,   396,   396,   201,   396,
     190,   201,   395,   190,   201,   190,   201,   395,   395,   396,
      19,   190,   201,   479,    19,   190,   201,   479,    19,   190,
     201,   479,   396,   396,   479,   396,   396,   396,   577,   582,
     396,   396,   396,   396,    22,   396,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   152,   153,   190,   201,
     235,   236,   392,   479,   395,   239,   373,   219,   219,   201,
     483,   219,   322,   219,   322,   219,   322,   214,   219,   489,
     214,   219,   489,   344,   580,   219,   489,   150,   395,   237,
     201,   484,   249,   285,   286,   285,   286,   395,   184,   201,
     433,   434,   478,   425,   425,   425,   396,   341,   201,   449,
     451,   419,   395,   201,   184,   201,   492,   436,   395,   249,
     496,   504,   506,   525,   492,   492,   525,    70,   373,   545,
     549,   201,   395,   525,   565,   567,   569,   492,   580,   219,
     489,   211,   580,   239,   492,   492,   492,   239,   492,   239,
     492,   580,   492,   427,   580,   557,   430,   219,   395,   395,
     492,   321,   239,   239,   239,   239,   239,   239,   395,   184,
     201,   238,   299,   238,   395,   395,   395,   239,   190,   201,
      13,   343,   575,   201,   239,   545,   543,   211,   239,   239,
     237,   238,   321,     1,    26,    28,    29,    38,    40,    44,
      52,    54,    58,    59,    65,   128,   243,   244,   249,   274,
     275,   284,   295,   296,   298,   300,   301,   302,   303,   304,
     305,   306,   307,   338,   346,   351,   352,   353,   354,   355,
     357,   361,   382,   396,   378,   218,   238,   218,   238,   238,
     238,   237,   238,   238,   238,   237,   237,   218,   238,   218,
     238,   218,   238,    19,   190,   201,   479,   214,   190,   201,
     395,   238,   238,   190,   201,   395,     1,   238,   237,   211,
     239,   508,   502,   211,   219,   242,   510,   219,   514,   219,
     518,   219,   525,   522,   426,   525,   524,   429,   219,   239,
     498,   525,   395,   212,   248,   450,   461,   249,   426,   530,
     429,   535,   239,   238,    43,   211,   214,   217,   432,   336,
     212,   450,   461,    40,   203,   244,   320,   420,   239,   238,
      43,   249,   436,   395,   249,   219,   219,   219,   545,   239,
     239,   239,   219,   489,   239,   219,   492,   427,   430,   219,
     239,   238,   492,   395,   239,   219,   219,   219,   219,   219,
     239,   219,   219,   239,   492,   219,   378,   239,   239,   219,
     322,   238,   214,   258,   238,    43,   201,   359,    20,   211,
     299,   239,   238,   167,   432,   238,   244,   580,   239,   211,
     237,   236,   543,   150,   167,   201,   212,   217,   376,   377,
     322,   150,   395,   334,    61,   395,   201,   201,   249,   192,
      58,   395,   278,   150,   395,   339,   249,   249,    10,    10,
      11,   282,    13,     9,    42,   249,   249,   249,   249,   249,
     249,    66,   358,   249,   131,   132,   133,   134,   135,   136,
     137,   138,   144,   145,   150,   159,   160,   161,   167,   170,
     171,   179,   180,   181,   212,   321,   399,   395,   403,   395,
     236,   239,   373,   543,   395,   395,   239,   373,   395,   401,
     395,   405,   395,   409,   395,   218,   238,   396,   238,   237,
     395,   236,   239,   373,   543,   238,   237,   393,   239,   373,
     201,   488,   201,   512,   516,   520,   500,   493,   395,   201,
     248,   540,   244,   244,   395,   201,   184,   201,   525,   395,
     244,   395,   449,   467,   201,     8,   413,   418,   395,   201,
     395,   249,   552,   554,   349,   239,   238,   201,   371,   219,
     219,   219,   572,   343,   219,   364,   366,   398,   368,   574,
     560,   563,   219,   556,   238,   278,   388,   219,   239,   373,
     259,   209,   395,   201,   211,   239,   373,   184,   201,   238,
      20,   167,   432,   395,   395,   395,   299,   239,   543,   239,
     201,   201,   238,   201,   201,   211,   239,   278,   395,    14,
     395,   212,   212,   214,   192,   334,   395,   341,   238,   238,
     236,   313,   314,   314,   238,   238,   243,   360,   413,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     577,   582,   396,   396,   577,   582,   396,   396,   396,   396,
     396,   396,   396,   396,   322,   492,   239,   525,   239,   543,
     211,   239,   239,   239,   239,   239,   239,   492,   239,   525,
     239,   525,   239,   407,   395,   395,   239,   543,   211,   239,
     239,   395,   239,   493,   212,   244,   239,   238,    43,   432,
      37,   331,   244,   212,    57,    63,   416,    67,   417,   249,
     239,   249,   238,   238,   395,   219,   566,   568,   570,   238,
     239,   238,   238,   238,   238,   238,   238,    70,   549,   238,
     558,   238,   239,   395,   334,   385,   390,   239,   257,   239,
     201,   239,   238,    43,   359,   373,   395,   395,   239,    20,
     237,   201,   376,   374,   334,   525,   395,   340,   395,   395,
     312,   238,   238,    56,   395,   359,   440,   441,   278,   219,
     219,   237,   543,   219,   219,   219,   525,   239,   239,   237,
     543,   239,   395,   395,   201,   395,   332,   525,    47,   417,
      46,   129,   414,   549,   549,   239,   238,   238,   238,   238,
     342,   343,   395,   395,   395,   395,   373,   549,   238,   549,
     239,   385,   205,   242,   395,   201,   239,   239,   167,   432,
     373,   239,   239,   244,   239,   239,   237,   299,   395,   267,
     239,   239,   244,   249,   441,   374,   400,   404,   239,   239,
     402,   406,   410,   219,   239,   239,   239,   243,   249,    33,
     415,   414,   416,   238,   543,   546,   547,   548,   548,   395,
     549,   549,   543,   544,   239,   239,   239,   239,   239,   239,
     580,   548,   549,   544,   395,   239,   242,   395,   395,   239,
     331,    12,   283,   278,    20,   239,   239,   278,   214,   437,
     408,   341,   421,   415,   433,   434,   435,   543,   211,   580,
     239,   239,   239,   548,   548,   239,   239,   239,   544,   239,
     242,   579,   395,   242,   284,   351,   352,   353,   354,   396,
     249,   297,   373,   278,   278,   334,   492,   436,   333,   328,
     422,   239,   238,   239,   239,   239,    53,   237,   579,   395,
     243,   287,   290,   239,   334,   334,   436,   395,   244,   249,
     328,   543,   395,   237,   579,   288,    12,    23,    24,   276,
     279,   284,   278,   395,   249,   278,   239,   244,   341,   278,
     238,   249,   334,   249,   374,   289,   280,   395,   244,   243,
     291,   294,   239,   331,   292,   284,   278,   341,   249,   281,
     293,   291,   244,   279,   331
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   246,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   248,   248,   249,
     249,   250,   251,   251,   251,   252,   252,   253,   253,   254,
     255,   255,   255,   255,   256,   256,   257,   257,   258,   259,
     258,   260,   260,   260,   261,   262,   262,   264,   263,   265,
     266,   267,   267,   267,   267,   267,   267,   267,   268,   268,
     269,   269,   269,   270,   270,   271,   272,   272,   273,   273,
     274,   275,   275,   276,   276,   277,   277,   278,   278,   279,
     280,   279,   281,   279,   282,   282,   283,   283,   284,   284,
     284,   284,   284,   285,   285,   286,   286,   288,   289,   287,
     290,   287,   292,   293,   291,   294,   291,   296,   297,   295,
     298,   299,   299,   299,   299,   299,   299,   299,   299,   301,
     300,   302,   304,   303,   305,   306,   306,   307,   307,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   309,   309,   310,   310,   310,   310,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   312,   312,   313,   313,   314,   314,   314,   315,
     315,   315,   315,   316,   316,   317,   317,   317,   317,   317,
     317,   317,   318,   318,   319,   319,   320,   320,   321,   321,
     321,   322,   322,   322,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   325,   325,   326,   327,   327,   327,   328,
     330,   329,   331,   332,   333,   331,   335,   336,   334,   337,
     337,   337,   337,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   339,   340,   338,   341,   341,   341,   342,   342,
     343,   343,   344,   344,   345,   345,   345,   346,   346,   348,
     349,   347,   347,   350,   350,   350,   350,   350,   350,   351,
     352,   353,   353,   353,   354,   354,   355,   356,   356,   356,
     357,   357,   358,   358,   359,   359,   360,   360,   361,   361,
     361,   363,   364,   362,   365,   366,   362,   367,   368,   362,
     370,   371,   369,   372,   372,   372,   373,   373,   373,   373,
     374,   374,   374,   375,   375,   375,   376,   376,   376,   376,
     376,   377,   377,   378,   378,   379,   380,   380,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   383,   383,   383,   384,   384,   384,   384,
     384,   385,   385,   386,   387,   388,   386,   389,   390,   386,
     391,   391,   391,   391,   391,   391,   391,   391,   391,   391,
     391,   391,   392,   393,   391,   394,   394,   394,   394,   394,
     394,   394,   395,   395,   395,   396,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     396,   397,   398,   396,   396,   396,   396,   396,   399,   400,
     396,   396,   396,   401,   402,   396,   396,   396,   396,   403,
     404,   396,   396,   396,   405,   406,   396,   396,   396,   407,
     408,   396,   396,   396,   409,   410,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     396,   396,   396,   411,   411,   411,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   413,   413,   414,
     414,   414,   415,   415,   416,   416,   416,   417,   417,   418,
     419,   419,   419,   420,   419,   421,   419,   422,   419,   423,
     424,   424,   425,   425,   425,   425,   425,   426,   426,   427,
     427,   428,   428,   428,   429,   429,   430,   430,   431,   431,
     431,   432,   432,   433,   433,   433,   434,   434,   435,   435,
     436,   436,   436,   437,   437,   438,   438,   438,   438,   438,
     438,   439,   439,   440,   440,   440,   441,   441,   441,   442,
     442,   442,   443,   443,   444,   444,   444,   445,   445,   446,
     445,   447,   448,   447,   449,   449,   450,   450,   451,   451,
     451,   452,   452,   452,   454,   453,   455,   455,   457,   456,
     458,   458,   458,   459,   460,   460,   461,   461,   462,   462,
     463,   463,   465,   466,   467,   464,   468,   468,   469,   469,
     470,   471,   471,   471,   471,   472,   472,   472,   473,   473,
     475,   476,   477,   474,   478,   478,   478,   478,   478,   478,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     480,   480,   480,   480,   480,   480,   480,   480,   481,   482,
     482,   482,   483,   483,   483,   484,   484,   484,   484,   484,
     485,   485,   485,   485,   485,   486,   487,   488,   486,   489,
     489,   490,   490,   491,   491,   491,   491,   492,   492,   493,
     493,   494,   494,   494,   494,   495,   496,   494,   494,   494,
     494,   497,   498,   494,   499,   500,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   501,
     502,   494,   494,   503,   504,   494,   505,   506,   494,   507,
     508,   494,   494,   509,   510,   494,   511,   512,   494,   494,
     513,   514,   494,   515,   516,   494,   494,   517,   518,   494,
     519,   520,   494,   521,   522,   494,   523,   524,   494,   525,
     525,   525,   527,   528,   529,   530,   526,   532,   533,   534,
     535,   531,   537,   538,   539,   540,   536,   541,   541,   541,
     541,   541,   541,   541,   542,   542,   542,   542,   542,   543,
     543,   543,   543,   543,   543,   543,   543,   544,   544,   545,
     546,   546,   547,   547,   548,   548,   549,   549,   551,   552,
     550,   553,   554,   550,   555,   556,   550,   557,   558,   550,
     559,   560,   550,   561,   562,   563,   561,   564,   565,   566,
     564,   567,   568,   564,   569,   570,   564,   564,   571,   572,
     564,   564,   573,   574,   564,   575,   575,   576,   577,   578,
     578,   578,   579,   579,   580,   580,   581,   581,   582
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     3,
       3,     2,     2,     2,     2,     2,     2,     1,     1,     1,
       1,     2,     0,     1,     1,     1,     1,     0,     2,     5,
       1,     1,     2,     2,     3,     2,     0,     2,     0,     0,
       3,     0,     2,     5,     3,     1,     2,     0,     4,     2,
       2,     1,     2,     3,     3,     3,     3,     3,     0,     2,
       3,     5,     5,     0,     1,     2,     1,     3,     1,     3,
       3,     3,     2,     1,     1,     1,     2,     0,     1,     0,
       0,     4,     0,     8,     1,     1,     0,     2,     1,     1,
       1,     1,     1,     1,     2,     0,     1,     0,     0,     6,
       0,     3,     0,     0,     6,     0,     3,     0,     0,     9,
       7,     1,     4,     3,     3,     3,     6,     5,     5,     0,
      10,     3,     0,     8,     0,     7,     8,     4,     4,     1,
       1,     1,     1,     2,     2,     1,     2,     1,     2,     1,
       1,     1,     3,     1,     1,     1,     1,     3,     3,     4,
       3,     3,     4,     4,     3,     4,     3,     4,     3,     3,
       1,     5,     1,     3,     3,     4,     0,     3,     1,     1,
       1,     1,     1,     1,     4,     1,     2,     3,     3,     3,
       3,     2,     1,     3,     0,     3,     0,     4,     0,     2,
       3,     0,     2,     2,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       3,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     3,     2,     2,     3,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     3,     2,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     4,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     4,     0,     1,     1,     3,
       0,     5,     0,     0,     0,     6,     0,     0,     6,     2,
       2,     2,     2,     1,     2,     2,     1,     1,     1,     1,
       2,     1,     2,     2,     2,     2,     1,     1,     1,     2,
       2,     2,     0,     0,     6,     0,     2,     2,     0,     2,
       0,     2,     1,     3,     1,     3,     2,     2,     3,     0,
       0,     5,     1,     2,     5,     5,     5,     6,     2,     1,
       1,     1,     2,     3,     2,     3,     4,     1,     1,     0,
       1,     1,     1,     0,     1,     3,     8,     7,     4,     3,
       5,     0,     0,     9,     0,     0,     9,     0,     0,     9,
       0,     0,     6,     5,     8,    10,     1,     2,     3,     4,
       1,     2,     3,     1,     1,     1,     2,     2,     2,     2,
       4,     1,     3,     0,     4,     7,     7,     3,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     6,     8,     6,     5,     6,     8,     8,
       6,     1,     4,     2,     0,     0,     7,     0,     0,     8,
       3,     4,     3,     5,     6,     5,     6,     8,     8,     6,
       5,     6,     0,     0,     5,     3,     4,     4,     5,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     4,     3,     4,     5,     4,
       4,     5,     4,     3,     4,     3,     1,     1,     2,     4,
       4,     0,     0,     9,     1,     3,     3,     5,     0,     0,
       8,     3,     3,     0,     0,     8,     3,     3,     3,     0,
       0,     8,     3,     3,     0,     0,     8,     3,     4,     0,
       0,     9,     4,     3,     0,     0,     8,     3,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     1,     4,
       3,     3,     3,     7,     8,     7,     4,     4,     4,     4,
       4,     1,     6,     7,     6,     6,     7,     6,     7,     6,
       6,     6,     7,     6,     6,     6,     5,     0,     1,     0,
       1,     1,     0,     1,     0,     1,     1,     0,     1,     5,
       0,     2,     6,     0,     4,     0,     9,     0,    11,     3,
       3,     4,     1,     1,     3,     3,     3,     1,     3,     1,
       3,     0,     1,     3,     3,     1,     1,     3,     0,     1,
       3,     1,     1,     1,     2,     3,     3,     5,     1,     1,
       1,     1,     1,     0,     1,     1,     4,     3,     3,     6,
       5,     1,     3,     0,     2,     2,     4,     6,     5,     4,
       6,     5,     0,     1,     0,     1,     1,     0,     2,     0,
       4,     6,     0,     6,     1,     3,     1,     2,     0,     1,
       3,     0,     1,     1,     0,     5,     3,     3,     0,     6,
       0,     1,     1,     1,     0,     2,     0,     1,     1,     2,
       0,     1,     0,     0,     0,    13,     0,     2,     0,     1,
       3,     1,     1,     2,     2,     0,     1,     1,     1,     3,
       0,     0,     0,     9,     1,     4,     3,     3,     6,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     1,     3,     3,     0,     1,     3,     3,     5,
       0,     2,     2,     2,     2,     4,     0,     0,     7,     1,
       1,     1,     3,     3,     2,     4,     3,     1,     2,     0,
       4,     1,     1,     1,     1,     0,     0,     6,     4,     4,
       3,     0,     0,     7,     0,     0,     8,     4,     2,     2,
       3,     2,     3,     2,     2,     3,     3,     3,     2,     0,
       0,     6,     2,     0,     0,     6,     0,     0,     6,     0,
       0,     6,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     0,     0,     6,     0,     0,     6,     1,
       3,     3,     0,     0,     0,     0,    12,     0,     0,     0,
       0,    12,     0,     0,     0,     0,    13,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     5,     5,     6,     6,     8,     8,     0,     1,     2,
       3,     5,     1,     2,     1,     0,     0,     1,     0,     0,
      10,     0,     0,    10,     0,     0,    10,     0,     0,    11,
       0,     0,     7,     5,     0,     0,    10,     3,     0,     0,
      11,     0,     0,    11,     0,     0,    10,     5,     0,     0,
       9,     5,     0,     0,    10,     1,     3,     0,     5,     5,
       7,     9,     0,     3,     0,     1,    11,    12,    13
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = DAS2_YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == DAS2_YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use DAS2_YYerror or DAS2_YYUNDEF. */
#define YYERRCODE DAS2_YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if DAS2_YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined DAS2_YYLTYPE_IS_TRIVIAL && DAS2_YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (scanner);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, yyscan_t scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !DAS2_YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !DAS2_YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, yyscan_t scanner)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
    case YYSYMBOL_NAME: /* "name"  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_module_name: /* module_name  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_character_sequence: /* character_sequence  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_string_constant: /* string_constant  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_format_string: /* format_string  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_optional_format_string: /* optional_format_string  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_string_builder_body: /* string_builder_body  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_string_builder: /* string_builder  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_reader: /* expr_reader  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_require_module_name: /* require_module_name  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_optional_require_guard: /* optional_require_guard  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_expression_label: /* expression_label  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_goto: /* expression_goto  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_else: /* expression_else  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_else_one_liner: /* expression_else_one_liner  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_if_one_liner: /* expression_if_one_liner  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_if_block: /* expression_if_block  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_else_block: /* expression_else_block  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_if_then_else: /* expression_if_then_else  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_if_then_else_oneliner: /* expression_if_then_else_oneliner  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_for_variable_name_with_pos_list: /* for_variable_name_with_pos_list  */
            { delete ((*yyvaluep).pNameWithPosList); }
        break;

    case YYSYMBOL_expression_for_loop: /* expression_for_loop  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_unsafe: /* expression_unsafe  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_while_loop: /* expression_while_loop  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_with: /* expression_with  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_with_alias: /* expression_with_alias  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_annotation_argument_value: /* annotation_argument_value  */
            { delete ((*yyvaluep).aa); }
        break;

    case YYSYMBOL_annotation_argument_value_list: /* annotation_argument_value_list  */
            { delete ((*yyvaluep).aaList); }
        break;

    case YYSYMBOL_annotation_argument_name: /* annotation_argument_name  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_annotation_argument: /* annotation_argument  */
            { delete ((*yyvaluep).aa); }
        break;

    case YYSYMBOL_annotation_argument_list: /* annotation_argument_list  */
            { delete ((*yyvaluep).aaList); }
        break;

    case YYSYMBOL_metadata_argument_list: /* metadata_argument_list  */
            { delete ((*yyvaluep).aaList); }
        break;

    case YYSYMBOL_optional_for_annotations: /* optional_for_annotations  */
            { delete ((*yyvaluep).aaList); }
        break;

    case YYSYMBOL_annotation_declaration_name: /* annotation_declaration_name  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_annotation_declaration_basic: /* annotation_declaration_basic  */
            { /* gc owns AnnotationDeclaration */ }
        break;

    case YYSYMBOL_annotation_declaration: /* annotation_declaration  */
            { /* gc owns AnnotationDeclaration */ }
        break;

    case YYSYMBOL_annotation_list: /* annotation_list  */
            { delete ((*yyvaluep).faList); }
        break;

    case YYSYMBOL_optional_annotation_list: /* optional_annotation_list  */
            { delete ((*yyvaluep).faList); }
        break;

    case YYSYMBOL_optional_annotation_list_with_emit_semis: /* optional_annotation_list_with_emit_semis  */
            { delete ((*yyvaluep).faList); }
        break;

    case YYSYMBOL_optional_function_argument_list: /* optional_function_argument_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
        break;

    case YYSYMBOL_optional_function_type: /* optional_function_type  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_function_name: /* function_name  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_das_type_name: /* das_type_name  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_function_declaration_header: /* function_declaration_header  */
            { ((*yyvaluep).pFuncDecl)->delRef(); }
        break;

    case YYSYMBOL_function_declaration: /* function_declaration  */
            { ((*yyvaluep).pFuncDecl)->delRef(); }
        break;

    case YYSYMBOL_expression_block_finally: /* expression_block_finally  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_block: /* expression_block  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_call_pipe_no_bracket: /* expr_call_pipe_no_bracket  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_any: /* expression_any  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expressions: /* expressions  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_optional_expr_list: /* optional_expr_list  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_optional_expr_map_tuple_list: /* optional_expr_map_tuple_list  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_type_declaration_no_options_list: /* type_declaration_no_options_list  */
            { deleteTypeDeclarationList(((*yyvaluep).pTypeDeclList)); }
        break;

    case YYSYMBOL_name_in_namespace: /* name_in_namespace  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_expression_delete: /* expression_delete  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_new_type_declaration: /* new_type_declaration  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_expr_new: /* expr_new  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_break: /* expression_break  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_continue: /* expression_continue  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_return: /* expression_return  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_yield: /* expression_yield  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_try_catch: /* expression_try_catch  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_tuple_expansion: /* tuple_expansion  */
            { delete ((*yyvaluep).pNameList); }
        break;

    case YYSYMBOL_tuple_expansion_variable_declaration: /* tuple_expansion_variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_expression_let: /* expression_let  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_cast: /* expr_cast  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_type_decl: /* expr_type_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_type_info: /* expr_type_info  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_list: /* expr_list  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_block_or_simple_block: /* block_or_simple_block  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_capture_entry: /* capture_entry  */
            { delete ((*yyvaluep).pCapt); }
        break;

    case YYSYMBOL_capture_list: /* capture_list  */
            { delete ((*yyvaluep).pCaptList); }
        break;

    case YYSYMBOL_optional_capture_list: /* optional_capture_list  */
            { delete ((*yyvaluep).pCaptList); }
        break;

    case YYSYMBOL_expr_full_block: /* expr_full_block  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_full_block_assumed_piped: /* expr_full_block_assumed_piped  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_numeric_const: /* expr_numeric_const  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_assign_no_bracket: /* expr_assign_no_bracket  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_named_call: /* expr_named_call  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_method_call_no_bracket: /* expr_method_call_no_bracket  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_func_addr_name: /* func_addr_name  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_func_addr_expr: /* func_addr_expr  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_field_no_bracket: /* expr_field_no_bracket  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_call: /* expr_call  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr: /* expr  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_no_bracket: /* expr_no_bracket  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_generator: /* expr_generator  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_mtag_no_bracket: /* expr_mtag_no_bracket  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_optional_field_annotation: /* optional_field_annotation  */
            { delete ((*yyvaluep).aaList); }
        break;

    case YYSYMBOL_structure_variable_declaration: /* structure_variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_struct_variable_declaration_list: /* struct_variable_declaration_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
        break;

    case YYSYMBOL_function_argument_declaration_no_type: /* function_argument_declaration_no_type  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_function_argument_declaration_type: /* function_argument_declaration_type  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_function_argument_list: /* function_argument_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
        break;

    case YYSYMBOL_tuple_type: /* tuple_type  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_tuple_type_list: /* tuple_type_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
        break;

    case YYSYMBOL_tuple_alias_type_list: /* tuple_alias_type_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
        break;

    case YYSYMBOL_variant_type: /* variant_type  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_variant_type_list: /* variant_type_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
        break;

    case YYSYMBOL_variant_alias_type_list: /* variant_alias_type_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
        break;

    case YYSYMBOL_variable_declaration_no_type: /* variable_declaration_no_type  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_variable_declaration_type: /* variable_declaration_type  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_variable_declaration: /* variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_let_variable_name_with_pos_list: /* let_variable_name_with_pos_list  */
            { delete ((*yyvaluep).pNameWithPosList); }
        break;

    case YYSYMBOL_global_let_variable_name_with_pos_list: /* global_let_variable_name_with_pos_list  */
            { delete ((*yyvaluep).pNameWithPosList); }
        break;

    case YYSYMBOL_variable_declaration_list: /* variable_declaration_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
        break;

    case YYSYMBOL_let_variable_declaration: /* let_variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_global_let_variable_declaration: /* global_let_variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_global_variable_declaration_list: /* global_variable_declaration_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
        break;

    case YYSYMBOL_enum_expression: /* enum_expression  */
            { delete ((*yyvaluep).pEnumPair); }
        break;

    case YYSYMBOL_enum_list: /* enum_list  */
            { /* gc owns Enumeration */ }
        break;

    case YYSYMBOL_enum_name: /* enum_name  */
            { /* $$->delRef(); // if enum rule returns, module already has the link */ }
        break;

    case YYSYMBOL_optional_structure_parent: /* optional_structure_parent  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_optional_struct_variable_declaration_list: /* optional_struct_variable_declaration_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
        break;

    case YYSYMBOL_variable_name_with_pos_list: /* variable_name_with_pos_list  */
            { delete ((*yyvaluep).pNameWithPosList); }
        break;

    case YYSYMBOL_structure_type_declaration: /* structure_type_declaration  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_auto_type_declaration: /* auto_type_declaration  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_bitfield_bits: /* bitfield_bits  */
            { delete ((*yyvaluep).pNameList); }
        break;

    case YYSYMBOL_bitfield_alias_bits: /* bitfield_alias_bits  */
            { deleteNameExprList(((*yyvaluep).pNameExprList)); }
        break;

    case YYSYMBOL_bitfield_type_declaration: /* bitfield_type_declaration  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_table_type_pair: /* table_type_pair  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_dim_list: /* dim_list  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_type_declaration_no_options: /* type_declaration_no_options  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_optional_expr_list_in_braces: /* optional_expr_list_in_braces  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_type_declaration_no_options_no_dim: /* type_declaration_no_options_no_dim  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_type_declaration: /* type_declaration  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_make_decl: /* make_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_decl_no_bracket: /* make_decl_no_bracket  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_fields: /* make_struct_fields  */
            { /* gc owns MakeStruct */ }
        break;

    case YYSYMBOL_make_variant_dim: /* make_variant_dim  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_single: /* make_struct_single  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_dim_list: /* make_struct_dim_list  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_dim_decl: /* make_struct_dim_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_optional_make_struct_dim_decl: /* optional_make_struct_dim_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_decl: /* make_struct_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_tuple_call: /* make_tuple_call  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_dim_decl: /* make_dim_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_map_tuple_list: /* expr_map_tuple_list  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_table_decl: /* make_table_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_table_call: /* make_table_call  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_array_comprehension_where: /* array_comprehension_where  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_table_comprehension: /* table_comprehension  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_array_comprehension: /* array_comprehension  */
            { /* gc_node; */ }
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (yyscan_t scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined DAS2_YYLTYPE_IS_TRIVIAL && DAS2_YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = DAS2_YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == DAS2_YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= DAS2_YYEOF)
    {
      yychar = DAS2_YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == DAS2_YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = DAS2_YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = DAS2_YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 3: /* program: program module_declaration  */
                                   {
            if ( yyextra->das_has_type_declarations ) {
                das2_yyerror(scanner,"module name has to be first declaration",tokAt(scanner,(yylsp[0])), CompilationError::invalid_module);
            }
        }
    break;

  case 4: /* program: program structure_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 5: /* program: program enum_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 6: /* program: program global_let  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 7: /* program: program global_function_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 11: /* program: program alias_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 12: /* program: program variant_alias_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 13: /* program: program tuple_alias_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 14: /* program: program bitfield_alias_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 21: /* top_level_reader_macro: expr_reader SEMICOLON  */
                                   {
        (void)(yyvsp[-1].pExpression); // gc_node — Expression, don't delete
    }
    break;

  case 22: /* optional_public_or_private_module: %empty  */
                        { (yyval.b) = yyextra->g_Program->policies.default_module_public; }
    break;

  case 23: /* optional_public_or_private_module: "public"  */
                        { (yyval.b) = true; }
    break;

  case 24: /* optional_public_or_private_module: "private"  */
                        { (yyval.b) = false; }
    break;

  case 25: /* module_name: '$'  */
                    { (yyval.s) = new string("$"); }
    break;

  case 26: /* module_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 27: /* optional_not_required: %empty  */
        { (yyval.b) = false; }
    break;

  case 28: /* optional_not_required: '!' "inscope"  */
                        { (yyval.b) = true; }
    break;

  case 29: /* module_declaration: "module" module_name optional_shared optional_public_or_private_module optional_not_required  */
                                                                                                                                    {
        yyextra->g_Program->thisModuleName = *(yyvsp[-3].s);
        yyextra->g_Program->thisModule->isPublic = (yyvsp[-1].b);
        yyextra->g_Program->thisModule->isModule = true;
        yyextra->g_Program->thisModule->visibleEverywhere = (yyvsp[0].b);
        if ( yyextra->g_Program->thisModule->name.empty() ) {
            yyextra->g_Program->library.renameModule(yyextra->g_Program->thisModule.get(),*(yyvsp[-3].s));
        } else if ( yyextra->g_Program->thisModule->name != *(yyvsp[-3].s) ){
            das2_yyerror(scanner,"this module already has a name " + yyextra->g_Program->thisModule->name,tokAt(scanner,(yylsp[-3])),
                CompilationError::already_declared_module_name);
        }
        if ( !yyextra->g_Program->policies.ignore_shared_modules ) {
            yyextra->g_Program->promoteToBuiltin = (yyvsp[-2].b);
        }
        delete (yyvsp[-3].s);
    }
    break;

  case 30: /* character_sequence: STRING_CHARACTER  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
    break;

  case 31: /* character_sequence: STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
    break;

  case 32: /* character_sequence: character_sequence STRING_CHARACTER  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
    break;

  case 33: /* character_sequence: character_sequence STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
    break;

  case 34: /* string_constant: "start of the string" character_sequence "end of the string"  */
                                                           { (yyval.s) = (yyvsp[-1].s); }
    break;

  case 35: /* string_constant: "start of the string" "end of the string"  */
                                                           { (yyval.s) = new string(); }
    break;

  case 36: /* format_string: %empty  */
        { (yyval.s) = new string(); }
    break;

  case 37: /* format_string: format_string STRING_CHARACTER  */
                                                 { (yyval.s) = (yyvsp[-1].s); (yyvsp[-1].s)->push_back((yyvsp[0].ch)); }
    break;

  case 38: /* optional_format_string: %empty  */
        { (yyval.s) = new string(""); }
    break;

  case 39: /* $@1: %empty  */
            { das2_strfmt(scanner); }
    break;

  case 40: /* optional_format_string: ':' $@1 format_string  */
                                                         { (yyval.s) = (yyvsp[0].s); }
    break;

  case 41: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder(tokAt(scanner,(yyloc)));
    }
    break;

  case 42: /* string_builder_body: string_builder_body character_sequence  */
                                                                                  {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das2_yyerror(scanner,"invalid escape sequence",tokAt(scanner,(yylsp[-1])), CompilationError::invalid_escape);
        auto sc = new ExprConstString(tokAt(scanner,(yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 43: /* string_builder_body: string_builder_body "{" expr optional_format_string "}"  */
                                                                                                                                     {
        auto se = (yyvsp[-2].pExpression);
        if ( !(yyvsp[-1].s)->empty() ) {
            auto call_fmt = new ExprCall(tokRangeAt(scanner,(yylsp[-2]),(yylsp[-1])), "_::fmt");
            call_fmt->arguments.push_back(new ExprConstString(tokAt(scanner,(yylsp[-1])),":" + *(yyvsp[-1].s)));
            call_fmt->arguments.push_back(se);
            se = call_fmt;
        }
        static_cast<ExprStringBuilder *>((yyvsp[-4].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-4].pExpression);
        delete (yyvsp[-1].s);
    }
    break;

  case 44: /* string_builder: "start of the string" string_builder_body "end of the string"  */
                                                                   {
        auto strb = static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression));
        if ( strb->elements.size()==0 ) {
            (yyval.pExpression) = new ExprConstString(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),"");
            // gc_node — don't delete $sb
        } else if ( strb->elements.size()==1 && strb->elements[0]->rtti_isStringConstant() ) {
            auto sconst = static_cast<ExprConstString*>(strb->elements[0]);
            (yyval.pExpression) = new ExprConstString(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),sconst->text);
            // gc_node — don't delete $sb
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
            (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
        }
    }
    break;

  case 45: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 46: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 47: /* $@2: %empty  */
                                        {
        auto macros = yyextra->g_Program->getReaderMacro(*(yyvsp[0].s));
        if ( macros.size()==0 ) {
            das2_yyerror(scanner,"reader macro " + *(yyvsp[0].s) + " not found",tokAt(scanner,(yylsp[0])),
                CompilationError::lookup_macro);
            if ( yychar == '~' ) {
                yyextra->g_ReaderMacro = das_unknown_reader_macro();
                yyextra->g_ReaderExpr = new ExprReader(tokAt(scanner,(yylsp[-1])),yyextra->g_ReaderMacro);
                yyclearin ;
                das2_yybegin_reader(scanner);
            }
        } else if ( macros.size()>1 ) {
            string options;
            for ( auto & x : macros ) {
                options += "\t" + x->module->name + "::" + x->name + "\n";
            }
            das2_yyerror(scanner,"too many options for the reader macro " + *(yyvsp[0].s) +  "\n" + options, tokAt(scanner,(yylsp[0])),
                CompilationError::ambiguous_macro);
        } else if ( yychar != '~' ) {
            das2_yyerror(scanner,"expecting ~ after the reader macro", tokAt(scanner,(yylsp[0])),
                CompilationError::invalid_macro);
        } else {
            yyextra->g_ReaderMacro = macros.back();
            yyextra->g_ReaderExpr = new ExprReader(tokAt(scanner,(yylsp[-1])),yyextra->g_ReaderMacro);
            yyclearin ;
            das2_yybegin_reader(scanner);
        }
    }
    break;

  case 48: /* expr_reader: '%' name_in_namespace $@2 reader_character_sequence  */
                                     {
        yyextra->g_ReaderExpr->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = yyextra->g_ReaderExpr;
        int thisLine = 0;
        FileInfo * info = nullptr;
        if ( auto seqt = yyextra->g_ReaderMacro->suffix(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, thisLine, info, tokAt(scanner,(yylsp[0]))) ) {
            das2_accept_sequence(scanner,seqt,strlen(seqt),thisLine,info,yyextra->g_ReaderExpr->at);
            yylloc.first_column = (yylsp[0]).first_column;
            yylloc.first_line = (yylsp[0]).first_line;
            yylloc.last_column = (yylsp[0]).last_column;
            yylloc.last_line = (yylsp[0]).last_line;
        }
        delete (yyvsp[-2].s);
        yyextra->g_ReaderMacro = nullptr;
        yyextra->g_ReaderExpr = nullptr;
    }
    break;

  case 49: /* options_declaration: "options" annotation_argument_list  */
                                                   {
        for ( auto & opt : *(yyvsp[0].aaList) ) {
            if ( yyextra->g_Access->isOptionAllowed(opt.name, yyextra->g_Program->thisModule->fileName) ) {
                if ( yyextra->g_Access->isOptionBlocked(opt.name, yyextra->g_Program->thisModule->fileName) ) {
                    // blocked: ok to write, silently ignored (not applied)
                } else {
                    yyextra->g_Program->options.push_back(opt);
                }
            } else {
                das2_yyerror(scanner,"option " + opt.name + " is not allowed here",
                    tokAt(scanner,(yylsp[0])), CompilationError::invalid_options);
            }
        }
        delete (yyvsp[0].aaList);
    }
    break;

  case 51: /* require_module_name: "name"  */
                   {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 52: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 53: /* require_module_name: '.' '/' require_module_name  */
                                         {
        *(yyvsp[0].s) = "./" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 54: /* require_module_name: ".." '/' require_module_name  */
                                            {
        *(yyvsp[0].s) = "../" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 55: /* require_module_name: '%' '/' require_module_name  */
                                         {
        *(yyvsp[0].s) = "%/" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 56: /* require_module_name: require_module_name '.' "name"  */
                                                {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 57: /* require_module_name: require_module_name '/' "name"  */
                                                {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 58: /* optional_require_guard: %empty  */
                                            { (yyval.s) = nullptr; }
    break;

  case 59: /* optional_require_guard: '?' require_module_name  */
                                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 60: /* require_module: optional_require_guard require_module_name is_public_module  */
                                                                                       {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])),(yyvsp[-2].s));
    }
    break;

  case 61: /* require_module: optional_require_guard require_module_name "as" "name" is_public_module  */
                                                                                                            {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])),(yyvsp[-4].s));
    }
    break;

  case 62: /* require_module: optional_require_guard '[' "name" ']' is_public_module  */
                                                                                {
        ast_requireModuleGroup(scanner,(yyvsp[-2].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-2])),(yyvsp[-4].s));
    }
    break;

  case 63: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 64: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 68: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 69: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 70: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 71: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 72: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yyloc)),(yyvsp[0].pExpression));
    }
    break;

  case 73: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 74: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 79: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 80: /* $@3: %empty  */
                                           {
    }
    break;

  case 81: /* expression_else: "else" optional_emit_semis $@3 expression_else_block  */
                                   {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 82: /* $@4: %empty  */
                                                                        {
    }
    break;

  case 83: /* expression_else: elif_or_static_elif '(' expr ')' optional_emit_semis $@4 expression_else_block expression_else  */
                                                         {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yyloc)),(yyvsp[-5].pExpression),(yyvsp[-1].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-7].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 84: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 85: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 86: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 87: /* expression_else_one_liner: "else" expression_if_one_liner  */
                                                      {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 88: /* expression_if_one_liner: expr_no_bracket  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 89: /* expression_if_one_liner: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 90: /* expression_if_one_liner: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 91: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 92: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 97: /* $@5: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 98: /* $@6: %empty  */
                         {
        yyextra->pop_nesteds();
    }
    break;

  case 99: /* expression_if_block: '{' $@5 expressions $@6 '}' expression_block_finally  */
                                        {
        (yyval.pExpression) = (yyvsp[-3].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-5]),(yylsp[0]));
        if ( (yyvsp[0].pExpression) ) {
            auto pF = (ExprBlock *) (yyvsp[0].pExpression);
            auto pB = (ExprBlock *) (yyval.pExpression);
            swap ( pB->finalList, pF->list );
            // gc_node — don't delete Expression
        }
    }
    break;

  case 100: /* $@7: %empty  */
       {
        yyextra->das_keyword = false;
    }
    break;

  case 101: /* expression_if_block: $@7 expression_if_one_liner SEMICOLON  */
                                               {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyloc) = (yylsp[-1]);
    }
    break;

  case 102: /* $@8: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 103: /* $@9: %empty  */
                         {
        yyextra->pop_nesteds();
    }
    break;

  case 104: /* expression_else_block: '{' $@8 expressions $@9 '}' expression_block_finally  */
                                        {
        (yyval.pExpression) = (yyvsp[-3].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-5]),(yylsp[0]));
        if ( (yyvsp[0].pExpression) ) {
            auto pF = (ExprBlock *) (yyvsp[0].pExpression);
            auto pB = (ExprBlock *) (yyval.pExpression);
            swap ( pB->finalList, pF->list );
            // gc_node — don't delete Expression
        }
    }
    break;

  case 105: /* $@10: %empty  */
       {
        yyextra->das_keyword = false;
    }
    break;

  case 106: /* expression_else_block: $@10 expression_if_one_liner SEMICOLON  */
                                               {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyloc) = (yylsp[-1]);
    }
    break;

  case 107: /* $@11: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 108: /* $@12: %empty  */
                                                                  {
    }
    break;

  case 109: /* expression_if_then_else: $@11 if_or_static_if '(' expr ')' optional_emit_semis $@12 expression_if_block expression_else  */
                                                       {
        yyextra->das_keyword = false;
        auto blk = (yyvsp[-1].pExpression)->rtti_isBlock() ? static_cast<ExprBlock *>((yyvsp[-1].pExpression)) : ast_wrapInBlock((yyvsp[-1].pExpression));
        auto eite = new ExprIfThenElse(tokAt(scanner,(yyloc)),(yyvsp[-5].pExpression),blk,(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-7].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 110: /* expression_if_then_else_oneliner: expression_if_one_liner "if" '(' expr ')' expression_else_one_liner SEMICOLON  */
                                                                                                                      {
        (yyval.pExpression) = new ExprIfThenElse(tokRangeAt(scanner,(yylsp[-6]),(yylsp[-1])),(yyvsp[-3].pExpression),ast_wrapInBlock((yyvsp[-6].pExpression)),(yyvsp[-1].pExpression) ? ast_wrapInBlock((yyvsp[-1].pExpression)) : nullptr);
    }
    break;

  case 111: /* for_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 112: /* for_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 113: /* for_variable_name_with_pos_list: "name" "aka" "name"  */
                                         {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 114: /* for_variable_name_with_pos_list: '(' tuple_expansion ')'  */
                                       {
        auto pSL = new vector<VariableNameAndPosition>();
        for ( auto & x : *(yyvsp[-1].pNameList) ) {
            das_checkName(scanner,x,tokAt(scanner,(yylsp[-1])));
        }
        pSL->push_back(VariableNameAndPosition((yyvsp[-1].pNameList),tokAt(scanner,(yylsp[-1]))));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 115: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 116: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                               {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 117: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 118: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' '(' tuple_expansion ')'  */
                                                                                 {
        for ( auto & x : *(yyvsp[-1].pNameList) ) {
            das_checkName(scanner,x,tokAt(scanner,(yylsp[-1])));
        }
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition((yyvsp[-1].pNameList),tokAt(scanner,(yylsp[-1]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
    }
    break;

  case 119: /* $@13: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 120: /* expression_for_loop: $@13 "for" optional_for_annotations '(' for_variable_name_with_pos_list "in" expr_list ')' optional_emit_semis expression_block  */
                                                                                                                                                                    {
        yyextra->das_keyword = false;
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-5].pNameWithPosList),(yyvsp[-3].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yyloc)),tokAt(scanner,(yylsp[0])),(yyvsp[-7].aaList));
    }
    break;

  case 121: /* expression_unsafe: "unsafe" optional_emit_semis expression_block  */
                                                                    {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yyloc)));
        pUnsafe->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 122: /* $@14: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 123: /* expression_while_loop: $@14 "while" optional_for_annotations '(' expr ')' optional_emit_semis expression_block  */
                                                                                                                        {
        yyextra->das_keyword = false;
        auto pWhile = new ExprWhile(tokAt(scanner,(yyloc)));
        pWhile->cond = (yyvsp[-3].pExpression);
        pWhile->body = (yyvsp[0].pExpression);
        if ( (yyvsp[-5].aaList) ) { pWhile->annotations = move(*(yyvsp[-5].aaList)); delete (yyvsp[-5].aaList); }
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 124: /* with_keyword_on: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 125: /* expression_with: with_keyword_on "with" '(' expr ')' optional_emit_semis expression_block  */
                                                                                                     {
        yyextra->das_keyword = false;
        auto pWith = new ExprWith(tokAt(scanner,(yyloc)));
        pWith->with = (yyvsp[-3].pExpression);
        pWith->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pWith;
    }
    break;

  case 126: /* expression_with: with_keyword_on "with" '(' "module" require_module_name ')' optional_emit_semis expression_block  */
                                                                                                                               {
        yyextra->das_keyword = false;
        auto pWith = new ExprWith(tokAt(scanner,(yyloc)));
        { // single-name targets honor `require foo as bar` aliases
            auto ita = yyextra->das_module_alias.find(*(yyvsp[-3].s));
            if ( ita != yyextra->das_module_alias.end() ) *(yyvsp[-3].s) = ita->second;
        }
        pWith->moduleName = *(yyvsp[-3].s);
        delete (yyvsp[-3].s);
        if ( yyextra->g_Access ) { // .das_project may demand unsafe for this target module
            auto fi = pWith->at.fileInfo;
            pWith->moduleUnsafeByProject = yyextra->g_Access->isWithModuleUnsafe(pWith->moduleName, fi ? fi->name : "");
        }
        pWith->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pWith;
    }
    break;

  case 127: /* expression_with_alias: "assume" "name" '=' expr  */
                                                      {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yyloc)), *(yyvsp[-2].s), ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-2].s);
    }
    break;

  case 128: /* expression_with_alias: "typedef" "name" '=' type_declaration  */
                                                                {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yyloc)), *(yyvsp[-2].s), TypeDeclPtr((yyvsp[0].pTypeDecl)));
        delete (yyvsp[-2].s);
    }
    break;

  case 129: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 130: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 131: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 132: /* annotation_argument_value: "long integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",int64_t((yyvsp[0].i64))); }
    break;

  case 133: /* annotation_argument_value: '-' "long integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",int64_t(0ull - uint64_t((yyvsp[0].i64)))); }
    break;

  case 134: /* annotation_argument_value: '-' "int64 minimum magnitude (requires minus)"  */
                                 { (yyval.aa) = new AnnotationArgument("",int64_t(INT64_MIN)); }
    break;

  case 135: /* annotation_argument_value: "unsigned long integer constant"  */
                                     { (yyval.aa) = new AnnotationArgument("",uint64_t((yyvsp[0].ui64))); }
    break;

  case 136: /* annotation_argument_value: '-' "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",int32_t(0u - uint32_t((yyvsp[0].i)))); }
    break;

  case 137: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 138: /* annotation_argument_value: '-' "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",-float((yyvsp[0].fd))); }
    break;

  case 139: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 140: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 141: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 142: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 143: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 144: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 145: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 146: /* annotation_argument_name: "default"  */
                    { (yyval.s) = new string("default"); }
    break;

  case 147: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 148: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 149: /* annotation_argument: annotation_argument_name '=' "@@" "name"  */
                                                                      { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[0].s); delete (yyvsp[-3].s); }
    break;

  case 150: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 151: /* annotation_argument: annotation_argument_name '=' "long integer constant"  */
                                                               { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),int64_t((yyvsp[0].i64)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 152: /* annotation_argument: annotation_argument_name '=' '-' "long integer constant"  */
                                                                   { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),int64_t(0ull - uint64_t((yyvsp[0].i64))),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[-3].s); }
    break;

  case 153: /* annotation_argument: annotation_argument_name '=' '-' "int64 minimum magnitude (requires minus)"  */
                                                                { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),int64_t(INT64_MIN),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[-3].s); }
    break;

  case 154: /* annotation_argument: annotation_argument_name '=' "unsigned long integer constant"  */
                                                                        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),uint64_t((yyvsp[0].ui64)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 155: /* annotation_argument: annotation_argument_name '=' '-' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),int32_t(0u - uint32_t((yyvsp[0].i))),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[-3].s); }
    break;

  case 156: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 157: /* annotation_argument: annotation_argument_name '=' '-' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),-float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[-3].s); }
    break;

  case 158: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 159: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 160: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 161: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 162: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 163: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 164: /* metadata_argument_list: "@field" annotation_argument optional_emit_semis  */
                                                              {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[-1].aa));
    }
    break;

  case 165: /* metadata_argument_list: metadata_argument_list "@field" annotation_argument optional_emit_semis  */
                                                                                           {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-3].aaList),(yyvsp[-1].aa));
    }
    break;

  case 166: /* optional_for_annotations: %empty  */
                    {
        (yyval.aaList) = nullptr;
    }
    break;

  case 167: /* optional_for_annotations: '[' annotation_argument_list ']'  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 168: /* optional_for_annotations: metadata_argument_list  */
                                     {
        (yyval.aaList) = (yyvsp[0].aaList);
    }
    break;

  case 169: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 170: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 171: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 172: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 173: /* annotation_declaration_basic: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( yyextra->g_Access->isAnnotationAllowed(*(yyvsp[0].s), yyextra->g_Program->thisModuleName) ) {
            if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
                (yyval.fa)->annotation = ann;
            } else {
                (yyval.fa)->annotation = new Annotation(*(yyvsp[0].s));
                das2_yyerror(scanner,"annotation " + *(yyvsp[0].s) + " is not found",
                            tokAt(scanner,(yylsp[0])), CompilationError::lookup_annotation);
            }
        } else {
            (yyval.fa)->annotation = new Annotation(*(yyvsp[0].s));
            das2_yyerror(scanner,"annotation " + *(yyvsp[0].s) + " is not allowed here",
                        tokAt(scanner,(yylsp[0])), CompilationError::invalid_annotation);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 174: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
                                                                                 {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[-3]));
        if ( yyextra->g_Access->isAnnotationAllowed(*(yyvsp[-3].s), yyextra->g_Program->thisModuleName) ) {
            if ( auto ann = findAnnotation(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3]))) ) {
                (yyval.fa)->annotation = ann;
            } else {
                (yyval.fa)->annotation = new Annotation(*(yyvsp[-3].s));
                das2_yyerror(scanner,"annotation " + *(yyvsp[-3].s) + " is not found",
                            tokAt(scanner,(yylsp[-3])), CompilationError::lookup_annotation);
            }
        } else {
            (yyval.fa)->annotation = new Annotation(*(yyvsp[-3].s));
            das2_yyerror(scanner,"annotation " + *(yyvsp[-3].s) + " is not allowed here",
                        tokAt(scanner,(yylsp[-3])), CompilationError::invalid_annotation);
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
    break;

  case 175: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 176: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            (yyvsp[0].fa) = nullptr; // gc_node — don't delete AnnotationDeclaration
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 177: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation);
            (yyvsp[-2].fa) = nullptr; // gc_node — don't delete AnnotationDeclaration
        }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            (yyvsp[0].fa) = nullptr; // gc_node — don't delete AnnotationDeclaration
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::And,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 178: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
                                                                            {
        if ( !(yyvsp[-2].fa)->annotation || !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation);
            (yyvsp[-2].fa) = nullptr; // gc_node — don't delete AnnotationDeclaration
        }
        if ( !(yyvsp[0].fa)->annotation || !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            (yyvsp[0].fa) = nullptr; // gc_node — don't delete AnnotationDeclaration
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Or,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 179: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation);
            (yyvsp[-2].fa) = nullptr; // gc_node — don't delete AnnotationDeclaration
        }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            (yyvsp[0].fa) = nullptr; // gc_node — don't delete AnnotationDeclaration
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Xor,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 180: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 181: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 182: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 183: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 184: /* optional_annotation_list: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 185: /* optional_annotation_list: '[' annotation_list ']'  */
                                       { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 186: /* optional_annotation_list_with_emit_semis: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 187: /* optional_annotation_list_with_emit_semis: '[' annotation_list ']' optional_emit_semis  */
                                                          { (yyval.faList) = (yyvsp[-2].faList); }
    break;

  case 188: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 189: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 190: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 191: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yyloc));
    }
    break;

  case 192: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 193: /* optional_function_type: "->" type_declaration  */
                                           {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 194: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 195: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 196: /* function_name: "operator" '='  */
                             { (yyval.s) = new string("="); }
    break;

  case 197: /* function_name: "operator" "<-"  */
                             { (yyval.s) = new string("<-"); }
    break;

  case 198: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 199: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 200: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 201: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 202: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 203: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 204: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 205: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 206: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 207: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 208: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 209: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 210: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 211: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 212: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 213: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 214: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 215: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 216: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 217: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 218: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 219: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 220: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 221: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 222: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 223: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 224: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 225: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 226: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 227: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 228: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 229: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 230: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 231: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 232: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 233: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 234: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 235: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 236: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 237: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 238: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 239: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 240: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 241: /* function_name: "operator" '[' ']' '='  */
                                 { (yyval.s) = new string("[]="); }
    break;

  case 242: /* function_name: "operator" '[' ']' "<-"  */
                                    { (yyval.s) = new string("[]<-"); }
    break;

  case 243: /* function_name: "operator" '[' ']' ":="  */
                                      { (yyval.s) = new string("[]:="); }
    break;

  case 244: /* function_name: "operator" '[' ']' "+="  */
                                     { (yyval.s) = new string("[]+="); }
    break;

  case 245: /* function_name: "operator" '[' ']' "-="  */
                                     { (yyval.s) = new string("[]-="); }
    break;

  case 246: /* function_name: "operator" '[' ']' "*="  */
                                     { (yyval.s) = new string("[]*="); }
    break;

  case 247: /* function_name: "operator" '[' ']' "/="  */
                                     { (yyval.s) = new string("[]/="); }
    break;

  case 248: /* function_name: "operator" '[' ']' "%="  */
                                     { (yyval.s) = new string("[]%="); }
    break;

  case 249: /* function_name: "operator" '[' ']' "&="  */
                                     { (yyval.s) = new string("[]&="); }
    break;

  case 250: /* function_name: "operator" '[' ']' "|="  */
                                     { (yyval.s) = new string("[]|="); }
    break;

  case 251: /* function_name: "operator" '[' ']' "^="  */
                                     { (yyval.s) = new string("[]^="); }
    break;

  case 252: /* function_name: "operator" '[' ']' "&&="  */
                                        { (yyval.s) = new string("[]&&="); }
    break;

  case 253: /* function_name: "operator" '[' ']' "||="  */
                                        { (yyval.s) = new string("[]||="); }
    break;

  case 254: /* function_name: "operator" '[' ']' "^^="  */
                                        { (yyval.s) = new string("[]^^="); }
    break;

  case 255: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 256: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 257: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 258: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 259: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 260: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 261: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 262: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 263: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 264: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 265: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 266: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 267: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 268: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 269: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 270: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 271: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 272: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 273: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 274: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 275: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 276: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 277: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 278: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 279: /* function_name: "operator" "is" das_type_name  */
                                                { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 280: /* function_name: "operator" "as" das_type_name  */
                                                { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 281: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 282: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 283: /* function_name: "operator" '?' "as" das_type_name  */
                                                    { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 284: /* function_name: das_type_name  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 285: /* das_type_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 286: /* das_type_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 287: /* das_type_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 288: /* das_type_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 289: /* das_type_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 290: /* das_type_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 291: /* das_type_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 292: /* das_type_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 293: /* das_type_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 294: /* das_type_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 295: /* das_type_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 296: /* das_type_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 297: /* das_type_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 298: /* das_type_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 299: /* das_type_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 300: /* das_type_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 301: /* das_type_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 302: /* das_type_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 303: /* das_type_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 304: /* das_type_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 305: /* das_type_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 306: /* das_type_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 307: /* das_type_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 308: /* das_type_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 309: /* das_type_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 310: /* das_type_name: "float16"  */
                     { (yyval.s) = new string("float16"); }
    break;

  case 311: /* das_type_name: "half2"  */
                     { (yyval.s) = new string("half2"); }
    break;

  case 312: /* das_type_name: "half3"  */
                     { (yyval.s) = new string("half3"); }
    break;

  case 313: /* das_type_name: "half4"  */
                     { (yyval.s) = new string("half4"); }
    break;

  case 314: /* das_type_name: "half8"  */
                     { (yyval.s) = new string("half8"); }
    break;

  case 315: /* das_type_name: "short2"  */
                     { (yyval.s) = new string("short2"); }
    break;

  case 316: /* das_type_name: "short3"  */
                     { (yyval.s) = new string("short3"); }
    break;

  case 317: /* das_type_name: "short4"  */
                     { (yyval.s) = new string("short4"); }
    break;

  case 318: /* das_type_name: "short8"  */
                     { (yyval.s) = new string("short8"); }
    break;

  case 319: /* das_type_name: "ushort2"  */
                     { (yyval.s) = new string("ushort2"); }
    break;

  case 320: /* das_type_name: "ushort3"  */
                     { (yyval.s) = new string("ushort3"); }
    break;

  case 321: /* das_type_name: "ushort4"  */
                     { (yyval.s) = new string("ushort4"); }
    break;

  case 322: /* das_type_name: "ushort8"  */
                     { (yyval.s) = new string("ushort8"); }
    break;

  case 323: /* das_type_name: "byte2"  */
                     { (yyval.s) = new string("byte2"); }
    break;

  case 324: /* das_type_name: "byte3"  */
                     { (yyval.s) = new string("byte3"); }
    break;

  case 325: /* das_type_name: "byte4"  */
                     { (yyval.s) = new string("byte4"); }
    break;

  case 326: /* das_type_name: "byte8"  */
                     { (yyval.s) = new string("byte8"); }
    break;

  case 327: /* das_type_name: "byte16"  */
                     { (yyval.s) = new string("byte16"); }
    break;

  case 328: /* das_type_name: "ubyte2"  */
                     { (yyval.s) = new string("ubyte2"); }
    break;

  case 329: /* das_type_name: "ubyte3"  */
                     { (yyval.s) = new string("ubyte3"); }
    break;

  case 330: /* das_type_name: "ubyte4"  */
                     { (yyval.s) = new string("ubyte4"); }
    break;

  case 331: /* das_type_name: "ubyte8"  */
                     { (yyval.s) = new string("ubyte8"); }
    break;

  case 332: /* das_type_name: "ubyte16"  */
                     { (yyval.s) = new string("ubyte16"); }
    break;

  case 333: /* optional_template: %empty  */
                                        { (yyval.b) = false; }
    break;

  case 334: /* optional_template: "template"  */
                                        { (yyval.b) = true; }
    break;

  case 335: /* global_function_declaration: optional_annotation_list_with_emit_semis "def" optional_template function_declaration  */
                                                                                                                              {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
        (yyvsp[0].pFuncDecl)->isTemplate = (yyvsp[-1].b);
        assignDefaultArguments((yyvsp[0].pFuncDecl));
        runFunctionAnnotations(scanner, yyextra, (yyvsp[0].pFuncDecl), (yyvsp[-3].faList), tokAt(scanner,(yylsp[-3])));
        if ( (yyvsp[0].pFuncDecl)->isGeneric() ) {
            implAddGenericFunction(scanner,(yyvsp[0].pFuncDecl));
        } else {
            if ( !yyextra->g_Program->addFunction((yyvsp[0].pFuncDecl)) ) {
                das2_yyerror(scanner,"function is already defined " +
                    (yyvsp[0].pFuncDecl)->getMangledName(),(yyvsp[0].pFuncDecl)->at,
                        CompilationError::already_declared_function);
            }
        }
        (yyvsp[0].pFuncDecl)->delRef();
    }
    break;

  case 336: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 337: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 338: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 339: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 340: /* $@15: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 341: /* function_declaration: optional_public_or_private_function $@15 function_declaration_header optional_emit_semis block_or_simple_block  */
                                                                                         {
        (yyvsp[-2].pFuncDecl)->body = (yyvsp[0].pExpression);
        (yyvsp[-2].pFuncDecl)->privateFunction = !(yyvsp[-4].b);
        (yyval.pFuncDecl) = (yyvsp[-2].pFuncDecl);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
        }
    }
    break;

  case 342: /* expression_block_finally: %empty  */
        {
        (yyval.pExpression) = nullptr;
    }
    break;

  case 343: /* $@16: %empty  */
                  {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 344: /* $@17: %empty  */
                             {
        yyextra->pop_nesteds();
    }
    break;

  case 345: /* expression_block_finally: "finally" $@16 '{' expressions $@17 '}'  */
          {
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 346: /* $@18: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 347: /* $@19: %empty  */
                         {
        yyextra->pop_nesteds();
    }
    break;

  case 348: /* expression_block: '{' $@18 expressions $@19 '}' expression_block_finally  */
                                        {
        (yyval.pExpression) = (yyvsp[-3].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-5]),(yylsp[0]));
        if ( (yyvsp[0].pExpression) ) {
            auto pF = (ExprBlock *) (yyvsp[0].pExpression);
            auto pB = (ExprBlock *) (yyval.pExpression);
            swap ( pB->finalList, pF->list );
            // gc_node — don't delete Expression
        }
    }
    break;

  case 349: /* expr_call_pipe_no_bracket: expr_call expr_full_block_assumed_piped  */
                                                           {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) (yyvsp[-1].pExpression);
            pCall->arguments.push_back((yyvsp[0].pExpression));
            pCall->pipedCallArgument = true;
            pCall->at = tokAt(scanner,(yyloc));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else if ( (yyvsp[-1].pExpression)->rtti_isNamedCall() ) {
            // piped block on a named call: pad-aware resolution lands it on the block param
            auto nc = (ExprNamedCall *) (yyvsp[-1].pExpression);
            nc->nonNamedArguments.push_back((yyvsp[0].pExpression));
            nc->pipedCallArgument = true;
            nc->at = tokAt(scanner,(yyloc));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            das2_yyerror(scanner,"piped block requires a function call",tokAt(scanner,(yylsp[0])),
                CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    }
    break;

  case 350: /* expr_call_pipe_no_bracket: expr_method_call_no_bracket expr_full_block_assumed_piped  */
                                                                             {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) (yyvsp[-1].pExpression);
            pCall->arguments.push_back((yyvsp[0].pExpression));
            pCall->pipedCallArgument = true;
            pCall->at = tokAt(scanner,(yyloc));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else if ( (yyvsp[-1].pExpression)->rtti_isNamedCall() ) {
            // piped block on a named call: pad-aware resolution lands it on the block param
            auto nc = (ExprNamedCall *) (yyvsp[-1].pExpression);
            nc->nonNamedArguments.push_back((yyvsp[0].pExpression));
            nc->pipedCallArgument = true;
            nc->at = tokAt(scanner,(yyloc));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            das2_yyerror(scanner,"piped block requires a function call",tokAt(scanner,(yylsp[0])),
                CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    }
    break;

  case 351: /* expr_call_pipe_no_bracket: expr_field_no_bracket expr_full_block_assumed_piped  */
                                                                       {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) (yyvsp[-1].pExpression);
            pCall->arguments.push_back((yyvsp[0].pExpression));
            pCall->pipedCallArgument = true;
            pCall->at = tokAt(scanner,(yyloc));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else if ( (yyvsp[-1].pExpression)->rtti_isNamedCall() ) {
            // piped block on a named call: pad-aware resolution lands it on the block param
            auto nc = (ExprNamedCall *) (yyvsp[-1].pExpression);
            nc->nonNamedArguments.push_back((yyvsp[0].pExpression));
            nc->pipedCallArgument = true;
            nc->at = tokAt(scanner,(yyloc));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            das2_yyerror(scanner,"piped block requires a function call",tokAt(scanner,(yylsp[0])),
                CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    }
    break;

  case 352: /* expr_call_pipe_no_bracket: expr_named_call expr_full_block_assumed_piped  */
                                                                 {
        // free-function named call + piped block: pad-aware resolution lands it on the block param
        auto nc = (ExprNamedCall *) (yyvsp[-1].pExpression);
        nc->nonNamedArguments.push_back((yyvsp[0].pExpression));
        nc->pipedCallArgument = true;
        nc->at = tokAt(scanner,(yyloc));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 353: /* expression_any: SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 354: /* expression_any: expr_assign_no_bracket SEMICOLON  */
                                                    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 355: /* expression_any: expression_delete SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 356: /* expression_any: expression_let  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 357: /* expression_any: expression_while_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 358: /* expression_any: expression_unsafe  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 359: /* expression_any: expression_with  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 360: /* expression_any: expression_with_alias SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 361: /* expression_any: expression_for_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 362: /* expression_any: expression_break SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 363: /* expression_any: expression_continue SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 364: /* expression_any: expression_return SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 365: /* expression_any: expression_yield SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 366: /* expression_any: expression_if_then_else  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 367: /* expression_any: expression_if_then_else_oneliner  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 368: /* expression_any: expression_try_catch  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 369: /* expression_any: expression_label SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 370: /* expression_any: expression_goto SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 371: /* expression_any: "pass" SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 372: /* $@20: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 373: /* $@21: %empty  */
                         {
        yyextra->pop_nesteds();
    }
    break;

  case 374: /* expression_any: '{' $@20 expressions $@21 '}' expression_block_finally  */
                                        {
        (yyval.pExpression) = (yyvsp[-3].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-5]),(yylsp[0]));
        if ( (yyvsp[0].pExpression) ) {
            auto pF = (ExprBlock *) (yyvsp[0].pExpression);
            auto pB = (ExprBlock *) (yyval.pExpression);
            swap ( pB->finalList, pF->list );
            // gc_node — don't delete Expression
        }
    }
    break;

  case 375: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 376: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 377: /* expressions: expressions error  */
                                 {
        (void)(yyvsp[-1].pExpression); /* gc_node — don't delete Expression */ (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 378: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 379: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 380: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 381: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 382: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 383: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 384: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 385: /* name_in_namespace: "name" "::" "name"  */
                                               {
            auto ita = yyextra->das_module_alias.find(*(yyvsp[-2].s));
            if ( ita == yyextra->das_module_alias.end() ) {
                *(yyvsp[-2].s) += "::";
            } else {
                *(yyvsp[-2].s) = ita->second + "::";
            }
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
    break;

  case 386: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 387: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yyloc)), (yyvsp[0].pExpression));
    }
    break;

  case 388: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yyloc)), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 389: /* $@22: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 390: /* $@23: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 391: /* new_type_declaration: '<' $@22 type_declaration '>' $@23  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 392: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 393: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yyloc)),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 394: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yyloc)),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 395: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yyloc)),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 396: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokRangeAt(scanner,(yylsp[-3]),(yyloc));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression));
    }
    break;

  case 397: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression));
    }
    break;

  case 398: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yyloc)),(yyvsp[0].pExpression));
    }
    break;

  case 399: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 400: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 401: /* expression_return: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 402: /* expression_return: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yyloc)),(yyvsp[0].pExpression));
    }
    break;

  case 403: /* expression_return: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yyloc)),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 404: /* expression_yield: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yyloc)),(yyvsp[0].pExpression));
    }
    break;

  case 405: /* expression_yield: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yyloc)),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 406: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 407: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 408: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 409: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 410: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 411: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 412: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 413: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 414: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 415: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 416: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 417: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                        {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-5])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 418: /* expression_let: kwd_let optional_in_scope optional_field_annotation let_variable_declaration  */
                                                                                                {
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-3].b),(yyvsp[-2].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 419: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 420: /* expression_let: kwd_let optional_in_scope '{' variable_declaration_list '}'  */
                                                                               {
        (yyval.pExpression) = ast_LetList(scanner,(yyvsp[-4].b),(yyvsp[-3].b),*(yyvsp[-1].pVarDeclList),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 421: /* $@24: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 422: /* $@25: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 423: /* expr_cast: "cast" '<' $@24 type_declaration_no_options '>' $@25 '(' expr ')'  */
                                                                                                                                                        {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),(yyvsp[-5].pTypeDecl));
    }
    break;

  case 424: /* $@26: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 425: /* $@27: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 426: /* expr_cast: "upcast" '<' $@26 type_declaration_no_options '>' $@27 '(' expr ')'  */
                                                                                                                                                          {
        auto pCast = new ExprCast(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),(yyvsp[-5].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 427: /* $@28: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 428: /* $@29: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 429: /* expr_cast: "reinterpret" '<' $@28 type_declaration_no_options '>' $@29 '(' expr ')'  */
                                                                                                                                                               {
        auto pCast = new ExprCast(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),(yyvsp[-5].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 430: /* $@30: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 431: /* $@31: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 432: /* expr_type_decl: "type" '<' $@30 type_declaration '>' $@31  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 433: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
                                                                          {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yyloc)),*(yyvsp[-3].s),ptd->typeexpr);
                // gc_node — don't delete Expression
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yyloc)),*(yyvsp[-3].s),(yyvsp[-1].pExpression));
            }
            delete (yyvsp[-3].s);
    }
    break;

  case 434: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
                                                                                                {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yyloc)),*(yyvsp[-6].s),ptd->typeexpr,*(yyvsp[-4].s));
                // gc_node — don't delete Expression
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yyloc)),*(yyvsp[-6].s),(yyvsp[-1].pExpression),*(yyvsp[-4].s));
            }
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 435: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" c_or_s "name" '>' '(' expr ')'  */
                                                                                                                        {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yyloc)),*(yyvsp[-8].s),ptd->typeexpr,*(yyvsp[-6].s),*(yyvsp[-4].s));
                // gc_node — don't delete Expression
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yyloc)),*(yyvsp[-8].s),(yyvsp[-1].pExpression),*(yyvsp[-6].s),*(yyvsp[-4].s));
            }
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 436: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 437: /* expr_list: "<-" expr  */
                             {
            (yyval.pExpression) = ast_makeMoveArgument(scanner, (yyvsp[0].pExpression), tokAt(scanner,(yyloc)));
    }
    break;

  case 438: /* expr_list: expr_list ',' expr  */
                                        {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 439: /* expr_list: expr_list ',' "<-" expr  */
                                                   {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yyloc)),(yyvsp[-3].pExpression),ast_makeMoveArgument(scanner, (yyvsp[0].pExpression), tokAt(scanner,(yylsp[0]))));
    }
    break;

  case 440: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 441: /* block_or_simple_block: "=>" expr_no_bracket  */
                                                   {
            auto retE = new ExprReturn(tokAt(scanner,(yyloc)), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yyloc));
            blkE->generated = true;
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 442: /* block_or_simple_block: "=>" "<-" expr_no_bracket  */
                                                          {
            auto retE = new ExprReturn(tokAt(scanner,(yyloc)), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yyloc));
            blkE->generated = true;
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 443: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 444: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 445: /* block_or_lambda: "@@"  */
                  { (yyval.i) = 2;   /* local function */ }
    break;

  case 446: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 447: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 448: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 449: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 450: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 451: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 452: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 453: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 454: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 455: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yyloc)),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 456: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yyloc)),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 457: /* expr_full_block_assumed_piped: '{' expressions '}'  */
                                   {
        // block span is brace-to-brace (@$), not the statements' span (@block)
        (yyval.pExpression) = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc))),(yyvsp[-1].pExpression),tokAt(scanner,(yyloc)),tokAt(scanner,(yyloc)),tokAt(scanner,(yyloc)),LineInfo());
    }
    break;

  case 458: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstInt>(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 459: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstUInt>(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 460: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstInt64>(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 461: /* expr_numeric_const: '-' "int64 minimum magnitude (requires minus)"  */
                                             { (yyval.pExpression) = newConstLiteral<ExprConstInt64>(tokAt(scanner,(yyloc)),int64_t(INT64_MIN)); }
    break;

  case 462: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstUInt64>(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 463: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstUInt8>(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 464: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstFloat>(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 465: /* expr_numeric_const: "float16 constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstFloat16>(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 466: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstDouble>(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 467: /* expr_assign_no_bracket: expr_no_bracket  */
                                                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 468: /* expr_assign_no_bracket: expr_no_bracket '=' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 469: /* expr_assign_no_bracket: expr_no_bracket "<-" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr_assign_no_bracket: expr_no_bracket "<-" make_table_decl  */
                                                                   { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 471: /* expr_assign_no_bracket: expr_no_bracket "<-" array_comprehension  */
                                                                     { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr_assign_no_bracket: expr_no_bracket ":=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 473: /* expr_assign_no_bracket: expr_no_bracket "!==" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),true); }
    break;

  case 474: /* expr_assign_no_bracket: expr_no_bracket "!<-" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),true); }
    break;

  case 475: /* expr_assign_no_bracket: expr_no_bracket "!<-" make_table_decl  */
                                                                      { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),true); }
    break;

  case 476: /* expr_assign_no_bracket: expr_no_bracket "!<-" array_comprehension  */
                                                                        { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),true); }
    break;

  case 477: /* expr_assign_no_bracket: expr_no_bracket "!:=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),true); }
    break;

  case 478: /* expr_assign_no_bracket: expr_no_bracket "&=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 479: /* expr_assign_no_bracket: expr_no_bracket "|=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 480: /* expr_assign_no_bracket: expr_no_bracket "^=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 481: /* expr_assign_no_bracket: expr_no_bracket "&&=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 482: /* expr_assign_no_bracket: expr_no_bracket "||=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 483: /* expr_assign_no_bracket: expr_no_bracket "^^=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 484: /* expr_assign_no_bracket: expr_no_bracket "+=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 485: /* expr_assign_no_bracket: expr_no_bracket "-=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 486: /* expr_assign_no_bracket: expr_no_bracket "*=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 487: /* expr_assign_no_bracket: expr_no_bracket "/=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 488: /* expr_assign_no_bracket: expr_no_bracket "%=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 489: /* expr_assign_no_bracket: expr_no_bracket "<<=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 490: /* expr_assign_no_bracket: expr_no_bracket ">>=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 491: /* expr_assign_no_bracket: expr_no_bracket "<<<=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 492: /* expr_assign_no_bracket: expr_no_bracket ">>>=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 493: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yyloc)),*(yyvsp[-5].s));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 494: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yyloc)),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 495: /* expr_named_call: name_in_namespace '(' expr_list ',' make_struct_fields ')'  */
                                                                                          {
        // bracket-less mixed named call: foo(pos..., name = value) -- named args are a strict suffix
        auto nc = new ExprNamedCall(tokAt(scanner,(yyloc)),*(yyvsp[-5].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-3].pExpression));
        nc->arguments = (yyvsp[-1].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 496: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' ')'  */
                                                                    {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yyloc)), tokRangeAt(scanner,(yylsp[-4]),(yylsp[-2])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 497: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' expr_list ')'  */
                                                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yyloc)), tokRangeAt(scanner,(yylsp[-5]),(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 498: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                                     {
        auto nc = new ExprNamedCall(tokAt(scanner,(yyloc)),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = (yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-7].pExpression));
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 499: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' expr_list ',' make_struct_fields ')'  */
                                                                                                                      {
        // bracket-less mixed named method call: a->m(pos..., name = value)
        auto nc = new ExprNamedCall(tokAt(scanner,(yyloc)),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = (yyvsp[-1].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-7].pExpression));
        auto callArgs = sequenceToList((yyvsp[-3].pExpression));
        nc->nonNamedArguments.insert ( nc->nonNamedArguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 500: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' make_struct_fields ')'  */
                                                                                             {
        // bracket-less all-named method call, no positional: a->m(name = value)
        auto nc = new ExprNamedCall(tokAt(scanner,(yyloc)),*(yyvsp[-3].s));
        nc->methodCall = true;
        nc->arguments = (yyvsp[-1].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-5].pExpression));
        delete (yyvsp[-3].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 501: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 502: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 503: /* func_addr_expr: "@@" func_addr_name  */
                                            {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 504: /* $@32: %empty  */
                      { yyextra->das_arrow_depth ++; }
    break;

  case 505: /* $@33: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 506: /* func_addr_expr: "@@" '<' $@32 type_declaration_no_options '>' $@33 func_addr_name  */
                                                                                                                                                         {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 507: /* $@34: %empty  */
                      { yyextra->das_arrow_depth ++; }
    break;

  case 508: /* $@35: %empty  */
                                                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 509: /* func_addr_expr: "@@" '<' $@34 optional_function_argument_list optional_function_type '>' $@35 func_addr_name  */
                                                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value));
        expr->funcType = new TypeDecl(Type::tFunction, expr->at);
        expr->funcType->firstType = (yyvsp[-3].pTypeDecl);
        if ( (yyvsp[-4].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, expr->funcType, (yyvsp[-4].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-4].pVarDeclList));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 510: /* expr_field_no_bracket: expr_no_bracket '.' "name"  */
                                                         {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yyloc)), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 511: /* expr_field_no_bracket: expr_no_bracket '.' '.' "name"  */
                                                             {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yyloc)), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 512: /* expr_field_no_bracket: expr_no_bracket "!." "name"  */
                                                            {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yyloc)), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 513: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' ')'  */
                                                                 {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yyloc)), tokRangeAt(scanner,(yylsp[-4]),(yylsp[-2])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 514: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' expr_list ')'  */
                                                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yyloc)), tokRangeAt(scanner,(yylsp[-5]),(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 515: /* expr_field_no_bracket: expr_no_bracket "!." "name" '(' ')'  */
                                                                    {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yyloc)), tokRangeAt(scanner,(yylsp[-4]),(yylsp[-2])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), *(yyvsp[-2].s), true);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 516: /* expr_field_no_bracket: expr_no_bracket "!." "name" '(' expr_list ')'  */
                                                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yyloc)), tokRangeAt(scanner,(yylsp[-5]),(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), *(yyvsp[-3].s), true);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 517: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yyloc)),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = (yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-7].pExpression));
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 518: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' expr_list ',' make_struct_fields ')'  */
                                                                                                                   {
        // bracket-less mixed named method call: a.m(pos..., name = value)
        auto nc = new ExprNamedCall(tokAt(scanner,(yyloc)),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = (yyvsp[-1].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-7].pExpression));
        auto callArgs = sequenceToList((yyvsp[-3].pExpression));
        nc->nonNamedArguments.insert ( nc->nonNamedArguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 519: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' make_struct_fields ')'  */
                                                                                          {
        // bracket-less all-named method call, no positional: a.m(name = value)
        auto nc = new ExprNamedCall(tokAt(scanner,(yyloc)),*(yyvsp[-3].s));
        nc->methodCall = true;
        nc->arguments = (yyvsp[-1].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-5].pExpression));
        delete (yyvsp[-3].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 520: /* expr_field_no_bracket: expr_no_bracket '.' basic_type_declaration '(' ')'  */
                                                                                   {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yyloc)), tokRangeAt(scanner,(yylsp[-4]),(yylsp[-2])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 521: /* expr_field_no_bracket: expr_no_bracket '.' basic_type_declaration '(' expr_list ')'  */
                                                                                                        {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yyloc)), tokRangeAt(scanner,(yylsp[-5]),(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 522: /* $@36: %empty  */
                                          { yyextra->das_suppress_errors=true; }
    break;

  case 523: /* $@37: %empty  */
                                                                                       { yyextra->das_suppress_errors=false; }
    break;

  case 524: /* expr_field_no_bracket: expr_no_bracket '.' $@36 error $@37  */
                                                                                                                               {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yyloc)), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 525: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yyloc)),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 526: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
                                                          {
            auto dd = new ExprMakeStruct(tokAt(scanner,(yyloc)));
            dd->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            dd->useInitializer = false;
            dd->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = dd;
    }
    break;

  case 527: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokRangeAt(scanner,(yylsp[-3]),(yyloc));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 528: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 529: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yyloc)),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 530: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yyloc)),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 531: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yyloc)),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 532: /* expr: expr_no_bracket  */
                                       { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 533: /* expr: make_table_decl  */
                                     { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 534: /* expr: array_comprehension  */
                                     { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 535: /* expr_no_bracket: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 536: /* expr_no_bracket: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 537: /* expr_no_bracket: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 538: /* expr_no_bracket: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 539: /* expr_no_bracket: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 540: /* expr_no_bracket: make_decl_no_bracket  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 541: /* expr_no_bracket: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 542: /* expr_no_bracket: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 543: /* expr_no_bracket: expr_field_no_bracket  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 544: /* expr_no_bracket: expr_mtag_no_bracket  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 545: /* expr_no_bracket: '!' expr_no_bracket  */
                                                         { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yyloc)),"!",(yyvsp[0].pExpression)); }
    break;

  case 546: /* expr_no_bracket: '~' expr_no_bracket  */
                                                         { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yyloc)),"~",(yyvsp[0].pExpression)); }
    break;

  case 547: /* expr_no_bracket: '+' expr_no_bracket  */
                                                             { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yyloc)),"+",(yyvsp[0].pExpression)); }
    break;

  case 548: /* expr_no_bracket: '-' expr_no_bracket  */
                                                             { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yyloc)),"-",(yyvsp[0].pExpression)); }
    break;

  case 549: /* expr_no_bracket: expr_no_bracket "<<" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 550: /* expr_no_bracket: expr_no_bracket ">>" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 551: /* expr_no_bracket: expr_no_bracket "<<<" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 552: /* expr_no_bracket: expr_no_bracket ">>>" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 553: /* expr_no_bracket: expr_no_bracket '+' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 554: /* expr_no_bracket: expr_no_bracket '-' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 555: /* expr_no_bracket: expr_no_bracket '*' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 556: /* expr_no_bracket: expr_no_bracket '/' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 557: /* expr_no_bracket: expr_no_bracket '%' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 558: /* expr_no_bracket: expr_no_bracket '<' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 559: /* expr_no_bracket: expr_no_bracket '>' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 560: /* expr_no_bracket: expr_no_bracket "==" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 561: /* expr_no_bracket: expr_no_bracket "!=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 562: /* expr_no_bracket: expr_no_bracket "<=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 563: /* expr_no_bracket: expr_no_bracket ">=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 564: /* expr_no_bracket: expr_no_bracket '&' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 565: /* expr_no_bracket: expr_no_bracket '|' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 566: /* expr_no_bracket: expr_no_bracket '^' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 567: /* expr_no_bracket: expr_no_bracket "&&" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 568: /* expr_no_bracket: expr_no_bracket "||" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 569: /* expr_no_bracket: expr_no_bracket "^^" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yyloc)),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 570: /* expr_no_bracket: expr_no_bracket ".." expr_no_bracket  */
                                                                   {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yyloc)),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 571: /* expr_no_bracket: "++" expr_no_bracket  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yyloc)),"++", (yyvsp[0].pExpression)); }
    break;

  case 572: /* expr_no_bracket: "--" expr_no_bracket  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yyloc)),"--", (yyvsp[0].pExpression)); }
    break;

  case 573: /* expr_no_bracket: expr_no_bracket "++"  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yyloc)),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 574: /* expr_no_bracket: expr_no_bracket "--"  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yyloc)),"---", (yyvsp[-1].pExpression)); }
    break;

  case 575: /* expr_no_bracket: '(' expr_list optional_comma ')'  */
                                                         {
            if ( (yyvsp[-2].pExpression)->rtti_isSequence() ) {
                auto mkt = new ExprMakeTuple(tokAt(scanner,(yyloc)));
                mkt->values = sequenceToList((yyvsp[-2].pExpression));
                mkt->shorthandRecordNames = ast_tupleCollectShorthandNames(mkt->values);
                (yyval.pExpression) = mkt;
            } else if ( (yyvsp[-1].b) ) {
                auto mkt = new ExprMakeTuple(tokAt(scanner,(yyloc)));
                mkt->values.push_back((yyvsp[-2].pExpression));
                mkt->shorthandRecordNames = ast_tupleCollectShorthandNames(mkt->values);
                (yyval.pExpression) = mkt;
            } else {
                (yyval.pExpression) = (yyvsp[-2].pExpression);
            }
        }
    break;

  case 576: /* expr_no_bracket: '(' make_struct_single ')'  */
                                      {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yyloc)));
        for ( auto & arg : *(((ExprMakeStruct *)(yyvsp[-1].pExpression))->structs.back()) ) {
            mkt->values.push_back(arg->value);
            mkt->recordNames.push_back(arg->name);
        }
        // gc_node — don't delete Expression
        (yyval.pExpression) = mkt;
    }
    break;

  case 577: /* expr_no_bracket: expr_no_bracket '[' expr ']'  */
                                                            { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yyloc)), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 578: /* expr_no_bracket: expr_no_bracket '.' '[' expr ']'  */
                                                                { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yyloc)), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 579: /* expr_no_bracket: expr_no_bracket "![" expr ']'  */
                                                               { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yyloc)), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 580: /* expr_no_bracket: expr_no_bracket "?[" expr ']'  */
                                                            { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yyloc)), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 581: /* expr_no_bracket: expr_no_bracket '.' "?[" expr ']'  */
                                                                { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yyloc)), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 582: /* expr_no_bracket: expr_no_bracket "!?[" expr ']'  */
                                                               { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yyloc)), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 583: /* expr_no_bracket: expr_no_bracket "?." "name"  */
                                                            { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yyloc)), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 584: /* expr_no_bracket: expr_no_bracket '.' "?." "name"  */
                                                                { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yyloc)), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 585: /* expr_no_bracket: expr_no_bracket "!?." "name"  */
                                                               { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yyloc)), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 586: /* expr_no_bracket: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 587: /* expr_no_bracket: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 588: /* expr_no_bracket: '*' expr_no_bracket  */
                                                              { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yyloc)),(yyvsp[0].pExpression)); }
    break;

  case 589: /* expr_no_bracket: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression)); }
    break;

  case 590: /* expr_no_bracket: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression)); }
    break;

  case 591: /* $@38: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 592: /* $@39: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 593: /* expr_no_bracket: "addr" '<' $@38 type_declaration_no_options '>' $@39 '(' expr ')'  */
                                                                                                                                                        {
        auto pRef2Ptr = new ExprRef2Ptr(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression));
        pRef2Ptr->generated = true;
        auto pCast = new ExprCast(tokAt(scanner,(yyloc)),pRef2Ptr,(yyvsp[-5].pTypeDecl));
        pCast->reinterpret = true;
        pCast->fromAddrSugar = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 594: /* expr_no_bracket: expr_generator  */
                                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 595: /* expr_no_bracket: expr_no_bracket "??" expr_no_bracket  */
                                                                         { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 596: /* expr_no_bracket: expr_no_bracket "!??" expr_no_bracket  */
                                                                            {
        auto nc = new ExprNullCoalescing(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        nc->no_promotion = true;
        (yyval.pExpression) = nc;
    }
    break;

  case 597: /* expr_no_bracket: expr_no_bracket '?' expr_no_bracket ':' expr_no_bracket  */
                                                                                           {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yyloc)),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 598: /* $@40: %empty  */
                                                          { yyextra->das_arrow_depth ++; }
    break;

  case 599: /* $@41: %empty  */
                                                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 600: /* expr_no_bracket: expr_no_bracket "is" "type" '<' $@40 type_declaration_no_options '>' $@41  */
                                                                                                                                                                  {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yyloc)),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 601: /* expr_no_bracket: expr_no_bracket "is" basic_type_declaration  */
                                                                          {
        auto vdecl = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc)));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 602: /* expr_no_bracket: expr_no_bracket "is" "name"  */
                                                         {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 603: /* $@42: %empty  */
                                                         { yyextra->das_arrow_depth ++; }
    break;

  case 604: /* $@43: %empty  */
                                                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 605: /* expr_no_bracket: expr_no_bracket "!is" "type" '<' $@42 type_declaration_no_options '>' $@43  */
                                                                                                                                                                 {
        auto isx = new ExprIs(tokAt(scanner,(yyloc)),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
        isx->no_promotion = true;
        (yyval.pExpression) = isx;
    }
    break;

  case 606: /* expr_no_bracket: expr_no_bracket "!is" basic_type_declaration  */
                                                                         {
        auto vdecl = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc)));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        auto isx = new ExprIs(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),vdecl);
        isx->no_promotion = true;
        (yyval.pExpression) = isx;
    }
    break;

  case 607: /* expr_no_bracket: expr_no_bracket "!is" "name"  */
                                                        {
        auto isv = new ExprIsVariant(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        isv->no_promotion = true;
        (yyval.pExpression) = isv;
        delete (yyvsp[0].s);
    }
    break;

  case 608: /* expr_no_bracket: expr_no_bracket "as" "name"  */
                                                         {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 609: /* $@44: %empty  */
                                                          { yyextra->das_arrow_depth ++; }
    break;

  case 610: /* $@45: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 611: /* expr_no_bracket: expr_no_bracket "as" "type" '<' $@44 type_declaration '>' $@45  */
                                                                                                                                                       {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 612: /* expr_no_bracket: expr_no_bracket "as" basic_type_declaration  */
                                                                          {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 613: /* expr_no_bracket: expr_no_bracket "!as" "name"  */
                                                        {
        auto asv = new ExprAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
        delete (yyvsp[0].s);
    }
    break;

  case 614: /* $@46: %empty  */
                                                         { yyextra->das_arrow_depth ++; }
    break;

  case 615: /* $@47: %empty  */
                                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 616: /* expr_no_bracket: expr_no_bracket "!as" "type" '<' $@46 type_declaration '>' $@47  */
                                                                                                                                                      {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        auto asv = new ExprAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-7].pExpression),vname);
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 617: /* expr_no_bracket: expr_no_bracket "!as" basic_type_declaration  */
                                                                         {
        auto asv = new ExprAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
    }
    break;

  case 618: /* expr_no_bracket: expr_no_bracket '?' "as" "name"  */
                                                             {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 619: /* $@48: %empty  */
                                                              { yyextra->das_arrow_depth ++; }
    break;

  case 620: /* $@49: %empty  */
                                                                                                                          { yyextra->das_arrow_depth --; }
    break;

  case 621: /* expr_no_bracket: expr_no_bracket '?' "as" "type" '<' $@48 type_declaration '>' $@49  */
                                                                                                                                                           {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 622: /* expr_no_bracket: expr_no_bracket '?' "as" basic_type_declaration  */
                                                                              {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 623: /* expr_no_bracket: expr_no_bracket "!?as" "name"  */
                                                         {
        auto asv = new ExprSafeAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
        delete (yyvsp[0].s);
    }
    break;

  case 624: /* $@50: %empty  */
                                                          { yyextra->das_arrow_depth ++; }
    break;

  case 625: /* $@51: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 626: /* expr_no_bracket: expr_no_bracket "!?as" "type" '<' $@50 type_declaration '>' $@51  */
                                                                                                                                                       {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        auto asv = new ExprSafeAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-7].pExpression),vname);
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 627: /* expr_no_bracket: expr_no_bracket "!?as" basic_type_declaration  */
                                                                          {
        auto asv = new ExprSafeAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
    }
    break;

  case 628: /* expr_no_bracket: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 629: /* expr_no_bracket: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 630: /* expr_no_bracket: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 631: /* expr_no_bracket: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 632: /* expr_no_bracket: expr_method_call_no_bracket  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 633: /* expr_no_bracket: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 634: /* expr_no_bracket: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 635: /* expr_no_bracket: expr_no_bracket "<|" expr_no_bracket  */
                                                                      { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1])),true); }
    break;

  case 636: /* expr_no_bracket: expr_no_bracket "|>" expr_no_bracket  */
                                                                      { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 637: /* expr_no_bracket: expr_no_bracket "|>" basic_type_declaration  */
                                                                     {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 638: /* expr_no_bracket: expr_call_pipe_no_bracket  */
                                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 639: /* expr_no_bracket: "unsafe" '(' expr ')'  */
                                         {
            (yyvsp[-1].pExpression)->alwaysSafe = true;
            (yyvsp[-1].pExpression)->userSaidItsSafe = true;
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    break;

  case 640: /* expr_no_bracket: expr_no_bracket "=>" expr_no_bracket  */
                                                               {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yyloc)));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 641: /* expr_no_bracket: expr_no_bracket "=>" make_table_decl  */
                                                               {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yyloc)));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 642: /* expr_no_bracket: expr_no_bracket "=>" array_comprehension  */
                                                                   {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yyloc)));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 643: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 644: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yyloc)),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 645: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list optional_emit_semis expression_block  */
                                                                                                                                                  {
        auto closure = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->returnType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),closure,tokAt(scanner,(yyloc)),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 646: /* expr_mtag_no_bracket: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),"e"); }
    break;

  case 647: /* expr_mtag_no_bracket: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),"i"); }
    break;

  case 648: /* expr_mtag_no_bracket: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),"v"); }
    break;

  case 649: /* expr_mtag_no_bracket: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),"b"); }
    break;

  case 650: /* expr_mtag_no_bracket: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),"a"); }
    break;

  case 651: /* expr_mtag_no_bracket: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 652: /* expr_mtag_no_bracket: "$c" '(' expr ')' '(' ')'  */
                                                                  {
            auto ccall = yyextra->g_Program->makeCall(tokRangeAt(scanner,(yylsp[-1]),(yylsp[0])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 653: /* expr_mtag_no_bracket: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                      {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 654: /* expr_mtag_no_bracket: expr_no_bracket '.' "$f" '(' expr ')'  */
                                                                           {
        auto cfield = new ExprField(tokAt(scanner,(yyloc)), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 655: /* expr_mtag_no_bracket: expr_no_bracket "?." "$f" '(' expr ')'  */
                                                                            {
        auto cfield = new ExprSafeField(tokAt(scanner,(yyloc)), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 656: /* expr_mtag_no_bracket: expr_no_bracket '.' '.' "$f" '(' expr ')'  */
                                                                               {
        auto cfield = new ExprField(tokAt(scanner,(yyloc)), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 657: /* expr_mtag_no_bracket: expr_no_bracket "!." "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprField(tokAt(scanner,(yyloc)), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 658: /* expr_mtag_no_bracket: expr_no_bracket '.' "?." "$f" '(' expr ')'  */
                                                                                {
        auto cfield = new ExprSafeField(tokAt(scanner,(yyloc)), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 659: /* expr_mtag_no_bracket: expr_no_bracket "!?." "$f" '(' expr ')'  */
                                                                               {
        auto cfield = new ExprSafeField(tokAt(scanner,(yyloc)), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 660: /* expr_mtag_no_bracket: expr_no_bracket "as" "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 661: /* expr_mtag_no_bracket: expr_no_bracket "!as" "$f" '(' expr ')'  */
                                                                             {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        cfield->no_promotion = true;
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 662: /* expr_mtag_no_bracket: expr_no_bracket '?' "as" "$f" '(' expr ')'  */
                                                                                  {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 663: /* expr_mtag_no_bracket: expr_no_bracket "!?as" "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yyloc)),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        cfield->no_promotion = true;
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 664: /* expr_mtag_no_bracket: expr_no_bracket "is" "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yyloc)),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 665: /* expr_mtag_no_bracket: expr_no_bracket "!is" "$f" '(' expr ')'  */
                                                                             {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yyloc)),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        cfield->no_promotion = true;
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 666: /* expr_mtag_no_bracket: "@@" "$c" '(' expr ')'  */
                                                           {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yyloc)),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 667: /* optional_field_annotation: %empty  */
                                      { (yyval.aaList) = nullptr; }
    break;

  case 668: /* optional_field_annotation: metadata_argument_list  */
                                      { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 669: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 670: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 671: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 672: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 673: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 674: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 675: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 676: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 677: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 678: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 679: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 680: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 681: /* struct_variable_declaration_list: struct_variable_declaration_list "new line, semicolon"  */
                                                                 { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 682: /* struct_variable_declaration_list: struct_variable_declaration_list "typedef" "name" '=' type_declaration SEMICOLON  */
                                                                                                                {
        (yyval.pVarDeclList) = (yyvsp[-5].pVarDeclList);
        ast_structureAlias(scanner,(yyvsp[-3].s),(yyvsp[-1].pTypeDecl),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 683: /* $@52: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 684: /* struct_variable_declaration_list: struct_variable_declaration_list $@52 structure_variable_declaration SEMICOLON  */
                                                     {
        (yyval.pVarDeclList) = (yyvsp[-3].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-3].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *((yyvsp[-1].pVarDecl)->pNameList) ) {
                    crd->afterStructureField(nl.name.c_str(), nl.at);
                }
            }
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructureFields(tak);
        }
    }
    break;

  case 685: /* $@53: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 686: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable "abstract" optional_constant $@53 function_declaration_header SEMICOLON  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 687: /* $@54: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 688: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@54 function_declaration_header optional_emit_semis block_or_simple_block  */
                                                                                                 {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-10].pVarDeclList),(yyvsp[-9].faList),(yyvsp[-6].b),(yyvsp[-7].b),(yyvsp[-5].i),(yyvsp[-4].b),(yyvsp[-2].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-8]),(yylsp[0])),tokAt(scanner,(yylsp[-9])));
            }
    break;

  case 689: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
                                                                                                          {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            if ( (yyvsp[-1].b) ) {
                (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            } else {
                (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            }
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 690: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
                                                                                                       {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            if ( (yyvsp[-1].b) ) {
                (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            } else {
                (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            }
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 691: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none, tokAt(scanner,(yyloc))), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 692: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 693: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 694: /* function_argument_list: function_argument_declaration_no_type ';' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 695: /* function_argument_list: function_argument_declaration_type ';' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 696: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 697: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 698: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 699: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 700: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 701: /* tuple_alias_type_list: %empty  */
      {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 702: /* tuple_alias_type_list: tuple_type  */
                       {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 703: /* tuple_alias_type_list: tuple_alias_type_list semis tuple_type  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
        if ( !yyextra->g_CommentReaders.empty() ) {
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *((yyvsp[0].pVarDecl)->pNameList) ) {
                    crd->afterTupleEntry(nl.name.c_str(), nl.at);
                }
            }
        }
    }
    break;

  case 704: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 705: /* variant_type: "name"  */
                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,new TypeDecl(Type::tVoid,tokAt(scanner,(yylsp[0]))),nullptr);
        delete (yyvsp[0].s);
    }
    break;

  case 706: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 707: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 708: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 709: /* variant_alias_type_list: variant_type  */
                         {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 710: /* variant_alias_type_list: variant_alias_type_list semis variant_type  */
                                                               {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
        if ( !yyextra->g_CommentReaders.empty() ) {
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *((yyvsp[0].pVarDecl)->pNameList) ) {
                    crd->afterVariantEntry(nl.name.c_str(), nl.at);
                }
            }
        }
    }
    break;

  case 711: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 712: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 713: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[0])));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 714: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-1])));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 715: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-2])));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 716: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 717: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 718: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 719: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 720: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 721: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 722: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 723: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 724: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 725: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 726: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 727: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
                                         {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 728: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 729: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                               {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 730: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 731: /* global_let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 732: /* global_let_variable_name_with_pos_list: global_let_variable_name_with_pos_list ',' "name"  */
                                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 733: /* variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 734: /* variable_declaration_list: variable_declaration_list SEMICOLON  */
                                                  {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 735: /* variable_declaration_list: variable_declaration_list let_variable_declaration  */
                                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 736: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 737: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 738: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-4])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 739: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                         {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 740: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                               {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 741: /* global_let_variable_declaration: global_let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-4])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 742: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 743: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 744: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 745: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 746: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 747: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 748: /* global_variable_declaration_list: global_variable_declaration_list SEMICOLON  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 749: /* $@55: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 750: /* global_variable_declaration_list: global_variable_declaration_list $@55 optional_field_annotation let_variable_declaration  */
                                                                      {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders )
                for ( auto & nl : *((yyvsp[0].pVarDecl)->pNameList) )
                    crd->afterGlobalVariable(nl.name.c_str(),tak);
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterGlobalVariables(tak);
        }
        (yyval.pVarDeclList) = (yyvsp[-3].pVarDeclList);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyvsp[-3].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 751: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 752: /* $@56: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 753: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@56 optional_field_annotation global_let_variable_declaration  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders )
                for ( auto & nl : *((yyvsp[0].pVarDecl)->pNameList) )
                    crd->afterGlobalVariable(nl.name.c_str(),tak);
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterGlobalVariables(tak);
        }
        ast_globalLet(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].aaList),(yyvsp[0].pVarDecl));
    }
    break;

  case 754: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        delete (yyvsp[0].s);
    }
    break;

  case 755: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
        delete (yyvsp[-2].s);
    }
    break;

  case 758: /* enum_list: %empty  */
        {
        (yyval.pEnumList) = new Enumeration();
    }
    break;

  case 759: /* enum_list: enum_expression  */
                            {
        (yyval.pEnumList) = new Enumeration();
        if ( !(yyval.pEnumList)->add((yyvsp[0].pEnumPair)->name,(yyvsp[0].pEnumPair)->expr,(yyvsp[0].pEnumPair)->at) ) {
            das2_yyerror(scanner,"enumeration already declared " + (yyvsp[0].pEnumPair)->name, (yyvsp[0].pEnumPair)->at,
                CompilationError::already_declared_enumerator);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            for ( auto & crd : yyextra->g_CommentReaders ) {
                crd->afterEnumerationEntry((yyvsp[0].pEnumPair)->name.c_str(), (yyvsp[0].pEnumPair)->at);
            }
        }
        delete (yyvsp[0].pEnumPair);
    }
    break;

  case 760: /* enum_list: enum_list commas enum_expression  */
                                                 {
        if ( !(yyvsp[-2].pEnumList)->add((yyvsp[0].pEnumPair)->name,(yyvsp[0].pEnumPair)->expr,(yyvsp[0].pEnumPair)->at) ) {
            das2_yyerror(scanner,"enumeration already declared " + (yyvsp[0].pEnumPair)->name, (yyvsp[0].pEnumPair)->at,
                CompilationError::already_declared_enumerator);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            for ( auto & crd : yyextra->g_CommentReaders ) {
                crd->afterEnumerationEntry((yyvsp[0].pEnumPair)->name.c_str(), (yyvsp[0].pEnumPair)->at);
            }
        }
        delete (yyvsp[0].pEnumPair);
        (yyval.pEnumList) = (yyvsp[-2].pEnumList);
    }
    break;

  case 761: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 762: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 763: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 764: /* $@57: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 765: /* single_alias: optional_public_or_private_alias "name" $@57 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->isPrivateAlias = !(yyvsp[-4].b);
        if ( (yyvsp[0].pTypeDecl)->baseType == Type::alias ) {
            das2_yyerror(scanner,"alias cannot be defined in terms of another alias "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::invalid_type_alias);
        }
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !yyextra->g_Program->addAlias((yyvsp[0].pTypeDecl)) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::already_declared_type_alias);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterAlias((yyvsp[-3].s)->c_str(),pubename);
        }
        delete (yyvsp[-3].s);
    }
    break;

  case 768: /* $@58: %empty  */
                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 769: /* distinct_alias: optional_public_or_private_alias "name" "name" $@58 '=' type_declaration  */
                                  {
        if ( *(yyvsp[-4].s) != "distinct" ) {
            das2_yyerror(scanner,"expected 'distinct', got '"+*(yyvsp[-4].s)+"'",tokAt(scanner,(yylsp[-4])),
                CompilationError::invalid_distinct_type);
        } else {
            ast_distinctDeclaration(scanner,(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),!(yyvsp[-5].b),(yyvsp[0].pTypeDecl));
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterAlias((yyvsp[-3].s)->c_str(),pubename);
        }
        delete (yyvsp[-4].s);
        delete (yyvsp[-3].s);
    }
    break;

  case 770: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 771: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 772: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 773: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 774: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 775: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 782: /* $@59: %empty  */
                                                                     {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 783: /* $@60: %empty  */
                                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 784: /* $@61: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 785: /* enum_declaration: optional_annotation_list_with_emit_semis "enum" $@59 optional_public_or_private_enum enum_name optional_enum_basic_type_declaration optional_emit_commas '{' $@60 enum_list optional_commas $@61 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-8].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-12].faList),tokAt(scanner,(yylsp[-12])),(yyvsp[-9].b),(yyvsp[-8].pEnum),(yyvsp[-3].pEnumList),(yyvsp[-7].type));
    }
    break;

  case 786: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 787: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 788: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 789: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 790: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 791: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 792: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 793: /* class_or_struct: "class" "template"  */
                                  { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 794: /* class_or_struct: "struct" "template"  */
                                  { (yyval.i) = CorS_StructTemplate; }
    break;

  case 795: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 796: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 797: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 798: /* optional_struct_variable_declaration_list: ';'  */
            {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 799: /* optional_struct_variable_declaration_list: '{' struct_variable_declaration_list '}'  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 800: /* $@62: %empty  */
                                                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 801: /* $@63: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 802: /* $@64: %empty  */
                                             {
        if ( (yyvsp[-1].pStructure) ) {
            (yyvsp[-1].pStructure)->isClass = (yyvsp[-4].i)==CorS_Class || (yyvsp[-4].i)==CorS_ClassTemplate;
            (yyvsp[-1].pStructure)->isTemplate = (yyvsp[-4].i)==CorS_ClassTemplate || (yyvsp[-4].i)==CorS_StructTemplate;
            (yyvsp[-1].pStructure)->privateStructure = !(yyvsp[-3].b);
        }
    }
    break;

  case 803: /* structure_declaration: optional_annotation_list_with_emit_semis $@62 class_or_struct optional_public_or_private_structure $@63 structure_name optional_emit_semis $@64 optional_struct_variable_declaration_list  */
                                                      {
        yyextra->pop_nesteds();
        if ( (yyvsp[-3].pStructure) ) {
            ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-6])), (yyvsp[-3].pStructure), tokAt(scanner,(yylsp[-3])), (yyvsp[0].pVarDeclList) );
            if ( !yyextra->g_CommentReaders.empty() ) {
                auto tak = tokAt(scanner,(yylsp[-6]));
                for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-3].pStructure),tak);
            }
        } else {
            deleteVariableDeclarationList((yyvsp[0].pVarDeclList));
        }
    }
    break;

  case 804: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 805: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 806: /* variable_name_with_pos_list: "name" "aka" "name"  */
                                         {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 807: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 808: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                           {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 809: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 810: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 811: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 812: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 813: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 814: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 815: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 816: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 817: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 818: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 819: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 820: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 821: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 822: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 823: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 824: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 825: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 826: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 827: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 828: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 829: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 830: /* basic_type_declaration: "float16"  */
                        { (yyval.type) = Type::tFloat16; }
    break;

  case 831: /* basic_type_declaration: "half2"  */
                        { (yyval.type) = Type::tHalf2; }
    break;

  case 832: /* basic_type_declaration: "half3"  */
                        { (yyval.type) = Type::tHalf3; }
    break;

  case 833: /* basic_type_declaration: "half4"  */
                        { (yyval.type) = Type::tHalf4; }
    break;

  case 834: /* basic_type_declaration: "half8"  */
                        { (yyval.type) = Type::tHalf8; }
    break;

  case 835: /* basic_type_declaration: "short2"  */
                        { (yyval.type) = Type::tShort2; }
    break;

  case 836: /* basic_type_declaration: "short3"  */
                        { (yyval.type) = Type::tShort3; }
    break;

  case 837: /* basic_type_declaration: "short4"  */
                        { (yyval.type) = Type::tShort4; }
    break;

  case 838: /* basic_type_declaration: "short8"  */
                        { (yyval.type) = Type::tShort8; }
    break;

  case 839: /* basic_type_declaration: "ushort2"  */
                        { (yyval.type) = Type::tUShort2; }
    break;

  case 840: /* basic_type_declaration: "ushort3"  */
                        { (yyval.type) = Type::tUShort3; }
    break;

  case 841: /* basic_type_declaration: "ushort4"  */
                        { (yyval.type) = Type::tUShort4; }
    break;

  case 842: /* basic_type_declaration: "ushort8"  */
                        { (yyval.type) = Type::tUShort8; }
    break;

  case 843: /* basic_type_declaration: "byte2"  */
                        { (yyval.type) = Type::tByte2; }
    break;

  case 844: /* basic_type_declaration: "byte3"  */
                        { (yyval.type) = Type::tByte3; }
    break;

  case 845: /* basic_type_declaration: "byte4"  */
                        { (yyval.type) = Type::tByte4; }
    break;

  case 846: /* basic_type_declaration: "byte8"  */
                        { (yyval.type) = Type::tByte8; }
    break;

  case 847: /* basic_type_declaration: "byte16"  */
                        { (yyval.type) = Type::tByte16; }
    break;

  case 848: /* basic_type_declaration: "ubyte2"  */
                        { (yyval.type) = Type::tUByte2; }
    break;

  case 849: /* basic_type_declaration: "ubyte3"  */
                        { (yyval.type) = Type::tUByte3; }
    break;

  case 850: /* basic_type_declaration: "ubyte4"  */
                        { (yyval.type) = Type::tUByte4; }
    break;

  case 851: /* basic_type_declaration: "ubyte8"  */
                        { (yyval.type) = Type::tUByte8; }
    break;

  case 852: /* basic_type_declaration: "ubyte16"  */
                        { (yyval.type) = Type::tUByte16; }
    break;

  case 853: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 854: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 855: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 856: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 857: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 858: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 859: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 860: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 861: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 862: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 863: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 864: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 865: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 866: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 867: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 868: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 869: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 870: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 871: /* auto_type_declaration: "$t" '(' expr ')'  */
                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::alias, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->alias = "``MACRO``TAG``";
        (yyval.pTypeDecl)->isTag = true;
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner, (yylsp[-1]));
        (yyval.pTypeDecl)->firstType->typeMacroExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 872: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 873: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 874: /* bitfield_bits: bitfield_bits ',' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 875: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<tuple<string,Expression *>>();
        (yyval.pNameExprList) = pSL;

    }
    break;

  case 876: /* bitfield_alias_bits: "name"  */
                   {
        (yyval.pNameExprList) = new vector<tuple<string,Expression *>>();
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pNameExprList)->emplace_back(*(yyvsp[0].s),nullptr);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[0].s)->c_str(),atvname);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 877: /* bitfield_alias_bits: "name" '=' expr  */
                                   {
        (yyval.pNameExprList) = new vector<tuple<string,Expression *>>();
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pNameExprList)->emplace_back(*(yyvsp[-2].s),(yyvsp[0].pExpression));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[-2].s)->c_str(),atvname);
        }
        delete (yyvsp[-2].s);
    }
    break;

  case 878: /* bitfield_alias_bits: bitfield_alias_bits commas "name"  */
                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameExprList)->emplace_back(*(yyvsp[0].s),nullptr);
        (yyval.pNameExprList) = (yyvsp[-2].pNameExprList);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[0].s)->c_str(),atvname);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 879: /* bitfield_alias_bits: bitfield_alias_bits commas "name" '=' expr  */
                                                                    {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyvsp[-4].pNameExprList)->emplace_back(*(yyvsp[-2].s),(yyvsp[0].pExpression));
        (yyval.pNameExprList) = (yyvsp[-4].pNameExprList);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[-2].s)->c_str(),atvname);
        }
        delete (yyvsp[-2].s);
    }
    break;

  case 880: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 881: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 882: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 883: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 884: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 885: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' '>'  */
                                                                          {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-2].type), tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-2]));
    }
    break;

  case 886: /* $@65: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 887: /* $@66: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 888: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@65 bitfield_bits '>' $@66  */
                                                                                                                                                             {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-5].type), tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->argNames = *(yyvsp[-2].pNameList);
            auto maxBits = (yyval.pTypeDecl)->maxBitfieldBits();
            if ( (yyval.pTypeDecl)->argNames.size()>maxBits ) {
                das_yyerror(scanner,"only " + to_string(maxBits) + " different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-5])),
                    CompilationError::exceeds_bitfield);
            }
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
            delete (yyvsp[-2].pNameList);
    }
    break;

  case 891: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.aTypePair).secondType->at = (yyval.aTypePair).firstType->at;
    }
    break;

  case 892: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 893: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = appendDimExpr(nullptr, (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 894: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = appendDimExpr(nullptr, nullptr, tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 895: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = appendDimExpr((yyvsp[-3].pTypeDecl), (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 896: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = appendDimExpr((yyvsp[-2].pTypeDecl), nullptr, tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 897: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 898: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
                                                                       {
        if ( (yyvsp[-1].pTypeDecl)->baseType==Type::typeDecl ) {
            das2_yyerror(scanner,"type declaration can`t be used as array base type",tokAt(scanner,(yylsp[-1])),
                CompilationError::invalid_array_type);
        } else if ( (yyvsp[-1].pTypeDecl)->baseType==Type::typeMacro ) {
            das2_yyerror(scanner,"macro can`t be used as array base type",tokAt(scanner,(yylsp[-1])),
                CompilationError::invalid_array_type);
        }
        (yyval.pTypeDecl) = attachDimChain((yyvsp[0].pTypeDecl), (yyvsp[-1].pTypeDecl));
    }
    break;

  case 899: /* optional_expr_list_in_braces: %empty  */
            { (yyval.pExpression) = nullptr; }
    break;

  case 900: /* optional_expr_list_in_braces: '(' expr_list optional_comma ')'  */
                                                { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 901: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc))); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 902: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 903: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 904: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 905: /* $@67: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 906: /* $@68: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 907: /* type_declaration_no_options_no_dim: "type" '<' $@67 type_declaration '>' $@68  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 908: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->typeMacroExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 909: /* type_declaration_no_options_no_dim: name_in_namespace '(' optional_expr_list ')'  */
                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->typeMacroExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 910: /* type_declaration_no_options_no_dim: '$' name_in_namespace optional_expr_list_in_braces  */
                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-1]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = sequenceToList((yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-1])), *(yyvsp[-1].s)));
        delete (yyvsp[-1].s);
    }
    break;

  case 911: /* $@69: %empty  */
                                    { yyextra->das_arrow_depth ++; }
    break;

  case 912: /* $@70: %empty  */
                                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 913: /* type_declaration_no_options_no_dim: name_in_namespace '<' $@69 type_declaration_no_options_list '>' $@70 optional_expr_list_in_braces  */
                                                                                                                                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-6]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-6])), *(yyvsp[-6].s)));
        delete (yyvsp[-6].s);
    }
    break;

  case 914: /* $@71: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 915: /* $@72: %empty  */
                                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 916: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@71 type_declaration_no_options_list '>' $@72 optional_expr_list_in_braces  */
                                                                                                                                                                                              {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-6]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-6])), *(yyvsp[-6].s)));
        delete (yyvsp[-6].s);
    }
    break;

  case 917: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 918: /* type_declaration_no_options_no_dim: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 919: /* type_declaration_no_options_no_dim: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 920: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 921: /* type_declaration_no_options_no_dim: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 922: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 923: /* type_declaration_no_options_no_dim: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 924: /* type_declaration_no_options_no_dim: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 925: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 926: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 927: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 928: /* type_declaration_no_options_no_dim: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 929: /* $@73: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 930: /* $@74: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 931: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@73 type_declaration '>' $@74  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 932: /* type_declaration_no_options_no_dim: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 933: /* $@75: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 934: /* $@76: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 935: /* type_declaration_no_options_no_dim: "array" '<' $@75 type_declaration '>' $@76  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 936: /* $@77: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 937: /* $@78: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 938: /* type_declaration_no_options_no_dim: "table" '<' $@77 table_type_pair '>' $@78  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 939: /* $@79: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 940: /* $@80: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 941: /* type_declaration_no_options_no_dim: "iterator" '<' $@79 type_declaration '>' $@80  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 942: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 943: /* $@81: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 944: /* $@82: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 945: /* type_declaration_no_options_no_dim: "block" '<' $@81 type_declaration '>' $@82  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 946: /* $@83: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 947: /* $@84: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 948: /* type_declaration_no_options_no_dim: "block" '<' $@83 optional_function_argument_list optional_function_type '>' $@84  */
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 949: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 950: /* $@85: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 951: /* $@86: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 952: /* type_declaration_no_options_no_dim: "function" '<' $@85 type_declaration '>' $@86  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 953: /* $@87: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 954: /* $@88: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 955: /* type_declaration_no_options_no_dim: "function" '<' $@87 optional_function_argument_list optional_function_type '>' $@88  */
                                                                                                                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 956: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 957: /* $@89: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 958: /* $@90: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 959: /* type_declaration_no_options_no_dim: "lambda" '<' $@89 type_declaration '>' $@90  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 960: /* $@91: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 961: /* $@92: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 962: /* type_declaration_no_options_no_dim: "lambda" '<' $@91 optional_function_argument_list optional_function_type '>' $@92  */
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 963: /* $@93: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 964: /* $@94: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 965: /* type_declaration_no_options_no_dim: "tuple" '<' $@93 tuple_type_list '>' $@94  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 966: /* $@95: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 967: /* $@96: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 968: /* type_declaration_no_options_no_dim: "variant" '<' $@95 variant_type_list '>' $@96  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 969: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 970: /* type_declaration: type_declaration '|' type_declaration_no_options  */
                                                                     {
        if ( (yyvsp[-2].pTypeDecl)->baseType==Type::option ) {
            (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[0].pTypeDecl));
        } else {
            (yyval.pTypeDecl) = new TypeDecl(Type::option, tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[-2].pTypeDecl));
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[0].pTypeDecl));
        }
    }
    break;

  case 971: /* type_declaration: type_declaration '|' '#'  */
                                             {
        if ( (yyvsp[-2].pTypeDecl)->baseType==Type::option ) {
            (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
            (yyval.pTypeDecl)->argTypes.push_back(new TypeDecl(*(yyvsp[-2].pTypeDecl)->argTypes.back()));
            (yyvsp[-2].pTypeDecl)->argTypes.back()->temporary ^= true;
        } else {
            (yyval.pTypeDecl) = new TypeDecl(Type::option, tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[-2].pTypeDecl));
            (yyval.pTypeDecl)->argTypes.push_back(new TypeDecl(*(yyvsp[-2].pTypeDecl)));
            (yyval.pTypeDecl)->argTypes.back()->temporary ^= true;
        }
    }
    break;

  case 972: /* $@97: %empty  */
                   {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 973: /* $@98: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 974: /* $@99: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 975: /* $@100: %empty  */
                                                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 976: /* tuple_alias_declaration: "tuple" $@97 optional_public_or_private_alias "name" optional_emit_semis $@98 '{' $@99 tuple_alias_type_list optional_semis $@100 '}'  */
          {
        auto vtype = new TypeDecl(Type::tTuple, tokAt(scanner,(yyloc)));
        vtype->alias = *(yyvsp[-8].s);
        vtype->at = tokAt(scanner,(yylsp[-8]));
        vtype->isPrivateAlias = !(yyvsp[-9].b);
        varDeclToTypeDecl(scanner, vtype, (yyvsp[-3].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-8].s),tokAt(scanner,(yylsp[-8])),
                CompilationError::already_declared_type_alias);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-8]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTuple((yyvsp[-8].s)->c_str(),atvname);
        }
        delete (yyvsp[-8].s);
    }
    break;

  case 977: /* $@101: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 978: /* $@102: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 979: /* $@103: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 980: /* $@104: %empty  */
                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 981: /* variant_alias_declaration: "variant" $@101 optional_public_or_private_alias "name" optional_emit_semis $@102 '{' $@103 variant_alias_type_list optional_semis $@104 '}'  */
          {
        auto vtype = new TypeDecl(Type::tVariant, tokAt(scanner,(yyloc)));
        vtype->alias = *(yyvsp[-8].s);
        vtype->at = tokAt(scanner,(yylsp[-8]));
        vtype->isPrivateAlias = !(yyvsp[-9].b);
        varDeclToTypeDecl(scanner, vtype, (yyvsp[-3].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-8].s),tokAt(scanner,(yylsp[-8])),
                CompilationError::already_declared_type_alias);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-8]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariant((yyvsp[-8].s)->c_str(),atvname);
        }
        delete (yyvsp[-8].s);
    }
    break;

  case 982: /* $@105: %empty  */
                      {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 983: /* $@106: %empty  */
                                                                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 984: /* $@107: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 985: /* $@108: %empty  */
                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 986: /* bitfield_alias_declaration: "bitfield" $@105 optional_public_or_private_alias "name" bitfield_basic_type_declaration optional_emit_commas $@106 '{' $@107 bitfield_alias_bits optional_commas $@108 '}'  */
          {
        auto btype = new TypeDecl((yyvsp[-8].type), tokAt(scanner,(yyloc)));
        btype->alias = *(yyvsp[-9].s);
        btype->at = tokAt(scanner,(yylsp[-9]));
        btype->isPrivateAlias = !(yyvsp[-10].b);
        for ( auto & p : *(yyvsp[-3].pNameExprList) ) {
            if ( !get<1>(p) ) {
                btype->argNames.push_back(get<0>(p));
            }
        }
        auto maxBits = btype->maxBitfieldBits();
        if ( btype->argNames.size()>maxBits ) {
            das_yyerror(scanner,"only " + to_string(maxBits) + " different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-9])),
                CompilationError::exceeds_bitfield);
        }
        for ( auto & p : *(yyvsp[-3].pNameExprList) ) {
            if ( get<1>(p) ) {
                ast_globalBitfieldConst ( scanner, btype, (yyvsp[-10].b), get<0>(p), get<1>(p) );
            }
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-9].s),tokAt(scanner,(yylsp[-9])),
                CompilationError::already_declared_type_alias);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-9]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfield((yyvsp[-9].s)->c_str(),atvname);
        }
        delete (yyvsp[-9].s);
        delete (yyvsp[-3].pNameExprList);
    }
    break;

  case 987: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 988: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 989: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 990: /* make_decl: make_table_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 991: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 992: /* make_decl: table_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 993: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 994: /* make_decl_no_bracket: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 995: /* make_decl_no_bracket: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 996: /* make_decl_no_bracket: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 997: /* make_decl_no_bracket: table_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 998: /* make_decl_no_bracket: make_table_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 999: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 1000: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 1001: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 1002: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 1003: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 1004: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 1005: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 1006: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 1007: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 1008: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 1009: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct(tokAt(scanner,(yyloc)));
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 1010: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct(tokAt(scanner,(yyloc)));
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 1011: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 1012: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct(tokAt(scanner,(yyloc)));
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 1013: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1014: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 1015: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 1016: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 1017: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 1018: /* $@109: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 1019: /* $@110: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 1020: /* make_struct_decl: "struct" '<' $@109 type_declaration_no_options '>' $@110 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yyloc));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1021: /* $@111: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 1022: /* $@112: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 1023: /* make_struct_decl: "class" '<' $@111 type_declaration_no_options '>' $@112 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yyloc));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1024: /* $@113: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 1025: /* $@114: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 1026: /* make_struct_decl: "variant" '<' $@113 variant_type_list '>' $@114 '(' use_initializer make_variant_dim ')'  */
                                                                                                                                                                                  {
        auto mkt = new TypeDecl(Type::tVariant, tokAt(scanner,(yylsp[-9])));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yyloc));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1027: /* $@115: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 1028: /* $@116: %empty  */
                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 1029: /* make_struct_decl: "variant" "type" '<' $@115 type_declaration_no_options '>' $@116 '(' use_initializer make_variant_dim ')'  */
                                                                                                                                                                                                    {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yyloc));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1030: /* $@117: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 1031: /* $@118: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 1032: /* make_struct_decl: "default" '<' $@117 type_declaration_no_options '>' $@118 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct(tokAt(scanner,(yyloc)));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 1033: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yyloc)));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pExpression) = mkt;
    }
    break;

  case 1034: /* $@119: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 1035: /* $@120: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 1036: /* make_tuple_call: "tuple" '<' $@119 tuple_type_list '>' $@120 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                 {
        auto mkt = new TypeDecl(Type::tTuple, tokAt(scanner,(yylsp[-9])));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yyloc));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1037: /* make_dim_decl: '[' optional_expr_list ']'  */
                                                  {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yyloc)));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
            mka->gen2 = true;
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yyloc)),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto mks = new ExprMakeStruct(tokAt(scanner,(yyloc)));
            mks->makeType = new TypeDecl(Type::tArray, mks->at);
            mks->makeType->firstType = new TypeDecl(Type::autoinfer, mks->at);
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }
    }
    break;

  case 1038: /* $@121: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 1039: /* $@122: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 1040: /* make_dim_decl: "array" "struct" '<' $@121 type_declaration_no_options '>' $@122 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yyloc));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yyloc)),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 1041: /* $@123: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 1042: /* $@124: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 1043: /* make_dim_decl: "array" "tuple" '<' $@123 tuple_type_list '>' $@124 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        auto mkt = new TypeDecl(Type::tTuple, tokAt(scanner,(yylsp[-10])));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yyloc));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yyloc)),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 1044: /* $@125: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 1045: /* $@126: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 1046: /* make_dim_decl: "array" "variant" '<' $@125 variant_type_list '>' $@126 '(' make_variant_dim ')'  */
                                                                                                                                                                      {
        auto mkt = new TypeDecl(Type::tVariant, tokAt(scanner,(yylsp[-9])));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-5].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-5].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yyloc));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yyloc)),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 1047: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = new ExprMakeArray(tokAt(scanner,(yyloc)));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        mka->gen2 = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yyloc)),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 1048: /* $@127: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 1049: /* $@128: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 1050: /* make_dim_decl: "array" '<' $@127 type_declaration_no_options '>' $@128 '(' optional_expr_list ')'  */
                                                                                                                                                                        {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yyloc)));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = (yyvsp[-5].pTypeDecl);
            mka->gen2 = true;
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yyloc)),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto msd = new ExprMakeStruct(tokAt(scanner,(yyloc)));
            msd->makeType = new TypeDecl(Type::tArray, msd->at);
            msd->makeType->firstType = (yyvsp[-5].pTypeDecl);
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 1051: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yyloc)));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 1052: /* $@129: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 1053: /* $@130: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 1054: /* make_dim_decl: "fixed_array" '<' $@129 type_declaration_no_options '>' $@130 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yyloc)));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 1055: /* expr_map_tuple_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 1056: /* expr_map_tuple_list: expr_map_tuple_list ',' expr  */
                                                      {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yyloc)),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 1057: /* push_table_nesting: %empty  */
                    {
        yyextra->das_nested_parentheses ++;
    }
    break;

  case 1058: /* make_table_decl: '{' push_table_nesting optional_emit_semis optional_expr_map_tuple_list '}'  */
                                                                                                     {
        yyextra->das_nested_parentheses --;
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yyloc)));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yyloc)),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto mks = new ExprMakeStruct(tokAt(scanner,(yyloc)));
            mks->makeType = new TypeDecl(Type::tTable, tokAt(scanner,(yyloc)));
            mks->makeType->firstType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-4])));
            mks->makeType->secondType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[0])));
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }
    }
    break;

  case 1059: /* make_table_call: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = new ExprMakeArray(tokAt(scanner,(yyloc)));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yyloc)),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 1060: /* make_table_call: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                 {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yyloc)));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = (yyvsp[-4].pTypeDecl);
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yyloc)),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct(tokAt(scanner,(yyloc)));
            msd->makeType = new TypeDecl(Type::tTable, msd->at);
            msd->makeType->firstType = (yyvsp[-4].pTypeDecl);
            msd->makeType->secondType = new TypeDecl(Type::tVoid, tokAt(scanner,(yylsp[-6])));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 1061: /* make_table_call: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                                                             {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yyloc)));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = new TypeDecl(Type::tTuple, tokAt(scanner,(yyloc)));
            mka->makeType->argTypes.push_back((yyvsp[-6].pTypeDecl));
            mka->makeType->argTypes.push_back((yyvsp[-4].pTypeDecl));
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yyloc)),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct(tokAt(scanner,(yyloc)));
            msd->makeType = new TypeDecl(Type::tTable, msd->at);
            msd->makeType->firstType = (yyvsp[-6].pTypeDecl);
            msd->makeType->secondType = (yyvsp[-4].pTypeDecl);
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 1062: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 1063: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 1064: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 1065: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 1066: /* table_comprehension: '[' "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                                   {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yyloc)),tokRangeAt(scanner,(yylsp[-9]),(yylsp[-4])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 1067: /* table_comprehension: '[' "iterator" "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                                                {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yyloc)),tokRangeAt(scanner,(yylsp[-9]),(yylsp[-4])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 1068: /* array_comprehension: '{' push_table_nesting optional_emit_semis "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where '}'  */
                                                                                                                                                                                                          {
        yyextra->das_nested_parentheses --;
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yyloc)),tokRangeAt(scanner,(yylsp[-9]),(yylsp[-4])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;



      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == DAS2_YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, scanner, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= DAS2_YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == DAS2_YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, scanner);
          yychar = DAS2_YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != DAS2_YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}



void das2_yyfatalerror ( DAS2_YYLTYPE * lloc, yyscan_t scanner, const string & error, CompilationError cerr ) {
    yyextra->g_Program->error(error,"","",LineInfo(yyextra->g_FileAccessStack.back(),
        lloc->first_column,lloc->first_line,lloc->last_column,lloc->last_line),cerr);
}

// a keyword where the grammar wanted a name is the usual shape behind "unexpected <word>, expecting ... name":
// the word is bison's token text, so the note names it as the reserved word it is
static string das2_reservedWordNote ( const string & error ) {
    const char * prefix = "unexpected ";
    auto at = error.find(prefix);
    if ( at==string::npos ) return "";
    auto expecting = error.find("expecting", at);
    if ( expecting==string::npos ) return "";
    if ( error.find(" name", expecting)==string::npos ) return "";   // the expectation list wanted a name
    auto from = at + strlen(prefix);
    auto to = error.find(',', from);
    string tok = error.substr(from, to==string::npos ? string::npos : to-from);
    if ( tok.empty() || tok=="name" ) return "";   // "unexpected name" is an identifier out of place, not a keyword
    for ( char c : tok ) {   // "end of file", "$end", "'('", "integer constant" are not words; int2, float4, uint16 are
        if ( !((c>='a' && c<='z') || (c>='0' && c<='9') || c=='_') ) return "";
    }
    return " - '" + tok + "' is a reserved word and cannot be used as a name";
}

void das2_yyerror ( DAS2_YYLTYPE * lloc, yyscan_t scanner, const string & error ) {
    if ( !yyextra->das_suppress_errors ) {
        yyextra->g_Program->error(error + das2_reservedWordNote(error),"","",LineInfo(yyextra->g_FileAccessStack.back(),
            lloc->first_column,lloc->first_line,lloc->last_column,lloc->last_line),
                CompilationError::invalid_expression);
    }
}

LineInfo tokAt ( yyscan_t scanner, const struct DAS2_YYLTYPE & li ) {
    return LineInfo(yyextra->g_FileAccessStack.back(),
        li.first_column,li.first_line,
        li.last_column,li.last_line);
}

LineInfo tokRangeAt ( yyscan_t scanner, const struct DAS2_YYLTYPE & li, const struct DAS2_YYLTYPE & lie ) {
    return LineInfo(yyextra->g_FileAccessStack.back(),
        li.first_column,li.first_line,
        lie.last_column,lie.last_line);
}



