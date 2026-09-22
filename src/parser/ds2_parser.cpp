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
  YYSYMBOL_453_58 = 453,                   /* $@58  */
  YYSYMBOL_optional_public_or_private_enum = 454, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 455,                /* enum_name  */
  YYSYMBOL_optional_enum_basic_type_declaration = 456, /* optional_enum_basic_type_declaration  */
  YYSYMBOL_optional_commas = 457,          /* optional_commas  */
  YYSYMBOL_emit_commas = 458,              /* emit_commas  */
  YYSYMBOL_optional_emit_commas = 459,     /* optional_emit_commas  */
  YYSYMBOL_enum_declaration = 460,         /* enum_declaration  */
  YYSYMBOL_461_59 = 461,                   /* $@59  */
  YYSYMBOL_462_60 = 462,                   /* $@60  */
  YYSYMBOL_463_61 = 463,                   /* $@61  */
  YYSYMBOL_optional_structure_parent = 464, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 465,          /* optional_sealed  */
  YYSYMBOL_structure_name = 466,           /* structure_name  */
  YYSYMBOL_class_or_struct = 467,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 468, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 469, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 470,    /* structure_declaration  */
  YYSYMBOL_471_62 = 471,                   /* $@62  */
  YYSYMBOL_472_63 = 472,                   /* $@63  */
  YYSYMBOL_473_64 = 473,                   /* $@64  */
  YYSYMBOL_variable_name_with_pos_list = 474, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 475,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 476, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 477, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 478,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 479,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 480,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_basic_type_declaration = 481, /* bitfield_basic_type_declaration  */
  YYSYMBOL_bitfield_type_declaration = 482, /* bitfield_type_declaration  */
  YYSYMBOL_483_65 = 483,                   /* $@65  */
  YYSYMBOL_484_66 = 484,                   /* $@66  */
  YYSYMBOL_c_or_s = 485,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 486,          /* table_type_pair  */
  YYSYMBOL_dim_list = 487,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 488, /* type_declaration_no_options  */
  YYSYMBOL_optional_expr_list_in_braces = 489, /* optional_expr_list_in_braces  */
  YYSYMBOL_type_declaration_no_options_no_dim = 490, /* type_declaration_no_options_no_dim  */
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
  YYSYMBOL_518_94 = 518,                   /* $@94  */
  YYSYMBOL_type_declaration = 519,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 520,  /* tuple_alias_declaration  */
  YYSYMBOL_521_95 = 521,                   /* $@95  */
  YYSYMBOL_522_96 = 522,                   /* $@96  */
  YYSYMBOL_523_97 = 523,                   /* $@97  */
  YYSYMBOL_524_98 = 524,                   /* $@98  */
  YYSYMBOL_variant_alias_declaration = 525, /* variant_alias_declaration  */
  YYSYMBOL_526_99 = 526,                   /* $@99  */
  YYSYMBOL_527_100 = 527,                  /* $@100  */
  YYSYMBOL_528_101 = 528,                  /* $@101  */
  YYSYMBOL_529_102 = 529,                  /* $@102  */
  YYSYMBOL_bitfield_alias_declaration = 530, /* bitfield_alias_declaration  */
  YYSYMBOL_531_103 = 531,                  /* $@103  */
  YYSYMBOL_532_104 = 532,                  /* $@104  */
  YYSYMBOL_533_105 = 533,                  /* $@105  */
  YYSYMBOL_534_106 = 534,                  /* $@106  */
  YYSYMBOL_make_decl = 535,                /* make_decl  */
  YYSYMBOL_make_decl_no_bracket = 536,     /* make_decl_no_bracket  */
  YYSYMBOL_make_struct_fields = 537,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 538,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 539,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim_list = 540,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 541,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 542, /* optional_make_struct_dim_decl  */
  YYSYMBOL_use_initializer = 543,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 544,         /* make_struct_decl  */
  YYSYMBOL_545_107 = 545,                  /* $@107  */
  YYSYMBOL_546_108 = 546,                  /* $@108  */
  YYSYMBOL_547_109 = 547,                  /* $@109  */
  YYSYMBOL_548_110 = 548,                  /* $@110  */
  YYSYMBOL_549_111 = 549,                  /* $@111  */
  YYSYMBOL_550_112 = 550,                  /* $@112  */
  YYSYMBOL_551_113 = 551,                  /* $@113  */
  YYSYMBOL_552_114 = 552,                  /* $@114  */
  YYSYMBOL_553_115 = 553,                  /* $@115  */
  YYSYMBOL_554_116 = 554,                  /* $@116  */
  YYSYMBOL_make_tuple_call = 555,          /* make_tuple_call  */
  YYSYMBOL_556_117 = 556,                  /* $@117  */
  YYSYMBOL_557_118 = 557,                  /* $@118  */
  YYSYMBOL_make_dim_decl = 558,            /* make_dim_decl  */
  YYSYMBOL_559_119 = 559,                  /* $@119  */
  YYSYMBOL_560_120 = 560,                  /* $@120  */
  YYSYMBOL_561_121 = 561,                  /* $@121  */
  YYSYMBOL_562_122 = 562,                  /* $@122  */
  YYSYMBOL_563_123 = 563,                  /* $@123  */
  YYSYMBOL_564_124 = 564,                  /* $@124  */
  YYSYMBOL_565_125 = 565,                  /* $@125  */
  YYSYMBOL_566_126 = 566,                  /* $@126  */
  YYSYMBOL_567_127 = 567,                  /* $@127  */
  YYSYMBOL_568_128 = 568,                  /* $@128  */
  YYSYMBOL_expr_map_tuple_list = 569,      /* expr_map_tuple_list  */
  YYSYMBOL_push_table_nesting = 570,       /* push_table_nesting  */
  YYSYMBOL_make_table_decl = 571,          /* make_table_decl  */
  YYSYMBOL_make_table_call = 572,          /* make_table_call  */
  YYSYMBOL_array_comprehension_where = 573, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 574,           /* optional_comma  */
  YYSYMBOL_table_comprehension = 575,      /* table_comprehension  */
  YYSYMBOL_array_comprehension = 576       /* array_comprehension  */
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
#define YYLAST   12613

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  242
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  335
/* YYNRULES -- Number of rules.  */
#define YYNRULES  1045
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1868

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
     693,   697,   702,   711,   726,   742,   747,   755,   755,   800,
     818,   822,   825,   829,   833,   837,   841,   847,   856,   857,
     861,   864,   867,   873,   874,   878,   882,   883,   887,   890,
     896,   902,   905,   911,   912,   916,   917,   921,   922,   926,
     927,   927,   931,   931,   940,   941,   945,   946,   952,   953,
     954,   955,   956,   960,   961,   965,   966,   970,   972,   970,
     984,   984,   992,   994,   992,  1006,  1006,  1014,  1016,  1014,
    1027,  1034,  1041,  1046,  1055,  1063,  1069,  1073,  1081,  1091,
    1091,  1100,  1108,  1108,  1124,  1130,  1137,  1156,  1160,  1167,
    1168,  1169,  1170,  1171,  1172,  1176,  1181,  1189,  1190,  1191,
    1192,  1196,  1197,  1198,  1199,  1200,  1201,  1202,  1203,  1204,
    1210,  1213,  1219,  1222,  1228,  1231,  1234,  1240,  1241,  1242,
    1243,  1247,  1265,  1288,  1291,  1301,  1316,  1331,  1346,  1349,
    1356,  1360,  1367,  1368,  1372,  1373,  1377,  1378,  1379,  1383,
    1387,  1391,  1398,  1402,  1403,  1404,  1405,  1406,  1407,  1408,
    1409,  1410,  1411,  1412,  1413,  1414,  1415,  1416,  1417,  1418,
    1419,  1420,  1421,  1422,  1423,  1424,  1425,  1426,  1427,  1428,
    1429,  1430,  1431,  1432,  1433,  1434,  1435,  1436,  1437,  1438,
    1439,  1440,  1441,  1442,  1443,  1444,  1445,  1446,  1447,  1448,
    1449,  1450,  1451,  1452,  1453,  1454,  1455,  1456,  1457,  1458,
    1459,  1460,  1461,  1462,  1463,  1464,  1465,  1466,  1467,  1468,
    1469,  1470,  1471,  1472,  1473,  1474,  1475,  1476,  1477,  1478,
    1479,  1480,  1481,  1482,  1483,  1484,  1485,  1486,  1487,  1488,
    1489,  1493,  1494,  1495,  1496,  1497,  1498,  1499,  1500,  1501,
    1502,  1503,  1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,
    1512,  1513,  1514,  1515,  1516,  1517,  1518,  1519,  1520,  1521,
    1522,  1523,  1524,  1525,  1526,  1527,  1528,  1529,  1530,  1531,
    1532,  1533,  1534,  1535,  1536,  1537,  1538,  1539,  1540,  1544,
    1545,  1549,  1568,  1569,  1570,  1574,  1580,  1580,  1597,  1600,
    1602,  1600,  1614,  1616,  1614,  1631,  1649,  1667,  1685,  1696,
    1697,  1698,  1699,  1700,  1701,  1702,  1703,  1704,  1705,  1706,
    1707,  1708,  1709,  1710,  1711,  1712,  1713,  1714,  1715,  1717,
    1715,  1732,  1737,  1743,  1749,  1750,  1754,  1755,  1759,  1763,
    1770,  1771,  1782,  1786,  1789,  1797,  1797,  1797,  1800,  1806,
    1809,  1813,  1817,  1824,  1831,  1837,  1841,  1845,  1848,  1851,
    1859,  1862,  1870,  1876,  1877,  1878,  1882,  1883,  1887,  1888,
    1892,  1897,  1905,  1912,  1924,  1928,  1931,  1941,  1941,  1941,
    1944,  1944,  1944,  1949,  1949,  1949,  1957,  1957,  1957,  1963,
    1973,  1984,  1999,  2002,  2005,  2008,  2014,  2015,  2023,  2035,
    2036,  2037,  2041,  2042,  2043,  2044,  2045,  2049,  2054,  2062,
    2063,  2067,  2074,  2078,  2085,  2086,  2087,  2088,  2089,  2090,
    2091,  2092,  2096,  2097,  2098,  2099,  2100,  2101,  2102,  2103,
    2104,  2105,  2106,  2107,  2108,  2109,  2110,  2111,  2112,  2113,
    2114,  2115,  2116,  2120,  2126,  2133,  2145,  2151,  2159,  2167,
    2178,  2190,  2194,  2201,  2204,  2204,  2204,  2209,  2209,  2209,
    2222,  2226,  2230,  2234,  2240,  2248,  2254,  2262,  2270,  2281,
    2290,  2296,  2304,  2304,  2304,  2311,  2315,  2324,  2332,  2340,
    2344,  2347,  2355,  2356,  2357,  2364,  2365,  2366,  2367,  2368,
    2369,  2370,  2371,  2372,  2373,  2374,  2375,  2376,  2377,  2378,
    2379,  2380,  2381,  2382,  2383,  2384,  2385,  2386,  2387,  2388,
    2389,  2390,  2391,  2392,  2393,  2394,  2395,  2396,  2397,  2398,
    2399,  2405,  2406,  2407,  2408,  2409,  2424,  2433,  2434,  2435,
    2436,  2437,  2438,  2439,  2440,  2441,  2442,  2443,  2444,  2445,
    2446,  2447,  2447,  2447,  2455,  2456,  2457,  2462,  2465,  2465,
    2465,  2468,  2473,  2477,  2477,  2477,  2482,  2489,  2495,  2499,
    2499,  2499,  2504,  2507,  2513,  2513,  2513,  2520,  2525,  2529,
    2529,  2529,  2534,  2537,  2543,  2543,  2543,  2550,  2555,  2556,
    2557,  2558,  2559,  2560,  2561,  2562,  2563,  2565,  2569,  2570,
    2575,  2581,  2587,  2596,  2599,  2602,  2611,  2612,  2613,  2614,
    2615,  2616,  2617,  2621,  2625,  2629,  2633,  2637,  2641,  2645,
    2649,  2653,  2658,  2662,  2667,  2671,  2676,  2683,  2684,  2688,
    2689,  2690,  2694,  2695,  2699,  2700,  2701,  2705,  2706,  2710,
    2722,  2725,  2726,  2730,  2730,  2749,  2748,  2763,  2762,  2779,
    2791,  2800,  2810,  2811,  2812,  2813,  2814,  2818,  2821,  2830,
    2831,  2835,  2838,  2842,  2855,  2864,  2865,  2869,  2872,  2876,
    2889,  2890,  2894,  2899,  2904,  2912,  2915,  2922,  2925,  2931,
    2932,  2933,  2937,  2938,  2942,  2949,  2954,  2963,  2969,  2973,
    2984,  2991,  3000,  3003,  3006,  3013,  3017,  3023,  3034,  3037,
    3042,  3053,  3054,  3058,  3059,  3060,  3064,  3067,  3070,  3070,
    3090,  3093,  3093,  3111,  3116,  3124,  3125,  3129,  3132,  3145,
    3162,  3163,  3164,  3169,  3169,  3195,  3196,  3203,  3203,  3225,
    3226,  3227,  3231,  3241,  3244,  3250,  3251,  3255,  3256,  3260,
    3261,  3265,  3267,  3272,  3265,  3288,  3289,  3293,  3294,  3298,
    3304,  3305,  3306,  3307,  3311,  3312,  3313,  3317,  3320,  3326,
    3328,  3333,  3326,  3354,  3361,  3366,  3375,  3381,  3385,  3396,
    3397,  3398,  3399,  3400,  3401,  3402,  3403,  3404,  3405,  3406,
    3407,  3408,  3409,  3410,  3411,  3412,  3413,  3414,  3415,  3416,
    3417,  3418,  3419,  3420,  3421,  3422,  3423,  3424,  3425,  3426,
    3427,  3428,  3429,  3430,  3431,  3432,  3433,  3434,  3435,  3436,
    3437,  3438,  3439,  3440,  3441,  3442,  3443,  3444,  3445,  3449,
    3450,  3451,  3452,  3453,  3454,  3455,  3456,  3460,  3471,  3475,
    3482,  3494,  3501,  3507,  3516,  3521,  3531,  3541,  3551,  3564,
    3565,  3566,  3567,  3568,  3572,  3576,  3576,  3576,  3590,  3591,
    3595,  3600,  3607,  3610,  3613,  3616,  3622,  3625,  3639,  3640,
    3644,  3645,  3646,  3647,  3648,  3648,  3648,  3652,  3657,  3664,
    3671,  3671,  3678,  3678,  3685,  3689,  3693,  3698,  3703,  3708,
    3713,  3717,  3721,  3726,  3730,  3734,  3739,  3739,  3739,  3745,
    3752,  3752,  3752,  3757,  3757,  3757,  3763,  3763,  3763,  3768,
    3774,  3774,  3774,  3779,  3779,  3779,  3788,  3794,  3794,  3794,
    3799,  3799,  3799,  3808,  3814,  3814,  3814,  3819,  3819,  3819,
    3828,  3828,  3828,  3834,  3834,  3834,  3843,  3846,  3857,  3873,
    3875,  3880,  3885,  3873,  3911,  3913,  3918,  3924,  3911,  3950,
    3952,  3957,  3962,  3950,  4003,  4004,  4005,  4006,  4007,  4008,
    4009,  4013,  4014,  4015,  4016,  4017,  4021,  4028,  4035,  4041,
    4047,  4054,  4061,  4067,  4076,  4079,  4085,  4093,  4098,  4105,
    4110,  4116,  4117,  4121,  4122,  4126,  4126,  4126,  4134,  4134,
    4134,  4141,  4141,  4141,  4151,  4151,  4151,  4158,  4158,  4158,
    4169,  4175,  4175,  4175,  4188,  4207,  4207,  4207,  4217,  4217,
    4217,  4230,  4230,  4230,  4243,  4252,  4252,  4252,  4272,  4279,
    4279,  4279,  4289,  4292,  4303,  4309,  4332,  4340,  4360,  4385,
    4386,  4390,  4391,  4396,  4399,  4409
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

