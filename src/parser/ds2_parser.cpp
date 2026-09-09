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
  YYSYMBOL_NOTIS = 159,                    /* "!is"  */
  YYSYMBOL_NOTAS = 160,                    /* "!as"  */
  YYSYMBOL_NOTQAS = 161,                   /* "!?as"  */
  YYSYMBOL_LPIPE = 162,                    /* "<|"  */
  YYSYMBOL_RPIPE = 163,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 164,                 /* ":="  */
  YYSYMBOL_ROTL = 165,                     /* "<<<"  */
  YYSYMBOL_ROTR = 166,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 167,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 168,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 169,                    /* "=>"  */
  YYSYMBOL_DOUBLE_AT = 170,                /* "@@"  */
  YYSYMBOL_AT_FIELD = 171,                 /* "@field"  */
  YYSYMBOL_COLCOL = 172,                   /* "::"  */
  YYSYMBOL_ANDAND = 173,                   /* "&&"  */
  YYSYMBOL_OROR = 174,                     /* "||"  */
  YYSYMBOL_XORXOR = 175,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 176,                /* "&&="  */
  YYSYMBOL_OROREQU = 177,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 178,                /* "^^="  */
  YYSYMBOL_DOTDOT = 179,                   /* ".."  */
  YYSYMBOL_MTAG_E = 180,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 181,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 182,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 183,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 184,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 185,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 186,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 187,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 188,           /* "..."  */
  YYSYMBOL_INTEGER = 189,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 190,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 191,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 192,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 193,            /* "unsigned int8 constant"  */
  YYSYMBOL_DAS_FLOAT = 194,                /* "floating point constant"  */
  YYSYMBOL_DAS_FLOAT16_CONST = 195,        /* "float16 constant"  */
  YYSYMBOL_DOUBLE = 196,                   /* "double constant"  */
  YYSYMBOL_NAME = 197,                     /* "name"  */
  YYSYMBOL_DAS_EMIT_COMMA = 198,           /* "new line, comma"  */
  YYSYMBOL_DAS_EMIT_SEMICOLON = 199,       /* "new line, semicolon"  */
  YYSYMBOL_BEGIN_STRING = 200,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 201,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 202,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 203,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 204,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 205,          /* "}"  */
  YYSYMBOL_END_OF_READ = 206,              /* "end of failed eader macro"  */
  YYSYMBOL_207_ = 207,                     /* ','  */
  YYSYMBOL_208_ = 208,                     /* '='  */
  YYSYMBOL_209_ = 209,                     /* '?'  */
  YYSYMBOL_210_ = 210,                     /* ':'  */
  YYSYMBOL_211_ = 211,                     /* '|'  */
  YYSYMBOL_212_ = 212,                     /* '^'  */
  YYSYMBOL_213_ = 213,                     /* '&'  */
  YYSYMBOL_214_ = 214,                     /* '<'  */
  YYSYMBOL_215_ = 215,                     /* '>'  */
  YYSYMBOL_216_ = 216,                     /* '-'  */
  YYSYMBOL_217_ = 217,                     /* '+'  */
  YYSYMBOL_218_ = 218,                     /* '*'  */
  YYSYMBOL_219_ = 219,                     /* '/'  */
  YYSYMBOL_220_ = 220,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 221,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 222,               /* UNARY_PLUS  */
  YYSYMBOL_223_ = 223,                     /* '~'  */
  YYSYMBOL_224_ = 224,                     /* '!'  */
  YYSYMBOL_PRE_INC = 225,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 226,                  /* PRE_DEC  */
  YYSYMBOL_LLPIPE = 227,                   /* LLPIPE  */
  YYSYMBOL_POST_INC = 228,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 229,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 230,                    /* DEREF  */
  YYSYMBOL_231_ = 231,                     /* '.'  */
  YYSYMBOL_232_ = 232,                     /* '['  */
  YYSYMBOL_233_ = 233,                     /* ']'  */
  YYSYMBOL_234_ = 234,                     /* '('  */
  YYSYMBOL_235_ = 235,                     /* ')'  */
  YYSYMBOL_236_ = 236,                     /* '$'  */
  YYSYMBOL_237_ = 237,                     /* '@'  */
  YYSYMBOL_238_ = 238,                     /* ';'  */
  YYSYMBOL_239_ = 239,                     /* '{'  */
  YYSYMBOL_240_ = 240,                     /* '}'  */
  YYSYMBOL_241_ = 241,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 242,                 /* $accept  */
  YYSYMBOL_program = 243,                  /* program  */
  YYSYMBOL_COMMA = 244,                    /* COMMA  */
  YYSYMBOL_SEMICOLON = 245,                /* SEMICOLON  */
  YYSYMBOL_top_level_reader_macro = 246,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 247, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 248,              /* module_name  */
  YYSYMBOL_optional_not_required = 249,    /* optional_not_required  */
  YYSYMBOL_module_declaration = 250,       /* module_declaration  */
  YYSYMBOL_character_sequence = 251,       /* character_sequence  */
  YYSYMBOL_string_constant = 252,          /* string_constant  */
  YYSYMBOL_format_string = 253,            /* format_string  */
  YYSYMBOL_optional_format_string = 254,   /* optional_format_string  */
  YYSYMBOL_255_1 = 255,                    /* $@1  */
  YYSYMBOL_string_builder_body = 256,      /* string_builder_body  */
  YYSYMBOL_string_builder = 257,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 258, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 259,              /* expr_reader  */
  YYSYMBOL_260_2 = 260,                    /* $@2  */
  YYSYMBOL_options_declaration = 261,      /* options_declaration  */
  YYSYMBOL_require_declaration = 262,      /* require_declaration  */
  YYSYMBOL_require_module_name = 263,      /* require_module_name  */
  YYSYMBOL_optional_require_guard = 264,   /* optional_require_guard  */
  YYSYMBOL_require_module = 265,           /* require_module  */
  YYSYMBOL_is_public_module = 266,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 267,       /* expect_declaration  */
  YYSYMBOL_expect_list = 268,              /* expect_list  */
  YYSYMBOL_expect_error = 269,             /* expect_error  */
  YYSYMBOL_expression_label = 270,         /* expression_label  */
  YYSYMBOL_expression_goto = 271,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 272,      /* elif_or_static_elif  */
  YYSYMBOL_emit_semis = 273,               /* emit_semis  */
  YYSYMBOL_optional_emit_semis = 274,      /* optional_emit_semis  */
  YYSYMBOL_expression_else = 275,          /* expression_else  */
  YYSYMBOL_276_3 = 276,                    /* $@3  */
  YYSYMBOL_277_4 = 277,                    /* $@4  */
  YYSYMBOL_if_or_static_if = 278,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 279, /* expression_else_one_liner  */
  YYSYMBOL_expression_if_one_liner = 280,  /* expression_if_one_liner  */
  YYSYMBOL_semis = 281,                    /* semis  */
  YYSYMBOL_optional_semis = 282,           /* optional_semis  */
  YYSYMBOL_expression_if_block = 283,      /* expression_if_block  */
  YYSYMBOL_284_5 = 284,                    /* $@5  */
  YYSYMBOL_285_6 = 285,                    /* $@6  */
  YYSYMBOL_286_7 = 286,                    /* $@7  */
  YYSYMBOL_expression_else_block = 287,    /* expression_else_block  */
  YYSYMBOL_288_8 = 288,                    /* $@8  */
  YYSYMBOL_289_9 = 289,                    /* $@9  */
  YYSYMBOL_290_10 = 290,                   /* $@10  */
  YYSYMBOL_expression_if_then_else = 291,  /* expression_if_then_else  */
  YYSYMBOL_292_11 = 292,                   /* $@11  */
  YYSYMBOL_293_12 = 293,                   /* $@12  */
  YYSYMBOL_expression_if_then_else_oneliner = 294, /* expression_if_then_else_oneliner  */
  YYSYMBOL_for_variable_name_with_pos_list = 295, /* for_variable_name_with_pos_list  */
  YYSYMBOL_expression_for_loop = 296,      /* expression_for_loop  */
  YYSYMBOL_297_13 = 297,                   /* $@13  */
  YYSYMBOL_expression_unsafe = 298,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 299,    /* expression_while_loop  */
  YYSYMBOL_300_14 = 300,                   /* $@14  */
  YYSYMBOL_with_keyword_on = 301,          /* with_keyword_on  */
  YYSYMBOL_expression_with = 302,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 303,    /* expression_with_alias  */
  YYSYMBOL_annotation_argument_value = 304, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 305, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 306, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 307,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 308, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 309,   /* metadata_argument_list  */
  YYSYMBOL_optional_for_annotations = 310, /* optional_for_annotations  */
  YYSYMBOL_annotation_declaration_name = 311, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 312, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 313,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 314,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 315, /* optional_annotation_list  */
  YYSYMBOL_optional_annotation_list_with_emit_semis = 316, /* optional_annotation_list_with_emit_semis  */
  YYSYMBOL_optional_function_argument_list = 317, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 318,   /* optional_function_type  */
  YYSYMBOL_function_name = 319,            /* function_name  */
  YYSYMBOL_das_type_name = 320,            /* das_type_name  */
  YYSYMBOL_optional_template = 321,        /* optional_template  */
  YYSYMBOL_global_function_declaration = 322, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 323, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 324, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 325,     /* function_declaration  */
  YYSYMBOL_326_15 = 326,                   /* $@15  */
  YYSYMBOL_expression_block_finally = 327, /* expression_block_finally  */
  YYSYMBOL_328_16 = 328,                   /* $@16  */
  YYSYMBOL_329_17 = 329,                   /* $@17  */
  YYSYMBOL_expression_block = 330,         /* expression_block  */
  YYSYMBOL_331_18 = 331,                   /* $@18  */
  YYSYMBOL_332_19 = 332,                   /* $@19  */
  YYSYMBOL_expr_call_pipe_no_bracket = 333, /* expr_call_pipe_no_bracket  */
  YYSYMBOL_expression_any = 334,           /* expression_any  */
  YYSYMBOL_335_20 = 335,                   /* $@20  */
  YYSYMBOL_336_21 = 336,                   /* $@21  */
  YYSYMBOL_expressions = 337,              /* expressions  */
  YYSYMBOL_optional_expr_list = 338,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_map_tuple_list = 339, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 340, /* type_declaration_no_options_list  */
  YYSYMBOL_name_in_namespace = 341,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 342,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 343,     /* new_type_declaration  */
  YYSYMBOL_344_22 = 344,                   /* $@22  */
  YYSYMBOL_345_23 = 345,                   /* $@23  */
  YYSYMBOL_expr_new = 346,                 /* expr_new  */
  YYSYMBOL_expression_break = 347,         /* expression_break  */
  YYSYMBOL_expression_continue = 348,      /* expression_continue  */
  YYSYMBOL_expression_return = 349,        /* expression_return  */
  YYSYMBOL_expression_yield = 350,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 351,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 352,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 353,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 354,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 355,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 356, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 357,           /* expression_let  */
  YYSYMBOL_expr_cast = 358,                /* expr_cast  */
  YYSYMBOL_359_24 = 359,                   /* $@24  */
  YYSYMBOL_360_25 = 360,                   /* $@25  */
  YYSYMBOL_361_26 = 361,                   /* $@26  */
  YYSYMBOL_362_27 = 362,                   /* $@27  */
  YYSYMBOL_363_28 = 363,                   /* $@28  */
  YYSYMBOL_364_29 = 364,                   /* $@29  */
  YYSYMBOL_expr_type_decl = 365,           /* expr_type_decl  */
  YYSYMBOL_366_30 = 366,                   /* $@30  */
  YYSYMBOL_367_31 = 367,                   /* $@31  */
  YYSYMBOL_expr_type_info = 368,           /* expr_type_info  */
  YYSYMBOL_expr_list = 369,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 370,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 371,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 372,            /* capture_entry  */
  YYSYMBOL_capture_list = 373,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 374,    /* optional_capture_list  */
  YYSYMBOL_expr_full_block = 375,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 376, /* expr_full_block_assumed_piped  */
  YYSYMBOL_expr_numeric_const = 377,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign_no_bracket = 378,   /* expr_assign_no_bracket  */
  YYSYMBOL_expr_named_call = 379,          /* expr_named_call  */
  YYSYMBOL_expr_method_call_no_bracket = 380, /* expr_method_call_no_bracket  */
  YYSYMBOL_func_addr_name = 381,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 382,           /* func_addr_expr  */
  YYSYMBOL_383_32 = 383,                   /* $@32  */
  YYSYMBOL_384_33 = 384,                   /* $@33  */
  YYSYMBOL_385_34 = 385,                   /* $@34  */
  YYSYMBOL_386_35 = 386,                   /* $@35  */
  YYSYMBOL_expr_field_no_bracket = 387,    /* expr_field_no_bracket  */
  YYSYMBOL_388_36 = 388,                   /* $@36  */
  YYSYMBOL_389_37 = 389,                   /* $@37  */
  YYSYMBOL_expr_call = 390,                /* expr_call  */
  YYSYMBOL_expr = 391,                     /* expr  */
  YYSYMBOL_expr_no_bracket = 392,          /* expr_no_bracket  */
  YYSYMBOL_393_38 = 393,                   /* $@38  */
  YYSYMBOL_394_39 = 394,                   /* $@39  */
  YYSYMBOL_395_40 = 395,                   /* $@40  */
  YYSYMBOL_396_41 = 396,                   /* $@41  */
  YYSYMBOL_397_42 = 397,                   /* $@42  */
  YYSYMBOL_398_43 = 398,                   /* $@43  */
  YYSYMBOL_399_44 = 399,                   /* $@44  */
  YYSYMBOL_400_45 = 400,                   /* $@45  */
  YYSYMBOL_401_46 = 401,                   /* $@46  */
  YYSYMBOL_402_47 = 402,                   /* $@47  */
  YYSYMBOL_403_48 = 403,                   /* $@48  */
  YYSYMBOL_404_49 = 404,                   /* $@49  */
  YYSYMBOL_405_50 = 405,                   /* $@50  */
  YYSYMBOL_406_51 = 406,                   /* $@51  */
  YYSYMBOL_expr_generator = 407,           /* expr_generator  */
  YYSYMBOL_expr_mtag_no_bracket = 408,     /* expr_mtag_no_bracket  */
  YYSYMBOL_optional_field_annotation = 409, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 410,        /* optional_override  */
  YYSYMBOL_optional_constant = 411,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 412, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 413, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 414, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 415, /* struct_variable_declaration_list  */
  YYSYMBOL_416_52 = 416,                   /* $@52  */
  YYSYMBOL_417_53 = 417,                   /* $@53  */
  YYSYMBOL_418_54 = 418,                   /* $@54  */
  YYSYMBOL_function_argument_declaration_no_type = 419, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 420, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 421,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 422,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 423,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 424,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 425,             /* variant_type  */
  YYSYMBOL_variant_type_list = 426,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 427,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 428,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 429, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 430, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 431,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 432,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 433,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 434, /* let_variable_name_with_pos_list  */
  YYSYMBOL_global_let_variable_name_with_pos_list = 435, /* global_let_variable_name_with_pos_list  */
  YYSYMBOL_variable_declaration_list = 436, /* variable_declaration_list  */
  YYSYMBOL_let_variable_declaration = 437, /* let_variable_declaration  */
  YYSYMBOL_global_let_variable_declaration = 438, /* global_let_variable_declaration  */
  YYSYMBOL_optional_shared = 439,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 440, /* optional_public_or_private_variable  */
  YYSYMBOL_global_variable_declaration_list = 441, /* global_variable_declaration_list  */
  YYSYMBOL_442_55 = 442,                   /* $@55  */
  YYSYMBOL_global_let = 443,               /* global_let  */
  YYSYMBOL_444_56 = 444,                   /* $@56  */
  YYSYMBOL_enum_expression = 445,          /* enum_expression  */
  YYSYMBOL_commas = 446,                   /* commas  */
  YYSYMBOL_enum_list = 447,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 448, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 449,             /* single_alias  */
  YYSYMBOL_450_57 = 450,                   /* $@57  */
  YYSYMBOL_alias_declaration = 451,        /* alias_declaration  */
  YYSYMBOL_distinct_alias = 452,           /* distinct_alias  */
  YYSYMBOL_optional_public_or_private_enum = 453, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 454,                /* enum_name  */
  YYSYMBOL_optional_enum_basic_type_declaration = 455, /* optional_enum_basic_type_declaration  */
  YYSYMBOL_optional_commas = 456,          /* optional_commas  */
  YYSYMBOL_emit_commas = 457,              /* emit_commas  */
  YYSYMBOL_optional_emit_commas = 458,     /* optional_emit_commas  */
  YYSYMBOL_enum_declaration = 459,         /* enum_declaration  */
  YYSYMBOL_460_58 = 460,                   /* $@58  */
  YYSYMBOL_461_59 = 461,                   /* $@59  */
  YYSYMBOL_462_60 = 462,                   /* $@60  */
  YYSYMBOL_optional_structure_parent = 463, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 464,          /* optional_sealed  */
  YYSYMBOL_structure_name = 465,           /* structure_name  */
  YYSYMBOL_class_or_struct = 466,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 467, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 468, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 469,    /* structure_declaration  */
  YYSYMBOL_470_61 = 470,                   /* $@61  */
  YYSYMBOL_471_62 = 471,                   /* $@62  */
  YYSYMBOL_472_63 = 472,                   /* $@63  */
  YYSYMBOL_variable_name_with_pos_list = 473, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 474,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 475, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 476, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 477,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 478,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 479,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_basic_type_declaration = 480, /* bitfield_basic_type_declaration  */
  YYSYMBOL_bitfield_type_declaration = 481, /* bitfield_type_declaration  */
  YYSYMBOL_482_64 = 482,                   /* $@64  */
  YYSYMBOL_483_65 = 483,                   /* $@65  */
  YYSYMBOL_c_or_s = 484,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 485,          /* table_type_pair  */
  YYSYMBOL_dim_list = 486,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 487, /* type_declaration_no_options  */
  YYSYMBOL_optional_expr_list_in_braces = 488, /* optional_expr_list_in_braces  */
  YYSYMBOL_type_declaration_no_options_no_dim = 489, /* type_declaration_no_options_no_dim  */
  YYSYMBOL_490_66 = 490,                   /* $@66  */
  YYSYMBOL_491_67 = 491,                   /* $@67  */
  YYSYMBOL_492_68 = 492,                   /* $@68  */
  YYSYMBOL_493_69 = 493,                   /* $@69  */
  YYSYMBOL_494_70 = 494,                   /* $@70  */
  YYSYMBOL_495_71 = 495,                   /* $@71  */
  YYSYMBOL_496_72 = 496,                   /* $@72  */
  YYSYMBOL_497_73 = 497,                   /* $@73  */
  YYSYMBOL_498_74 = 498,                   /* $@74  */
  YYSYMBOL_499_75 = 499,                   /* $@75  */
  YYSYMBOL_500_76 = 500,                   /* $@76  */
  YYSYMBOL_501_77 = 501,                   /* $@77  */
  YYSYMBOL_502_78 = 502,                   /* $@78  */
  YYSYMBOL_503_79 = 503,                   /* $@79  */
  YYSYMBOL_504_80 = 504,                   /* $@80  */
  YYSYMBOL_505_81 = 505,                   /* $@81  */
  YYSYMBOL_506_82 = 506,                   /* $@82  */
  YYSYMBOL_507_83 = 507,                   /* $@83  */
  YYSYMBOL_508_84 = 508,                   /* $@84  */
  YYSYMBOL_509_85 = 509,                   /* $@85  */
  YYSYMBOL_510_86 = 510,                   /* $@86  */
  YYSYMBOL_511_87 = 511,                   /* $@87  */
  YYSYMBOL_512_88 = 512,                   /* $@88  */
  YYSYMBOL_513_89 = 513,                   /* $@89  */
  YYSYMBOL_514_90 = 514,                   /* $@90  */
  YYSYMBOL_515_91 = 515,                   /* $@91  */
  YYSYMBOL_516_92 = 516,                   /* $@92  */
  YYSYMBOL_517_93 = 517,                   /* $@93  */
  YYSYMBOL_type_declaration = 518,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 519,  /* tuple_alias_declaration  */
  YYSYMBOL_520_94 = 520,                   /* $@94  */
  YYSYMBOL_521_95 = 521,                   /* $@95  */
  YYSYMBOL_522_96 = 522,                   /* $@96  */
  YYSYMBOL_523_97 = 523,                   /* $@97  */
  YYSYMBOL_variant_alias_declaration = 524, /* variant_alias_declaration  */
  YYSYMBOL_525_98 = 525,                   /* $@98  */
  YYSYMBOL_526_99 = 526,                   /* $@99  */
  YYSYMBOL_527_100 = 527,                  /* $@100  */
  YYSYMBOL_528_101 = 528,                  /* $@101  */
  YYSYMBOL_bitfield_alias_declaration = 529, /* bitfield_alias_declaration  */
  YYSYMBOL_530_102 = 530,                  /* $@102  */
  YYSYMBOL_531_103 = 531,                  /* $@103  */
  YYSYMBOL_532_104 = 532,                  /* $@104  */
  YYSYMBOL_533_105 = 533,                  /* $@105  */
  YYSYMBOL_make_decl = 534,                /* make_decl  */
  YYSYMBOL_make_decl_no_bracket = 535,     /* make_decl_no_bracket  */
  YYSYMBOL_make_struct_fields = 536,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 537,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 538,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim_list = 539,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 540,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 541, /* optional_make_struct_dim_decl  */
  YYSYMBOL_use_initializer = 542,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 543,         /* make_struct_decl  */
  YYSYMBOL_544_106 = 544,                  /* $@106  */
  YYSYMBOL_545_107 = 545,                  /* $@107  */
  YYSYMBOL_546_108 = 546,                  /* $@108  */
  YYSYMBOL_547_109 = 547,                  /* $@109  */
  YYSYMBOL_548_110 = 548,                  /* $@110  */
  YYSYMBOL_549_111 = 549,                  /* $@111  */
  YYSYMBOL_550_112 = 550,                  /* $@112  */
  YYSYMBOL_551_113 = 551,                  /* $@113  */
  YYSYMBOL_552_114 = 552,                  /* $@114  */
  YYSYMBOL_553_115 = 553,                  /* $@115  */
  YYSYMBOL_make_tuple_call = 554,          /* make_tuple_call  */
  YYSYMBOL_555_116 = 555,                  /* $@116  */
  YYSYMBOL_556_117 = 556,                  /* $@117  */
  YYSYMBOL_make_dim_decl = 557,            /* make_dim_decl  */
  YYSYMBOL_558_118 = 558,                  /* $@118  */
  YYSYMBOL_559_119 = 559,                  /* $@119  */
  YYSYMBOL_560_120 = 560,                  /* $@120  */
  YYSYMBOL_561_121 = 561,                  /* $@121  */
  YYSYMBOL_562_122 = 562,                  /* $@122  */
  YYSYMBOL_563_123 = 563,                  /* $@123  */
  YYSYMBOL_564_124 = 564,                  /* $@124  */
  YYSYMBOL_565_125 = 565,                  /* $@125  */
  YYSYMBOL_566_126 = 566,                  /* $@126  */
  YYSYMBOL_567_127 = 567,                  /* $@127  */
  YYSYMBOL_expr_map_tuple_list = 568,      /* expr_map_tuple_list  */
  YYSYMBOL_push_table_nesting = 569,       /* push_table_nesting  */
  YYSYMBOL_make_table_decl = 570,          /* make_table_decl  */
  YYSYMBOL_make_table_call = 571,          /* make_table_call  */
  YYSYMBOL_array_comprehension_where = 572, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 573,           /* optional_comma  */
  YYSYMBOL_table_comprehension = 574,      /* table_comprehension  */
  YYSYMBOL_array_comprehension = 575       /* array_comprehension  */
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
#define YYLAST   12584

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  242
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  334
/* YYNRULES -- Number of rules.  */
#define YYNRULES  1044
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1867

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   469


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
       2,     2,     2,   224,     2,   241,   236,   220,   213,     2,
     234,   235,   218,   217,   207,   216,   231,   219,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   210,   238,
     214,   208,   215,   209,   237,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   232,     2,   233,   212,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   239,   211,   240,   223,     2,     2,     2,
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
     205,   206,   221,   222,   225,   226,   227,   228,   229,   230
};

