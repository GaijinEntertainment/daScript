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
    void das2_accept_sequence ( yyscan_t yyscanner, const char * seq, size_t seqLen, int lineNo, FileInfo * info );
    void das2_strfmt ( yyscan_t yyscanner );

    namespace das { class Module; }
    void das2_collect_keywords ( das::Module * mod, yyscan_t yyscanner );

    #undef yyextra
    #define yyextra (*((das::DasParserState **)(scanner)))


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
  YYSYMBOL_type_declaration = 523,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 524,  /* tuple_alias_declaration  */
  YYSYMBOL_525_95 = 525,                   /* $@95  */
  YYSYMBOL_526_96 = 526,                   /* $@96  */
  YYSYMBOL_527_97 = 527,                   /* $@97  */
  YYSYMBOL_528_98 = 528,                   /* $@98  */
  YYSYMBOL_variant_alias_declaration = 529, /* variant_alias_declaration  */
  YYSYMBOL_530_99 = 530,                   /* $@99  */
  YYSYMBOL_531_100 = 531,                  /* $@100  */
  YYSYMBOL_532_101 = 532,                  /* $@101  */
  YYSYMBOL_533_102 = 533,                  /* $@102  */
  YYSYMBOL_bitfield_alias_declaration = 534, /* bitfield_alias_declaration  */
  YYSYMBOL_535_103 = 535,                  /* $@103  */
  YYSYMBOL_536_104 = 536,                  /* $@104  */
  YYSYMBOL_537_105 = 537,                  /* $@105  */
  YYSYMBOL_538_106 = 538,                  /* $@106  */
  YYSYMBOL_make_decl = 539,                /* make_decl  */
  YYSYMBOL_make_decl_no_bracket = 540,     /* make_decl_no_bracket  */
  YYSYMBOL_make_struct_fields = 541,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 542,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 543,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim_list = 544,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 545,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 546, /* optional_make_struct_dim_decl  */
  YYSYMBOL_use_initializer = 547,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 548,         /* make_struct_decl  */
  YYSYMBOL_549_107 = 549,                  /* $@107  */
  YYSYMBOL_550_108 = 550,                  /* $@108  */
  YYSYMBOL_551_109 = 551,                  /* $@109  */
  YYSYMBOL_552_110 = 552,                  /* $@110  */
  YYSYMBOL_553_111 = 553,                  /* $@111  */
  YYSYMBOL_554_112 = 554,                  /* $@112  */
  YYSYMBOL_555_113 = 555,                  /* $@113  */
  YYSYMBOL_556_114 = 556,                  /* $@114  */
  YYSYMBOL_557_115 = 557,                  /* $@115  */
  YYSYMBOL_558_116 = 558,                  /* $@116  */
  YYSYMBOL_make_tuple_call = 559,          /* make_tuple_call  */
  YYSYMBOL_560_117 = 560,                  /* $@117  */
  YYSYMBOL_561_118 = 561,                  /* $@118  */
  YYSYMBOL_make_dim_decl = 562,            /* make_dim_decl  */
  YYSYMBOL_563_119 = 563,                  /* $@119  */
  YYSYMBOL_564_120 = 564,                  /* $@120  */
  YYSYMBOL_565_121 = 565,                  /* $@121  */
  YYSYMBOL_566_122 = 566,                  /* $@122  */
  YYSYMBOL_567_123 = 567,                  /* $@123  */
  YYSYMBOL_568_124 = 568,                  /* $@124  */
  YYSYMBOL_569_125 = 569,                  /* $@125  */
  YYSYMBOL_570_126 = 570,                  /* $@126  */
  YYSYMBOL_571_127 = 571,                  /* $@127  */
  YYSYMBOL_572_128 = 572,                  /* $@128  */
  YYSYMBOL_expr_map_tuple_list = 573,      /* expr_map_tuple_list  */
  YYSYMBOL_push_table_nesting = 574,       /* push_table_nesting  */
  YYSYMBOL_make_table_decl = 575,          /* make_table_decl  */
  YYSYMBOL_make_table_call = 576,          /* make_table_call  */
  YYSYMBOL_array_comprehension_where = 577, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 578,           /* optional_comma  */
  YYSYMBOL_table_comprehension = 579,      /* table_comprehension  */
  YYSYMBOL_array_comprehension = 580       /* array_comprehension  */
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
#define YYLAST   12905

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  246
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  335
/* YYNRULES -- Number of rules.  */
#define YYNRULES  1065
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1893

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
       0,   606,   606,   607,   612,   613,   614,   615,   616,   617,
     618,   619,   620,   621,   622,   623,   624,   628,   629,   633,
     634,   638,   644,   645,   646,   650,   651,   655,   656,   660,
     679,   680,   681,   682,   686,   687,   691,   692,   696,   697,
     697,   701,   706,   715,   730,   746,   751,   759,   759,   804,
     822,   826,   829,   833,   837,   841,   845,   851,   860,   861,
     865,   868,   871,   877,   878,   882,   886,   887,   891,   894,
     900,   906,   909,   915,   916,   920,   921,   925,   926,   930,
     931,   931,   935,   935,   944,   945,   949,   950,   956,   957,
     958,   959,   960,   964,   965,   969,   970,   974,   976,   974,
     988,   988,   996,   998,   996,  1010,  1010,  1018,  1020,  1018,
    1031,  1038,  1045,  1050,  1059,  1067,  1073,  1077,  1085,  1095,
    1095,  1104,  1112,  1112,  1128,  1134,  1141,  1160,  1164,  1171,
    1172,  1173,  1174,  1175,  1176,  1177,  1178,  1179,  1180,  1181,
    1182,  1186,  1191,  1199,  1200,  1201,  1202,  1206,  1207,  1208,
    1209,  1210,  1211,  1212,  1213,  1214,  1215,  1216,  1217,  1218,
    1219,  1220,  1226,  1229,  1235,  1238,  1244,  1247,  1250,  1256,
    1257,  1258,  1259,  1263,  1281,  1304,  1307,  1317,  1332,  1347,
    1362,  1365,  1372,  1376,  1383,  1384,  1388,  1389,  1393,  1394,
    1395,  1399,  1403,  1407,  1414,  1418,  1419,  1420,  1421,  1422,
    1423,  1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,
    1433,  1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,
    1443,  1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,  1452,
    1453,  1454,  1455,  1456,  1457,  1458,  1459,  1460,  1461,  1462,
    1463,  1464,  1465,  1466,  1467,  1468,  1469,  1470,  1471,  1472,
    1473,  1474,  1475,  1476,  1477,  1478,  1479,  1480,  1481,  1482,
    1483,  1484,  1485,  1486,  1487,  1488,  1489,  1490,  1491,  1492,
    1493,  1494,  1495,  1496,  1497,  1498,  1499,  1500,  1501,  1502,
    1503,  1504,  1505,  1506,  1507,  1511,  1512,  1513,  1514,  1515,
    1516,  1517,  1518,  1519,  1520,  1521,  1522,  1523,  1524,  1525,
    1526,  1527,  1528,  1529,  1530,  1531,  1532,  1533,  1534,  1535,
    1536,  1537,  1538,  1539,  1540,  1541,  1542,  1543,  1544,  1545,
    1546,  1547,  1548,  1549,  1550,  1551,  1552,  1553,  1554,  1555,
    1556,  1557,  1558,  1562,  1563,  1567,  1586,  1587,  1588,  1592,
    1598,  1598,  1615,  1618,  1620,  1618,  1632,  1634,  1632,  1649,
    1667,  1685,  1703,  1714,  1715,  1716,  1717,  1718,  1719,  1720,
    1721,  1722,  1723,  1724,  1725,  1726,  1727,  1728,  1729,  1730,
    1731,  1732,  1733,  1735,  1733,  1750,  1755,  1761,  1767,  1768,
    1772,  1773,  1777,  1781,  1788,  1789,  1800,  1804,  1807,  1815,
    1815,  1815,  1818,  1824,  1827,  1831,  1835,  1842,  1849,  1855,
    1859,  1863,  1866,  1869,  1877,  1880,  1888,  1894,  1895,  1896,
    1900,  1901,  1905,  1906,  1910,  1915,  1923,  1930,  1942,  1946,
    1949,  1959,  1959,  1959,  1962,  1962,  1962,  1967,  1967,  1967,
    1975,  1975,  1975,  1981,  1991,  2002,  2017,  2020,  2023,  2026,
    2032,  2033,  2041,  2053,  2054,  2055,  2059,  2060,  2061,  2062,
    2063,  2067,  2072,  2080,  2081,  2085,  2092,  2096,  2103,  2104,
    2105,  2106,  2107,  2108,  2109,  2110,  2111,  2115,  2116,  2117,
    2118,  2119,  2120,  2121,  2122,  2123,  2124,  2125,  2126,  2127,
    2128,  2129,  2130,  2131,  2132,  2133,  2134,  2135,  2136,  2137,
    2138,  2139,  2140,  2144,  2150,  2157,  2169,  2175,  2183,  2191,
    2202,  2214,  2218,  2225,  2228,  2228,  2228,  2233,  2233,  2233,
    2246,  2250,  2254,  2258,  2264,  2272,  2278,  2286,  2294,  2305,
    2314,  2320,  2328,  2328,  2328,  2335,  2339,  2348,  2356,  2364,
    2368,  2371,  2379,  2380,  2381,  2388,  2389,  2390,  2391,  2392,
    2393,  2394,  2395,  2396,  2397,  2398,  2399,  2400,  2401,  2402,
    2403,  2404,  2405,  2406,  2407,  2408,  2409,  2410,  2411,  2412,
    2413,  2414,  2415,  2416,  2417,  2418,  2419,  2420,  2421,  2422,
    2423,  2429,  2430,  2431,  2432,  2433,  2448,  2457,  2458,  2459,
    2460,  2461,  2462,  2463,  2464,  2465,  2466,  2467,  2468,  2469,
    2470,  2471,  2471,  2471,  2479,  2480,  2481,  2486,  2489,  2489,
    2489,  2492,  2497,  2501,  2501,  2501,  2506,  2513,  2519,  2523,
    2523,  2523,  2528,  2531,  2537,  2537,  2537,  2544,  2549,  2553,
    2553,  2553,  2558,  2561,  2567,  2567,  2567,  2574,  2579,  2580,
    2581,  2582,  2583,  2584,  2585,  2586,  2587,  2589,  2593,  2594,
    2599,  2605,  2611,  2620,  2623,  2626,  2635,  2636,  2637,  2638,
    2639,  2640,  2641,  2645,  2649,  2653,  2657,  2661,  2665,  2669,
    2673,  2677,  2682,  2686,  2691,  2695,  2700,  2707,  2708,  2712,
    2713,  2714,  2718,  2719,  2723,  2724,  2725,  2729,  2730,  2734,
    2746,  2749,  2750,  2754,  2754,  2773,  2772,  2787,  2786,  2803,
    2815,  2824,  2834,  2835,  2836,  2837,  2838,  2842,  2845,  2854,
    2855,  2859,  2862,  2866,  2879,  2888,  2889,  2893,  2896,  2900,
    2913,  2914,  2918,  2923,  2928,  2936,  2939,  2946,  2949,  2955,
    2956,  2957,  2961,  2962,  2966,  2973,  2978,  2987,  2993,  2997,
    3008,  3015,  3024,  3027,  3030,  3037,  3041,  3047,  3058,  3061,
    3066,  3077,  3078,  3082,  3083,  3084,  3088,  3091,  3094,  3094,
    3114,  3117,  3117,  3135,  3140,  3148,  3149,  3153,  3156,  3169,
    3186,  3187,  3188,  3193,  3193,  3219,  3220,  3227,  3227,  3249,
    3250,  3251,  3255,  3265,  3268,  3274,  3275,  3279,  3280,  3284,
    3285,  3289,  3291,  3296,  3289,  3312,  3313,  3317,  3318,  3322,
    3328,  3329,  3330,  3331,  3335,  3336,  3337,  3341,  3344,  3350,
    3352,  3357,  3350,  3378,  3385,  3390,  3399,  3405,  3409,  3420,
    3421,  3422,  3423,  3424,  3425,  3426,  3427,  3428,  3429,  3430,
    3431,  3432,  3433,  3434,  3435,  3436,  3437,  3438,  3439,  3440,
    3441,  3442,  3443,  3444,  3445,  3446,  3447,  3448,  3449,  3450,
    3451,  3452,  3453,  3454,  3455,  3456,  3457,  3458,  3459,  3460,
    3461,  3462,  3463,  3464,  3465,  3466,  3467,  3468,  3469,  3473,
    3474,  3475,  3476,  3477,  3478,  3479,  3480,  3484,  3495,  3499,
    3506,  3518,  3525,  3531,  3540,  3545,  3555,  3565,  3575,  3588,
    3589,  3590,  3591,  3592,  3596,  3600,  3600,  3600,  3614,  3615,
    3619,  3624,  3631,  3634,  3637,  3640,  3646,  3649,  3663,  3664,
    3668,  3669,  3670,  3671,  3672,  3672,  3672,  3676,  3681,  3688,
    3695,  3695,  3702,  3702,  3709,  3713,  3717,  3722,  3727,  3732,
    3737,  3741,  3745,  3750,  3754,  3758,  3763,  3763,  3763,  3769,
    3776,  3776,  3776,  3781,  3781,  3781,  3787,  3787,  3787,  3792,
    3798,  3798,  3798,  3803,  3803,  3803,  3812,  3818,  3818,  3818,
    3823,  3823,  3823,  3832,  3838,  3838,  3838,  3843,  3843,  3843,
    3852,  3852,  3852,  3858,  3858,  3858,  3867,  3870,  3881,  3897,
    3899,  3904,  3909,  3897,  3935,  3937,  3942,  3948,  3935,  3974,
    3976,  3981,  3986,  3974,  4027,  4028,  4029,  4030,  4031,  4032,
    4033,  4037,  4038,  4039,  4040,  4041,  4045,  4052,  4059,  4065,
    4071,  4078,  4085,  4091,  4100,  4103,  4109,  4117,  4122,  4129,
    4134,  4140,  4141,  4145,  4146,  4150,  4150,  4150,  4158,  4158,
    4158,  4165,  4165,  4165,  4175,  4175,  4175,  4182,  4182,  4182,
    4193,  4199,  4199,  4199,  4212,  4231,  4231,  4231,  4241,  4241,
    4241,  4254,  4254,  4254,  4267,  4276,  4276,  4276,  4296,  4303,
    4303,  4303,  4313,  4316,  4327,  4333,  4356,  4364,  4384,  4409,
    4410,  4414,  4415,  4420,  4423,  4433
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
  "$@94", "type_declaration", "tuple_alias_declaration", "$@95", "$@96",
  "$@97", "$@98", "variant_alias_declaration", "$@99", "$@100", "$@101",
  "$@102", "bitfield_alias_declaration", "$@103", "$@104", "$@105",
  "$@106", "make_decl", "make_decl_no_bracket", "make_struct_fields",
  "make_variant_dim", "make_struct_single", "make_struct_dim_list",
  "make_struct_dim_decl", "optional_make_struct_dim_decl",
  "use_initializer", "make_struct_decl", "$@107", "$@108", "$@109",
  "$@110", "$@111", "$@112", "$@113", "$@114", "$@115", "$@116",
  "make_tuple_call", "$@117", "$@118", "make_dim_decl", "$@119", "$@120",
  "$@121", "$@122", "$@123", "$@124", "$@125", "$@126", "$@127", "$@128",
  "expr_map_tuple_list", "push_table_nesting", "make_table_decl",
  "make_table_call", "array_comprehension_where", "optional_comma",
  "table_comprehension", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1711)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-958)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1711,    79, -1711, -1711,    97,   -44,    -6,   732, -1711,   -91,
   -1711, -1711, -1711, -1711,  -110,   199, -1711, -1711, -1711, -1711,
      65,    65,    65, -1711,   191, -1711,   225, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,   100, -1711,
     114,   130,   171, -1711,   174,   189, -1711, -1711, -1711,   205,
      65,    65, -1711, -1711,   225,   732,   732,   732,   237,   270,
   -1711, -1711, -1711, -1711,   199,   199,   199,   258, -1711,   876,
     215, -1711, -1711, -1711, -1711,   333, -1711,   851, -1711,   762,
      56,    97,   350,   -44,   335, -1711,   293,   379,   313,   430,
      21,   438, -1711, -1711,   845,   453,   472,   501, -1711,   514,
     515, -1711, -1711,     3,    97,   199,   199,   199,   199,   541,
   -1711,   853,   863,   640,   647,   865, -1711, -1711,   542, -1711,
   -1711,   557, -1711, -1711, -1711, -1711, -1711,   874,   686,   126,
   -1711, -1711, -1711, -1711,   174,   174,   565,   174,   637,   685,
     657,   710, -1711, -1711, -1711,   715, -1711, -1711,   702,   704,
     541,   541, -1711, -1711,   735, -1711,    63, -1711,   698,   760,
     876, -1711,   769, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
     776, -1711, -1711, -1711, -1711, -1711, -1711,   828, -1711, -1711,
   -1711, -1711,   888, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711,   721, -1711, -1711,    67,   565,   565,
     565, -1711, -1711,   929,   929,   798, 11182,   947, -1711,   712,
     841, -1711, -1711, -1711, -1711, -1711, 12382, -1711,   805,   932,
     173,    97,   890,   886, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711,   126, -1711, -1711, -1711, 11182,   867,   870,   872,   861,
     873,   882, -1711, -1711, -1711,   877, -1711, -1711, -1711, -1711,
   -1711,   719, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711,   892, -1711, -1711, -1711,   900,   901, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711,   902,   903,   885,  -110,
    -104, -1711, -1711, -1711, -1711,   344,   891,   887, -1711, -1711,
   -1711, -1711, -1711, -1711,   906, -1711,   879,   881, 11354, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711,  1091,  1102, -1711,
     904, -1711,   541,   864,   841, -1711,   937,   541, -1711, -1711,
     828,   541,    97, -1711,   326, -1711, -1711,   887, -1711, -1711,
   -1711,  9365, -1711, -1711,   938,   912,   -83,   -49,   -22, -1711,
   -1711,  9365,   -95, -1711,  6846, -1711, -1711, -1711,    11, -1711,
   -1711, -1711,    47, -1711,  7075,   905, 10686, -1711,   909, -1711,
   -1711, 12595, 12649, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711,   944,   911, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,  1127,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711,   952,   917, -1711, -1711,   -66,   -81,   -89, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,   914,   955,
   -1711,   283, -1711,   541,   969, 11182, -1711,   -10, 11182, 11182,
   11182,   960,   961, -1711, -1711,    57,  -110,   962,    35, -1711,
     -18,   945,   964,   966,   202,   968,   949,   311,   971, -1711,
     470,    48,   972, 10502, 10502,   381,   953,   954,   957,   958,
     967,   976, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711,  9821, 10502, 10502, 10502, 10502,  4327,  5243, -1711,
   -1711, -1711, -1711, -1711, -1711,   977, -1711, -1711, -1711, -1711,
     963, -1711, -1711,   -45,   -45, -1711,   -45,   -45,   965, 10687,
   -1711, -1711,   978, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, 11182, 11182,   979,   974, 11182,   904, 11182,   904, 11182,
     904, 11353,   999,   981, -1711,  6846, -1711, 11182,  9365,   986,
     995, -1711, -1711, -1711, -1711, -1711,   989, -1711, -1711,   990,
    7304, -1711,   344, -1711, 11353,   999, -1711, -1711, -1711, -1711,
   -1711, -1711, 12704,  1030,  1888,   991, -1711,    82,   988,   -21,
     992, 11182, 11182, -1711, 10048, -1711, -1711, -1711, -1711,  -110,
   -1711,   713,   994,  1190,   689, -1711, -1711, -1711,   368, -1711,
   -1711, -1711,  9365,   706,   723,  1015,   526, -1711, -1711, -1711,
   -1711,  1003, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711,   480, -1711,  1027,  1029,  1031, -1711,  6846, 11182,  9365,
    9365, -1711, -1711, -1711,  9365, -1711,  9365, -1711,  6846, -1711,
   -1711,  6846,  1032, -1711, 11182,   196,   196,  1010,  1013,    85,
   -1711, -1711,  9365,  9365,  9365,  9365,  9365,  9365,   852, -1711,
     196,   196,   507,   196,   196,  1014,  1240,  1017,  1019,   243,
     995,  1047,  1020,   541,  3869,   199,  1256, -1711, -1711,   963,
   -1711, -1711, -1711, -1711, -1711,  2210, 11953, 10502, 10502, -1711,
   -1711, 10502, 10502, 10502, 10502,  1060, 10502,   287,  9365,   439,
     607,  9365,  9365, 10502, 12008, 12139, 12194, 10502, 10502, 10502,
   10502,  9365, 10502, 10502, 10502, 10502, 10275, 10502, 10502, 10502,
   10502, 10502, 10502, 10502, 10502, 10502, 10502, 12456,  9365,  5472,
     731,   756, -1711, -1711,  1061,   779,   -81,   790,   -81,   796,
     -81,   106, -1711,   395,   887,  1049, -1711,   464, -1711, 11182,
     995,   519,   887, -1711, -1711,  7533, -1711, -1711, -1711, -1711,
    1037,  1065, -1711,    65, -1711,    65, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711,  9365, -1711, -1711,   431,    27,    27,
      27, -1711,   887,   887, 10502, 11459, -1711,  1076, -1711, -1711,
   -1711, -1711,  9365,  1078,   561, 11182,   -10, -1711,  9365,    65,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, 11182, 11182, 11182,
   11182,  4556,  1082,  9365, 11182, -1711, -1711, -1711, 11182,   995,
     494, -1711,  1075,  1048, 11182, 11182, 11182,  1052, 11182,  1053,
   11182,   995, 11182, 11353,   995, -1711,   999,   465,  9365,  9365,
   11182,   904,  1054,  1055,  1058,  1059,  1063,  1064, -1711,  9365,
     794,   -94,  1057, -1711,  9365, -1711,  9365, -1711,  9365,  1066,
     615, -1711, -1711,  7762,   307,  4098, -1711,    77,  1067,   267,
    1062,   904,  2409,  1256,  1070,  1069, -1711, -1711,  1071,  1072,
   -1711, -1711, 11696, 11696, 11787, 11787,   827,   827,  1079,  1348,
    1080, -1711,  1083,  1081,  1084,  1085, -1711,  1087,  1088,  1348,
    1086,  1097, -1711, -1711,  1098,  1099, -1711, -1711,  1103,  1101,
   -1711, -1711,   316,   316,   978, 11696, 11696, 11459, -1711, -1711,
    1773, 11649,  1410, 11459, 12325,  2043,  1941, 11740, 11177, 11787,
   11787,  1657,  1657,  1348,  1348,  1348,   661,  9365,  1106,  1107,
     668,  9365,  1289,  1129,  1089, -1711,   155, -1711, -1711, -1711,
     238, -1711,  1090, -1711,  1094, -1711,  1110, 11182, -1711, 11353,
   11182, -1711,   999,   573,  1132,  1130, 11182,  9365, -1711, -1711,
    1160,   621, -1711, 11009, -1711,   -79, -1711,  1134,  1136,  1298,
   -1711, -1711,   167, -1711, -1711, -1711, 11558,  2653,  1163, -1711,
     621,    33,  1137, -1711,  1140,  1336,   368,  9365,    65, -1711,
   -1711, -1711, -1711,   887,   506,   609,   803,   687,   169,  1141,
    1142,   579,  1143,   806, 11182, 11353,   999,   784,  1145,  1148,
   11182,  9365,  1149, -1711,   884,   927,  1051, -1711,  1056, -1711,
    1095,  1151,  1195,   585,  1152, 11182,   599,  1256,  1154,  1155,
    1377,   -81, -1711, -1711, -1711, -1711, -1711,  1161,  1173,  1164,
    1360,  1203,    26,   -94,  1166, -1711, -1711, -1711,  1168,     1,
    1169,  1165,  1075,   129, -1711,  1174,   342,  5701, -1711, -1711,
   -1711,   374,   -81, -1711,  7991, -1711,  1175,  8220,  1218,  1219,
   -1711,    65,  1229,  8449,   132,  8678, -1711, -1711, -1711,    65,
      65,  1412, -1711,  1016, -1711, -1711,  1286, -1711, -1711,  1414,
    1382, -1711,    65, -1711,    65,    65,    65,    65,    65, -1711,
    1359, -1711,    65, 10803,   904, -1711,  9365, -1711,  9365,  4785,
    9365, -1711,  9365,  5930,  9365, -1711, -1711, -1711,  9365, -1711,
    9365, -1711,  9365,  1208,  1189, -1711, -1711, 10502,  1191, -1711,
    1193,  9365,  5014,  1196, -1711,  1198, -1711,  6159, -1711,  7533,
   -1711, -1711, -1711,  1232, -1711,  1235, -1711, -1711, -1711, -1711,
   -1711, -1711,   887, -1711, -1711,   887, -1711, -1711,  1130, -1711,
   -1711,   887, -1711,  9365, -1711,   406, -1711, -1711, -1711,  1199,
   -1711,  1201, -1711,  9365,  1238,   619, 11182, -1711,  9365,  1202,
    9365,   690, -1711,  1246, -1711, -1711,  1440,   828, -1711,  9365,
    1248, -1711,  9365,    65, -1711, -1711, -1711, -1711,  1211, -1711,
   -1711, -1711,  1215,  1253, -1711, -1711,  1448,   628,   631, -1711,
   -1711,  9365,  1547, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711,  1558, -1711,   209, -1711, -1711, -1711,
    1237,  6388, -1711,  1251,  9365,  1261, -1711,   175,  6846,    96,
      40,   161,  9365,  9365,  9365,   -94, -1711, -1711, -1711,   615,
    1224,  4098,   235,  1263,  1264,  1230,  1266,  1268, -1711,   292,
     541,  9365, -1711,  1457,  9365, -1711,  1262,  1265, -1711,  1259,
    1283, -1711,  1175,  9365, -1711, -1711, -1711, -1711,  1241, -1711,
   -1711,  1242,   -62,   -62,  1244, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711,   -76, -1711, 10502, 10502, 10502, 10502, 10502, 10502,
   10502, 10502, 10502, 10502,  9365, 10502,  9365, 10502, 10502, 10502,
   10502, 10502, 10502, 10502, 10502,   -81, 11182,  1239, 11182,  1245,
    4098, -1711,   299,   321,  1247,  1249, -1711,   334,  1252, 11182,
    1254, 11182,  1255, 11182,  1257, -1711,  9365, 11558,  9365, -1711,
    1276,  4098, -1711,   348,   351,  9365, -1711, -1711, -1711,   372,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,  1271, -1711,
    1243, -1711, -1711,  1277, -1711,  1260,  1433,   121, -1711,  1455,
   -1711, -1711,  1273,  1306,   866,  1418,    65,  1280, -1711,    65,
   -1711,  1282,  1284, -1711, -1711,  9365,  1302, -1711, -1711, -1711,
   -1711,  1285,  1287,  1291,  1292,  1293,  1295,  1296,  1297,  1425,
    1299, -1711,  1300,  8907,  1175,   516, -1711, -1711,   410, -1711,
   -1711,  1288, -1711,  1335, -1711,   411,  1301,  1498,  1203,  6846,
    9365,  9365,  1303, -1711, -1711,    50, -1711,   466, -1711, -1711,
   -1711,  1342, -1711, -1711,   374, -1711,   -89, -1711,  1175, -1711,
   11182,  9365, -1711, -1711, -1711, -1711,  2897,  9365,  9365,    97,
     890,  1307,  1308,  9136,  1203, -1711, -1711,   283, 10687, 10687,
   10687, 10687, 10687, 10687, 10687, 10687, 10687, 10687, 10687, -1711,
   -1711, 10687, 10687, -1711, -1711, 10687, 10687, 10687, 10687, 10687,
   10687, 10687, 10687,   541,  1621, -1711,   809, -1711,   498,  6617,
   -1711, -1711, -1711, -1711, -1711, -1711,  1678, -1711,   817, -1711,
     818, -1711, 11182,  1309,  1315, -1711,   522,  6617, -1711, -1711,
    1316, -1711,  9365, -1711, -1711,  9365,  1346,  9365, -1711, -1711,
   -1711, 11182, -1711, -1711,   707, -1711,    18, -1711, -1711, -1711,
    1425,  1425,  1321,  1333,  1343,  1347,  1349,  6846, -1711,  9365,
    9365,  9365,  9365,  9365,  6846, -1711, -1711,  1425,  1351,  1425,
   -1711,  1353, -1711, -1711,   516, -1711,  1388, -1711, -1711,  1356,
    9365,  1400,   433,   437, -1711, -1711,   252,  6846,  1363,  1364,
   -1711, -1711, -1711,   887, -1711,  1361,  1365,  1367,   529,   -94,
    9365,   174,  1370,   441,   278, -1711,   -89, -1711, -1711,  1371,
     447, -1711, -1711, -1711,   819, -1711, -1711,  1372,   488, -1711,
   -1711,  1373, -1711, -1711,  1374,    98,  1580,    18, -1711, -1711,
     866,   221,   221, -1711,  9365,  1425,  1425,   687,  1375,  1376,
    1381,  1396,  1397,  1401,   995,   221,  1425,   687, -1711, -1711,
   -1711,  9365,  1402, -1711, -1711,  1406,  9365,  9365,   493, -1711,
   -1711,  1455,  1604,   541, -1711,    61,  1403,   620,   541,    -2,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,  1580,
     431,   687,  1428,  1432, -1711,  1405,  1411,  1413,   221,   221,
    1428,  1417, -1711, -1711, -1711, -1711, -1711, -1711,  1419,  1420,
     687,  1421,  1407, -1711,  9365, -1711, -1711,  1409, -1711,  9594,
      65, -1711,  6846,   541,   541,  1175, 11182,   -10, -1711,  3141,
   12382, -1711, -1711, -1711, -1711,   513,  1415, -1711, -1711, -1711,
   -1711,  1423,  1427, -1711, -1711, -1711,  1430, -1711,  1611,  1434,
    1407,  9365, -1711, -1711, -1711, -1711, -1711, 10687, -1711,  1429,
     539,  1175,  1175, -1711,   383,  9365,  1431,    65, 12382, -1711,
     687, -1711, -1711, -1711,  9365, -1711,  1436,  1407, -1711,   812,
    9594,   541, -1711, -1711,  9365,    65, -1711, -1711,   541,   544,
   -1711, -1711,  1439, -1711,   541, -1711, -1711,  1438, -1711,    65,
    1175,    65, -1711,   -89, -1711, -1711,  3385, -1711,  9365, -1711,
   -1711, -1711, -1711,  1441,  1444,  1435,  1455, -1711, -1711,  9594,
     541, -1711, -1711,    65, -1711,  3629, -1711,  1444,  1445,   812,
    1455, -1711, -1711
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   186,     1,   410,     0,     0,    58,   760,   411,     0,
     979,   969,   974,    20,     0,     0,    19,    16,    15,     3,
       0,     0,     0,     8,   799,     7,   741,     6,    11,     5,
       4,    13,    12,    14,   144,   145,   146,   143,   160,   162,
      49,    68,    65,    66,     0,     0,    50,   762,   761,     0,
       0,     0,    26,    25,   741,   760,   760,   760,     0,   384,
      47,   170,   171,   172,     0,     0,     0,   173,   175,   182,
       0,   169,    21,    10,     9,   333,   781,     0,   742,   743,
       0,     0,     0,     0,     0,    51,     0,     0,    59,     0,
      63,   763,   765,   766,    22,     0,     0,     0,   386,     0,
       0,   181,   176,     0,     0,     0,     0,     0,     0,    77,
     334,   336,   769,   791,   790,   794,   745,   744,   751,   158,
     159,     0,   150,   151,   154,   156,   148,     0,     0,     0,
     147,   163,    69,    67,     0,     0,    52,     0,     0,     0,
       0,     0,    64,    60,   767,     0,    23,    24,    27,   879,
      77,    77,   385,    45,    48,   180,     0,   177,   178,   179,
     183,    75,    78,   187,   338,   337,   340,   335,   771,   770,
       0,   793,   792,   796,   795,   800,   746,   667,   149,    30,
      31,    35,     0,   155,   152,   153,   157,   139,   140,   131,
     132,   135,   137,   130,     0,   129,   141,     0,    54,    55,
      53,    57,    56,    63,    63,     0,     0,     0,    29,     0,
     779,   970,   975,    46,   174,    76,     0,   772,   773,   787,
     748,     0,   668,     0,    32,    33,    34,   136,   133,   134,
     138,     0,   161,    62,    61,     0,     0,     0,     0,     0,
       0,     0,   809,   852,   810,   868,   811,   815,   816,   817,
     818,   858,   822,   823,   824,   825,   826,   827,   828,   853,
     854,   855,   856,   939,   814,   821,   857,   946,   953,   812,
     819,   813,   820,   829,   830,   831,   832,   833,   834,   835,
     836,   837,   838,   839,   840,   841,   842,   843,   844,   845,
     846,   847,   848,   849,   850,   851,     0,     0,     0,     0,
     867,   900,   903,   901,   902,   966,   896,   764,    28,   882,
     883,   880,   881,   777,   780,   980,     0,     0,     0,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,     0,     0,   194,
     188,   284,    77,     0,   779,   788,     0,    77,   750,   747,
     667,    77,     0,   730,   722,   752,   142,   768,   904,   930,
     933,     0,   936,   926,     0,     0,   940,   947,   954,   960,
     963,     0,   898,   910,   378,   916,   921,   915,     0,   929,
     925,   918,     0,   920,     0,   897,     0,   778,     0,   971,
     976,   275,   276,   273,   199,   200,   202,   201,   203,   204,
     205,   206,   232,   233,   230,   231,   223,   234,   235,   224,
     221,   222,   197,   274,   257,     0,   272,   236,   237,   238,
     239,   210,   211,   212,   207,   208,   209,   220,   196,     0,
     226,   227,   225,   218,   219,   214,   213,   215,   216,   217,
     198,   195,   256,     0,   228,   229,   667,   191,     0,   859,
     862,   865,   866,   860,   863,   861,   864,   774,     0,   785,
     801,     0,   164,    77,     0,     0,   723,     0,     0,     0,
       0,     0,     0,   541,   542,     0,     0,     0,     0,   535,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   858,
       0,     0,     0,     0,     0,   445,     0,     0,     0,     0,
       0,     0,   651,   458,   460,   459,   462,   463,   464,   465,
     466,    41,     0,     0,     0,     0,     0,   378,     0,   443,
     444,  1054,   539,   538,   638,   536,   631,   630,   629,   628,
     184,   634,   537,   633,   632,   586,   543,   587,     0,   532,
     594,   544,     0,   540,   991,   993,   992,   533,   995,   994,
     534,     0,     0,     0,   885,     0,   188,     0,   188,     0,
     188,     0,     0,     0,   912,     0,   909,     0,     0,     0,
    1061,   436,   923,   924,   917,   919,     0,   922,   893,     0,
       0,   968,   967,   981,   701,   707,   277,   279,   278,   280,
     271,   255,   281,   258,   240,     0,   189,   409,   692,   693,
       0,     0,     0,   339,     0,   346,   440,   341,   782,     0,
     789,     0,     0,   724,   722,   749,   165,   731,     0,   720,
     721,   719,     0,     0,     0,     0,   890,  1015,  1018,   389,
     867,   393,   392,   398,   984,   990,   985,   986,   987,   989,
     988,     0,   430,     0,     0,     0,  1045,     0,     0,     0,
       0,   421,   424,   591,     0,   427,     0,  1049,     0,  1027,
    1031,     0,     0,  1021,     0,   571,   572,     0,     0,   504,
     501,   503,     0,     0,     0,     0,     0,     0,     0,   461,
     548,   547,   588,   546,   545,     0,     0,     0,     0,   384,
    1061,  1061,     0,    77,     0,     0,   453,   445,   375,   184,
     352,   350,   351,   349,   907,     0,     0,     0,     0,   573,
     574,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   522,     0,     0,
       0,     0,   869,   884,     0,     0,   191,     0,   191,     0,
     191,   384,   699,     0,   697,     0,   705,     0,   870,     0,
    1061,     0,   382,   437,   908,  1062,   379,   914,   892,   895,
       0,   874,   702,    95,   708,    95,   282,   283,   260,   261,
     263,   262,   264,   265,   266,   267,   259,   268,   269,   270,
     244,   245,   247,   246,   248,   249,   250,   251,   242,   243,
     252,   253,   254,   241,     0,   407,   408,     0,   667,   667,
     667,   190,   193,   192,     0,   441,   375,   757,   786,   797,
     680,   802,     0,     0,     0,     0,     0,   738,     0,     0,
     905,   931,   934,    18,    17,   888,   889,     0,     0,     0,
       0,  1013,     0,     0,     0,  1035,  1038,  1041,     0,  1061,
       0,  1052,  1061,     0,     0,     0,     0,     0,     0,     0,
       0,  1061,     0,     0,  1061,  1024,     0,     0,     0,     0,
       0,   188,     0,     0,     0,     0,     0,     0,    44,     0,
      42,     0,     0,  1034,     0,   711,     0,   710,     0,     0,
    1062,  1006,   576,   380,     0,   378,   525,     0,     0,     0,
       0,   188,     0,   453,     0,     0,   602,   601,     0,     0,
     608,   612,   549,   550,   562,   563,   560,   561,     0,   595,
       0,   583,     0,     0,   512,     0,   585,     0,     0,   596,
       0,     0,   607,   606,     0,     0,   613,   617,     0,     0,
     623,   627,   635,   636,   637,   551,   552,   640,   641,   642,
     567,   568,   569,   570,     0,     0,   565,   566,   564,   558,
     559,   554,   553,   555,   556,   557,     0,     0,     0,   510,
       0,     0,     0,     0,     0,   530,     0,   937,   927,   871,
       0,   941,     0,   948,     0,   955,     0,     0,   961,     0,
       0,   964,     0,     0,     0,   898,     0,     0,   438,   894,
     875,   775,    93,    96,   972,    96,   977,     0,     0,   803,
     689,   690,   712,   694,   696,   695,   442,     0,   753,   758,
     775,   683,     0,   726,     0,   727,     0,     0,     0,   740,
     906,   932,   935,   891,     0,     0,     0,  1014,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1062,     0,   589,     0,     0,     0,   590,     0,   639,
       0,     0,     0,     0,     0,     0,     0,   453,     0,     0,
       0,   191,   646,   647,   648,   649,   650,     0,    38,     0,
     111,     0,     0,     0,     0,   997,   996,   575,     0,     0,
       0,     0,  1061,     0,   526,     0,     0,     0,   529,   527,
     185,     0,   191,   377,   401,   399,     0,     0,     0,     0,
     400,     0,     0,     0,    77,     0,   372,   457,   353,     0,
       0,     0,   366,     0,   367,   361,     0,   358,   357,     0,
       0,   359,     0,   376,     0,    91,    92,    89,    90,   368,
     413,   356,     0,   467,   188,   598,     0,   609,     0,     0,
       0,   580,     0,     0,     0,   579,   582,   603,     0,   614,
       0,   624,     0,     0,     0,   618,   622,     0,     0,   584,
       0,     0,     0,     0,   511,     0,   523,     0,   577,     0,
     531,   938,   928,     0,   886,     0,   942,   944,   949,   951,
     956,   958,   698,   962,   700,   704,   965,   706,   898,   899,
     911,   383,   439,     0,   755,   776,   982,    94,   703,     0,
     709,     0,   691,     0,     0,     0,     0,   713,     0,     0,
       0,   776,   783,     0,   681,   798,     0,   667,   725,     0,
       0,   735,     0,     0,   739,  1016,  1019,   390,     0,   395,
     396,   394,     0,     0,   433,   431,     0,     0,     0,  1046,
    1044,   380,     0,  1053,  1056,   422,   425,   592,   428,  1050,
    1048,  1028,  1032,  1030,     0,  1022,    77,   502,   666,   505,
       0,     0,    39,     0,     0,     0,   414,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1055,   381,   528,     0,
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
     873,   887,   872,   945,   952,   959,   913,   876,   877,   756,
       0,   973,   978,     0,   805,     0,   806,   715,   714,   342,
     754,   759,     0,     0,   674,   677,     0,     0,   729,     0,
     737,     0,     0,   391,   397,     0,     0,   432,  1036,  1039,
    1042,     0,     0,     0,     0,     0,     0,     0,     0,  1013,
       0,  1025,     0,     0,     0,     0,   508,   652,     0,    36,
      43,     0,   113,     0,   114,     0,     0,   115,     0,     0,
       0,     0,     0,   999,   998,     0,   493,     0,   495,   448,
     449,     0,   447,   446,     0,   454,     0,   403,     0,   388,
       0,     0,    70,    71,   121,   405,     0,     0,     0,     0,
     168,     0,     0,     0,     0,   732,   419,     0,   484,   485,
     487,   486,   488,   478,   479,   480,   489,   490,   469,   470,
     471,   473,   474,   475,   476,   477,   472,   491,   492,   481,
     482,   483,   468,    77,     0,   664,     0,   660,     0,     0,
     497,   500,   655,   657,   516,   659,     0,   665,     0,   661,
       0,   663,     0,     0,     0,   654,     0,     0,   514,   519,
       0,   521,     0,   983,   804,     0,     0,     0,   343,   348,
     784,     0,   675,   676,   677,   678,   669,   684,   728,   736,
    1013,  1013,     0,     0,     0,     0,     0,   378,  1057,   380,
       0,     0,     0,     0,     0,  1014,  1029,  1013,     0,  1013,
     643,     0,   645,   506,     0,   653,    40,   112,   415,     0,
       0,     0,     0,     0,  1001,  1000,     0,     0,     0,     0,
     452,   455,   406,   128,   127,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   418,     0,   599,   610,     0,
       0,   604,   615,   625,     0,   662,   658,     0,     0,   656,
     878,     0,   808,   716,     0,     0,   672,   669,   670,   671,
     674,  1012,  1012,   434,     0,  1013,  1013,  1004,     0,     0,
       0,     0,     0,     0,  1061,  1012,  1013,  1004,   644,   509,
      37,     0,     0,   117,   118,     0,     0,     0,     0,   494,
     450,   342,    86,    77,   167,     0,     0,     0,    77,   722,
     420,   733,   734,   456,   600,   611,   498,   499,   605,   616,
     626,   620,   517,   518,   807,   375,   682,   673,   685,   672,
       0,     0,  1009,  1061,  1011,     0,     0,     0,  1012,  1012,
    1005,     0,  1047,  1058,   423,   426,   593,   429,     0,     0,
    1004,     0,  1059,   116,     0,  1003,  1002,     0,   374,     0,
       0,   108,     0,    77,    77,     0,     0,     0,   621,     0,
       0,   687,   718,   717,   679,     0,  1062,  1010,  1017,  1020,
     435,     0,     0,  1043,  1051,  1033,     0,  1023,     0,     0,
    1059,     0,    87,    91,    92,    89,    90,    88,   110,   100,
       0,     0,     0,   125,     0,     0,     0,     0,     0,  1007,
       0,  1037,  1040,  1026,     0,  1063,     0,  1059,    97,    79,
       0,    77,   123,   126,     0,     0,   345,   686,    77,     0,
    1060,  1064,     0,   375,    77,    73,    74,     0,   109,     0,
       0,     0,   417,     0,  1008,  1065,     0,    80,     0,   101,
     120,   416,   688,     0,   105,     0,   342,   102,    81,     0,
      77,    99,   375,     0,    82,     0,   106,   105,     0,    79,
     342,    83,   104
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1711, -1711, -1002,    -1, -1711, -1711, -1711, -1711, -1711,   982,
    1577, -1711, -1711, -1711, -1711, -1711, -1711,  1690, -1711, -1711,
   -1711,   -38, -1711, -1711,   836, -1711, -1711,  1609, -1711, -1711,
   -1711, -1711,  -146,  -193, -1711, -1711, -1711, -1711, -1710,   889,
     895, -1711, -1711, -1711, -1711,  -188, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1115, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711,  1466, -1711, -1711,   -43,  -101,  -280,   337, -1711,
   -1711,   528,   987,   984,   642,  -554,  -757, -1711,  -367, -1711,
   -1711, -1711, -1670, -1711, -1711, -1665, -1711, -1711, -1129, -1711,
   -1711, -1711, -1711, -1711, -1711,  -841,  -395, -1256,   907,   -13,
   -1711, -1711, -1711, -1711, -1711, -1703, -1702, -1695, -1694, -1711,
   -1711,  1700, -1711, -1208, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711,  -538, -1491,   385,
     193, -1711,  -869, -1711,   509, -1711, -1711, -1711, -1711, -1426,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,   533,
     590, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711,  -171,    28,   -35,
      30,   112, -1711, -1711, -1711, -1711, -1711, -1711, -1711,   220,
    -590,  -861, -1711,  -597,  -819, -1711, -1026,   -33,   -32, -1711,
    -636,  -631, -1711, -1711, -1711, -1476, -1711,  1668, -1711, -1711,
   -1711, -1711, -1711,   463,   666, -1711,  1050, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711,   669, -1711,  1354, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711,  -131, -1711,  1225, -1711, -1711, -1711,  1481,
   -1711, -1711, -1711,  -645, -1711, -1711,  -389,  -988, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711,  -190, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711,  -898, -1606,  -694, -1711, -1711,
   -1587, -1364,  1231, -1711, -1711, -1711, -1711, -1711, -1711, -1711,
   -1711, -1711, -1711,  1236, -1711, -1711,  1250, -1711, -1711, -1711,
   -1711, -1711, -1711, -1711, -1711, -1711, -1711,  1068, -1711,  -491,
    1258, -1414,  -698,  1269,  -477
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   875,   876,    18,   148,    54,   208,    19,   182,
     195,  1636,  1313,  1489,   708,   552,   154,   553,   100,    21,
      22,    88,    45,    46,   143,    23,    42,    43,  1159,  1160,
    1857,   162,   163,  1858,  1874,  1887,  1361,  1780,  1161,  1043,
    1044,  1839,  1853,  1873,  1840,  1878,  1882,  1888,  1879,  1162,
    1163,  1819,  1164,  1122,  1165,  1166,  1167,  1168,  1169,  1170,
    1171,  1172,   196,   197,    38,    39,    40,   222,  1531,    67,
      68,    69,    70,   726,    24,   477,   633,   370,   371,   111,
      25,   166,   372,   167,   216,  1599,  1684,  1826,   636,   856,
    1259,   554,  1173,  1355,  1655,   942,   717,  1131,   801,   555,
    1174,   661,   880,  1463,   556,  1175,  1176,  1177,  1178,  1179,
     847,  1180,  1372,  1317,  1536,  1181,   557,   894,  1474,   895,
    1475,   898,  1477,   558,   884,  1467,   559,   600,   637,   560,
    1338,  1339,   941,   561,   730,   562,  1182,   563,   564,   701,
     565,   910,  1485,   911,  1634,   566,  1012,  1427,   567,   601,
     569,   896,  1476,  1396,  1734,  1409,  1738,  1398,  1735,  1411,
    1739,  1582,  1788,  1413,  1740,   570,   571,   627,  1690,  1748,
    1604,  1606,  1456,  1061,  1267,  1790,  1828,   628,   629,   630,
     792,   793,   813,   796,   797,   815,   928,  1050,  1051,  1794,
     652,   497,   644,   384,  1664,   645,   385,    79,   118,   220,
     380,    27,   177,  1059,  1245,  1060,    49,    50,   145,    28,
      51,   205,   170,   218,   374,  1246,   314,   315,    29,   112,
     857,  1452,   640,   376,   377,   115,   175,   861,    30,    77,
     219,   641,  1052,   572,   487,   302,   303,  1020,  1041,   210,
     304,   784,  1431,  1029,   655,   415,   305,   596,   306,   498,
    1070,   597,   799,   582,  1222,   499,  1071,   500,  1072,   581,
    1221,   585,  1226,   586,  1433,   587,  1228,   588,  1434,   589,
    1230,   590,  1435,   591,  1233,   592,  1236,   794,    31,    56,
     316,   614,  1249,    32,    57,   317,   615,  1251,    33,    55,
     418,   811,  1440,   663,   573,   721,  1761,   722,  1753,  1754,
    1755,  1080,   574,   878,  1461,   879,  1462,   906,  1482,  1105,
    1628,   902,  1479,   575,   903,  1480,   576,  1084,  1614,  1085,
    1615,  1086,  1616,   888,  1471,   900,  1478,  1132,   723,   577,
     578,  1809,   806,   579,   580
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,    60,    71,   156,   211,   212,   223,    90,  1320,   599,
     720,   877,   868,   866,   667,  1057,   307,  1343,   814,    72,
      73,    74,   929,   931,   812,  1651,  1258,   612,   670,  1022,
     938,  1024,   786,  1026,   788,  1472,   790,  1136,   131,  1244,
     673,  -186,  1103,   141,   602,   387,  1318,  1240,   136,    92,
      93,    71,    71,    71,   617,   619,  1778,   800,  1244,  1633,
    1499,  1665,   501,   502,  1688,    58,  -943,   692,   631,  1812,
    1647,   119,   120,  1263,  1184,   301,  1813,  1814,   142,     2,
     604,  1782,   508,   634,  1815,  1816,     3,  1106,   510,   845,
    1119,    59,    71,    71,    71,    71,   198,   199,   221,   200,
    -950,  1771,  1034,  1324,   301,  1756,   648,  1120,   221,     4,
      52,     5,   221,     6,   403,  1626,    34,    35,  1769,     7,
    1827,   625,   795,   594,    13,   517,   518,  -957,   727,     8,
    1859,  -943,   846,   632,   404,     9,  -943,  1813,  1814,   889,
     649,   187,   188,   595,  1121,  1815,  1816,  1689,    41,    53,
     901,   925,  1032,   904,   635,  -943,  1036,   650,  1848,    10,
     674,   675,  1534,    16,  1806,  -950,    36,  1535,  1323,  1883,
    -950,  1801,  1802,   626,  1529,  1733,  1813,  1814,   381,   105,
     106,   107,   520,   521,  1815,  1816,   937,  1079,  1732,  -950,
     849,  1088,  -957,   300,  1092,   549,   550,  -957,   728,    75,
     678,   221,   651,  1101,    11,    12,  1104,    44,  1709,   491,
    1505,  1881,  1786,   927,   625,   496,  -957,   735,   736,   379,
     679,   850,   300,  1524,  1287,  1892,   478,    76,   603,   121,
    1067,   490,    58,    61,  -507,   492,  1264,  1319,  1306,  1332,
    1135,  1016,   155,  1439,   138,  1090,  1691,  1692,   122,   123,
    1436,  1319,   124,   676,   125,   817,   139,   126,    59,  1439,
     127,  1319,    62,  1705,   605,  1707,   693,  1288,    13,    15,
     988,   925,  1319,   677,    81,   659,   128,  1265,   231,   925,
    1496,    99,    13,   606,   989,   301,   402,    78,  1137,   890,
    1642,  1403,   607,   547,   129,  1501,   926,  1497,    37,  -507,
     551,    13,   214,    14,  -507,   907,   232,    16,   653,   654,
     656,   925,    80,   416,  1424,    15,  1138,   925,   189,   190,
    1027,    16,   191,  -507,   192,    81,  1663,   193,  1500,    63,
     127,  1758,  1759,   927,  1498,   161,   416,   739,   740,   493,
      16,   927,  1770,  1078,    82,   745,   194,   646,   747,   748,
     749,   750,   751,   752,  1310,  1632,    84,  1111,   754,   755,
     756,   757,   758,  1699,   301,    64,  1219,   301,   301,   301,
     686,    84,  1872,   927,    58,    85,    13,   405,  1255,   927,
    1219,  1256,    83,  1278,  1257,  1340,  1493,  1142,  1036,  1652,
      85,   780,   781,   925,  1220,   785,  1836,   787,    86,   789,
      59,   405,   925,   300,   406,   407,    91,   802,  1279,    87,
     926,   718,   161,    86,  1494,    16,   405,   378,    99,  1716,
     683,  1597,  1133,  1852,    87,    89,   108,    65,   406,   407,
    1272,   777,   778,  1507,  1327,  1237,  1283,    66,    98,  1234,
     684,   852,   853,   406,   407,    99,  1329,  1483,  1250,  1223,
     301,   301,   109,  1248,   301,   927,   301,  1224,   301,  1751,
     301,  1032,   642,   110,   927,   745,   301,   642,   747,   748,
     749,   750,   751,   752,  1508,    84,  1066,   960,   108,   643,
    1225,    13,   300,   301,   643,   300,   300,   300,   961,  1074,
    1075,   408,   660,   671,    85,   409,   104,   718,   405,  1087,
     301,   301,  1568,  1514,  1140,  1094,  1095,  1096,  1133,  1098,
    1569,  1100,   700,  1102,  1526,   408,   135,    86,   649,   409,
      16,  1110,  1730,  1586,  1333,   406,   407,   405,    87,   687,
     408,  1515,  1329,   649,   409,   650,   138,   494,  1570,   405,
     495,  1334,   132,   496,  1725,  1219,  1134,   301,   139,   688,
     650,   777,   778,  1329,   406,   407,    58,   410,   134,  1587,
    1571,   411,  1329,   301,   412,   697,   406,   407,   300,   300,
     698,    13,   300,  1574,   300,  1335,   300,   933,   300,  1330,
     651,   410,    59,  1219,   300,   411,  1336,  1588,   412,   413,
    1589,  1337,   101,   102,   103,   651,   410,   873,    13,   699,
     411,   300,   137,   412,   947,   951,   874,  1438,   873,   802,
      16,  1591,   408,   413,  1028,  1048,   409,   874,   300,   300,
    1717,  1219,  1219,   973,   977,   981,   858,   984,   413,   963,
    1395,   140,  1049,   157,   158,   159,   160,    16,  1563,   144,
     964,   408,   405,  1032,  1493,   409,  1013,   867,  1219,  1635,
    1639,  1402,  1493,   408,   149,  1407,  1823,   409,  1329,   747,
     748,   749,   750,   751,   752,   300,   873,    13,   301,   406,
     407,  1670,  1714,   150,  1423,   874,  1715,  1329,   410,  1429,
    1729,   300,   411,  1031,  1107,   412,  1737,  1073,   690,  1678,
    1076,    58,  1842,  1843,  1083,  1286,   873,    13,   882,  1329,
     697,  1292,   151,  1648,  1219,   874,    16,   410,   691,  1329,
     413,   411,    71,  1089,   412,   152,  1304,    59,   883,   410,
     153,   873,    13,   411,  1329,  1275,   412,  1743,   873,    13,
     874,  1870,  1777,  1329,   301,  1669,    16,   874,  1035,   413,
      81,   416,   777,   778,   161,  1064,   301,   301,   301,   301,
    1219,   413,  1829,   301,  1686,  1329,   408,   301,   178,  1677,
     409,    16,  1065,   301,   301,   301,  1724,   301,    16,   301,
     171,   301,   301,  1488,  1605,   873,    13,   172,  1841,   301,
    1495,   873,    13,  1864,   874,   176,   300,   873,    13,    47,
     874,   309,  1238,  1752,  1752,    48,   874,   965,  1282,  1760,
     139,   873,    13,  1445,  1302,  1128,   310,  1752,   966,  1760,
     874,   311,  1042,   312,  1042,    16,  1129,   405,  1305,   116,
    1446,    16,   410,   873,  1854,   117,   411,    16,  1276,   412,
     873,    13,   874,   873,    13,  1855,  1856,  1232,   201,   874,
    1235,    16,   874,   138,   406,   407,  1241,  1469,   735,   736,
    1470,  1208,   300,  1795,   413,   139,   113,   114,  1213,  1784,
    1752,  1752,  1209,  1206,   300,   300,   300,   300,  1069,  1214,
      16,   300,  1760,    16,   105,   300,   107,   718,   183,   184,
     185,   300,   300,   300,   186,   300,   202,   300,  1133,   300,
     300,  1058,   873,  1549,   203,  1553,   301,   300,   301,   301,
     864,   874,   146,   865,  1789,   301,   496,  1550,   147,  1554,
     164,   204,   301,   227,   228,   229,   165,   405,   209,   230,
     168,   416,   173,  1602,   568,   870,   169,   206,   174,  1603,
     207,   408,  1849,   209,   593,   409,   105,  -879,   416,   479,
     213,  1158,   871,   480,   406,   407,   416,   609,   729,   729,
    1017,   729,   729,   301,   301,   859,   860,   481,   482,   301,
     405,  1643,   483,   484,   485,   486,   737,   738,   739,   740,
     741,   416,   215,   742,   301,  1018,   745,   217,   746,   747,
     748,   749,   750,   751,   752,   753,   142,   406,   407,   754,
     755,   756,   757,   758,   416,   759,   760,   410,  1021,   224,
     225,   411,   221,  1289,   412,   416,  1768,  1564,  1352,  1023,
     235,   416,  1866,   308,   300,  1025,   300,   300,   416,   373,
    1576,   416,  1277,   300,   416,  1285,  1359,  1360,  1668,   413,
     300,   408,   416,   416,   416,   409,  1672,  1673,  1741,   233,
     234,  1885,  1247,   313,  1247,   770,   771,   772,   773,   774,
     775,   776,   105,   106,   107,  1797,  1158,   179,   180,   918,
     919,   375,   777,   778,   382,  1271,  1447,  1274,  1053,  1054,
    1055,   300,   300,   731,   408,   732,   733,   300,   409,   179,
     180,   181,  1530,  1530,   405,   388,  1704,   383,   389,   405,
     390,   392,   300,   224,   225,   226,  1455,   410,  1787,   391,
     393,   411,   416,  1295,   412,    95,    96,    97,   417,  1718,
     396,   406,   407,   695,   696,   394,   406,   407,   397,   398,
     399,   400,   419,   401,   420,   301,   474,   414,   405,   413,
     584,   803,   710,   711,   712,   713,   714,   475,   489,   583,
     410,   610,   476,   810,   411,   620,  1296,   412,   621,   622,
    1348,  1825,   613,   623,   624,   406,   407,   638,  1356,  1357,
    1484,   818,   819,   820,   821,   822,   823,   824,   825,   639,
     647,  1365,   413,  1366,  1367,  1368,  1369,  1370,   657,   658,
     672,  1373,   681,   680,   682,   869,   685,   686,  1844,   689,
     694,   702,   703,   783,  1516,   704,   705,   826,   408,   725,
     795,  1537,   409,   408,   734,   706,   805,   409,  1566,   827,
     828,   829,   891,   893,   707,   724,   779,   897,   782,   899,
     798,  1578,  1698,  1580,   855,   804,   807,   808,   405,   844,
     848,   851,   862,   863,   872,   912,   913,   914,   915,   916,
     917,   881,   408,   300,  1820,   885,   409,   886,   908,   887,
     905,   909,   921,   922,   923,   406,   407,   924,   930,   932,
     940,   958,  1019,  1030,   410,   301,  1040,   301,   411,   410,
    1297,   412,  1460,   411,  1039,  1298,   412,  1058,   301,  1063,
     301,   962,   301,  1081,   967,   968,  1091,  1093,  1185,  1187,
    1216,  1097,  1099,  1112,  1113,  1123,   413,  1114,  1115,  1362,
    1141,   413,  1116,  1117,  1197,  1127,  1139,  1186,   410,  1227,
    1188,  1014,   411,  1229,  1299,   412,  1199,  1189,  1190,  1192,
    1191,  1201,  1193,  1194,  1195,  1196,  1218,   952,   953,  1231,
    1653,   954,   955,   956,   957,  1198,   959,  1200,  1038,  1202,
     413,  1254,   408,   969,  1211,  1212,   409,   982,   983,   985,
     986,   987,   990,   991,   992,   993,   995,   996,   997,   998,
     999,  1000,  1001,  1002,  1003,  1004,  1005,  1217,   595,   735,
     736,  1239,  1243,  1252,  1253,  1260,  1268,  1047,  1269,  1270,
    1280,  1281,  1284,   300,  1290,   300,  1291,  1312,  1294,   301,
    1300,  1303,  1674,  1307,  1308,  1062,   300,  1824,   300,  1311,
     300,  1068,  1314,  1315,  1316,  1321,  1322,  1325,   410,  1326,
     405,  1685,   411,  1328,  1301,   412,  1082,  1666,   635,  1346,
    1347,  1349,  1358,  1363,  1364,  1371,  1415,  1416,  1658,  1418,
    1419,   735,   736,  1430,  1425,  1426,  1432,   406,   407,  1444,
     413,  1108,  1109,  1441,  1056,  1442,  1449,  1453,  1454,  1458,
    1464,   301,  1118,  1465,  1466,  1607,  1486,  1124,  1609,  1125,
    1490,  1126,  1492,  1506,  1509,  1510,   891,  1512,  1511,  1513,
     301,  1518,   700,  1522,  1520,  1523,  1596,  1521,  1565,  1527,
    1528,   405,  1533,  1592,  1567,  1605,  1572,  1593,  1573,   739,
     740,  1575,  1598,  1577,  1579,  1625,  1581,   745,  1595,   746,
     747,   748,   749,   750,   751,   752,   753,   300,   406,   407,
     754,   755,   756,   757,   758,  1585,  1594,  1600,  1601,  1608,
    1610,  1613,  1611,  1617,   408,  1158,  1618,  1637,   409,  1619,
    1620,  1621,  1183,  1622,  1623,  1624,  1638,  1627,  1629,  1640,
    1210,  1641,  1646,  1649,  1215,  1659,  1660,  1682,  1675,   737,
     738,   739,   740,   741,  1676,  1679,   742,   743,   744,   745,
    1693,   746,   747,   748,   749,   750,   751,   752,   753,   300,
    1242,  1694,   754,   755,   756,   757,   758,  1781,   759,   760,
     405,  1695,  1785,   777,   778,  1696,   762,  1697,   300,  1706,
     410,   405,  1708,  1710,   411,   408,  1309,   412,  1711,   409,
    1273,  1713,  1719,  1720,  1722,  1721,  1723,   406,   407,  1728,
    1736,  1742,  1744,  1747,  1762,  1763,  1779,  1745,   406,   407,
    1764,   700,   413,  1727,  1293,   767,   768,   769,   770,   771,
     772,   773,   774,   775,   776,  1765,  1766,  1821,  1822,  1329,
    1767,  1773,  1783,  1796,  1798,   777,   778,  1183,  1774,  1808,
    1799,  1811,  1800,  1830,   405,   301,  1803,   130,  1804,  1805,
    1807,   410,  1831,  1731,  1834,   411,  1832,  1468,   412,  1833,
    1038,  1835,  1838,  1851,  1880,  1846,  1868,  1342,   735,   736,
    1345,   406,   407,  1865,  1746,  1876,  1351,  1877,  1354,  1890,
     920,    20,   133,   413,   408,  1860,  1891,   386,   409,  1889,
    1532,    26,  1863,  1266,  1045,   408,  1033,  1650,  1867,   409,
    1046,   405,   939,   943,  1791,  1749,  1687,  1792,  1793,  1397,
    1750,  1399,    94,  1404,  1451,  1405,  1261,  1408,   488,  1262,
     662,  1410,   395,  1412,  1884,  1414,   664,     0,   406,   407,
       0,   665,     0,     0,  1420,     0,     0,   892,     0,     0,
       0,     0,  1038,     0,     0,   666,     0,     0,     0,     0,
     410,     0,     0,   668,   411,     0,  1473,   412,   408,     0,
       0,   410,   409,   300,   669,   411,  1437,  1481,   412,  1818,
       0,     0,     0,     0,     0,     0,  1443,     0,  1158,     0,
       0,  1448,   413,  1450,   735,   736,     0,  1417,   739,   740,
       0,     0,  1457,   413,     0,  1459,   745,     0,   746,   747,
     748,   749,   750,   751,   752,   753,     0,     0,     0,   754,
     755,   756,   757,   758,   891,   408,  1847,     0,     0,   409,
       0,     0,     0,     0,   410,     0,     0,     0,   411,     0,
    1667,   412,     0,     0,  1862,     0,     0,  1491,     0,     0,
       0,     0,     0,     0,     0,  1502,  1503,  1504,  1869,     0,
    1871,     0,     0,     0,     0,  1158,   413,     0,     0,     0,
       0,     0,     0,     0,  1517,     0,     0,  1519,     0,   774,
     775,   776,  1886,     0,  1158,     0,  1525,     0,     0,     0,
       0,   410,   777,   778,     0,   411,     0,  1671,   412,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   737,   738,   739,   740,   741,     0,     0,   742,
     743,   744,   745,   413,   746,   747,   748,   749,   750,   751,
     752,   753,     0,     0,     0,   754,   755,   756,   757,   758,
       0,   759,   760,     0,     0,     0,     0,     0,     0,  1583,
       0,  1584,     0,     0,     0,     0,     0,     0,  1590,     0,
       0,     0,   735,   736,  1538,  1539,  1540,  1541,  1542,  1543,
    1544,  1545,  1546,  1547,  1548,  1551,  1552,  1555,  1556,  1557,
    1558,  1559,  1560,  1561,  1562,     0,     0,     0,   767,   768,
     769,   770,   771,   772,   773,   774,   775,   776,  1612,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   777,   778,
       0,     0,     0,     0,     0,     0,  1631,     0,     0,   830,
     831,   832,   833,   834,   835,   836,   837,     0,     0,     0,
       0,     0,     0,  1644,  1645,     0,     0,     0,   838,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1654,   839,     0,     0,     0,     0,
    1656,  1657,     0,     0,   735,   736,  1662,   840,   841,   842,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     737,   738,   739,   740,   741,     0,     0,   742,   743,   744,
     745,     0,   746,   747,   748,   749,   750,   751,   752,   753,
     843,     0,  1038,   754,   755,   756,   757,   758,     0,   759,
     760,     0,     0,     0,     0,     0,  1183,     0,     0,     0,
    1038,     0,     0,     0,     0,  1680,     0,     0,  1681,     0,
    1683,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   891,  1700,  1701,  1702,  1703,   768,   769,   770,
     771,   772,   773,   774,   775,   776,     0,     0,     0,     0,
       0,     0,     0,  1712,     0,     0,   777,   778,     0,     0,
       0,     0,   737,   738,   739,   740,   741,     0,     0,   742,
     743,   744,   745,  1726,   746,   747,   748,   749,   750,   751,
     752,   753,     0,     0,     0,   754,   755,   756,   757,   758,
       0,   759,   760,     0,     0,   761,     0,     0,     0,   762,
     763,   764,     0,     0,     0,   765,     0,  1757,     0,   944,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1772,     0,     0,     0,     0,  1775,
    1776,     0,     0,     0,     0,     0,   766,  1207,   767,   768,
     769,   770,   771,   772,   773,   774,   775,   776,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   777,   778,
       0,   242,   243,   244,     0,   246,   247,   248,   249,   250,
     519,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,     0,   264,   265,   266,     0,  1810,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1837,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1845,     0,
       0,     0,     0,     0,     0,     0,     0,  1850,     0,  1817,
       0,     0,     0,     0,     0,     0,     0,  1861,     0,  1183,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     945,  1875,     0,     0,     0,     0,     0,     0,     0,     0,
    1143,   946,     0,     0,   501,   502,     3,     0,  -122,  -107,
    -107,     0,  -119,     0,   503,   504,   505,   506,   507,     0,
    1817,     0,     0,     0,   508,  1144,   509,  1145,  1146,     0,
     510,     0,     0,     0,     0,     0,     0,  1147,   511,  1148,
       0,  -124,     0,  1149,   512,     0,  1183,   513,     0,     8,
     514,  1150,     0,  1151,   515,     0,     0,  1152,  1153,  1817,
       0,     0,     0,     0,  1154,  1183,     0,   517,   518,     0,
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
      59,     0,    13,   541,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   542,
     543,   544,     0,    14,     0,     0,   545,   546,     0,     0,
       0,     0,     0,     0,     0,   547,     0,   548,     0,   549,
     550,    16,  1156,  1157,  1143,     0,     0,     0,   501,   502,
       3,     0,  -122,  -107,  -107,     0,  -119,     0,   503,   504,
     505,   506,   507,     0,     0,     0,     0,     0,   508,  1144,
     509,  1145,  1146,     0,   510,     0,     0,     0,     0,     0,
       0,  1147,   511,  1148,     0,  -124,     0,  1149,   512,     0,
       0,   513,     0,     8,   514,  1150,     0,  1151,   515,     0,
       0,  1152,  1153,     0,     0,     0,     0,     0,  1154,     0,
       0,   517,   518,     0,   242,   243,   244,     0,   246,   247,
     248,   249,   250,   519,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,     0,   264,   265,   266,     0,
       0,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   520,   521,
     522,  1155,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   525,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   526,   527,   528,   529,
     530,     0,   531,     0,   532,   533,   534,     0,   535,   536,
     537,   538,   539,   540,    59,     0,    13,   541,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,   543,   544,     0,    14,     0,     0,
     545,   546,     0,     0,     0,     0,     0,     0,     0,   547,
       0,   548,     0,   549,   550,    16,  1156,  -347,  1143,     0,
       0,     0,   501,   502,     3,     0,  -122,  -107,  -107,     0,
    -119,     0,   503,   504,   505,   506,   507,     0,     0,     0,
       0,     0,   508,  1144,   509,  1145,  1146,     0,   510,     0,
       0,     0,     0,     0,     0,  1147,   511,  1148,     0,  -124,
       0,  1149,   512,     0,     0,   513,     0,     8,   514,  1150,
       0,  1151,   515,     0,     0,  1152,  1153,     0,     0,     0,
       0,     0,  1154,     0,     0,   517,   518,     0,   242,   243,
     244,     0,   246,   247,   248,   249,   250,   519,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,     0,
     264,   265,   266,     0,     0,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   520,   521,   522,  1155,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     526,   527,   528,   529,   530,     0,   531,     0,   532,   533,
     534,     0,   535,   536,   537,   538,   539,   540,    59,     0,
      13,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   542,   543,   544,
       0,    14,     0,     0,   545,   546,     0,     0,     0,     0,
       0,     0,     0,   547,     0,   548,     0,   549,   550,    16,
    1156,  -373,  1143,     0,     0,     0,   501,   502,     3,     0,
    -122,  -107,  -107,     0,  -119,     0,   503,   504,   505,   506,
     507,     0,     0,     0,     0,     0,   508,  1144,   509,  1145,
    1146,     0,   510,     0,     0,     0,     0,     0,     0,  1147,
     511,  1148,     0,  -124,     0,  1149,   512,     0,     0,   513,
       0,     8,   514,  1150,     0,  1151,   515,     0,     0,  1152,
    1153,     0,     0,     0,     0,     0,  1154,     0,     0,   517,
     518,     0,   242,   243,   244,     0,   246,   247,   248,   249,
     250,   519,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,     0,   264,   265,   266,     0,     0,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   520,   521,   522,  1155,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   525,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   526,   527,   528,   529,   530,     0,
     531,     0,   532,   533,   534,     0,   535,   536,   537,   538,
     539,   540,    59,     0,    13,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,   543,   544,     0,    14,     0,     0,   545,   546,
       0,     0,     0,     0,     0,     0,     0,   547,     0,   548,
       0,   549,   550,    16,  1156,  -344,  1143,     0,     0,     0,
     501,   502,     3,     0,  -122,  -107,  -107,     0,  -119,     0,
     503,   504,   505,   506,   507,     0,     0,     0,     0,     0,
     508,  1144,   509,  1145,  1146,     0,   510,     0,     0,     0,
       0,     0,     0,  1147,   511,  1148,     0,  -124,     0,  1149,
     512,     0,     0,   513,     0,     8,   514,  1150,     0,  1151,
     515,     0,     0,  1152,  1153,     0,     0,     0,     0,     0,
    1154,     0,     0,   517,   518,     0,   242,   243,   244,     0,
     246,   247,   248,   249,   250,   519,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,     0,   264,   265,
     266,     0,     0,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     520,   521,   522,  1155,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   525,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,   529,   530,     0,   531,     0,   532,   533,   534,     0,
     535,   536,   537,   538,   539,   540,    59,     0,    13,   541,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   542,   543,   544,     0,    14,
       0,     0,   545,   546,     0,     0,     0,     0,     0,     0,
       0,   547,     0,   548,     0,   549,   550,    16,  1156,   -98,
    1143,     0,     0,     0,   501,   502,     3,     0,  -122,  -107,
    -107,     0,  -119,     0,   503,   504,   505,   506,   507,     0,
       0,     0,     0,     0,   508,  1144,   509,  1145,  1146,     0,
     510,     0,     0,     0,     0,     0,     0,  1147,   511,  1148,
       0,  -124,     0,  1149,   512,     0,     0,   513,     0,     8,
     514,  1150,     0,  1151,   515,     0,     0,  1152,  1153,     0,
       0,     0,     0,     0,  1154,     0,     0,   517,   518,     0,
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
      59,     0,    13,   541,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   542,
     543,   544,     0,    14,     0,     0,   545,   546,     0,     0,
       0,     0,     0,     0,     0,   547,     0,   548,     0,   549,
     550,    16,  1156,  -103,   501,   502,     0,     0,     0,     0,
       0,     0,     0,     0,   503,   504,   505,   506,   507,     0,
       0,     0,     0,     0,   508,     0,   509,     0,     0,     0,
     510,     0,     0,     0,     0,     0,     0,     0,   511,     0,
       0,     0,     0,     0,   512,     0,     0,   513,     0,     0,
     514,     0,     0,     0,   515,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   516,     0,     0,   517,   518,   934,
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
       0,     0,     0,   501,   502,   935,     0,   548,   936,   549,
     550,   715,   551,   503,   504,   505,   506,   507,     0,     0,
       0,     0,     0,   508,     0,   509,     0,     0,     0,   510,
       0,     0,     0,     0,     0,     0,     0,   511,     0,     0,
       0,     0,     0,   512,     0,     0,   513,   716,     0,   514,
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
     715,   551,   503,   504,   505,   506,   507,     0,     0,     0,
       0,     0,   508,     0,   509,     0,     0,     0,   510,     0,
       0,     0,     0,     0,     0,     0,   511,     0,     0,     0,
       0,     0,   512,     0,     0,   513,   716,     0,   514,     0,
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
       0,   501,   502,   547,     0,   548,     0,   549,   550,     0,
     551,   503,   504,   505,   506,   507,     0,     0,     0,     0,
       0,   508,     0,   509,     0,     0,     0,   510,     0,     0,
       0,     0,     0,     0,     0,   511,     0,     0,     0,     0,
       0,   512,     0,     0,   513,     0,     0,   514,     0,     0,
       0,   515,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,     0,     0,   517,   518,  1077,   242,   243,   244,
       0,   246,   247,   248,   249,   250,   519,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,     0,   264,
     265,   266,     0,     0,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   520,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,     0,     0,     0,     0,   598,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   526,
     527,   528,   529,   530,     0,   531,   718,   532,   533,   534,
       0,   535,   536,   537,   538,   539,   540,   719,     0,     0,
     541,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   542,   543,   544,     0,
      14,     0,     0,   545,   546,     0,     0,     0,     0,     0,
     501,   502,   547,     0,   548,     0,   549,   550,     0,   551,
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
     528,   529,   530,     0,   531,   718,   532,   533,   534,     0,
     535,   536,   537,   538,   539,   540,   719,     0,     0,   541,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   542,   543,   544,     0,    14,
       0,     0,   545,   546,     0,     0,     0,     0,     0,   501,
     502,  1400,     0,   548,  1401,   549,   550,     0,   551,   503,
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
     529,   530,     0,   531,   718,   532,   533,   534,     0,   535,
     536,   537,   538,   539,   540,   719,     0,     0,   541,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   542,   543,   544,     0,    14,     0,
       0,   545,   546,     0,     0,     0,     0,     0,   501,   502,
    1421,     0,   548,  1422,   549,   550,     0,   551,   503,   504,
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
     530,     0,   531,   718,   532,   533,   534,     0,   535,   536,
     537,   538,   539,   540,   719,     0,     0,   541,     0,     0,
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
       0,     0,   598,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   526,   527,   528,   529,   530,
       0,   531,     0,   532,   533,   534,     0,   535,   536,   537,
     538,   539,   540,    59,     0,     0,   541,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   542,   543,   544,     0,    14,     0,     0,   545,
     546,     0,     0,     0,     0,     0,   501,   502,   547,     0,
     548,  1015,   549,   550,     0,   551,   503,   504,   505,   506,
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
       0,  1037,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   525,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   526,   527,   528,   529,   530,     0,
     531,   718,   532,   533,   534,     0,   535,   536,   537,   538,
     539,   540,   719,     0,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,   543,   544,     0,    14,     0,     0,   545,   546,
       0,     0,     0,     0,     0,   501,   502,  1331,     0,   548,
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
       0,   532,   533,   534,     0,   535,   536,   537,   538,   539,
     540,    59,     0,     0,   541,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,   543,   544,     0,    14,     0,     0,   545,   546,     0,
       0,     0,     0,     0,   501,   502,   547,     0,   548,  1406,
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
       0,     0,   526,   527,   528,   529,   530,     0,   531,     0,
     532,   533,   534,     0,   535,   536,   537,   538,   539,   540,
      59,     0,     0,   541,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   542,
     543,   544,     0,    14,     0,     0,   545,   546,     0,     0,
       0,     0,     0,   501,   502,   547,     0,   548,  1428,   549,
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
       0,   526,   527,   528,   529,   530,     0,   531,     0,   532,
     533,   534,     0,   535,   536,   537,   538,   539,   540,    59,
       0,     0,   541,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   542,   543,
     544,     0,    14,     0,     0,   545,   546,     0,     0,     0,
       0,     0,   501,   502,   547,     0,   548,  1487,   549,   550,
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
       0,     0,     0,     0,     0,     0,     0,  1037,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     526,   527,   528,   529,   530,     0,   531,   718,   532,   533,
     534,     0,   535,   536,   537,   538,   539,   540,   719,     0,
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
       0,     0,     0,     0,     0,     0,   598,     0,     0,     0,
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
     515,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
     502,   547,   608,   548,     0,   549,   550,     0,   551,   503,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   526,   527,   528,
     529,   530,     0,   531,     0,   532,   533,   534,     0,   535,
     536,   537,   538,   539,   540,    59,     0,     0,   541,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   542,   543,   544,     0,    14,     0,
       0,   545,   546,     0,     0,     0,     0,     0,   501,   502,
     547,   809,   548,     0,   549,   550,     0,   551,   503,   504,
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
       0,     0,     0,  1037,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   525,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   526,   527,   528,   529,
     530,     0,   531,     0,   532,   533,   534,     0,   535,   536,
     537,   538,   539,   540,    59,     0,     0,   541,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,   543,   544,     0,    14,     0,     0,
     545,   546,     0,     0,     0,     0,     0,   501,   502,   547,
       0,   548,     0,   549,   550,  1130,   551,   503,   504,   505,
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
       0,  1341,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,  1344,     0,     0,     0,   516,     0,     0,   517,   518,
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
       0,     0,     0,     0,   508,     0,   509,     0,     0,     0,
     510,     0,     0,     0,     0,     0,     0,     0,   511,     0,
       0,     0,     0,     0,   512,     0,     0,   513,     0,     0,
     514,     0,     0,     0,   515,     0,     0,  1350,     0,     0,
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
       0,     0,     0,   501,   502,   547,     0,   548,     0,   549,
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
     524,     0,     0,     0,     0,     0,     0,     0,  1353,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   526,   527,   528,   529,   530,     0,   531,     0,   532,
     533,   534,     0,   535,   536,   537,   538,   539,   540,    59,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     526,   527,   528,   529,   530,     0,   531,     0,   532,   533,
     534,     0,   535,   536,   537,   538,   539,   540,    59,     0,
       0,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   542,   543,   544,
       0,    14,     0,     0,   545,   546,     0,     0,     0,     0,
       0,   501,   502,   547,     0,   548,  1630,   549,   550,     0,
     551,   503,   504,   505,   506,   507,     0,     0,     0,     0,
       0,   508,     0,   509,     0,     0,     0,   510,     0,     0,
       0,     0,     0,     0,     0,   511,     0,     0,     0,     0,
       0,   512,     0,     0,   513,     0,     0,   514,     0,     0,
       0,   515,  1661,     0,     0,     0,     0,     0,     0,     0,
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
       0,   535,   536,   537,   538,   539,   540,    59,     0,     0,
     541,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   542,   543,   544,     0,
      14,     0,     0,   545,   546,     0,     0,     0,     0,     0,
     501,   502,   547,     0,   548,     0,   549,   550,     0,   551,
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
    1144,   509,  1145,     0,     0,   510,     0,     0,     0,     0,
       0,     0,     0,   511,     0,     0,     0,     0,     0,   512,
       0,     0,   513,     0,     0,   514,  1150,     0,     0,   515,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   516,
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
     536,   537,   538,   539,   540,    59,     0,     0,   541,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   542,   543,   544,     0,    14,     0,
       0,   545,   546,     0,     0,     0,   501,   502,     0,     0,
     547,     0,   548,     0,   549,   550,   503,   504,   505,   506,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   525,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   526,   527,   528,   529,   530,     0,
     531,     0,   532,   533,   534,   709,   535,   536,   537,   538,
     539,   540,    59,     0,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,   543,   544,     0,    14,     0,     0,   545,   546,
       0,     0,     0,   501,   502,     0,     0,   547,     0,   548,
       0,   549,   550,   503,   504,   505,   506,   507,     0,     0,
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
     524,     0,     0,     0,     0,     0,     0,     0,   854,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   526,   527,   528,   529,   530,     0,   531,     0,   532,
     533,   534,     0,   535,   536,   537,   538,   539,   540,    59,
       0,     0,   541,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   542,   543,
     544,     0,    14,     0,     0,   545,   546,     0,     0,     0,
     501,   502,     0,     0,   547,     0,   548,     0,   549,   550,
     503,   504,   505,   506,   507,     0,     0,   994,     0,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   525,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,   529,   530,     0,   531,     0,   532,   533,   534,     0,
     535,   536,   537,   538,   539,   540,    59,     0,     0,   541,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   542,   543,   544,     0,    14,
       0,     0,   545,   546,     0,     0,     0,   501,   502,     0,
       0,   547,     0,   548,     0,   549,   550,   503,   504,   505,
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
     538,   539,   540,    59,     0,   236,   541,     0,   735,   736,
       0,   237,     0,     0,     0,     0,     0,   238,     0,     0,
       0,     0,   542,   543,   544,     0,    14,   239,     0,   545,
     546,     0,     0,     0,     0,   240,     0,     0,   547,     0,
     548,     0,   549,   550,     0,     0,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   -88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   735,   736,   737,   738,   739,   740,
     741,     0,     0,   742,   743,   744,   745,     0,   746,   747,
     748,   749,   750,   751,   752,   753,     0,     0,     0,   754,
     755,   756,   757,   758,     0,   759,   760,     0,     0,   761,
       0,    58,     0,   762,   763,   764,     0,     0,     0,   765,
       0,     0,     0,     0,   298,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     766,     0,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   777,   778,     0,     0,   299,     0,     0,     0,
       0,   611,     0,     0,  1374,  1375,  1376,  1377,  1378,  1379,
    1380,  1381,   737,   738,   739,   740,   741,  1382,  1383,   742,
     743,   744,   745,  1384,   746,   747,   748,   749,   750,   751,
     752,   753,  1385,  1386,  1387,   754,   755,   756,   757,   758,
    1388,   759,   760,  1389,  1390,   761,     0,     0,     0,   762,
     763,   764,  1391,  1392,  1393,   765,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1394,   766,     0,   767,   768,
     769,   770,   771,   772,   773,   774,   775,   776,   236,     0,
       0,     0,     0,     0,   237,     0,     0,     0,   777,   778,
     238,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     239,     0,     0,     0,     0,     0,     0,     0,   240,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   298,   735,   736,
       0,   236,     0,     0,     0,     0,     0,   237,     0,     0,
     791,     0,    13,   238,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   239,     0,     0,     0,     0,     0,     0,
       0,   240,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   241,     0,     0,   299,
       0,    16,     0,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,     0,
       0,     0,     0,     0,     0,     0,   737,   738,   739,   740,
     741,     0,     0,   742,   743,   744,   745,     0,   746,   747,
     748,   749,   750,   751,   752,   753,     0,     0,     0,   754,
     755,   756,   757,   758,     0,   759,   760,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,     0,   236,     0,     0,   421,   422,     0,   237,     0,
       0,     0,     0,    59,   238,     0,     0,     0,     0,     0,
       0,     0,   423,     0,   239,   770,   771,   772,   773,   774,
     775,   776,   240,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   777,   778,     0,     0,     0,   241,     0,     0,
       0,     0,   299,     0,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     735,   736,     0,     0,     0,   424,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,     0,   442,   443,   444,   445,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   446,   447,   448,   449,   450,     0,     0,    58,     0,
     451,   452,   453,   454,   455,   456,   457,     0,     0,     0,
       0,   298,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   791,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   458,   459,     0,   460,
     461,   462,   463,   464,   465,   466,   467,   468,   469,   735,
     736,   470,   471,     0,     0,     0,     0,     0,     0,   472,
     473,     0,     0,   299,     0,     0,     0,     0,   737,   738,
     739,   740,   741,     0,     0,   742,   743,   744,   745,     0,
     746,   747,   748,   749,   750,   751,   752,   753,     0,     0,
       0,   754,   755,   756,   757,   758,     0,   759,   760,     0,
       0,  -958,     0,     0,     0,   762,   763,   764,     0,     0,
       0,  -958,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     735,   736,   766,     0,   767,   768,   769,   770,   771,   772,
     773,   774,   775,   776,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   777,   778,     0,   737,   738,   739,
     740,   741,     0,     0,   742,   743,   744,   745,     0,   746,
     747,   748,   749,   750,   751,   752,   753,   735,   736,     0,
     754,   755,   756,   757,   758,     0,   759,   760,     0,     0,
       0,     0,     0,     0,   762,   763,   764,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   735,   736,     0,     0,     0,     0,     0,     0,     0,
       0,   766,     0,   767,   768,   769,   770,   771,   772,   773,
     774,   775,   776,     0,     0,     0,     0,     0,   737,   738,
     739,   740,   741,   777,   778,   742,   743,   744,   745,     0,
     746,   747,   748,   749,   750,   751,   752,   753,   735,   736,
       0,   754,   755,   756,   757,   758,     0,   759,   760,     0,
       0,     0,     0,     0,     0,   762,     0,   764,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   739,   740,     0,
       0,     0,     0,     0,     0,   745,     0,   746,   747,   748,
     749,   750,   751,   752,   753,     0,     0,     0,   754,   755,
     756,   757,   758,     0,   767,   768,   769,   770,   771,   772,
     773,   774,   775,   776,     0,     0,     0,     0,     0,   737,
     738,   739,   740,   741,   777,   778,   742,   743,   744,   745,
       0,   746,   747,   748,   749,   750,   751,   752,   753,     0,
       0,     0,   754,   755,   756,   757,   758,     0,   759,   760,
       0,     0,     0,     0,     0,     0,   772,   773,   774,   775,
     776,     0,     0,     0,     0,     0,   737,   738,   739,   740,
       0,   777,   778,     0,     0,     0,   745,     0,   746,   747,
     748,   749,   750,   751,   752,   753,     0,     0,     0,   754,
     755,   756,   757,   758,     0,   759,   760,   769,   770,   771,
     772,   773,   774,   775,   776,     0,     0,     0,     0,     0,
       0,     0,   948,     0,     0,   777,   778,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   772,   773,   774,
     775,   776,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   777,   778,   242,   243,   244,   970,   246,   247,
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
       0,     0,     0,   949,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   950,     0,     0,     0,   974,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   971,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   972,
     242,   243,   244,   978,   246,   247,   248,   249,   250,   519,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,     0,   264,   265,   266,     0,     0,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,     0,   242,   243,   244,     0,   246,
     247,   248,   249,   250,   519,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,     0,   264,   265,   266,
       0,     0,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   975,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     976,     0,     0,     0,  1203,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   979,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   980,   242,   243,   244,     0,
     246,   247,   248,   249,   250,   519,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   318,   264,   265,
     266,     0,     0,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
       0,     0,     0,   319,     0,   320,     0,   321,   322,   323,
     324,   325,     0,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   337,   338,   339,     0,     0,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,     0,     0,     0,
       0,     0,     0,     0,     0,  1204,     0,     0,     0,     0,
       0,     0,     0,   367,   368,     0,  1205,   242,   243,   244,
       0,   246,   247,   248,   249,   250,   519,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,     0,   264,
     265,   266,     0,     0,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,     0,     0,   369,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1006,  1007,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1008,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1009,     0,     0,
       0,     0,     0,     0,     0,     0,   319,     0,   320,     0,
     321,   322,   323,   324,   325,     0,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,     0,   337,   338,
     339,  1010,  1011,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     319,     0,   320,     0,   321,   322,   323,   324,   325,     0,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,     0,   337,   338,   339,     0,     0,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,     0,   319,     0,   320,     0,   321,
     322,   323,   324,   325,     0,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   616,   337,   338,   339,
       0,     0,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     618,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   816
};