#define YYPACT_NINF (-1685)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-938)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1685,    48, -1685, -1685,    77,  -110,   -90,   351, -1685,   -15,
   -1685, -1685, -1685, -1685,   360,   101, -1685, -1685, -1685, -1685,
     132,   132,   132, -1685,   386, -1685,   173, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,   147, -1685,
      95,   187,   224, -1685,   198,   258, -1685, -1685, -1685,   255,
     132,   132, -1685, -1685,   173,   351,   351,   351,   338,   427,
   -1685, -1685, -1685, -1685,   101,   101,   101,   229, -1685,   671,
     166, -1685, -1685, -1685, -1685,   456, -1685,   854, -1685,   530,
     118,    77,   431,  -110,   469, -1685,   201,   472,   471,   445,
      17,   567, -1685, -1685,   531,   574,   607,   624, -1685,   680,
     536, -1685, -1685,   -66,    77,   101,   101,   101,   101,   557,
   -1685,   769,   800,   754,   756,   804, -1685, -1685,   676, -1685,
   -1685,   728, -1685, -1685, -1685,   865,   106, -1685, -1685, -1685,
   -1685,   198,   198,   700,   198,   743,   758,   724,   765, -1685,
   -1685, -1685,   768, -1685, -1685,   757,   774,   557,   557, -1685,
   -1685,   797, -1685,   200, -1685,   552,   836,   671, -1685,   816,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685,   826, -1685, -1685,
   -1685, -1685, -1685, -1685,   855, -1685, -1685, -1685, -1685,   880,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685,   227,   700,   700,
     700, -1685, -1685,   976,   976,   845, 10675,   997, -1685,   678,
     878, -1685, -1685, -1685, -1685, -1685, 12117, -1685,   876,   966,
      71,    77,   931,   909, -1685, -1685, -1685,   106, -1685, -1685,
   -1685, 10675,   913,   915,   916,   897,   920,   921, -1685, -1685,
   -1685,   902, -1685, -1685, -1685, -1685, -1685,   688, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,   923,
   -1685, -1685, -1685,   924,   925, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685,   929,   933,   911,   360,   158, -1685, -1685, -1685,
   -1685,   363,   917,   939, -1685, -1685, -1685, -1685, -1685, -1685,
     953, -1685,   914,   928, 10843, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685,  1120,  1127, -1685,   934, -1685,   557,  1317,
     878, -1685,   972,   557, -1685, -1685,   855,   557,    77, -1685,
     -87, -1685, -1685,   939, -1685, -1685, -1685,  9292, -1685, -1685,
     977,   949,    -4,    67,    74, -1685, -1685,  9292,   176, -1685,
    6817, -1685, -1685, -1685,    13, -1685, -1685, -1685,     5, -1685,
    7042,   941,  1938, -1685,   936, -1685, -1685, 12245, 12362, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,   979,
     945, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685,  1157, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685,   984,   950, -1685, -1685,
     -70,   -44,  -100, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685,   943,   974, -1685,   -65, -1685,   557,   988, 10675,
   -1685,   -47, 10675, 10675, 10675,   973,   985, -1685, -1685,   124,
     360,   986,    37, -1685,   234,   952,   991,   992,   466,   993,
     957,   487,   994, -1685,   492,    38,   998, 10186, 10186,   320,
     961,   964,   968,   987,   989,   990, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, 10186, 10186, 10186, 10186,
   10186,  4342,  5242, -1685, -1685, -1685, -1685, -1685, -1685,   995,
   -1685, -1685, -1685, -1685,   983, -1685, -1685,   -79,   -79, -1685,
     -79,   -79,   978, 10948, -1685, -1685,   996, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, 10675, 10675,  1002,  1003, 10675,
     934, 10675,   934, 10675,   934, 10842,  1022,  1005, -1685,  6817,
   -1685, 10675,  9292,  1006,  1013, -1685, -1685, -1685, -1685, -1685,
    1000, -1685, -1685,  1018,  7267, -1685,   363, -1685, 10842,  1022,
   -1685, -1685, -1685, -1685, -1685, -1685, 12416,  1112,   705,  1023,
   -1685,    61,  1015,   155,  1021, 10675, 10675, -1685,  9740, -1685,
   -1685, -1685, -1685,   360, -1685,   669,  1025,  1182,   335, -1685,
   -1685, -1685,   266, -1685, -1685, -1685,  9292,   689,   712,  1012,
     274, -1685, -1685, -1685, -1685,  1027, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685,   540, -1685,  1017,  1049,  1051,
   -1685,  6817, 10675,  9292,  9292, -1685, -1685, -1685,  9292, -1685,
    9292, -1685,  6817, -1685, -1685,  6817,  1052, -1685, 10675,   556,
     556,  1033,  1034,   113, -1685, -1685,  9292,  9292,  9292,  9292,
    9292,  9292,   834,   556,   556,   578,   556,   556,  1035,  1257,
    1038,  1039,   195,  1013,  1065,  1040,   557,  3892,   101,  1270,
   -1685, -1685,   983, -1685, -1685, -1685, -1685, -1685,  2322, 11696,
   10186, 10186, -1685, -1685, 10186, 10186, 10186, 10186,  1080, 10186,
     -60,  9292,    16,   566,  9292,  9292, 10186, 11751, 11878, 11933,
   10186, 10186, 10186, 10186,  9292, 10186, 10186, 10186, 10186,  9963,
   10186, 10186, 10186, 10186, 10186, 10186, 10186, 10186, 10186, 10186,
   12189,  9292,  5467,   722,   727, -1685, -1685,  1081,   745,   -44,
     771,   -44,   772,   -44,   -86, -1685,   242,   939,  1070, -1685,
     414, -1685, 10675,  1013,   447,   939, -1685, -1685,  7492, -1685,
   -1685, -1685, -1685,  1053,  1087, -1685,   132, -1685,   132, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685,  9292, -1685, -1685,
     263,   283,   283,   283, -1685,   939,   939, 10186, 11043, -1685,
    1088, -1685, -1685, -1685, -1685,  9292,  1090,   506, 10675,   -47,
   -1685,  9292,   132, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   10675, 10675, 10675, 10675,  4567,  1094,  9292, 10675, -1685, -1685,
   -1685, 10675,  1013,   615, -1685,  1086,  1059, 10675, 10675, 10675,
    1060, 10675,  1061, 10675,  1013, 10675, 10842,  1013, -1685,  1022,
     382,  9292,  9292, 10675,   934,  1062,  1064,  1066,  1067,  1068,
    1069, -1685,  9292,   844,   168,  1071, -1685,  9292, -1685,  9292,
   -1685,  9292,  1072,   599, -1685, -1685,  7717,    94,  4117, -1685,
     251,  1073,   212,  1075,   934,  2456,  1270,  1098,  1082, -1685,
   -1685,  1100,  1083, -1685, -1685,  1314,  1314,   812,   812,  1490,
    1490,  1084,   587,  1085, -1685,  1089,  1096,  1097,  1103, -1685,
    1095,  1099,   587,  1107,  1105, -1685, -1685,  1109,  1108, -1685,
   -1685,  1130,  1115, -1685, -1685,   462,   462,   996,  1314,  1314,
   11043, -1685, -1685, 11332, 11217, 11304, 11043, 12060,  2135, 11419,
   11506, 11534,   812,   812,  1728,  1728,   587,   587,   587,   605,
    9292,  1116,  1117,   619,  9292,  1345,  1119,  1121, -1685,   287,
   -1685, -1685, -1685,   171, -1685,  1140, -1685,  1147, -1685,  1149,
   10675, -1685, 10842, 10675, -1685,  1022,   553,  1131,  1135, 10675,
    9292, -1685, -1685,  1166,   516, -1685, 10506, -1685,     2, -1685,
    1142,  1141,  1336, -1685, -1685,   129, -1685, -1685, -1685, 11130,
    2696,  1173, -1685,   516,    26,  1148, -1685,  1150,  1339,   266,
    9292,   132, -1685, -1685, -1685, -1685,   939,   423,   856,   790,
     665,   291,  1153,  1154,   560,  1155,   793, 10675, 10842,  1022,
     981,  1162,  1151, 10675,  9292,  1163, -1685,  1001,  1019,  1111,
   -1685,  1132, -1685,  1178,  1164,  1221,   707,  1167, 10675,   711,
    1270,  1168,  1171,  1310,   -44, -1685, -1685, -1685, -1685, -1685,
    1175,  1202,  1179,  1371,  1223,    27,   168,  1186, -1685, -1685,
   -1685,  1189,    30,  1190,  1185,  1086,   111, -1685,  1191,   298,
    5692, -1685, -1685, -1685,   133,   -44, -1685,  7942, -1685,  1193,
    8167,  1232,  1236, -1685,   132,  1250,  8392,    -2,  8617, -1685,
   -1685, -1685,   132,   132,  1430, -1685,  1063, -1685, -1685,  1428,
   -1685, -1685,  1433,  1401, -1685,   132, -1685,   132,   132,   132,
     132,   132, -1685,  1378, -1685,   132, 10304,   934, -1685,  9292,
   -1685,  9292,  4792,  9292, -1685,  9292,  5917,  9292, -1685, -1685,
   -1685,  9292, -1685,  9292, -1685,  9292,  1231,  1212, -1685, -1685,
   10186,  1213, -1685,  1215,  9292,  5017,  1216, -1685,  1218, -1685,
    6142, -1685,  7492, -1685, -1685, -1685,  1256, -1685,  1262, -1685,
   -1685, -1685, -1685, -1685, -1685,   939, -1685, -1685,   939, -1685,
   -1685,  1135, -1685, -1685,   939, -1685,  9292, -1685,   714, -1685,
   -1685, -1685,  1214, -1685,  1220, -1685,  9292,  1267,   584, 10675,
   -1685,  9292,  1238,  9292,   732, -1685,  1283, -1685, -1685,  1473,
     855, -1685,  9292,  1285, -1685,  9292,   132, -1685, -1685, -1685,
   -1685,  1248, -1685, -1685, -1685,  1251,  1287, -1685, -1685,  1367,
     721,   781, -1685, -1685,  9292,  1456, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685,  1551, -1685,    86,
   -1685, -1685, -1685,  1271,  6367, -1685,  1286,  9292,  1291, -1685,
     292,  6817,   177,    47,   140,  9292,  9292,  9292,   168, -1685,
   -1685, -1685,   599,  1255,  4117,   293,  1295,  1296,  1260,  1298,
    1299, -1685,   295,   557,  9292, -1685,  1485,  9292, -1685,  1292,
    1293, -1685,  1294,  1313, -1685,  1193,  9292, -1685, -1685, -1685,
   -1685,  1269, -1685, -1685,  1272,   -56,   -56,  1273, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685,   -98, -1685, 10186, 10186, 10186,
   10186, 10186, 10186, 10186, 10186, 10186, 10186,  9292, 10186, 10186,
   10186, 10186, 10186, 10186, 10186,   -44, 10675,  1274, 10675,  1275,
    4117, -1685,   304,   312,  1278,  1289, -1685,   316,  1300, 10675,
    1302, 10675,  1303, 10675,  1304, -1685,  9292, 11130,  9292, -1685,
    1305,  4117, -1685,   347,   348,  9292, -1685, -1685, -1685,   354,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,  1312, -1685,
    1268, -1685, -1685,  1306, -1685,  1288,  1484,    -6, -1685,  1491,
   -1685, -1685,  1307,  1321,   828,  1475,   132,  1308, -1685,   132,
   -1685,  1315,  1316, -1685, -1685,  9292,  1333, -1685, -1685, -1685,
   -1685,  1318,  1319,  1322,  1324,  1325,  1330,  1331,  1332,  1483,
    1334, -1685,  1335,  8842,  1193,   343, -1685, -1685,   389, -1685,
   -1685,  1338, -1685,  1370, -1685,   398,  1337,  1527,  1223,  6817,
    9292,  9292,  1340, -1685, -1685,    52, -1685,   311, -1685, -1685,
   -1685,  1377, -1685, -1685,   133, -1685,  -100, -1685,  1193, -1685,
   10675,  9292, -1685, -1685, -1685, -1685,  2936,  9292,  9292,    77,
     931,  1344,  1351,  9067,  1223, -1685, -1685,   -65, 10948, 10948,
   10948, 10948, 10948, 10948, 10948, 10948, 10948, 10948, 10948, -1685,
   -1685, 10948, 10948, 10948, 10948, 10948, 10948, 10948,   557,  1564,
   -1685,   801, -1685,   357,  6592, -1685, -1685, -1685, -1685, -1685,
   -1685,  1621, -1685,   806, -1685,   807, -1685, 10675,  1352,  1353,
   -1685,   368,  6592, -1685, -1685,  1354, -1685,  9292, -1685, -1685,
    9292,  1393,  9292, -1685, -1685, -1685, 10675, -1685, -1685,   590,
   -1685,    39, -1685, -1685, -1685,  1483,  1483,  1356,  1358,  1359,
    1360,  1361,  6817, -1685,  9292,  9292,  9292,  9292,  9292,  6817,
   -1685, -1685,  1483,  1364,  1483, -1685,  1365, -1685, -1685,   343,
   -1685,  1398, -1685, -1685,  1363,  9292,  1405,   400,   421, -1685,
   -1685,   204,  6817,  1374,  1375, -1685, -1685, -1685,   939, -1685,
    1366,  1379,  1382,   373,   168,  9292,   198,  1383,   437,    15,
   -1685,  -100, -1685, -1685,  1384,   443, -1685, -1685, -1685,   809,
   -1685, -1685,  1385,   446, -1685, -1685,  1386, -1685, -1685,  1387,
    -128,  1572,    39, -1685, -1685,   828,   135,   135, -1685,  9292,
    1483,  1483,   665,  1388,  1392,  1399,  1400,  1402,  1422,  1013,
     135,  1483,   665, -1685, -1685, -1685,  9292,  1423, -1685, -1685,
    1390,  9292,  9292,   451, -1685, -1685,  1491,  1601,   557, -1685,
      56,  1426,   559,   557,   100, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685,  1572,   263,   665,  1415,  1455, -1685,
    1431,  1432,  1435,   135,   135,  1415,  1439, -1685, -1685, -1685,
   -1685, -1685, -1685,  1440,  1441,   665,  1442,  1425, -1685,  9292,
   -1685, -1685,  1446, -1685,  9517,   132, -1685,  6817,   557,   557,
    1193, 10675,   -47, -1685,  3176, 12117, -1685, -1685, -1685, -1685,
     464,  1406, -1685, -1685, -1685, -1685,  1443,  1444, -1685, -1685,
   -1685,  1450, -1685,  1615,  1454,  1425,  9292, -1685, -1685, -1685,
   -1685, -1685, 10948, -1685,  1449,   485,  1193,  1193, -1685,   300,
    9292,  1451,   132, 12117, -1685,   665, -1685, -1685, -1685,  9292,
   -1685,  1459,  1425, -1685,   864,  9517,   557, -1685, -1685,  9292,
     132, -1685, -1685,   557,   489, -1685, -1685,  1453, -1685,   557,
   -1685, -1685,  1460, -1685,   132,  1193,   132, -1685,  -100, -1685,
   -1685,  3416, -1685,  9292, -1685, -1685, -1685, -1685,  1461,  1457,
    1464,  1491, -1685, -1685,  9517,   557, -1685, -1685,   132, -1685,
    3656, -1685,  1457,  1463,   864,  1491, -1685, -1685
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   174,     1,   396,     0,     0,    58,   740,   397,     0,
     959,   949,   954,    20,     0,     0,    19,    16,    15,     3,
       0,     0,     0,     8,   779,     7,   721,     6,    11,     5,
       4,    13,    12,    14,   138,   139,   140,   137,   148,   150,
      49,    68,    65,    66,     0,     0,    50,   742,   741,     0,
       0,     0,    26,    25,   721,   740,   740,   740,     0,   370,
      47,   158,   159,   160,     0,     0,     0,   161,   163,   170,
       0,   157,    21,    10,     9,   319,   761,     0,   722,   723,
       0,     0,     0,     0,     0,    51,     0,     0,    59,     0,
      63,   743,   745,   746,    22,     0,     0,     0,   372,     0,
       0,   169,   164,     0,     0,     0,     0,     0,     0,    77,
     320,   322,   749,   771,   770,   774,   725,   724,   731,   146,
     147,     0,   144,   145,   142,     0,     0,   141,   151,    69,
      67,     0,     0,    52,     0,     0,     0,     0,     0,    64,
      60,   747,     0,    23,    24,    27,   859,    77,    77,   371,
      45,    48,   168,     0,   165,   166,   167,   171,    75,    78,
     175,   324,   323,   326,   321,   751,   750,     0,   773,   772,
     776,   775,   780,   726,   647,   143,    30,    31,    35,     0,
     133,   134,   131,   132,   130,   129,   135,     0,    54,    55,
      53,    57,    56,    63,    63,     0,     0,     0,    29,     0,
     759,   950,   955,    46,   162,    76,     0,   752,   753,   767,
     728,     0,   648,     0,    32,    33,    34,     0,   149,    62,
      61,     0,     0,     0,     0,     0,     0,     0,   789,   832,
     790,   848,   791,   795,   796,   797,   798,   838,   802,   803,
     804,   805,   806,   807,   808,   833,   834,   835,   836,   919,
     794,   801,   837,   926,   933,   792,   799,   793,   800,   809,
     810,   811,   812,   813,   814,   815,   816,   817,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,   828,   829,
     830,   831,     0,     0,     0,     0,   847,   880,   883,   881,
     882,   946,   876,   744,    28,   862,   863,   860,   861,   757,
     760,   960,     0,     0,     0,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,     0,     0,   182,   176,   270,    77,     0,
     759,   768,     0,    77,   730,   727,   647,    77,     0,   710,
     702,   732,   136,   748,   884,   910,   913,     0,   916,   906,
       0,     0,   920,   927,   934,   940,   943,     0,   878,   890,
     364,   896,   901,   895,     0,   909,   905,   898,     0,   900,
       0,   877,     0,   758,     0,   951,   956,   261,   262,   259,
     185,   186,   188,   187,   189,   190,   191,   192,   218,   219,
     216,   217,   209,   220,   221,   210,   207,   208,   260,   243,
       0,   258,   222,   223,   224,   225,   196,   197,   198,   193,
     194,   195,   206,     0,   212,   213,   211,   204,   205,   200,
     199,   201,   202,   203,   184,   183,   242,     0,   214,   215,
     647,   179,     0,   839,   842,   845,   846,   840,   843,   841,
     844,   754,     0,   765,   781,     0,   152,    77,     0,     0,
     703,     0,     0,     0,     0,     0,     0,   521,   522,     0,
       0,     0,     0,   515,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   838,     0,     0,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,   631,   444,   446,   445,
     447,   448,   449,   450,   451,    41,     0,     0,     0,     0,
       0,   364,     0,   429,   430,  1034,   519,   518,   618,   516,
     611,   610,   609,   608,   172,   614,   517,   613,   612,   566,
     523,   567,     0,   512,   574,   524,     0,   520,   971,   973,
     972,   513,   975,   974,   514,     0,     0,     0,   865,     0,
     176,     0,   176,     0,   176,     0,     0,     0,   892,     0,
     889,     0,     0,     0,  1041,   422,   903,   904,   897,   899,
       0,   902,   873,     0,     0,   948,   947,   961,   681,   687,
     263,   265,   264,   266,   257,   241,   267,   244,   226,     0,
     177,   395,   672,   673,     0,     0,     0,   325,     0,   332,
     426,   327,   762,     0,   769,     0,     0,   704,   702,   729,
     153,   711,     0,   700,   701,   699,     0,     0,     0,     0,
     870,   995,   998,   375,   847,   379,   378,   384,   964,   970,
     965,   966,   967,   969,   968,     0,   416,     0,     0,     0,
    1025,     0,     0,     0,     0,   407,   410,   571,     0,   413,
       0,  1029,     0,  1007,  1011,     0,     0,  1001,     0,   551,
     552,     0,     0,   484,   481,   483,     0,     0,     0,     0,
       0,     0,     0,   528,   527,   568,   526,   525,     0,     0,
       0,     0,   370,  1041,  1041,     0,    77,     0,     0,   439,
     431,   361,   172,   338,   336,   337,   335,   887,     0,     0,
       0,     0,   553,   554,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     502,     0,     0,     0,     0,   849,   864,     0,     0,   179,
       0,   179,     0,   179,   370,   679,     0,   677,     0,   685,
       0,   850,     0,  1041,     0,   368,   423,   888,  1042,   365,
     894,   872,   875,     0,   854,   682,    95,   688,    95,   268,
     269,   246,   247,   249,   248,   250,   251,   252,   253,   245,
     254,   255,   256,   230,   231,   233,   232,   234,   235,   236,
     237,   228,   229,   238,   239,   240,   227,     0,   393,   394,
       0,   647,   647,   647,   178,   181,   180,     0,   427,   361,
     737,   766,   777,   660,   782,     0,     0,     0,     0,     0,
     718,     0,     0,   885,   911,   914,    18,    17,   868,   869,
       0,     0,     0,     0,   993,     0,     0,     0,  1015,  1018,
    1021,     0,  1041,     0,  1032,  1041,     0,     0,     0,     0,
       0,     0,     0,     0,  1041,     0,     0,  1041,  1004,     0,
       0,     0,     0,     0,   176,     0,     0,     0,     0,     0,
       0,    44,     0,    42,     0,     0,  1014,     0,   691,     0,
     690,     0,     0,  1042,   986,   556,   366,     0,   364,   505,
       0,     0,     0,     0,   176,     0,   439,     0,     0,   582,
     581,     0,     0,   588,   592,   529,   530,   542,   543,   540,
     541,     0,   575,     0,   563,     0,     0,   492,     0,   565,
       0,     0,   576,     0,     0,   587,   586,     0,     0,   593,
     597,     0,     0,   603,   607,   615,   616,   617,   531,   532,
     620,   621,   622,   547,   548,   549,   550,     0,     0,   545,
     546,   544,   538,   539,   534,   533,   535,   536,   537,     0,
       0,     0,   490,     0,     0,     0,     0,     0,   510,     0,
     917,   907,   851,     0,   921,     0,   928,     0,   935,     0,
       0,   941,     0,     0,   944,     0,     0,     0,   878,     0,
       0,   424,   874,   855,   755,    93,    96,   952,    96,   957,
       0,     0,   783,   669,   670,   692,   674,   676,   675,   428,
       0,   733,   738,   755,   663,     0,   706,     0,   707,     0,
       0,     0,   720,   886,   912,   915,   871,     0,     0,     0,
     994,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1042,     0,   569,     0,     0,     0,
     570,     0,   619,     0,     0,     0,     0,     0,     0,     0,
     439,     0,     0,     0,   179,   626,   627,   628,   629,   630,
       0,    38,     0,   111,     0,     0,     0,     0,   977,   976,
     555,     0,     0,     0,     0,  1041,     0,   506,     0,     0,
       0,   509,   507,   173,     0,   179,   363,   387,   385,     0,
       0,     0,     0,   386,     0,     0,     0,    77,     0,   358,
     443,   339,     0,     0,     0,   352,     0,   353,   347,     0,
     344,   343,     0,     0,   345,     0,   362,     0,    91,    92,
      89,    90,   354,   399,   342,     0,   452,   176,   578,     0,
     589,     0,     0,     0,   560,     0,     0,     0,   559,   562,
     583,     0,   594,     0,   604,     0,     0,     0,   598,   602,
       0,     0,   564,     0,     0,     0,     0,   491,     0,   503,
       0,   557,     0,   511,   918,   908,     0,   866,     0,   922,
     924,   929,   931,   936,   938,   678,   942,   680,   684,   945,
     686,   878,   879,   891,   369,   425,     0,   735,   756,   962,
      94,   683,     0,   689,     0,   671,     0,     0,     0,     0,
     693,     0,     0,     0,   756,   763,     0,   661,   778,     0,
     647,   705,     0,     0,   715,     0,     0,   719,   996,   999,
     376,     0,   381,   382,   380,     0,     0,   419,   417,     0,
       0,     0,  1026,  1024,   366,     0,  1033,  1036,   408,   411,
     572,   414,  1030,  1028,  1008,  1012,  1010,     0,  1002,    77,
     482,   646,   485,     0,     0,    39,     0,     0,     0,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1035,
     367,   508,     0,     0,   364,     0,     0,     0,     0,     0,
       0,   437,     0,    77,     0,   388,     0,     0,   373,     0,
       0,   357,     0,     0,    72,     0,     0,   390,   361,   355,
     356,     0,    84,    85,     0,   154,   154,     0,   346,   341,
     348,   349,   350,   351,   398,   647,   340,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   179,     0,     0,     0,     0,
     364,   476,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,     0,     0,     0,   599,     0,   577,     0,   561,
       0,   364,   493,     0,     0,     0,   558,   504,   500,     0,
     853,   867,   852,   925,   932,   939,   893,   856,   857,   736,
       0,   953,   958,     0,   785,     0,   786,   695,   694,   328,
     734,   739,     0,     0,   654,   657,     0,     0,   709,     0,
     717,     0,     0,   377,   383,     0,     0,   418,  1016,  1019,
    1022,     0,     0,     0,     0,     0,     0,     0,     0,   993,
       0,  1005,     0,     0,     0,     0,   488,   632,     0,    36,
      43,     0,   113,     0,   114,     0,     0,   115,     0,     0,
       0,     0,     0,   979,   978,     0,   473,     0,   475,   434,
     435,     0,   433,   432,     0,   440,     0,   389,     0,   374,
       0,     0,    70,    71,   121,   391,     0,     0,     0,     0,
     156,     0,     0,     0,     0,   712,   405,     0,   464,   465,
     467,   466,   468,   458,   459,   460,   469,   470,   454,   455,
     456,   457,   471,   472,   461,   462,   463,   453,    77,     0,
     644,     0,   640,     0,     0,   477,   480,   635,   637,   496,
     639,     0,   645,     0,   641,     0,   643,     0,     0,     0,
     634,     0,     0,   494,   499,     0,   501,     0,   963,   784,
       0,     0,     0,   329,   334,   764,     0,   655,   656,   657,
     658,   649,   664,   708,   716,   993,   993,     0,     0,     0,
       0,     0,   364,  1037,   366,     0,     0,     0,     0,     0,
     994,  1009,   993,     0,   993,   623,     0,   625,   486,     0,
     633,    40,   112,   401,     0,     0,     0,     0,     0,   981,
     980,     0,     0,     0,     0,   438,   441,   392,   128,   127,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,     0,   579,   590,     0,     0,   584,   595,   605,     0,
     642,   638,     0,     0,   636,   858,     0,   788,   696,     0,
       0,   652,   649,   650,   651,   654,   992,   992,   420,     0,
     993,   993,   984,     0,     0,     0,     0,     0,     0,  1041,
     992,   993,   984,   624,   489,    37,     0,     0,   117,   118,
       0,     0,     0,     0,   474,   436,   328,    86,    77,   155,
       0,     0,     0,    77,   702,   406,   713,   714,   442,   580,
     591,   478,   479,   585,   596,   606,   600,   497,   498,   787,
     361,   662,   653,   665,   652,     0,     0,   989,  1041,   991,
       0,     0,     0,   992,   992,   985,     0,  1027,  1038,   409,
     412,   573,   415,     0,     0,   984,     0,  1039,   116,     0,
     983,   982,     0,   360,     0,     0,   108,     0,    77,    77,
       0,     0,     0,   601,     0,     0,   667,   698,   697,   659,
       0,  1042,   990,   997,  1000,   421,     0,     0,  1023,  1031,
    1013,     0,  1003,     0,     0,  1039,     0,    87,    91,    92,
      89,    90,    88,   110,   100,     0,     0,     0,   125,     0,
       0,     0,     0,     0,   987,     0,  1017,  1020,  1006,     0,
    1043,     0,  1039,    97,    79,     0,    77,   123,   126,     0,
       0,   331,   666,    77,     0,  1040,  1044,     0,   361,    77,
      73,    74,     0,   109,     0,     0,     0,   403,     0,   988,
    1045,     0,    80,     0,   101,   120,   402,   668,     0,   105,
       0,   328,   102,    81,     0,    77,    99,   361,     0,    82,
       0,   106,   105,     0,    79,   328,    83,   104
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1685, -1685,  -968,    -1, -1685, -1685, -1685, -1685, -1685,  1008,
    1609, -1685, -1685, -1685, -1685, -1685, -1685,  1694, -1685, -1685,
   -1685,   -32, -1685, -1685,   896, -1685, -1685,  1630, -1685, -1685,
   -1685, -1685,  -143,  -150, -1685, -1685, -1685, -1685, -1684,   918,
     919, -1685, -1685, -1685, -1685,  -144, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1091, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685,  1502, -1685, -1685,   -52,  -101,  -241,   377, -1685,
   -1685,   561,  1037,  1030,   699,  -537,  -741, -1685,  -364, -1685,
   -1685, -1685, -1612, -1685, -1685, -1632, -1685, -1685, -1113, -1685,
   -1685, -1685, -1685, -1685, -1685,  -817,  -383, -1250,   965,   -13,
   -1685, -1685, -1685, -1685, -1685, -1677, -1673, -1667, -1665, -1685,
   -1685,  1745, -1685, -1114, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685,  -521, -1456,   396,
     257, -1685,  -861, -1685,   508, -1685, -1685, -1685, -1685, -1405,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,   670,
     367, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685,  -166,    90,    29,
      91,   178, -1685, -1685, -1685, -1685, -1685, -1685, -1685,   160,
    -573,  -825, -1685,  -582,  -830, -1685,  -999,    33,    34, -1685,
    -622,  -619, -1685, -1685, -1685, -1455, -1685,  1701, -1685, -1685,
   -1685, -1685, -1685,   517,   719, -1685,  1043, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685,   726, -1685,  1410, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685,   -78, -1685,  1276, -1685, -1685, -1685,  1534,
   -1685, -1685, -1685,  -628, -1685, -1685,  -312,  -977, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685,  -190, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685,  -855, -1598,  -689, -1685, -1685,
   -1555, -1414,  1297, -1685, -1685, -1685, -1685, -1685, -1685, -1685,
   -1685, -1685, -1685,  1309, -1685, -1685,  1320, -1685, -1685, -1685,
   -1685, -1685, -1685, -1685, -1685, -1685, -1685,  1113, -1685,  -475,
    1323, -1498,  -677,  1326,  -466
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   858,   859,    18,   145,    54,   198,    19,   179,
     185,  1611,  1296,  1469,   692,   536,   151,   537,   100,    21,
      22,    88,    45,    46,   140,    23,    42,    43,  1142,  1143,
    1832,   159,   160,  1833,  1849,  1862,  1344,  1755,  1144,  1026,
    1027,  1814,  1828,  1848,  1815,  1853,  1857,  1863,  1854,  1145,
    1146,  1794,  1147,  1105,  1148,  1149,  1150,  1151,  1152,  1153,
    1154,  1155,   186,   187,    38,    39,    40,   212,  1511,    67,
      68,    69,    70,   709,    24,   461,   617,   356,   357,   111,
      25,   163,   358,   164,   206,  1574,  1659,  1801,   620,   839,
    1242,   538,  1156,  1338,  1630,   925,   700,  1114,   784,   539,
    1157,   645,   863,  1443,   540,  1158,  1159,  1160,  1161,  1162,
     830,  1163,  1355,  1300,  1516,  1164,   541,   877,  1454,   878,
    1455,   881,  1457,   542,   867,  1447,   543,   584,   621,   544,
    1321,  1322,   924,   545,   713,   546,  1165,   547,   548,   685,
     549,   893,  1465,   894,  1609,   550,   995,  1407,   551,   585,
     553,   879,  1456,  1376,  1709,  1389,  1713,  1378,  1710,  1391,
    1714,  1557,  1763,  1393,  1715,   554,   555,   611,  1665,  1723,
    1579,  1581,  1436,  1044,  1250,  1765,  1803,   612,   613,   614,
     775,   776,   796,   779,   780,   798,   911,  1033,  1034,  1769,
     636,   481,   628,   370,  1639,   629,   371,    79,   118,   210,
     366,    27,   174,  1042,  1228,  1043,    49,    50,   142,    28,
      51,   195,   167,   208,   360,  1229,   300,   301,    29,   112,
     840,  1432,   624,   362,   363,   115,   172,   844,    30,    77,
     209,   625,  1035,   556,   471,   288,   289,  1003,  1024,   200,
     290,   767,  1411,  1012,   639,   401,   291,   580,   292,   482,
    1053,   581,   782,   566,  1205,   483,  1054,   484,  1055,   565,
    1204,   569,  1209,   570,  1413,   571,  1211,   572,  1414,   573,
    1213,   574,  1415,   575,  1216,   576,  1219,   777,    31,    56,
     302,   598,  1232,    32,    57,   303,   599,  1234,    33,    55,
     404,   794,  1420,   647,   557,   704,  1736,   705,  1728,  1729,
    1730,  1063,   558,   861,  1441,   862,  1442,   889,  1462,  1088,
    1603,   885,  1459,   559,   886,  1460,   560,  1067,  1589,  1068,
    1590,  1069,  1591,   871,  1451,   883,  1458,  1115,   706,   561,
     562,  1784,   789,   563,   564
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,    60,    71,   153,   201,   202,   293,   583,   213,   849,
     851,   703,   860,    90,   651,  1303,  1326,   797,   921,    72,
      73,    74,  1040,   654,  1452,   795,   912,   914,  1005,   128,
    1007,   373,  1009,   769,  -174,   771,  1241,   773,   588,   138,
    1626,  1223,   657,   601,   603,  1601,   586,  1301,     2,    92,
      93,    71,    71,    71,   133,     3,  1227,   676,   783,  1089,
    1608,  1086,  1640,  1119,  1753,  1167,  1246,  1479,   828,   618,
    1787,    13,  1622,   211,   139,  1227,  1757,  1788,     4,    41,
       5,  1789,     6,   402,  1746,  1663,    99,  1790,     7,  1791,
     596,   710,    71,    71,    71,    71,    34,    35,     8,   188,
     189,   211,   190,   633,     9,   615,  1017,   105,   106,   107,
      16,   829,  1731,  1307,   609,   211,   626,   634,   287,    44,
     478,   180,   181,   479,  1010,  1744,   480,   943,    10,   485,
     486,  1834,   627,   119,   120,    61,  1514,   944,  1788,   619,
     872,  1515,  1789,   287,   908,  -923,    36,  1781,  1790,   492,
    1791,   884,  1015,  1802,   887,   494,  1019,   533,   534,   367,
     711,   635,   658,   659,    62,   610,   616,   632,  1664,   152,
    1858,  1666,  1667,    11,    12,  1062,  1509,  1788,  1776,  1777,
     908,  1789,    52,   286,  1707,  1708,   920,  1790,  1680,  1791,
    1682,  1823,   501,   502,  1306,  1071,   626,   158,  1075,   778,
     475,    13,   910,   946,  1684,   402,  -923,  1084,   286,   365,
    1087,  -923,   627,   947,    13,   462,  -930,  1485,   589,  1856,
     474,    53,  1504,  -937,   476,  1247,   587,  1050,  1118,  1289,
    -923,    63,   670,  1867,  1302,    78,   135,   590,   910,  1271,
      16,   999,   800,  1270,  1416,  1073,   591,    13,   136,   504,
     505,   660,   677,    16,  1302,  1705,  1733,  1734,    15,  1302,
    1419,   908,  -487,  1302,    64,  1315,  1248,  1745,    14,   971,
      13,   661,   388,    58,    37,   909,  1419,  -930,   972,   908,
      15,   701,  -930,  1316,  -937,   158,    16,  1811,   121,  -937,
     908,  1116,   637,   638,   640,   182,    58,  1317,    59,   391,
     183,  -930,    81,   184,  1480,  1481,   125,   122,  -937,    16,
    1761,   364,   123,   480,  1827,   124,   477,  1383,   125,   910,
    1463,    59,   701,  -487,   287,    65,   392,   393,  -487,  1117,
    1318,    13,  1116,   391,   630,    66,  1238,   910,   643,  1239,
    1404,  1319,  1240,  1061,  1674,   908,  1320,  -487,   910,  1102,
     873,  1607,   126,  1293,   908,    80,   531,  1094,  1476,   909,
     392,   393,   832,   535,  1617,  1103,   890,    99,  1691,  1726,
      16,  1261,   389,   108,  1477,   763,   764,    84,  1206,   768,
      84,   770,  1847,   772,  1323,  1627,  1207,  1125,  1019,   286,
     578,   785,   390,   833,    75,    85,   391,    82,    85,   109,
    1638,   287,  1104,   910,   287,   287,   287,    81,    47,  1208,
     579,  1478,   910,   394,    48,   391,   633,   395,    86,   108,
     132,    86,    76,   392,   393,   835,   836,  1255,  1572,    87,
     634,    83,    87,  1220,   217,   204,  1266,    84,  1310,  1217,
     856,    13,   392,   393,  1031,  1123,  1233,   394,   662,   857,
     633,   395,    91,  1231,   211,    85,   391,  1011,  1120,  1487,
    1032,  1015,   218,   104,   634,    13,   286,   609,   663,   286,
     286,   286,   856,    13,   635,   396,   644,   655,    86,   397,
      16,   857,   398,   392,   393,   402,  1121,   287,   287,    87,
      89,   287,    58,   287,  1202,   287,   684,   287,  1202,  1473,
    1312,   681,  1494,   287,    16,  1312,   682,   399,   635,   396,
     394,  1544,    16,   397,   395,    58,   398,    59,  1312,  1312,
     287,  1506,  1203,  1202,   681,  1543,  1262,  1474,  1488,   394,
    1495,  1313,    58,   395,   683,    98,  1049,   287,   287,  1545,
      59,   399,   847,  1700,  1623,   848,  1561,  1546,   480,  1057,
    1058,  1549,   286,   286,  1562,  1312,   286,    59,   286,  1070,
     286,  1202,   286,   916,  1312,  1077,  1078,  1079,   286,  1081,
     394,  1083,   396,  1085,   395,  1312,   397,   718,   719,   398,
      81,  1093,  1563,  1564,   287,   286,   110,   116,   143,  1566,
    1644,   396,   785,   117,   144,   397,  1202,  1090,   398,    99,
     287,  1652,   286,   286,   399,  1202,  1699,  1473,   718,   719,
     841,   728,   856,    13,   730,   731,   732,   733,   734,   735,
     129,   857,  1692,   399,  1610,   101,   102,   103,  1202,  1014,
    1375,   850,   396,  1614,  1538,  1689,   397,  1661,  1258,   398,
     930,   934,   137,  1015,  1473,   856,    13,  1798,   391,   286,
    1312,  1382,    16,  1312,   857,  1387,  1690,  1580,  1202,   956,
     960,   964,  1018,   967,   399,   286,   154,   155,   156,   157,
    1056,  1312,  1704,  1059,  1403,   392,   393,  1066,  1712,  1409,
     667,  1718,   996,  1817,  1818,    16,  1752,  1047,   131,  1645,
     135,   134,  1202,   760,   761,    71,  1312,   722,   723,  1804,
     668,   671,   136,  1048,   287,   728,   674,  1653,   730,   731,
     732,   733,   734,   735,   856,   737,   738,   739,   740,   741,
    1816,   672,  1845,   857,  1839,   105,   675,   107,   722,   723,
     730,   731,   732,   733,   734,   735,   728,   150,   729,   730,
     731,   732,   733,   734,   735,   736,   737,   738,   739,   740,
     741,   856,    13,   948,   865,  1269,   158,   295,   856,    13,
     857,  1275,   394,   949,   141,  1425,   395,   857,  1221,   286,
     287,   146,   296,  1468,   866,  1265,  1287,   297,   135,   298,
    1475,  1426,   287,   287,   287,   287,  1111,   760,   761,   287,
     136,    16,  1191,   287,  1759,  1025,  1112,  1025,    16,   287,
     287,   287,  1192,   287,   147,   287,  1196,   287,   287,   760,
     761,  1727,  1727,   856,    13,   287,  1197,  1735,   760,   761,
    1215,   148,   857,  1218,   396,  1727,   161,  1735,   397,  1224,
    1072,   398,   162,   718,   719,   286,   813,   814,   815,   816,
     817,   818,   819,   820,   105,   106,   107,   286,   286,   286,
     286,  1052,   701,    16,   286,   821,   399,   165,   286,   113,
     114,   170,  1116,   166,   286,   286,   286,   171,   286,   822,
     286,  1770,   286,   286,   679,   680,  1829,   149,  1727,  1727,
     286,   823,   824,   825,   168,  1577,   169,  1830,  1831,   391,
    1735,  1578,  1529,   693,   694,   695,   696,   697,   199,  1189,
     402,  1530,  -859,  1764,   853,   856,    13,   842,   843,   856,
      13,  1418,   856,   826,   857,   173,   392,   393,   857,   856,
      13,   857,  1285,   402,  1141,   175,  1288,   854,   857,  1041,
     856,   136,   287,   402,   287,   287,  1449,  1000,   402,   857,
     191,   287,  1001,   712,   712,    16,   712,   712,   287,    16,
    1824,   720,   721,   722,   723,   192,   402,   193,  1618,    16,
    1004,   728,   194,   729,   730,   731,   732,   733,   734,   735,
     736,   737,   738,   739,   740,   741,   196,   742,   743,   856,
      13,   197,   402,   402,   199,   838,  1006,  1008,   857,   287,
     287,  1036,  1037,  1038,  1335,   287,  1450,   286,   203,   286,
     286,   402,  1743,   394,   402,  1260,   286,   395,  1268,   105,
     287,  1841,   402,   286,   391,   205,  1643,   402,   402,    16,
     402,  1647,  1648,   207,  1716,  1230,   211,  1230,   755,   756,
     757,   758,   759,   139,   391,   176,   177,   901,   902,  1141,
    1860,   392,   393,   760,   761,   214,   215,   552,  1254,  1427,
    1257,  1772,   391,   221,   286,   286,   714,   577,   715,   716,
     286,   392,   393,   294,  1539,   396,   176,   177,   178,   397,
     593,  1259,   398,  1342,  1343,   286,   299,  1551,  1679,   392,
     393,   214,   215,   216,  1435,  1762,   359,   935,   936,   219,
     220,   937,   938,   939,   940,   361,   942,   399,    95,    96,
      97,  1693,   368,   952,  1510,  1510,   369,   965,   966,   968,
     969,   970,   973,   974,   975,   976,   978,   979,   980,   981,
     982,   983,   984,   985,   986,   987,   988,   374,   394,   375,
     376,   377,   395,  1331,   378,   379,   380,   382,   383,   384,
    1800,  1339,  1340,   385,   391,   387,  1464,   386,   394,   400,
     402,   403,   395,   405,  1348,   458,  1349,  1350,  1351,  1352,
    1353,   287,   459,   568,  1356,   391,   394,   406,   460,   473,
     395,   392,   393,   594,   567,   597,   604,  1819,   605,   606,
    1496,   607,   622,   608,   623,   631,   664,   641,  1541,  1517,
     396,   670,   392,   393,   397,   686,  1272,   398,   687,   642,
     656,  1553,   688,  1555,  1039,   665,   666,   669,   673,  1673,
     396,   391,   678,   717,   397,   708,  1278,   398,   766,   778,
     788,   689,   399,   690,   691,   846,   286,   855,   396,   707,
     762,   868,   397,   790,  1279,   398,  1795,   765,   392,   393,
     781,   787,   399,   801,   802,   803,   804,   805,   806,   807,
     808,   791,   786,   831,   391,  1440,   834,   827,   394,   845,
     399,   864,   395,   869,   793,   870,   888,   891,   892,   904,
     905,   906,   913,   907,   923,   915,   809,   941,  1002,   394,
    1013,   392,   393,   395,  1023,  1041,  1022,  1046,   810,   811,
     812,  1064,  1166,  1074,  1076,  1080,  1082,  1095,   287,  1096,
     287,  1097,  1098,  1099,  1100,  1106,   852,  1110,  1122,  1124,
    1628,   287,  1168,   287,  1170,   287,  1169,  1171,  1172,  1173,
     396,  1180,  1174,  1182,   397,   394,  1280,   398,  1178,   395,
    1175,  1176,  1179,   874,   876,   718,   719,  1177,   880,  1181,
     882,   396,  1183,   391,  1184,   397,  1199,  1281,   398,  1185,
    1194,  1195,   399,  1200,  1201,  1210,   895,   896,   897,   898,
     899,   900,  1212,   286,  1214,   286,  1222,  1649,   394,   579,
     392,   393,   395,   399,  1226,  1236,   286,  1235,   286,  1237,
     286,  1243,  1253,  1251,  1252,  1274,  1660,   396,  1263,  1264,
    1267,   397,   463,  1282,   398,  1641,   464,  1273,  1277,  1283,
     391,   945,  1286,  1290,   950,   951,  1291,  1166,  1633,  1294,
     465,   466,  1295,  1297,  1298,   467,   468,   469,   470,   399,
    1299,  1304,   287,  1305,  1308,  1309,  1311,   392,   393,  1329,
     396,   997,   619,  1330,   397,  1582,  1284,   398,  1584,  1332,
    1341,  1345,  1346,  1347,  1354,  1395,  1396,  1398,  1399,  1799,
    1405,  1406,   684,  1410,  1421,   722,   723,   394,  1021,  1412,
    1422,   395,   399,   728,  1424,   729,   730,   731,   732,   733,
     734,   735,   736,   737,   738,   739,   740,   741,  1429,   287,
    1433,  1434,  1438,  1444,  1446,  1445,  1466,   286,  1472,   391,
    1486,  1470,  1489,  1490,  1491,  1492,  1493,  1030,   287,  1498,
    1500,  1501,  1503,  1507,  1502,  1141,  1508,  1513,  1568,  1540,
    1542,   718,   719,  1547,   394,  1045,   392,   393,   395,   396,
    1567,  1051,  1570,   397,  1548,  1292,   398,  1571,  1573,  1576,
     755,   756,   757,   758,   759,  1550,  1065,  1552,  1554,  1556,
    1560,  1569,  1580,  1583,   286,   760,   761,  1575,  1588,  1585,
    1586,   399,  1592,  1600,  1593,  1756,  1594,  1397,  1595,  1596,
    1760,  1091,  1092,   286,  1597,  1598,  1599,  1613,  1602,  1604,
    1616,  1615,  1101,  1612,  1624,  1621,   396,  1107,  1634,  1108,
     397,  1109,  1448,   398,   391,  1635,   874,  1650,  1651,  1654,
    1657,  1668,  1669,  1670,  1671,  1672,   684,   391,  1681,  1685,
    1683,  1686,  1688,   394,  1702,  1722,  1696,   395,   399,  1694,
    1695,   392,   393,  1754,  1697,  1796,  1797,  1698,  1703,  1711,
    1717,  1719,  1312,  1737,   392,   393,  1720,  1738,  1749,   720,
     721,   722,   723,   724,  1739,  1740,   725,  1741,  1706,   728,
    1805,   729,   730,   731,   732,   733,   734,   735,   736,   737,
     738,   739,   740,   741,   391,   742,   743,  1742,  1748,  1721,
    1193,  1758,  1771,  1783,  1198,   396,  1773,  1774,  1809,   397,
    1775,  1453,   398,  1835,  1778,  1779,  1780,  1782,  1806,  1807,
    1838,   392,   393,   287,  1786,  1808,  1842,  1810,  1813,   127,
    1225,  1821,  1826,  1840,  1843,    20,  1852,   399,   394,  1855,
     903,  1851,   395,  1865,   753,   754,   755,   756,   757,   758,
     759,   394,  1859,   130,  1866,   395,  1028,  1029,  1864,   372,
    1256,   760,   761,  1512,  1518,  1519,  1520,  1521,  1522,  1523,
    1524,  1525,  1526,  1527,  1528,  1531,  1532,  1533,  1534,  1535,
    1536,  1537,   926,  1249,  1276,   922,    26,  1016,   286,   718,
     719,  1625,  1724,  1766,  1793,    94,  1725,  1662,  1767,  1768,
     396,  1431,  1244,  1141,   397,   646,  1461,   398,   394,  1245,
     472,   381,   395,   396,     0,     0,   875,   397,     0,  1642,
     398,     0,     0,     0,     0,     0,   648,     0,     0,     0,
    1021,     0,   399,     0,     0,     0,     0,  1325,   649,     0,
    1328,  1822,     0,     0,     0,   399,  1334,     0,  1337,   650,
       0,     0,   652,     0,     0,   653,     0,     0,     0,  1837,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,  1844,   397,  1846,  1646,   398,     0,  1377,
    1141,  1379,     0,  1384,     0,  1385,     0,  1388,     0,     0,
       0,  1390,     0,  1392,     0,  1394,     0,  1861,     0,  1141,
       0,     0,   399,     0,  1400,     0,     0,     0,     0,   722,
     723,     0,  1021,  1166,     0,     0,     0,   728,     0,   729,
     730,   731,   732,   733,   734,   735,   736,   737,   738,   739,
     740,   741,     0,     0,     0,     0,  1417,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1423,     0,     0,     0,
       0,  1428,     0,  1430,     0,     0,     0,     0,     0,     0,
       0,     0,  1437,     0,     0,  1439,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   874,     0,   757,   758,   759,     0,
       0,     0,     0,     0,     0,     0,     0,   222,     0,   760,
     761,     0,     0,   223,     0,     0,     0,  1471,     0,   224,
       0,     0,     0,     0,     0,  1482,  1483,  1484,     0,   225,
       0,     0,     0,     0,     0,     0,     0,   226,     0,     0,
       0,     0,     0,     0,  1497,     0,     0,  1499,     0,     0,
       0,     0,   227,     0,     0,     0,  1505,     0,     0,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,     0,  1558,     0,  1559,     0,
       0,     0,     0,     0,     0,  1565,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,  1587,     0,     0,     0,     0,
       0,  1792,     0,   284,     0,     0,     0,     0,     0,     0,
       0,  1166,     0,  1606,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1619,  1620,     0,     0,     0,     0,   718,   719,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1629,     0,     0,   285,     0,     0,  1631,  1632,   595,
       0,     0,  1792,  1637,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1166,     0,
       0,     0,     0,     0,  1021,     0,     0,     0,     0,     0,
       0,  1792,     0,     0,     0,     0,     0,  1166,     0,     0,
       0,     0,  1021,     0,     0,     0,     0,  1655,     0,     0,
    1656,     0,  1658,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   874,  1675,  1676,  1677,  1678,     0,
       0,     0,     0,     0,   720,   721,   722,   723,   724,     0,
       0,   725,   726,   727,   728,  1687,   729,   730,   731,   732,
     733,   734,   735,   736,   737,   738,   739,   740,   741,     0,
     742,   743,     0,     0,   744,  1701,     0,     0,   745,   746,
     747,     0,     0,     0,   748,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1732,
       0,   927,     0,     0,   749,  1190,   750,   751,   752,   753,
     754,   755,   756,   757,   758,   759,  1747,     0,     0,     0,
       0,  1750,  1751,     0,     0,     0,   760,   761,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   503,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,  1785,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1812,  1126,     0,     0,
       0,   485,   486,     3,     0,  -122,  -107,  -107,     0,  -119,
    1820,   487,   488,   489,   490,   491,     0,     0,     0,  1825,
       0,   492,  1127,   493,  1128,  1129,     0,   494,     0,  1836,
       0,     0,     0,     0,  1130,   495,  1131,     0,  -124,     0,
    1132,   496,     0,     0,   497,     0,     8,   498,  1133,   928,
    1134,   499,     0,  1850,  1135,  1136,     0,     0,     0,   929,
       0,  1137,     0,     0,   501,   502,     0,   228,   229,   230,
       0,   232,   233,   234,   235,   236,   503,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,     0,   250,
     251,   252,     0,     0,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   504,   505,   506,  1138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   507,   508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   509,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   510,   511,   512,   513,
     514,     0,   515,     0,   516,   517,   518,   519,   520,   521,
     522,   523,   524,    59,     0,    13,   525,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   526,   527,   528,     0,    14,     0,     0,   529,
     530,     0,     0,     0,     0,     0,     0,     0,   531,     0,
     532,     0,   533,   534,    16,  1139,  1140,  1126,     0,     0,
       0,   485,   486,     3,     0,  -122,  -107,  -107,     0,  -119,
       0,   487,   488,   489,   490,   491,     0,     0,     0,     0,
       0,   492,  1127,   493,  1128,  1129,     0,   494,     0,     0,
       0,     0,     0,     0,  1130,   495,  1131,     0,  -124,     0,
    1132,   496,     0,     0,   497,     0,     8,   498,  1133,     0,
    1134,   499,     0,     0,  1135,  1136,     0,     0,     0,     0,
       0,  1137,     0,     0,   501,   502,     0,   228,   229,   230,
       0,   232,   233,   234,   235,   236,   503,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,     0,   250,
     251,   252,     0,     0,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   504,   505,   506,  1138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   507,   508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   509,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   510,   511,   512,   513,
     514,     0,   515,     0,   516,   517,   518,   519,   520,   521,
     522,   523,   524,    59,     0,    13,   525,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   526,   527,   528,     0,    14,     0,     0,   529,
     530,     0,     0,     0,     0,     0,     0,     0,   531,     0,
     532,     0,   533,   534,    16,  1139,  -333,  1126,     0,     0,
       0,   485,   486,     3,     0,  -122,  -107,  -107,     0,  -119,
       0,   487,   488,   489,   490,   491,     0,     0,     0,     0,
       0,   492,  1127,   493,  1128,  1129,     0,   494,     0,     0,
       0,     0,     0,     0,  1130,   495,  1131,     0,  -124,     0,
    1132,   496,     0,     0,   497,     0,     8,   498,  1133,     0,
    1134,   499,     0,     0,  1135,  1136,     0,     0,     0,     0,
       0,  1137,     0,     0,   501,   502,     0,   228,   229,   230,
       0,   232,   233,   234,   235,   236,   503,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,     0,   250,
     251,   252,     0,     0,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   504,   505,   506,  1138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   507,   508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   509,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   510,   511,   512,   513,
     514,     0,   515,     0,   516,   517,   518,   519,   520,   521,
     522,   523,   524,    59,     0,    13,   525,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   526,   527,   528,     0,    14,     0,     0,   529,
     530,     0,     0,     0,     0,     0,     0,     0,   531,     0,
     532,     0,   533,   534,    16,  1139,  -359,  1126,     0,     0,
       0,   485,   486,     3,     0,  -122,  -107,  -107,     0,  -119,
       0,   487,   488,   489,   490,   491,     0,     0,     0,     0,
       0,   492,  1127,   493,  1128,  1129,     0,   494,     0,     0,
       0,     0,     0,     0,  1130,   495,  1131,     0,  -124,     0,
    1132,   496,     0,     0,   497,     0,     8,   498,  1133,     0,
    1134,   499,     0,     0,  1135,  1136,     0,     0,     0,     0,
       0,  1137,     0,     0,   501,   502,     0,   228,   229,   230,
       0,   232,   233,   234,   235,   236,   503,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,     0,   250,
     251,   252,     0,     0,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   504,   505,   506,  1138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   507,   508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   509,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   510,   511,   512,   513,
     514,     0,   515,     0,   516,   517,   518,   519,   520,   521,
     522,   523,   524,    59,     0,    13,   525,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   526,   527,   528,     0,    14,     0,     0,   529,
     530,     0,     0,     0,     0,     0,     0,     0,   531,     0,
     532,     0,   533,   534,    16,  1139,  -330,  1126,     0,     0,
       0,   485,   486,     3,     0,  -122,  -107,  -107,     0,  -119,
       0,   487,   488,   489,   490,   491,     0,     0,     0,     0,
       0,   492,  1127,   493,  1128,  1129,     0,   494,     0,     0,
       0,     0,     0,     0,  1130,   495,  1131,     0,  -124,     0,
    1132,   496,     0,     0,   497,     0,     8,   498,  1133,     0,
    1134,   499,     0,     0,  1135,  1136,     0,     0,     0,     0,
       0,  1137,     0,     0,   501,   502,     0,   228,   229,   230,
       0,   232,   233,   234,   235,   236,   503,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,     0,   250,
     251,   252,     0,     0,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   504,   505,   506,  1138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   507,   508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   509,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   510,   511,   512,   513,
     514,     0,   515,     0,   516,   517,   518,   519,   520,   521,
     522,   523,   524,    59,     0,    13,   525,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   526,   527,   528,     0,    14,     0,     0,   529,
     530,     0,     0,     0,     0,     0,     0,     0,   531,     0,
     532,     0,   533,   534,    16,  1139,   -98,  1126,     0,     0,
       0,   485,   486,     3,     0,  -122,  -107,  -107,     0,  -119,
       0,   487,   488,   489,   490,   491,     0,     0,     0,     0,
       0,   492,  1127,   493,  1128,  1129,     0,   494,     0,     0,
       0,     0,     0,     0,  1130,   495,  1131,     0,  -124,     0,
    1132,   496,     0,     0,   497,     0,     8,   498,  1133,     0,
    1134,   499,     0,     0,  1135,  1136,     0,     0,     0,     0,
       0,  1137,     0,     0,   501,   502,     0,   228,   229,   230,
       0,   232,   233,   234,   235,   236,   503,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,     0,   250,
     251,   252,     0,     0,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   504,   505,   506,  1138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   507,   508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   509,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   510,   511,   512,   513,
     514,     0,   515,     0,   516,   517,   518,   519,   520,   521,
     522,   523,   524,    59,     0,    13,   525,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   526,   527,   528,     0,    14,     0,     0,   529,
     530,     0,     0,     0,     0,     0,     0,     0,   531,     0,
     532,     0,   533,   534,    16,  1139,  -103,   485,   486,     0,
       0,     0,     0,     0,     0,     0,     0,   487,   488,   489,
     490,   491,     0,     0,     0,     0,     0,   492,     0,   493,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,   496,     0,     0,
     497,     0,     0,   498,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   500,     0,     0,
     501,   502,   917,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   503,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   504,   505,   506,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,     0,     0,     0,     0,     0,
       0,     0,   582,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,   513,   514,     0,   515,   701,
     516,   517,   518,   519,   520,   521,   522,   523,   524,   702,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,     0,    14,     0,     0,   529,   530,     0,     0,     0,
       0,     0,   485,   486,   918,     0,   532,   919,   533,   534,
     698,   535,   487,   488,   489,   490,   491,     0,     0,     0,
       0,     0,   492,     0,   493,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   496,     0,     0,   497,   699,     0,   498,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   500,     0,     0,   501,   502,     0,   228,   229,
     230,     0,   232,   233,   234,   235,   236,   503,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,     0,
     250,   251,   252,     0,     0,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   504,   505,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,   582,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   509,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   510,   511,   512,
     513,   514,     0,   515,   701,   516,   517,   518,   519,   520,
     521,   522,   523,   524,   702,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,     0,    14,     0,     0,
     529,   530,     0,     0,     0,     0,     0,   485,   486,   531,
       0,   532,     0,   533,   534,   698,   535,   487,   488,   489,
     490,   491,     0,     0,     0,     0,     0,   492,     0,   493,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,   496,     0,     0,
     497,   699,     0,   498,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   500,     0,     0,
     501,   502,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   503,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   504,   505,   506,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,     0,     0,     0,     0,     0,
       0,     0,   582,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,   513,   514,     0,   515,     0,
     516,   517,   518,   519,   520,   521,   522,   523,   524,    59,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,     0,    14,     0,     0,   529,   530,     0,     0,     0,
       0,     0,   485,   486,   531,     0,   532,     0,   533,   534,
       0,   535,   487,   488,   489,   490,   491,     0,     0,     0,
       0,     0,   492,     0,   493,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   496,     0,     0,   497,     0,     0,   498,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   500,     0,     0,   501,   502,  1060,   228,   229,
     230,     0,   232,   233,   234,   235,   236,   503,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,     0,
     250,   251,   252,     0,     0,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   504,   505,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,   582,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   509,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   510,   511,   512,
     513,   514,     0,   515,   701,   516,   517,   518,   519,   520,
     521,   522,   523,   524,   702,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,     0,    14,     0,     0,
     529,   530,     0,     0,     0,     0,     0,   485,   486,   531,
       0,   532,     0,   533,   534,     0,   535,   487,   488,   489,
     490,   491,     0,     0,     0,     0,     0,   492,     0,   493,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,   496,     0,     0,
     497,     0,     0,   498,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   500,     0,     0,
     501,   502,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   503,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   504,   505,   506,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,     0,     0,     0,     0,     0,
       0,     0,   582,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,   513,   514,     0,   515,   701,
     516,   517,   518,   519,   520,   521,   522,   523,   524,   702,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,     0,    14,     0,     0,   529,   530,     0,     0,     0,
       0,     0,   485,   486,  1380,     0,   532,  1381,   533,   534,
       0,   535,   487,   488,   489,   490,   491,     0,     0,     0,
       0,     0,   492,     0,   493,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   496,     0,     0,   497,     0,     0,   498,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   500,     0,     0,   501,   502,     0,   228,   229,
     230,     0,   232,   233,   234,   235,   236,   503,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,     0,
     250,   251,   252,     0,     0,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   504,   505,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,   582,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   509,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   510,   511,   512,
     513,   514,     0,   515,   701,   516,   517,   518,   519,   520,
     521,   522,   523,   524,   702,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,     0,    14,     0,     0,
     529,   530,     0,     0,     0,     0,     0,   485,   486,  1401,
       0,   532,  1402,   533,   534,     0,   535,   487,   488,   489,
     490,   491,     0,     0,     0,     0,     0,   492,     0,   493,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,   496,     0,     0,
     497,     0,     0,   498,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   500,     0,     0,
     501,   502,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   503,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   504,   505,   506,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,     0,     0,     0,     0,     0,
       0,     0,   582,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,   513,   514,     0,   515,   701,
     516,   517,   518,   519,   520,   521,   522,   523,   524,   702,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,     0,    14,     0,     0,   529,   530,     0,     0,     0,
       0,     0,   485,   486,   531,     0,   532,     0,   533,   534,
       0,   535,   487,   488,   489,   490,   491,     0,     0,     0,
       0,     0,   492,     0,   493,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   496,     0,     0,   497,     0,     0,   498,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   500,     0,     0,   501,   502,     0,   228,   229,
     230,     0,   232,   233,   234,   235,   236,   503,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,     0,
     250,   251,   252,     0,     0,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   504,   505,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,   582,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   509,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   510,   511,   512,
     513,   514,     0,   515,     0,   516,   517,   518,   519,   520,
     521,   522,   523,   524,    59,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,     0,    14,     0,     0,
     529,   530,     0,     0,     0,     0,     0,   485,   486,   531,
       0,   532,   998,   533,   534,     0,   535,   487,   488,   489,
     490,   491,     0,     0,     0,     0,     0,   492,     0,   493,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,   496,     0,     0,
     497,     0,     0,   498,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   500,     0,     0,
     501,   502,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   503,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   504,   505,   506,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,     0,     0,     0,     0,     0,
       0,     0,  1020,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,   513,   514,     0,   515,   701,
     516,   517,   518,   519,   520,   521,   522,   523,   524,   702,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,     0,    14,     0,     0,   529,   530,     0,     0,     0,
       0,     0,   485,   486,  1314,     0,   532,     0,   533,   534,
       0,   535,   487,   488,   489,   490,   491,     0,     0,     0,
       0,     0,   492,     0,   493,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   496,     0,     0,   497,     0,     0,   498,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   500,     0,     0,   501,   502,     0,   228,   229,
     230,     0,   232,   233,   234,   235,   236,   503,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,     0,
     250,   251,   252,     0,     0,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   504,   505,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,   582,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   509,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   510,   511,   512,
     513,   514,     0,   515,     0,   516,   517,   518,   519,   520,
     521,   522,   523,   524,    59,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,     0,    14,     0,     0,
     529,   530,     0,     0,     0,     0,     0,   485,   486,   531,
       0,   532,  1386,   533,   534,     0,   535,   487,   488,   489,
     490,   491,     0,     0,     0,     0,     0,   492,     0,   493,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,   496,     0,     0,
     497,     0,     0,   498,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   500,     0,     0,
     501,   502,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   503,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   504,   505,   506,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,     0,     0,     0,     0,     0,
       0,     0,   582,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,   513,   514,     0,   515,     0,
     516,   517,   518,   519,   520,   521,   522,   523,   524,    59,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,     0,    14,     0,     0,   529,   530,     0,     0,     0,
       0,     0,   485,   486,   531,     0,   532,  1408,   533,   534,
       0,   535,   487,   488,   489,   490,   491,     0,     0,     0,
       0,     0,   492,     0,   493,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   496,     0,     0,   497,     0,     0,   498,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   500,     0,     0,   501,   502,     0,   228,   229,
     230,     0,   232,   233,   234,   235,   236,   503,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,     0,
     250,   251,   252,     0,     0,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   504,   505,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,   582,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   509,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   510,   511,   512,
     513,   514,     0,   515,     0,   516,   517,   518,   519,   520,
     521,   522,   523,   524,    59,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,     0,    14,     0,     0,
     529,   530,     0,     0,     0,     0,     0,   485,   486,   531,
       0,   532,  1467,   533,   534,     0,   535,   487,   488,   489,
     490,   491,     0,     0,     0,     0,     0,   492,     0,   493,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,   496,     0,     0,
     497,     0,     0,   498,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   500,     0,     0,
     501,   502,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   503,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   504,   505,   506,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,     0,     0,     0,     0,     0,
       0,     0,  1020,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,   513,   514,     0,   515,   701,
     516,   517,   518,   519,   520,   521,   522,   523,   524,   702,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,     0,    14,     0,     0,   529,   530,     0,     0,     0,
       0,     0,   485,   486,   531,     0,   532,     0,   533,   534,
       0,   535,   487,   488,   489,   490,   491,     0,     0,     0,
       0,     0,   492,     0,   493,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   496,     0,     0,   497,     0,     0,   498,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   500,     0,     0,   501,   502,     0,   228,   229,
     230,     0,   232,   233,   234,   235,   236,   503,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,     0,
     250,   251,   252,     0,     0,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   504,   505,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,   582,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   509,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   510,   511,   512,
     513,   514,     0,   515,     0,   516,   517,   518,   519,   520,
     521,   522,   523,   524,    59,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,     0,    14,     0,     0,
     529,   530,     0,     0,     0,     0,     0,   485,   486,   531,
       0,   532,     0,   533,   534,     0,   535,   487,   488,   489,
     490,   491,     0,     0,     0,     0,     0,   492,     0,   493,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,   496,     0,     0,
     497,     0,     0,   498,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   500,     0,     0,
     501,   502,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   503,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   504,   505,   506,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,   513,   514,     0,   515,     0,
     516,   517,   518,   519,   520,   521,   522,   523,   524,    59,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,     0,    14,     0,     0,   529,   530,     0,     0,     0,
       0,     0,   485,   486,   531,   592,   532,     0,   533,   534,
       0,   535,   487,   488,   489,   490,   491,     0,     0,     0,
       0,     0,   492,     0,   493,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   496,     0,     0,   497,     0,     0,   498,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   500,     0,     0,   501,   502,     0,   228,   229,
     230,     0,   232,   233,   234,   235,   236,   503,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,     0,
     250,   251,   252,     0,     0,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   504,   505,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   509,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   510,   511,   512,
     513,   514,     0,   515,     0,   516,   517,   518,   519,   520,
     521,   522,   523,   524,    59,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,     0,    14,     0,     0,
     529,   530,     0,     0,     0,     0,     0,   485,   486,   531,
     792,   532,     0,   533,   534,     0,   535,   487,   488,   489,
     490,   491,     0,     0,     0,     0,     0,   492,     0,   493,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,   496,     0,     0,
     497,     0,     0,   498,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   500,     0,     0,
     501,   502,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   503,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   504,   505,   506,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,     0,     0,     0,     0,     0,
       0,     0,  1020,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,   513,   514,     0,   515,     0,
     516,   517,   518,   519,   520,   521,   522,   523,   524,    59,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,     0,    14,     0,     0,   529,   530,     0,     0,     0,
       0,     0,   485,   486,   531,     0,   532,     0,   533,   534,
    1113,   535,   487,   488,   489,   490,   491,     0,     0,     0,
       0,     0,   492,     0,   493,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   496,     0,     0,   497,     0,     0,   498,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   500,     0,     0,   501,   502,     0,   228,   229,
     230,     0,   232,   233,   234,   235,   236,   503,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,     0,
     250,   251,   252,     0,     0,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   504,   505,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   509,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   510,   511,   512,
     513,   514,     0,   515,     0,   516,   517,   518,   519,   520,
     521,   522,   523,   524,    59,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,     0,    14,     0,     0,
     529,   530,     0,     0,     0,     0,     0,   485,   486,   531,
       0,   532,     0,   533,   534,     0,   535,   487,   488,   489,
     490,   491,     0,     0,     0,     0,     0,   492,     0,   493,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,   496,     0,     0,
     497,     0,     0,   498,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   500,     0,     0,
     501,   502,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   503,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   504,   505,   506,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,     0,     0,     0,     0,     0,
       0,     0,  1324,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,   513,   514,     0,   515,     0,
     516,   517,   518,   519,   520,   521,   522,   523,   524,    59,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,     0,    14,     0,     0,   529,   530,     0,     0,     0,
       0,     0,   485,   486,   531,     0,   532,     0,   533,   534,
       0,   535,   487,   488,   489,   490,   491,     0,     0,     0,
       0,     0,   492,     0,   493,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   496,     0,     0,   497,     0,     0,   498,     0,
       0,     0,   499,     0,     0,     0,     0,     0,  1327,     0,
       0,     0,   500,     0,     0,   501,   502,     0,   228,   229,
     230,     0,   232,   233,   234,   235,   236,   503,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,     0,
     250,   251,   252,     0,     0,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   504,   505,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   509,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   510,   511,   512,
     513,   514,     0,   515,     0,   516,   517,   518,   519,   520,
     521,   522,   523,   524,    59,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,     0,    14,     0,     0,
     529,   530,     0,     0,     0,     0,     0,   485,   486,   531,
       0,   532,     0,   533,   534,     0,   535,   487,   488,   489,
     490,   491,     0,     0,     0,     0,     0,   492,     0,   493,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,   496,     0,     0,
     497,     0,     0,   498,     0,     0,     0,   499,     0,     0,
    1333,     0,     0,     0,     0,     0,     0,   500,     0,     0,
     501,   502,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   503,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   504,   505,   506,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,   513,   514,     0,   515,     0,
     516,   517,   518,   519,   520,   521,   522,   523,   524,    59,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,     0,    14,     0,     0,   529,   530,     0,     0,     0,
       0,     0,   485,   486,   531,     0,   532,     0,   533,   534,
       0,   535,   487,   488,   489,   490,   491,     0,     0,     0,
       0,     0,   492,     0,   493,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   496,     0,     0,   497,     0,     0,   498,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   500,     0,     0,   501,   502,     0,   228,   229,
     230,     0,   232,   233,   234,   235,   236,   503,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,     0,
     250,   251,   252,     0,     0,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   504,   505,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,  1336,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   509,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   510,   511,   512,
     513,   514,     0,   515,     0,   516,   517,   518,   519,   520,
     521,   522,   523,   524,    59,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,     0,    14,     0,     0,
     529,   530,     0,     0,     0,     0,     0,   485,   486,   531,
       0,   532,     0,   533,   534,     0,   535,   487,   488,   489,
     490,   491,     0,     0,     0,     0,     0,   492,     0,   493,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,   496,     0,     0,
     497,     0,     0,   498,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   500,     0,     0,
     501,   502,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   503,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   504,   505,   506,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,   513,   514,     0,   515,     0,
     516,   517,   518,   519,   520,   521,   522,   523,   524,    59,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,     0,    14,     0,     0,   529,   530,     0,     0,     0,
       0,     0,   485,   486,   531,     0,   532,  1605,   533,   534,
       0,   535,   487,   488,   489,   490,   491,     0,     0,     0,
       0,     0,   492,     0,   493,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   496,     0,     0,   497,     0,     0,   498,     0,
       0,     0,   499,  1636,     0,     0,     0,     0,     0,     0,
       0,     0,   500,     0,     0,   501,   502,     0,   228,   229,
     230,     0,   232,   233,   234,   235,   236,   503,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,     0,
     250,   251,   252,     0,     0,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   504,   505,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   509,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   510,   511,   512,
     513,   514,     0,   515,     0,   516,   517,   518,   519,   520,
     521,   522,   523,   524,    59,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,     0,    14,     0,     0,
     529,   530,     0,     0,     0,     0,     0,   485,   486,   531,
       0,   532,     0,   533,   534,     0,   535,   487,   488,   489,
     490,   491,     0,     0,     0,     0,     0,   492,     0,   493,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,   496,     0,     0,
     497,     0,     0,   498,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   500,     0,     0,
     501,   502,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   503,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   504,   505,   506,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,   513,   514,     0,   515,     0,
     516,   517,   518,   519,   520,   521,   522,   523,   524,    59,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,     0,    14,     0,     0,   529,   530,     0,     0,     0,
       0,     0,   485,   486,   531,     0,   532,     0,   533,   534,
       0,   535,   487,   488,   489,   490,   491,     0,     0,     0,
       0,     0,   492,  1127,   493,  1128,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   496,     0,     0,   497,     0,     0,   498,  1133,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   500,     0,     0,   501,   502,     0,   228,   229,
     230,     0,   232,   233,   234,   235,   236,   503,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,     0,
     250,   251,   252,     0,     0,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   504,   505,   506,  1138,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   509,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   510,   511,   512,
     513,   514,     0,   515,     0,   516,   517,   518,   519,   520,
     521,   522,   523,   524,    59,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,     0,    14,     0,     0,
     529,   530,     0,     0,     0,   485,   486,     0,     0,   531,
       0,   532,     0,   533,   534,   487,   488,   489,   490,   491,
       0,     0,     0,     0,     0,   492,     0,   493,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,   495,
       0,     0,     0,     0,     0,   496,     0,     0,   497,     0,
       0,   498,     0,     0,     0,   499,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   500,     0,     0,   501,   502,
       0,   228,   229,   230,     0,   232,   233,   234,   235,   236,
     503,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,     0,   250,   251,   252,     0,     0,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   504,   505,   506,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   507,   508,     0,     0,     0,     0,     0,     0,     0,
     837,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     509,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     510,   511,   512,   513,   514,     0,   515,     0,   516,   517,
     518,   519,   520,   521,   522,   523,   524,    59,     0,     0,
     525,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   526,   527,   528,     0,
      14,     0,     0,   529,   530,     0,     0,     0,   485,   486,
       0,     0,   531,     0,   532,     0,   533,   534,   487,   488,
     489,   490,   491,     0,     0,   977,     0,     0,   492,     0,
     493,     0,     0,     0,   494,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,     0,     0,     0,   496,     0,
       0,   497,     0,     0,   498,     0,     0,     0,   499,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   500,     0,
       0,   501,   502,     0,   228,   229,   230,     0,   232,   233,
     234,   235,   236,   503,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,     0,   250,   251,   252,     0,
       0,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   504,   505,
     506,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   509,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   510,   511,   512,   513,   514,     0,   515,
       0,   516,   517,   518,   519,   520,   521,   522,   523,   524,
      59,     0,     0,   525,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   526,
     527,   528,     0,    14,     0,     0,   529,   530,     0,     0,
       0,   485,   486,     0,     0,   531,     0,   532,     0,   533,
     534,   487,   488,   489,   490,   491,     0,     0,     0,     0,
       0,   492,     0,   493,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,   495,     0,     0,     0,     0,
       0,   496,     0,     0,   497,     0,     0,   498,     0,     0,
       0,   499,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   500,     0,     0,   501,   502,     0,   228,   229,   230,
       0,   232,   233,   234,   235,   236,   503,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,     0,   250,
     251,   252,     0,     0,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   504,   505,   506,   -88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   718,   719,   507,   508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   509,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   510,   511,   512,   513,
     514,     0,   515,     0,   516,   517,   518,   519,   520,   521,
     522,   523,   524,    59,     0,     0,   525,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   526,   527,   528,     0,    14,     0,     0,   529,
     530,     0,     0,     0,     0,     0,     0,     0,   531,     0,
     532,     0,   533,   534,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1357,  1358,  1359,  1360,  1361,
    1362,  1363,  1364,   720,   721,   722,   723,   724,  1365,  1366,
     725,   726,   727,   728,  1367,   729,   730,   731,   732,   733,
     734,   735,   736,   737,   738,   739,   740,   741,  1368,   742,
     743,  1369,  1370,   744,     0,     0,     0,   745,   746,   747,
    1371,  1372,  1373,   748,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1374,   749,     0,   750,   751,   752,   753,   754,
     755,   756,   757,   758,   759,   222,     0,     0,     0,     0,
       0,   223,     0,     0,     0,   760,   761,   224,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   225,     0,     0,
       0,     0,     0,     0,     0,   226,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     227,     0,     0,     0,     0,     0,     0,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   284,     0,     0,   222,     0,     0,     0,     0,     0,
     223,     0,     0,   774,     0,    13,   224,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   225,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   227,
       0,     0,   285,     0,    16,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     284,   222,     0,     0,   407,   408,     0,   223,     0,     0,
       0,     0,    59,   224,     0,     0,     0,     0,     0,     0,
       0,   409,     0,   225,     0,     0,     0,     0,     0,     0,
       0,   226,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   227,     0,     0,     0,
       0,   285,     0,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   718,
     719,     0,     0,     0,   410,   411,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   424,   425,
     426,   427,     0,     0,   428,   429,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   431,   432,   433,
     434,   435,     0,     0,    58,     0,   436,   437,   438,   439,
     440,   441,   442,     0,     0,     0,     0,   284,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   774,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   443,     0,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   718,   719,   454,   455,     0,     0,
       0,     0,     0,     0,   456,   457,     0,     0,   285,     0,
       0,     0,     0,     0,     0,     0,     0,   720,   721,   722,
     723,   724,     0,     0,   725,   726,   727,   728,     0,   729,
     730,   731,   732,   733,   734,   735,   736,   737,   738,   739,
     740,   741,     0,   742,   743,     0,     0,   744,     0,     0,
       0,   745,   746,   747,     0,     0,     0,   748,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   718,   719,     0,     0,     0,     0,   749,     0,   750,
     751,   752,   753,   754,   755,   756,   757,   758,   759,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   760,
     761,     0,   720,   721,   722,   723,   724,     0,     0,   725,
     726,   727,   728,     0,   729,   730,   731,   732,   733,   734,
     735,   736,   737,   738,   739,   740,   741,     0,   742,   743,
       0,     0,  -938,     0,     0,     0,   745,   746,   747,     0,
       0,     0,  -938,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   718,   719,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   749,     0,   750,   751,   752,   753,   754,   755,
     756,   757,   758,   759,     0,     0,     0,     0,     0,   720,
     721,   722,   723,   724,   760,   761,   725,   726,   727,   728,
       0,   729,   730,   731,   732,   733,   734,   735,   736,   737,
     738,   739,   740,   741,     0,   742,   743,     0,     0,     0,
       0,     0,     0,   745,   746,   747,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   718,   719,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   749,
       0,   750,   751,   752,   753,   754,   755,   756,   757,   758,
     759,     0,     0,   718,   719,     0,   720,   721,   722,   723,
     724,   760,   761,   725,   726,   727,   728,     0,   729,   730,
     731,   732,   733,   734,   735,   736,   737,   738,   739,   740,
     741,     0,   742,   743,     0,     0,     0,     0,     0,     0,
     745,     0,   747,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   750,   751,
     752,   753,   754,   755,   756,   757,   758,   759,     0,     0,
     718,   719,     0,   720,   721,   722,   723,   724,   760,   761,
     725,   726,   727,   728,     0,   729,   730,   731,   732,   733,
     734,   735,   736,   737,   738,   739,   740,   741,     0,   742,
     743,   720,   721,   722,   723,   724,     0,   745,   725,   726,
     727,   728,     0,   729,   730,   731,   732,   733,   734,   735,
     736,   737,   738,   739,   740,   741,     0,   742,   743,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   750,   751,   752,   753,   754,
     755,   756,   757,   758,   759,     0,     0,   718,   719,     0,
       0,     0,     0,     0,     0,   760,   761,     0,     0,     0,
       0,     0,     0,   750,   751,   752,   753,   754,   755,   756,
     757,   758,   759,     0,     0,   718,   719,     0,   720,   721,
     722,   723,   724,   760,   761,   725,   726,   727,   728,     0,
     729,   730,   731,   732,   733,   734,   735,   736,   737,   738,
     739,   740,   741,     0,   742,   743,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   751,   752,   753,   754,   755,   756,   757,   758,   759,
       0,     0,     0,     0,     0,   720,   721,   722,   723,   724,
     760,   761,   725,   726,   727,   728,     0,   729,   730,   731,
     732,   733,   734,   735,   736,   737,   738,   739,   740,   741,
       0,   742,   743,   720,   721,   722,   723,   724,     0,     0,
     725,   726,   727,   728,     0,   729,   730,   731,   732,   733,
     734,   735,   736,   737,   738,   739,   740,   741,     0,   742,
     743,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   931,     0,     0,     0,   752,
     753,   754,   755,   756,   757,   758,   759,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   760,   761,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   753,   754,
     755,   756,   757,   758,   759,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   760,   761,   228,   229,   230,
     953,   232,   233,   234,   235,   236,   503,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,     0,   250,
     251,   252,     0,     0,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,     0,   228,   229,   230,     0,   232,   233,   234,   235,
     236,   503,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,     0,   250,   251,   252,     0,     0,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,     0,     0,     0,     0,
       0,     0,     0,   932,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   933,     0,     0,     0,   957,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   954,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   955,   228,
     229,   230,   961,   232,   233,   234,   235,   236,   503,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
       0,   250,   251,   252,     0,     0,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,     0,   228,   229,   230,     0,   232,   233,
     234,   235,   236,   503,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,     0,   250,   251,   252,     0,
       0,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,     0,     0,
       0,     0,     0,     0,     0,   958,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   959,     0,     0,     0,  1186,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     962,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     963,   228,   229,   230,     0,   232,   233,   234,   235,   236,
     503,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   304,   250,   251,   252,     0,     0,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,     0,     0,     0,   305,     0,
     306,     0,   307,   308,   309,   310,   311,     0,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,     0,
     323,   324,   325,     0,     0,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,     0,     0,     0,     0,  1187,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1188,   353,   354,
     228,   229,   230,     0,   232,   233,   234,   235,   236,   503,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,     0,   250,   251,   252,     0,     0,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   355,     0,   305,     0,   306,     0,
     307,   308,   309,   310,   311,     0,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,     0,   323,   324,
     325,   989,   990,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
       0,     0,     0,     0,     0,     0,   991,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   992,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     993,   994,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   305,     0,   306,     0,   307,   308,   309,
     310,   311,   600,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,     0,   323,   324,   325,     0,     0,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   305,     0,   306,
       0,   307,   308,   309,   310,   311,     0,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,     0,   323,
     324,   325,     0,     0,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   602,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   799
};