#if DAS2_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   602,   602,   603,   608,   609,   610,   611,   612,   613,
     614,   615,   616,   617,   618,   619,   620,   624,   625,   629,
     630,   634,   640,   641,   642,   646,   647,   651,   652,   656,
     675,   676,   677,   678,   682,   683,   687,   688,   692,   693,
     693,   697,   702,   711,   726,   742,   747,   755,   755,   794,
     812,   816,   819,   823,   827,   831,   835,   841,   850,   851,
     855,   858,   861,   867,   868,   872,   876,   877,   881,   884,
     890,   896,   899,   905,   906,   910,   911,   915,   916,   920,
     921,   921,   925,   925,   934,   935,   939,   940,   946,   947,
     948,   949,   950,   954,   955,   959,   960,   964,   966,   964,
     978,   978,   986,   988,   986,  1000,  1000,  1008,  1010,  1008,
    1021,  1028,  1035,  1040,  1049,  1057,  1063,  1067,  1075,  1085,
    1085,  1094,  1102,  1102,  1118,  1124,  1131,  1150,  1154,  1161,
    1162,  1163,  1164,  1165,  1166,  1170,  1175,  1183,  1184,  1185,
    1186,  1190,  1191,  1192,  1193,  1194,  1195,  1196,  1197,  1198,
    1204,  1207,  1213,  1216,  1222,  1225,  1228,  1234,  1235,  1236,
    1237,  1241,  1259,  1282,  1285,  1295,  1310,  1325,  1340,  1343,
    1350,  1354,  1361,  1362,  1366,  1367,  1371,  1372,  1373,  1377,
    1381,  1385,  1392,  1396,  1397,  1398,  1399,  1400,  1401,  1402,
    1403,  1404,  1405,  1406,  1407,  1408,  1409,  1410,  1411,  1412,
    1413,  1414,  1415,  1416,  1417,  1418,  1419,  1420,  1421,  1422,
    1423,  1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,
    1433,  1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,
    1443,  1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,  1452,
    1453,  1454,  1455,  1456,  1457,  1458,  1459,  1460,  1461,  1462,
    1463,  1464,  1465,  1466,  1467,  1468,  1469,  1470,  1471,  1472,
    1473,  1474,  1475,  1476,  1477,  1478,  1479,  1480,  1481,  1482,
    1483,  1487,  1488,  1489,  1490,  1491,  1492,  1493,  1494,  1495,
    1496,  1497,  1498,  1499,  1500,  1501,  1502,  1503,  1504,  1505,
    1506,  1507,  1508,  1509,  1510,  1511,  1512,  1513,  1514,  1515,
    1516,  1517,  1518,  1519,  1520,  1521,  1522,  1523,  1524,  1525,
    1526,  1527,  1528,  1529,  1530,  1531,  1532,  1533,  1534,  1538,
    1539,  1543,  1562,  1563,  1564,  1568,  1574,  1574,  1591,  1594,
    1596,  1594,  1608,  1610,  1608,  1625,  1643,  1661,  1679,  1690,
    1691,  1692,  1693,  1694,  1695,  1696,  1697,  1698,  1699,  1700,
    1701,  1702,  1703,  1704,  1705,  1706,  1707,  1708,  1709,  1711,
    1709,  1726,  1731,  1737,  1743,  1744,  1748,  1749,  1753,  1757,
    1764,  1765,  1776,  1780,  1783,  1791,  1791,  1791,  1794,  1800,
    1803,  1807,  1811,  1818,  1825,  1831,  1835,  1839,  1842,  1845,
    1853,  1856,  1864,  1870,  1871,  1872,  1876,  1877,  1881,  1882,
    1886,  1891,  1899,  1906,  1918,  1922,  1925,  1935,  1935,  1935,
    1938,  1938,  1938,  1943,  1943,  1943,  1951,  1951,  1951,  1957,
    1967,  1978,  1993,  1996,  1999,  2002,  2008,  2009,  2017,  2029,
    2030,  2031,  2035,  2036,  2037,  2038,  2039,  2043,  2048,  2056,
    2057,  2061,  2068,  2072,  2079,  2080,  2081,  2082,  2083,  2084,
    2085,  2086,  2090,  2091,  2092,  2093,  2094,  2095,  2096,  2097,
    2098,  2099,  2100,  2101,  2102,  2103,  2104,  2105,  2106,  2107,
    2108,  2109,  2110,  2114,  2120,  2127,  2139,  2145,  2153,  2161,
    2172,  2184,  2188,  2195,  2198,  2198,  2198,  2203,  2203,  2203,
    2216,  2220,  2224,  2228,  2234,  2242,  2248,  2256,  2264,  2275,
    2284,  2290,  2298,  2298,  2298,  2305,  2309,  2318,  2326,  2334,
    2338,  2341,  2349,  2350,  2351,  2358,  2359,  2360,  2361,  2362,
    2363,  2364,  2365,  2366,  2367,  2368,  2369,  2370,  2371,  2372,
    2373,  2374,  2375,  2376,  2377,  2378,  2379,  2380,  2381,  2382,
    2383,  2384,  2385,  2386,  2387,  2388,  2389,  2390,  2391,  2392,
    2393,  2399,  2400,  2401,  2402,  2403,  2418,  2427,  2428,  2429,
    2430,  2431,  2432,  2433,  2434,  2435,  2436,  2437,  2438,  2439,
    2440,  2441,  2441,  2441,  2449,  2450,  2451,  2456,  2459,  2459,
    2459,  2462,  2467,  2471,  2471,  2471,  2476,  2483,  2489,  2493,
    2493,  2493,  2498,  2501,  2507,  2507,  2507,  2514,  2519,  2523,
    2523,  2523,  2528,  2531,  2537,  2537,  2537,  2544,  2549,  2550,
    2551,  2552,  2553,  2554,  2555,  2556,  2557,  2559,  2563,  2564,
    2569,  2575,  2581,  2590,  2593,  2596,  2605,  2606,  2607,  2608,
    2609,  2610,  2611,  2615,  2619,  2623,  2627,  2631,  2635,  2639,
    2643,  2647,  2652,  2656,  2661,  2665,  2670,  2677,  2678,  2682,
    2683,  2684,  2688,  2689,  2693,  2694,  2695,  2699,  2700,  2704,
    2716,  2719,  2720,  2724,  2724,  2743,  2742,  2757,  2756,  2773,
    2785,  2794,  2804,  2805,  2806,  2807,  2808,  2812,  2815,  2824,
    2825,  2829,  2832,  2836,  2849,  2858,  2859,  2863,  2866,  2870,
    2883,  2884,  2888,  2893,  2898,  2906,  2909,  2916,  2919,  2925,
    2926,  2927,  2931,  2932,  2936,  2943,  2948,  2957,  2963,  2967,
    2978,  2985,  2994,  2997,  3000,  3007,  3011,  3017,  3028,  3031,
    3036,  3047,  3048,  3052,  3053,  3054,  3058,  3061,  3064,  3064,
    3084,  3087,  3087,  3105,  3110,  3118,  3119,  3123,  3126,  3139,
    3156,  3157,  3158,  3163,  3163,  3189,  3190,  3197,  3210,  3211,
    3212,  3216,  3226,  3229,  3235,  3236,  3240,  3241,  3245,  3246,
    3250,  3252,  3257,  3250,  3273,  3274,  3278,  3279,  3283,  3289,
    3290,  3291,  3292,  3296,  3297,  3298,  3302,  3305,  3311,  3313,
    3318,  3311,  3339,  3346,  3351,  3360,  3366,  3370,  3381,  3382,
    3383,  3384,  3385,  3386,  3387,  3388,  3389,  3390,  3391,  3392,
    3393,  3394,  3395,  3396,  3397,  3398,  3399,  3400,  3401,  3402,
    3403,  3404,  3405,  3406,  3407,  3408,  3409,  3410,  3411,  3412,
    3413,  3414,  3415,  3416,  3417,  3418,  3419,  3420,  3421,  3422,
    3423,  3424,  3425,  3426,  3427,  3428,  3429,  3430,  3434,  3435,
    3436,  3437,  3438,  3439,  3440,  3441,  3445,  3456,  3460,  3467,
    3479,  3486,  3492,  3501,  3506,  3516,  3526,  3536,  3549,  3550,
    3551,  3552,  3553,  3557,  3561,  3561,  3561,  3575,  3576,  3580,
    3585,  3592,  3595,  3598,  3601,  3607,  3610,  3624,  3625,  3629,
    3630,  3631,  3632,  3633,  3633,  3633,  3637,  3642,  3649,  3656,
    3656,  3663,  3663,  3670,  3674,  3678,  3683,  3688,  3693,  3698,
    3702,  3706,  3711,  3715,  3719,  3724,  3724,  3724,  3730,  3737,
    3737,  3737,  3742,  3742,  3742,  3748,  3748,  3748,  3753,  3759,
    3759,  3759,  3764,  3764,  3764,  3773,  3779,  3779,  3779,  3784,
    3784,  3784,  3793,  3799,  3799,  3799,  3804,  3804,  3804,  3813,
    3813,  3813,  3819,  3819,  3819,  3828,  3831,  3842,  3858,  3860,
    3865,  3870,  3858,  3896,  3898,  3903,  3909,  3896,  3935,  3937,
    3942,  3947,  3935,  3988,  3989,  3990,  3991,  3992,  3993,  3994,
    3998,  3999,  4000,  4001,  4002,  4006,  4013,  4020,  4026,  4032,
    4039,  4046,  4052,  4061,  4064,  4070,  4078,  4083,  4090,  4095,
    4101,  4102,  4106,  4107,  4111,  4111,  4111,  4119,  4119,  4119,
    4126,  4126,  4126,  4136,  4136,  4136,  4143,  4143,  4143,  4154,
    4160,  4160,  4160,  4173,  4192,  4192,  4192,  4202,  4202,  4202,
    4215,  4215,  4215,  4228,  4237,  4237,  4237,  4257,  4264,  4264,
    4264,  4274,  4277,  4288,  4294,  4317,  4325,  4345,  4370,  4371,
    4375,  4376,  4381,  4384,  4394
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
  "\"!?.\"", "\"![\"", "\"!?[\"", "\"!??\"", "\"!is\"", "\"!as\"",
  "\"!?as\"", "\"<|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"",
  "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"@@\"", "\"@field\"", "\"::\"",
  "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"", "\"^^=\"", "\"..\"",
  "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"", "\"$a\"", "\"$t\"", "\"$c\"",
  "\"$f\"", "\"...\"", "\"integer constant\"", "\"long integer constant\"",
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
  "alias_declaration", "distinct_alias", "optional_public_or_private_enum",
  "enum_name", "optional_enum_basic_type_declaration", "optional_commas",
  "emit_commas", "optional_emit_commas", "enum_declaration", "$@58",
  "$@59", "$@60", "optional_structure_parent", "optional_sealed",
  "structure_name", "class_or_struct",
  "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@61", "$@62", "$@63", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_alias_bits", "bitfield_basic_type_declaration",
  "bitfield_type_declaration", "$@64", "$@65", "c_or_s", "table_type_pair",
  "dim_list", "type_declaration_no_options",
  "optional_expr_list_in_braces", "type_declaration_no_options_no_dim",
  "$@66", "$@67", "$@68", "$@69", "$@70", "$@71", "$@72", "$@73", "$@74",
  "$@75", "$@76", "$@77", "$@78", "$@79", "$@80", "$@81", "$@82", "$@83",
  "$@84", "$@85", "$@86", "$@87", "$@88", "$@89", "$@90", "$@91", "$@92",
  "$@93", "type_declaration", "tuple_alias_declaration", "$@94", "$@95",
  "$@96", "$@97", "variant_alias_declaration", "$@98", "$@99", "$@100",
  "$@101", "bitfield_alias_declaration", "$@102", "$@103", "$@104",
  "$@105", "make_decl", "make_decl_no_bracket", "make_struct_fields",
  "make_variant_dim", "make_struct_single", "make_struct_dim_list",
  "make_struct_dim_decl", "optional_make_struct_dim_decl",
  "use_initializer", "make_struct_decl", "$@106", "$@107", "$@108",
  "$@109", "$@110", "$@111", "$@112", "$@113", "$@114", "$@115",
  "make_tuple_call", "$@116", "$@117", "make_dim_decl", "$@118", "$@119",
  "$@120", "$@121", "$@122", "$@123", "$@124", "$@125", "$@126", "$@127",
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