static const yytype_int16 yycheck[] =
{
       1,    14,    15,   104,   150,   151,   177,    45,  1123,   404,
     548,   656,   648,   644,   505,   856,   206,  1146,   615,    20,
      21,    22,   720,   721,   614,  1516,  1052,   416,   505,   786,
     724,   788,   586,   790,   588,  1291,   590,   935,    81,  1041,
       5,     8,   903,    22,    33,   235,    20,  1035,    86,    50,
      51,    64,    65,    66,   421,   422,  1721,   595,  1060,  1485,
      20,  1537,     5,     6,    46,   175,   149,    19,   149,  1779,
      20,    15,    16,    40,   943,   206,  1779,  1779,    57,     0,
      33,    20,    25,   172,  1779,  1779,     7,   906,    31,     7,
     184,   201,   105,   106,   107,   108,   134,   135,   174,   137,
     149,  1707,   800,  1129,   235,  1692,   495,   201,   174,    30,
     201,    32,   174,    34,   218,  1479,    19,    20,  1705,    40,
    1790,   187,   201,   218,   203,    68,    69,   149,   173,    50,
    1840,   214,    50,   214,   238,    56,   219,  1840,  1840,   677,
     150,    15,    16,   238,   238,  1840,  1840,   129,   192,   240,
     688,   150,   797,   691,   243,   238,   801,   167,  1828,    80,
     125,   126,   238,   242,  1770,   214,    69,   243,   167,  1879,
     219,  1758,  1759,   239,   236,  1666,  1879,  1879,   221,   176,
     177,   178,   125,   126,  1879,  1879,   724,   881,  1664,   238,
     211,   889,   214,   206,   892,   240,   241,   219,   243,     8,
     218,   174,   212,   901,   125,   126,   904,   213,  1634,   380,
    1325,  1876,   214,   212,   187,   217,   238,    21,    22,   220,
     238,   242,   235,  1352,  1085,  1890,   372,    36,   217,   173,
     866,   377,   175,    34,   149,   381,   203,   211,  1107,  1137,
     934,   779,   239,  1245,   223,   890,  1610,  1611,   192,   193,
    1238,   211,   196,   218,   198,   622,   235,   201,   201,  1261,
     204,   211,    63,  1627,   217,  1629,   218,  1086,   203,   236,
     761,   150,   211,   238,   211,   218,   220,   244,   211,   150,
     184,   175,   203,   236,   761,   416,   299,    62,   211,   678,
    1498,  1189,   245,   236,   238,  1321,   167,   201,   201,   214,
     243,   203,   239,   224,   219,   694,   239,   242,   498,   499,
     500,   150,   212,   215,  1212,   236,   239,   150,   192,   193,
     214,   242,   196,   238,   198,   211,  1534,   201,   167,   130,
     204,  1695,  1696,   212,   238,   203,   215,   141,   142,   382,
     242,   212,  1706,   881,   214,   149,   220,   493,   152,   153,
     154,   155,   156,   157,  1111,  1484,   182,   911,   162,   163,
     164,   165,   166,  1619,   495,   166,   211,   498,   499,   500,
     238,   182,  1863,   212,   175,   201,   203,    33,   211,   212,
     211,   214,   211,  1077,   217,  1142,   211,   941,  1033,  1518,
     201,   581,   582,   150,   239,   585,  1810,   587,   224,   589,
     201,    33,   150,   416,    60,    61,   201,   597,   239,   235,
     167,   190,   203,   224,   239,   242,    33,   244,   175,   167,
     218,  1447,   201,  1837,   235,   236,   211,   228,    60,    61,
    1066,   235,   236,  1331,  1132,  1032,  1081,   238,   201,  1029,
     238,   631,   632,    60,    61,   175,   211,   238,  1045,   211,
     581,   582,   237,  1043,   585,   212,   587,   219,   589,   238,
     591,  1106,   184,   130,   212,   149,   597,   184,   152,   153,
     154,   155,   156,   157,   239,   182,   865,   190,   211,   201,
     242,   203,   495,   614,   201,   498,   499,   500,   201,   878,
     879,   147,   505,   506,   201,   151,   238,   190,    33,   888,
     631,   632,  1400,   211,   237,   894,   895,   896,   201,   898,
     211,   900,   525,   902,  1355,   147,   223,   224,   150,   151,
     242,   910,   244,  1421,   150,    60,    61,    33,   235,   218,
     147,   239,   211,   150,   151,   167,   223,   211,   239,    33,
     214,   167,   192,   217,  1659,   211,   239,   678,   235,   238,
     167,   235,   236,   211,    60,    61,   175,   213,   223,   211,
     239,   217,   211,   694,   220,   184,    60,    61,   581,   582,
     189,   203,   585,   239,   587,   201,   589,   723,   591,   237,
     212,   213,   201,   211,   597,   217,   212,   239,   220,   245,
     239,   217,    64,    65,    66,   212,   213,   202,   203,   218,
     217,   614,   223,   220,   735,   736,   211,   201,   202,   799,
     242,   239,   147,   245,   219,   184,   151,   211,   631,   632,
    1646,   211,   211,   754,   755,   756,   639,   758,   245,   190,
    1184,   201,   201,   105,   106,   107,   108,   242,  1395,   201,
     201,   147,    33,  1288,   211,   151,   777,   648,   211,   239,
     239,  1189,   211,   147,   201,  1193,  1785,   151,   211,   152,
     153,   154,   155,   156,   157,   678,   202,   203,   799,    60,
      61,  1569,   239,   201,  1212,   211,   239,   211,   213,  1217,
     239,   694,   217,   219,   219,   220,   239,   877,   218,  1587,
     880,   175,  1821,  1822,   884,  1084,   202,   203,   218,   211,
     184,  1090,   201,   237,   211,   211,   242,   213,   238,   211,
     245,   217,   725,   219,   220,   201,  1105,   201,   238,   213,
     205,   202,   203,   217,   211,   219,   220,   239,   202,   203,
     211,  1860,   239,   211,   865,   237,   242,   211,   219,   245,
     211,   215,   235,   236,   203,   184,   877,   878,   879,   880,
     211,   245,   239,   884,    47,   211,   147,   888,   201,   237,
     151,   242,   201,   894,   895,   896,   237,   898,   242,   900,
     130,   902,   903,  1311,    67,   202,   203,   130,   239,   910,
    1318,   202,   203,   239,   211,   243,   799,   202,   203,    57,
     211,    79,   219,  1691,  1692,    63,   211,   190,   219,  1697,
     235,   202,   203,   184,   219,   190,    94,  1705,   201,  1707,
     211,    99,   813,   101,   815,   242,   201,    33,   219,    57,
     201,   242,   213,   202,    12,    63,   217,   242,   219,   220,
     202,   203,   211,   202,   203,    23,    24,  1027,   201,   211,
    1030,   242,   211,   223,    60,    61,  1036,   219,    21,    22,
     219,   190,   865,  1751,   245,   235,     5,     6,   190,   239,
    1758,  1759,   201,   994,   877,   878,   879,   880,   869,   201,
     242,   884,  1770,   242,   176,   888,   178,   190,   192,   193,
     194,   894,   895,   896,   198,   898,   201,   900,   201,   902,
     903,   201,   202,  1384,   237,  1386,  1027,   910,  1029,  1030,
     211,   211,    57,   214,  1745,  1036,   217,  1384,    63,  1386,
      57,   201,  1043,   192,   193,   194,    63,    33,   214,   198,
      57,   215,    57,    57,   391,   219,    63,   212,    63,    63,
     228,   147,  1830,   214,   401,   151,   176,   218,   215,    75,
     205,   942,   219,    79,    60,    61,   215,   414,   563,   564,
     219,   566,   567,  1084,  1085,   242,   243,    93,    94,  1090,
      33,  1499,    98,    99,   100,   101,   139,   140,   141,   142,
     143,   215,   203,   146,  1105,   219,   149,   201,   151,   152,
     153,   154,   155,   156,   157,   158,    57,    60,    61,   162,
     163,   164,   165,   166,   215,   168,   169,   213,   219,   205,
     206,   217,   174,   219,   220,   215,  1704,  1396,  1154,   219,
     212,   215,  1853,    66,  1027,   219,  1029,  1030,   215,   214,
    1409,   215,   219,  1036,   215,   219,    10,    11,   219,   245,
    1043,   147,   215,   215,   215,   151,   219,   219,   219,   203,
     204,  1882,  1043,   202,  1045,   218,   219,   220,   221,   222,
     223,   224,   176,   177,   178,  1753,  1057,   205,   206,   207,
     208,   129,   235,   236,   174,  1066,  1256,  1068,   848,   849,
     850,  1084,  1085,   564,   147,   566,   567,  1090,   151,   205,
     206,   207,  1362,  1363,    33,   218,  1624,   201,   218,    33,
     218,   218,  1105,   205,   206,   207,  1267,   213,  1729,   238,
     218,   217,   215,   219,   220,    55,    56,    57,   202,  1647,
     218,    60,    61,   523,   524,   238,    60,    61,   218,   218,
     218,   218,   243,   238,   243,  1256,    35,   236,    33,   245,
     218,   598,   542,   543,   544,   545,   546,    35,   201,   201,
     213,   236,   238,   610,   217,   201,   219,   220,   237,    22,
    1151,  1787,   243,   201,   237,    60,    61,   243,  1159,  1160,
    1306,   131,   132,   133,   134,   135,   136,   137,   138,   214,
     201,  1172,   245,  1174,  1175,  1176,  1177,  1178,   218,   218,
     218,  1182,   218,   238,   218,   652,   218,   238,  1824,   218,
     218,   238,   238,   219,  1340,   238,   238,   167,   147,   236,
     201,  1372,   151,   147,   239,   238,   211,   151,  1398,   179,
     180,   181,   679,   680,   238,   238,   238,   684,   239,   686,
     239,  1411,  1617,  1413,   634,   239,   237,   237,    33,   238,
     242,   239,   238,    43,   219,   702,   703,   704,   705,   706,
     707,   238,   147,  1256,  1782,   218,   151,   218,   238,   218,
     218,   238,   238,    13,   237,    60,    61,   238,   211,   239,
       4,   201,   201,   214,   213,  1396,   201,  1398,   217,   213,
     219,   220,  1273,   217,   237,   219,   220,   201,  1409,   201,
    1411,   748,  1413,   201,   751,   752,   211,   239,   218,   218,
       1,   239,   239,   239,   239,   238,   245,   239,   239,    13,
     238,   245,   239,   239,   218,   239,   239,   238,   213,   219,
     238,   778,   217,   219,   219,   220,   218,   238,   238,   238,
     237,   218,   238,   238,   237,   237,   237,   737,   738,   219,
    1520,   741,   742,   743,   744,   238,   746,   238,   805,   238,
     245,    43,   147,   753,   238,   238,   151,   757,   758,   759,
     760,   761,   762,   763,   764,   765,   766,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,   238,   238,    21,
      22,   239,   212,   239,   238,   212,   239,   844,   238,    43,
     239,   239,   239,  1396,   239,  1398,   238,   214,   239,  1520,
     239,   239,  1582,   239,   239,   862,  1409,  1786,  1411,   238,
    1413,   868,   238,    43,   201,   239,   238,   238,   213,   244,
      33,  1601,   217,   239,   219,   220,   883,  1563,   243,   201,
     201,   192,    10,     9,    42,    66,   218,   238,  1529,   238,
     237,    21,    22,   201,   238,   237,   201,    60,    61,   201,
     245,   908,   909,   244,   854,   244,   244,   201,     8,   201,
     239,  1582,   919,   238,   201,  1456,   219,   924,  1459,   926,
     209,   928,   201,   239,   201,   201,   933,   201,   238,   201,
    1601,    14,  1485,   214,   212,   192,    43,   212,   239,   238,
     238,    33,   238,   212,   239,    67,   239,   244,   239,   141,
     142,   239,    37,   239,   239,    70,   239,   149,   238,   151,
     152,   153,   154,   155,   156,   157,   158,  1520,    60,    61,
     162,   163,   164,   165,   166,   239,   239,   244,   212,   239,
     238,   219,   238,   238,   147,  1526,   239,   239,   151,   238,
     238,   238,   942,   238,   238,   238,   201,   238,   238,   238,
    1007,    43,   239,   201,  1011,   238,   238,   201,   239,   139,
     140,   141,   142,   143,   239,   239,   146,   147,   148,   149,
     239,   151,   152,   153,   154,   155,   156,   157,   158,  1582,
    1037,   238,   162,   163,   164,   165,   166,  1723,   168,   169,
      33,   238,  1728,   235,   236,   238,   176,   238,  1601,   238,
     213,    33,   239,   205,   217,   147,   219,   220,   242,   151,
    1067,   201,   239,   239,   239,   244,   239,    60,    61,   239,
     239,   239,   239,    33,   239,   239,    12,   243,    60,    61,
     239,  1634,   245,  1661,  1091,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   239,   239,  1783,  1784,   211,
     239,   239,   239,   211,   239,   235,   236,  1057,   242,   242,
     239,   242,   239,   238,    33,  1786,   239,    80,   239,   239,
     239,   213,   239,  1664,    53,   217,   239,   219,   220,   239,
    1137,   237,   243,   237,   239,   244,   238,  1144,    21,    22,
    1147,    60,    61,   244,  1685,   244,  1153,   243,  1155,   244,
     708,     1,    83,   245,   147,  1841,  1889,   231,   151,  1887,
    1363,     1,  1848,  1061,   815,   147,   799,  1514,  1854,   151,
     815,    33,   725,   729,  1749,  1687,  1604,  1750,  1750,  1186,
    1690,  1188,    54,  1190,  1261,  1192,  1060,  1194,   374,  1060,
     505,  1198,   251,  1200,  1880,  1202,   505,    -1,    60,    61,
      -1,   505,    -1,    -1,  1211,    -1,    -1,   679,    -1,    -1,
      -1,    -1,  1219,    -1,    -1,   505,    -1,    -1,    -1,    -1,
     213,    -1,    -1,   505,   217,    -1,   219,   220,   147,    -1,
      -1,   213,   151,  1786,   505,   217,  1243,   219,   220,  1780,
      -1,    -1,    -1,    -1,    -1,    -1,  1253,    -1,  1789,    -1,
      -1,  1258,   245,  1260,    21,    22,    -1,  1207,   141,   142,
      -1,    -1,  1269,   245,    -1,  1272,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,    -1,    -1,    -1,   162,
     163,   164,   165,   166,  1291,   147,  1827,    -1,    -1,   151,
      -1,    -1,    -1,    -1,   213,    -1,    -1,    -1,   217,    -1,
     219,   220,    -1,    -1,  1845,    -1,    -1,  1314,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1322,  1323,  1324,  1859,    -1,
    1861,    -1,    -1,    -1,    -1,  1866,   245,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1341,    -1,    -1,  1344,    -1,   222,
     223,   224,  1883,    -1,  1885,    -1,  1353,    -1,    -1,    -1,
      -1,   213,   235,   236,    -1,   217,    -1,   219,   220,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,   245,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,    -1,    -1,   162,   163,   164,   165,   166,
      -1,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,  1416,
      -1,  1418,    -1,    -1,    -1,    -1,    -1,    -1,  1425,    -1,
      -1,    -1,    21,    22,  1374,  1375,  1376,  1377,  1378,  1379,
    1380,  1381,  1382,  1383,  1384,  1385,  1386,  1387,  1388,  1389,
    1390,  1391,  1392,  1393,  1394,    -1,    -1,    -1,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,  1465,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,
      -1,    -1,    -1,    -1,    -1,    -1,  1483,    -1,    -1,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    -1,  1500,  1501,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1521,   167,    -1,    -1,    -1,    -1,
    1527,  1528,    -1,    -1,    21,    22,  1533,   179,   180,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     212,    -1,  1569,   162,   163,   164,   165,   166,    -1,   168,
     169,    -1,    -1,    -1,    -1,    -1,  1526,    -1,    -1,    -1,
    1587,    -1,    -1,    -1,    -1,  1592,    -1,    -1,  1595,    -1,
    1597,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1619,  1620,  1621,  1622,  1623,   216,   217,   218,
     219,   220,   221,   222,   223,   224,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1640,    -1,    -1,   235,   236,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,  1660,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,    -1,    -1,   162,   163,   164,   165,   166,
      -1,   168,   169,    -1,    -1,   172,    -1,    -1,    -1,   176,
     177,   178,    -1,    -1,    -1,   182,    -1,  1694,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1711,    -1,    -1,    -1,    -1,  1716,
    1717,    -1,    -1,    -1,    -1,    -1,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,  1774,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1811,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1825,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1834,    -1,  1779,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1844,    -1,  1789,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,  1868,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,   201,    -1,    -1,     5,     6,     7,    -1,     9,    10,
      11,    -1,    13,    -1,    15,    16,    17,    18,    19,    -1,
    1840,    -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,
      -1,    42,    -1,    44,    45,    -1,  1866,    48,    -1,    50,
      51,    52,    -1,    54,    55,    -1,    -1,    58,    59,  1879,
      -1,    -1,    -1,    -1,    65,  1885,    -1,    68,    69,    -1,
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
     201,    -1,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,
     221,   222,    -1,   224,    -1,    -1,   227,   228,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   236,    -1,   238,    -1,   240,
     241,   242,   243,   244,     1,    -1,    -1,    -1,     5,     6,
       7,    -1,     9,    10,    11,    -1,    13,    -1,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    39,    40,    -1,    42,    -1,    44,    45,    -1,
      -1,    48,    -1,    50,    51,    52,    -1,    54,    55,    -1,
      -1,    58,    59,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,   186,
     187,    -1,   189,    -1,   191,   192,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   220,   221,   222,    -1,   224,    -1,    -1,
     227,   228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   236,
      -1,   238,    -1,   240,   241,   242,   243,   244,     1,    -1,
      -1,    -1,     5,     6,     7,    -1,     9,    10,    11,    -1,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    39,    40,    -1,    42,
      -1,    44,    45,    -1,    -1,    48,    -1,    50,    51,    52,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,   184,   185,   186,   187,    -1,   189,    -1,   191,   192,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,   221,   222,
      -1,   224,    -1,    -1,   227,   228,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   236,    -1,   238,    -1,   240,   241,   242,
     243,   244,     1,    -1,    -1,    -1,     5,     6,     7,    -1,
       9,    10,    11,    -1,    13,    -1,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      39,    40,    -1,    42,    -1,    44,    45,    -1,    -1,    48,
      -1,    50,    51,    52,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,   184,   185,   186,   187,    -1,
     189,    -1,   191,   192,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   220,   221,   222,    -1,   224,    -1,    -1,   227,   228,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   236,    -1,   238,
      -1,   240,   241,   242,   243,   244,     1,    -1,    -1,    -1,
       5,     6,     7,    -1,     9,    10,    11,    -1,    13,    -1,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    -1,    44,
      45,    -1,    -1,    48,    -1,    50,    51,    52,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,
     185,   186,   187,    -1,   189,    -1,   191,   192,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   220,   221,   222,    -1,   224,
      -1,    -1,   227,   228,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   236,    -1,   238,    -1,   240,   241,   242,   243,   244,
       1,    -1,    -1,    -1,     5,     6,     7,    -1,     9,    10,
      11,    -1,    13,    -1,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    48,    -1,    50,
      51,    52,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
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
     201,    -1,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,
     221,   222,    -1,   224,    -1,    -1,   227,   228,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   236,    -1,   238,    -1,   240,
     241,   242,   243,   244,     5,     6,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    70,
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
     241,    13,   243,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
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
      13,   243,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
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
      -1,     5,     6,   236,    -1,   238,    -1,   240,   241,    -1,
     243,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    70,    71,    72,    73,
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
     185,   186,   187,    -1,   189,   190,   191,   192,   193,    -1,
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
     186,   187,    -1,   189,   190,   191,   192,   193,    -1,   195,
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
     187,    -1,   189,   190,   191,   192,   193,    -1,   195,   196,
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
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   191,   192,   193,    -1,   195,   196,   197,   198,   199,
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
      -1,    -1,   183,   184,   185,   186,   187,    -1,   189,    -1,
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
      -1,   183,   184,   185,   186,   187,    -1,   189,    -1,   191,
     192,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,   221,
     222,    -1,   224,    -1,    -1,   227,   228,    -1,    -1,    -1,
      -1,    -1,     5,     6,   236,    -1,   238,   239,   240,   241,
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
     183,   184,   185,   186,   187,    -1,   189,   190,   191,   192,
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
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
       6,   236,   237,   238,    -1,   240,   241,    -1,   243,    15,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,
     186,   187,    -1,   189,    -1,   191,   192,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   220,   221,   222,    -1,   224,    -1,
      -1,   227,   228,    -1,    -1,    -1,    -1,    -1,     5,     6,
     236,   237,   238,    -1,   240,   241,    -1,   243,    15,    16,
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
      -1,   238,    -1,   240,   241,    13,   243,    15,    16,    17,
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
      -1,    61,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
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
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    58,    -1,    -1,
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
      -1,    -1,    -1,     5,     6,   236,    -1,   238,    -1,   240,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      26,    27,    28,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    52,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
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
     196,   197,   198,   199,   200,   201,    -1,    -1,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   220,   221,   222,    -1,   224,    -1,
      -1,   227,   228,    -1,    -1,    -1,     5,     6,    -1,    -1,
     236,    -1,   238,    -1,   240,   241,    15,    16,    17,    18,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,   184,   185,   186,   187,    -1,
     189,    -1,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   220,   221,   222,    -1,   224,    -1,    -1,   227,   228,
      -1,    -1,    -1,     5,     6,    -1,    -1,   236,    -1,   238,
      -1,   240,   241,    15,    16,    17,    18,    19,    -1,    -1,
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
       5,     6,    -1,    -1,   236,    -1,   238,    -1,   240,   241,
      15,    16,    17,    18,    19,    -1,    -1,    22,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,
     185,   186,   187,    -1,   189,    -1,   191,   192,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   220,   221,   222,    -1,   224,
      -1,    -1,   227,   228,    -1,    -1,    -1,     5,     6,    -1,
      -1,   236,    -1,   238,    -1,   240,   241,    15,    16,    17,
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
     198,   199,   200,   201,    -1,    19,   204,    -1,    21,    22,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,   220,   221,   222,    -1,   224,    41,    -1,   227,
     228,    -1,    -1,    -1,    -1,    49,    -1,    -1,   236,    -1,
     238,    -1,   240,   241,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,    -1,    -1,    -1,   162,
     163,   164,   165,   166,    -1,   168,   169,    -1,    -1,   172,
      -1,   175,    -1,   176,   177,   178,    -1,    -1,    -1,   182,
      -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     213,    -1,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,   236,    -1,    -1,   240,    -1,    -1,    -1,
      -1,   245,    -1,    -1,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,    -1,    -1,    -1,   176,
     177,   178,   179,   180,   181,   182,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   212,   213,    -1,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,   235,   236,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    21,    22,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
     201,    -1,   203,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,   240,
      -1,   242,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,    -1,    -1,    -1,   162,
     163,   164,   165,   166,    -1,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,    19,    -1,    -1,    21,    22,    -1,    25,    -1,
      -1,    -1,    -1,   201,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    41,   218,   219,   220,   221,   222,
     223,   224,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,   236,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,   240,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
      21,    22,    -1,    -1,    -1,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,   168,   169,   170,   171,    -1,    -1,   175,    -1,
     176,   177,   178,   179,   180,   181,   182,    -1,    -1,    -1,
      -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   212,   213,    -1,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,    21,
      22,   227,   228,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     236,    -1,    -1,   240,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,    -1,    -1,
      -1,   162,   163,   164,   165,   166,    -1,   168,   169,    -1,
      -1,   172,    -1,    -1,    -1,   176,   177,   178,    -1,    -1,
      -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,   213,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   235,   236,    -1,   139,   140,   141,
     142,   143,    -1,    -1,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,    21,    22,    -1,
     162,   163,   164,   165,   166,    -1,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   213,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,   235,   236,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,    21,    22,
      -1,   162,   163,   164,   165,   166,    -1,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,   176,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,    -1,    -1,    -1,   162,   163,
     164,   165,   166,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   235,   236,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      -1,    -1,   162,   163,   164,   165,   166,    -1,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,   220,   221,   222,   223,
     224,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
      -1,   235,   236,    -1,    -1,    -1,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,    -1,    -1,    -1,   162,
     163,   164,   165,   166,    -1,   168,   169,   217,   218,   219,
     220,   221,   222,   223,   224,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,   235,   236,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,   221,   222,
     223,   224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,   236,    71,    72,    73,    19,    75,    76,
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
      -1,    -1,    -1,    -1,   201,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
      71,    72,    73,    19,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     201,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   201,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    35,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      -1,    -1,    -1,    71,    -1,    73,    -1,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   201,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    -1,    -1,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,   235,   236,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      71,    -1,    73,    -1,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    -1,    71,    -1,    73,    -1,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,   201,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   201
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   247,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   125,   126,   203,   224,   236,   242,   249,   250,   254,
     263,   265,   266,   271,   320,   326,   357,   447,   455,   464,
     474,   524,   529,   534,    19,    20,    69,   201,   310,   311,
     312,   192,   272,   273,   213,   268,   269,    57,    63,   452,
     453,   456,   201,   240,   252,   535,   525,   530,   175,   201,
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
     345,   479,   481,   482,   486,   492,   494,   523,    66,    79,
      94,    99,   101,   202,   462,   463,   526,   531,    35,    71,
      73,    75,    76,    77,    78,    79,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    93,    94,    95,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   141,   142,   201,
     323,   324,   328,   214,   460,   129,   469,   470,   244,   249,
     446,   311,   174,   201,   439,   442,   308,   523,   218,   218,
     218,   238,   218,   218,   238,   485,   218,   218,   218,   218,
     218,   238,   345,   218,   238,    33,    60,    61,   147,   151,
     213,   217,   220,   245,   236,   491,   215,   202,   536,   243,
     243,    21,    22,    38,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   150,   151,   152,   153,   167,   168,   169,   170,
     171,   176,   177,   178,   179,   180,   181,   182,   212,   213,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     227,   228,   235,   236,    35,    35,   238,   321,   278,    75,
      79,    93,    94,    98,    99,   100,   101,   480,   463,   201,
     278,   413,   278,   311,   211,   214,   217,   437,   495,   501,
     503,     5,     6,    15,    16,    17,    18,    19,    25,    27,
      31,    39,    45,    48,    51,    55,    65,    68,    69,    80,
     125,   126,   127,   141,   142,   173,   183,   184,   185,   186,
     187,   189,   191,   192,   193,   195,   196,   197,   198,   199,
     200,   204,   220,   221,   222,   227,   228,   236,   238,   240,
     241,   243,   261,   263,   337,   345,   350,   362,   369,   372,
     375,   379,   381,   383,   384,   386,   391,   394,   395,   396,
     411,   412,   479,   540,   548,   559,   562,   575,   576,   579,
     580,   505,   499,   201,   218,   507,   509,   511,   513,   515,
     517,   519,   521,   395,   218,   238,   493,   497,   150,   342,
     373,   395,    33,   217,    33,   217,   236,   245,   237,   395,
     236,   245,   492,   243,   527,   532,   201,   324,   201,   324,
     201,   237,    22,   201,   237,   187,   239,   413,   423,   424,
     425,   149,   214,   322,   172,   243,   334,   374,   243,   214,
     468,   477,   184,   201,   438,   441,   278,   201,   492,   150,
     167,   212,   436,   523,   523,   490,   523,   218,   218,   218,
     345,   347,   481,   539,   548,   559,   562,   575,   576,   579,
     580,   345,   218,     5,   125,   126,   218,   238,   218,   238,
     238,   218,   218,   218,   238,   218,   238,   218,   238,   218,
     218,   238,    19,   218,   218,   396,   396,   184,   189,   218,
     345,   385,   238,   238,   238,   238,   238,   238,   260,   194,
     396,   396,   396,   396,   396,    13,    49,   342,   190,   201,
     373,   541,   543,   574,   238,   236,   319,   173,   243,   375,
     380,   380,   380,   380,   239,    21,    22,   139,   140,   141,
     142,   143,   146,   147,   148,   149,   151,   152,   153,   154,
     155,   156,   157,   158,   162,   163,   164,   165,   166,   168,
     169,   172,   176,   177,   178,   182,   213,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   235,   236,   238,
     523,   523,   239,   219,   487,   523,   321,   523,   321,   523,
     321,   201,   426,   427,   523,   201,   429,   430,   239,   498,
     373,   344,   523,   395,   239,   211,   578,   237,   237,   237,
     395,   537,   426,   428,   429,   431,   201,   324,   131,   132,
     133,   134,   135,   136,   137,   138,   167,   179,   180,   181,
     131,   132,   133,   134,   135,   136,   137,   138,   150,   167,
     179,   180,   181,   212,   238,     7,    50,   356,   242,   211,
     242,   239,   523,   523,   150,   396,   335,   466,   345,   242,
     243,   473,   238,    43,   211,   214,   437,   249,   436,   395,
     219,   219,   219,   202,   211,   248,   249,   489,   549,   551,
     348,   238,   218,   238,   370,   218,   218,   218,   569,   373,
     492,   395,   573,   395,   363,   365,   397,   395,   367,   395,
     571,   373,   557,   560,   373,   218,   553,   492,   238,   238,
     387,   389,   395,   395,   395,   395,   395,   395,   207,   208,
     255,   238,    13,   237,   238,   150,   167,   212,   432,   578,
     211,   578,   239,   278,    70,   236,   239,   373,   543,   318,
       4,   378,   341,   319,    19,   190,   201,   479,    19,   190,
     201,   479,   396,   396,   396,   396,   396,   396,   201,   396,
     190,   201,   395,   190,   201,   190,   201,   395,   395,   396,
      19,   190,   201,   479,    19,   190,   201,   479,    19,   190,
     201,   479,   396,   396,   479,   396,   396,   396,   575,   580,
     396,   396,   396,   396,    22,   396,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   152,   153,   190,   201,
     235,   236,   392,   479,   395,   239,   373,   219,   219,   201,
     483,   219,   322,   219,   322,   219,   322,   214,   219,   489,
     214,   219,   489,   344,   578,   219,   489,   150,   395,   237,
     201,   484,   249,   285,   286,   285,   286,   395,   184,   201,
     433,   434,   478,   425,   425,   425,   396,   341,   201,   449,
     451,   419,   395,   201,   184,   201,   492,   436,   395,   249,
     496,   502,   504,   523,   492,   492,   523,    70,   373,   543,
     547,   201,   395,   523,   563,   565,   567,   492,   578,   219,
     489,   211,   578,   239,   492,   492,   492,   239,   492,   239,
     492,   578,   492,   427,   578,   555,   430,   219,   395,   395,
     492,   321,   239,   239,   239,   239,   239,   239,   395,   184,
     201,   238,   299,   238,   395,   395,   395,   239,   190,   201,
      13,   343,   573,   201,   239,   543,   541,   211,   239,   239,
     237,   238,   321,     1,    26,    28,    29,    38,    40,    44,
      52,    54,    58,    59,    65,   128,   243,   244,   249,   274,
     275,   284,   295,   296,   298,   300,   301,   302,   303,   304,
     305,   306,   307,   338,   346,   351,   352,   353,   354,   355,
     357,   361,   382,   396,   378,   218,   238,   218,   238,   238,
     238,   237,   238,   238,   238,   237,   237,   218,   238,   218,
     238,   218,   238,    19,   190,   201,   479,   214,   190,   201,
     395,   238,   238,   190,   201,   395,     1,   238,   237,   211,
     239,   506,   500,   211,   219,   242,   508,   219,   512,   219,
     516,   219,   523,   520,   426,   523,   522,   429,   219,   239,
     493,   523,   395,   212,   248,   450,   461,   249,   426,   528,
     429,   533,   239,   238,    43,   211,   214,   217,   432,   336,
     212,   450,   461,    40,   203,   244,   320,   420,   239,   238,
      43,   249,   436,   395,   249,   219,   219,   219,   543,   239,
     239,   239,   219,   489,   239,   219,   492,   427,   430,   219,
     239,   238,   492,   395,   239,   219,   219,   219,   219,   219,
     239,   219,   219,   239,   492,   219,   378,   239,   239,   219,
     322,   238,   214,   258,   238,    43,   201,   359,    20,   211,
     299,   239,   238,   167,   432,   238,   244,   578,   239,   211,
     237,   236,   541,   150,   167,   201,   212,   217,   376,   377,
     322,   150,   395,   334,    61,   395,   201,   201,   249,   192,
      58,   395,   278,   150,   395,   339,   249,   249,    10,    10,
      11,   282,    13,     9,    42,   249,   249,   249,   249,   249,
     249,    66,   358,   249,   131,   132,   133,   134,   135,   136,
     137,   138,   144,   145,   150,   159,   160,   161,   167,   170,
     171,   179,   180,   181,   212,   321,   399,   395,   403,   395,
     236,   239,   373,   541,   395,   395,   239,   373,   395,   401,
     395,   405,   395,   409,   395,   218,   238,   396,   238,   237,
     395,   236,   239,   373,   541,   238,   237,   393,   239,   373,
     201,   488,   201,   510,   514,   518,   493,   395,   201,   248,
     538,   244,   244,   395,   201,   184,   201,   523,   395,   244,
     395,   449,   467,   201,     8,   413,   418,   395,   201,   395,
     249,   550,   552,   349,   239,   238,   201,   371,   219,   219,
     219,   570,   343,   219,   364,   366,   398,   368,   572,   558,
     561,   219,   554,   238,   278,   388,   219,   239,   373,   259,
     209,   395,   201,   211,   239,   373,   184,   201,   238,    20,
     167,   432,   395,   395,   395,   299,   239,   541,   239,   201,
     201,   238,   201,   201,   211,   239,   278,   395,    14,   395,
     212,   212,   214,   192,   334,   395,   341,   238,   238,   236,
     313,   314,   314,   238,   238,   243,   360,   413,   396,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   575,
     580,   396,   396,   575,   580,   396,   396,   396,   396,   396,
     396,   396,   396,   322,   492,   239,   523,   239,   541,   211,
     239,   239,   239,   239,   239,   239,   492,   239,   523,   239,
     523,   239,   407,   395,   395,   239,   541,   211,   239,   239,
     395,   239,   212,   244,   239,   238,    43,   432,    37,   331,
     244,   212,    57,    63,   416,    67,   417,   249,   239,   249,
     238,   238,   395,   219,   564,   566,   568,   238,   239,   238,
     238,   238,   238,   238,   238,    70,   547,   238,   556,   238,
     239,   395,   334,   385,   390,   239,   257,   239,   201,   239,
     238,    43,   359,   373,   395,   395,   239,    20,   237,   201,
     376,   374,   334,   523,   395,   340,   395,   395,   312,   238,
     238,    56,   395,   359,   440,   441,   278,   219,   219,   237,
     541,   219,   219,   219,   523,   239,   239,   237,   541,   239,
     395,   395,   201,   395,   332,   523,    47,   417,    46,   129,
     414,   547,   547,   239,   238,   238,   238,   238,   342,   343,
     395,   395,   395,   395,   373,   547,   238,   547,   239,   385,
     205,   242,   395,   201,   239,   239,   167,   432,   373,   239,
     239,   244,   239,   239,   237,   299,   395,   267,   239,   239,
     244,   249,   441,   374,   400,   404,   239,   239,   402,   406,
     410,   219,   239,   239,   239,   243,   249,    33,   415,   414,
     416,   238,   541,   544,   545,   546,   546,   395,   547,   547,
     541,   542,   239,   239,   239,   239,   239,   239,   578,   546,
     547,   542,   395,   239,   242,   395,   395,   239,   331,    12,
     283,   278,    20,   239,   239,   278,   214,   437,   408,   341,
     421,   415,   433,   434,   435,   541,   211,   578,   239,   239,
     239,   546,   546,   239,   239,   239,   542,   239,   242,   577,
     395,   242,   284,   351,   352,   353,   354,   396,   249,   297,
     373,   278,   278,   334,   492,   436,   333,   328,   422,   239,
     238,   239,   239,   239,    53,   237,   577,   395,   243,   287,
     290,   239,   334,   334,   436,   395,   244,   249,   328,   541,
     395,   237,   577,   288,    12,    23,    24,   276,   279,   284,
     278,   395,   249,   278,   239,   244,   341,   278,   238,   249,
     334,   249,   374,   289,   280,   395,   244,   243,   291,   294,
     239,   331,   292,   284,   278,   341,   249,   281,   293,   291,
     244,   279,   331
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
     427,   428,   428,   428,   429,   430,   430,   431,   431,   431,
     432,   432,   433,   433,   433,   434,   434,   435,   435,   436,
     436,   436,   437,   437,   438,   438,   438,   438,   438,   438,
     439,   439,   440,   440,   440,   441,   441,   441,   442,   442,
     442,   443,   443,   444,   444,   444,   445,   445,   446,   445,
     447,   448,   447,   449,   449,   450,   450,   451,   451,   451,
     452,   452,   452,   454,   453,   455,   455,   457,   456,   458,
     458,   458,   459,   460,   460,   461,   461,   462,   462,   463,
     463,   465,   466,   467,   464,   468,   468,   469,   469,   470,
     471,   471,   471,   471,   472,   472,   472,   473,   473,   475,
     476,   477,   474,   478,   478,   478,   478,   478,   478,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   480,
     480,   480,   480,   480,   480,   480,   480,   481,   482,   482,
     482,   483,   483,   483,   484,   484,   484,   484,   484,   485,
     485,   485,   485,   485,   486,   487,   488,   486,   489,   489,
     490,   490,   491,   491,   491,   491,   492,   492,   493,   493,
     494,   494,   494,   494,   495,   496,   494,   494,   494,   494,
     497,   494,   498,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   499,   500,   494,   494,
     501,   502,   494,   503,   504,   494,   505,   506,   494,   494,
     507,   508,   494,   509,   510,   494,   494,   511,   512,   494,
     513,   514,   494,   494,   515,   516,   494,   517,   518,   494,
     519,   520,   494,   521,   522,   494,   523,   523,   523,   525,
     526,   527,   528,   524,   530,   531,   532,   533,   529,   535,
     536,   537,   538,   534,   539,   539,   539,   539,   539,   539,
     539,   540,   540,   540,   540,   540,   541,   541,   541,   541,
     541,   541,   541,   541,   542,   542,   543,   544,   544,   545,
     545,   546,   546,   547,   547,   549,   550,   548,   551,   552,
     548,   553,   554,   548,   555,   556,   548,   557,   558,   548,
     559,   560,   561,   559,   562,   563,   564,   562,   565,   566,
     562,   567,   568,   562,   562,   569,   570,   562,   562,   571,
     572,   562,   573,   573,   574,   575,   576,   576,   576,   577,
     577,   578,   578,   579,   579,   580
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
       3,     0,     1,     3,     3,     1,     3,     0,     1,     3,
       1,     1,     1,     2,     3,     3,     5,     1,     1,     1,
       1,     1,     0,     1,     1,     4,     3,     3,     6,     5,
       1,     3,     0,     2,     2,     4,     6,     5,     4,     6,
       5,     0,     1,     0,     1,     1,     0,     2,     0,     4,
       6,     0,     6,     1,     3,     1,     2,     0,     1,     3,
       0,     1,     1,     0,     5,     3,     3,     0,     6,     0,
       1,     1,     1,     0,     2,     0,     1,     1,     2,     0,
       1,     0,     0,     0,    13,     0,     2,     0,     1,     3,
       1,     1,     2,     2,     0,     1,     1,     1,     3,     0,
       0,     0,     9,     1,     4,     3,     3,     6,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     1,     3,     3,     0,     1,     3,     3,     5,     0,
       2,     2,     2,     2,     4,     0,     0,     7,     1,     1,
       1,     3,     3,     2,     4,     3,     1,     2,     0,     4,
       1,     1,     1,     1,     0,     0,     6,     4,     4,     3,
       0,     6,     0,     7,     4,     2,     2,     3,     2,     3,
       2,     2,     3,     3,     3,     2,     0,     0,     6,     2,
       0,     0,     6,     0,     0,     6,     0,     0,     6,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       0,     0,     6,     0,     0,     6,     1,     3,     3,     0,
       0,     0,     0,    12,     0,     0,     0,     0,    12,     0,
       0,     0,     0,    13,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     5,     5,
       6,     6,     8,     8,     0,     1,     2,     3,     5,     1,
       2,     1,     0,     0,     1,     0,     0,    10,     0,     0,
      10,     0,     0,    10,     0,     0,    11,     0,     0,     7,
       5,     0,     0,    10,     3,     0,     0,    11,     0,     0,
      11,     0,     0,    10,     5,     0,     0,     9,     5,     0,
       0,    10,     1,     3,     0,     5,     5,     7,     9,     0,
       3,     0,     1,    11,    12,    13
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
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
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
            auto call_fmt = new ExprCall(tokAt(scanner,(yylsp[-1])), "_::fmt");
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
            das2_accept_sequence(scanner,seqt,strlen(seqt),thisLine,info);
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
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
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
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-7])),(yyvsp[-5].pExpression),(yyvsp[-1].pExpression),(yyvsp[0].pExpression));
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
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-7])),(yyvsp[-5].pExpression),blk,(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-7].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 110: /* expression_if_then_else_oneliner: expression_if_one_liner "if" '(' expr ')' expression_else_one_liner SEMICOLON  */
                                                                                                                      {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ast_wrapInBlock((yyvsp[-6].pExpression)),(yyvsp[-1].pExpression) ? ast_wrapInBlock((yyvsp[-1].pExpression)) : nullptr);
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
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-5].pNameWithPosList),(yyvsp[-3].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-8])),tokAt(scanner,(yylsp[0])),(yyvsp[-7].aaList));
    }
    break;

  case 121: /* expression_unsafe: "unsafe" optional_emit_semis expression_block  */
                                                                    {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-2])));
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
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-6])));
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
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-5])));
        pWith->with = (yyvsp[-3].pExpression);
        pWith->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pWith;
    }
    break;

  case 126: /* expression_with: with_keyword_on "with" '(' "module" require_module_name ')' optional_emit_semis expression_block  */
                                                                                                                               {
        yyextra->das_keyword = false;
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-6])));
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
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-2].s);
    }
    break;

  case 128: /* expression_with_alias: "typedef" "name" '=' type_declaration  */
                                                                {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), TypeDeclPtr((yyvsp[0].pTypeDecl)));
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
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else if ( (yyvsp[-1].pExpression)->rtti_isNamedCall() ) {
            // piped block on a named call: pad-aware resolution lands it on the block param
            auto nc = (ExprNamedCall *) (yyvsp[-1].pExpression);
            nc->nonNamedArguments.push_back((yyvsp[0].pExpression));
            nc->pipedCallArgument = true;
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
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else if ( (yyvsp[-1].pExpression)->rtti_isNamedCall() ) {
            // piped block on a named call: pad-aware resolution lands it on the block param
            auto nc = (ExprNamedCall *) (yyvsp[-1].pExpression);
            nc->nonNamedArguments.push_back((yyvsp[0].pExpression));
            nc->pipedCallArgument = true;
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
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else if ( (yyvsp[-1].pExpression)->rtti_isNamedCall() ) {
            // piped block on a named call: pad-aware resolution lands it on the block param
            auto nc = (ExprNamedCall *) (yyvsp[-1].pExpression);
            nc->nonNamedArguments.push_back((yyvsp[0].pExpression));
            nc->pipedCallArgument = true;
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
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 388: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
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
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 394: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 395: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 396: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 397: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 398: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
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
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 403: /* expression_return: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 404: /* expression_yield: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 405: /* expression_yield: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 406: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
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
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-8])),(yyvsp[-1].pExpression),(yyvsp[-5].pTypeDecl));
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
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-8])),(yyvsp[-1].pExpression),(yyvsp[-5].pTypeDecl));
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
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-8])),(yyvsp[-1].pExpression),(yyvsp[-5].pTypeDecl));
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
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),ptd->typeexpr);
                // gc_node — don't delete Expression
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),(yyvsp[-1].pExpression));
            }
            delete (yyvsp[-3].s);
    }
    break;

  case 434: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
                                                                                                {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),ptd->typeexpr,*(yyvsp[-4].s));
                // gc_node — don't delete Expression
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),(yyvsp[-1].pExpression),*(yyvsp[-4].s));
            }
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 435: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" c_or_s "name" '>' '(' expr ')'  */
                                                                                                                        {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),ptd->typeexpr,*(yyvsp[-6].s),*(yyvsp[-4].s));
                // gc_node — don't delete Expression
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),(yyvsp[-1].pExpression),*(yyvsp[-6].s),*(yyvsp[-4].s));
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
            (yyval.pExpression) = ast_makeMoveArgument(scanner, (yyvsp[0].pExpression), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 438: /* expr_list: expr_list ',' expr  */
                                        {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 439: /* expr_list: expr_list ',' "<-" expr  */
                                                   {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-3])),(yyvsp[-3].pExpression),ast_makeMoveArgument(scanner, (yyvsp[0].pExpression), tokAt(scanner,(yylsp[0]))));
    }
    break;

  case 440: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 441: /* block_or_simple_block: "=>" expr_no_bracket  */
                                                   {
            auto retE = new ExprReturn(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->generated = true;
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 442: /* block_or_simple_block: "=>" "<-" expr_no_bracket  */
                                                          {
            auto retE = new ExprReturn(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
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
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 456: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 457: /* expr_full_block_assumed_piped: '{' expressions '}'  */
                                   {
        // block span is brace-to-brace (@$), not the statements' span (@block)
        (yyval.pExpression) = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc))),(yyvsp[-1].pExpression),tokAt(scanner,(yyloc)),tokAt(scanner,(yyloc)),LineInfo());
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
                                             { (yyval.pExpression) = newConstLiteral<ExprConstInt64>(tokAt(scanner,(yylsp[0])),int64_t(INT64_MIN)); }
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
                                                                   { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 469: /* expr_assign_no_bracket: expr_no_bracket "<-" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr_assign_no_bracket: expr_no_bracket "<-" make_table_decl  */
                                                                   { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 471: /* expr_assign_no_bracket: expr_no_bracket "<-" array_comprehension  */
                                                                     { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr_assign_no_bracket: expr_no_bracket ":=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 473: /* expr_assign_no_bracket: expr_no_bracket "!==" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),true); }
    break;

  case 474: /* expr_assign_no_bracket: expr_no_bracket "!<-" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),true); }
    break;

  case 475: /* expr_assign_no_bracket: expr_no_bracket "!<-" make_table_decl  */
                                                                      { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),true); }
    break;

  case 476: /* expr_assign_no_bracket: expr_no_bracket "!<-" array_comprehension  */
                                                                        { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),true); }
    break;

  case 477: /* expr_assign_no_bracket: expr_no_bracket "!:=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),true); }
    break;

  case 478: /* expr_assign_no_bracket: expr_no_bracket "&=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 479: /* expr_assign_no_bracket: expr_no_bracket "|=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 480: /* expr_assign_no_bracket: expr_no_bracket "^=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 481: /* expr_assign_no_bracket: expr_no_bracket "&&=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 482: /* expr_assign_no_bracket: expr_no_bracket "||=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 483: /* expr_assign_no_bracket: expr_no_bracket "^^=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 484: /* expr_assign_no_bracket: expr_no_bracket "+=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 485: /* expr_assign_no_bracket: expr_no_bracket "-=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 486: /* expr_assign_no_bracket: expr_no_bracket "*=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 487: /* expr_assign_no_bracket: expr_no_bracket "/=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 488: /* expr_assign_no_bracket: expr_no_bracket "%=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 489: /* expr_assign_no_bracket: expr_no_bracket "<<=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 490: /* expr_assign_no_bracket: expr_no_bracket ">>=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 491: /* expr_assign_no_bracket: expr_no_bracket "<<<=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 492: /* expr_assign_no_bracket: expr_no_bracket ">>>=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 493: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 494: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 495: /* expr_named_call: name_in_namespace '(' expr_list ',' make_struct_fields ')'  */
                                                                                          {
        // bracket-less mixed named call: foo(pos..., name = value) -- named args are a strict suffix
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-3].pExpression));
        nc->arguments = (yyvsp[-1].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 496: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' ')'  */
                                                                    {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 497: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' expr_list ')'  */
                                                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 498: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                                     {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
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
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
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
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
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
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
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
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 511: /* expr_field_no_bracket: expr_no_bracket '.' '.' "name"  */
                                                             {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 512: /* expr_field_no_bracket: expr_no_bracket "!." "name"  */
                                                            {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 513: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' ')'  */
                                                                 {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 514: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' expr_list ')'  */
                                                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 515: /* expr_field_no_bracket: expr_no_bracket "!." "name" '(' ')'  */
                                                                    {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), *(yyvsp[-2].s), true);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 516: /* expr_field_no_bracket: expr_no_bracket "!." "name" '(' expr_list ')'  */
                                                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), *(yyvsp[-3].s), true);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 517: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
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
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
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
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
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
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 521: /* expr_field_no_bracket: expr_no_bracket '.' basic_type_declaration '(' expr_list ')'  */
                                                                                                        {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), method_name);
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
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 525: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 526: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
                                                          {
            auto dd = new ExprMakeStruct(tokAt(scanner,(yylsp[-3])));
            dd->at = tokAt(scanner,(yylsp[-3]));
            dd->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            dd->useInitializer = false;
            dd->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = dd;
    }
    break;

  case 527: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 528: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 529: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 530: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 531: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
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
                                                         { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 546: /* expr_no_bracket: '~' expr_no_bracket  */
                                                         { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 547: /* expr_no_bracket: '+' expr_no_bracket  */
                                                             { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 548: /* expr_no_bracket: '-' expr_no_bracket  */
                                                             { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 549: /* expr_no_bracket: expr_no_bracket "<<" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 550: /* expr_no_bracket: expr_no_bracket ">>" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 551: /* expr_no_bracket: expr_no_bracket "<<<" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 552: /* expr_no_bracket: expr_no_bracket ">>>" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 553: /* expr_no_bracket: expr_no_bracket '+' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 554: /* expr_no_bracket: expr_no_bracket '-' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 555: /* expr_no_bracket: expr_no_bracket '*' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 556: /* expr_no_bracket: expr_no_bracket '/' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 557: /* expr_no_bracket: expr_no_bracket '%' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 558: /* expr_no_bracket: expr_no_bracket '<' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 559: /* expr_no_bracket: expr_no_bracket '>' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 560: /* expr_no_bracket: expr_no_bracket "==" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 561: /* expr_no_bracket: expr_no_bracket "!=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 562: /* expr_no_bracket: expr_no_bracket "<=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 563: /* expr_no_bracket: expr_no_bracket ">=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 564: /* expr_no_bracket: expr_no_bracket '&' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 565: /* expr_no_bracket: expr_no_bracket '|' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 566: /* expr_no_bracket: expr_no_bracket '^' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 567: /* expr_no_bracket: expr_no_bracket "&&" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 568: /* expr_no_bracket: expr_no_bracket "||" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 569: /* expr_no_bracket: expr_no_bracket "^^" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 570: /* expr_no_bracket: expr_no_bracket ".." expr_no_bracket  */
                                                                   {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 571: /* expr_no_bracket: "++" expr_no_bracket  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 572: /* expr_no_bracket: "--" expr_no_bracket  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 573: /* expr_no_bracket: expr_no_bracket "++"  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 574: /* expr_no_bracket: expr_no_bracket "--"  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 575: /* expr_no_bracket: '(' expr_list optional_comma ')'  */
                                                         {
            if ( (yyvsp[-2].pExpression)->rtti_isSequence() ) {
                auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
                mkt->values = sequenceToList((yyvsp[-2].pExpression));
                mkt->shorthandRecordNames = ast_tupleCollectShorthandNames(mkt->values);
                (yyval.pExpression) = mkt;
            } else if ( (yyvsp[-1].b) ) {
                auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
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
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        for ( auto & arg : *(((ExprMakeStruct *)(yyvsp[-1].pExpression))->structs.back()) ) {
            mkt->values.push_back(arg->value);
            mkt->recordNames.push_back(arg->name);
        }
        // gc_node — don't delete Expression
        (yyval.pExpression) = mkt;
    }
    break;

  case 577: /* expr_no_bracket: expr_no_bracket '[' expr ']'  */
                                                            { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 578: /* expr_no_bracket: expr_no_bracket '.' '[' expr ']'  */
                                                                { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 579: /* expr_no_bracket: expr_no_bracket "![" expr ']'  */
                                                               { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 580: /* expr_no_bracket: expr_no_bracket "?[" expr ']'  */
                                                            { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 581: /* expr_no_bracket: expr_no_bracket '.' "?[" expr ']'  */
                                                                { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 582: /* expr_no_bracket: expr_no_bracket "!?[" expr ']'  */
                                                               { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 583: /* expr_no_bracket: expr_no_bracket "?." "name"  */
                                                            { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 584: /* expr_no_bracket: expr_no_bracket '.' "?." "name"  */
                                                                { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 585: /* expr_no_bracket: expr_no_bracket "!?." "name"  */
                                                               { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 586: /* expr_no_bracket: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 587: /* expr_no_bracket: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 588: /* expr_no_bracket: '*' expr_no_bracket  */
                                                              { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 589: /* expr_no_bracket: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 590: /* expr_no_bracket: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 591: /* $@38: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 592: /* $@39: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 593: /* expr_no_bracket: "addr" '<' $@38 type_declaration_no_options '>' $@39 '(' expr ')'  */
                                                                                                                                                        {
        auto pRef2Ptr = new ExprRef2Ptr(tokAt(scanner,(yylsp[-8])),(yyvsp[-1].pExpression));
        pRef2Ptr->generated = true;
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-8])),pRef2Ptr,(yyvsp[-5].pTypeDecl));
        pCast->reinterpret = true;
        pCast->fromAddrSugar = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 594: /* expr_no_bracket: expr_generator  */
                                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 595: /* expr_no_bracket: expr_no_bracket "??" expr_no_bracket  */
                                                                         { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 596: /* expr_no_bracket: expr_no_bracket "!??" expr_no_bracket  */
                                                                            {
        auto nc = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        nc->no_promotion = true;
        (yyval.pExpression) = nc;
    }
    break;

  case 597: /* expr_no_bracket: expr_no_bracket '?' expr_no_bracket ':' expr_no_bracket  */
                                                                                           {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
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
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 601: /* expr_no_bracket: expr_no_bracket "is" basic_type_declaration  */
                                                                          {
        auto vdecl = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc)));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 602: /* expr_no_bracket: expr_no_bracket "is" "name"  */
                                                         {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
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
        auto isx = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
        isx->no_promotion = true;
        (yyval.pExpression) = isx;
    }
    break;

  case 606: /* expr_no_bracket: expr_no_bracket "!is" basic_type_declaration  */
                                                                         {
        auto vdecl = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc)));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        auto isx = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
        isx->no_promotion = true;
        (yyval.pExpression) = isx;
    }
    break;

  case 607: /* expr_no_bracket: expr_no_bracket "!is" "name"  */
                                                        {
        auto isv = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        isv->no_promotion = true;
        (yyval.pExpression) = isv;
        delete (yyvsp[0].s);
    }
    break;

  case 608: /* expr_no_bracket: expr_no_bracket "as" "name"  */
                                                         {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
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
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 612: /* expr_no_bracket: expr_no_bracket "as" basic_type_declaration  */
                                                                          {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 613: /* expr_no_bracket: expr_no_bracket "!as" "name"  */
                                                        {
        auto asv = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
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
        auto asv = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 617: /* expr_no_bracket: expr_no_bracket "!as" basic_type_declaration  */
                                                                         {
        auto asv = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
    }
    break;

  case 618: /* expr_no_bracket: expr_no_bracket '?' "as" "name"  */
                                                             {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
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
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 622: /* expr_no_bracket: expr_no_bracket '?' "as" basic_type_declaration  */
                                                                              {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 623: /* expr_no_bracket: expr_no_bracket "!?as" "name"  */
                                                         {
        auto asv = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
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
        auto asv = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 627: /* expr_no_bracket: expr_no_bracket "!?as" basic_type_declaration  */
                                                                          {
        auto asv = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
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
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 641: /* expr_no_bracket: expr_no_bracket "=>" make_table_decl  */
                                                               {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 642: /* expr_no_bracket: expr_no_bracket "=>" array_comprehension  */
                                                                   {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
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
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 645: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list optional_emit_semis expression_block  */
                                                                                                                                                  {
        auto closure = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->returnType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),closure,tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 646: /* expr_mtag_no_bracket: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 647: /* expr_mtag_no_bracket: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 648: /* expr_mtag_no_bracket: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 649: /* expr_mtag_no_bracket: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 650: /* expr_mtag_no_bracket: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 651: /* expr_mtag_no_bracket: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 652: /* expr_mtag_no_bracket: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 653: /* expr_mtag_no_bracket: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 654: /* expr_mtag_no_bracket: expr_no_bracket '.' "$f" '(' expr ')'  */
                                                                           {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 655: /* expr_mtag_no_bracket: expr_no_bracket "?." "$f" '(' expr ')'  */
                                                                            {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 656: /* expr_mtag_no_bracket: expr_no_bracket '.' '.' "$f" '(' expr ')'  */
                                                                               {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 657: /* expr_mtag_no_bracket: expr_no_bracket "!." "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 658: /* expr_mtag_no_bracket: expr_no_bracket '.' "?." "$f" '(' expr ')'  */
                                                                                {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 659: /* expr_mtag_no_bracket: expr_no_bracket "!?." "$f" '(' expr ')'  */
                                                                               {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 660: /* expr_mtag_no_bracket: expr_no_bracket "as" "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 661: /* expr_mtag_no_bracket: expr_no_bracket "!as" "$f" '(' expr ')'  */
                                                                             {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        cfield->no_promotion = true;
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 662: /* expr_mtag_no_bracket: expr_no_bracket '?' "as" "$f" '(' expr ')'  */
                                                                                  {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 663: /* expr_mtag_no_bracket: expr_no_bracket "!?as" "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        cfield->no_promotion = true;
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 664: /* expr_mtag_no_bracket: expr_no_bracket "is" "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 665: /* expr_mtag_no_bracket: expr_no_bracket "!is" "$f" '(' expr ')'  */
                                                                             {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        cfield->no_promotion = true;
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 666: /* expr_mtag_no_bracket: "@@" "$c" '(' expr ')'  */
                                                           {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
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

  case 705: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 706: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 707: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 708: /* variant_alias_type_list: variant_type  */
                         {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 709: /* variant_alias_type_list: variant_alias_type_list semis variant_type  */
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

  case 710: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 711: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 712: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[0])));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 713: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-1])));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 714: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-2])));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 715: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 716: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 717: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 718: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 719: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 720: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 721: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 722: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 723: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 724: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 725: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 726: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 727: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 728: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                               {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 729: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 730: /* global_let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 731: /* global_let_variable_name_with_pos_list: global_let_variable_name_with_pos_list ',' "name"  */
                                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 732: /* variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 733: /* variable_declaration_list: variable_declaration_list SEMICOLON  */
                                                  {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 734: /* variable_declaration_list: variable_declaration_list let_variable_declaration  */
                                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 735: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 736: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 737: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-4])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 738: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                         {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 739: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                               {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 740: /* global_let_variable_declaration: global_let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-4])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 741: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 742: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 743: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 744: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 745: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 746: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 747: /* global_variable_declaration_list: global_variable_declaration_list SEMICOLON  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 748: /* $@55: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 749: /* global_variable_declaration_list: global_variable_declaration_list $@55 optional_field_annotation let_variable_declaration  */
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

  case 750: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 751: /* $@56: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 752: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@56 optional_field_annotation global_let_variable_declaration  */
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

  case 753: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        delete (yyvsp[0].s);
    }
    break;

  case 754: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
        delete (yyvsp[-2].s);
    }
    break;

  case 757: /* enum_list: %empty  */
        {
        (yyval.pEnumList) = new Enumeration();
    }
    break;

  case 758: /* enum_list: enum_expression  */
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

  case 759: /* enum_list: enum_list commas enum_expression  */
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

  case 760: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 761: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 762: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 763: /* $@57: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 764: /* single_alias: optional_public_or_private_alias "name" $@57 '=' type_declaration  */
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

  case 767: /* $@58: %empty  */
                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 768: /* distinct_alias: optional_public_or_private_alias "name" "name" $@58 '=' type_declaration  */
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

  case 769: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 770: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 771: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 772: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 773: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 774: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 781: /* $@59: %empty  */
                                                                     {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 782: /* $@60: %empty  */
                                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 783: /* $@61: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 784: /* enum_declaration: optional_annotation_list_with_emit_semis "enum" $@59 optional_public_or_private_enum enum_name optional_enum_basic_type_declaration optional_emit_commas '{' $@60 enum_list optional_commas $@61 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-8].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-12].faList),tokAt(scanner,(yylsp[-12])),(yyvsp[-9].b),(yyvsp[-8].pEnum),(yyvsp[-3].pEnumList),(yyvsp[-7].type));
    }
    break;

  case 785: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 786: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 787: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 788: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 789: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 790: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 791: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 792: /* class_or_struct: "class" "template"  */
                                  { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 793: /* class_or_struct: "struct" "template"  */
                                  { (yyval.i) = CorS_StructTemplate; }
    break;

  case 794: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 795: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 796: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 797: /* optional_struct_variable_declaration_list: ';'  */
            {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 798: /* optional_struct_variable_declaration_list: '{' struct_variable_declaration_list '}'  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 799: /* $@62: %empty  */
                                                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 800: /* $@63: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 801: /* $@64: %empty  */
                                             {
        if ( (yyvsp[-1].pStructure) ) {
            (yyvsp[-1].pStructure)->isClass = (yyvsp[-4].i)==CorS_Class || (yyvsp[-4].i)==CorS_ClassTemplate;
            (yyvsp[-1].pStructure)->isTemplate = (yyvsp[-4].i)==CorS_ClassTemplate || (yyvsp[-4].i)==CorS_StructTemplate;
            (yyvsp[-1].pStructure)->privateStructure = !(yyvsp[-3].b);
        }
    }
    break;

  case 802: /* structure_declaration: optional_annotation_list_with_emit_semis $@62 class_or_struct optional_public_or_private_structure $@63 structure_name optional_emit_semis $@64 optional_struct_variable_declaration_list  */
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

  case 803: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 804: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 805: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 806: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 807: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                           {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 808: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 809: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 810: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 811: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 812: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 813: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 814: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 815: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 816: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 817: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 818: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 819: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 820: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 821: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 822: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 823: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 824: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 825: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 826: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 827: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 828: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 829: /* basic_type_declaration: "float16"  */
                        { (yyval.type) = Type::tFloat16; }
    break;

  case 830: /* basic_type_declaration: "half2"  */
                        { (yyval.type) = Type::tHalf2; }
    break;

  case 831: /* basic_type_declaration: "half3"  */
                        { (yyval.type) = Type::tHalf3; }
    break;

  case 832: /* basic_type_declaration: "half4"  */
                        { (yyval.type) = Type::tHalf4; }
    break;

  case 833: /* basic_type_declaration: "half8"  */
                        { (yyval.type) = Type::tHalf8; }
    break;

  case 834: /* basic_type_declaration: "short2"  */
                        { (yyval.type) = Type::tShort2; }
    break;

  case 835: /* basic_type_declaration: "short3"  */
                        { (yyval.type) = Type::tShort3; }
    break;

  case 836: /* basic_type_declaration: "short4"  */
                        { (yyval.type) = Type::tShort4; }
    break;

  case 837: /* basic_type_declaration: "short8"  */
                        { (yyval.type) = Type::tShort8; }
    break;

  case 838: /* basic_type_declaration: "ushort2"  */
                        { (yyval.type) = Type::tUShort2; }
    break;

  case 839: /* basic_type_declaration: "ushort3"  */
                        { (yyval.type) = Type::tUShort3; }
    break;

  case 840: /* basic_type_declaration: "ushort4"  */
                        { (yyval.type) = Type::tUShort4; }
    break;

  case 841: /* basic_type_declaration: "ushort8"  */
                        { (yyval.type) = Type::tUShort8; }
    break;

  case 842: /* basic_type_declaration: "byte2"  */
                        { (yyval.type) = Type::tByte2; }
    break;

  case 843: /* basic_type_declaration: "byte3"  */
                        { (yyval.type) = Type::tByte3; }
    break;

  case 844: /* basic_type_declaration: "byte4"  */
                        { (yyval.type) = Type::tByte4; }
    break;

  case 845: /* basic_type_declaration: "byte8"  */
                        { (yyval.type) = Type::tByte8; }
    break;

  case 846: /* basic_type_declaration: "byte16"  */
                        { (yyval.type) = Type::tByte16; }
    break;

  case 847: /* basic_type_declaration: "ubyte2"  */
                        { (yyval.type) = Type::tUByte2; }
    break;

  case 848: /* basic_type_declaration: "ubyte3"  */
                        { (yyval.type) = Type::tUByte3; }
    break;

  case 849: /* basic_type_declaration: "ubyte4"  */
                        { (yyval.type) = Type::tUByte4; }
    break;

  case 850: /* basic_type_declaration: "ubyte8"  */
                        { (yyval.type) = Type::tUByte8; }
    break;

  case 851: /* basic_type_declaration: "ubyte16"  */
                        { (yyval.type) = Type::tUByte16; }
    break;

  case 852: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 853: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 854: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 855: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 856: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 857: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 858: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 859: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 860: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 861: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 862: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 863: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 864: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 865: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 866: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 867: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 868: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 869: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 870: /* auto_type_declaration: "$t" '(' expr ')'  */
                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::alias, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = "``MACRO``TAG``";
        (yyval.pTypeDecl)->isTag = true;
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner, (yylsp[-1]));
        (yyval.pTypeDecl)->firstType->typeMacroExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 871: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 872: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 873: /* bitfield_bits: bitfield_bits ',' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 874: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<tuple<string,Expression *>>();
        (yyval.pNameExprList) = pSL;

    }
    break;

  case 875: /* bitfield_alias_bits: "name"  */
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

  case 876: /* bitfield_alias_bits: "name" '=' expr  */
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

  case 877: /* bitfield_alias_bits: bitfield_alias_bits commas "name"  */
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

  case 878: /* bitfield_alias_bits: bitfield_alias_bits commas "name" '=' expr  */
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

  case 879: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 880: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 881: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 882: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 883: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 884: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' '>'  */
                                                                          {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-2].type), tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-2]));
    }
    break;

  case 885: /* $@65: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 886: /* $@66: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 887: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@65 bitfield_bits '>' $@66  */
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

  case 890: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.aTypePair).secondType->at = (yyval.aTypePair).firstType->at;
    }
    break;

  case 891: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 892: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = appendDimExpr(nullptr, (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 893: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = appendDimExpr(nullptr, nullptr, tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 894: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = appendDimExpr((yyvsp[-3].pTypeDecl), (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 895: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = appendDimExpr((yyvsp[-2].pTypeDecl), nullptr, tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 896: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 897: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
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

  case 898: /* optional_expr_list_in_braces: %empty  */
            { (yyval.pExpression) = nullptr; }
    break;

  case 899: /* optional_expr_list_in_braces: '(' expr_list optional_comma ')'  */
                                                { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 900: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc))); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 901: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 902: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 903: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 904: /* $@67: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 905: /* $@68: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 906: /* type_declaration_no_options_no_dim: "type" '<' $@67 type_declaration '>' $@68  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 907: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->typeMacroExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 908: /* type_declaration_no_options_no_dim: name_in_namespace '(' optional_expr_list ')'  */
                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->typeMacroExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 909: /* type_declaration_no_options_no_dim: '$' name_in_namespace optional_expr_list_in_braces  */
                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-1]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = sequenceToList((yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-1])), *(yyvsp[-1].s)));
        delete (yyvsp[-1].s);
    }
    break;

  case 910: /* $@69: %empty  */
                                    { yyextra->das_arrow_depth ++; }
    break;

  case 911: /* type_declaration_no_options_no_dim: name_in_namespace '<' $@69 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 912: /* $@70: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 913: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@70 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 914: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 915: /* type_declaration_no_options_no_dim: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 916: /* type_declaration_no_options_no_dim: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 917: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 918: /* type_declaration_no_options_no_dim: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 919: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 920: /* type_declaration_no_options_no_dim: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 921: /* type_declaration_no_options_no_dim: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 922: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 923: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 924: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 925: /* type_declaration_no_options_no_dim: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 926: /* $@71: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 927: /* $@72: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 928: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@71 type_declaration '>' $@72  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 929: /* type_declaration_no_options_no_dim: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 930: /* $@73: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 931: /* $@74: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 932: /* type_declaration_no_options_no_dim: "array" '<' $@73 type_declaration '>' $@74  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 933: /* $@75: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 934: /* $@76: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 935: /* type_declaration_no_options_no_dim: "table" '<' $@75 table_type_pair '>' $@76  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 936: /* $@77: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 937: /* $@78: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 938: /* type_declaration_no_options_no_dim: "iterator" '<' $@77 type_declaration '>' $@78  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 939: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 940: /* $@79: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 941: /* $@80: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 942: /* type_declaration_no_options_no_dim: "block" '<' $@79 type_declaration '>' $@80  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 943: /* $@81: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 944: /* $@82: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 945: /* type_declaration_no_options_no_dim: "block" '<' $@81 optional_function_argument_list optional_function_type '>' $@82  */
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

  case 946: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 947: /* $@83: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 948: /* $@84: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 949: /* type_declaration_no_options_no_dim: "function" '<' $@83 type_declaration '>' $@84  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 950: /* $@85: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 951: /* $@86: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 952: /* type_declaration_no_options_no_dim: "function" '<' $@85 optional_function_argument_list optional_function_type '>' $@86  */
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

  case 953: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 954: /* $@87: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 955: /* $@88: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 956: /* type_declaration_no_options_no_dim: "lambda" '<' $@87 type_declaration '>' $@88  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 957: /* $@89: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 958: /* $@90: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 959: /* type_declaration_no_options_no_dim: "lambda" '<' $@89 optional_function_argument_list optional_function_type '>' $@90  */
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

  case 960: /* $@91: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 961: /* $@92: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 962: /* type_declaration_no_options_no_dim: "tuple" '<' $@91 tuple_type_list '>' $@92  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 963: /* $@93: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 964: /* $@94: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 965: /* type_declaration_no_options_no_dim: "variant" '<' $@93 variant_type_list '>' $@94  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 966: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 967: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 968: /* type_declaration: type_declaration '|' '#'  */
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

  case 969: /* $@95: %empty  */
                   {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 970: /* $@96: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 971: /* $@97: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 972: /* $@98: %empty  */
                                                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 973: /* tuple_alias_declaration: "tuple" $@95 optional_public_or_private_alias "name" optional_emit_semis $@96 '{' $@97 tuple_alias_type_list optional_semis $@98 '}'  */
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

  case 974: /* $@99: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 975: /* $@100: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 976: /* $@101: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 977: /* $@102: %empty  */
                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 978: /* variant_alias_declaration: "variant" $@99 optional_public_or_private_alias "name" optional_emit_semis $@100 '{' $@101 variant_alias_type_list optional_semis $@102 '}'  */
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

  case 979: /* $@103: %empty  */
                      {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 980: /* $@104: %empty  */
                                                                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 981: /* $@105: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 982: /* $@106: %empty  */
                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 983: /* bitfield_alias_declaration: "bitfield" $@103 optional_public_or_private_alias "name" bitfield_basic_type_declaration optional_emit_commas $@104 '{' $@105 bitfield_alias_bits optional_commas $@106 '}'  */
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

  case 984: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 985: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 986: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 987: /* make_decl: make_table_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 988: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 989: /* make_decl: table_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 990: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 991: /* make_decl_no_bracket: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 992: /* make_decl_no_bracket: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 993: /* make_decl_no_bracket: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 994: /* make_decl_no_bracket: table_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 995: /* make_decl_no_bracket: make_table_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 996: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 997: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 998: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 999: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 1000: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 1001: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 1002: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 1003: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 1004: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 1005: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 1006: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 1007: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 1008: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 1009: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 1010: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1011: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 1012: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 1013: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 1014: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 1015: /* $@107: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 1016: /* $@108: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 1017: /* make_struct_decl: "struct" '<' $@107 type_declaration_no_options '>' $@108 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1018: /* $@109: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 1019: /* $@110: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 1020: /* make_struct_decl: "class" '<' $@109 type_declaration_no_options '>' $@110 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1021: /* $@111: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 1022: /* $@112: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 1023: /* make_struct_decl: "variant" '<' $@111 variant_type_list '>' $@112 '(' use_initializer make_variant_dim ')'  */
                                                                                                                                                                                  {
        auto mkt = new TypeDecl(Type::tVariant, tokAt(scanner,(yylsp[-9])));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1024: /* $@113: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 1025: /* $@114: %empty  */
                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 1026: /* make_struct_decl: "variant" "type" '<' $@113 type_declaration_no_options '>' $@114 '(' use_initializer make_variant_dim ')'  */
                                                                                                                                                                                                    {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
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

  case 1029: /* make_struct_decl: "default" '<' $@115 type_declaration_no_options '>' $@116 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 1030: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pExpression) = mkt;
    }
    break;

  case 1031: /* $@117: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 1032: /* $@118: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 1033: /* make_tuple_call: "tuple" '<' $@117 tuple_type_list '>' $@118 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                 {
        auto mkt = new TypeDecl(Type::tTuple, tokAt(scanner,(yylsp[-9])));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1034: /* make_dim_decl: '[' optional_expr_list ']'  */
                                                  {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-2])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
            mka->gen2 = true;
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto mks = new ExprMakeStruct();
            mks->at = tokAt(scanner,(yylsp[-2]));
            mks->makeType = new TypeDecl(Type::tArray, mks->at);
            mks->makeType->firstType = new TypeDecl(Type::autoinfer, mks->at);
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }
    }
    break;

  case 1035: /* $@119: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 1036: /* $@120: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 1037: /* make_dim_decl: "array" "struct" '<' $@119 type_declaration_no_options '>' $@120 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-10])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 1038: /* $@121: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 1039: /* $@122: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 1040: /* make_dim_decl: "array" "tuple" '<' $@121 tuple_type_list '>' $@122 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        auto mkt = new TypeDecl(Type::tTuple, tokAt(scanner,(yylsp[-10])));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-10])),"to_array_move");
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

  case 1043: /* make_dim_decl: "array" "variant" '<' $@123 variant_type_list '>' $@124 '(' make_variant_dim ')'  */
                                                                                                                                                                      {
        auto mkt = new TypeDecl(Type::tVariant, tokAt(scanner,(yylsp[-9])));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-5].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-5].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 1044: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        mka->gen2 = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 1045: /* $@125: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 1046: /* $@126: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 1047: /* make_dim_decl: "array" '<' $@125 type_declaration_no_options '>' $@126 '(' optional_expr_list ')'  */
                                                                                                                                                                        {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = (yyvsp[-5].pTypeDecl);
            mka->gen2 = true;
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->makeType = new TypeDecl(Type::tArray, msd->at);
            msd->makeType->firstType = (yyvsp[-5].pTypeDecl);
            msd->at = tokAt(scanner,(yylsp[-5]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 1048: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 1049: /* $@127: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 1050: /* $@128: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 1051: /* make_dim_decl: "fixed_array" '<' $@127 type_declaration_no_options '>' $@128 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 1052: /* expr_map_tuple_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 1053: /* expr_map_tuple_list: expr_map_tuple_list ',' expr  */
                                                      {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 1054: /* push_table_nesting: %empty  */
                    {
        yyextra->das_nested_parentheses ++;
    }
    break;

  case 1055: /* make_table_decl: '{' push_table_nesting optional_emit_semis optional_expr_map_tuple_list '}'  */
                                                                                                     {
        yyextra->das_nested_parentheses --;
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto mks = new ExprMakeStruct();
            mks->at = tokAt(scanner,(yylsp[-4]));
            mks->makeType = new TypeDecl(Type::tTable, tokAt(scanner,(yyloc)));
            mks->makeType->firstType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-4])));
            mks->makeType->secondType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[0])));
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }
    }
    break;

  case 1056: /* make_table_call: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 1057: /* make_table_call: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                 {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-6])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = (yyvsp[-4].pTypeDecl);
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->makeType = new TypeDecl(Type::tTable, msd->at);
            msd->makeType->firstType = (yyvsp[-4].pTypeDecl);
            msd->makeType->secondType = new TypeDecl(Type::tVoid, tokAt(scanner,(yylsp[-6])));
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 1058: /* make_table_call: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                                                             {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = new TypeDecl(Type::tTuple, tokAt(scanner,(yyloc)));
            mka->makeType->argTypes.push_back((yyvsp[-6].pTypeDecl));
            mka->makeType->argTypes.push_back((yyvsp[-4].pTypeDecl));
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->makeType = new TypeDecl(Type::tTable, msd->at);
            msd->makeType->firstType = (yyvsp[-6].pTypeDecl);
            msd->makeType->secondType = (yyvsp[-4].pTypeDecl);
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 1059: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 1060: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 1061: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 1062: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 1063: /* table_comprehension: '[' "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                               {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 1064: /* table_comprehension: '[' "iterator" "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 1065: /* array_comprehension: '{' push_table_nesting optional_emit_semis "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where '}'  */
                                                                                                                                                                                                      {
        yyextra->das_nested_parentheses --;
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
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

void das2_yyerror ( DAS2_YYLTYPE * lloc, yyscan_t scanner, const string & error ) {
    if ( !yyextra->das_suppress_errors ) {
        yyextra->g_Program->error(error,"","",LineInfo(yyextra->g_FileAccessStack.back(),
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