static const yytype_int16 yycheck[] =
{
       1,    14,    15,   104,   147,   148,   196,   390,   174,   628,
     632,   532,   640,    45,   489,  1106,  1129,   599,   707,    20,
      21,    22,   839,   489,  1274,   598,   703,   704,   769,    81,
     771,   221,   773,   570,     8,   572,  1035,   574,    33,    22,
    1496,  1018,     5,   407,   408,  1459,    33,    20,     0,    50,
      51,    64,    65,    66,    86,     7,  1024,    19,   579,   889,
    1465,   886,  1517,   918,  1696,   926,    40,    20,     7,   169,
    1754,   199,    20,   171,    57,  1043,    20,  1754,    30,   189,
      32,  1754,    34,   211,  1682,    46,   172,  1754,    40,  1754,
     402,   170,   105,   106,   107,   108,    19,    20,    50,   131,
     132,   171,   134,   150,    56,   149,   783,   173,   174,   175,
     238,    50,  1667,  1112,   184,   171,   181,   164,   196,   209,
     207,    15,    16,   210,   210,  1680,   213,   187,    80,     5,
       6,  1815,   197,    15,    16,    34,   234,   197,  1815,   239,
     661,   239,  1815,   221,   150,   149,    69,  1745,  1815,    25,
    1815,   672,   780,  1765,   675,    31,   784,   236,   237,   211,
     239,   208,   125,   126,    63,   235,   210,   479,   129,   235,
    1854,  1585,  1586,   125,   126,   864,   232,  1854,  1733,  1734,
     150,  1854,   197,   196,  1639,  1641,   707,  1854,  1602,  1854,
    1604,  1803,    68,    69,   164,   872,   181,   199,   875,   197,
     366,   199,   208,   187,  1609,   211,   210,   884,   221,   210,
     887,   215,   197,   197,   199,   358,   149,  1308,   213,  1851,
     363,   236,  1335,   149,   367,   199,   213,   849,   917,  1090,
     234,   130,   234,  1865,   207,    62,   219,   232,   208,  1069,
     238,   762,   606,  1068,  1221,   873,   241,   199,   231,   125,
     126,   214,   214,   238,   207,   240,  1670,  1671,   232,   207,
    1228,   150,   149,   207,   163,  1120,   240,  1681,   220,   744,
     199,   234,   285,   172,   197,   164,  1244,   210,   744,   150,
     232,   187,   215,   150,   210,   199,   238,  1785,   170,   215,
     150,   197,   482,   483,   484,   189,   172,   164,   197,    33,
     194,   234,   207,   197,   164,  1304,   200,   189,   234,   238,
     210,   240,   194,   213,  1812,   197,   368,  1172,   200,   208,
     234,   197,   187,   210,   402,   224,    60,    61,   215,   235,
     197,   199,   197,    33,   477,   234,   207,   208,   214,   210,
    1195,   208,   213,   864,  1594,   150,   213,   234,   208,   181,
     662,  1464,   234,  1094,   150,   208,   232,   894,   181,   164,
      60,    61,   207,   239,  1478,   197,   678,   172,   164,   234,
     238,  1060,   214,   207,   197,   565,   566,   179,   207,   569,
     179,   571,  1838,   573,  1125,  1498,   215,   924,  1016,   402,
     214,   581,   234,   238,     8,   197,    33,   210,   197,   233,
    1514,   479,   234,   208,   482,   483,   484,   207,    57,   238,
     234,   234,   208,   147,    63,    33,   150,   151,   220,   207,
     219,   220,    36,    60,    61,   615,   616,  1049,  1427,   231,
     164,   207,   231,  1015,   207,   235,  1064,   179,  1115,  1012,
     198,   199,    60,    61,   181,   233,  1028,   147,   214,   207,
     150,   151,   197,  1026,   171,   197,    33,   215,   207,  1314,
     197,  1089,   235,   234,   164,   199,   479,   184,   234,   482,
     483,   484,   198,   199,   208,   209,   489,   490,   220,   213,
     238,   207,   216,    60,    61,   211,   235,   565,   566,   231,
     232,   569,   172,   571,   207,   573,   509,   575,   207,   207,
     207,   181,   207,   581,   238,   207,   186,   241,   208,   209,
     147,   207,   238,   213,   151,   172,   216,   197,   207,   207,
     598,  1338,   235,   207,   181,  1380,   235,   235,   235,   147,
     235,   233,   172,   151,   214,   197,   848,   615,   616,   235,
     197,   241,   207,  1634,   233,   210,  1401,   235,   213,   861,
     862,   235,   565,   566,   207,   207,   569,   197,   571,   871,
     573,   207,   575,   706,   207,   877,   878,   879,   581,   881,
     147,   883,   209,   885,   151,   207,   213,    21,    22,   216,
     207,   893,   235,   235,   662,   598,   130,    57,    57,   235,
     233,   209,   782,    63,    63,   213,   207,   215,   216,   172,
     678,   233,   615,   616,   241,   207,   233,   207,    21,    22,
     623,   149,   198,   199,   152,   153,   154,   155,   156,   157,
     189,   207,  1621,   241,   235,    64,    65,    66,   207,   215,
    1167,   632,   209,   235,  1375,   235,   213,    47,   215,   216,
     718,   719,   197,  1271,   207,   198,   199,  1760,    33,   662,
     207,  1172,   238,   207,   207,  1176,   235,    67,   207,   737,
     738,   739,   215,   741,   241,   678,   105,   106,   107,   108,
     860,   207,   235,   863,  1195,    60,    61,   867,   235,  1200,
     214,   235,   760,  1796,  1797,   238,   235,   181,   219,  1544,
     219,   219,   207,   231,   232,   708,   207,   141,   142,   235,
     234,   214,   231,   197,   782,   149,   214,  1562,   152,   153,
     154,   155,   156,   157,   198,   159,   160,   161,   162,   163,
     235,   234,  1835,   207,   235,   173,   234,   175,   141,   142,
     152,   153,   154,   155,   156,   157,   149,   201,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   198,   199,   187,   214,  1067,   199,    79,   198,   199,
     207,  1073,   147,   197,   197,   181,   151,   207,   215,   782,
     848,   197,    94,  1294,   234,   215,  1088,    99,   219,   101,
    1301,   197,   860,   861,   862,   863,   187,   231,   232,   867,
     231,   238,   187,   871,   235,   796,   197,   798,   238,   877,
     878,   879,   197,   881,   197,   883,   187,   885,   886,   231,
     232,  1666,  1667,   198,   199,   893,   197,  1672,   231,   232,
    1010,   197,   207,  1013,   209,  1680,    57,  1682,   213,  1019,
     215,   216,    63,    21,    22,   848,   131,   132,   133,   134,
     135,   136,   137,   138,   173,   174,   175,   860,   861,   862,
     863,   852,   187,   238,   867,   150,   241,    57,   871,     5,
       6,    57,   197,    63,   877,   878,   879,    63,   881,   164,
     883,  1726,   885,   886,   507,   508,    12,   197,  1733,  1734,
     893,   176,   177,   178,   130,    57,   130,    23,    24,    33,
    1745,    63,  1367,   526,   527,   528,   529,   530,   210,   977,
     211,  1367,   214,  1720,   215,   198,   199,   238,   239,   198,
     199,   197,   198,   208,   207,   239,    60,    61,   207,   198,
     199,   207,   215,   211,   925,   197,   215,   215,   207,   197,
     198,   231,  1010,   211,  1012,  1013,   215,   215,   211,   207,
     197,  1019,   215,   547,   548,   238,   550,   551,  1026,   238,
    1805,   139,   140,   141,   142,   197,   211,   233,  1479,   238,
     215,   149,   197,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   208,   165,   166,   198,
     199,   224,   211,   211,   210,   618,   215,   215,   207,  1067,
    1068,   831,   832,   833,  1137,  1073,   215,  1010,   201,  1012,
    1013,   211,  1679,   147,   211,   215,  1019,   151,   215,   173,
    1088,  1828,   211,  1026,    33,   199,   215,   211,   211,   238,
     211,   215,   215,   197,   215,  1026,   171,  1028,   216,   217,
     218,   219,   220,    57,    33,   201,   202,   203,   204,  1040,
    1857,    60,    61,   231,   232,   201,   202,   377,  1049,  1239,
    1051,  1728,    33,   208,  1067,  1068,   548,   387,   550,   551,
    1073,    60,    61,    66,  1376,   209,   201,   202,   203,   213,
     400,   215,   216,    10,    11,  1088,   198,  1389,  1599,    60,
      61,   201,   202,   203,  1250,  1704,   210,   720,   721,   193,
     194,   724,   725,   726,   727,   129,   729,   241,    55,    56,
      57,  1622,   171,   736,  1345,  1346,   197,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,   750,   751,   752,
     753,   754,   755,   756,   757,   758,   759,   214,   147,   214,
     214,   234,   151,  1134,   214,   214,   234,   214,   214,   214,
    1762,  1142,  1143,   214,    33,   234,  1289,   214,   147,   232,
     211,   198,   151,   239,  1155,    35,  1157,  1158,  1159,  1160,
    1161,  1239,    35,   214,  1165,    33,   147,   239,   234,   197,
     151,    60,    61,   232,   197,   239,   197,  1799,   233,    22,
    1323,   197,   239,   233,   210,   197,   234,   214,  1378,  1355,
     209,   234,    60,    61,   213,   234,   215,   216,   234,   214,
     214,  1391,   234,  1393,   837,   214,   214,   214,   214,  1592,
     209,    33,   214,   235,   213,   232,   215,   216,   215,   197,
     207,   234,   241,   234,   234,    43,  1239,   215,   209,   234,
     234,   214,   213,   233,   215,   216,  1757,   235,    60,    61,
     235,   235,   241,   131,   132,   133,   134,   135,   136,   137,
     138,   233,   582,   238,    33,  1256,   235,   234,   147,   234,
     241,   234,   151,   214,   594,   214,   214,   234,   234,   234,
      13,   233,   207,   234,     4,   235,   164,   197,   197,   147,
     210,    60,    61,   151,   197,   197,   233,   197,   176,   177,
     178,   197,   925,   207,   235,   235,   235,   235,  1376,   235,
    1378,   235,   235,   235,   235,   234,   636,   235,   235,   234,
    1500,  1389,   214,  1391,   214,  1393,   234,   234,   234,   234,
     209,   214,   233,   214,   213,   147,   215,   216,   233,   151,
     234,   234,   233,   663,   664,    21,    22,   234,   668,   234,
     670,   209,   234,    33,   214,   213,     1,   215,   216,   234,
     234,   234,   241,   234,   233,   215,   686,   687,   688,   689,
     690,   691,   215,  1376,   215,  1378,   235,  1557,   147,   234,
      60,    61,   151,   241,   208,   234,  1389,   235,  1391,    43,
    1393,   208,    43,   235,   234,   234,  1576,   209,   235,   235,
     235,   213,    75,   215,   216,  1538,    79,   235,   235,   235,
      33,   731,   235,   235,   734,   735,   235,  1040,  1509,   234,
      93,    94,   210,   234,    43,    98,    99,   100,   101,   241,
     197,   235,  1500,   234,   234,   240,   235,    60,    61,   197,
     209,   761,   239,   197,   213,  1436,   215,   216,  1439,   189,
      10,    13,     9,    42,    66,   214,   234,   234,   233,  1761,
     234,   233,  1465,   197,   240,   141,   142,   147,   788,   197,
     240,   151,   241,   149,   197,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   240,  1557,
     197,     8,   197,   235,   197,   234,   215,  1500,   197,    33,
     235,   205,   197,   197,   234,   197,   197,   827,  1576,    14,
     208,   208,   189,   234,   210,  1506,   234,   234,   240,   235,
     235,    21,    22,   235,   147,   845,    60,    61,   151,   209,
     208,   851,   234,   213,   235,   215,   216,    43,    37,   208,
     216,   217,   218,   219,   220,   235,   866,   235,   235,   235,
     235,   235,    67,   235,  1557,   231,   232,   240,   215,   234,
     234,   241,   234,    70,   235,  1698,   234,  1190,   234,   234,
    1703,   891,   892,  1576,   234,   234,   234,   197,   234,   234,
      43,   234,   902,   235,   197,   235,   209,   907,   234,   909,
     213,   911,   215,   216,    33,   234,   916,   235,   235,   235,
     197,   235,   234,   234,   234,   234,  1609,    33,   234,   201,
     235,   238,   197,   147,  1636,    33,   240,   151,   241,   235,
     235,    60,    61,    12,   235,  1758,  1759,   235,   235,   235,
     235,   235,   207,   235,    60,    61,   239,   235,   238,   139,
     140,   141,   142,   143,   235,   235,   146,   235,  1639,   149,
     234,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    33,   165,   166,   235,   235,  1660,
     990,   235,   207,   238,   994,   209,   235,   235,    53,   213,
     235,   215,   216,  1816,   235,   235,   235,   235,   235,   235,
    1823,    60,    61,  1761,   238,   235,  1829,   233,   239,    80,
    1020,   240,   233,   240,   234,     1,   239,   241,   147,   235,
     692,   240,   151,   240,   214,   215,   216,   217,   218,   219,
     220,   147,  1855,    83,  1864,   151,   798,   798,  1862,   217,
    1050,   231,   232,  1346,  1357,  1358,  1359,  1360,  1361,  1362,
    1363,  1364,  1365,  1366,  1367,  1368,  1369,  1370,  1371,  1372,
    1373,  1374,   712,  1044,  1074,   708,     1,   782,  1761,    21,
      22,  1494,  1662,  1724,  1755,    54,  1665,  1579,  1725,  1725,
     209,  1244,  1043,  1764,   213,   489,   215,   216,   147,  1043,
     360,   237,   151,   209,    -1,    -1,   663,   213,    -1,   215,
     216,    -1,    -1,    -1,    -1,    -1,   489,    -1,    -1,    -1,
    1120,    -1,   241,    -1,    -1,    -1,    -1,  1127,   489,    -1,
    1130,  1802,    -1,    -1,    -1,   241,  1136,    -1,  1138,   489,
      -1,    -1,   489,    -1,    -1,   489,    -1,    -1,    -1,  1820,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,    -1,    -1,  1834,   213,  1836,   215,   216,    -1,  1169,
    1841,  1171,    -1,  1173,    -1,  1175,    -1,  1177,    -1,    -1,
      -1,  1181,    -1,  1183,    -1,  1185,    -1,  1858,    -1,  1860,
      -1,    -1,   241,    -1,  1194,    -1,    -1,    -1,    -1,   141,
     142,    -1,  1202,  1506,    -1,    -1,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,    -1,    -1,  1226,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1236,    -1,    -1,    -1,
      -1,  1241,    -1,  1243,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1252,    -1,    -1,  1255,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1274,    -1,   218,   219,   220,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,   231,
     232,    -1,    -1,    25,    -1,    -1,    -1,  1297,    -1,    31,
      -1,    -1,    -1,    -1,    -1,  1305,  1306,  1307,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,  1324,    -1,    -1,  1327,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,  1336,    -1,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,    -1,  1396,    -1,  1398,    -1,
      -1,    -1,    -1,    -1,    -1,  1405,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     172,    -1,    -1,    -1,    -1,  1445,    -1,    -1,    -1,    -1,
      -1,  1754,    -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1764,    -1,  1463,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1480,  1481,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1501,    -1,    -1,   236,    -1,    -1,  1507,  1508,   241,
      -1,    -1,  1815,  1513,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1841,    -1,
      -1,    -1,    -1,    -1,  1544,    -1,    -1,    -1,    -1,    -1,
      -1,  1854,    -1,    -1,    -1,    -1,    -1,  1860,    -1,    -1,
      -1,    -1,  1562,    -1,    -1,    -1,    -1,  1567,    -1,    -1,
    1570,    -1,  1572,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1594,  1595,  1596,  1597,  1598,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,  1615,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
     165,   166,    -1,    -1,   169,  1635,    -1,    -1,   173,   174,
     175,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1669,
      -1,    19,    -1,    -1,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,  1686,    -1,    -1,    -1,
      -1,  1691,  1692,    -1,    -1,    -1,   231,   232,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,  1749,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1786,     1,    -1,    -1,
      -1,     5,     6,     7,    -1,     9,    10,    11,    -1,    13,
    1800,    15,    16,    17,    18,    19,    -1,    -1,    -1,  1809,
      -1,    25,    26,    27,    28,    29,    -1,    31,    -1,  1819,
      -1,    -1,    -1,    -1,    38,    39,    40,    -1,    42,    -1,
      44,    45,    -1,    -1,    48,    -1,    50,    51,    52,   187,
      54,    55,    -1,  1843,    58,    59,    -1,    -1,    -1,   197,
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
     194,   195,   196,   197,    -1,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,
     234,    -1,   236,   237,   238,   239,   240,     1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,
     234,    -1,   236,   237,   238,   239,   240,     1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,
     234,    -1,   236,   237,   238,   239,   240,     1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,
     234,    -1,   236,   237,   238,   239,   240,     1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,
     234,    -1,   236,   237,   238,   239,   240,     1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,
     234,    -1,   236,   237,   238,   239,   240,     5,     6,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    70,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,   183,   184,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
      -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,    -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,
      -1,    -1,     5,     6,   232,    -1,   234,   235,   236,   237,
      13,   239,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
     183,   184,    -1,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,
     223,   224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,
      -1,   234,    -1,   236,   237,    13,   239,    15,    16,    17,
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
      -1,    -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,   183,   184,    -1,   186,    -1,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
      -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,    -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,
      -1,    -1,     5,     6,   232,    -1,   234,    -1,   236,   237,
      -1,   239,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    70,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
     183,   184,    -1,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,
     223,   224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,
      -1,   234,    -1,   236,   237,    -1,   239,    15,    16,    17,
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
      -1,    -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,   183,   184,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
      -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,    -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,
      -1,    -1,     5,     6,   232,    -1,   234,   235,   236,   237,
      -1,   239,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
     183,   184,    -1,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,
     223,   224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,
      -1,   234,   235,   236,   237,    -1,   239,    15,    16,    17,
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
      -1,    -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,   183,   184,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
      -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,    -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,
      -1,    -1,     5,     6,   232,    -1,   234,    -1,   236,   237,
      -1,   239,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
     183,   184,    -1,   186,    -1,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,
     223,   224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,
      -1,   234,   235,   236,   237,    -1,   239,    15,    16,    17,
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
      -1,    -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,   183,   184,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
      -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,    -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,
      -1,    -1,     5,     6,   232,    -1,   234,    -1,   236,   237,
      -1,   239,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
     183,   184,    -1,   186,    -1,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,
     223,   224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,
      -1,   234,   235,   236,   237,    -1,   239,    15,    16,    17,
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
      -1,    -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,   183,   184,    -1,   186,    -1,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
      -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,    -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,
      -1,    -1,     5,     6,   232,    -1,   234,   235,   236,   237,
      -1,   239,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
     183,   184,    -1,   186,    -1,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,
     223,   224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,
      -1,   234,   235,   236,   237,    -1,   239,    15,    16,    17,
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
      -1,    -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,   183,   184,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
      -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,    -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,
      -1,    -1,     5,     6,   232,    -1,   234,    -1,   236,   237,
      -1,   239,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
     183,   184,    -1,   186,    -1,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,
     223,   224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,
      -1,   234,    -1,   236,   237,    -1,   239,    15,    16,    17,
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
      -1,    -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,   183,   184,    -1,   186,    -1,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
      -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,    -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,
      -1,    -1,     5,     6,   232,   233,   234,    -1,   236,   237,
      -1,   239,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
     183,   184,    -1,   186,    -1,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,
     223,   224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,
     233,   234,    -1,   236,   237,    -1,   239,    15,    16,    17,
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
      -1,    -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,   183,   184,    -1,   186,    -1,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
      -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,    -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,
      -1,    -1,     5,     6,   232,    -1,   234,    -1,   236,   237,
      13,   239,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
     183,   184,    -1,   186,    -1,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,
     223,   224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,
      -1,   234,    -1,   236,   237,    -1,   239,    15,    16,    17,
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
      -1,    -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,   183,   184,    -1,   186,    -1,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
      -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,    -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,
      -1,    -1,     5,     6,   232,    -1,   234,    -1,   236,   237,
      -1,   239,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    61,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
     183,   184,    -1,   186,    -1,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,
     223,   224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,
      -1,   234,    -1,   236,   237,    -1,   239,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
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
      -1,    -1,     5,     6,   232,    -1,   234,    -1,   236,   237,
      -1,   239,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
     183,   184,    -1,   186,    -1,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,
     223,   224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,
      -1,   234,    -1,   236,   237,    -1,   239,    15,    16,    17,
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
      -1,    -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,   183,   184,    -1,   186,    -1,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
      -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,    -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,
      -1,    -1,     5,     6,   232,    -1,   234,   235,   236,   237,
      -1,   239,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
     183,   184,    -1,   186,    -1,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,
     223,   224,    -1,    -1,    -1,    -1,    -1,     5,     6,   232,
      -1,   234,    -1,   236,   237,    -1,   239,    15,    16,    17,
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
      -1,    -1,   170,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,   183,   184,    -1,   186,    -1,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
      -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,    -1,   220,    -1,    -1,   223,   224,    -1,    -1,    -1,
      -1,    -1,     5,     6,   232,    -1,   234,    -1,   236,   237,
      -1,   239,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    52,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
     183,   184,    -1,   186,    -1,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,
     223,   224,    -1,    -1,    -1,     5,     6,    -1,    -1,   232,
      -1,   234,    -1,   236,   237,    15,    16,    17,    18,    19,
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
     170,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,   181,   182,   183,   184,    -1,   186,    -1,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,    -1,    -1,
     200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,    -1,
     220,    -1,    -1,   223,   224,    -1,    -1,    -1,     5,     6,
      -1,    -1,   232,    -1,   234,    -1,   236,   237,    15,    16,
      17,    18,    19,    -1,    -1,    22,    -1,    -1,    25,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,   172,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,   181,   182,   183,   184,    -1,   186,
      -1,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,    -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,   218,    -1,   220,    -1,    -1,   223,   224,    -1,    -1,
      -1,     5,     6,    -1,    -1,   232,    -1,   234,    -1,   236,
     237,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
     124,   125,   126,   127,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,   141,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,    -1,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,
     234,    -1,   236,   237,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,    -1,    -1,    -1,   173,   174,   175,
     176,   177,   178,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   208,   209,    -1,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,   231,   232,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,   197,    -1,   199,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,   236,    -1,   238,    -1,    71,    72,    73,    74,
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
     185,    19,    -1,    -1,    21,    22,    -1,    25,    -1,    -1,
      -1,    -1,   197,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,   236,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,    21,
      22,    -1,    -1,    -1,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,   165,   166,
     167,   168,    -1,    -1,   172,    -1,   173,   174,   175,   176,
     177,   178,   179,    -1,    -1,    -1,    -1,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,    -1,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,    21,    22,   223,   224,    -1,    -1,
      -1,    -1,    -1,    -1,   231,   232,    -1,    -1,   236,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,   165,   166,    -1,    -1,   169,    -1,    -1,
      -1,   173,   174,   175,    -1,    -1,    -1,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,   209,    -1,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,
     232,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,   165,   166,
      -1,    -1,   169,    -1,    -1,    -1,   173,   174,   175,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,    -1,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   231,   232,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,   173,   174,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
      -1,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,    -1,    -1,    21,    22,    -1,   139,   140,   141,   142,
     143,   231,   232,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,    -1,    -1,
      21,    22,    -1,   139,   140,   141,   142,   143,   231,   232,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,   165,
     166,   139,   140,   141,   142,   143,    -1,   173,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,   231,   232,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,    -1,    -1,    21,    22,    -1,   139,   140,
     141,   142,   143,   231,   232,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   212,   213,   214,   215,   216,   217,   218,   219,   220,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
     231,   232,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,   165,   166,   139,   140,   141,   142,   143,    -1,    -1,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,   213,
     214,   215,   216,   217,   218,   219,   220,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,   232,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
     216,   217,   218,   219,   220,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   231,   232,    71,    72,    73,
      19,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,    71,
      72,    73,    19,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    35,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   141,   142,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   197,    -1,    71,    -1,    73,    -1,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,   152,   153,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     231,   232,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    73,    -1,    75,    76,    77,
      78,    79,   197,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    71,    -1,    73,
      -1,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   243,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   125,   126,   199,   220,   232,   238,   245,   246,   250,
     259,   261,   262,   267,   316,   322,   353,   443,   451,   460,
     470,   520,   525,   530,    19,    20,    69,   197,   306,   307,
     308,   189,   268,   269,   209,   264,   265,    57,    63,   448,
     449,   452,   197,   236,   248,   531,   521,   526,   172,   197,
     341,    34,    63,   130,   163,   224,   234,   311,   312,   313,
     314,   341,   245,   245,   245,     8,    36,   471,    62,   439,
     208,   207,   210,   207,   179,   197,   220,   231,   263,   232,
     263,   197,   245,   245,   439,   448,   448,   448,   197,   172,
     260,   313,   313,   313,   234,   173,   174,   175,   207,   233,
     130,   321,   461,     5,     6,   467,    57,    63,   440,    15,
      16,   170,   189,   194,   197,   200,   234,   252,   307,   189,
     269,   219,   219,   263,   219,   219,   231,   197,    22,    57,
     266,   197,   450,    57,    63,   247,   197,   197,   197,   197,
     201,   258,   235,   308,   313,   313,   313,   313,   199,   273,
     274,    57,    63,   323,   325,    57,    63,   454,   130,   130,
      57,    63,   468,   239,   444,   197,   201,   202,   203,   251,
      15,    16,   189,   194,   197,   252,   304,   305,   263,   263,
     263,   197,   197,   233,   197,   453,   208,   224,   249,   210,
     481,   274,   274,   201,   235,   199,   326,   197,   455,   472,
     441,   171,   309,   409,   201,   202,   203,   207,   235,   266,
     266,   208,    19,    25,    31,    41,    49,    64,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   185,   236,   341,   475,   477,   478,
     482,   488,   490,   519,    66,    79,    94,    99,   101,   198,
     458,   459,   522,   527,    35,    71,    73,    75,    76,    77,
      78,    79,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    93,    94,    95,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   141,   142,   197,   319,   320,   324,   210,
     456,   129,   465,   466,   240,   245,   442,   307,   171,   197,
     435,   438,   304,   519,   214,   214,   214,   234,   214,   214,
     234,   481,   214,   214,   214,   214,   214,   234,   341,   214,
     234,    33,    60,    61,   147,   151,   209,   213,   216,   241,
     232,   487,   211,   198,   532,   239,   239,    21,    22,    38,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   151,   152,
     153,   164,   165,   166,   167,   168,   173,   174,   175,   176,
     177,   178,   179,   209,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   223,   224,   231,   232,    35,    35,
     234,   317,   274,    75,    79,    93,    94,    98,    99,   100,
     101,   476,   459,   197,   274,   409,   274,   307,   207,   210,
     213,   433,   491,   497,   499,     5,     6,    15,    16,    17,
      18,    19,    25,    27,    31,    39,    45,    48,    51,    55,
      65,    68,    69,    80,   125,   126,   127,   141,   142,   170,
     180,   181,   182,   183,   184,   186,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   200,   216,   217,   218,   223,
     224,   232,   234,   236,   237,   239,   257,   259,   333,   341,
     346,   358,   365,   368,   371,   375,   377,   379,   380,   382,
     387,   390,   391,   392,   407,   408,   475,   536,   544,   555,
     558,   571,   572,   575,   576,   501,   495,   197,   214,   503,
     505,   507,   509,   511,   513,   515,   517,   391,   214,   234,
     489,   493,   150,   338,   369,   391,    33,   213,    33,   213,
     232,   241,   233,   391,   232,   241,   488,   239,   523,   528,
     197,   320,   197,   320,   197,   233,    22,   197,   233,   184,
     235,   409,   419,   420,   421,   149,   210,   318,   169,   239,
     330,   370,   239,   210,   464,   473,   181,   197,   434,   437,
     274,   197,   488,   150,   164,   208,   432,   519,   519,   486,
     519,   214,   214,   214,   341,   343,   477,   535,   544,   555,
     558,   571,   572,   575,   576,   341,   214,     5,   125,   126,
     214,   234,   214,   234,   234,   214,   214,   214,   234,   214,
     234,   214,   234,   214,   214,   234,    19,   214,   214,   392,
     392,   181,   186,   214,   341,   381,   234,   234,   234,   234,
     234,   234,   256,   392,   392,   392,   392,   392,    13,    49,
     338,   187,   197,   369,   537,   539,   570,   234,   232,   315,
     170,   239,   371,   376,   376,   376,   376,   235,    21,    22,
     139,   140,   141,   142,   143,   146,   147,   148,   149,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   165,   166,   169,   173,   174,   175,   179,   209,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     231,   232,   234,   519,   519,   235,   215,   483,   519,   317,
     519,   317,   519,   317,   197,   422,   423,   519,   197,   425,
     426,   235,   494,   369,   340,   519,   391,   235,   207,   574,
     233,   233,   233,   391,   533,   422,   424,   425,   427,   197,
     320,   131,   132,   133,   134,   135,   136,   137,   138,   164,
     176,   177,   178,   131,   132,   133,   134,   135,   136,   137,
     138,   150,   164,   176,   177,   178,   208,   234,     7,    50,
     352,   238,   207,   238,   235,   519,   519,   150,   392,   331,
     462,   341,   238,   239,   469,   234,    43,   207,   210,   433,
     245,   432,   391,   215,   215,   215,   198,   207,   244,   245,
     485,   545,   547,   344,   234,   214,   234,   366,   214,   214,
     214,   565,   369,   488,   391,   569,   391,   359,   361,   393,
     391,   363,   391,   567,   369,   553,   556,   369,   214,   549,
     488,   234,   234,   383,   385,   391,   391,   391,   391,   391,
     391,   203,   204,   251,   234,    13,   233,   234,   150,   164,
     208,   428,   574,   207,   574,   235,   274,    70,   232,   235,
     369,   539,   314,     4,   374,   337,   315,    19,   187,   197,
     475,    19,   187,   197,   475,   392,   392,   392,   392,   392,
     392,   197,   392,   187,   197,   391,   187,   197,   187,   197,
     391,   391,   392,    19,   187,   197,   475,    19,   187,   197,
     475,    19,   187,   197,   475,   392,   392,   475,   392,   392,
     392,   571,   576,   392,   392,   392,   392,    22,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   152,
     153,   187,   197,   231,   232,   388,   475,   391,   235,   369,
     215,   215,   197,   479,   215,   318,   215,   318,   215,   318,
     210,   215,   485,   210,   215,   485,   340,   574,   215,   485,
     150,   391,   233,   197,   480,   245,   281,   282,   281,   282,
     391,   181,   197,   429,   430,   474,   421,   421,   421,   392,
     337,   197,   445,   447,   415,   391,   197,   181,   197,   488,
     432,   391,   245,   492,   498,   500,   519,   488,   488,   519,
      70,   369,   539,   543,   197,   391,   519,   559,   561,   563,
     488,   574,   215,   485,   207,   574,   235,   488,   488,   488,
     235,   488,   235,   488,   574,   488,   423,   574,   551,   426,
     215,   391,   391,   488,   317,   235,   235,   235,   235,   235,
     235,   391,   181,   197,   234,   295,   234,   391,   391,   391,
     235,   187,   197,    13,   339,   569,   197,   235,   539,   537,
     207,   235,   235,   233,   234,   317,     1,    26,    28,    29,
      38,    40,    44,    52,    54,    58,    59,    65,   128,   239,
     240,   245,   270,   271,   280,   291,   292,   294,   296,   297,
     298,   299,   300,   301,   302,   303,   334,   342,   347,   348,
     349,   350,   351,   353,   357,   378,   392,   374,   214,   234,
     214,   234,   234,   234,   233,   234,   234,   234,   233,   233,
     214,   234,   214,   234,   214,   234,    19,   187,   197,   475,
     210,   187,   197,   391,   234,   234,   187,   197,   391,     1,
     234,   233,   207,   235,   502,   496,   207,   215,   238,   504,
     215,   508,   215,   512,   215,   519,   516,   422,   519,   518,
     425,   215,   235,   489,   519,   391,   208,   244,   446,   457,
     245,   422,   524,   425,   529,   235,   234,    43,   207,   210,
     213,   428,   332,   208,   446,   457,    40,   199,   240,   316,
     416,   235,   234,    43,   245,   432,   391,   245,   215,   215,
     215,   539,   235,   235,   235,   215,   485,   235,   215,   488,
     423,   426,   215,   235,   234,   488,   391,   235,   215,   215,
     215,   215,   215,   235,   215,   215,   235,   488,   215,   374,
     235,   235,   215,   318,   234,   210,   254,   234,    43,   197,
     355,    20,   207,   295,   235,   234,   164,   428,   234,   240,
     574,   235,   207,   233,   232,   537,   150,   164,   197,   208,
     213,   372,   373,   318,   150,   391,   330,    61,   391,   197,
     197,   245,   189,    58,   391,   274,   150,   391,   335,   245,
     245,    10,    10,    11,   278,    13,     9,    42,   245,   245,
     245,   245,   245,   245,    66,   354,   245,   131,   132,   133,
     134,   135,   136,   137,   138,   144,   145,   150,   164,   167,
     168,   176,   177,   178,   208,   317,   395,   391,   399,   391,
     232,   235,   369,   537,   391,   391,   235,   369,   391,   397,
     391,   401,   391,   405,   391,   214,   234,   392,   234,   233,
     391,   232,   235,   369,   537,   234,   233,   389,   235,   369,
     197,   484,   197,   506,   510,   514,   489,   391,   197,   244,
     534,   240,   240,   391,   197,   181,   197,   519,   391,   240,
     391,   445,   463,   197,     8,   409,   414,   391,   197,   391,
     245,   546,   548,   345,   235,   234,   197,   367,   215,   215,
     215,   566,   339,   215,   360,   362,   394,   364,   568,   554,
     557,   215,   550,   234,   274,   384,   215,   235,   369,   255,
     205,   391,   197,   207,   235,   369,   181,   197,   234,    20,
     164,   428,   391,   391,   391,   295,   235,   537,   235,   197,
     197,   234,   197,   197,   207,   235,   274,   391,    14,   391,
     208,   208,   210,   189,   330,   391,   337,   234,   234,   232,
     309,   310,   310,   234,   234,   239,   356,   409,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   571,
     576,   392,   392,   392,   392,   392,   392,   392,   318,   488,
     235,   519,   235,   537,   207,   235,   235,   235,   235,   235,
     235,   488,   235,   519,   235,   519,   235,   403,   391,   391,
     235,   537,   207,   235,   235,   391,   235,   208,   240,   235,
     234,    43,   428,    37,   327,   240,   208,    57,    63,   412,
      67,   413,   245,   235,   245,   234,   234,   391,   215,   560,
     562,   564,   234,   235,   234,   234,   234,   234,   234,   234,
      70,   543,   234,   552,   234,   235,   391,   330,   381,   386,
     235,   253,   235,   197,   235,   234,    43,   355,   369,   391,
     391,   235,    20,   233,   197,   372,   370,   330,   519,   391,
     336,   391,   391,   308,   234,   234,    56,   391,   355,   436,
     437,   274,   215,   215,   233,   537,   215,   215,   215,   519,
     235,   235,   233,   537,   235,   391,   391,   197,   391,   328,
     519,    47,   413,    46,   129,   410,   543,   543,   235,   234,
     234,   234,   234,   338,   339,   391,   391,   391,   391,   369,
     543,   234,   543,   235,   381,   201,   238,   391,   197,   235,
     235,   164,   428,   369,   235,   235,   240,   235,   235,   233,
     295,   391,   263,   235,   235,   240,   245,   437,   370,   396,
     400,   235,   235,   398,   402,   406,   215,   235,   235,   235,
     239,   245,    33,   411,   410,   412,   234,   537,   540,   541,
     542,   542,   391,   543,   543,   537,   538,   235,   235,   235,
     235,   235,   235,   574,   542,   543,   538,   391,   235,   238,
     391,   391,   235,   327,    12,   279,   274,    20,   235,   235,
     274,   210,   433,   404,   337,   417,   411,   429,   430,   431,
     537,   207,   574,   235,   235,   235,   542,   542,   235,   235,
     235,   538,   235,   238,   573,   391,   238,   280,   347,   348,
     349,   350,   392,   245,   293,   369,   274,   274,   330,   488,
     432,   329,   324,   418,   235,   234,   235,   235,   235,    53,
     233,   573,   391,   239,   283,   286,   235,   330,   330,   432,
     391,   240,   245,   324,   537,   391,   233,   573,   284,    12,
      23,    24,   272,   275,   280,   274,   391,   245,   274,   235,
     240,   337,   274,   234,   245,   330,   245,   370,   285,   276,
     391,   240,   239,   287,   290,   235,   327,   288,   280,   274,
     337,   245,   277,   289,   287,   240,   275,   327
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
     448,   448,   448,   450,   449,   451,   451,   453,   452,   454,
     454,   454,   455,   456,   456,   457,   457,   458,   458,   459,
     459,   461,   462,   463,   460,   464,   464,   465,   465,   466,
     467,   467,   467,   467,   468,   468,   468,   469,   469,   471,
     472,   473,   470,   474,   474,   474,   474,   474,   474,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   476,
     476,   476,   476,   476,   476,   476,   476,   477,   478,   478,
     478,   479,   479,   479,   480,   480,   480,   480,   480,   481,
     481,   481,   481,   481,   482,   483,   484,   482,   485,   485,
     486,   486,   487,   487,   487,   487,   488,   488,   489,   489,
     490,   490,   490,   490,   491,   492,   490,   490,   490,   490,
     493,   490,   494,   490,   490,   490,   490,   490,   490,   490,
     490,   490,   490,   490,   490,   490,   495,   496,   490,   490,
     497,   498,   490,   499,   500,   490,   501,   502,   490,   490,
     503,   504,   490,   505,   506,   490,   490,   507,   508,   490,
     509,   510,   490,   490,   511,   512,   490,   513,   514,   490,
     515,   516,   490,   517,   518,   490,   519,   519,   519,   521,
     522,   523,   524,   520,   526,   527,   528,   529,   525,   531,
     532,   533,   534,   530,   535,   535,   535,   535,   535,   535,
     535,   536,   536,   536,   536,   536,   537,   537,   537,   537,
     537,   537,   537,   537,   538,   538,   539,   540,   540,   541,
     541,   542,   542,   543,   543,   545,   546,   544,   547,   548,
     544,   549,   550,   544,   551,   552,   544,   553,   554,   544,
     555,   556,   557,   555,   558,   559,   560,   558,   561,   562,
     558,   563,   564,   558,   558,   565,   566,   558,   558,   567,
     568,   558,   569,   569,   570,   571,   572,   572,   572,   573,
     573,   574,   574,   575,   575,   576
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
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 477: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' expr_list ')'  */
                                                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
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
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 494: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' expr_list ')'  */
                                                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 495: /* expr_field_no_bracket: expr_no_bracket "!." "name" '(' ')'  */
                                                                    {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), *(yyvsp[-2].s), true);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 496: /* expr_field_no_bracket: expr_no_bracket "!." "name" '(' expr_list ')'  */
                                                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), *(yyvsp[-3].s), true);
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
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 501: /* expr_field_no_bracket: expr_no_bracket '.' basic_type_declaration '(' expr_list ')'  */
                                                                                                        {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), method_name);
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

  case 747: /* $@58: %empty  */
                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 748: /* distinct_alias: optional_public_or_private_alias "name" "name" $@58 '=' type_declaration  */
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

  case 749: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 750: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 751: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 752: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 753: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 754: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 761: /* $@59: %empty  */
                                                                     {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 762: /* $@60: %empty  */
                                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 763: /* $@61: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 764: /* enum_declaration: optional_annotation_list_with_emit_semis "enum" $@59 optional_public_or_private_enum enum_name optional_enum_basic_type_declaration optional_emit_commas '{' $@60 enum_list optional_commas $@61 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-8].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-12].faList),tokAt(scanner,(yylsp[-12])),(yyvsp[-9].b),(yyvsp[-8].pEnum),(yyvsp[-3].pEnumList),(yyvsp[-7].type));
    }
    break;

  case 765: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 766: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 767: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 768: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 769: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 770: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 771: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 772: /* class_or_struct: "class" "template"  */
                                  { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 773: /* class_or_struct: "struct" "template"  */
                                  { (yyval.i) = CorS_StructTemplate; }
    break;

  case 774: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 775: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 776: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 777: /* optional_struct_variable_declaration_list: ';'  */
            {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 778: /* optional_struct_variable_declaration_list: '{' struct_variable_declaration_list '}'  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 779: /* $@62: %empty  */
                                                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 780: /* $@63: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 781: /* $@64: %empty  */
                                             {
        if ( (yyvsp[-1].pStructure) ) {
            (yyvsp[-1].pStructure)->isClass = (yyvsp[-4].i)==CorS_Class || (yyvsp[-4].i)==CorS_ClassTemplate;
            (yyvsp[-1].pStructure)->isTemplate = (yyvsp[-4].i)==CorS_ClassTemplate || (yyvsp[-4].i)==CorS_StructTemplate;
            (yyvsp[-1].pStructure)->privateStructure = !(yyvsp[-3].b);
        }
    }
    break;

  case 782: /* structure_declaration: optional_annotation_list_with_emit_semis $@62 class_or_struct optional_public_or_private_structure $@63 structure_name optional_emit_semis $@64 optional_struct_variable_declaration_list  */
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

  case 783: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 784: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 785: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 786: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 787: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                           {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 788: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 789: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 790: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 791: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 792: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 793: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 794: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 795: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 796: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 797: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 798: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 799: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 800: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 801: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 802: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 803: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 804: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 805: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 806: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 807: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 808: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 809: /* basic_type_declaration: "float16"  */
                        { (yyval.type) = Type::tFloat16; }
    break;

  case 810: /* basic_type_declaration: "half2"  */
                        { (yyval.type) = Type::tHalf2; }
    break;

  case 811: /* basic_type_declaration: "half3"  */
                        { (yyval.type) = Type::tHalf3; }
    break;

  case 812: /* basic_type_declaration: "half4"  */
                        { (yyval.type) = Type::tHalf4; }
    break;

  case 813: /* basic_type_declaration: "half8"  */
                        { (yyval.type) = Type::tHalf8; }
    break;

  case 814: /* basic_type_declaration: "short2"  */
                        { (yyval.type) = Type::tShort2; }
    break;

  case 815: /* basic_type_declaration: "short3"  */
                        { (yyval.type) = Type::tShort3; }
    break;

  case 816: /* basic_type_declaration: "short4"  */
                        { (yyval.type) = Type::tShort4; }
    break;

  case 817: /* basic_type_declaration: "short8"  */
                        { (yyval.type) = Type::tShort8; }
    break;

  case 818: /* basic_type_declaration: "ushort2"  */
                        { (yyval.type) = Type::tUShort2; }
    break;

  case 819: /* basic_type_declaration: "ushort3"  */
                        { (yyval.type) = Type::tUShort3; }
    break;

  case 820: /* basic_type_declaration: "ushort4"  */
                        { (yyval.type) = Type::tUShort4; }
    break;

  case 821: /* basic_type_declaration: "ushort8"  */
                        { (yyval.type) = Type::tUShort8; }
    break;

  case 822: /* basic_type_declaration: "byte2"  */
                        { (yyval.type) = Type::tByte2; }
    break;

  case 823: /* basic_type_declaration: "byte3"  */
                        { (yyval.type) = Type::tByte3; }
    break;

  case 824: /* basic_type_declaration: "byte4"  */
                        { (yyval.type) = Type::tByte4; }
    break;

  case 825: /* basic_type_declaration: "byte8"  */
                        { (yyval.type) = Type::tByte8; }
    break;

  case 826: /* basic_type_declaration: "byte16"  */
                        { (yyval.type) = Type::tByte16; }
    break;

  case 827: /* basic_type_declaration: "ubyte2"  */
                        { (yyval.type) = Type::tUByte2; }
    break;

  case 828: /* basic_type_declaration: "ubyte3"  */
                        { (yyval.type) = Type::tUByte3; }
    break;

  case 829: /* basic_type_declaration: "ubyte4"  */
                        { (yyval.type) = Type::tUByte4; }
    break;

  case 830: /* basic_type_declaration: "ubyte8"  */
                        { (yyval.type) = Type::tUByte8; }
    break;

  case 831: /* basic_type_declaration: "ubyte16"  */
                        { (yyval.type) = Type::tUByte16; }
    break;

  case 832: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 833: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 834: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 835: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 836: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 837: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 838: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 839: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 840: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 841: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 842: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 843: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 844: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 845: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 846: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 847: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 848: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 849: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 850: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 851: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 852: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 853: /* bitfield_bits: bitfield_bits ',' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 854: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<tuple<string,Expression *>>();
        (yyval.pNameExprList) = pSL;

    }
    break;

  case 855: /* bitfield_alias_bits: "name"  */
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

  case 856: /* bitfield_alias_bits: "name" '=' expr  */
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

  case 857: /* bitfield_alias_bits: bitfield_alias_bits commas "name"  */
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

  case 858: /* bitfield_alias_bits: bitfield_alias_bits commas "name" '=' expr  */
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

  case 859: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 860: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 861: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 862: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 863: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 864: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' '>'  */
                                                                          {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-2].type), tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-2]));
    }
    break;

  case 865: /* $@65: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 866: /* $@66: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 867: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@65 bitfield_bits '>' $@66  */
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

  case 870: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.aTypePair).secondType->at = (yyval.aTypePair).firstType->at;
    }
    break;

  case 871: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 872: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = appendDimExpr(nullptr, (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 873: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = appendDimExpr(nullptr, nullptr, tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 874: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = appendDimExpr((yyvsp[-3].pTypeDecl), (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 875: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = appendDimExpr((yyvsp[-2].pTypeDecl), nullptr, tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 876: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 877: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
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

  case 878: /* optional_expr_list_in_braces: %empty  */
            { (yyval.pExpression) = nullptr; }
    break;

  case 879: /* optional_expr_list_in_braces: '(' expr_list optional_comma ')'  */
                                                { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 880: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc))); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 881: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 882: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 883: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 884: /* $@67: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 885: /* $@68: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 886: /* type_declaration_no_options_no_dim: "type" '<' $@67 type_declaration '>' $@68  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 887: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->typeMacroExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 888: /* type_declaration_no_options_no_dim: name_in_namespace '(' optional_expr_list ')'  */
                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->typeMacroExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 889: /* type_declaration_no_options_no_dim: '$' name_in_namespace optional_expr_list_in_braces  */
                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-1]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = sequenceToList((yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-1])), *(yyvsp[-1].s)));
        delete (yyvsp[-1].s);
    }
    break;

  case 890: /* $@69: %empty  */
                                    { yyextra->das_arrow_depth ++; }
    break;

  case 891: /* type_declaration_no_options_no_dim: name_in_namespace '<' $@69 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 892: /* $@70: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 893: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@70 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 894: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 895: /* type_declaration_no_options_no_dim: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 896: /* type_declaration_no_options_no_dim: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 897: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 898: /* type_declaration_no_options_no_dim: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 899: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 900: /* type_declaration_no_options_no_dim: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 901: /* type_declaration_no_options_no_dim: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 902: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 903: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 904: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 905: /* type_declaration_no_options_no_dim: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 906: /* $@71: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 907: /* $@72: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 908: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@71 type_declaration '>' $@72  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 909: /* type_declaration_no_options_no_dim: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 910: /* $@73: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 911: /* $@74: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 912: /* type_declaration_no_options_no_dim: "array" '<' $@73 type_declaration '>' $@74  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 913: /* $@75: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 914: /* $@76: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 915: /* type_declaration_no_options_no_dim: "table" '<' $@75 table_type_pair '>' $@76  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 916: /* $@77: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 917: /* $@78: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 918: /* type_declaration_no_options_no_dim: "iterator" '<' $@77 type_declaration '>' $@78  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 919: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 920: /* $@79: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 921: /* $@80: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 922: /* type_declaration_no_options_no_dim: "block" '<' $@79 type_declaration '>' $@80  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 923: /* $@81: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 924: /* $@82: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 925: /* type_declaration_no_options_no_dim: "block" '<' $@81 optional_function_argument_list optional_function_type '>' $@82  */
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

  case 926: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 927: /* $@83: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 928: /* $@84: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 929: /* type_declaration_no_options_no_dim: "function" '<' $@83 type_declaration '>' $@84  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 930: /* $@85: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 931: /* $@86: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 932: /* type_declaration_no_options_no_dim: "function" '<' $@85 optional_function_argument_list optional_function_type '>' $@86  */
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

  case 933: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 934: /* $@87: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 935: /* $@88: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 936: /* type_declaration_no_options_no_dim: "lambda" '<' $@87 type_declaration '>' $@88  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 937: /* $@89: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 938: /* $@90: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 939: /* type_declaration_no_options_no_dim: "lambda" '<' $@89 optional_function_argument_list optional_function_type '>' $@90  */
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

  case 940: /* $@91: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 941: /* $@92: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 942: /* type_declaration_no_options_no_dim: "tuple" '<' $@91 tuple_type_list '>' $@92  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 943: /* $@93: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 944: /* $@94: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 945: /* type_declaration_no_options_no_dim: "variant" '<' $@93 variant_type_list '>' $@94  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 946: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 947: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 948: /* type_declaration: type_declaration '|' '#'  */
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

  case 949: /* $@95: %empty  */
                   {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 950: /* $@96: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 951: /* $@97: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 952: /* $@98: %empty  */
                                                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 953: /* tuple_alias_declaration: "tuple" $@95 optional_public_or_private_alias "name" optional_emit_semis $@96 '{' $@97 tuple_alias_type_list optional_semis $@98 '}'  */
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

  case 954: /* $@99: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 955: /* $@100: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 956: /* $@101: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 957: /* $@102: %empty  */
                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 958: /* variant_alias_declaration: "variant" $@99 optional_public_or_private_alias "name" optional_emit_semis $@100 '{' $@101 variant_alias_type_list optional_semis $@102 '}'  */
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

  case 959: /* $@103: %empty  */
                      {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 960: /* $@104: %empty  */
                                                                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 961: /* $@105: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 962: /* $@106: %empty  */
                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 963: /* bitfield_alias_declaration: "bitfield" $@103 optional_public_or_private_alias "name" bitfield_basic_type_declaration optional_emit_commas $@104 '{' $@105 bitfield_alias_bits optional_commas $@106 '}'  */
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

  case 964: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 965: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 966: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 967: /* make_decl: make_table_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 968: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 969: /* make_decl: table_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 970: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 971: /* make_decl_no_bracket: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 972: /* make_decl_no_bracket: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 973: /* make_decl_no_bracket: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 974: /* make_decl_no_bracket: table_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 975: /* make_decl_no_bracket: make_table_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 976: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 977: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 978: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 979: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 980: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 981: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 982: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 983: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 984: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 985: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 986: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 987: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 988: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 989: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 990: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 991: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 992: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 993: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 994: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 995: /* $@107: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 996: /* $@108: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 997: /* make_struct_decl: "struct" '<' $@107 type_declaration_no_options '>' $@108 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 998: /* $@109: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 999: /* $@110: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 1000: /* make_struct_decl: "class" '<' $@109 type_declaration_no_options '>' $@110 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1001: /* $@111: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 1002: /* $@112: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 1003: /* make_struct_decl: "variant" '<' $@111 variant_type_list '>' $@112 '(' use_initializer make_variant_dim ')'  */
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

  case 1004: /* $@113: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 1005: /* $@114: %empty  */
                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 1006: /* make_struct_decl: "variant" "type" '<' $@113 type_declaration_no_options '>' $@114 '(' use_initializer make_variant_dim ')'  */
                                                                                                                                                                                                    {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1007: /* $@115: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 1008: /* $@116: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 1009: /* make_struct_decl: "default" '<' $@115 type_declaration_no_options '>' $@116 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 1010: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pExpression) = mkt;
    }
    break;

  case 1011: /* $@117: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 1012: /* $@118: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 1013: /* make_tuple_call: "tuple" '<' $@117 tuple_type_list '>' $@118 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 1014: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 1015: /* $@119: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 1016: /* $@120: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 1017: /* make_dim_decl: "array" "struct" '<' $@119 type_declaration_no_options '>' $@120 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 1018: /* $@121: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 1019: /* $@122: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 1020: /* make_dim_decl: "array" "tuple" '<' $@121 tuple_type_list '>' $@122 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 1021: /* $@123: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 1022: /* $@124: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 1023: /* make_dim_decl: "array" "variant" '<' $@123 variant_type_list '>' $@124 '(' make_variant_dim ')'  */
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

  case 1024: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 1025: /* $@125: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 1026: /* $@126: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 1027: /* make_dim_decl: "array" '<' $@125 type_declaration_no_options '>' $@126 '(' optional_expr_list ')'  */
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

  case 1028: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 1029: /* $@127: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 1030: /* $@128: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 1031: /* make_dim_decl: "fixed_array" '<' $@127 type_declaration_no_options '>' $@128 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 1032: /* expr_map_tuple_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 1033: /* expr_map_tuple_list: expr_map_tuple_list ',' expr  */
                                                      {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 1034: /* push_table_nesting: %empty  */
                    {
        yyextra->das_nested_parentheses ++;
    }
    break;

  case 1035: /* make_table_decl: '{' push_table_nesting optional_emit_semis optional_expr_map_tuple_list '}'  */
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

  case 1036: /* make_table_call: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 1037: /* make_table_call: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 1038: /* make_table_call: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 1039: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 1040: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 1041: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 1042: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 1043: /* table_comprehension: '[' "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                               {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 1044: /* table_comprehension: '[' "iterator" "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 1045: /* array_comprehension: '{' push_table_nesting optional_emit_semis "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where '}'  */
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