#define YYPACT_NINF (-1694)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-937)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1694,    48, -1694, -1694,    70,   -31,    43,   150, -1694,    33,
   -1694, -1694, -1694, -1694,    -2,   155, -1694, -1694, -1694, -1694,
      99,    99,    99, -1694,    77, -1694,   146, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,   101, -1694,
      53,   192,   260, -1694,   333,     1, -1694, -1694, -1694,   294,
      99,    99, -1694, -1694,   146,   150,   150,   150,   302,   325,
   -1694, -1694, -1694, -1694,   155,   155,   155,   284, -1694,   799,
     -41, -1694, -1694, -1694, -1694,   401, -1694,   360, -1694,   555,
     111,    70,   344,   -31,   335, -1694,   319,   341,   103,   423,
      18,   537, -1694, -1694,   598,   581,   592,   608, -1694,   642,
     385, -1694, -1694,    61,    70,   155,   155,   155,   155,   501,
   -1694,   820,   840,   661,   695,   848, -1694, -1694,   605, -1694,
   -1694,   659, -1694, -1694, -1694,   727,   149, -1694, -1694, -1694,
   -1694,   333,   333,   627,   333,   689,   713,   679,   735, -1694,
   -1694,   759,   782, -1694, -1694,   711,   733,   501,   501, -1694,
   -1694,   775, -1694,   105, -1694,   353,   822,   799, -1694,   836,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694,   851, -1694, -1694,
   -1694, -1694, -1694, -1694,   866, -1694, -1694, -1694, -1694,   859,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694,   112,   627,   627,
     627, -1694, -1694,   994,   994, 10674, 10674,   989, -1694,   632,
     860, -1694, -1694, -1694, -1694, -1694, 12088, -1694,   847,   934,
     175,    70,   896,   871, -1694, -1694, -1694,   149, -1694, -1694,
   -1694,   861,   862,   864,   843,   867,   868, -1694, -1694, -1694,
     849, -1694, -1694, -1694, -1694, -1694,   531, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,   870, -1694,
   -1694, -1694,   875,   880, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694,   882,   883,   865,    -2,   200, -1694, -1694, -1694, -1694,
     126,   872,   887,   887, -1694, -1694, -1694, -1694, -1694, -1694,
     905, -1694,   888,   889, 10842, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694,  1070,  1091, -1694,   895, -1694,   501,   761,
     860, -1694,   933,   501, -1694, -1694,   866,   501,    70, -1694,
     712, -1694, -1694, -1694, -1694, -1694,  9291, -1694, -1694,   936,
     917,   -90,   -86,   141, -1694, -1694,  9291,   331, -1694,  6816,
   -1694, -1694, -1694,    25, -1694, -1694, -1694,    14, -1694,  7041,
     902,  1937, -1694,   897, -1694, -1694, 12216, 12333, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,   938,   904,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694,  1116, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694,   942,   909, -1694, -1694,   -74,
     -80,  -105, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694,   907,   937, -1694,   397, -1694,   501,   951, 10674, -1694,
      -9, 10674, 10674, 10674,   935,   940, -1694, -1694,    78,    -2,
     947,    31, -1694,   429,   916,   948,   950,   513,   953,   918,
     515,   954, -1694,   552,    26,   955, 10185, 10185,   438,   939,
     941,   943,   944,   946,   949, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, 10185, 10185, 10185, 10185, 10185,
    4341,  5241, -1694, -1694, -1694, -1694, -1694, -1694,   952, -1694,
   -1694, -1694, -1694,   919, -1694, -1694,   -58,   -58, -1694,   -58,
     -58,   959, 10947, -1694, -1694,   961, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, 10674, 10674,   966,   956, 10674,   895,
   10674,   895, 10674,   895, 10841,   975,   970, -1694,  6816, -1694,
   10674,  9291,   987,   967, -1694, -1694, -1694, -1694, -1694,   963,
   -1694, -1694,   990,  7266, -1694,   126, -1694, 10841,   975, -1694,
   -1694, -1694, -1694, -1694, -1694, 12387,   639,  1109,   992, -1694,
      66,   986,   -67,   993, 10674, 10674, -1694,  9739, -1694, -1694,
   -1694, -1694,    -2, -1694,   726,   995,  1133,   714, -1694, -1694,
   -1694,    75, -1694, -1694, -1694,  9291,   366,   755,   969,   389,
   -1694, -1694, -1694, -1694,   996, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694,   553, -1694,   968,   971,   974, -1694,
    6816, 10674,  9291,  9291, -1694, -1694, -1694,  9291, -1694,  9291,
   -1694,  6816, -1694, -1694,  6816,  1013, -1694, 10674,  1927,  1927,
     997,   998,   183, -1694, -1694,  9291,  9291,  9291,  9291,  9291,
    9291,   841,  1927,  1927,   207,  1927,  1927,  1001,  1223,  1004,
    1005,   164,   967,  1031,  1017,   501,  3891,   155,  1244, -1694,
   -1694,   919, -1694, -1694, -1694, -1694, -1694,  2321, 11667, 10185,
   10185, -1694, -1694, 10185, 10185, 10185, 10185,  1053, 10185,   505,
    9291,   644,   655,  9291,  9291, 10185, 11722, 11849, 11904, 10185,
   10185, 10185, 10185,  9291, 10185, 10185, 10185, 10185,  9962, 10185,
   10185, 10185, 10185, 10185, 10185, 10185, 10185, 10185, 10185, 12160,
    9291,  5466,   768,   770, -1694, -1694,  1060,   771,   -80,   776,
     -80,   786,   -80,   -91, -1694,   399,   887,  1048, -1694,   427,
   -1694, 10674,   967,   434,   887, -1694, -1694,  7491, -1694, -1694,
   -1694, -1694,  1027,  1064, -1694,    99, -1694,    99, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694,  9291, -1694, -1694,   398,
     -34,   -34,   -34, -1694,   887,   887, 10185, 11042, -1694,  1065,
   -1694, -1694, -1694, -1694,  9291,  1067,   489, 10674,    -9, -1694,
    9291,    99, -1694, -1694, -1694, -1694, -1694, -1694, -1694, 10674,
   10674, 10674, 10674,  4566,  1068,  9291, 10674, -1694, -1694, -1694,
   10674,   967,   243, -1694,  1059,  1032, 10674, 10674, 10674,  1033,
   10674,  1034, 10674,   967, 10674, 10841,   967, -1694,   975,   376,
    9291,  9291, 10674,   895,  1035,  1036,  1037,  1045,  1047,  1057,
   -1694,  9291,   368,   134,  1061, -1694,  9291, -1694,  9291, -1694,
    9291,  1063,   691, -1694, -1694,  7716,    85,  4116, -1694,   188,
    1069,   163,  1062,   895,  2455,  1244,  1079,  1066, -1694, -1694,
    1080,  1072, -1694, -1694,  1058,  1058,   797,   797,  1489,  1489,
    1073,   601,  1075, -1694,  1082,  1084,  1085,  1087, -1694,  1083,
    1089,   601,  1088,  1092, -1694, -1694,  1099,  1093, -1694, -1694,
    1111,  1096, -1694, -1694,   419,   419,   961,  1058,  1058, 11042,
   -1694, -1694, 11331, 11216, 11303, 11042, 12031,  2134, 11418, 11505,
    1307,   797,   797,  1727,  1727,   601,   601,   601,   717,  9291,
    1097,  1100,   719,  9291,  1302,  1104,  1090, -1694,   236, -1694,
   -1694, -1694,   375, -1694,  1125, -1694,  1127, -1694,  1128, 10674,
   -1694, 10841, 10674, -1694,   975,   481,  1114,  1110, 10674,  9291,
   -1694, -1694,  1143,   246, -1694, 10505, -1694,   213, -1694,  1119,
    1129,  1318, -1694, -1694,   247, -1694, -1694, -1694, 11129,  2695,
    1158, -1694,   246,    30,  1134, -1694,  1136,  1324,    75,  9291,
      99, -1694, -1694, -1694, -1694,   887,   460,   762,   789,   721,
     266,  1139,  1141,   484,  1145,   791, 10674, 10841,   975,  1040,
    1146,  1138, 10674,  9291,  1148, -1694,  1132,  1137,  1173, -1694,
    1275, -1694,  1332,  1149,  1368,   499,  1150, 10674,   510,  1244,
    1155,  1156,  1383,   -80, -1694, -1694, -1694, -1694, -1694,  1161,
    1186,  1163,  1328,  1201,    41,   134,  1164, -1694, -1694, -1694,
    1168,   237,  1171,  1169,  1059,   271, -1694,  1175,   178,  5691,
   -1694, -1694, -1694,   350,   -80, -1694,  7941, -1694,  1172,  8166,
    1211,  1215, -1694,    99,  1224,  8391,  -112,  8616, -1694, -1694,
   -1694,    99,    99,  1405, -1694,   657, -1694, -1694,  1404, -1694,
   -1694,  1409,  1377, -1694,    99, -1694,    99,    99,    99,    99,
      99, -1694,  1354, -1694,    99, 10303,   895, -1694,  9291, -1694,
    9291,  4791,  9291, -1694,  9291,  5916,  9291, -1694, -1694, -1694,
    9291, -1694,  9291, -1694,  9291,  1209,  1191, -1694, -1694, 10185,
    1193, -1694,  1198,  9291,  5016,  1199, -1694,  1202, -1694,  6141,
   -1694,  7491, -1694, -1694, -1694,  1235, -1694,  1239, -1694, -1694,
   -1694, -1694, -1694, -1694,   887, -1694, -1694,   887, -1694, -1694,
    1110, -1694, -1694,   887, -1694,  9291, -1694,   678, -1694, -1694,
   -1694,  1200, -1694,  1205, -1694,  9291,  1241,   543, 10674, -1694,
    9291,  1212,  9291,   693, -1694,  1242, -1694, -1694,  1433,   866,
   -1694,  9291,  1245, -1694,  9291,    99, -1694, -1694, -1694, -1694,
    1236, -1694, -1694, -1694,  1240,  1278, -1694, -1694,  1541,   613,
     628, -1694, -1694,  9291,  1620, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694,  1713, -1694,   184, -1694,
   -1694, -1694,  1261,  6366, -1694,  1272,  9291,  1283, -1694,   300,
    6816,   172,    46,   324,  9291,  9291,  9291,   134, -1694, -1694,
   -1694,   691,  1246,  4116,   373,  1285,  1288,  1253,  1292,  1295,
   -1694,   421,   501,  9291, -1694,  1479,  9291, -1694,  1286,  1287,
   -1694,  1289,  1309, -1694,  1172,  9291, -1694, -1694, -1694, -1694,
    1266, -1694, -1694,  1267,   -70,   -70,  1268, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694,   -85, -1694, 10185, 10185, 10185, 10185,
   10185, 10185, 10185, 10185, 10185, 10185,  9291, 10185, 10185, 10185,
   10185, 10185, 10185, 10185,   -80, 10674,  1270, 10674,  1271,  4116,
   -1694,   422,   431,  1273,  1274, -1694,   470,  1277, 10674,  1282,
   10674,  1293, 10674,  1294, -1694,  9291, 11129,  9291, -1694,  1296,
    4116, -1694,   477,   478,  9291, -1694, -1694, -1694,   480, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694,  1299, -1694,  1263,
   -1694, -1694,  1297, -1694,  1279,  1475,   113, -1694,  1496, -1694,
   -1694,  1300,  1329,   863,  1469,    99,  1311, -1694,    99, -1694,
    1308,  1310, -1694, -1694,  9291,  1334, -1694, -1694, -1694, -1694,
    1316,  1317,  1319,  1321,  1323,  1330,  1331,  1333,  1481,  1335,
   -1694,  1336,  8841,  1172,   332, -1694, -1694,   486, -1694, -1694,
    1337, -1694,  1361, -1694,   488,  1341,  1520,  1201,  6816,  9291,
    9291,  1344, -1694, -1694,    51, -1694,   233, -1694, -1694, -1694,
    1369, -1694, -1694,   350, -1694,  -105, -1694,  1172, -1694, 10674,
    9291, -1694, -1694, -1694, -1694,  2935,  9291,  9291,    70,   896,
    1348,  1352,  9066,  1201, -1694, -1694,   397, 10947, 10947, 10947,
   10947, 10947, 10947, 10947, 10947, 10947, 10947, 10947, -1694, -1694,
   10947, 10947, 10947, 10947, 10947, 10947, 10947,   501,  1726, -1694,
     807, -1694,   273,  6591, -1694, -1694, -1694, -1694, -1694, -1694,
    1924, -1694,   810, -1694,   816, -1694, 10674,  1353,  1355, -1694,
     276,  6591, -1694, -1694,  1356, -1694,  9291, -1694, -1694,  9291,
    1371,  9291, -1694, -1694, -1694, 10674, -1694, -1694,   743, -1694,
      16, -1694, -1694, -1694,  1481,  1481,  1358,  1360,  1363,  1366,
    1370,  6816, -1694,  9291,  9291,  9291,  9291,  9291,  6816, -1694,
   -1694,  1481,  1372,  1481, -1694,  1373, -1694, -1694,   332, -1694,
    1386, -1694, -1694,  1351,  9291,  1408,   497,   500, -1694, -1694,
     358,  6816,  1375,  1376, -1694, -1694, -1694,   887, -1694,  1367,
    1378,  1381,   282,   134,  9291,   333,  1382,   503,   265, -1694,
    -105, -1694, -1694,  1384,   511, -1694, -1694, -1694,   819, -1694,
   -1694,  1385,   558, -1694, -1694,  1387, -1694, -1694,  1379,   -15,
    1579,    16, -1694, -1694,   863,   110,   110, -1694,  9291,  1481,
    1481,   721,  1388,  1390,  1391,  1392,  1398,  1399,   967,   110,
    1481,   721, -1694, -1694, -1694,  9291,  1401, -1694, -1694,  1418,
    9291,  9291,   573, -1694, -1694,  1496,  1609,   501, -1694,    52,
    1422,   649,   501,   219, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694,  1579,   398,   721,  1432,  1453, -1694,  1426,
    1427,  1429,   110,   110,  1432,  1430, -1694, -1694, -1694, -1694,
   -1694, -1694,  1431,  1434,   721,  1435,  1436, -1694,  9291, -1694,
   -1694,  1437, -1694,  9516,    99, -1694,  6816,   501,   501,  1172,
   10674,    -9, -1694,  3175, 12088, -1694, -1694, -1694, -1694,   578,
    1439, -1694, -1694, -1694, -1694,  1441,  1442, -1694, -1694, -1694,
    1443, -1694,  1614,  1438,  1436,  9291, -1694, -1694, -1694, -1694,
   -1694, 10947, -1694,  1444,   594,  1172,  1172, -1694,   269,  9291,
    1428,    99, 12088, -1694,   721, -1694, -1694, -1694,  9291, -1694,
    1451,  1436, -1694,   858,  9516,   501, -1694, -1694,  9291,    99,
   -1694, -1694,   501,   602, -1694, -1694,  1446, -1694,   501, -1694,
   -1694,  1456, -1694,    99,  1172,    99, -1694,  -105, -1694, -1694,
    3415, -1694,  9291, -1694, -1694, -1694, -1694,  1447,  1452,  1458,
    1496, -1694, -1694,  9516,   501, -1694, -1694,    99, -1694,  3655,
   -1694,  1452,  1454,   858,  1496, -1694, -1694
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   174,     1,   396,     0,     0,    58,   740,   397,     0,
     958,   948,   953,    20,     0,     0,    19,    16,    15,     3,
       0,     0,     0,     8,   778,     7,   721,     6,    11,     5,
       4,    13,    12,    14,   138,   139,   140,   137,   148,   150,
      49,    68,    65,    66,     0,     0,    50,   742,   741,     0,
       0,     0,    26,    25,   721,   740,   740,   740,     0,   370,
      47,   158,   159,   160,     0,     0,     0,   161,   163,   170,
       0,   157,    21,    10,     9,   319,   760,     0,   722,   723,
       0,     0,     0,     0,     0,    51,     0,     0,    59,     0,
      63,   743,   745,   746,    22,     0,     0,     0,   372,     0,
       0,   169,   164,     0,     0,     0,     0,     0,     0,    77,
     320,   322,   748,   770,   769,   773,   725,   724,   731,   146,
     147,     0,   144,   145,   142,     0,     0,   141,   151,    69,
      67,     0,     0,    52,     0,     0,     0,     0,     0,    64,
      60,     0,     0,    23,    24,    27,   858,    77,    77,   371,
      45,    48,   168,     0,   165,   166,   167,   171,    75,    78,
     175,   324,   323,   326,   321,   750,   749,     0,   772,   771,
     775,   774,   779,   726,   647,   143,    30,    31,    35,     0,
     133,   134,   131,   132,   130,   129,   135,     0,    54,    55,
      53,    57,    56,    63,    63,     0,     0,     0,    29,     0,
     758,   949,   954,    46,   162,    76,     0,   751,   752,   766,
     728,     0,   648,     0,    32,    33,    34,     0,   149,    62,
      61,     0,     0,     0,     0,     0,     0,   788,   831,   789,
     847,   790,   794,   795,   796,   797,   837,   801,   802,   803,
     804,   805,   806,   807,   832,   833,   834,   835,   918,   793,
     800,   836,   925,   932,   791,   798,   792,   799,   808,   809,
     810,   811,   812,   813,   814,   815,   816,   817,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,   828,   829,
     830,     0,     0,     0,     0,   846,   879,   882,   880,   881,
     945,   875,   747,   744,    28,   861,   862,   859,   860,   756,
     759,   959,     0,     0,     0,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,     0,     0,   182,   176,   270,    77,     0,
     758,   767,     0,    77,   730,   727,   647,    77,     0,   710,
     702,   732,   136,   883,   909,   912,     0,   915,   905,     0,
       0,   919,   926,   933,   939,   942,     0,   877,   889,   364,
     895,   900,   894,     0,   908,   904,   897,     0,   899,     0,
     876,     0,   757,     0,   950,   955,   261,   262,   259,   185,
     186,   188,   187,   189,   190,   191,   192,   218,   219,   216,
     217,   209,   220,   221,   210,   207,   208,   260,   243,     0,
     258,   222,   223,   224,   225,   196,   197,   198,   193,   194,
     195,   206,     0,   212,   213,   211,   204,   205,   200,   199,
     201,   202,   203,   184,   183,   242,     0,   214,   215,   647,
     179,     0,   838,   841,   844,   845,   839,   842,   840,   843,
     753,     0,   764,   780,     0,   152,    77,     0,     0,   703,
       0,     0,     0,     0,     0,     0,   521,   522,     0,     0,
       0,     0,   515,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   837,     0,     0,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,   631,   444,   446,   445,   447,
     448,   449,   450,   451,    41,     0,     0,     0,     0,     0,
     364,     0,   429,   430,  1033,   519,   518,   618,   516,   611,
     610,   609,   608,   172,   614,   517,   613,   612,   566,   523,
     567,     0,   512,   574,   524,     0,   520,   970,   972,   971,
     513,   974,   973,   514,     0,     0,     0,   864,     0,   176,
       0,   176,     0,   176,     0,     0,     0,   891,     0,   888,
       0,     0,     0,  1040,   422,   902,   903,   896,   898,     0,
     901,   872,     0,     0,   947,   946,   960,   681,   687,   263,
     265,   264,   266,   257,   241,   267,   244,   226,     0,   177,
     395,   672,   673,     0,     0,     0,   325,     0,   332,   426,
     327,   761,     0,   768,     0,     0,   704,   702,   729,   153,
     711,     0,   700,   701,   699,     0,     0,     0,     0,   869,
     994,   997,   375,   846,   379,   378,   384,   963,   969,   964,
     965,   966,   968,   967,     0,   416,     0,     0,     0,  1024,
       0,     0,     0,     0,   407,   410,   571,     0,   413,     0,
    1028,     0,  1006,  1010,     0,     0,  1000,     0,   551,   552,
       0,     0,   484,   481,   483,     0,     0,     0,     0,     0,
       0,     0,   528,   527,   568,   526,   525,     0,     0,     0,
       0,   370,  1040,  1040,     0,    77,     0,     0,   439,   431,
     361,   172,   338,   336,   337,   335,   886,     0,     0,     0,
       0,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   502,
       0,     0,     0,     0,   848,   863,     0,     0,   179,     0,
     179,     0,   179,   370,   679,     0,   677,     0,   685,     0,
     849,     0,  1040,     0,   368,   423,   887,  1041,   365,   893,
     871,   874,     0,   853,   682,    95,   688,    95,   268,   269,
     246,   247,   249,   248,   250,   251,   252,   253,   245,   254,
     255,   256,   230,   231,   233,   232,   234,   235,   236,   237,
     228,   229,   238,   239,   240,   227,     0,   393,   394,     0,
     647,   647,   647,   178,   181,   180,     0,   427,   361,   737,
     765,   776,   660,   781,     0,     0,     0,     0,     0,   718,
       0,     0,   884,   910,   913,    18,    17,   867,   868,     0,
       0,     0,     0,   992,     0,     0,     0,  1014,  1017,  1020,
       0,  1040,     0,  1031,  1040,     0,     0,     0,     0,     0,
       0,     0,     0,  1040,     0,     0,  1040,  1003,     0,     0,
       0,     0,     0,   176,     0,     0,     0,     0,     0,     0,
      44,     0,    42,     0,     0,  1013,     0,   691,     0,   690,
       0,     0,  1041,   985,   556,   366,     0,   364,   505,     0,
       0,     0,     0,   176,     0,   439,     0,     0,   582,   581,
       0,     0,   588,   592,   529,   530,   542,   543,   540,   541,
       0,   575,     0,   563,     0,     0,   492,     0,   565,     0,
       0,   576,     0,     0,   587,   586,     0,     0,   593,   597,
       0,     0,   603,   607,   615,   616,   617,   531,   532,   620,
     621,   622,   547,   548,   549,   550,     0,     0,   545,   546,
     544,   538,   539,   534,   533,   535,   536,   537,     0,     0,
       0,   490,     0,     0,     0,     0,     0,   510,     0,   916,
     906,   850,     0,   920,     0,   927,     0,   934,     0,     0,
     940,     0,     0,   943,     0,     0,     0,   877,     0,     0,
     424,   873,   854,   754,    93,    96,   951,    96,   956,     0,
       0,   782,   669,   670,   692,   674,   676,   675,   428,     0,
     733,   738,   754,   663,     0,   706,     0,   707,     0,     0,
       0,   720,   885,   911,   914,   870,     0,     0,     0,   993,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1041,     0,   569,     0,     0,     0,   570,
       0,   619,     0,     0,     0,     0,     0,     0,     0,   439,
       0,     0,     0,   179,   626,   627,   628,   629,   630,     0,
      38,     0,   111,     0,     0,     0,     0,   976,   975,   555,
       0,     0,     0,     0,  1040,     0,   506,     0,     0,     0,
     509,   507,   173,     0,   179,   363,   387,   385,     0,     0,
       0,     0,   386,     0,     0,     0,    77,     0,   358,   443,
     339,     0,     0,     0,   352,     0,   353,   347,     0,   344,
     343,     0,     0,   345,     0,   362,     0,    91,    92,    89,
      90,   354,   399,   342,     0,   452,   176,   578,     0,   589,
       0,     0,     0,   560,     0,     0,     0,   559,   562,   583,
       0,   594,     0,   604,     0,     0,     0,   598,   602,     0,
       0,   564,     0,     0,     0,     0,   491,     0,   503,     0,
     557,     0,   511,   917,   907,     0,   865,     0,   921,   923,
     928,   930,   935,   937,   678,   941,   680,   684,   944,   686,
     877,   878,   890,   369,   425,     0,   735,   755,   961,    94,
     683,     0,   689,     0,   671,     0,     0,     0,     0,   693,
       0,     0,     0,   755,   762,     0,   661,   777,     0,   647,
     705,     0,     0,   715,     0,     0,   719,   995,   998,   376,
       0,   381,   382,   380,     0,     0,   419,   417,     0,     0,
       0,  1025,  1023,   366,     0,  1032,  1035,   408,   411,   572,
     414,  1029,  1027,  1007,  1011,  1009,     0,  1001,    77,   482,
     646,   485,     0,     0,    39,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1034,   367,
     508,     0,     0,   364,     0,     0,     0,     0,     0,     0,
     437,     0,    77,     0,   388,     0,     0,   373,     0,     0,
     357,     0,     0,    72,     0,     0,   390,   361,   355,   356,
       0,    84,    85,     0,   154,   154,     0,   346,   341,   348,
     349,   350,   351,   398,   647,   340,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   179,     0,     0,     0,     0,   364,
     476,     0,     0,     0,     0,   495,     0,     0,     0,     0,
       0,     0,     0,     0,   599,     0,   577,     0,   561,     0,
     364,   493,     0,     0,     0,   558,   504,   500,     0,   852,
     866,   851,   924,   931,   938,   892,   855,   856,   736,     0,
     952,   957,     0,   784,     0,   785,   695,   694,   328,   734,
     739,     0,     0,   654,   657,     0,     0,   709,     0,   717,
       0,     0,   377,   383,     0,     0,   418,  1015,  1018,  1021,
       0,     0,     0,     0,     0,     0,     0,     0,   992,     0,
    1004,     0,     0,     0,     0,   488,   632,     0,    36,    43,
       0,   113,     0,   114,     0,     0,   115,     0,     0,     0,
       0,     0,   978,   977,     0,   473,     0,   475,   434,   435,
       0,   433,   432,     0,   440,     0,   389,     0,   374,     0,
       0,    70,    71,   121,   391,     0,     0,     0,     0,   156,
       0,     0,     0,     0,   712,   405,     0,   464,   465,   467,
     466,   468,   458,   459,   460,   469,   470,   454,   455,   456,
     457,   471,   472,   461,   462,   463,   453,    77,     0,   644,
       0,   640,     0,     0,   477,   480,   635,   637,   496,   639,
       0,   645,     0,   641,     0,   643,     0,     0,     0,   634,
       0,     0,   494,   499,     0,   501,     0,   962,   783,     0,
       0,     0,   329,   334,   763,     0,   655,   656,   657,   658,
     649,   664,   708,   716,   992,   992,     0,     0,     0,     0,
       0,   364,  1036,   366,     0,     0,     0,     0,     0,   993,
    1008,   992,     0,   992,   623,     0,   625,   486,     0,   633,
      40,   112,   401,     0,     0,     0,     0,     0,   980,   979,
       0,     0,     0,     0,   438,   441,   392,   128,   127,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   404,
       0,   579,   590,     0,     0,   584,   595,   605,     0,   642,
     638,     0,     0,   636,   857,     0,   787,   696,     0,     0,
     652,   649,   650,   651,   654,   991,   991,   420,     0,   992,
     992,   983,     0,     0,     0,     0,     0,     0,  1040,   991,
     992,   983,   624,   489,    37,     0,     0,   117,   118,     0,
       0,     0,     0,   474,   436,   328,    86,    77,   155,     0,
       0,     0,    77,   702,   406,   713,   714,   442,   580,   591,
     478,   479,   585,   596,   606,   600,   497,   498,   786,   361,
     662,   653,   665,   652,     0,     0,   988,  1040,   990,     0,
       0,     0,   991,   991,   984,     0,  1026,  1037,   409,   412,
     573,   415,     0,     0,   983,     0,  1038,   116,     0,   982,
     981,     0,   360,     0,     0,   108,     0,    77,    77,     0,
       0,     0,   601,     0,     0,   667,   698,   697,   659,     0,
    1041,   989,   996,   999,   421,     0,     0,  1022,  1030,  1012,
       0,  1002,     0,     0,  1038,     0,    87,    91,    92,    89,
      90,    88,   110,   100,     0,     0,     0,   125,     0,     0,
       0,     0,     0,   986,     0,  1016,  1019,  1005,     0,  1042,
       0,  1038,    97,    79,     0,    77,   123,   126,     0,     0,
     331,   666,    77,     0,  1039,  1043,     0,   361,    77,    73,
      74,     0,   109,     0,     0,     0,   403,     0,   987,  1044,
       0,    80,     0,   101,   120,   402,   668,     0,   105,     0,
     328,   102,    81,     0,    77,    99,   361,     0,    82,     0,
     106,   105,     0,    79,   328,    83,   104
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1694, -1694,  -986,    -1, -1694, -1694, -1694, -1694, -1694,  1006,
    1615, -1694, -1694, -1694, -1694, -1694, -1694,  1695, -1694, -1694,
   -1694,   -32, -1694, -1694,   826, -1694, -1694,  1616, -1694, -1694,
   -1694, -1694,  -143,  -165, -1694, -1694, -1694, -1694, -1693,   903,
     913, -1694, -1694, -1694, -1694,  -160, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1090, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694,  1485, -1694, -1694,   -42,  -101,  -312,   369, -1694,
   -1694,   539,  1008,  1002,   669,  -545,  -743, -1694,  -363, -1694,
   -1694, -1694, -1659, -1694, -1694, -1563, -1694, -1694, -1106, -1694,
   -1694, -1694, -1694, -1694, -1694,  -820,  -383, -1239,   960,   -13,
   -1694, -1694, -1694, -1694, -1694, -1676, -1662, -1642, -1639, -1694,
   -1694,  1715, -1694, -1362, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694,  -455, -1464,   395,
     224, -1694,  -883, -1694,   461, -1694, -1694, -1694, -1694, -1407,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,   670,
     367, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694,  -164,    57,    19,
      58,   166, -1694, -1694, -1694, -1694, -1694, -1694, -1694,   234,
    -562,  -806, -1694,  -566,  -814, -1694, -1004,    21,    27, -1694,
    -617,  -615, -1694, -1694, -1694, -1470, -1694,  1698, -1694, -1694,
   -1694, -1694, -1694,   512,   716, -1694,  1015, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694,   718, -1694,  1403, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694,   -78, -1694,  1276, -1694, -1694, -1694,  1525, -1694,
   -1694, -1694,  -616, -1694, -1694,  -336,  -976, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694,  -188, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694,  -764, -1613,  -673, -1694, -1694, -1308,
   -1391,  1280, -1694, -1694, -1694, -1694, -1694, -1694, -1694, -1694,
   -1694, -1694,  1281, -1694, -1694,  1284, -1694, -1694, -1694, -1694,
   -1694, -1694, -1694, -1694, -1694, -1694,  1103, -1694,  -479,  1290,
   -1678,  -686,  1291,  -477
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   857,   858,    18,   145,    54,   198,    19,   179,
     185,  1610,  1295,  1468,   691,   535,   151,   536,   100,    21,
      22,    88,    45,    46,   140,    23,    42,    43,  1141,  1142,
    1831,   159,   160,  1832,  1848,  1861,  1343,  1754,  1143,  1025,
    1026,  1813,  1827,  1847,  1814,  1852,  1856,  1862,  1853,  1144,
    1145,  1793,  1146,  1104,  1147,  1148,  1149,  1150,  1151,  1152,
    1153,  1154,   186,   187,    38,    39,    40,   212,  1510,    67,
      68,    69,    70,   708,    24,   460,   616,   356,   357,   111,
      25,   163,   358,   164,   206,  1573,  1658,  1800,   619,   838,
    1241,   537,  1155,  1337,  1629,   924,   699,  1113,   783,   538,
    1156,   644,   862,  1442,   539,  1157,  1158,  1159,  1160,  1161,
     829,  1162,  1354,  1299,  1515,  1163,   540,   876,  1453,   877,
    1454,   880,  1456,   541,   866,  1446,   542,   583,   620,   543,
    1320,  1321,   923,   544,   712,   545,  1164,   546,   547,   684,
     548,   892,  1464,   893,  1608,   549,   994,  1406,   550,   584,
     552,   878,  1455,  1375,  1708,  1388,  1712,  1377,  1709,  1390,
    1713,  1556,  1762,  1392,  1714,   553,   554,   610,  1664,  1722,
    1578,  1580,  1435,  1043,  1249,  1764,  1802,   611,   612,   613,
     774,   775,   795,   778,   779,   797,   910,  1032,  1033,  1768,
     635,   480,   627,   370,  1638,   628,   371,    79,   118,   210,
     366,    27,   174,  1041,  1227,  1042,    49,    50,   142,    28,
      51,   167,   208,   360,  1228,   300,   301,    29,   112,   839,
    1431,   623,   362,   363,   115,   172,   843,    30,    77,   209,
     624,  1034,   555,   470,   287,   288,  1002,  1023,   200,   289,
     766,  1410,  1011,   638,   400,   290,   579,   291,   481,  1052,
     580,   781,   565,  1204,   482,  1053,   483,  1054,   564,  1203,
     568,  1208,   569,  1412,   570,  1210,   571,  1413,   572,  1212,
     573,  1414,   574,  1215,   575,  1218,   776,    31,    56,   302,
     597,  1231,    32,    57,   303,   598,  1233,    33,    55,   403,
     793,  1419,   646,   556,   703,  1735,   704,  1727,  1728,  1729,
    1062,   557,   860,  1440,   861,  1441,   888,  1461,  1087,  1602,
     884,  1458,   558,   885,  1459,   559,  1066,  1588,  1067,  1589,
    1068,  1590,   870,  1450,   882,  1457,  1114,   705,   560,   561,
    1783,   788,   562,   563
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,    60,    71,   153,   201,   202,   582,   292,   293,   650,
     213,   653,   848,    90,   850,  1302,   911,   913,  1039,    72,
      73,    74,  1325,   859,   768,  1004,   770,  1006,   772,  1008,
    1240,  1625,   796,   920,  1451,   794,   656,  1226,  -174,   128,
     138,  1222,  1166,   600,   602,   675,  1639,   587,     2,    92,
      93,    71,    71,    71,   133,     3,  1226,  1607,   585,  -922,
    1786,  1300,  1662,  -929,   617,   595,  1478,  1600,  1745,   614,
    1245,  1621,  1756,   827,  1088,   139,   702,  1787,     4,  1085,
       5,    99,     6,   484,   485,    75,   211,   158,     7,    34,
      35,  1788,    71,    71,    71,    71,  1016,   211,     8,   188,
     189,   211,   190,   491,     9,  1801,  1810,  1306,   390,   493,
     608,  1789,   709,    76,  1790,  1616,   828,   286,   286,  1009,
    -922,  1833,   669,   782,  -929,  -922,   119,   120,    10,  -929,
     615,  1780,  1752,  1826,   618,   391,   392,   211,  1787,    36,
     831,   632,   631,  1822,  -922,  1663,   500,   501,  -929,  1513,
     608,  1637,  1788,  1118,  1514,   633,   657,   658,    41,   390,
    1857,   609,  1508,  1014,   180,   181,   108,  1018,  1706,   367,
      58,   832,  1789,    11,    12,  1790,  1707,  1787,   532,   533,
      84,   710,   285,   285,    13,  1070,   391,   392,  1074,    61,
    1061,  1788,   109,  1665,  1666,    59,   401,  1083,    85,   634,
    1086,  1683,   474,   503,   504,   871,  1288,    47,    78,   365,
    1679,  1789,  1681,    48,  1790,   461,   883,  1484,    62,   886,
     473,    86,   393,    16,   475,   632,   394,   588,  1503,  1246,
      52,  1049,    87,    89,   105,   106,   107,   135,   586,   633,
     676,  1418,   799,  1117,  1415,   659,   589,    13,  1301,   136,
      58,   919,    44,  1301,  1270,   590,  1072,  1418,  1301,  1301,
      81,  1269,    15,   907,   970,   660,   971,    37,    14,    53,
    1247,   387,   700,   393,    13,    59,   390,   394,  1732,  1733,
      15,   121,  1115,   634,   395,    63,    16,  1855,   396,  1744,
    -936,   397,   642,   636,   637,   639,   152,   700,    13,  1480,
     122,  1866,   390,   391,   392,   123,   998,  1115,   124,    80,
     530,   125,    81,    16,   907,  1101,   398,   534,    64,   217,
    1116,   909,   135,   286,   401,   872,   476,    58,   908,   391,
     392,  1102,  -487,   629,   136,   395,    99,    16,   182,   396,
     204,   889,   397,   183,  1725,   126,   184,   218,  1093,   125,
    1292,  -936,    59,  1475,  1673,  1314,  -936,  1606,  1730,   729,
     730,   731,   732,   733,   734,   113,   114,   398,  1103,  1476,
     108,  1743,   909,  1846,    13,  -936,   762,   763,  1124,    65,
     767,  1322,   769,   158,   771,  1311,  1260,   907,   285,    66,
     393,  1626,   784,  -487,   394,  1119,  1122,   907,  -487,  1018,
     286,  1305,    82,   286,   286,   286,  1477,  1382,  1060,   390,
     777,  1312,    13,    16,   388,   364,   393,  -487,  1462,   632,
     394,   907,  1571,  1120,  1775,  1776,   834,   835,  1309,  1760,
    1403,  1254,   479,   633,   389,   908,   391,   392,   759,   760,
    1311,   855,    13,  1201,   855,   909,   625,  1265,  1219,  1216,
     856,    16,   395,   856,  1237,   909,   396,  1238,  1071,   397,
    1239,  1232,   626,  1230,    13,   285,  1622,    83,   285,   285,
     285,  1202,  1014,  1201,   907,   643,   654,   634,   395,   909,
    1311,    16,   396,  1311,   398,   397,   286,   286,  1479,    81,
     286,    91,   286,   390,   286,   683,   286,    99,    84,    98,
    1315,  1261,   286,    16,    58,  1704,  1643,  1472,   907,  1651,
     398,  1048,    84,   680,  1316,  1698,    85,  1505,   104,   286,
     391,   392,  1690,   393,  1056,  1057,   105,   394,   107,    59,
      85,   110,   909,   129,  1069,  1473,   286,   286,   132,    86,
    1076,  1077,  1078,  1699,  1080,   577,  1082,  1317,  1084,  1486,
      87,   285,   285,    86,   131,   285,  1092,   285,  1318,   285,
     134,   285,   915,  1319,    87,   578,   909,   285,   727,   214,
     215,   729,   730,   731,   732,   733,   734,   401,   625,  1030,
    1311,   852,  1205,   286,   285,   395,   150,   855,    13,   396,
    1206,  1089,   397,   784,   626,  1031,   856,   855,    13,   286,
     401,   285,   285,   101,   102,   103,   856,   393,  1487,   840,
      58,   394,   116,  1207,  1010,  1542,  1691,   398,   117,   680,
     137,  1374,   717,   718,   681,   855,    13,    16,  1493,  1543,
     849,  1537,   855,    13,   856,    59,  1560,    16,  1311,   929,
     933,   856,  1013,   661,   154,   155,   156,   157,   285,  1017,
     759,   760,   682,  1797,  1014,   143,  1494,  1544,   955,   959,
     963,   144,   966,   662,   285,    16,  1545,  1341,  1342,   395,
    1046,  1055,    16,   396,  1058,  1257,   397,  1201,  1065,   855,
      13,   995,   855,    13,  1561,  1311,  1047,  1201,   856,  1816,
    1817,   856,   942,  1201,    71,  1201,  1220,   855,    13,  1264,
     158,   398,   943,   286,  1472,  1548,   856,  1201,   855,    13,
    1472,   295,  1562,  1563,  1284,  1565,  1381,   856,  1311,    16,
    1386,  1609,    16,  1613,  1424,  1287,   296,   666,  1844,   670,
    1268,   297,  1688,   298,   141,  1689,  1274,    16,  1703,  1402,
    1425,   199,   721,   722,  1408,  -858,  1711,   667,    16,   671,
     727,  1286,   728,   729,   730,   731,   732,   733,   734,   735,
     736,   737,   738,   739,   740,  1311,   673,   864,   285,   286,
     800,   801,   802,   803,   804,   805,   806,   807,   146,  1644,
    1201,   286,   286,   286,   286,  1311,   674,   865,   286,   147,
    1660,   168,   286,  1717,  1024,   390,  1024,  1652,   286,   286,
     286,  1201,   286,   808,   286,   148,   286,   286,  1751,  1311,
    1579,   855,    13,  1803,   286,   809,   810,   811,   717,   718,
     856,  1214,   391,   392,  1217,   169,   855,    13,  1448,  1815,
    1223,   945,   759,   760,   285,   856,   462,  1838,  1467,   149,
     463,   946,   947,  1449,   173,  1474,   285,   285,   285,   285,
    1051,    16,   948,   285,   464,   465,   175,   285,   136,   466,
     467,   468,   469,   285,   285,   285,    16,   285,   135,   285,
    1828,   285,   285,   678,   679,  1417,   855,   161,  1110,   285,
     136,  1829,  1830,   162,  1758,   856,   191,  1528,  1111,  1529,
    1040,   855,   692,   693,   694,   695,   696,   165,  1188,  1763,
     856,  1726,  1726,   166,  1190,   170,  1195,  1734,   700,   393,
     192,   171,   193,   394,  1191,  1726,  1196,  1734,  1115,   477,
    1576,   846,   478,  1140,   847,   479,  1577,   479,   176,   177,
     178,   286,   194,   286,   286,   197,   719,   720,   721,   722,
     286,   711,   711,   199,   711,   711,   727,   286,   728,   729,
     730,   731,   732,   733,   734,   735,   736,   737,   738,   739,
     740,  1769,   741,   742,   841,   842,   401,   195,  1726,  1726,
     853,   395,   105,   106,   107,   396,   203,  1258,   397,   401,
    1734,   401,   401,   999,   837,  1000,  1003,   401,   286,   286,
     196,  1005,  1742,  1334,   286,   105,   285,   401,   285,   285,
     401,  1007,   401,   398,  1259,   285,  1267,  1840,   713,   286,
     714,   715,   285,   754,   755,   756,   757,   758,   401,   219,
     220,   401,  1642,  1617,  1229,  1646,  1229,   401,   759,   760,
     401,  1647,  1509,  1509,  1715,   205,  1859,   211,  1140,  1538,
    1823,  1771,   176,   177,   900,   901,   551,  1253,   207,  1256,
    1426,   139,  1550,   285,   285,   294,   576,   359,   299,   285,
     214,   215,   216,   361,  1035,  1036,  1037,   368,   369,   592,
      95,    96,    97,   390,   285,   373,   374,   376,   375,   717,
     718,   377,   378,   379,   381,  1434,   934,   935,  1761,   382,
     936,   937,   938,   939,   383,   941,   384,   385,   401,   386,
     391,   392,   951,   402,   399,   457,   964,   965,   967,   968,
     969,   972,   973,   974,   975,   977,   978,   979,   980,   981,
     982,   983,   984,   985,   986,   987,   458,   404,   405,   459,
     472,   567,  1330,   566,   593,   603,   596,   604,   605,   606,
    1338,  1339,   607,  1678,  1799,  1463,   621,   622,   630,   640,
     663,   707,   669,  1347,   641,  1348,  1349,  1350,  1351,  1352,
     286,   655,   664,  1355,   665,   390,  1692,   668,   672,   677,
     390,   765,   777,   685,   787,   686,   845,   687,   688,  1495,
     689,  1818,   867,   690,   854,   868,   706,   393,   869,  1540,
    1516,   394,   391,   392,   716,   761,   789,   391,   392,   721,
     722,   764,  1552,  1038,  1554,   780,   390,   727,  1672,   728,
     729,   730,   731,   732,   733,   734,   735,   736,   737,   738,
     739,   740,   786,   790,   830,   285,   826,   887,   833,   844,
     863,   890,   891,   391,   392,   903,   904,   905,   912,   906,
     812,   813,   814,   815,   816,   817,   818,   819,   922,   395,
     940,   785,   914,   396,  1439,  1271,   397,  1001,  1012,   820,
    1021,  1022,  1040,   792,  1045,  1063,  1073,  1075,  1079,  1081,
    1094,  1095,  1096,   821,   754,   755,   756,   757,   758,   393,
    1097,   398,  1098,   394,   393,   822,   823,   824,   394,   759,
     760,  1165,  1099,  1167,  1169,  1105,  1123,   286,  1109,   286,
    1168,  1794,  1179,  1198,  1121,   851,  1170,  1171,   390,  1172,
     286,  1627,   286,  1181,   286,  1173,  1177,   825,  1174,  1175,
     393,  1176,  1178,  1200,   394,  1183,  1180,  1182,   717,   718,
    1184,  1193,   873,   875,  1194,   391,   392,   879,  1199,   881,
    1209,   395,  1211,  1213,   578,   396,   395,  1277,   397,  1221,
     396,  1225,  1278,   397,  1234,   894,   895,   896,   897,   898,
     899,  1236,   285,  1235,   285,   390,  1242,  1252,  1648,  1250,
    1251,  1297,  1273,   398,  1262,   285,  1263,   285,   398,   285,
    1266,  1272,   395,  1276,  1282,  1285,   396,  1659,  1279,   397,
    1289,  1290,   391,   392,  1640,  1293,  1294,  1296,  1298,  1303,
     944,   390,  1304,   949,   950,  1307,  1165,  1632,  1328,  1308,
    1310,   618,  1329,  1331,   398,  1340,   390,  1344,  1345,  1346,
    1353,   286,   393,  1394,  1798,  1395,   394,  1397,   391,   392,
     996,  1398,  1409,  1404,  1581,  1405,  1411,  1583,  1423,  1432,
    1420,  1433,  1437,   391,   392,  1421,   719,   720,   721,   722,
     723,   683,  1428,   724,   725,   726,   727,  1020,   728,   729,
     730,   731,   732,   733,   734,   735,   736,   737,   738,   739,
     740,  1443,   741,   742,  1444,  1445,  1465,  1469,   286,   393,
    1471,  1485,  1488,   394,   395,  1489,   285,  1490,   396,  1491,
    1280,   397,  1492,  1497,  1499,  1500,  1029,   286,  1502,  1501,
    1506,  1507,  1512,  1567,  1140,  1539,  1541,  1566,  1546,  1547,
     717,   718,  1549,  1569,  1044,   393,   398,  1551,  1570,   394,
    1050,   752,   753,   754,   755,   756,   757,   758,  1553,  1555,
     393,  1559,  1568,  1572,   394,  1064,  1579,  1575,   759,   760,
    1574,   395,  1584,   285,  1585,   396,  1582,  1281,   397,  1587,
    1591,  1599,  1592,  1593,  1755,  1594,  1396,  1595,  1612,  1759,
    1090,  1091,   285,  1615,  1596,  1597,  1623,  1598,  1656,  1601,
    1603,  1100,  1611,   398,   390,  1614,  1106,   395,  1107,  1620,
    1108,   396,  1633,  1283,   397,   873,  1634,  1684,  1649,  1685,
    1650,  1653,   395,  1667,  1668,   683,   396,  1669,  1291,   397,
    1670,   391,   392,  1701,  1671,  1687,  1680,  1695,  1682,   398,
    1693,  1694,  1721,  1696,  1795,  1796,  1697,  1702,  1719,  1710,
    1716,  1753,  1718,  1736,   398,  1737,  1738,  1739,   719,   720,
     721,   722,   723,  1740,  1741,   724,  1747,  1705,   727,  1311,
     728,   729,   730,   731,   732,   733,   734,   735,   736,   737,
     738,   739,   740,   390,   741,   742,  1748,  1757,  1720,  1192,
    1770,  1772,  1773,  1197,  1774,  1777,  1778,  1808,  1820,  1779,
    1781,  1809,  1834,  1804,  1782,  1785,  1805,  1806,  1807,  1837,
     391,   392,   286,  1812,  1825,  1841,  1839,  1850,   393,  1224,
    1842,  1851,   394,  1854,  1864,   127,    20,   902,  1865,   130,
    1027,  1863,   372,   752,   753,   754,   755,   756,   757,   758,
    1028,  1858,  1248,   925,  1511,   921,    26,  1624,  1723,  1255,
     759,   760,  1724,  1517,  1518,  1519,  1520,  1521,  1522,  1523,
    1524,  1525,  1526,  1527,  1530,  1531,  1532,  1533,  1534,  1535,
    1536,  1015,  1765,  1275,  1661,  1766,   390,   285,   717,   718,
     395,  1767,    94,  1792,   396,  1430,  1447,   397,  1243,   390,
    1244,   380,  1140,   471,   645,   874,     0,   393,   647,   648,
       0,   394,   649,   391,   392,     0,     0,     0,   651,   652,
       0,     0,   398,     0,     0,     0,   391,   392,     0,  1020,
       0,     0,     0,     0,     0,     0,  1324,     0,     0,  1327,
    1821,     0,     0,     0,     0,  1333,     0,  1336,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1836,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
       0,     0,  1843,   396,  1845,  1452,   397,     0,  1376,  1140,
    1378,     0,  1383,     0,  1384,     0,  1387,     0,     0,     0,
    1389,     0,  1391,     0,  1393,     0,  1860,     0,  1140,     0,
     393,   398,     0,  1399,   394,     0,     0,     0,   721,   722,
       0,  1020,  1165,   393,     0,     0,   727,   394,   728,   729,
     730,   731,   732,   733,   734,   735,   736,   737,   738,   739,
     740,     0,     0,     0,     0,  1416,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1422,     0,     0,     0,     0,
    1427,     0,  1429,     0,     0,     0,     0,     0,     0,     0,
       0,  1436,   395,     0,  1438,     0,   396,     0,  1460,   397,
       0,     0,     0,     0,     0,   395,     0,     0,     0,   396,
       0,  1641,   397,   873,     0,   756,   757,   758,   717,   718,
       0,     0,     0,     0,   398,     0,   221,   390,   759,   760,
       0,     0,   222,     0,     0,     0,  1470,   398,   223,     0,
       0,     0,     0,     0,  1481,  1482,  1483,     0,   224,     0,
       0,     0,     0,     0,   391,   392,   225,     0,     0,     0,
       0,     0,     0,  1496,     0,     0,  1498,     0,     0,     0,
       0,   226,     0,     0,     0,  1504,     0,     0,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,     0,  1557,     0,  1558,   721,   722,
       0,   393,     0,     0,  1564,   394,   727,     0,     0,   729,
     730,   731,   732,   733,   734,     0,   736,   737,   738,   739,
     740,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,  1586,     0,     0,     0,     0,     0,
    1791,     0,   283,     0,     0,     0,     0,     0,     0,     0,
    1165,     0,  1605,   395,    59,     0,     0,   396,     0,  1645,
     397,     0,     0,     0,     0,     0,     0,     0,     0,  1618,
    1619,     0,     0,     0,     0,   717,   718,     0,   759,   760,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
    1628,     0,     0,   284,     0,     0,  1630,  1631,   594,     0,
       0,  1791,  1636,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1165,     0,     0,
       0,     0,     0,  1020,     0,     0,     0,     0,     0,     0,
    1791,     0,     0,     0,     0,     0,  1165,     0,     0,     0,
       0,  1020,     0,     0,     0,     0,  1654,     0,     0,  1655,
       0,  1657,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   873,  1674,  1675,  1676,  1677,     0,     0,
       0,     0,     0,   719,   720,   721,   722,   723,     0,     0,
     724,   725,   726,   727,  1686,   728,   729,   730,   731,   732,
     733,   734,   735,   736,   737,   738,   739,   740,     0,   741,
     742,     0,     0,   743,  1700,     0,     0,   744,   745,   746,
       0,     0,     0,   747,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1731,     0,
     926,     0,     0,   748,  1189,   749,   750,   751,   752,   753,
     754,   755,   756,   757,   758,  1746,     0,     0,     0,     0,
    1749,  1750,     0,     0,     0,   759,   760,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   227,   228,   229,     0,   231,   232,   233,   234,
     235,   502,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,     0,   249,   250,   251,     0,  1784,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1811,  1125,     0,     0,     0,
     484,   485,     3,     0,  -122,  -107,  -107,     0,  -119,  1819,
     486,   487,   488,   489,   490,     0,     0,     0,  1824,     0,
     491,  1126,   492,  1127,  1128,     0,   493,     0,  1835,     0,
       0,     0,     0,  1129,   494,  1130,     0,  -124,     0,  1131,
     495,     0,     0,   496,     0,     8,   497,  1132,   927,  1133,
     498,     0,  1849,  1134,  1135,     0,     0,     0,   928,     0,
    1136,     0,     0,   500,   501,     0,   227,   228,   229,     0,
     231,   232,   233,   234,   235,   502,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,     0,   249,   250,
     251,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     503,   504,   505,  1137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,   507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   508,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   509,   510,   511,   512,   513,
       0,   514,     0,   515,   516,   517,   518,   519,   520,   521,
     522,   523,    59,     0,    13,   524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,   526,   527,     0,    14,     0,     0,   528,   529,
       0,     0,     0,     0,     0,     0,     0,   530,     0,   531,
       0,   532,   533,    16,  1138,  1139,  1125,     0,     0,     0,
     484,   485,     3,     0,  -122,  -107,  -107,     0,  -119,     0,
     486,   487,   488,   489,   490,     0,     0,     0,     0,     0,
     491,  1126,   492,  1127,  1128,     0,   493,     0,     0,     0,
       0,     0,     0,  1129,   494,  1130,     0,  -124,     0,  1131,
     495,     0,     0,   496,     0,     8,   497,  1132,     0,  1133,
     498,     0,     0,  1134,  1135,     0,     0,     0,     0,     0,
    1136,     0,     0,   500,   501,     0,   227,   228,   229,     0,
     231,   232,   233,   234,   235,   502,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,     0,   249,   250,
     251,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     503,   504,   505,  1137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,   507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   508,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   509,   510,   511,   512,   513,
       0,   514,     0,   515,   516,   517,   518,   519,   520,   521,
     522,   523,    59,     0,    13,   524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,   526,   527,     0,    14,     0,     0,   528,   529,
       0,     0,     0,     0,     0,     0,     0,   530,     0,   531,
       0,   532,   533,    16,  1138,  -333,  1125,     0,     0,     0,
     484,   485,     3,     0,  -122,  -107,  -107,     0,  -119,     0,
     486,   487,   488,   489,   490,     0,     0,     0,     0,     0,
     491,  1126,   492,  1127,  1128,     0,   493,     0,     0,     0,
       0,     0,     0,  1129,   494,  1130,     0,  -124,     0,  1131,
     495,     0,     0,   496,     0,     8,   497,  1132,     0,  1133,
     498,     0,     0,  1134,  1135,     0,     0,     0,     0,     0,
    1136,     0,     0,   500,   501,     0,   227,   228,   229,     0,
     231,   232,   233,   234,   235,   502,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,     0,   249,   250,
     251,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     503,   504,   505,  1137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,   507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   508,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   509,   510,   511,   512,   513,
       0,   514,     0,   515,   516,   517,   518,   519,   520,   521,
     522,   523,    59,     0,    13,   524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,   526,   527,     0,    14,     0,     0,   528,   529,
       0,     0,     0,     0,     0,     0,     0,   530,     0,   531,
       0,   532,   533,    16,  1138,  -359,  1125,     0,     0,     0,
     484,   485,     3,     0,  -122,  -107,  -107,     0,  -119,     0,
     486,   487,   488,   489,   490,     0,     0,     0,     0,     0,
     491,  1126,   492,  1127,  1128,     0,   493,     0,     0,     0,
       0,     0,     0,  1129,   494,  1130,     0,  -124,     0,  1131,
     495,     0,     0,   496,     0,     8,   497,  1132,     0,  1133,
     498,     0,     0,  1134,  1135,     0,     0,     0,     0,     0,
    1136,     0,     0,   500,   501,     0,   227,   228,   229,     0,
     231,   232,   233,   234,   235,   502,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,     0,   249,   250,
     251,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     503,   504,   505,  1137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,   507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   508,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   509,   510,   511,   512,   513,
       0,   514,     0,   515,   516,   517,   518,   519,   520,   521,
     522,   523,    59,     0,    13,   524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,   526,   527,     0,    14,     0,     0,   528,   529,
       0,     0,     0,     0,     0,     0,     0,   530,     0,   531,
       0,   532,   533,    16,  1138,  -330,  1125,     0,     0,     0,
     484,   485,     3,     0,  -122,  -107,  -107,     0,  -119,     0,
     486,   487,   488,   489,   490,     0,     0,     0,     0,     0,
     491,  1126,   492,  1127,  1128,     0,   493,     0,     0,     0,
       0,     0,     0,  1129,   494,  1130,     0,  -124,     0,  1131,
     495,     0,     0,   496,     0,     8,   497,  1132,     0,  1133,
     498,     0,     0,  1134,  1135,     0,     0,     0,     0,     0,
    1136,     0,     0,   500,   501,     0,   227,   228,   229,     0,
     231,   232,   233,   234,   235,   502,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,     0,   249,   250,
     251,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     503,   504,   505,  1137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,   507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   508,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   509,   510,   511,   512,   513,
       0,   514,     0,   515,   516,   517,   518,   519,   520,   521,
     522,   523,    59,     0,    13,   524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,   526,   527,     0,    14,     0,     0,   528,   529,
       0,     0,     0,     0,     0,     0,     0,   530,     0,   531,
       0,   532,   533,    16,  1138,   -98,  1125,     0,     0,     0,
     484,   485,     3,     0,  -122,  -107,  -107,     0,  -119,     0,
     486,   487,   488,   489,   490,     0,     0,     0,     0,     0,
     491,  1126,   492,  1127,  1128,     0,   493,     0,     0,     0,
       0,     0,     0,  1129,   494,  1130,     0,  -124,     0,  1131,
     495,     0,     0,   496,     0,     8,   497,  1132,     0,  1133,
     498,     0,     0,  1134,  1135,     0,     0,     0,     0,     0,
    1136,     0,     0,   500,   501,     0,   227,   228,   229,     0,
     231,   232,   233,   234,   235,   502,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,     0,   249,   250,
     251,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     503,   504,   505,  1137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,   507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   508,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   509,   510,   511,   512,   513,
       0,   514,     0,   515,   516,   517,   518,   519,   520,   521,
     522,   523,    59,     0,    13,   524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,   526,   527,     0,    14,     0,     0,   528,   529,
       0,     0,     0,     0,     0,     0,     0,   530,     0,   531,
       0,   532,   533,    16,  1138,  -103,   484,   485,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,   489,
     490,     0,     0,     0,     0,     0,   491,     0,   492,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
     494,     0,     0,     0,     0,     0,   495,     0,     0,   496,
       0,     0,   497,     0,     0,     0,   498,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,     0,     0,   500,
     501,   916,   227,   228,   229,     0,   231,   232,   233,   234,
     235,   502,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,     0,   249,   250,   251,     0,     0,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,   581,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,   700,   515,
     516,   517,   518,   519,   520,   521,   522,   523,   701,     0,
       0,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
       0,    14,     0,     0,   528,   529,     0,     0,     0,     0,
       0,   484,   485,   917,     0,   531,   918,   532,   533,   697,
     534,   486,   487,   488,   489,   490,     0,     0,     0,     0,
       0,   491,     0,   492,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,   495,     0,     0,   496,   698,     0,   497,     0,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   499,     0,     0,   500,   501,     0,   227,   228,   229,
       0,   231,   232,   233,   234,   235,   502,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,     0,   249,
     250,   251,     0,     0,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   503,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,   581,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   509,   510,   511,   512,
     513,     0,   514,   700,   515,   516,   517,   518,   519,   520,
     521,   522,   523,   701,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,     0,    14,     0,     0,   528,
     529,     0,     0,     0,     0,     0,   484,   485,   530,     0,
     531,     0,   532,   533,   697,   534,   486,   487,   488,   489,
     490,     0,     0,     0,     0,     0,   491,     0,   492,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
     494,     0,     0,     0,     0,     0,   495,     0,     0,   496,
     698,     0,   497,     0,     0,     0,   498,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,     0,     0,   500,
     501,     0,   227,   228,   229,     0,   231,   232,   233,   234,
     235,   502,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,     0,   249,   250,   251,     0,     0,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,   581,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,     0,   515,
     516,   517,   518,   519,   520,   521,   522,   523,    59,     0,
       0,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
       0,    14,     0,     0,   528,   529,     0,     0,     0,     0,
       0,   484,   485,   530,     0,   531,     0,   532,   533,     0,
     534,   486,   487,   488,   489,   490,     0,     0,     0,     0,
       0,   491,     0,   492,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,   495,     0,     0,   496,     0,     0,   497,     0,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   499,     0,     0,   500,   501,  1059,   227,   228,   229,
       0,   231,   232,   233,   234,   235,   502,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,     0,   249,
     250,   251,     0,     0,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   503,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,   581,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   509,   510,   511,   512,
     513,     0,   514,   700,   515,   516,   517,   518,   519,   520,
     521,   522,   523,   701,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,     0,    14,     0,     0,   528,
     529,     0,     0,     0,     0,     0,   484,   485,   530,     0,
     531,     0,   532,   533,     0,   534,   486,   487,   488,   489,
     490,     0,     0,     0,     0,     0,   491,     0,   492,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
     494,     0,     0,     0,     0,     0,   495,     0,     0,   496,
       0,     0,   497,     0,     0,     0,   498,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,     0,     0,   500,
     501,     0,   227,   228,   229,     0,   231,   232,   233,   234,
     235,   502,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,     0,   249,   250,   251,     0,     0,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,   581,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,   700,   515,
     516,   517,   518,   519,   520,   521,   522,   523,   701,     0,
       0,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
       0,    14,     0,     0,   528,   529,     0,     0,     0,     0,
       0,   484,   485,  1379,     0,   531,  1380,   532,   533,     0,
     534,   486,   487,   488,   489,   490,     0,     0,     0,     0,
       0,   491,     0,   492,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,   495,     0,     0,   496,     0,     0,   497,     0,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   499,     0,     0,   500,   501,     0,   227,   228,   229,
       0,   231,   232,   233,   234,   235,   502,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,     0,   249,
     250,   251,     0,     0,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   503,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,   581,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   509,   510,   511,   512,
     513,     0,   514,   700,   515,   516,   517,   518,   519,   520,
     521,   522,   523,   701,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,     0,    14,     0,     0,   528,
     529,     0,     0,     0,     0,     0,   484,   485,  1400,     0,
     531,  1401,   532,   533,     0,   534,   486,   487,   488,   489,
     490,     0,     0,     0,     0,     0,   491,     0,   492,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
     494,     0,     0,     0,     0,     0,   495,     0,     0,   496,
       0,     0,   497,     0,     0,     0,   498,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,     0,     0,   500,
     501,     0,   227,   228,   229,     0,   231,   232,   233,   234,
     235,   502,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,     0,   249,   250,   251,     0,     0,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,   581,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,   700,   515,
     516,   517,   518,   519,   520,   521,   522,   523,   701,     0,
       0,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
       0,    14,     0,     0,   528,   529,     0,     0,     0,     0,
       0,   484,   485,   530,     0,   531,     0,   532,   533,     0,
     534,   486,   487,   488,   489,   490,     0,     0,     0,     0,
       0,   491,     0,   492,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,   495,     0,     0,   496,     0,     0,   497,     0,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   499,     0,     0,   500,   501,     0,   227,   228,   229,
       0,   231,   232,   233,   234,   235,   502,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,     0,   249,
     250,   251,     0,     0,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   503,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,   581,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   509,   510,   511,   512,
     513,     0,   514,     0,   515,   516,   517,   518,   519,   520,
     521,   522,   523,    59,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,     0,    14,     0,     0,   528,
     529,     0,     0,     0,     0,     0,   484,   485,   530,     0,
     531,   997,   532,   533,     0,   534,   486,   487,   488,   489,
     490,     0,     0,     0,     0,     0,   491,     0,   492,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
     494,     0,     0,     0,     0,     0,   495,     0,     0,   496,
       0,     0,   497,     0,     0,     0,   498,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,     0,     0,   500,
     501,     0,   227,   228,   229,     0,   231,   232,   233,   234,
     235,   502,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,     0,   249,   250,   251,     0,     0,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,  1019,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,   700,   515,
     516,   517,   518,   519,   520,   521,   522,   523,   701,     0,
       0,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
       0,    14,     0,     0,   528,   529,     0,     0,     0,     0,
       0,   484,   485,  1313,     0,   531,     0,   532,   533,     0,
     534,   486,   487,   488,   489,   490,     0,     0,     0,     0,
       0,   491,     0,   492,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,   495,     0,     0,   496,     0,     0,   497,     0,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   499,     0,     0,   500,   501,     0,   227,   228,   229,
       0,   231,   232,   233,   234,   235,   502,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,     0,   249,
     250,   251,     0,     0,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   503,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,   581,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   509,   510,   511,   512,
     513,     0,   514,     0,   515,   516,   517,   518,   519,   520,
     521,   522,   523,    59,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,     0,    14,     0,     0,   528,
     529,     0,     0,     0,     0,     0,   484,   485,   530,     0,
     531,  1385,   532,   533,     0,   534,   486,   487,   488,   489,
     490,     0,     0,     0,     0,     0,   491,     0,   492,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
     494,     0,     0,     0,     0,     0,   495,     0,     0,   496,
       0,     0,   497,     0,     0,     0,   498,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,     0,     0,   500,
     501,     0,   227,   228,   229,     0,   231,   232,   233,   234,
     235,   502,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,     0,   249,   250,   251,     0,     0,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,   581,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,     0,   515,
     516,   517,   518,   519,   520,   521,   522,   523,    59,     0,
       0,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
       0,    14,     0,     0,   528,   529,     0,     0,     0,     0,
       0,   484,   485,   530,     0,   531,  1407,   532,   533,     0,
     534,   486,   487,   488,   489,   490,     0,     0,     0,     0,
       0,   491,     0,   492,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,   495,     0,     0,   496,     0,     0,   497,     0,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   499,     0,     0,   500,   501,     0,   227,   228,   229,
       0,   231,   232,   233,   234,   235,   502,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,     0,   249,
     250,   251,     0,     0,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   503,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,   581,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   509,   510,   511,   512,
     513,     0,   514,     0,   515,   516,   517,   518,   519,   520,
     521,   522,   523,    59,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,     0,    14,     0,     0,   528,
     529,     0,     0,     0,     0,     0,   484,   485,   530,     0,
     531,  1466,   532,   533,     0,   534,   486,   487,   488,   489,
     490,     0,     0,     0,     0,     0,   491,     0,   492,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
     494,     0,     0,     0,     0,     0,   495,     0,     0,   496,
       0,     0,   497,     0,     0,     0,   498,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,     0,     0,   500,
     501,     0,   227,   228,   229,     0,   231,   232,   233,   234,
     235,   502,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,     0,   249,   250,   251,     0,     0,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,  1019,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,   700,   515,
     516,   517,   518,   519,   520,   521,   522,   523,   701,     0,
       0,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
       0,    14,     0,     0,   528,   529,     0,     0,     0,     0,
       0,   484,   485,   530,     0,   531,     0,   532,   533,     0,
     534,   486,   487,   488,   489,   490,     0,     0,     0,     0,
       0,   491,     0,   492,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,   495,     0,     0,   496,     0,     0,   497,     0,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   499,     0,     0,   500,   501,     0,   227,   228,   229,
       0,   231,   232,   233,   234,   235,   502,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,     0,   249,
     250,   251,     0,     0,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   503,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,   581,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   509,   510,   511,   512,
     513,     0,   514,     0,   515,   516,   517,   518,   519,   520,
     521,   522,   523,    59,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,     0,    14,     0,     0,   528,
     529,     0,     0,     0,     0,     0,   484,   485,   530,     0,
     531,     0,   532,   533,     0,   534,   486,   487,   488,   489,
     490,     0,     0,     0,     0,     0,   491,     0,   492,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
     494,     0,     0,     0,     0,     0,   495,     0,     0,   496,
       0,     0,   497,     0,     0,     0,   498,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,     0,     0,   500,
     501,     0,   227,   228,   229,     0,   231,   232,   233,   234,
     235,   502,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,     0,   249,   250,   251,     0,     0,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,     0,   515,
     516,   517,   518,   519,   520,   521,   522,   523,    59,     0,
       0,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
       0,    14,     0,     0,   528,   529,     0,     0,     0,     0,
       0,   484,   485,   530,   591,   531,     0,   532,   533,     0,
     534,   486,   487,   488,   489,   490,     0,     0,     0,     0,
       0,   491,     0,   492,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,   495,     0,     0,   496,     0,     0,   497,     0,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   499,     0,     0,   500,   501,     0,   227,   228,   229,
       0,   231,   232,   233,   234,   235,   502,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,     0,   249,
     250,   251,     0,     0,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   503,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   509,   510,   511,   512,
     513,     0,   514,     0,   515,   516,   517,   518,   519,   520,
     521,   522,   523,    59,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,     0,    14,     0,     0,   528,
     529,     0,     0,     0,     0,     0,   484,   485,   530,   791,
     531,     0,   532,   533,     0,   534,   486,   487,   488,   489,
     490,     0,     0,     0,     0,     0,   491,     0,   492,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
     494,     0,     0,     0,     0,     0,   495,     0,     0,   496,
       0,     0,   497,     0,     0,     0,   498,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,     0,     0,   500,
     501,     0,   227,   228,   229,     0,   231,   232,   233,   234,
     235,   502,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,     0,   249,   250,   251,     0,     0,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,  1019,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,     0,   515,
     516,   517,   518,   519,   520,   521,   522,   523,    59,     0,
       0,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
       0,    14,     0,     0,   528,   529,     0,     0,     0,     0,
       0,   484,   485,   530,     0,   531,     0,   532,   533,  1112,
     534,   486,   487,   488,   489,   490,     0,     0,     0,     0,
       0,   491,     0,   492,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,   495,     0,     0,   496,     0,     0,   497,     0,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   499,     0,     0,   500,   501,     0,   227,   228,   229,
       0,   231,   232,   233,   234,   235,   502,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,     0,   249,
     250,   251,     0,     0,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   503,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   509,   510,   511,   512,
     513,     0,   514,     0,   515,   516,   517,   518,   519,   520,
     521,   522,   523,    59,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,     0,    14,     0,     0,   528,
     529,     0,     0,     0,     0,     0,   484,   485,   530,     0,
     531,     0,   532,   533,     0,   534,   486,   487,   488,   489,
     490,     0,     0,     0,     0,     0,   491,     0,   492,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
     494,     0,     0,     0,     0,     0,   495,     0,     0,   496,
       0,     0,   497,     0,     0,     0,   498,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,     0,     0,   500,
     501,     0,   227,   228,   229,     0,   231,   232,   233,   234,
     235,   502,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,     0,   249,   250,   251,     0,     0,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,  1323,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,     0,   515,
     516,   517,   518,   519,   520,   521,   522,   523,    59,     0,
       0,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
       0,    14,     0,     0,   528,   529,     0,     0,     0,     0,
       0,   484,   485,   530,     0,   531,     0,   532,   533,     0,
     534,   486,   487,   488,   489,   490,     0,     0,     0,     0,
       0,   491,     0,   492,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,   495,     0,     0,   496,     0,     0,   497,     0,     0,
       0,   498,     0,     0,     0,     0,     0,  1326,     0,     0,
       0,   499,     0,     0,   500,   501,     0,   227,   228,   229,
       0,   231,   232,   233,   234,   235,   502,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,     0,   249,
     250,   251,     0,     0,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   503,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   509,   510,   511,   512,
     513,     0,   514,     0,   515,   516,   517,   518,   519,   520,
     521,   522,   523,    59,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,     0,    14,     0,     0,   528,
     529,     0,     0,     0,     0,     0,   484,   485,   530,     0,
     531,     0,   532,   533,     0,   534,   486,   487,   488,   489,
     490,     0,     0,     0,     0,     0,   491,     0,   492,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
     494,     0,     0,     0,     0,     0,   495,     0,     0,   496,
       0,     0,   497,     0,     0,     0,   498,     0,     0,  1332,
       0,     0,     0,     0,     0,     0,   499,     0,     0,   500,
     501,     0,   227,   228,   229,     0,   231,   232,   233,   234,
     235,   502,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,     0,   249,   250,   251,     0,     0,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,     0,   515,
     516,   517,   518,   519,   520,   521,   522,   523,    59,     0,
       0,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
       0,    14,     0,     0,   528,   529,     0,     0,     0,     0,
       0,   484,   485,   530,     0,   531,     0,   532,   533,     0,
     534,   486,   487,   488,   489,   490,     0,     0,     0,     0,
       0,   491,     0,   492,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,   495,     0,     0,   496,     0,     0,   497,     0,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   499,     0,     0,   500,   501,     0,   227,   228,   229,
       0,   231,   232,   233,   234,   235,   502,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,     0,   249,
     250,   251,     0,     0,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   503,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,  1335,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   509,   510,   511,   512,
     513,     0,   514,     0,   515,   516,   517,   518,   519,   520,
     521,   522,   523,    59,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,     0,    14,     0,     0,   528,
     529,     0,     0,     0,     0,     0,   484,   485,   530,     0,
     531,     0,   532,   533,     0,   534,   486,   487,   488,   489,
     490,     0,     0,     0,     0,     0,   491,     0,   492,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
     494,     0,     0,     0,     0,     0,   495,     0,     0,   496,
       0,     0,   497,     0,     0,     0,   498,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,     0,     0,   500,
     501,     0,   227,   228,   229,     0,   231,   232,   233,   234,
     235,   502,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,     0,   249,   250,   251,     0,     0,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,     0,   515,
     516,   517,   518,   519,   520,   521,   522,   523,    59,     0,
       0,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
       0,    14,     0,     0,   528,   529,     0,     0,     0,     0,
       0,   484,   485,   530,     0,   531,  1604,   532,   533,     0,
     534,   486,   487,   488,   489,   490,     0,     0,     0,     0,
       0,   491,     0,   492,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,   495,     0,     0,   496,     0,     0,   497,     0,     0,
       0,   498,  1635,     0,     0,     0,     0,     0,     0,     0,
       0,   499,     0,     0,   500,   501,     0,   227,   228,   229,
       0,   231,   232,   233,   234,   235,   502,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,     0,   249,
     250,   251,     0,     0,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   503,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   509,   510,   511,   512,
     513,     0,   514,     0,   515,   516,   517,   518,   519,   520,
     521,   522,   523,    59,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,     0,    14,     0,     0,   528,
     529,     0,     0,     0,     0,     0,   484,   485,   530,     0,
     531,     0,   532,   533,     0,   534,   486,   487,   488,   489,
     490,     0,     0,     0,     0,     0,   491,     0,   492,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,     0,
     494,     0,     0,     0,     0,     0,   495,     0,     0,   496,
       0,     0,   497,     0,     0,     0,   498,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,     0,     0,   500,
     501,     0,   227,   228,   229,     0,   231,   232,   233,   234,
     235,   502,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,     0,   249,   250,   251,     0,     0,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,     0,   515,
     516,   517,   518,   519,   520,   521,   522,   523,    59,     0,
       0,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
       0,    14,     0,     0,   528,   529,     0,     0,     0,     0,
       0,   484,   485,   530,     0,   531,     0,   532,   533,     0,
     534,   486,   487,   488,   489,   490,     0,     0,     0,     0,
       0,   491,  1126,   492,  1127,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,   495,     0,     0,   496,     0,     0,   497,  1132,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   499,     0,     0,   500,   501,     0,   227,   228,   229,
       0,   231,   232,   233,   234,   235,   502,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,     0,   249,
     250,   251,     0,     0,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   503,   504,   505,  1137,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   509,   510,   511,   512,
     513,     0,   514,     0,   515,   516,   517,   518,   519,   520,
     521,   522,   523,    59,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,     0,    14,     0,     0,   528,
     529,     0,     0,     0,   484,   485,     0,     0,   530,     0,
     531,     0,   532,   533,   486,   487,   488,   489,   490,     0,
       0,     0,     0,     0,   491,     0,   492,     0,     0,     0,
     493,     0,     0,     0,     0,     0,     0,     0,   494,     0,
       0,     0,     0,     0,   495,     0,     0,   496,     0,     0,
     497,     0,     0,     0,   498,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   499,     0,     0,   500,   501,     0,
     227,   228,   229,     0,   231,   232,   233,   234,   235,   502,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,     0,   249,   250,   251,     0,     0,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   503,   504,   505,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   507,     0,     0,     0,     0,     0,     0,     0,   836,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   508,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   509,
     510,   511,   512,   513,     0,   514,     0,   515,   516,   517,
     518,   519,   520,   521,   522,   523,    59,     0,     0,   524,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525,   526,   527,     0,    14,
       0,     0,   528,   529,     0,     0,     0,   484,   485,     0,
       0,   530,     0,   531,     0,   532,   533,   486,   487,   488,
     489,   490,     0,     0,   976,     0,     0,   491,     0,   492,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,   494,     0,     0,     0,     0,     0,   495,     0,     0,
     496,     0,     0,   497,     0,     0,     0,   498,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   499,     0,     0,
     500,   501,     0,   227,   228,   229,     0,   231,   232,   233,
     234,   235,   502,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,     0,   249,   250,   251,     0,     0,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   503,   504,   505,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   506,   507,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   508,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   509,   510,   511,   512,   513,     0,   514,     0,
     515,   516,   517,   518,   519,   520,   521,   522,   523,    59,
       0,     0,   524,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   525,   526,
     527,     0,    14,     0,     0,   528,   529,     0,     0,     0,
     484,   485,     0,     0,   530,     0,   531,     0,   532,   533,
     486,   487,   488,   489,   490,     0,     0,     0,     0,     0,
     491,     0,   492,     0,     0,     0,   493,     0,     0,     0,
       0,     0,     0,     0,   494,     0,     0,     0,     0,     0,
     495,     0,     0,   496,     0,     0,   497,     0,     0,     0,
     498,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     499,     0,     0,   500,   501,     0,   227,   228,   229,     0,
     231,   232,   233,   234,   235,   502,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,     0,   249,   250,
     251,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     503,   504,   505,   -88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   717,   718,   506,   507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   508,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   509,   510,   511,   512,   513,
       0,   514,     0,   515,   516,   517,   518,   519,   520,   521,
     522,   523,    59,     0,     0,   524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,   526,   527,     0,    14,     0,     0,   528,   529,
       0,     0,     0,     0,     0,     0,     0,   530,     0,   531,
       0,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1356,  1357,  1358,  1359,  1360,  1361,
    1362,  1363,   719,   720,   721,   722,   723,  1364,  1365,   724,
     725,   726,   727,  1366,   728,   729,   730,   731,   732,   733,
     734,   735,   736,   737,   738,   739,   740,  1367,   741,   742,
    1368,  1369,   743,     0,     0,     0,   744,   745,   746,  1370,
    1371,  1372,   747,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1373,   748,     0,   749,   750,   751,   752,   753,   754,
     755,   756,   757,   758,   221,     0,     0,     0,     0,     0,
     222,     0,     0,     0,   759,   760,   223,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,     0,     0,     0,   225,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   226,
       0,     0,     0,     0,     0,     0,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     283,     0,     0,   221,     0,     0,     0,     0,     0,   222,
       0,     0,   773,     0,    13,   223,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   224,     0,     0,     0,     0,
       0,     0,     0,   225,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,   284,     0,    16,     0,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   283,
     221,     0,     0,   406,   407,     0,   222,     0,     0,     0,
       0,    59,   223,     0,     0,     0,     0,     0,     0,     0,
     408,     0,   224,     0,     0,     0,     0,     0,     0,     0,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   226,     0,     0,     0,     0,
     284,     0,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   717,   718,
       0,     0,     0,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   424,   425,
     426,     0,     0,   427,   428,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,   431,   432,   433,
     434,     0,     0,    58,     0,   435,   436,   437,   438,   439,
     440,   441,     0,     0,     0,     0,   283,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   773,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   442,     0,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   717,   718,   453,   454,     0,     0,     0,
       0,     0,     0,   455,   456,     0,     0,   284,     0,     0,
       0,     0,     0,     0,     0,     0,   719,   720,   721,   722,
     723,     0,     0,   724,   725,   726,   727,     0,   728,   729,
     730,   731,   732,   733,   734,   735,   736,   737,   738,   739,
     740,     0,   741,   742,     0,     0,   743,     0,     0,     0,
     744,   745,   746,     0,     0,     0,   747,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     717,   718,     0,     0,     0,     0,   748,     0,   749,   750,
     751,   752,   753,   754,   755,   756,   757,   758,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   759,   760,
       0,   719,   720,   721,   722,   723,     0,     0,   724,   725,
     726,   727,     0,   728,   729,   730,   731,   732,   733,   734,
     735,   736,   737,   738,   739,   740,     0,   741,   742,     0,
       0,  -937,     0,     0,     0,   744,   745,   746,     0,     0,
       0,  -937,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   717,   718,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   748,     0,   749,   750,   751,   752,   753,   754,   755,
     756,   757,   758,     0,     0,     0,     0,     0,   719,   720,
     721,   722,   723,   759,   760,   724,   725,   726,   727,     0,
     728,   729,   730,   731,   732,   733,   734,   735,   736,   737,
     738,   739,   740,     0,   741,   742,     0,     0,     0,     0,
       0,     0,   744,   745,   746,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   717,   718,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   748,     0,
     749,   750,   751,   752,   753,   754,   755,   756,   757,   758,
       0,     0,   717,   718,     0,   719,   720,   721,   722,   723,
     759,   760,   724,   725,   726,   727,     0,   728,   729,   730,
     731,   732,   733,   734,   735,   736,   737,   738,   739,   740,
       0,   741,   742,     0,     0,     0,     0,     0,     0,   744,
       0,   746,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   749,   750,   751,
     752,   753,   754,   755,   756,   757,   758,     0,     0,   717,
     718,     0,   719,   720,   721,   722,   723,   759,   760,   724,
     725,   726,   727,     0,   728,   729,   730,   731,   732,   733,
     734,   735,   736,   737,   738,   739,   740,     0,   741,   742,
     719,   720,   721,   722,   723,     0,   744,   724,   725,   726,
     727,     0,   728,   729,   730,   731,   732,   733,   734,   735,
     736,   737,   738,   739,   740,     0,   741,   742,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   749,   750,   751,   752,   753,   754,
     755,   756,   757,   758,     0,     0,   717,   718,     0,     0,
       0,     0,     0,     0,   759,   760,     0,     0,     0,     0,
       0,     0,   749,   750,   751,   752,   753,   754,   755,   756,
     757,   758,     0,     0,     0,     0,     0,   719,   720,   721,
     722,   723,   759,   760,   724,   725,   726,   727,     0,   728,
     729,   730,   731,   732,   733,   734,   735,   736,   737,   738,
     739,   740,     0,   741,   742,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     750,   751,   752,   753,   754,   755,   756,   757,   758,     0,
       0,     0,     0,     0,   719,   720,   721,   722,   723,   759,
     760,   724,   725,   726,   727,     0,   728,   729,   730,   731,
     732,   733,   734,   735,   736,   737,   738,   739,   740,     0,
     741,   742,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   930,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   751,   752,
     753,   754,   755,   756,   757,   758,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   759,   760,   227,   228,
     229,   952,   231,   232,   233,   234,   235,   502,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,     0,
     249,   250,   251,     0,     0,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,     0,   227,   228,   229,     0,   231,   232,   233,
     234,   235,   502,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,     0,   249,   250,   251,     0,     0,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,     0,     0,     0,
       0,     0,     0,     0,   931,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   932,     0,     0,     0,   956,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   953,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   954,
     227,   228,   229,   960,   231,   232,   233,   234,   235,   502,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,     0,   249,   250,   251,     0,     0,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,     0,   227,   228,   229,     0,   231,
     232,   233,   234,   235,   502,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,     0,   249,   250,   251,
       0,     0,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,     0,
       0,     0,     0,     0,     0,     0,   957,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   958,     0,     0,     0,
    1185,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   961,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   962,   227,   228,   229,     0,   231,   232,   233,   234,
     235,   502,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   304,   249,   250,   251,     0,     0,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,     0,     0,     0,   305,
       0,   306,     0,   307,   308,   309,   310,   311,     0,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
       0,   323,   324,   325,     0,     0,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,     0,     0,     0,     0,     0,  1186,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1187,   353,
     354,   227,   228,   229,     0,   231,   232,   233,   234,   235,
     502,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,     0,   249,   250,   251,     0,     0,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   355,     0,   305,     0,   306,
       0,   307,   308,   309,   310,   311,     0,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,     0,   323,
     324,   325,   988,   989,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,     0,     0,     0,     0,     0,     0,   990,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   991,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   992,   993,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   305,     0,   306,     0,   307,   308,
     309,   310,   311,   599,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,     0,   323,   324,   325,     0,
       0,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   305,     0,
     306,     0,   307,   308,   309,   310,   311,     0,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,     0,
     323,   324,   325,     0,     0,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     601,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   798
};

static const yytype_int16 yycheck[] =
{
       1,    14,    15,   104,   147,   148,   389,   195,   196,   488,
     174,   488,   627,    45,   631,  1105,   702,   703,   838,    20,
      21,    22,  1128,   639,   569,   768,   571,   770,   573,   772,
    1034,  1495,   598,   706,  1273,   597,     5,  1023,     8,    81,
      22,  1017,   925,   406,   407,    19,  1516,    33,     0,    50,
      51,    64,    65,    66,    86,     7,  1042,  1464,    33,   149,
    1753,    20,    46,   149,   169,   401,    20,  1458,  1681,   149,
      40,    20,    20,     7,   888,    57,   531,  1753,    30,   885,
      32,   172,    34,     5,     6,     8,   171,   199,    40,    19,
      20,  1753,   105,   106,   107,   108,   782,   171,    50,   131,
     132,   171,   134,    25,    56,  1764,  1784,  1111,    33,    31,
     184,  1753,   170,    36,  1753,  1477,    50,   195,   196,   210,
     210,  1814,   234,   578,   210,   215,    15,    16,    80,   215,
     210,  1744,  1695,  1811,   239,    60,    61,   171,  1814,    69,
     207,   150,   478,  1802,   234,   129,    68,    69,   234,   234,
     184,  1513,  1814,   917,   239,   164,   125,   126,   189,    33,
    1853,   235,   232,   779,    15,    16,   207,   783,  1638,   211,
     172,   238,  1814,   125,   126,  1814,  1640,  1853,   236,   237,
     179,   239,   195,   196,   199,   871,    60,    61,   874,    34,
     863,  1853,   233,  1584,  1585,   197,   211,   883,   197,   208,
     886,  1608,   366,   125,   126,   660,  1089,    57,    62,   210,
    1601,  1853,  1603,    63,  1853,   358,   671,  1307,    63,   674,
     363,   220,   147,   238,   367,   150,   151,   213,  1334,   199,
     197,   848,   231,   232,   173,   174,   175,   219,   213,   164,
     214,  1227,   605,   916,  1220,   214,   232,   199,   207,   231,
     172,   706,   209,   207,  1068,   241,   872,  1243,   207,   207,
     207,  1067,   232,   150,   743,   234,   743,   197,   220,   236,
     240,   284,   187,   147,   199,   197,    33,   151,  1669,  1670,
     232,   170,   197,   208,   209,   130,   238,  1850,   213,  1680,
     149,   216,   214,   481,   482,   483,   235,   187,   199,  1303,
     189,  1864,    33,    60,    61,   194,   761,   197,   197,   208,
     232,   200,   207,   238,   150,   181,   241,   239,   163,   207,
     235,   208,   219,   401,   211,   661,   368,   172,   164,    60,
      61,   197,   149,   476,   231,   209,   172,   238,   189,   213,
     235,   677,   216,   194,   234,   234,   197,   235,   893,   200,
    1093,   210,   197,   181,  1593,  1119,   215,  1463,  1666,   152,
     153,   154,   155,   156,   157,     5,     6,   241,   234,   197,
     207,  1679,   208,  1837,   199,   234,   564,   565,   923,   224,
     568,  1124,   570,   199,   572,   207,  1059,   150,   401,   234,
     147,  1497,   580,   210,   151,   207,   233,   150,   215,  1015,
     478,   164,   210,   481,   482,   483,   234,  1171,   863,    33,
     197,   233,   199,   238,   214,   240,   147,   234,   234,   150,
     151,   150,  1426,   235,  1732,  1733,   614,   615,  1114,   210,
    1194,  1048,   213,   164,   234,   164,    60,    61,   231,   232,
     207,   198,   199,   207,   198,   208,   181,  1063,  1014,  1011,
     207,   238,   209,   207,   207,   208,   213,   210,   215,   216,
     213,  1027,   197,  1025,   199,   478,   233,   207,   481,   482,
     483,   235,  1088,   207,   150,   488,   489,   208,   209,   208,
     207,   238,   213,   207,   241,   216,   564,   565,   164,   207,
     568,   197,   570,    33,   572,   508,   574,   172,   179,   197,
     150,   235,   580,   238,   172,   240,   233,   207,   150,   233,
     241,   847,   179,   181,   164,   233,   197,  1337,   234,   597,
      60,    61,   164,   147,   860,   861,   173,   151,   175,   197,
     197,   130,   208,   189,   870,   235,   614,   615,   219,   220,
     876,   877,   878,  1633,   880,   214,   882,   197,   884,  1313,
     231,   564,   565,   220,   219,   568,   892,   570,   208,   572,
     219,   574,   705,   213,   231,   234,   208,   580,   149,   201,
     202,   152,   153,   154,   155,   156,   157,   211,   181,   181,
     207,   215,   207,   661,   597,   209,   201,   198,   199,   213,
     215,   215,   216,   781,   197,   197,   207,   198,   199,   677,
     211,   614,   615,    64,    65,    66,   207,   147,   235,   622,
     172,   151,    57,   238,   215,  1379,  1620,   241,    63,   181,
     197,  1166,    21,    22,   186,   198,   199,   238,   207,   207,
     631,  1374,   198,   199,   207,   197,  1400,   238,   207,   717,
     718,   207,   215,   214,   105,   106,   107,   108,   661,   215,
     231,   232,   214,  1759,  1270,    57,   235,   235,   736,   737,
     738,    63,   740,   234,   677,   238,   235,    10,    11,   209,
     181,   859,   238,   213,   862,   215,   216,   207,   866,   198,
     199,   759,   198,   199,   207,   207,   197,   207,   207,  1795,
    1796,   207,   187,   207,   707,   207,   215,   198,   199,   215,
     199,   241,   197,   781,   207,   235,   207,   207,   198,   199,
     207,    79,   235,   235,   215,   235,  1171,   207,   207,   238,
    1175,   235,   238,   235,   181,   215,    94,   214,  1834,   214,
    1066,    99,   235,   101,   197,   235,  1072,   238,   235,  1194,
     197,   210,   141,   142,  1199,   214,   235,   234,   238,   234,
     149,  1087,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   207,   214,   214,   781,   847,
     131,   132,   133,   134,   135,   136,   137,   138,   197,  1543,
     207,   859,   860,   861,   862,   207,   234,   234,   866,   197,
      47,   130,   870,   235,   795,    33,   797,  1561,   876,   877,
     878,   207,   880,   164,   882,   197,   884,   885,   235,   207,
      67,   198,   199,   235,   892,   176,   177,   178,    21,    22,
     207,  1009,    60,    61,  1012,   130,   198,   199,   215,   235,
    1018,   187,   231,   232,   847,   207,    75,   235,  1293,   197,
      79,   197,   187,   215,   239,  1300,   859,   860,   861,   862,
     851,   238,   197,   866,    93,    94,   197,   870,   231,    98,
      99,   100,   101,   876,   877,   878,   238,   880,   219,   882,
      12,   884,   885,   506,   507,   197,   198,    57,   187,   892,
     231,    23,    24,    63,   235,   207,   197,  1366,   197,  1366,
     197,   198,   525,   526,   527,   528,   529,    57,   976,  1719,
     207,  1665,  1666,    63,   187,    57,   187,  1671,   187,   147,
     197,    63,   233,   151,   197,  1679,   197,  1681,   197,   207,
      57,   207,   210,   924,   210,   213,    63,   213,   201,   202,
     203,  1009,   197,  1011,  1012,   224,   139,   140,   141,   142,
    1018,   546,   547,   210,   549,   550,   149,  1025,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,  1725,   165,   166,   238,   239,   211,   208,  1732,  1733,
     215,   209,   173,   174,   175,   213,   201,   215,   216,   211,
    1744,   211,   211,   215,   617,   215,   215,   211,  1066,  1067,
     208,   215,  1678,  1136,  1072,   173,  1009,   211,  1011,  1012,
     211,   215,   211,   241,   215,  1018,   215,  1827,   547,  1087,
     549,   550,  1025,   216,   217,   218,   219,   220,   211,   193,
     194,   211,   215,  1478,  1025,   215,  1027,   211,   231,   232,
     211,   215,  1344,  1345,   215,   199,  1856,   171,  1039,  1375,
    1804,  1727,   201,   202,   203,   204,   376,  1048,   197,  1050,
    1238,    57,  1388,  1066,  1067,    66,   386,   210,   198,  1072,
     201,   202,   203,   129,   830,   831,   832,   171,   197,   399,
      55,    56,    57,    33,  1087,   214,   214,   234,   214,    21,
      22,   214,   214,   234,   214,  1249,   719,   720,  1703,   214,
     723,   724,   725,   726,   214,   728,   214,   214,   211,   234,
      60,    61,   735,   198,   232,    35,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,   750,   751,   752,
     753,   754,   755,   756,   757,   758,    35,   239,   239,   234,
     197,   214,  1133,   197,   232,   197,   239,   233,    22,   197,
    1141,  1142,   233,  1598,  1761,  1288,   239,   210,   197,   214,
     234,   232,   234,  1154,   214,  1156,  1157,  1158,  1159,  1160,
    1238,   214,   214,  1164,   214,    33,  1621,   214,   214,   214,
      33,   215,   197,   234,   207,   234,    43,   234,   234,  1322,
     234,  1798,   214,   234,   215,   214,   234,   147,   214,  1377,
    1354,   151,    60,    61,   235,   234,   233,    60,    61,   141,
     142,   235,  1390,   836,  1392,   235,    33,   149,  1591,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   235,   233,   238,  1238,   234,   214,   235,   234,
     234,   234,   234,    60,    61,   234,    13,   233,   207,   234,
     131,   132,   133,   134,   135,   136,   137,   138,     4,   209,
     197,   581,   235,   213,  1255,   215,   216,   197,   210,   150,
     233,   197,   197,   593,   197,   197,   207,   235,   235,   235,
     235,   235,   235,   164,   216,   217,   218,   219,   220,   147,
     235,   241,   235,   151,   147,   176,   177,   178,   151,   231,
     232,   924,   235,   214,   214,   234,   234,  1375,   235,  1377,
     234,  1756,   214,     1,   235,   635,   234,   234,    33,   234,
    1388,  1499,  1390,   214,  1392,   233,   233,   208,   234,   234,
     147,   234,   233,   233,   151,   214,   234,   234,    21,    22,
     234,   234,   662,   663,   234,    60,    61,   667,   234,   669,
     215,   209,   215,   215,   234,   213,   209,   215,   216,   235,
     213,   208,   215,   216,   235,   685,   686,   687,   688,   689,
     690,    43,  1375,   234,  1377,    33,   208,    43,  1556,   235,
     234,    43,   234,   241,   235,  1388,   235,  1390,   241,  1392,
     235,   235,   209,   235,   235,   235,   213,  1575,   215,   216,
     235,   235,    60,    61,  1537,   234,   210,   234,   197,   235,
     730,    33,   234,   733,   734,   234,  1039,  1508,   197,   240,
     235,   239,   197,   189,   241,    10,    33,    13,     9,    42,
      66,  1499,   147,   214,  1760,   234,   151,   234,    60,    61,
     760,   233,   197,   234,  1435,   233,   197,  1438,   197,   197,
     240,     8,   197,    60,    61,   240,   139,   140,   141,   142,
     143,  1464,   240,   146,   147,   148,   149,   787,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   235,   165,   166,   234,   197,   215,   205,  1556,   147,
     197,   235,   197,   151,   209,   197,  1499,   234,   213,   197,
     215,   216,   197,    14,   208,   208,   826,  1575,   189,   210,
     234,   234,   234,   240,  1505,   235,   235,   208,   235,   235,
      21,    22,   235,   234,   844,   147,   241,   235,    43,   151,
     850,   214,   215,   216,   217,   218,   219,   220,   235,   235,
     147,   235,   235,    37,   151,   865,    67,   208,   231,   232,
     240,   209,   234,  1556,   234,   213,   235,   215,   216,   215,
     234,    70,   235,   234,  1697,   234,  1189,   234,   197,  1702,
     890,   891,  1575,    43,   234,   234,   197,   234,   197,   234,
     234,   901,   235,   241,    33,   234,   906,   209,   908,   235,
     910,   213,   234,   215,   216,   915,   234,   201,   235,   238,
     235,   235,   209,   235,   234,  1608,   213,   234,   215,   216,
     234,    60,    61,  1635,   234,   197,   234,   240,   235,   241,
     235,   235,    33,   235,  1757,  1758,   235,   235,   239,   235,
     235,    12,   235,   235,   241,   235,   235,   235,   139,   140,
     141,   142,   143,   235,   235,   146,   235,  1638,   149,   207,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    33,   165,   166,   238,   235,  1659,   989,
     207,   235,   235,   993,   235,   235,   235,    53,   240,   235,
     235,   233,  1815,   234,   238,   238,   235,   235,   235,  1822,
      60,    61,  1760,   239,   233,  1828,   240,   240,   147,  1019,
     234,   239,   151,   235,   240,    80,     1,   691,  1863,    83,
     797,  1861,   217,   214,   215,   216,   217,   218,   219,   220,
     797,  1854,  1043,   711,  1345,   707,     1,  1493,  1661,  1049,
     231,   232,  1664,  1356,  1357,  1358,  1359,  1360,  1361,  1362,
    1363,  1364,  1365,  1366,  1367,  1368,  1369,  1370,  1371,  1372,
    1373,   781,  1723,  1073,  1578,  1724,    33,  1760,    21,    22,
     209,  1724,    54,  1754,   213,  1243,   215,   216,  1042,    33,
    1042,   236,  1763,   360,   488,   662,    -1,   147,   488,   488,
      -1,   151,   488,    60,    61,    -1,    -1,    -1,   488,   488,
      -1,    -1,   241,    -1,    -1,    -1,    60,    61,    -1,  1119,
      -1,    -1,    -1,    -1,    -1,    -1,  1126,    -1,    -1,  1129,
    1801,    -1,    -1,    -1,    -1,  1135,    -1,  1137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1819,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
      -1,    -1,  1833,   213,  1835,   215,   216,    -1,  1168,  1840,
    1170,    -1,  1172,    -1,  1174,    -1,  1176,    -1,    -1,    -1,
    1180,    -1,  1182,    -1,  1184,    -1,  1857,    -1,  1859,    -1,
     147,   241,    -1,  1193,   151,    -1,    -1,    -1,   141,   142,
      -1,  1201,  1505,   147,    -1,    -1,   149,   151,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,    -1,    -1,  1225,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1235,    -1,    -1,    -1,    -1,
    1240,    -1,  1242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1251,   209,    -1,  1254,    -1,   213,    -1,   215,   216,
      -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,   213,
      -1,   215,   216,  1273,    -1,   218,   219,   220,    21,    22,
      -1,    -1,    -1,    -1,   241,    -1,    19,    33,   231,   232,
      -1,    -1,    25,    -1,    -1,    -1,  1296,   241,    31,    -1,
      -1,    -1,    -1,    -1,  1304,  1305,  1306,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    60,    61,    49,    -1,    -1,    -1,
      -1,    -1,    -1,  1323,    -1,    -1,  1326,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,  1335,    -1,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,    -1,  1395,    -1,  1397,   141,   142,
      -1,   147,    -1,    -1,  1404,   151,   149,    -1,    -1,   152,
     153,   154,   155,   156,   157,    -1,   159,   160,   161,   162,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    -1,    -1,  1444,    -1,    -1,    -1,    -1,    -1,
    1753,    -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1763,    -1,  1462,   209,   197,    -1,    -1,   213,    -1,   215,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1479,
    1480,    -1,    -1,    -1,    -1,    21,    22,    -1,   231,   232,
      -1,    -1,    -1,    -1,    -1,   241,    -1,    -1,    -1,    -1,
    1500,    -1,    -1,   236,    -1,    -1,  1506,  1507,   241,    -1,
      -1,  1814,  1512,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1840,    -1,    -1,
      -1,    -1,    -1,  1543,    -1,    -1,    -1,    -1,    -1,    -1,
    1853,    -1,    -1,    -1,    -1,    -1,  1859,    -1,    -1,    -1,
      -1,  1561,    -1,    -1,    -1,    -1,  1566,    -1,    -1,  1569,
      -1,  1571,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1593,  1594,  1595,  1596,  1597,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
     146,   147,   148,   149,  1614,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,   165,
     166,    -1,    -1,   169,  1634,    -1,    -1,   173,   174,   175,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1668,    -1,
      19,    -1,    -1,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,  1685,    -1,    -1,    -1,    -1,
    1690,  1691,    -1,    -1,    -1,   231,   232,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,  1748,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1785,     1,    -1,    -1,    -1,
       5,     6,     7,    -1,     9,    10,    11,    -1,    13,  1799,
      15,    16,    17,    18,    19,    -1,    -1,    -1,  1808,    -1,
      25,    26,    27,    28,    29,    -1,    31,    -1,  1818,    -1,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    -1,    44,
      45,    -1,    -1,    48,    -1,    50,    51,    52,   187,    54,
      55,    -1,  1842,    58,    59,    -1,    -1,    -1,   197,    -1,
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
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,   184,
      -1,   186,    -1,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,    -1,   199,   200,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,   224,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,   234,
      -1,   236,   237,   238,   239,   240,     1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,   184,
      -1,   186,    -1,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,    -1,   199,   200,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,   224,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,   234,
      -1,   236,   237,   238,   239,   240,     1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,   184,
      -1,   186,    -1,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,    -1,   199,   200,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,   224,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,   234,
      -1,   236,   237,   238,   239,   240,     1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,   184,
      -1,   186,    -1,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,    -1,   199,   200,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,   224,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,   234,
      -1,   236,   237,   238,   239,   240,     1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,   184,
      -1,   186,    -1,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,    -1,   199,   200,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,   224,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,   234,
      -1,   236,   237,   238,   239,   240,     1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,   184,
      -1,   186,    -1,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,    -1,   199,   200,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,   224,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,   234,
      -1,   236,   237,   238,   239,   240,     5,     6,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    16,    17,    18,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,    -1,
      -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,    -1,
      -1,     5,     6,   232,    -1,   234,   235,   236,   237,    13,
     239,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,    -1,
     234,    -1,   236,   237,    13,   239,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,    -1,   186,    -1,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,    -1,
      -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,    -1,
      -1,     5,     6,   232,    -1,   234,    -1,   236,   237,    -1,
     239,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,    -1,
     234,    -1,   236,   237,    -1,   239,    15,    16,    17,    18,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,    -1,
      -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,    -1,
      -1,     5,     6,   232,    -1,   234,   235,   236,   237,    -1,
     239,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,    -1,
     234,   235,   236,   237,    -1,   239,    15,    16,    17,    18,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,    -1,
      -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,    -1,
      -1,     5,     6,   232,    -1,   234,    -1,   236,   237,    -1,
     239,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,    -1,
     234,   235,   236,   237,    -1,   239,    15,    16,    17,    18,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,    -1,
      -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,    -1,
      -1,     5,     6,   232,    -1,   234,    -1,   236,   237,    -1,
     239,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,    -1,
     234,   235,   236,   237,    -1,   239,    15,    16,    17,    18,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,    -1,   186,    -1,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,    -1,
      -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,    -1,
      -1,     5,     6,   232,    -1,   234,   235,   236,   237,    -1,
     239,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,    -1,
     234,   235,   236,   237,    -1,   239,    15,    16,    17,    18,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,    -1,
      -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,    -1,
      -1,     5,     6,   232,    -1,   234,    -1,   236,   237,    -1,
     239,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,    -1,
     234,    -1,   236,   237,    -1,   239,    15,    16,    17,    18,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,    -1,   186,    -1,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,    -1,
      -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,    -1,
      -1,     5,     6,   232,   233,   234,    -1,   236,   237,    -1,
     239,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,   233,
     234,    -1,   236,   237,    -1,   239,    15,    16,    17,    18,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,    -1,   186,    -1,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,    -1,
      -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,    -1,
      -1,     5,     6,   232,    -1,   234,    -1,   236,   237,    13,
     239,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,    -1,
     234,    -1,   236,   237,    -1,   239,    15,    16,    17,    18,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,    -1,   186,    -1,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,    -1,
      -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,    -1,
      -1,     5,     6,   232,    -1,   234,    -1,   236,   237,    -1,
     239,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,    -1,
     234,    -1,   236,   237,    -1,   239,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    58,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,    -1,   186,    -1,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,    -1,
      -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,    -1,
      -1,     5,     6,   232,    -1,   234,    -1,   236,   237,    -1,
     239,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,    -1,
     234,    -1,   236,   237,    -1,   239,    15,    16,    17,    18,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,    -1,   186,    -1,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,    -1,
      -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,    -1,
      -1,     5,     6,   232,    -1,   234,   235,   236,   237,    -1,
     239,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,    -1,
     234,    -1,   236,   237,    -1,   239,    15,    16,    17,    18,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,    -1,   186,    -1,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,    -1,
      -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,    -1,
      -1,     5,     6,   232,    -1,   234,    -1,   236,   237,    -1,
     239,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    52,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,     5,     6,    -1,    -1,   232,    -1,
     234,    -1,   236,   237,    15,    16,    17,    18,    19,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
     181,   182,   183,   184,    -1,   186,    -1,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,    -1,    -1,   200,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,   218,    -1,   220,
      -1,    -1,   223,   224,    -1,    -1,    -1,     5,     6,    -1,
      -1,   232,    -1,   234,    -1,   236,   237,    15,    16,    17,
      18,    19,    -1,    -1,    22,    -1,    -1,    25,    -1,    27,
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
      -1,    -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,   183,   184,    -1,   186,    -1,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
      -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,    -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,
       5,     6,    -1,    -1,   232,    -1,   234,    -1,   236,   237,
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
     125,   126,   127,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,   141,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,   184,
      -1,   186,    -1,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,    -1,    -1,   200,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,   224,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,   234,
      -1,   236,   237,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,    -1,    -1,   173,   174,   175,   176,
     177,   178,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   208,   209,    -1,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,   231,   232,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,   197,    -1,   199,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,   236,    -1,   238,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
      19,    -1,    -1,    21,    22,    -1,    25,    -1,    -1,    -1,
      -1,   197,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
     236,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,    21,    22,
      -1,    -1,    -1,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,   165,   166,   167,
     168,    -1,    -1,   172,    -1,   173,   174,   175,   176,   177,
     178,   179,    -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,    -1,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,    21,    22,   223,   224,    -1,    -1,    -1,
      -1,    -1,    -1,   231,   232,    -1,    -1,   236,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,   165,   166,    -1,    -1,   169,    -1,    -1,    -1,
     173,   174,   175,    -1,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,   209,    -1,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,   232,
      -1,   139,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,   165,   166,    -1,
      -1,   169,    -1,    -1,    -1,   173,   174,   175,    -1,    -1,
      -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,    -1,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,   231,   232,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,   173,   174,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
      -1,    -1,    21,    22,    -1,   139,   140,   141,   142,   143,
     231,   232,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,    -1,    -1,    21,
      22,    -1,   139,   140,   141,   142,   143,   231,   232,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,   165,   166,
     139,   140,   141,   142,   143,    -1,   173,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,   231,   232,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,   231,   232,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     212,   213,   214,   215,   216,   217,   218,   219,   220,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,   231,
     232,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,   214,
     215,   216,   217,   218,   219,   220,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   231,   232,    71,    72,
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
      -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
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
      -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    35,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    -1,    -1,    -1,    71,
      -1,    73,    -1,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    -1,    -1,    -1,    -1,    -1,   187,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   141,
     142,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   197,    -1,    71,    -1,    73,
      -1,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,   152,   153,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   231,   232,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,    76,
      77,    78,    79,   197,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    71,    -1,
      73,    -1,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   243,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   125,   126,   199,   220,   232,   238,   245,   246,   250,
     259,   261,   262,   267,   316,   322,   353,   443,   451,   459,
     469,   519,   524,   529,    19,    20,    69,   197,   306,   307,
     308,   189,   268,   269,   209,   264,   265,    57,    63,   448,
     449,   452,   197,   236,   248,   530,   520,   525,   172,   197,
     341,    34,    63,   130,   163,   224,   234,   311,   312,   313,
     314,   341,   245,   245,   245,     8,    36,   470,    62,   439,
     208,   207,   210,   207,   179,   197,   220,   231,   263,   232,
     263,   197,   245,   245,   439,   448,   448,   448,   197,   172,
     260,   313,   313,   313,   234,   173,   174,   175,   207,   233,
     130,   321,   460,     5,     6,   466,    57,    63,   440,    15,
      16,   170,   189,   194,   197,   200,   234,   252,   307,   189,
     269,   219,   219,   263,   219,   219,   231,   197,    22,    57,
     266,   197,   450,    57,    63,   247,   197,   197,   197,   197,
     201,   258,   235,   308,   313,   313,   313,   313,   199,   273,
     274,    57,    63,   323,   325,    57,    63,   453,   130,   130,
      57,    63,   467,   239,   444,   197,   201,   202,   203,   251,
      15,    16,   189,   194,   197,   252,   304,   305,   263,   263,
     263,   197,   197,   233,   197,   208,   208,   224,   249,   210,
     480,   274,   274,   201,   235,   199,   326,   197,   454,   471,
     441,   171,   309,   409,   201,   202,   203,   207,   235,   266,
     266,    19,    25,    31,    41,    49,    64,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   185,   236,   341,   474,   476,   477,   481,
     487,   489,   518,   518,    66,    79,    94,    99,   101,   198,
     457,   458,   521,   526,    35,    71,    73,    75,    76,    77,
      78,    79,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    93,    94,    95,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   141,   142,   197,   319,   320,   324,   210,
     455,   129,   464,   465,   240,   245,   442,   307,   171,   197,
     435,   438,   304,   214,   214,   214,   234,   214,   214,   234,
     480,   214,   214,   214,   214,   214,   234,   341,   214,   234,
      33,    60,    61,   147,   151,   209,   213,   216,   241,   232,
     486,   211,   198,   531,   239,   239,    21,    22,    38,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   151,   152,   153,
     164,   165,   166,   167,   168,   173,   174,   175,   176,   177,
     178,   179,   209,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   223,   224,   231,   232,    35,    35,   234,
     317,   274,    75,    79,    93,    94,    98,    99,   100,   101,
     475,   458,   197,   274,   409,   274,   307,   207,   210,   213,
     433,   490,   496,   498,     5,     6,    15,    16,    17,    18,
      19,    25,    27,    31,    39,    45,    48,    51,    55,    65,
      68,    69,    80,   125,   126,   127,   141,   142,   170,   180,
     181,   182,   183,   184,   186,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   200,   216,   217,   218,   223,   224,
     232,   234,   236,   237,   239,   257,   259,   333,   341,   346,
     358,   365,   368,   371,   375,   377,   379,   380,   382,   387,
     390,   391,   392,   407,   408,   474,   535,   543,   554,   557,
     570,   571,   574,   575,   500,   494,   197,   214,   502,   504,
     506,   508,   510,   512,   514,   516,   391,   214,   234,   488,
     492,   150,   338,   369,   391,    33,   213,    33,   213,   232,
     241,   233,   391,   232,   241,   487,   239,   522,   527,   197,
     320,   197,   320,   197,   233,    22,   197,   233,   184,   235,
     409,   419,   420,   421,   149,   210,   318,   169,   239,   330,
     370,   239,   210,   463,   472,   181,   197,   434,   437,   274,
     197,   487,   150,   164,   208,   432,   518,   518,   485,   518,
     214,   214,   214,   341,   343,   476,   534,   543,   554,   557,
     570,   571,   574,   575,   341,   214,     5,   125,   126,   214,
     234,   214,   234,   234,   214,   214,   214,   234,   214,   234,
     214,   234,   214,   214,   234,    19,   214,   214,   392,   392,
     181,   186,   214,   341,   381,   234,   234,   234,   234,   234,
     234,   256,   392,   392,   392,   392,   392,    13,    49,   338,
     187,   197,   369,   536,   538,   569,   234,   232,   315,   170,
     239,   371,   376,   376,   376,   376,   235,    21,    22,   139,
     140,   141,   142,   143,   146,   147,   148,   149,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   165,   166,   169,   173,   174,   175,   179,   209,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   231,
     232,   234,   518,   518,   235,   215,   482,   518,   317,   518,
     317,   518,   317,   197,   422,   423,   518,   197,   425,   426,
     235,   493,   369,   340,   518,   391,   235,   207,   573,   233,
     233,   233,   391,   532,   422,   424,   425,   427,   197,   320,
     131,   132,   133,   134,   135,   136,   137,   138,   164,   176,
     177,   178,   131,   132,   133,   134,   135,   136,   137,   138,
     150,   164,   176,   177,   178,   208,   234,     7,    50,   352,
     238,   207,   238,   235,   518,   518,   150,   392,   331,   461,
     341,   238,   239,   468,   234,    43,   207,   210,   433,   245,
     432,   391,   215,   215,   215,   198,   207,   244,   245,   484,
     544,   546,   344,   234,   214,   234,   366,   214,   214,   214,
     564,   369,   487,   391,   568,   391,   359,   361,   393,   391,
     363,   391,   566,   369,   552,   555,   369,   214,   548,   487,
     234,   234,   383,   385,   391,   391,   391,   391,   391,   391,
     203,   204,   251,   234,    13,   233,   234,   150,   164,   208,
     428,   573,   207,   573,   235,   274,    70,   232,   235,   369,
     538,   314,     4,   374,   337,   315,    19,   187,   197,   474,
      19,   187,   197,   474,   392,   392,   392,   392,   392,   392,
     197,   392,   187,   197,   391,   187,   197,   187,   197,   391,
     391,   392,    19,   187,   197,   474,    19,   187,   197,   474,
      19,   187,   197,   474,   392,   392,   474,   392,   392,   392,
     570,   575,   392,   392,   392,   392,    22,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   152,   153,
     187,   197,   231,   232,   388,   474,   391,   235,   369,   215,
     215,   197,   478,   215,   318,   215,   318,   215,   318,   210,
     215,   484,   210,   215,   484,   340,   573,   215,   484,   150,
     391,   233,   197,   479,   245,   281,   282,   281,   282,   391,
     181,   197,   429,   430,   473,   421,   421,   421,   392,   337,
     197,   445,   447,   415,   391,   197,   181,   197,   487,   432,
     391,   245,   491,   497,   499,   518,   487,   487,   518,    70,
     369,   538,   542,   197,   391,   518,   558,   560,   562,   487,
     573,   215,   484,   207,   573,   235,   487,   487,   487,   235,
     487,   235,   487,   573,   487,   423,   573,   550,   426,   215,
     391,   391,   487,   317,   235,   235,   235,   235,   235,   235,
     391,   181,   197,   234,   295,   234,   391,   391,   391,   235,
     187,   197,    13,   339,   568,   197,   235,   538,   536,   207,
     235,   235,   233,   234,   317,     1,    26,    28,    29,    38,
      40,    44,    52,    54,    58,    59,    65,   128,   239,   240,
     245,   270,   271,   280,   291,   292,   294,   296,   297,   298,
     299,   300,   301,   302,   303,   334,   342,   347,   348,   349,
     350,   351,   353,   357,   378,   392,   374,   214,   234,   214,
     234,   234,   234,   233,   234,   234,   234,   233,   233,   214,
     234,   214,   234,   214,   234,    19,   187,   197,   474,   210,
     187,   197,   391,   234,   234,   187,   197,   391,     1,   234,
     233,   207,   235,   501,   495,   207,   215,   238,   503,   215,
     507,   215,   511,   215,   518,   515,   422,   518,   517,   425,
     215,   235,   488,   518,   391,   208,   244,   446,   456,   245,
     422,   523,   425,   528,   235,   234,    43,   207,   210,   213,
     428,   332,   208,   446,   456,    40,   199,   240,   316,   416,
     235,   234,    43,   245,   432,   391,   245,   215,   215,   215,
     538,   235,   235,   235,   215,   484,   235,   215,   487,   423,
     426,   215,   235,   234,   487,   391,   235,   215,   215,   215,
     215,   215,   235,   215,   215,   235,   487,   215,   374,   235,
     235,   215,   318,   234,   210,   254,   234,    43,   197,   355,
      20,   207,   295,   235,   234,   164,   428,   234,   240,   573,
     235,   207,   233,   232,   536,   150,   164,   197,   208,   213,
     372,   373,   318,   150,   391,   330,    61,   391,   197,   197,
     245,   189,    58,   391,   274,   150,   391,   335,   245,   245,
      10,    10,    11,   278,    13,     9,    42,   245,   245,   245,
     245,   245,   245,    66,   354,   245,   131,   132,   133,   134,
     135,   136,   137,   138,   144,   145,   150,   164,   167,   168,
     176,   177,   178,   208,   317,   395,   391,   399,   391,   232,
     235,   369,   536,   391,   391,   235,   369,   391,   397,   391,
     401,   391,   405,   391,   214,   234,   392,   234,   233,   391,
     232,   235,   369,   536,   234,   233,   389,   235,   369,   197,
     483,   197,   505,   509,   513,   488,   391,   197,   244,   533,
     240,   240,   391,   197,   181,   197,   518,   391,   240,   391,
     445,   462,   197,     8,   409,   414,   391,   197,   391,   245,
     545,   547,   345,   235,   234,   197,   367,   215,   215,   215,
     565,   339,   215,   360,   362,   394,   364,   567,   553,   556,
     215,   549,   234,   274,   384,   215,   235,   369,   255,   205,
     391,   197,   207,   235,   369,   181,   197,   234,    20,   164,
     428,   391,   391,   391,   295,   235,   536,   235,   197,   197,
     234,   197,   197,   207,   235,   274,   391,    14,   391,   208,
     208,   210,   189,   330,   391,   337,   234,   234,   232,   309,
     310,   310,   234,   234,   239,   356,   409,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   570,   575,
     392,   392,   392,   392,   392,   392,   392,   318,   487,   235,
     518,   235,   536,   207,   235,   235,   235,   235,   235,   235,
     487,   235,   518,   235,   518,   235,   403,   391,   391,   235,
     536,   207,   235,   235,   391,   235,   208,   240,   235,   234,
      43,   428,    37,   327,   240,   208,    57,    63,   412,    67,
     413,   245,   235,   245,   234,   234,   391,   215,   559,   561,
     563,   234,   235,   234,   234,   234,   234,   234,   234,    70,
     542,   234,   551,   234,   235,   391,   330,   381,   386,   235,
     253,   235,   197,   235,   234,    43,   355,   369,   391,   391,
     235,    20,   233,   197,   372,   370,   330,   518,   391,   336,
     391,   391,   308,   234,   234,    56,   391,   355,   436,   437,
     274,   215,   215,   233,   536,   215,   215,   215,   518,   235,
     235,   233,   536,   235,   391,   391,   197,   391,   328,   518,
      47,   413,    46,   129,   410,   542,   542,   235,   234,   234,
     234,   234,   338,   339,   391,   391,   391,   391,   369,   542,
     234,   542,   235,   381,   201,   238,   391,   197,   235,   235,
     164,   428,   369,   235,   235,   240,   235,   235,   233,   295,
     391,   263,   235,   235,   240,   245,   437,   370,   396,   400,
     235,   235,   398,   402,   406,   215,   235,   235,   235,   239,
     245,    33,   411,   410,   412,   234,   536,   539,   540,   541,
     541,   391,   542,   542,   536,   537,   235,   235,   235,   235,
     235,   235,   573,   541,   542,   537,   391,   235,   238,   391,
     391,   235,   327,    12,   279,   274,    20,   235,   235,   274,
     210,   433,   404,   337,   417,   411,   429,   430,   431,   536,
     207,   573,   235,   235,   235,   541,   541,   235,   235,   235,
     537,   235,   238,   572,   391,   238,   280,   347,   348,   349,
     350,   392,   245,   293,   369,   274,   274,   330,   487,   432,
     329,   324,   418,   235,   234,   235,   235,   235,    53,   233,
     572,   391,   239,   283,   286,   235,   330,   330,   432,   391,
     240,   245,   324,   536,   391,   233,   572,   284,    12,    23,
      24,   272,   275,   280,   274,   391,   245,   274,   235,   240,
     337,   274,   234,   245,   330,   245,   370,   285,   276,   391,
     240,   239,   287,   290,   235,   327,   288,   280,   274,   337,
     245,   277,   289,   287,   240,   275,   327
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   242,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   244,   244,   245,
     245,   246,   247,   247,   247,   248,   248,   249,   249,   250,
     251,   251,   251,   251,   252,   252,   253,   253,   254,   255,
     254,   256,   256,   256,   257,   258,   258,   260,   259,   261,
     262,   263,   263,   263,   263,   263,   263,   263,   264,   264,
     265,   265,   265,   266,   266,   267,   268,   268,   269,   269,
     270,   271,   271,   272,   272,   273,   273,   274,   274,   275,
     276,   275,   277,   275,   278,   278,   279,   279,   280,   280,
     280,   280,   280,   281,   281,   282,   282,   284,   285,   283,
     286,   283,   288,   289,   287,   290,   287,   292,   293,   291,
     294,   295,   295,   295,   295,   295,   295,   295,   295,   297,
     296,   298,   300,   299,   301,   302,   302,   303,   303,   304,
     304,   304,   304,   304,   304,   305,   305,   306,   306,   306,
     306,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     308,   308,   309,   309,   310,   310,   310,   311,   311,   311,
     311,   312,   312,   313,   313,   313,   313,   313,   313,   313,
     314,   314,   315,   315,   316,   316,   317,   317,   317,   318,
     318,   318,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   321,
     321,   322,   323,   323,   323,   324,   326,   325,   327,   328,
     329,   327,   331,   332,   330,   333,   333,   333,   333,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   335,   336,
     334,   337,   337,   337,   338,   338,   339,   339,   340,   340,
     341,   341,   341,   342,   342,   344,   345,   343,   343,   346,
     346,   346,   346,   346,   346,   347,   348,   349,   349,   349,
     350,   350,   351,   352,   352,   352,   353,   353,   354,   354,
     355,   355,   356,   356,   357,   357,   357,   359,   360,   358,
     361,   362,   358,   363,   364,   358,   366,   367,   365,   368,
     368,   368,   369,   369,   369,   369,   370,   370,   370,   371,
     371,   371,   372,   372,   372,   372,   372,   373,   373,   374,
     374,   375,   376,   376,   377,   377,   377,   377,   377,   377,
     377,   377,   378,   378,   378,   378,   378,   378,   378,   378,
     378,   378,   378,   378,   378,   378,   378,   378,   378,   378,
     378,   378,   378,   379,   379,   379,   380,   380,   380,   380,
     380,   381,   381,   382,   383,   384,   382,   385,   386,   382,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   388,   389,   387,   390,   390,   390,   390,   390,
     390,   390,   391,   391,   391,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   393,   394,   392,   392,   392,   392,   392,   395,   396,
     392,   392,   392,   397,   398,   392,   392,   392,   392,   399,
     400,   392,   392,   392,   401,   402,   392,   392,   392,   403,
     404,   392,   392,   392,   405,   406,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   407,   407,   407,   408,   408,   408,   408,
     408,   408,   408,   408,   408,   408,   408,   408,   408,   408,
     408,   408,   408,   408,   408,   408,   408,   409,   409,   410,
     410,   410,   411,   411,   412,   412,   412,   413,   413,   414,
     415,   415,   415,   416,   415,   417,   415,   418,   415,   419,
     420,   420,   421,   421,   421,   421,   421,   422,   422,   423,
     423,   424,   424,   424,   425,   426,   426,   427,   427,   427,
     428,   428,   429,   429,   429,   430,   430,   431,   431,   432,
     432,   432,   433,   433,   434,   434,   434,   434,   434,   434,
     435,   435,   436,   436,   436,   437,   437,   437,   438,   438,
     438,   439,   439,   440,   440,   440,   441,   441,   442,   441,
     443,   444,   443,   445,   445,   446,   446,   447,   447,   447,
     448,   448,   448,   450,   449,   451,   451,   452,   453,   453,
     453,   454,   455,   455,   456,   456,   457,   457,   458,   458,
     460,   461,   462,   459,   463,   463,   464,   464,   465,   466,
     466,   466,   466,   467,   467,   467,   468,   468,   470,   471,
     472,   469,   473,   473,   473,   473,   473,   473,   474,   474,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   474,   474,   475,   475,
     475,   475,   475,   475,   475,   475,   476,   477,   477,   477,
     478,   478,   478,   479,   479,   479,   479,   479,   480,   480,
     480,   480,   480,   481,   482,   483,   481,   484,   484,   485,
     485,   486,   486,   486,   486,   487,   487,   488,   488,   489,
     489,   489,   489,   490,   491,   489,   489,   489,   489,   492,
     489,   493,   489,   489,   489,   489,   489,   489,   489,   489,
     489,   489,   489,   489,   489,   494,   495,   489,   489,   496,
     497,   489,   498,   499,   489,   500,   501,   489,   489,   502,
     503,   489,   504,   505,   489,   489,   506,   507,   489,   508,
     509,   489,   489,   510,   511,   489,   512,   513,   489,   514,
     515,   489,   516,   517,   489,   518,   518,   518,   520,   521,
     522,   523,   519,   525,   526,   527,   528,   524,   530,   531,
     532,   533,   529,   534,   534,   534,   534,   534,   534,   534,
     535,   535,   535,   535,   535,   536,   536,   536,   536,   536,
     536,   536,   536,   537,   537,   538,   539,   539,   540,   540,
     541,   541,   542,   542,   544,   545,   543,   546,   547,   543,
     548,   549,   543,   550,   551,   543,   552,   553,   543,   554,
     555,   556,   554,   557,   558,   559,   557,   560,   561,   557,
     562,   563,   557,   557,   564,   565,   557,   557,   566,   567,
     557,   568,   568,   569,   570,   571,   571,   571,   572,   572,
     573,   573,   574,   574,   575
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
       1,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     3,     3,     4,     3,     3,     3,     3,     1,     5,
       1,     3,     3,     4,     0,     3,     1,     1,     1,     1,
       1,     1,     4,     1,     2,     3,     3,     3,     3,     2,
       1,     3,     0,     3,     0,     4,     0,     2,     3,     0,
       2,     2,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     3,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     3,     2,     2,     3,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     3,     2,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     4,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     4,     0,     1,     1,     3,     0,     5,     0,     0,
       0,     6,     0,     0,     6,     2,     2,     2,     2,     1,
       2,     2,     1,     1,     1,     1,     2,     1,     2,     2,
       2,     2,     1,     1,     1,     2,     2,     2,     0,     0,
       6,     0,     2,     2,     0,     2,     0,     2,     1,     3,
       1,     3,     2,     2,     3,     0,     0,     5,     1,     2,
       5,     5,     5,     6,     2,     1,     1,     1,     2,     3,
       2,     3,     4,     1,     1,     0,     1,     1,     1,     0,
       1,     3,     8,     7,     4,     3,     5,     0,     0,     9,
       0,     0,     9,     0,     0,     9,     0,     0,     6,     5,
       8,    10,     1,     2,     3,     4,     1,     2,     3,     1,
       1,     1,     2,     2,     2,     2,     4,     1,     3,     0,
       4,     7,     7,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
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
       0,     1,     1,     0,     5,     3,     3,     5,     0,     1,
       1,     1,     0,     2,     0,     1,     1,     2,     0,     1,
       0,     0,     0,    13,     0,     2,     0,     1,     3,     1,
       1,     2,     2,     0,     1,     1,     1,     3,     0,     0,
       0,     9,     1,     4,     3,     3,     6,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       1,     3,     3,     0,     1,     3,     3,     5,     0,     2,
       2,     2,     2,     4,     0,     0,     7,     1,     1,     1,
       3,     3,     2,     4,     3,     1,     2,     0,     4,     1,
       1,     1,     1,     0,     0,     6,     4,     4,     3,     0,
       6,     0,     7,     4,     2,     2,     3,     2,     3,     2,
       2,     3,     3,     3,     2,     0,     0,     6,     2,     0,
       0,     6,     0,     0,     6,     0,     0,     6,     1,     0,
       0,     6,     0,     0,     7,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     0,
       0,     6,     0,     0,     6,     1,     3,     3,     0,     0,
       0,     0,    12,     0,     0,     0,     0,    12,     0,     0,
       0,     0,    13,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     5,     5,     6,
       6,     8,     8,     0,     1,     2,     3,     5,     1,     2,
       1,     0,     0,     1,     0,     0,    10,     0,     0,    10,
       0,     0,    10,     0,     0,    11,     0,     0,     7,     5,
       0,     0,    10,     3,     0,     0,    11,     0,     0,    11,
       0,     0,    10,     5,     0,     0,     9,     5,     0,     0,
      10,     1,     3,     0,     5,     5,     7,     9,     0,     3,
       0,     1,    11,    12,    13
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

  case 132: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 133: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 134: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 135: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 136: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 137: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 138: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 139: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 140: /* annotation_argument_name: "default"  */
                    { (yyval.s) = new string("default"); }
    break;

  case 141: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 142: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 143: /* annotation_argument: annotation_argument_name '=' "@@" "name"  */
                                                                      { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[0].s); delete (yyvsp[-3].s); }
    break;

  case 144: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 145: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 146: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 147: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 148: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 149: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 150: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 151: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 152: /* metadata_argument_list: "@field" annotation_argument optional_emit_semis  */
                                                              {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[-1].aa));
    }
    break;

  case 153: /* metadata_argument_list: metadata_argument_list "@field" annotation_argument optional_emit_semis  */
                                                                                           {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-3].aaList),(yyvsp[-1].aa));
    }
    break;

  case 154: /* optional_for_annotations: %empty  */
                    {
        (yyval.aaList) = nullptr;
    }
    break;

  case 155: /* optional_for_annotations: '[' annotation_argument_list ']'  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 156: /* optional_for_annotations: metadata_argument_list  */
                                     {
        (yyval.aaList) = (yyvsp[0].aaList);
    }
    break;

  case 157: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 158: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 159: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 160: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 161: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 162: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 163: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 164: /* annotation_declaration: '!' annotation_declaration  */
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

  case 165: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 166: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 167: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 168: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 169: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 170: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 171: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 172: /* optional_annotation_list: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 173: /* optional_annotation_list: '[' annotation_list ']'  */
                                       { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 174: /* optional_annotation_list_with_emit_semis: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 175: /* optional_annotation_list_with_emit_semis: '[' annotation_list ']' optional_emit_semis  */
                                                          { (yyval.faList) = (yyvsp[-2].faList); }
    break;

  case 176: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 177: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 178: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 179: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yyloc));
    }
    break;

  case 180: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 181: /* optional_function_type: "->" type_declaration  */
                                           {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 182: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 183: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 184: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 185: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 186: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 187: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 188: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 189: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 190: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 191: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 192: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 193: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 194: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 195: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 196: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 197: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 198: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 199: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 200: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 201: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 202: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 203: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 204: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 205: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 206: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 207: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 208: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 209: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 210: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 211: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 212: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 213: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 214: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 215: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 216: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 217: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 218: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 219: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 220: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 221: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 222: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 223: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 224: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 225: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 226: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 227: /* function_name: "operator" '[' ']' '='  */
                                 { (yyval.s) = new string("[]="); }
    break;

  case 228: /* function_name: "operator" '[' ']' "<-"  */
                                    { (yyval.s) = new string("[]<-"); }
    break;

  case 229: /* function_name: "operator" '[' ']' ":="  */
                                      { (yyval.s) = new string("[]:="); }
    break;

  case 230: /* function_name: "operator" '[' ']' "+="  */
                                     { (yyval.s) = new string("[]+="); }
    break;

  case 231: /* function_name: "operator" '[' ']' "-="  */
                                     { (yyval.s) = new string("[]-="); }
    break;

  case 232: /* function_name: "operator" '[' ']' "*="  */
                                     { (yyval.s) = new string("[]*="); }
    break;

  case 233: /* function_name: "operator" '[' ']' "/="  */
                                     { (yyval.s) = new string("[]/="); }
    break;

  case 234: /* function_name: "operator" '[' ']' "%="  */
                                     { (yyval.s) = new string("[]%="); }
    break;

  case 235: /* function_name: "operator" '[' ']' "&="  */
                                     { (yyval.s) = new string("[]&="); }
    break;

  case 236: /* function_name: "operator" '[' ']' "|="  */
                                     { (yyval.s) = new string("[]|="); }
    break;

  case 237: /* function_name: "operator" '[' ']' "^="  */
                                     { (yyval.s) = new string("[]^="); }
    break;

  case 238: /* function_name: "operator" '[' ']' "&&="  */
                                        { (yyval.s) = new string("[]&&="); }
    break;

  case 239: /* function_name: "operator" '[' ']' "||="  */
                                        { (yyval.s) = new string("[]||="); }
    break;

  case 240: /* function_name: "operator" '[' ']' "^^="  */
                                        { (yyval.s) = new string("[]^^="); }
    break;

  case 241: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 242: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 243: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 244: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 245: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 246: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 247: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 248: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 249: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 250: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 251: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 252: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 253: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 254: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 255: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 256: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 257: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 258: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 259: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 260: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 261: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 262: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 263: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 264: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 265: /* function_name: "operator" "is" das_type_name  */
                                                { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 266: /* function_name: "operator" "as" das_type_name  */
                                                { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 267: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 268: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 269: /* function_name: "operator" '?' "as" das_type_name  */
                                                    { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 270: /* function_name: das_type_name  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 271: /* das_type_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 272: /* das_type_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 273: /* das_type_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 274: /* das_type_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 275: /* das_type_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 276: /* das_type_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 277: /* das_type_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 278: /* das_type_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 279: /* das_type_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 280: /* das_type_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 281: /* das_type_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 282: /* das_type_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 283: /* das_type_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 284: /* das_type_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 285: /* das_type_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 286: /* das_type_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 287: /* das_type_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 288: /* das_type_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 289: /* das_type_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 290: /* das_type_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 291: /* das_type_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 292: /* das_type_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 293: /* das_type_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 294: /* das_type_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 295: /* das_type_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 296: /* das_type_name: "float16"  */
                     { (yyval.s) = new string("float16"); }
    break;

  case 297: /* das_type_name: "half2"  */
                     { (yyval.s) = new string("half2"); }
    break;

  case 298: /* das_type_name: "half3"  */
                     { (yyval.s) = new string("half3"); }
    break;

  case 299: /* das_type_name: "half4"  */
                     { (yyval.s) = new string("half4"); }
    break;

  case 300: /* das_type_name: "half8"  */
                     { (yyval.s) = new string("half8"); }
    break;

  case 301: /* das_type_name: "short2"  */
                     { (yyval.s) = new string("short2"); }
    break;

  case 302: /* das_type_name: "short3"  */
                     { (yyval.s) = new string("short3"); }
    break;

  case 303: /* das_type_name: "short4"  */
                     { (yyval.s) = new string("short4"); }
    break;

  case 304: /* das_type_name: "short8"  */
                     { (yyval.s) = new string("short8"); }
    break;

  case 305: /* das_type_name: "ushort2"  */
                     { (yyval.s) = new string("ushort2"); }
    break;

  case 306: /* das_type_name: "ushort3"  */
                     { (yyval.s) = new string("ushort3"); }
    break;

  case 307: /* das_type_name: "ushort4"  */
                     { (yyval.s) = new string("ushort4"); }
    break;

  case 308: /* das_type_name: "ushort8"  */
                     { (yyval.s) = new string("ushort8"); }
    break;

  case 309: /* das_type_name: "byte2"  */
                     { (yyval.s) = new string("byte2"); }
    break;

  case 310: /* das_type_name: "byte3"  */
                     { (yyval.s) = new string("byte3"); }
    break;

  case 311: /* das_type_name: "byte4"  */
                     { (yyval.s) = new string("byte4"); }
    break;

  case 312: /* das_type_name: "byte8"  */
                     { (yyval.s) = new string("byte8"); }
    break;

  case 313: /* das_type_name: "byte16"  */
                     { (yyval.s) = new string("byte16"); }
    break;

  case 314: /* das_type_name: "ubyte2"  */
                     { (yyval.s) = new string("ubyte2"); }
    break;

  case 315: /* das_type_name: "ubyte3"  */
                     { (yyval.s) = new string("ubyte3"); }
    break;

  case 316: /* das_type_name: "ubyte4"  */
                     { (yyval.s) = new string("ubyte4"); }
    break;

  case 317: /* das_type_name: "ubyte8"  */
                     { (yyval.s) = new string("ubyte8"); }
    break;

  case 318: /* das_type_name: "ubyte16"  */
                     { (yyval.s) = new string("ubyte16"); }
    break;

  case 319: /* optional_template: %empty  */
                                        { (yyval.b) = false; }
    break;

  case 320: /* optional_template: "template"  */
                                        { (yyval.b) = true; }
    break;

  case 321: /* global_function_declaration: optional_annotation_list_with_emit_semis "def" optional_template function_declaration  */
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

  case 322: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 323: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 324: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 325: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 326: /* $@15: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 327: /* function_declaration: optional_public_or_private_function $@15 function_declaration_header optional_emit_semis block_or_simple_block  */
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

  case 328: /* expression_block_finally: %empty  */
        {
        (yyval.pExpression) = nullptr;
    }
    break;

  case 329: /* $@16: %empty  */
                  {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 330: /* $@17: %empty  */
                             {
        yyextra->pop_nesteds();
    }
    break;

  case 331: /* expression_block_finally: "finally" $@16 '{' expressions $@17 '}'  */
          {
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 332: /* $@18: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 333: /* $@19: %empty  */
                         {
        yyextra->pop_nesteds();
    }
    break;

  case 334: /* expression_block: '{' $@18 expressions $@19 '}' expression_block_finally  */
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

  case 335: /* expr_call_pipe_no_bracket: expr_call expr_full_block_assumed_piped  */
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

  case 336: /* expr_call_pipe_no_bracket: expr_method_call_no_bracket expr_full_block_assumed_piped  */
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

  case 337: /* expr_call_pipe_no_bracket: expr_field_no_bracket expr_full_block_assumed_piped  */
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

  case 338: /* expr_call_pipe_no_bracket: expr_named_call expr_full_block_assumed_piped  */
                                                                 {
        // free-function named call + piped block: pad-aware resolution lands it on the block param
        auto nc = (ExprNamedCall *) (yyvsp[-1].pExpression);
        nc->nonNamedArguments.push_back((yyvsp[0].pExpression));
        nc->pipedCallArgument = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 339: /* expression_any: SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 340: /* expression_any: expr_assign_no_bracket SEMICOLON  */
                                                    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 341: /* expression_any: expression_delete SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 342: /* expression_any: expression_let  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 343: /* expression_any: expression_while_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 344: /* expression_any: expression_unsafe  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 345: /* expression_any: expression_with  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 346: /* expression_any: expression_with_alias SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 347: /* expression_any: expression_for_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 348: /* expression_any: expression_break SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 349: /* expression_any: expression_continue SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 350: /* expression_any: expression_return SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 351: /* expression_any: expression_yield SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 352: /* expression_any: expression_if_then_else  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 353: /* expression_any: expression_if_then_else_oneliner  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 354: /* expression_any: expression_try_catch  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 355: /* expression_any: expression_label SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 356: /* expression_any: expression_goto SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 357: /* expression_any: "pass" SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 358: /* $@20: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 359: /* $@21: %empty  */
                         {
        yyextra->pop_nesteds();
    }
    break;

  case 360: /* expression_any: '{' $@20 expressions $@21 '}' expression_block_finally  */
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

  case 361: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 362: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 363: /* expressions: expressions error  */
                                 {
        (void)(yyvsp[-1].pExpression); /* gc_node — don't delete Expression */ (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 364: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 365: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 366: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 367: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 368: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 369: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 370: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 371: /* name_in_namespace: "name" "::" "name"  */
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

  case 372: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 373: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 374: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 375: /* $@22: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 376: /* $@23: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 377: /* new_type_declaration: '<' $@22 type_declaration '>' $@23  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 378: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 379: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 380: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 381: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 382: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 383: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 384: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 385: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 386: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 387: /* expression_return: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 388: /* expression_return: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 389: /* expression_return: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 390: /* expression_yield: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 391: /* expression_yield: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 392: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 393: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 394: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 395: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 396: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 397: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 398: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 399: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 400: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 401: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 402: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 403: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr SEMICOLON  */
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

  case 404: /* expression_let: kwd_let optional_in_scope optional_field_annotation let_variable_declaration  */
                                                                                                {
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-3].b),(yyvsp[-2].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 405: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 406: /* expression_let: kwd_let optional_in_scope '{' variable_declaration_list '}'  */
                                                                               {
        (yyval.pExpression) = ast_LetList(scanner,(yyvsp[-4].b),(yyvsp[-3].b),*(yyvsp[-1].pVarDeclList),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 407: /* $@24: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 408: /* $@25: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 409: /* expr_cast: "cast" '<' $@24 type_declaration_no_options '>' $@25 '(' expr ')'  */
                                                                                                                                                        {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-8])),(yyvsp[-1].pExpression),(yyvsp[-5].pTypeDecl));
    }
    break;

  case 410: /* $@26: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 411: /* $@27: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 412: /* expr_cast: "upcast" '<' $@26 type_declaration_no_options '>' $@27 '(' expr ')'  */
                                                                                                                                                          {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-8])),(yyvsp[-1].pExpression),(yyvsp[-5].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 413: /* $@28: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 414: /* $@29: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 415: /* expr_cast: "reinterpret" '<' $@28 type_declaration_no_options '>' $@29 '(' expr ')'  */
                                                                                                                                                               {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-8])),(yyvsp[-1].pExpression),(yyvsp[-5].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 416: /* $@30: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 417: /* $@31: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 418: /* expr_type_decl: "type" '<' $@30 type_declaration '>' $@31  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 419: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 420: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 421: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" c_or_s "name" '>' '(' expr ')'  */
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

  case 422: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 423: /* expr_list: "<-" expr  */
                             {
            (yyval.pExpression) = ast_makeMoveArgument(scanner, (yyvsp[0].pExpression), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 424: /* expr_list: expr_list ',' expr  */
                                        {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 425: /* expr_list: expr_list ',' "<-" expr  */
                                                   {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-3])),(yyvsp[-3].pExpression),ast_makeMoveArgument(scanner, (yyvsp[0].pExpression), tokAt(scanner,(yylsp[0]))));
    }
    break;

  case 426: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 427: /* block_or_simple_block: "=>" expr_no_bracket  */
                                                   {
            auto retE = new ExprReturn(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->generated = true;
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 428: /* block_or_simple_block: "=>" "<-" expr_no_bracket  */
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

  case 429: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 430: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 431: /* block_or_lambda: "@@"  */
                  { (yyval.i) = 2;   /* local function */ }
    break;

  case 432: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 433: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 434: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 435: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 436: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 437: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 438: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 439: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 440: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 441: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 442: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 443: /* expr_full_block_assumed_piped: '{' expressions '}'  */
                                   {
        // block span is brace-to-brace (@$), not the statements' span (@block)
        (yyval.pExpression) = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc))),(yyvsp[-1].pExpression),tokAt(scanner,(yyloc)),tokAt(scanner,(yyloc)),LineInfo());
    }
    break;

  case 444: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstInt>(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 445: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstUInt>(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 446: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstInt64>(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 447: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstUInt64>(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 448: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstUInt8>(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 449: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstFloat>(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 450: /* expr_numeric_const: "float16 constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstFloat16>(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 451: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstDouble>(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 452: /* expr_assign_no_bracket: expr_no_bracket  */
                                                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 453: /* expr_assign_no_bracket: expr_no_bracket '=' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 454: /* expr_assign_no_bracket: expr_no_bracket "<-" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 455: /* expr_assign_no_bracket: expr_no_bracket "<-" make_table_decl  */
                                                                   { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 456: /* expr_assign_no_bracket: expr_no_bracket "<-" array_comprehension  */
                                                                     { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 457: /* expr_assign_no_bracket: expr_no_bracket ":=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 458: /* expr_assign_no_bracket: expr_no_bracket "&=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 459: /* expr_assign_no_bracket: expr_no_bracket "|=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 460: /* expr_assign_no_bracket: expr_no_bracket "^=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 461: /* expr_assign_no_bracket: expr_no_bracket "&&=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 462: /* expr_assign_no_bracket: expr_no_bracket "||=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 463: /* expr_assign_no_bracket: expr_no_bracket "^^=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 464: /* expr_assign_no_bracket: expr_no_bracket "+=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 465: /* expr_assign_no_bracket: expr_no_bracket "-=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 466: /* expr_assign_no_bracket: expr_no_bracket "*=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 467: /* expr_assign_no_bracket: expr_no_bracket "/=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 468: /* expr_assign_no_bracket: expr_no_bracket "%=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 469: /* expr_assign_no_bracket: expr_no_bracket "<<=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr_assign_no_bracket: expr_no_bracket ">>=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 471: /* expr_assign_no_bracket: expr_no_bracket "<<<=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr_assign_no_bracket: expr_no_bracket ">>>=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 473: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 474: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 475: /* expr_named_call: name_in_namespace '(' expr_list ',' make_struct_fields ')'  */
                                                                                          {
        // bracket-less mixed named call: foo(pos..., name = value) -- named args are a strict suffix
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-3].pExpression));
        nc->arguments = (yyvsp[-1].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 476: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' ')'  */
                                                                    {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 477: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' expr_list ')'  */
                                                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 478: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                                     {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = (yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-7].pExpression));
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 479: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' expr_list ',' make_struct_fields ')'  */
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

  case 480: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' make_struct_fields ')'  */
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

  case 481: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 482: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 483: /* func_addr_expr: "@@" func_addr_name  */
                                            {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 484: /* $@32: %empty  */
                      { yyextra->das_arrow_depth ++; }
    break;

  case 485: /* $@33: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 486: /* func_addr_expr: "@@" '<' $@32 type_declaration_no_options '>' $@33 func_addr_name  */
                                                                                                                                                         {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 487: /* $@34: %empty  */
                      { yyextra->das_arrow_depth ++; }
    break;

  case 488: /* $@35: %empty  */
                                                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 489: /* func_addr_expr: "@@" '<' $@34 optional_function_argument_list optional_function_type '>' $@35 func_addr_name  */
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

  case 490: /* expr_field_no_bracket: expr_no_bracket '.' "name"  */
                                                         {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 491: /* expr_field_no_bracket: expr_no_bracket '.' '.' "name"  */
                                                             {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 492: /* expr_field_no_bracket: expr_no_bracket "!." "name"  */
                                                            {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 493: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' ')'  */
                                                                 {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 494: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' expr_list ')'  */
                                                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 495: /* expr_field_no_bracket: expr_no_bracket "!." "name" '(' ')'  */
                                                                    {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s), true);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 496: /* expr_field_no_bracket: expr_no_bracket "!." "name" '(' expr_list ')'  */
                                                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s), true);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 497: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = (yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-7].pExpression));
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 498: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' expr_list ',' make_struct_fields ')'  */
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

  case 499: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' make_struct_fields ')'  */
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

  case 500: /* expr_field_no_bracket: expr_no_bracket '.' basic_type_declaration '(' ')'  */
                                                                                   {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 501: /* expr_field_no_bracket: expr_no_bracket '.' basic_type_declaration '(' expr_list ')'  */
                                                                                                        {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 502: /* $@36: %empty  */
                                          { yyextra->das_suppress_errors=true; }
    break;

  case 503: /* $@37: %empty  */
                                                                                       { yyextra->das_suppress_errors=false; }
    break;

  case 504: /* expr_field_no_bracket: expr_no_bracket '.' $@36 error $@37  */
                                                                                                                               {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 505: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 506: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 507: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 508: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 509: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 510: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 511: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 512: /* expr: expr_no_bracket  */
                                       { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 513: /* expr: make_table_decl  */
                                     { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 514: /* expr: array_comprehension  */
                                     { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 515: /* expr_no_bracket: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 516: /* expr_no_bracket: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 517: /* expr_no_bracket: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 518: /* expr_no_bracket: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 519: /* expr_no_bracket: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 520: /* expr_no_bracket: make_decl_no_bracket  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 521: /* expr_no_bracket: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 522: /* expr_no_bracket: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 523: /* expr_no_bracket: expr_field_no_bracket  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 524: /* expr_no_bracket: expr_mtag_no_bracket  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 525: /* expr_no_bracket: '!' expr_no_bracket  */
                                                         { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 526: /* expr_no_bracket: '~' expr_no_bracket  */
                                                         { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 527: /* expr_no_bracket: '+' expr_no_bracket  */
                                                             { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 528: /* expr_no_bracket: '-' expr_no_bracket  */
                                                             { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 529: /* expr_no_bracket: expr_no_bracket "<<" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 530: /* expr_no_bracket: expr_no_bracket ">>" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 531: /* expr_no_bracket: expr_no_bracket "<<<" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 532: /* expr_no_bracket: expr_no_bracket ">>>" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 533: /* expr_no_bracket: expr_no_bracket '+' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 534: /* expr_no_bracket: expr_no_bracket '-' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 535: /* expr_no_bracket: expr_no_bracket '*' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 536: /* expr_no_bracket: expr_no_bracket '/' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 537: /* expr_no_bracket: expr_no_bracket '%' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 538: /* expr_no_bracket: expr_no_bracket '<' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 539: /* expr_no_bracket: expr_no_bracket '>' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 540: /* expr_no_bracket: expr_no_bracket "==" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 541: /* expr_no_bracket: expr_no_bracket "!=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 542: /* expr_no_bracket: expr_no_bracket "<=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 543: /* expr_no_bracket: expr_no_bracket ">=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 544: /* expr_no_bracket: expr_no_bracket '&' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 545: /* expr_no_bracket: expr_no_bracket '|' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 546: /* expr_no_bracket: expr_no_bracket '^' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 547: /* expr_no_bracket: expr_no_bracket "&&" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 548: /* expr_no_bracket: expr_no_bracket "||" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 549: /* expr_no_bracket: expr_no_bracket "^^" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 550: /* expr_no_bracket: expr_no_bracket ".." expr_no_bracket  */
                                                                   {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 551: /* expr_no_bracket: "++" expr_no_bracket  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 552: /* expr_no_bracket: "--" expr_no_bracket  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 553: /* expr_no_bracket: expr_no_bracket "++"  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 554: /* expr_no_bracket: expr_no_bracket "--"  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 555: /* expr_no_bracket: '(' expr_list optional_comma ')'  */
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

  case 556: /* expr_no_bracket: '(' make_struct_single ')'  */
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

  case 557: /* expr_no_bracket: expr_no_bracket '[' expr ']'  */
                                                            { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 558: /* expr_no_bracket: expr_no_bracket '.' '[' expr ']'  */
                                                                { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 559: /* expr_no_bracket: expr_no_bracket "![" expr ']'  */
                                                               { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 560: /* expr_no_bracket: expr_no_bracket "?[" expr ']'  */
                                                            { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 561: /* expr_no_bracket: expr_no_bracket '.' "?[" expr ']'  */
                                                                { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 562: /* expr_no_bracket: expr_no_bracket "!?[" expr ']'  */
                                                               { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 563: /* expr_no_bracket: expr_no_bracket "?." "name"  */
                                                            { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 564: /* expr_no_bracket: expr_no_bracket '.' "?." "name"  */
                                                                { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 565: /* expr_no_bracket: expr_no_bracket "!?." "name"  */
                                                               { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 566: /* expr_no_bracket: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 567: /* expr_no_bracket: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 568: /* expr_no_bracket: '*' expr_no_bracket  */
                                                              { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 569: /* expr_no_bracket: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 570: /* expr_no_bracket: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 571: /* $@38: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 572: /* $@39: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 573: /* expr_no_bracket: "addr" '<' $@38 type_declaration_no_options '>' $@39 '(' expr ')'  */
                                                                                                                                                        {
        auto pRef2Ptr = new ExprRef2Ptr(tokAt(scanner,(yylsp[-8])),(yyvsp[-1].pExpression));
        pRef2Ptr->generated = true;
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-8])),pRef2Ptr,(yyvsp[-5].pTypeDecl));
        pCast->reinterpret = true;
        pCast->fromAddrSugar = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 574: /* expr_no_bracket: expr_generator  */
                                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 575: /* expr_no_bracket: expr_no_bracket "??" expr_no_bracket  */
                                                                         { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 576: /* expr_no_bracket: expr_no_bracket "!??" expr_no_bracket  */
                                                                            {
        auto nc = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        nc->no_promotion = true;
        (yyval.pExpression) = nc;
    }
    break;

  case 577: /* expr_no_bracket: expr_no_bracket '?' expr_no_bracket ':' expr_no_bracket  */
                                                                                           {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 578: /* $@40: %empty  */
                                                          { yyextra->das_arrow_depth ++; }
    break;

  case 579: /* $@41: %empty  */
                                                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 580: /* expr_no_bracket: expr_no_bracket "is" "type" '<' $@40 type_declaration_no_options '>' $@41  */
                                                                                                                                                                  {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 581: /* expr_no_bracket: expr_no_bracket "is" basic_type_declaration  */
                                                                          {
        auto vdecl = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc)));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 582: /* expr_no_bracket: expr_no_bracket "is" "name"  */
                                                         {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 583: /* $@42: %empty  */
                                                         { yyextra->das_arrow_depth ++; }
    break;

  case 584: /* $@43: %empty  */
                                                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 585: /* expr_no_bracket: expr_no_bracket "!is" "type" '<' $@42 type_declaration_no_options '>' $@43  */
                                                                                                                                                                 {
        auto isx = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
        isx->no_promotion = true;
        (yyval.pExpression) = isx;
    }
    break;

  case 586: /* expr_no_bracket: expr_no_bracket "!is" basic_type_declaration  */
                                                                         {
        auto vdecl = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc)));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        auto isx = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
        isx->no_promotion = true;
        (yyval.pExpression) = isx;
    }
    break;

  case 587: /* expr_no_bracket: expr_no_bracket "!is" "name"  */
                                                        {
        auto isv = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        isv->no_promotion = true;
        (yyval.pExpression) = isv;
        delete (yyvsp[0].s);
    }
    break;

  case 588: /* expr_no_bracket: expr_no_bracket "as" "name"  */
                                                         {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 589: /* $@44: %empty  */
                                                          { yyextra->das_arrow_depth ++; }
    break;

  case 590: /* $@45: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 591: /* expr_no_bracket: expr_no_bracket "as" "type" '<' $@44 type_declaration '>' $@45  */
                                                                                                                                                       {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 592: /* expr_no_bracket: expr_no_bracket "as" basic_type_declaration  */
                                                                          {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 593: /* expr_no_bracket: expr_no_bracket "!as" "name"  */
                                                        {
        auto asv = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
        delete (yyvsp[0].s);
    }
    break;

  case 594: /* $@46: %empty  */
                                                         { yyextra->das_arrow_depth ++; }
    break;

  case 595: /* $@47: %empty  */
                                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 596: /* expr_no_bracket: expr_no_bracket "!as" "type" '<' $@46 type_declaration '>' $@47  */
                                                                                                                                                      {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        auto asv = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 597: /* expr_no_bracket: expr_no_bracket "!as" basic_type_declaration  */
                                                                         {
        auto asv = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
    }
    break;

  case 598: /* expr_no_bracket: expr_no_bracket '?' "as" "name"  */
                                                             {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 599: /* $@48: %empty  */
                                                              { yyextra->das_arrow_depth ++; }
    break;

  case 600: /* $@49: %empty  */
                                                                                                                          { yyextra->das_arrow_depth --; }
    break;

  case 601: /* expr_no_bracket: expr_no_bracket '?' "as" "type" '<' $@48 type_declaration '>' $@49  */
                                                                                                                                                           {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 602: /* expr_no_bracket: expr_no_bracket '?' "as" basic_type_declaration  */
                                                                              {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 603: /* expr_no_bracket: expr_no_bracket "!?as" "name"  */
                                                         {
        auto asv = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
        delete (yyvsp[0].s);
    }
    break;

  case 604: /* $@50: %empty  */
                                                          { yyextra->das_arrow_depth ++; }
    break;

  case 605: /* $@51: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 606: /* expr_no_bracket: expr_no_bracket "!?as" "type" '<' $@50 type_declaration '>' $@51  */
                                                                                                                                                       {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        auto asv = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 607: /* expr_no_bracket: expr_no_bracket "!?as" basic_type_declaration  */
                                                                          {
        auto asv = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
    }
    break;

  case 608: /* expr_no_bracket: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 609: /* expr_no_bracket: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 610: /* expr_no_bracket: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 611: /* expr_no_bracket: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 612: /* expr_no_bracket: expr_method_call_no_bracket  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 613: /* expr_no_bracket: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 614: /* expr_no_bracket: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 615: /* expr_no_bracket: expr_no_bracket "<|" expr_no_bracket  */
                                                                      { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1])),true); }
    break;

  case 616: /* expr_no_bracket: expr_no_bracket "|>" expr_no_bracket  */
                                                                      { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 617: /* expr_no_bracket: expr_no_bracket "|>" basic_type_declaration  */
                                                                     {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 618: /* expr_no_bracket: expr_call_pipe_no_bracket  */
                                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 619: /* expr_no_bracket: "unsafe" '(' expr ')'  */
                                         {
            (yyvsp[-1].pExpression)->alwaysSafe = true;
            (yyvsp[-1].pExpression)->userSaidItsSafe = true;
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    break;

  case 620: /* expr_no_bracket: expr_no_bracket "=>" expr_no_bracket  */
                                                               {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 621: /* expr_no_bracket: expr_no_bracket "=>" make_table_decl  */
                                                               {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 622: /* expr_no_bracket: expr_no_bracket "=>" array_comprehension  */
                                                                   {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 623: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 624: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 625: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list optional_emit_semis expression_block  */
                                                                                                                                                  {
        auto closure = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->returnType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),closure,tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 626: /* expr_mtag_no_bracket: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 627: /* expr_mtag_no_bracket: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 628: /* expr_mtag_no_bracket: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 629: /* expr_mtag_no_bracket: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 630: /* expr_mtag_no_bracket: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 631: /* expr_mtag_no_bracket: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 632: /* expr_mtag_no_bracket: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 633: /* expr_mtag_no_bracket: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 634: /* expr_mtag_no_bracket: expr_no_bracket '.' "$f" '(' expr ')'  */
                                                                           {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 635: /* expr_mtag_no_bracket: expr_no_bracket "?." "$f" '(' expr ')'  */
                                                                            {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 636: /* expr_mtag_no_bracket: expr_no_bracket '.' '.' "$f" '(' expr ')'  */
                                                                               {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 637: /* expr_mtag_no_bracket: expr_no_bracket "!." "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 638: /* expr_mtag_no_bracket: expr_no_bracket '.' "?." "$f" '(' expr ')'  */
                                                                                {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 639: /* expr_mtag_no_bracket: expr_no_bracket "!?." "$f" '(' expr ')'  */
                                                                               {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 640: /* expr_mtag_no_bracket: expr_no_bracket "as" "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 641: /* expr_mtag_no_bracket: expr_no_bracket "!as" "$f" '(' expr ')'  */
                                                                             {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        cfield->no_promotion = true;
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 642: /* expr_mtag_no_bracket: expr_no_bracket '?' "as" "$f" '(' expr ')'  */
                                                                                  {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 643: /* expr_mtag_no_bracket: expr_no_bracket "!?as" "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        cfield->no_promotion = true;
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 644: /* expr_mtag_no_bracket: expr_no_bracket "is" "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 645: /* expr_mtag_no_bracket: expr_no_bracket "!is" "$f" '(' expr ')'  */
                                                                             {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        cfield->no_promotion = true;
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 646: /* expr_mtag_no_bracket: "@@" "$c" '(' expr ')'  */
                                                           {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 647: /* optional_field_annotation: %empty  */
                                      { (yyval.aaList) = nullptr; }
    break;

  case 648: /* optional_field_annotation: metadata_argument_list  */
                                      { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 649: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 650: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 651: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 652: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 653: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 654: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 655: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 656: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 657: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 658: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 659: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 660: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 661: /* struct_variable_declaration_list: struct_variable_declaration_list "new line, semicolon"  */
                                                                 { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 662: /* struct_variable_declaration_list: struct_variable_declaration_list "typedef" "name" '=' type_declaration SEMICOLON  */
                                                                                                                {
        (yyval.pVarDeclList) = (yyvsp[-5].pVarDeclList);
        ast_structureAlias(scanner,(yyvsp[-3].s),(yyvsp[-1].pTypeDecl),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 663: /* $@52: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 664: /* struct_variable_declaration_list: struct_variable_declaration_list $@52 structure_variable_declaration SEMICOLON  */
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

  case 665: /* $@53: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 666: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable "abstract" optional_constant $@53 function_declaration_header SEMICOLON  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 667: /* $@54: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 668: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@54 function_declaration_header optional_emit_semis block_or_simple_block  */
                                                                                                 {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-10].pVarDeclList),(yyvsp[-9].faList),(yyvsp[-6].b),(yyvsp[-7].b),(yyvsp[-5].i),(yyvsp[-4].b),(yyvsp[-2].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-8]),(yylsp[0])),tokAt(scanner,(yylsp[-9])));
            }
    break;

  case 669: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 670: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 671: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none, tokAt(scanner,(yyloc))), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 672: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 673: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 674: /* function_argument_list: function_argument_declaration_no_type ';' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 675: /* function_argument_list: function_argument_declaration_type ';' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 676: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 677: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 678: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 679: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 680: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 681: /* tuple_alias_type_list: %empty  */
      {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 682: /* tuple_alias_type_list: tuple_type  */
                       {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 683: /* tuple_alias_type_list: tuple_alias_type_list semis tuple_type  */
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

  case 684: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 685: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 686: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 687: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 688: /* variant_alias_type_list: variant_type  */
                         {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 689: /* variant_alias_type_list: variant_alias_type_list semis variant_type  */
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

  case 690: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 691: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 692: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[0])));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 693: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-1])));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 694: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-2])));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 695: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 696: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 697: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 698: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 699: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 700: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 701: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 702: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 703: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 704: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 705: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 706: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 707: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 708: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                               {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 709: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 710: /* global_let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 711: /* global_let_variable_name_with_pos_list: global_let_variable_name_with_pos_list ',' "name"  */
                                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 712: /* variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 713: /* variable_declaration_list: variable_declaration_list SEMICOLON  */
                                                  {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 714: /* variable_declaration_list: variable_declaration_list let_variable_declaration  */
                                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 715: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 716: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 717: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-4])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 718: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                         {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 719: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                               {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 720: /* global_let_variable_declaration: global_let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-4])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 721: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 722: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 723: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 724: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 725: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 726: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 727: /* global_variable_declaration_list: global_variable_declaration_list SEMICOLON  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 728: /* $@55: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 729: /* global_variable_declaration_list: global_variable_declaration_list $@55 optional_field_annotation let_variable_declaration  */
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

  case 730: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 731: /* $@56: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 732: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@56 optional_field_annotation global_let_variable_declaration  */
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

  case 733: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        delete (yyvsp[0].s);
    }
    break;

  case 734: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
        delete (yyvsp[-2].s);
    }
    break;

  case 737: /* enum_list: %empty  */
        {
        (yyval.pEnumList) = new Enumeration();
    }
    break;

  case 738: /* enum_list: enum_expression  */
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

  case 739: /* enum_list: enum_list commas enum_expression  */
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

  case 740: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 741: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 742: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 743: /* $@57: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 744: /* single_alias: optional_public_or_private_alias "name" $@57 '=' type_declaration  */
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

  case 747: /* distinct_alias: optional_public_or_private_alias "name" "name" '=' type_declaration  */
                                                                                               {
        if ( *(yyvsp[-3].s) != "distinct" ) {
            das2_yyerror(scanner,"expected 'distinct', got '"+*(yyvsp[-3].s)+"'",tokAt(scanner,(yylsp[-3])),
                CompilationError::invalid_distinct_type);
        } else {
            ast_distinctDeclaration(scanner,(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])),!(yyvsp[-4].b),(yyvsp[0].pTypeDecl));
        }
        delete (yyvsp[-3].s);
        delete (yyvsp[-2].s);
    }
    break;

  case 748: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 749: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 750: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 751: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 752: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 753: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 760: /* $@58: %empty  */
                                                                     {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 761: /* $@59: %empty  */
                                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 762: /* $@60: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 763: /* enum_declaration: optional_annotation_list_with_emit_semis "enum" $@58 optional_public_or_private_enum enum_name optional_enum_basic_type_declaration optional_emit_commas '{' $@59 enum_list optional_commas $@60 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-8].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-12].faList),tokAt(scanner,(yylsp[-12])),(yyvsp[-9].b),(yyvsp[-8].pEnum),(yyvsp[-3].pEnumList),(yyvsp[-7].type));
    }
    break;

  case 764: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 765: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 766: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 767: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 768: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 769: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 770: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 771: /* class_or_struct: "class" "template"  */
                                  { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 772: /* class_or_struct: "struct" "template"  */
                                  { (yyval.i) = CorS_StructTemplate; }
    break;

  case 773: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 774: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 775: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 776: /* optional_struct_variable_declaration_list: ';'  */
            {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 777: /* optional_struct_variable_declaration_list: '{' struct_variable_declaration_list '}'  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 778: /* $@61: %empty  */
                                                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 779: /* $@62: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 780: /* $@63: %empty  */
                                             {
        if ( (yyvsp[-1].pStructure) ) {
            (yyvsp[-1].pStructure)->isClass = (yyvsp[-4].i)==CorS_Class || (yyvsp[-4].i)==CorS_ClassTemplate;
            (yyvsp[-1].pStructure)->isTemplate = (yyvsp[-4].i)==CorS_ClassTemplate || (yyvsp[-4].i)==CorS_StructTemplate;
            (yyvsp[-1].pStructure)->privateStructure = !(yyvsp[-3].b);
        }
    }
    break;

  case 781: /* structure_declaration: optional_annotation_list_with_emit_semis $@61 class_or_struct optional_public_or_private_structure $@62 structure_name optional_emit_semis $@63 optional_struct_variable_declaration_list  */
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

  case 782: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 783: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 784: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 785: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 786: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                           {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 787: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 788: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 789: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 790: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 791: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 792: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 793: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 794: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 795: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 796: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 797: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 798: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 799: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 800: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 801: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 802: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 803: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 804: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 805: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 806: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 807: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 808: /* basic_type_declaration: "float16"  */
                        { (yyval.type) = Type::tFloat16; }
    break;

  case 809: /* basic_type_declaration: "half2"  */
                        { (yyval.type) = Type::tHalf2; }
    break;

  case 810: /* basic_type_declaration: "half3"  */
                        { (yyval.type) = Type::tHalf3; }
    break;

  case 811: /* basic_type_declaration: "half4"  */
                        { (yyval.type) = Type::tHalf4; }
    break;

  case 812: /* basic_type_declaration: "half8"  */
                        { (yyval.type) = Type::tHalf8; }
    break;

  case 813: /* basic_type_declaration: "short2"  */
                        { (yyval.type) = Type::tShort2; }
    break;

  case 814: /* basic_type_declaration: "short3"  */
                        { (yyval.type) = Type::tShort3; }
    break;

  case 815: /* basic_type_declaration: "short4"  */
                        { (yyval.type) = Type::tShort4; }
    break;

  case 816: /* basic_type_declaration: "short8"  */
                        { (yyval.type) = Type::tShort8; }
    break;

  case 817: /* basic_type_declaration: "ushort2"  */
                        { (yyval.type) = Type::tUShort2; }
    break;

  case 818: /* basic_type_declaration: "ushort3"  */
                        { (yyval.type) = Type::tUShort3; }
    break;

  case 819: /* basic_type_declaration: "ushort4"  */
                        { (yyval.type) = Type::tUShort4; }
    break;

  case 820: /* basic_type_declaration: "ushort8"  */
                        { (yyval.type) = Type::tUShort8; }
    break;

  case 821: /* basic_type_declaration: "byte2"  */
                        { (yyval.type) = Type::tByte2; }
    break;

  case 822: /* basic_type_declaration: "byte3"  */
                        { (yyval.type) = Type::tByte3; }
    break;

  case 823: /* basic_type_declaration: "byte4"  */
                        { (yyval.type) = Type::tByte4; }
    break;

  case 824: /* basic_type_declaration: "byte8"  */
                        { (yyval.type) = Type::tByte8; }
    break;

  case 825: /* basic_type_declaration: "byte16"  */
                        { (yyval.type) = Type::tByte16; }
    break;

  case 826: /* basic_type_declaration: "ubyte2"  */
                        { (yyval.type) = Type::tUByte2; }
    break;

  case 827: /* basic_type_declaration: "ubyte3"  */
                        { (yyval.type) = Type::tUByte3; }
    break;

  case 828: /* basic_type_declaration: "ubyte4"  */
                        { (yyval.type) = Type::tUByte4; }
    break;

  case 829: /* basic_type_declaration: "ubyte8"  */
                        { (yyval.type) = Type::tUByte8; }
    break;

  case 830: /* basic_type_declaration: "ubyte16"  */
                        { (yyval.type) = Type::tUByte16; }
    break;

  case 831: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 832: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 833: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 834: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 835: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 836: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 837: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 838: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 839: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 840: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 841: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 842: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 843: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 844: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 845: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 846: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 847: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 848: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 849: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 850: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 851: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 852: /* bitfield_bits: bitfield_bits ',' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 853: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<tuple<string,Expression *>>();
        (yyval.pNameExprList) = pSL;

    }
    break;

  case 854: /* bitfield_alias_bits: "name"  */
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

  case 855: /* bitfield_alias_bits: "name" '=' expr  */
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

  case 856: /* bitfield_alias_bits: bitfield_alias_bits commas "name"  */
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

  case 857: /* bitfield_alias_bits: bitfield_alias_bits commas "name" '=' expr  */
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

  case 858: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 859: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 860: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 861: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 862: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 863: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' '>'  */
                                                                          {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-2].type), tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-2]));
    }
    break;

  case 864: /* $@64: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 865: /* $@65: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 866: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@64 bitfield_bits '>' $@65  */
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

  case 869: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.aTypePair).secondType->at = (yyval.aTypePair).firstType->at;
    }
    break;

  case 870: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 871: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = appendDimExpr(nullptr, (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 872: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = appendDimExpr(nullptr, nullptr, tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 873: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = appendDimExpr((yyvsp[-3].pTypeDecl), (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 874: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = appendDimExpr((yyvsp[-2].pTypeDecl), nullptr, tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 875: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 876: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
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

  case 877: /* optional_expr_list_in_braces: %empty  */
            { (yyval.pExpression) = nullptr; }
    break;

  case 878: /* optional_expr_list_in_braces: '(' expr_list optional_comma ')'  */
                                                { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 879: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc))); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 880: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 881: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 882: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 883: /* $@66: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 884: /* $@67: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 885: /* type_declaration_no_options_no_dim: "type" '<' $@66 type_declaration '>' $@67  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 886: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->typeMacroExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 887: /* type_declaration_no_options_no_dim: name_in_namespace '(' optional_expr_list ')'  */
                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->typeMacroExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 888: /* type_declaration_no_options_no_dim: '$' name_in_namespace optional_expr_list_in_braces  */
                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-1]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = sequenceToList((yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-1])), *(yyvsp[-1].s)));
        delete (yyvsp[-1].s);
    }
    break;

  case 889: /* $@68: %empty  */
                                    { yyextra->das_arrow_depth ++; }
    break;

  case 890: /* type_declaration_no_options_no_dim: name_in_namespace '<' $@68 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 891: /* $@69: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 892: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@69 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 893: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 894: /* type_declaration_no_options_no_dim: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 895: /* type_declaration_no_options_no_dim: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 896: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 897: /* type_declaration_no_options_no_dim: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 898: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 899: /* type_declaration_no_options_no_dim: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 900: /* type_declaration_no_options_no_dim: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 901: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 902: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 903: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 904: /* type_declaration_no_options_no_dim: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 905: /* $@70: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 906: /* $@71: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 907: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@70 type_declaration '>' $@71  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 908: /* type_declaration_no_options_no_dim: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 909: /* $@72: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 910: /* $@73: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 911: /* type_declaration_no_options_no_dim: "array" '<' $@72 type_declaration '>' $@73  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 912: /* $@74: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 913: /* $@75: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 914: /* type_declaration_no_options_no_dim: "table" '<' $@74 table_type_pair '>' $@75  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 915: /* $@76: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 916: /* $@77: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 917: /* type_declaration_no_options_no_dim: "iterator" '<' $@76 type_declaration '>' $@77  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 918: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 919: /* $@78: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 920: /* $@79: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 921: /* type_declaration_no_options_no_dim: "block" '<' $@78 type_declaration '>' $@79  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 922: /* $@80: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 923: /* $@81: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 924: /* type_declaration_no_options_no_dim: "block" '<' $@80 optional_function_argument_list optional_function_type '>' $@81  */
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

  case 925: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 926: /* $@82: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 927: /* $@83: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 928: /* type_declaration_no_options_no_dim: "function" '<' $@82 type_declaration '>' $@83  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 929: /* $@84: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 930: /* $@85: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 931: /* type_declaration_no_options_no_dim: "function" '<' $@84 optional_function_argument_list optional_function_type '>' $@85  */
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

  case 932: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 933: /* $@86: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 934: /* $@87: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 935: /* type_declaration_no_options_no_dim: "lambda" '<' $@86 type_declaration '>' $@87  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 936: /* $@88: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 937: /* $@89: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 938: /* type_declaration_no_options_no_dim: "lambda" '<' $@88 optional_function_argument_list optional_function_type '>' $@89  */
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

  case 939: /* $@90: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 940: /* $@91: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 941: /* type_declaration_no_options_no_dim: "tuple" '<' $@90 tuple_type_list '>' $@91  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 942: /* $@92: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 943: /* $@93: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 944: /* type_declaration_no_options_no_dim: "variant" '<' $@92 variant_type_list '>' $@93  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 945: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 946: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 947: /* type_declaration: type_declaration '|' '#'  */
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

  case 948: /* $@94: %empty  */
                   {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 949: /* $@95: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 950: /* $@96: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 951: /* $@97: %empty  */
                                                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 952: /* tuple_alias_declaration: "tuple" $@94 optional_public_or_private_alias "name" optional_emit_semis $@95 '{' $@96 tuple_alias_type_list optional_semis $@97 '}'  */
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

  case 953: /* $@98: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 954: /* $@99: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 955: /* $@100: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 956: /* $@101: %empty  */
                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 957: /* variant_alias_declaration: "variant" $@98 optional_public_or_private_alias "name" optional_emit_semis $@99 '{' $@100 variant_alias_type_list optional_semis $@101 '}'  */
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

  case 958: /* $@102: %empty  */
                      {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 959: /* $@103: %empty  */
                                                                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 960: /* $@104: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 961: /* $@105: %empty  */
                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 962: /* bitfield_alias_declaration: "bitfield" $@102 optional_public_or_private_alias "name" bitfield_basic_type_declaration optional_emit_commas $@103 '{' $@104 bitfield_alias_bits optional_commas $@105 '}'  */
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

  case 963: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 964: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 965: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 966: /* make_decl: make_table_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 967: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 968: /* make_decl: table_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 969: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 970: /* make_decl_no_bracket: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 971: /* make_decl_no_bracket: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 972: /* make_decl_no_bracket: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 973: /* make_decl_no_bracket: table_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 974: /* make_decl_no_bracket: make_table_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 975: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 976: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 977: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 978: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 979: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 980: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 981: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 982: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 983: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 984: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 985: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 986: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 987: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 988: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 989: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 990: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 991: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 992: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 993: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 994: /* $@106: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 995: /* $@107: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 996: /* make_struct_decl: "struct" '<' $@106 type_declaration_no_options '>' $@107 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 997: /* $@108: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 998: /* $@109: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 999: /* make_struct_decl: "class" '<' $@108 type_declaration_no_options '>' $@109 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1000: /* $@110: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 1001: /* $@111: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 1002: /* make_struct_decl: "variant" '<' $@110 variant_type_list '>' $@111 '(' use_initializer make_variant_dim ')'  */
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

  case 1003: /* $@112: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 1004: /* $@113: %empty  */
                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 1005: /* make_struct_decl: "variant" "type" '<' $@112 type_declaration_no_options '>' $@113 '(' use_initializer make_variant_dim ')'  */
                                                                                                                                                                                                    {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1006: /* $@114: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 1007: /* $@115: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 1008: /* make_struct_decl: "default" '<' $@114 type_declaration_no_options '>' $@115 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 1009: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pExpression) = mkt;
    }
    break;

  case 1010: /* $@116: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 1011: /* $@117: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 1012: /* make_tuple_call: "tuple" '<' $@116 tuple_type_list '>' $@117 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 1013: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 1014: /* $@118: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 1015: /* $@119: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 1016: /* make_dim_decl: "array" "struct" '<' $@118 type_declaration_no_options '>' $@119 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 1017: /* $@120: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 1018: /* $@121: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 1019: /* make_dim_decl: "array" "tuple" '<' $@120 tuple_type_list '>' $@121 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 1020: /* $@122: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 1021: /* $@123: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 1022: /* make_dim_decl: "array" "variant" '<' $@122 variant_type_list '>' $@123 '(' make_variant_dim ')'  */
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

  case 1023: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 1024: /* $@124: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 1025: /* $@125: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 1026: /* make_dim_decl: "array" '<' $@124 type_declaration_no_options '>' $@125 '(' optional_expr_list ')'  */
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

  case 1027: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 1028: /* $@126: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 1029: /* $@127: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 1030: /* make_dim_decl: "fixed_array" '<' $@126 type_declaration_no_options '>' $@127 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 1031: /* expr_map_tuple_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 1032: /* expr_map_tuple_list: expr_map_tuple_list ',' expr  */
                                                      {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 1033: /* push_table_nesting: %empty  */
                    {
        yyextra->das_nested_parentheses ++;
    }
    break;

  case 1034: /* make_table_decl: '{' push_table_nesting optional_emit_semis optional_expr_map_tuple_list '}'  */
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

  case 1035: /* make_table_call: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 1036: /* make_table_call: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 1037: /* make_table_call: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 1038: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 1039: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 1040: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 1041: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 1042: /* table_comprehension: '[' "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                               {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 1043: /* table_comprehension: '[' "iterator" "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 1044: /* array_comprehension: '{' push_table_nesting optional_emit_semis "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where '}'  */
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


