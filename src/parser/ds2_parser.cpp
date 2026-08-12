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
#define YYLAST   12434

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  242
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  334
/* YYNRULES -- Number of rules.  */
#define YYNRULES  1043
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1862

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
     855,   858,   864,   865,   869,   873,   874,   878,   881,   887,
     893,   896,   902,   903,   907,   908,   912,   913,   917,   918,
     918,   922,   922,   931,   932,   936,   937,   943,   944,   945,
     946,   947,   951,   952,   956,   957,   961,   963,   961,   975,
     975,   983,   985,   983,   997,   997,  1005,  1007,  1005,  1018,
    1025,  1032,  1037,  1046,  1054,  1060,  1064,  1072,  1082,  1082,
    1091,  1099,  1099,  1115,  1121,  1128,  1147,  1151,  1158,  1159,
    1160,  1161,  1162,  1163,  1167,  1172,  1180,  1181,  1182,  1183,
    1187,  1188,  1189,  1190,  1191,  1192,  1193,  1194,  1195,  1201,
    1204,  1210,  1213,  1219,  1222,  1225,  1231,  1232,  1233,  1234,
    1238,  1256,  1279,  1282,  1292,  1307,  1322,  1337,  1340,  1347,
    1351,  1358,  1359,  1363,  1364,  1368,  1369,  1370,  1374,  1378,
    1382,  1389,  1393,  1394,  1395,  1396,  1397,  1398,  1399,  1400,
    1401,  1402,  1403,  1404,  1405,  1406,  1407,  1408,  1409,  1410,
    1411,  1412,  1413,  1414,  1415,  1416,  1417,  1418,  1419,  1420,
    1421,  1422,  1423,  1424,  1425,  1426,  1427,  1428,  1429,  1430,
    1431,  1432,  1433,  1434,  1435,  1436,  1437,  1438,  1439,  1440,
    1441,  1442,  1443,  1444,  1445,  1446,  1447,  1448,  1449,  1450,
    1451,  1452,  1453,  1454,  1455,  1456,  1457,  1458,  1459,  1460,
    1461,  1462,  1463,  1464,  1465,  1466,  1467,  1468,  1469,  1470,
    1471,  1472,  1473,  1474,  1475,  1476,  1477,  1478,  1479,  1480,
    1484,  1485,  1486,  1487,  1488,  1489,  1490,  1491,  1492,  1493,
    1494,  1495,  1496,  1497,  1498,  1499,  1500,  1501,  1502,  1503,
    1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,  1512,  1513,
    1514,  1515,  1516,  1517,  1518,  1519,  1520,  1521,  1522,  1523,
    1524,  1525,  1526,  1527,  1528,  1529,  1530,  1531,  1535,  1536,
    1540,  1559,  1560,  1561,  1565,  1571,  1571,  1588,  1591,  1593,
    1591,  1605,  1607,  1605,  1622,  1640,  1658,  1676,  1687,  1688,
    1689,  1690,  1691,  1692,  1693,  1694,  1695,  1696,  1697,  1698,
    1699,  1700,  1701,  1702,  1703,  1704,  1705,  1706,  1708,  1706,
    1723,  1728,  1734,  1740,  1741,  1745,  1746,  1750,  1754,  1761,
    1762,  1773,  1777,  1780,  1788,  1788,  1788,  1791,  1797,  1800,
    1804,  1808,  1815,  1822,  1828,  1832,  1836,  1839,  1842,  1850,
    1853,  1861,  1867,  1868,  1869,  1873,  1874,  1878,  1879,  1883,
    1888,  1896,  1903,  1916,  1919,  1922,  1932,  1932,  1932,  1935,
    1935,  1935,  1940,  1940,  1940,  1948,  1948,  1948,  1954,  1964,
    1975,  1990,  1993,  1996,  1999,  2005,  2006,  2013,  2024,  2025,
    2026,  2030,  2031,  2032,  2033,  2034,  2038,  2043,  2051,  2052,
    2056,  2063,  2067,  2074,  2075,  2076,  2077,  2078,  2079,  2080,
    2081,  2085,  2086,  2087,  2088,  2089,  2090,  2091,  2092,  2093,
    2094,  2095,  2096,  2097,  2098,  2099,  2100,  2101,  2102,  2103,
    2104,  2105,  2109,  2115,  2122,  2134,  2140,  2148,  2156,  2167,
    2179,  2183,  2190,  2193,  2193,  2193,  2198,  2198,  2198,  2212,
    2216,  2220,  2224,  2230,  2238,  2244,  2252,  2260,  2271,  2280,
    2286,  2294,  2294,  2294,  2301,  2305,  2314,  2322,  2330,  2334,
    2337,  2345,  2346,  2347,  2354,  2355,  2356,  2357,  2358,  2359,
    2360,  2361,  2362,  2363,  2364,  2365,  2366,  2367,  2368,  2369,
    2370,  2371,  2372,  2373,  2374,  2375,  2376,  2377,  2378,  2379,
    2380,  2381,  2382,  2383,  2384,  2385,  2386,  2387,  2388,  2389,
    2395,  2396,  2397,  2398,  2399,  2414,  2423,  2424,  2425,  2426,
    2427,  2428,  2429,  2430,  2431,  2432,  2433,  2434,  2435,  2436,
    2437,  2437,  2437,  2445,  2446,  2447,  2452,  2455,  2455,  2455,
    2458,  2463,  2467,  2467,  2467,  2472,  2479,  2485,  2489,  2489,
    2489,  2494,  2497,  2503,  2503,  2503,  2510,  2515,  2519,  2519,
    2519,  2524,  2527,  2533,  2533,  2533,  2540,  2545,  2546,  2547,
    2548,  2549,  2550,  2551,  2552,  2553,  2555,  2559,  2560,  2565,
    2571,  2577,  2586,  2589,  2592,  2601,  2602,  2603,  2604,  2605,
    2606,  2607,  2611,  2615,  2619,  2623,  2627,  2631,  2635,  2639,
    2643,  2648,  2652,  2657,  2661,  2666,  2673,  2674,  2678,  2679,
    2680,  2684,  2685,  2689,  2690,  2691,  2695,  2696,  2700,  2712,
    2715,  2716,  2720,  2720,  2739,  2738,  2753,  2752,  2769,  2781,
    2790,  2800,  2801,  2802,  2803,  2804,  2808,  2811,  2820,  2821,
    2825,  2828,  2832,  2845,  2854,  2855,  2859,  2862,  2866,  2879,
    2880,  2884,  2890,  2896,  2905,  2908,  2915,  2918,  2924,  2925,
    2926,  2930,  2931,  2935,  2942,  2947,  2956,  2962,  2966,  2977,
    2984,  2993,  2996,  2999,  3006,  3010,  3016,  3028,  3031,  3036,
    3048,  3049,  3053,  3054,  3055,  3059,  3062,  3065,  3065,  3085,
    3088,  3088,  3106,  3111,  3119,  3120,  3124,  3127,  3140,  3157,
    3158,  3159,  3164,  3164,  3190,  3191,  3198,  3211,  3212,  3213,
    3217,  3227,  3230,  3236,  3237,  3241,  3242,  3246,  3247,  3251,
    3253,  3258,  3251,  3274,  3275,  3279,  3280,  3284,  3290,  3291,
    3292,  3293,  3297,  3298,  3299,  3303,  3306,  3312,  3314,  3319,
    3312,  3340,  3347,  3352,  3361,  3367,  3371,  3382,  3383,  3384,
    3385,  3386,  3387,  3388,  3389,  3390,  3391,  3392,  3393,  3394,
    3395,  3396,  3397,  3398,  3399,  3400,  3401,  3402,  3403,  3404,
    3405,  3406,  3407,  3408,  3409,  3410,  3411,  3412,  3413,  3414,
    3415,  3416,  3417,  3418,  3419,  3420,  3421,  3422,  3423,  3424,
    3425,  3426,  3427,  3428,  3429,  3430,  3431,  3435,  3436,  3437,
    3438,  3439,  3440,  3441,  3442,  3446,  3457,  3461,  3468,  3480,
    3487,  3493,  3502,  3507,  3517,  3527,  3537,  3550,  3551,  3552,
    3553,  3554,  3558,  3562,  3562,  3562,  3576,  3577,  3581,  3586,
    3593,  3596,  3599,  3602,  3608,  3611,  3625,  3626,  3630,  3631,
    3632,  3633,  3634,  3634,  3634,  3638,  3643,  3650,  3657,  3657,
    3664,  3664,  3671,  3675,  3679,  3684,  3689,  3694,  3699,  3703,
    3707,  3712,  3716,  3720,  3725,  3725,  3725,  3731,  3738,  3738,
    3738,  3743,  3743,  3743,  3749,  3749,  3749,  3754,  3760,  3760,
    3760,  3765,  3765,  3765,  3774,  3780,  3780,  3780,  3785,  3785,
    3785,  3794,  3800,  3800,  3800,  3805,  3805,  3805,  3814,  3814,
    3814,  3820,  3820,  3820,  3829,  3832,  3843,  3859,  3861,  3866,
    3871,  3859,  3897,  3899,  3904,  3910,  3897,  3936,  3938,  3943,
    3948,  3936,  3989,  3990,  3991,  3992,  3993,  3994,  3995,  3999,
    4000,  4001,  4002,  4003,  4007,  4014,  4021,  4027,  4033,  4040,
    4047,  4053,  4062,  4065,  4071,  4079,  4084,  4091,  4096,  4102,
    4103,  4107,  4108,  4112,  4112,  4112,  4120,  4120,  4120,  4127,
    4127,  4127,  4138,  4138,  4138,  4145,  4145,  4145,  4156,  4162,
    4162,  4162,  4176,  4197,  4197,  4197,  4207,  4207,  4207,  4221,
    4221,  4221,  4235,  4244,  4244,  4244,  4265,  4272,  4272,  4272,
    4282,  4285,  4296,  4302,  4325,  4333,  4355,  4381,  4382,  4386,
    4387,  4392,  4395,  4405
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

#define YYPACT_NINF (-1701)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-936)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1701,   202, -1701, -1701,    67,   -54,   -27,   495, -1701,   -81,
   -1701, -1701, -1701, -1701,   151,   590, -1701, -1701, -1701, -1701,
      75,    75,    75, -1701,    74, -1701,   231, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,   117, -1701,
     147,   220,   214, -1701,   200,   200, -1701, -1701, -1701,   257,
      75,    75, -1701, -1701,   231,   495,   495,   495,   298,   340,
   -1701, -1701, -1701, -1701,   590,   590,   590,   280, -1701,   710,
     192, -1701, -1701, -1701, -1701,   395, -1701,   849, -1701,   669,
      92,    67,   378,   -54,   317, -1701,   353,   350,   536,    22,
     435, -1701, -1701,   737,   437,   477,   481, -1701,   524,   499,
   -1701, -1701,   -42,    67,   590,   590,   590,   590,   514, -1701,
     749,   754,   666,   678,   819, -1701, -1701,   632, -1701, -1701,
     704, -1701, -1701, -1701,   768,   109, -1701, -1701, -1701, -1701,
     200,   200,   682,   200,   706,   718,   723, -1701, -1701,   721,
     726, -1701, -1701,   731,   734,   514,   514, -1701, -1701,   762,
   -1701,   111, -1701,   233,   777,   710, -1701,   792, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701,   796, -1701, -1701, -1701, -1701,
   -1701, -1701,   832, -1701, -1701, -1701, -1701,   897, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701,   198,   682,   682,   682, -1701,
   -1701,   957, 10546, 10546,   967, -1701,   609,   846, -1701, -1701,
   -1701, -1701, -1701, 11938, -1701,   836,   924,    79,    67,   883,
     861, -1701, -1701, -1701,   109, -1701, -1701,   842,   847,   848,
     831,   853,   860, -1701, -1701, -1701,   841, -1701, -1701, -1701,
   -1701, -1701,  -130, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701,   864, -1701, -1701, -1701,   869,   872,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701,   878,   892,   845,
     151,   162, -1701, -1701, -1701, -1701,  1131,   852,   891,   891,
   -1701, -1701, -1701, -1701, -1701, -1701,   909, -1701,   874,   880,
    2016, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,  1074,
    1087, -1701,   893, -1701,   514,  1069,   846, -1701,   928,   514,
   -1701, -1701,   832,   514,    67, -1701,   692, -1701, -1701, -1701,
   -1701, -1701,  9132, -1701, -1701,   933,   917,   -53,   -47,   -21,
   -1701, -1701,  9132,   405, -1701,  6657, -1701, -1701, -1701,    35,
   -1701, -1701, -1701,    36, -1701,  6882,   900, 10206, -1701,   894,
   -1701, -1701, 12066, 12183, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701,   937,   906, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,  1118, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701,   946,   912, -1701, -1701,   -96,   -64,  -127, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701,   907,   940, -1701,
     365, -1701,   514,   959, 10546, -1701,   116, 10546, 10546, 10546,
     943,   944, -1701, -1701,    58,   151,   951,    26, -1701,   455,
     926,   958,   963,   467,   966,   939,   478,   968, -1701,   502,
      37,   969, 10026, 10026,   327,   950,   952,   953,   954,   955,
     961, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, 10026, 10026, 10026, 10026, 10026,  4182,  5082, -1701, -1701,
   -1701, -1701, -1701, -1701,   962, -1701, -1701, -1701, -1701,   972,
   -1701, -1701,   -40,   -40, -1701,   -40,   -40,   936, 10825, -1701,
   -1701,   973, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   10546, 10546,   971,   970, 10546,   893, 10546,   893, 10546,   893,
   10713,  1012,   975, -1701,  6657, -1701, 10546,  9132,   976,  1010,
   -1701, -1701, -1701, -1701, -1701,   985, -1701, -1701,   987,  7107,
   -1701,  1131, -1701, 10713,  1012, -1701, -1701, -1701, -1701, -1701,
   -1701, 12237,  1523,  1360,   989, -1701,    54,   999,    12,  1005,
   10546, 10546, -1701,  9580, -1701, -1701, -1701, -1701,   151, -1701,
     773,  1007,  1204,   729, -1701, -1701, -1701,   888, -1701, -1701,
   -1701,  9132,    97,   513,  1033,   439, -1701, -1701, -1701, -1701,
    1015, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
     503, -1701,  1038,  1052,  1053, -1701,  6657, 10546,  9132,  9132,
   -1701, -1701, -1701,  9132, -1701,  9132, -1701,  6657, -1701, -1701,
    6657,  1054, -1701, 10546,   805,   805,  1035,  1036,   180, -1701,
   -1701,  9132,  9132,  9132,  9132,  9132,  9132,   887,   805,   805,
     303,   805,   805,  1037,  1260,  1041,  1042,   135,  1010,  1068,
    1044,   514,  3732,   590,  1273, -1701, -1701,   972, -1701, -1701,
   -1701, -1701, -1701, 11457, 11512, 10026, 10026, -1701, -1701, 10026,
   10026, 10026, 10026,  1088, 10026,   403,  9132,   438,   572,  9132,
    9132, 10026, 11639, 11694, 11821, 10026, 10026, 10026, 10026,  9132,
   10026, 10026, 10026, 10026,  9803, 10026, 10026, 10026, 10026, 10026,
   10026, 10026, 10026, 10026, 10026, 12010,  9132,  5307,   604,   617,
   -1701, -1701,  1097,   689,   -64,   712,   -64,   720,   -64,   -43,
   -1701,   520,   891,  1090, -1701,   603, -1701, 10546,  1010,   649,
     891, -1701, -1701,  7332, -1701, -1701, -1701, -1701,  1065,  1105,
   -1701,    75, -1701,    75, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701,  9132, -1701, -1701,   490,    32,    32,    32, -1701,
     891,   891, 10026, 10919, -1701,  1106, -1701, -1701, -1701, -1701,
    9132,  1109,   505, 10546,   116, -1701,  9132,    75, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, 10546, 10546, 10546, 10546,  4407,
    1110,  9132, 10546, -1701, -1701, -1701, 10546,  1010,   376, -1701,
    1103,  1076, 10546, 10546, 10546,  1078, 10546,  1079, 10546,  1010,
   10546, 10713,  1010, -1701,  1012,   355,  9132,  9132, 10546,   893,
    1080,  1081,  1082,  1083,  1084,  1091, -1701,  9132,   825,   119,
    1093, -1701,  9132, -1701,  9132, -1701,  9132,  1095,   602, -1701,
   -1701,  7557,   245,  3957, -1701,   206,  1096,   236,  1098,   893,
    2296,  1273,  1111,  1101, -1701, -1701,  1123,  1114, -1701, -1701,
     619,   619,  1810,  1810,  1073,  1073,  1116,   589,  1117, -1701,
    1112,  1119,  1120,  1121, -1701,  1124,  1126,   589,  1125,  1127,
   -1701, -1701,  1129,  1132, -1701, -1701,  1151,  1136, -1701, -1701,
     329,   329,   973,   619,   619, 10919, -1701, -1701, 11208, 11093,
   11180, 10919, 11876, 10702, 10366, 10214, 11295,  1810,  1810,  1102,
    1102,   589,   589,   589,   665,  9132,  1142,  1147,   693,  9132,
    1366,  1148,  1150, -1701,   210, -1701, -1701, -1701,    95, -1701,
    1169, -1701,  1170, -1701,  1172, 10546, -1701, 10713, 10546, -1701,
    1012,   659,  1155,  1158, 10546,  9132, -1701, -1701,  1185,   193,
   -1701, 10377, -1701,   301, -1701,  1159,  1162,  1354, -1701, -1701,
     -36, -1701, -1701, -1701, 11006,  2536,  1190, -1701,   193,    25,
    1165, -1701,  1167,  1361,   888,  9132,    75, -1701, -1701, -1701,
   -1701,   891,   386,   482,   730,   709,   241,  1173,  1174,   674,
    1176,   741, 10546, 10713,  1012,   582,  1177,  1171, 10546,  9132,
    1186, -1701,  1248,  1263,  1268, -1701,  1303, -1701,  1319,  1189,
    1356,   679,  1191, 10546,   833,  1273,  1193,  1196,  1374,   -64,
   -1701, -1701, -1701, -1701, -1701,  1198,  1227,  1205,  1398,  1245,
      15,   119,  1208, -1701, -1701, -1701,  1210,   141,  1211,  1206,
    1103,   181, -1701,  1213,   447,  5532, -1701, -1701, -1701,   -44,
     -64, -1701,  7782, -1701,  1212,  8007,  1252,  1255, -1701,    75,
    1264,  8232,   -24,  8457, -1701, -1701, -1701,    75,    75,  1446,
   -1701,  1019, -1701, -1701,  1445, -1701, -1701,  1450,  1418, -1701,
      75, -1701,    75,    75,    75,    75,    75, -1701,  1396, -1701,
      75,  1723,   893, -1701,  9132, -1701,  9132,  4632,  9132, -1701,
    9132,  5757,  9132, -1701, -1701, -1701,  9132, -1701,  9132, -1701,
    9132,  1251,  1233, -1701, -1701, 10026,  1235, -1701,  1238,  9132,
    4857,  1239, -1701,  1242, -1701,  5982, -1701,  7332, -1701, -1701,
   -1701,  1283, -1701,  1288, -1701, -1701, -1701, -1701, -1701, -1701,
     891, -1701, -1701,   891, -1701, -1701,  1158, -1701, -1701,   891,
   -1701,  9132, -1701,   606, -1701, -1701, -1701,  1246, -1701,  1250,
   -1701,  9132,  1291,   612, 10546, -1701,  9132,  1259,  9132,   672,
   -1701,  1305, -1701, -1701,  1498,   832, -1701,  9132,  1311, -1701,
    9132,    75, -1701, -1701, -1701, -1701,  1276, -1701, -1701, -1701,
    1279,  1320, -1701, -1701,  1454,   865,   870, -1701, -1701,  9132,
    1493, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701,  1515, -1701,   -14, -1701, -1701, -1701,  1307,  6207,
   -1701,  1315,  9132,  1326, -1701,   261,  6657,   153,    18,   254,
    9132,  9132,  9132,   119, -1701, -1701, -1701,   602,  1292,  3957,
     266,  1332,  1333,  1299,  1343,  1344, -1701,   283,   514,  9132,
   -1701,  1528,  9132, -1701,  1335,  1337, -1701,  1336,  1362, -1701,
    1212,  9132, -1701, -1701, -1701, -1701,  1318, -1701, -1701,  1322,
     -71,   -71,  1323, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
     282, -1701, 10026, 10026, 10026, 10026, 10026, 10026, 10026, 10026,
   10026, 10026,  9132, 10026, 10026, 10026, 10026, 10026, 10026, 10026,
     -64, 10546,  1314, 10546,  1324,  3957, -1701,   285,   287,  1327,
    1328, -1701,   310,  1329, 10546,  1331, 10546,  1338, 10546,  1339,
   -1701,  9132, 11006,  9132, -1701,  1342,  3957, -1701,   321,   331,
    9132, -1701, -1701, -1701,   342, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701,  1359, -1701,  1330, -1701, -1701,  1345, -1701,
    1347,  1535,    -3, -1701,  1545, -1701, -1701,  1346,  1376,   859,
    1521,    75,  1357, -1701,    75, -1701,  1368,  1369, -1701, -1701,
    9132,  1378, -1701, -1701, -1701, -1701,  1375,  1365,  1379,  1380,
    1382,  1383,  1384,  1385,  1524,  1386, -1701,  1387,  8682,  1212,
     226, -1701, -1701,   347, -1701, -1701,  1373, -1701,  1399, -1701,
     358,  1388,  1552,  1245,  6657,  9132,  9132,  1377, -1701, -1701,
      19, -1701,   449, -1701, -1701, -1701,  1427, -1701, -1701,   -44,
   -1701,  -127, -1701,  1212, -1701, 10546,  9132, -1701, -1701, -1701,
   -1701,  2776,  9132,  9132,    67,   883,  1393,  1394,  8907,  1245,
   -1701, -1701, -1701, 10825, 10825, 10825, 10825, 10825, 10825, 10825,
   10825, 10825, 10825, 10825, -1701, -1701, 10825, 10825, 10825, 10825,
   10825, 10825, 10825,   514,  1546, -1701,   785, -1701,   452,  6432,
   -1701, -1701, -1701, -1701, -1701, -1701,  1565, -1701,   802, -1701,
     808, -1701, 10546,  1395,  1397, -1701,   461,  6432, -1701, -1701,
    1400, -1701,  9132, -1701, -1701,  9132,  1432,  9132, -1701, -1701,
   -1701, 10546, -1701, -1701,   716, -1701,    20, -1701, -1701, -1701,
    1524,  1524,  1401,  1403,  1405,  1408,  1409,  6657, -1701,  9132,
    9132,  9132,  9132,  9132,  6657, -1701, -1701,  1524,  1411,  1524,
   -1701,  1413, -1701, -1701,   226, -1701,  1433, -1701, -1701,  1412,
    9132,  1449,   371,   372, -1701, -1701,   262,  6657,  1416,  1417,
   -1701, -1701, -1701,   891, -1701,  1424,  1430,  1436,   463,   119,
    9132,   200,  1437,   374,   -84,  -127, -1701, -1701,  1438,   396,
   -1701, -1701, -1701,   810, -1701, -1701,  1439,   401, -1701, -1701,
    1441, -1701, -1701,  1440,  -121,  1644,    20, -1701, -1701,   859,
      86,    86, -1701,  9132,  1524,  1524,   709,  1443,  1448,  1455,
    1456,  1463,  1468,  1010,    86,  1524,   709, -1701, -1701, -1701,
    9132,  1469, -1701, -1701,  1447,  9132,  9132,   414, -1701, -1701,
    1545,  1677,   514, -1701,    21,  1475,   258,   514,    66, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,  1644,   490,
     709,  1504,  1507, -1701,  1480,  1482,  1483,    86,    86,  1504,
    1484, -1701, -1701, -1701, -1701, -1701, -1701,  1485,  1486,   709,
    1487,  1488, -1701,  9132, -1701, -1701,  1489, -1701,  9357,    75,
   -1701,  6657,   514,   514,  1212, 10546,   116, -1701,  3016, 11938,
   -1701, -1701, -1701, -1701,   416,  1491, -1701, -1701, -1701, -1701,
    1494,  1497, -1701, -1701, -1701,  1500, -1701,  1670,  1503,  1488,
    9132, -1701, -1701, -1701, -1701, -1701, 10825, -1701,  1499,   423,
    1212,  1212, -1701,  1133,  9132,  1501,    75, 11938, -1701,   709,
   -1701, -1701, -1701,  9132, -1701,  1506,  1488, -1701,   564,  9357,
     514, -1701, -1701,  9132,    75, -1701, -1701,   514,   441, -1701,
   -1701,  1509, -1701,   514, -1701, -1701,  1512, -1701,    75,  1212,
      75, -1701,  -127, -1701, -1701,  3256, -1701,  9132, -1701, -1701,
   -1701, -1701,  1510,  1514,  1505,  1545, -1701, -1701,  9357,   514,
   -1701, -1701,    75, -1701,  3496, -1701,  1514,  1511,   564,  1545,
   -1701, -1701
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   173,     1,   395,     0,     0,    58,   739,   396,     0,
     957,   947,   952,    20,     0,     0,    19,    16,    15,     3,
       0,     0,     0,     8,   777,     7,   720,     6,    11,     5,
       4,    13,    12,    14,   137,   138,   139,   136,   147,   149,
      49,    67,    64,    65,     0,     0,    50,   741,   740,     0,
       0,     0,    26,    25,   720,   739,   739,   739,     0,   369,
      47,   157,   158,   159,     0,     0,     0,   160,   162,   169,
       0,   156,    21,    10,     9,   318,   759,     0,   721,   722,
       0,     0,     0,     0,     0,    51,     0,     0,    59,    62,
     742,   744,   745,    22,     0,     0,     0,   371,     0,     0,
     168,   163,     0,     0,     0,     0,     0,     0,    76,   319,
     321,   747,   769,   768,   772,   724,   723,   730,   145,   146,
       0,   143,   144,   141,     0,     0,   140,   150,    68,    66,
       0,     0,    52,     0,     0,     0,     0,    63,    60,     0,
       0,    23,    24,    27,   857,    76,    76,   370,    45,    48,
     167,     0,   164,   165,   166,   170,    74,    77,   174,   323,
     322,   325,   320,   749,   748,     0,   771,   770,   774,   773,
     778,   725,   646,   142,    30,    31,    35,     0,   132,   133,
     130,   131,   129,   128,   134,     0,    54,    55,    53,    57,
      56,    62,     0,     0,     0,    29,     0,   757,   948,   953,
      46,   161,    75,     0,   750,   751,   765,   727,     0,   647,
       0,    32,    33,    34,     0,   148,    61,     0,     0,     0,
       0,     0,     0,   787,   830,   788,   846,   789,   793,   794,
     795,   796,   836,   800,   801,   802,   803,   804,   805,   806,
     831,   832,   833,   834,   917,   792,   799,   835,   924,   931,
     790,   797,   791,   798,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,   817,   818,   819,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,     0,     0,     0,
       0,   845,   878,   881,   879,   880,   944,   874,   746,   743,
      28,   860,   861,   858,   859,   755,   758,   958,     0,     0,
       0,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,     0,
       0,   181,   175,   269,    76,     0,   757,   766,     0,    76,
     729,   726,   646,    76,     0,   709,   701,   731,   135,   882,
     908,   911,     0,   914,   904,     0,     0,   918,   925,   932,
     938,   941,     0,   876,   888,   363,   894,   899,   893,     0,
     907,   903,   896,     0,   898,     0,   875,     0,   756,     0,
     949,   954,   260,   261,   258,   184,   185,   187,   186,   188,
     189,   190,   191,   217,   218,   215,   216,   208,   219,   220,
     209,   206,   207,   259,   242,     0,   257,   221,   222,   223,
     224,   195,   196,   197,   192,   193,   194,   205,     0,   211,
     212,   210,   203,   204,   199,   198,   200,   201,   202,   183,
     182,   241,     0,   213,   214,   646,   178,     0,   837,   840,
     843,   844,   838,   841,   839,   842,   752,     0,   763,   779,
       0,   151,    76,     0,     0,   702,     0,     0,     0,     0,
       0,     0,   520,   521,     0,     0,     0,     0,   514,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   836,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,   630,   443,   445,   444,   446,   447,   448,   449,   450,
      41,     0,     0,     0,     0,     0,   363,     0,   428,   429,
    1032,   518,   517,   617,   515,   610,   609,   608,   607,   171,
     613,   516,   612,   611,   565,   522,   566,     0,   511,   573,
     523,     0,   519,   969,   971,   970,   512,   973,   972,   513,
       0,     0,     0,   863,     0,   175,     0,   175,     0,   175,
       0,     0,     0,   890,     0,   887,     0,     0,     0,  1039,
     421,   901,   902,   895,   897,     0,   900,   871,     0,     0,
     946,   945,   959,   680,   686,   262,   264,   263,   265,   256,
     240,   266,   243,   225,     0,   176,   394,   671,   672,     0,
       0,     0,   324,     0,   331,   425,   326,   760,     0,   767,
       0,     0,   703,   701,   728,   152,   710,     0,   699,   700,
     698,     0,     0,     0,     0,   868,   993,   996,   374,   845,
     378,   377,   383,   962,   968,   963,   964,   965,   967,   966,
       0,   415,     0,     0,     0,  1023,     0,     0,     0,     0,
     406,   409,   570,     0,   412,     0,  1027,     0,  1005,  1009,
       0,     0,   999,     0,   550,   551,     0,     0,   483,   480,
     482,     0,     0,     0,     0,     0,     0,     0,   527,   526,
     567,   525,   524,     0,     0,     0,     0,   369,  1039,  1039,
       0,    76,     0,     0,   438,   430,   360,   171,   337,   335,
     336,   334,   885,     0,     0,     0,     0,   552,   553,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   501,     0,     0,     0,     0,
     847,   862,     0,     0,   178,     0,   178,     0,   178,   369,
     678,     0,   676,     0,   684,     0,   848,     0,  1039,     0,
     367,   422,   886,  1040,   364,   892,   870,   873,     0,   852,
     681,    94,   687,    94,   267,   268,   245,   246,   248,   247,
     249,   250,   251,   252,   244,   253,   254,   255,   229,   230,
     232,   231,   233,   234,   235,   236,   227,   228,   237,   238,
     239,   226,     0,   392,   393,     0,   646,   646,   646,   177,
     180,   179,     0,   426,   360,   736,   764,   775,   659,   780,
       0,     0,     0,     0,     0,   717,     0,     0,   883,   909,
     912,    18,    17,   866,   867,     0,     0,     0,     0,   991,
       0,     0,     0,  1013,  1016,  1019,     0,  1039,     0,  1030,
    1039,     0,     0,     0,     0,     0,     0,     0,     0,  1039,
       0,     0,  1039,  1002,     0,     0,     0,     0,     0,   175,
       0,     0,     0,     0,     0,     0,    44,     0,    42,     0,
       0,  1012,     0,   690,     0,   689,     0,     0,  1040,   984,
     555,   365,     0,   363,   504,     0,     0,     0,     0,   175,
       0,   438,     0,     0,   581,   580,     0,     0,   587,   591,
     528,   529,   541,   542,   539,   540,     0,   574,     0,   562,
       0,     0,   491,     0,   564,     0,     0,   575,     0,     0,
     586,   585,     0,     0,   592,   596,     0,     0,   602,   606,
     614,   615,   616,   530,   531,   619,   620,   621,   546,   547,
     548,   549,     0,     0,   544,   545,   543,   537,   538,   533,
     532,   534,   535,   536,     0,     0,     0,   489,     0,     0,
       0,     0,     0,   509,     0,   915,   905,   849,     0,   919,
       0,   926,     0,   933,     0,     0,   939,     0,     0,   942,
       0,     0,     0,   876,     0,     0,   423,   872,   853,   753,
      92,    95,   950,    95,   955,     0,     0,   781,   668,   669,
     691,   673,   675,   674,   427,     0,   732,   737,   753,   662,
       0,   705,     0,   706,     0,     0,     0,   719,   884,   910,
     913,   869,     0,     0,     0,   992,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1040,
       0,   568,     0,     0,     0,   569,     0,   618,     0,     0,
       0,     0,     0,     0,     0,   438,     0,     0,     0,   178,
     625,   626,   627,   628,   629,     0,    38,     0,   110,     0,
       0,     0,     0,   975,   974,   554,     0,     0,     0,     0,
    1039,     0,   505,     0,     0,     0,   508,   506,   172,     0,
     178,   362,   386,   384,     0,     0,     0,     0,   385,     0,
       0,     0,    76,     0,   357,   442,   338,     0,     0,     0,
     351,     0,   352,   346,     0,   343,   342,     0,     0,   344,
       0,   361,     0,    90,    91,    88,    89,   353,   398,   341,
       0,   451,   175,   577,     0,   588,     0,     0,     0,   559,
       0,     0,     0,   558,   561,   582,     0,   593,     0,   603,
       0,     0,     0,   597,   601,     0,     0,   563,     0,     0,
       0,     0,   490,     0,   502,     0,   556,     0,   510,   916,
     906,     0,   864,     0,   920,   922,   927,   929,   934,   936,
     677,   940,   679,   683,   943,   685,   876,   877,   889,   368,
     424,     0,   734,   754,   960,    93,   682,     0,   688,     0,
     670,     0,     0,     0,     0,   692,     0,     0,     0,   754,
     761,     0,   660,   776,     0,   646,   704,     0,     0,   714,
       0,     0,   718,   994,   997,   375,     0,   380,   381,   379,
       0,     0,   418,   416,     0,     0,     0,  1024,  1022,   365,
       0,  1031,  1034,   407,   410,   571,   413,  1028,  1026,  1006,
    1010,  1008,     0,  1000,    76,   481,   645,   484,     0,     0,
      39,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1033,   366,   507,     0,     0,   363,
       0,     0,     0,     0,     0,     0,   436,     0,    76,     0,
     387,     0,     0,   372,     0,     0,   356,     0,     0,    71,
       0,     0,   389,   360,   354,   355,     0,    83,    84,     0,
     153,   153,     0,   345,   340,   347,   348,   349,   350,   397,
       0,   339,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     178,     0,     0,     0,     0,   363,   475,     0,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
     598,     0,   576,     0,   560,     0,   363,   492,     0,     0,
       0,   557,   503,   499,     0,   851,   865,   850,   923,   930,
     937,   891,   854,   855,   735,     0,   951,   956,     0,   783,
       0,   784,   694,   693,   327,   733,   738,     0,     0,   653,
     656,     0,     0,   708,     0,   716,     0,     0,   376,   382,
       0,     0,   417,  1014,  1017,  1020,     0,     0,     0,     0,
       0,     0,     0,     0,   991,     0,  1003,     0,     0,     0,
       0,   487,   631,     0,    36,    43,     0,   112,     0,   113,
       0,     0,   114,     0,     0,     0,     0,     0,   977,   976,
       0,   472,     0,   474,   433,   434,     0,   432,   431,     0,
     439,     0,   388,     0,   373,     0,     0,    69,    70,   120,
     390,     0,     0,     0,     0,   155,     0,     0,     0,     0,
     711,   404,   403,   463,   464,   466,   465,   467,   457,   458,
     459,   468,   469,   453,   454,   455,   456,   470,   471,   460,
     461,   462,   452,    76,     0,   643,     0,   639,     0,     0,
     476,   479,   634,   636,   495,   638,     0,   644,     0,   640,
       0,   642,     0,     0,     0,   633,     0,     0,   493,   498,
       0,   500,     0,   961,   782,     0,     0,     0,   328,   333,
     762,     0,   654,   655,   656,   657,   648,   663,   707,   715,
     991,   991,     0,     0,     0,     0,     0,   363,  1035,   365,
       0,     0,     0,     0,     0,   992,  1007,   991,     0,   991,
     622,     0,   624,   485,     0,   632,    40,   111,   400,     0,
       0,     0,     0,     0,   979,   978,     0,     0,     0,     0,
     437,   440,   391,   127,   126,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   578,   589,     0,     0,
     583,   594,   604,     0,   641,   637,     0,     0,   635,   856,
       0,   786,   695,     0,     0,   651,   648,   649,   650,   653,
     990,   990,   419,     0,   991,   991,   982,     0,     0,     0,
       0,     0,     0,  1039,   990,   991,   982,   623,   488,    37,
       0,     0,   116,   117,     0,     0,     0,     0,   473,   435,
     327,    85,    76,   154,     0,     0,     0,    76,   701,   405,
     712,   713,   441,   579,   590,   477,   478,   584,   595,   605,
     599,   496,   497,   785,   360,   661,   652,   664,   651,     0,
       0,   987,  1039,   989,     0,     0,     0,   990,   990,   983,
       0,  1025,  1036,   408,   411,   572,   414,     0,     0,   982,
       0,  1037,   115,     0,   981,   980,     0,   359,     0,     0,
     107,     0,    76,    76,     0,     0,     0,   600,     0,     0,
     666,   697,   696,   658,     0,  1040,   988,   995,   998,   420,
       0,     0,  1021,  1029,  1011,     0,  1001,     0,     0,  1037,
       0,    86,    90,    91,    88,    89,    87,   109,    99,     0,
       0,     0,   124,     0,     0,     0,     0,     0,   985,     0,
    1015,  1018,  1004,     0,  1041,     0,  1037,    96,    78,     0,
      76,   122,   125,     0,     0,   330,   665,    76,     0,  1038,
    1042,     0,   360,    76,    72,    73,     0,   108,     0,     0,
       0,   402,     0,   986,  1043,     0,    79,     0,   100,   119,
     401,   667,     0,   104,     0,   327,   101,    80,     0,    76,
      98,   360,     0,    81,     0,   105,   104,     0,    78,   327,
      82,   103
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1701, -1701,  -976,    -1, -1701, -1701, -1701, -1701, -1701,  1056,
    1678, -1701, -1701, -1701, -1701, -1701, -1701,  1759, -1701, -1701,
   -1701,   -32, -1701, -1701,  1573, -1701, -1701,  1682, -1701, -1701,
   -1701, -1701,  -142,   -91, -1701, -1701, -1701, -1701, -1700,   977,
     978, -1701, -1701, -1701, -1701,   -88, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1085, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701,  1555, -1701, -1701,   -49,   -98,  -299,   431, -1701,
   -1701,   600,  1070,  1072,   736,  -495,  -709, -1701,  -366, -1701,
   -1701, -1701, -1609, -1701, -1701, -1644, -1701, -1701, -1107, -1701,
   -1701, -1701, -1701, -1701, -1701,  -823,  -379, -1242,  1000,   -13,
   -1701, -1701, -1701, -1701, -1701, -1690, -1688, -1675, -1672, -1701,
   -1701,  1775, -1701, -1198, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701,  -456, -1457,   464,
     293, -1701,  -854, -1701,   211, -1701, -1701, -1701, -1701, -1413,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,   516,
    -380, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701,  -164,   127,    68,
     125,   215, -1701, -1701, -1701, -1701, -1701, -1701, -1701,   284,
    -568,  -836, -1701,  -572,  -803, -1701, -1012,    69,    71, -1701,
    -615,  -616, -1701, -1701, -1701, -1320, -1701,  1737, -1701, -1701,
   -1701, -1701, -1701,   553,   755, -1701,  1061, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701,   756, -1701,  1442, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701,    52, -1701,  1312, -1701, -1701, -1701,  1567, -1701,
   -1701, -1701,  -609, -1701, -1701,  -369,  -973, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701,  -183, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701,  -723, -1599,  -673, -1701, -1701, -1390,
   -1343,  1313, -1701, -1701, -1701, -1701, -1701, -1701, -1701, -1701,
   -1701, -1701,  1316, -1701, -1701,  1317, -1701, -1701, -1701, -1701,
   -1701, -1701, -1701, -1701, -1701, -1701,  1144, -1701,  -470,  1321,
   -1641,  -675,  1325,  -469
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   853,   854,    18,   143,    54,   195,    19,   177,
     183,  1606,  1291,  1464,   687,   531,   149,   532,    99,    21,
      22,    88,    45,    46,   138,    23,    42,    43,  1137,  1138,
    1826,   157,   158,  1827,  1843,  1856,  1339,  1749,  1139,  1021,
    1022,  1808,  1822,  1842,  1809,  1847,  1851,  1857,  1848,  1140,
    1141,  1788,  1142,  1100,  1143,  1144,  1145,  1146,  1147,  1148,
    1149,  1150,   184,   185,    38,    39,    40,   209,  1506,    67,
      68,    69,    70,   704,    24,   456,   612,   352,   353,   110,
      25,   161,   354,   162,   203,  1569,  1653,  1795,   615,   834,
    1237,   533,  1151,  1333,  1625,   920,   695,  1109,   779,   534,
    1152,   640,   858,  1438,   535,  1153,  1154,  1155,  1156,  1157,
     825,  1158,  1350,  1295,  1511,  1159,   536,   872,  1449,   873,
    1450,   876,  1452,   537,   862,  1442,   538,   579,   616,   539,
    1316,  1317,   919,   540,   708,   541,  1160,   542,   543,   680,
     544,   888,  1460,   889,  1604,   545,   990,  1402,   546,   580,
     548,   874,  1451,  1371,  1703,  1384,  1707,  1373,  1704,  1386,
    1708,  1552,  1757,  1388,  1709,   549,   550,   606,  1659,  1717,
    1574,  1576,  1431,  1039,  1245,  1759,  1797,   607,   608,   609,
     770,   771,   791,   774,   775,   793,   906,  1028,  1029,  1763,
     631,   476,   623,   366,  1634,   624,   367,    79,   117,   207,
     362,    27,   172,  1037,  1223,  1038,    49,    50,   140,    28,
      51,   165,   205,   356,  1224,   296,   297,    29,   111,   835,
    1427,   619,   358,   359,   114,   170,   839,    30,    77,   206,
     620,  1030,   551,   466,   283,   284,   998,  1019,   197,   285,
     762,  1406,  1007,   634,   396,   286,   575,   287,   477,  1048,
     576,   777,   561,  1200,   478,  1049,   479,  1050,   560,  1199,
     564,  1204,   565,  1408,   566,  1206,   567,  1409,   568,  1208,
     569,  1410,   570,  1211,   571,  1214,   772,    31,    56,   298,
     593,  1227,    32,    57,   299,   594,  1229,    33,    55,   399,
     789,  1415,   642,   552,   699,  1730,   700,  1722,  1723,  1724,
    1058,   553,   856,  1436,   857,  1437,   884,  1457,  1083,  1598,
     880,  1454,   554,   881,  1455,   555,  1062,  1584,  1063,  1585,
    1064,  1586,   866,  1446,   878,  1453,  1110,   701,   556,   557,
    1778,   784,   558,   559
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,    60,    71,   198,   199,   151,   578,   844,   210,   288,
     289,  1035,   846,    89,   646,   649,  1298,  1321,  1236,    72,
      73,    74,   792,   907,   909,   790,   855,  1447,   591,   916,
    1512,   652,   127,  -173,  1621,  1296,   596,   598,  1474,  1617,
    1218,  1751,   613,  1222,   136,  1081,  1747,  1603,  1781,    91,
      92,    71,    71,    71,   132,  1000,   671,  1002,  1782,  1004,
    1783,   823,  1222,   480,   481,  1241,  1657,  1162,   581,   583,
     764,   698,   766,  1784,   768,   208,  1785,  1740,    13,   137,
     196,  1084,    75,   487,  -857,   610,    34,    35,   604,   489,
     397,    71,    71,    71,    71,  1302,  -921,   621,   186,   187,
     208,   188,  -928,  1012,   824,   627,  1311,   118,   119,  1828,
      76,  1596,   614,   622,   903,    13,    52,    16,   778,  1782,
    1312,  1783,   674,   675,   178,   179,   496,   497,  -935,    98,
     705,   104,   105,   106,  1784,    41,    36,  1785,  1805,   605,
    1775,   688,   689,   690,   691,   692,   611,   903,  1852,  1658,
    1796,   653,   654,  1313,    16,    53,  1699,  -921,  1782,   363,
    1783,  1504,  -921,  -928,  1314,  1821,  1010,  1005,  -928,  1315,
    1014,  1233,   905,  1784,  1234,   156,  1785,  1235,  1702,   281,
     281,  -921,    44,   499,   500,   156,  1057,  -928,  1817,  -935,
    1114,  1678,  1066,   150,  -935,  1070,   528,   529,   470,   706,
     867,  1850,     2,   208,  1079,   905,   361,  1082,   397,     3,
     665,   879,   457,  -935,   882,  1861,   604,   469,  1480,   827,
    1458,   471,  1297,  1499,  1242,  1297,  1297,  1265,  1297,  1045,
      58,  1284,     4,   833,     5,   795,     6,  1660,  1661,  1113,
     655,   134,     7,  1411,   282,   282,   915,  1414,   582,   584,
     828,   672,     8,   135,  1674,    59,  1676,    15,     9,  1068,
     656,  1266,   120,  1414,    37,  1243,   628,   383,   585,   966,
     967,  1725,   638,   696,    13,  1612,  1755,   586,    13,   475,
     629,   121,    10,  1111,  1738,   903,   122,  1476,   868,   123,
     526,   903,   124,    78,   632,   633,   635,   530,   180,   904,
    1097,   994,  1201,   181,   885,  1301,   182,    98,   397,   124,
    1202,  1633,   848,    16,  1701,   472,  1098,    16,    81,   360,
    1720,  1727,  1728,    58,   630,    80,   125,    11,    12,  -486,
     625,   903,  1739,  1203,  1471,   930,   931,  1770,  1771,   932,
     933,   934,   935,   905,   937,   904,   201,  1668,    59,   905,
    1472,   947,  1602,  1099,    81,   960,   961,   963,   964,   965,
     968,   969,   970,   971,   973,   974,   975,   976,   977,   978,
     979,   980,   981,   982,   983,  1841,   384,   758,   759,    84,
    1288,   763,  1256,   765,   281,   767,  1622,  1473,   386,   905,
    -486,   851,  1310,   780,  1089,  -486,   385,    85,    58,   107,
     852,    13,  1014,  1056,   903,   214,   104,   676,   106,   386,
    1567,  1318,   903,  1115,  -486,   387,   388,  1197,  1475,   386,
      86,    83,    14,    59,  1120,   108,  1685,   830,   831,  1250,
      82,    87,   696,   215,    15,  1305,   387,   388,  1215,  1212,
      16,  1116,  1111,   107,  1378,  1198,   387,   388,  1197,   282,
    1261,  1228,  1034,  1226,    90,   725,   726,   727,   728,   729,
     730,   281,   905,   621,   281,   281,   281,  1399,  1468,  1118,
     905,   639,   650,  1307,  1044,  1010,  1257,   134,   723,   622,
    1112,   725,   726,   727,   728,   729,   730,  1052,  1053,   135,
    1489,   679,  1539,  1753,  1307,    97,  1469,  1065,   773,    58,
      13,  1483,   389,  1072,  1073,  1074,   390,  1076,   676,  1078,
    1501,  1080,    98,   677,   103,   386,  1509,  1197,  1490,  1088,
    1540,  1510,  1541,   389,    59,   109,   282,   390,  1557,   282,
     282,   282,    84,   389,   755,   756,   130,   390,  1307,    16,
    1161,   678,   387,   388,  1694,  1544,   621,   281,   281,  1197,
      85,   281,    47,   281,  1197,   281,  1558,   281,    48,   911,
     755,   756,   622,   281,   391,  1197,  1559,   128,   392,   133,
    1085,   393,   131,    86,   851,    13,  1823,  1561,  1468,  1197,
     281,  1468,  1605,   852,    87,   391,  1482,  1824,  1825,   392,
     938,  1067,   393,  1609,   780,   391,   394,   281,   281,   392,
     939,  1253,   393,  1307,  1686,   836,  1683,  1684,  1307,  1698,
     713,   714,   282,   282,    16,   386,   282,   394,   282,   573,
     282,  1197,   282,  1307,    61,   941,   845,   394,   282,   389,
    1197,  1706,   139,   390,   144,   942,  1712,   851,    13,   574,
     713,   714,   387,   388,   281,   282,   852,  1792,  1307,  1746,
     397,  1798,  1538,    62,  1307,  1161,  1307,  1010,  1810,  1307,
     281,  1533,   282,   282,   100,   101,   102,  1370,  1307,   657,
      81,  1026,  1051,  1556,   145,  1054,  1833,    16,   146,  1061,
    1308,   662,  1618,  1811,  1812,  1638,  1042,  1027,   291,   658,
      71,   391,   666,  1264,  1646,   392,  1693,  1254,   393,  1270,
     148,   663,  1043,   292,   152,   153,   154,   155,   293,   282,
     294,  1377,   667,   156,  1282,  1382,   669,   860,   851,    13,
      63,   147,  1839,   394,   397,   282,   115,   852,   849,   389,
     717,   718,   116,   390,  1398,  1006,   670,   861,   723,  1404,
     724,   725,   726,   727,   728,   729,   730,   731,   732,   733,
     734,   735,   736,    64,   709,   134,   710,   711,    16,   943,
     717,   718,    58,  1655,   281,   925,   929,   135,   723,   944,
     724,   725,   726,   727,   728,   729,   730,   731,   732,   733,
     734,   735,   736,  1575,   951,   955,   959,    59,   962,  1106,
    1020,   391,  1020,  1420,   141,   392,   166,  1267,   393,  1107,
     142,   851,    13,  1413,   851,  1392,   159,   991,   167,  1421,
     852,   163,   160,   852,    65,   397,  1639,   164,  1009,   995,
     755,   756,  1210,   394,    66,  1213,   713,   714,   397,   282,
     281,  1219,   996,  1463,  1647,   750,   751,   752,   753,   754,
    1470,    16,   281,   281,   281,   281,  1047,   851,    13,   281,
     755,   756,  1186,   281,   112,   113,   852,   851,    13,   281,
     281,   281,  1187,   281,  1013,   281,   852,   281,   281,  1036,
     851,   171,   851,    13,  1216,   281,   168,   851,    13,   852,
    1191,   852,   169,   104,   105,   106,   852,    16,   547,  1260,
    1192,  1758,  1524,  1525,  1280,   282,   696,    16,   572,   473,
     397,   173,   474,   189,   999,   475,  1111,   282,   282,   282,
     282,   588,    16,   135,   282,   190,  1572,    16,   282,  1136,
     191,   386,  1573,   397,   282,   282,   282,  1001,   282,   192,
     282,   397,   282,   282,   193,  1003,   842,  1721,  1721,   843,
     282,   397,   475,  1729,   196,  1255,   717,   718,   387,   388,
     104,  1721,   397,  1729,   723,   194,  1263,   725,   726,   727,
     728,   729,   730,   200,   732,   733,   734,   735,   736,   174,
     175,   176,  1513,  1514,  1515,  1516,  1517,  1518,  1519,  1520,
    1521,  1522,  1523,  1526,  1527,  1528,  1529,  1530,  1531,  1532,
    1330,   202,   281,   204,   281,   281,   397,  1764,  1737,  1835,
    1637,   281,  1534,   208,  1721,  1721,   707,   707,   281,   707,
     707,   837,   838,   397,   137,  1546,  1729,  1641,  1613,   397,
    1225,   397,  1225,  1642,  1184,  1710,   211,   212,  1854,  1337,
    1338,   851,    13,   290,  1136,   389,   755,   756,   628,   390,
     852,  1505,  1505,  1249,   295,  1252,   355,  1766,  1283,   281,
     281,  1422,   629,   357,   364,   281,   369,   282,   365,   282,
     282,   370,   371,   851,    13,   372,   282,   373,   851,    13,
     281,    16,   852,   282,   374,   375,  1818,   852,   377,   382,
    1444,  1430,  1756,   378,   395,  1445,   379,    13,   174,   175,
     896,   897,   380,   781,   713,   714,   630,   391,   211,   212,
     213,   392,   397,    16,   393,   788,   381,   398,    16,   453,
    1031,  1032,  1033,   400,   282,   282,    94,    95,    96,   401,
     282,  1161,   454,   713,   714,   468,    16,   455,  1326,   394,
     562,   563,   589,   592,   599,   282,  1334,  1335,  1673,   600,
     601,  1794,  1459,   602,   458,   603,   617,   847,   459,  1343,
     618,  1344,  1345,  1346,  1347,  1348,   626,   636,   637,  1351,
     659,  1687,   460,   461,   386,   651,   386,   462,   463,   464,
     465,   712,   660,   665,   869,   871,  1491,   661,  1813,   875,
     664,   877,   668,   673,   681,   761,   682,   683,   684,   685,
    1536,   387,   388,   387,   388,   686,   702,   890,   891,   892,
     893,   894,   895,  1548,   703,  1550,   760,   757,  1667,   773,
     776,   782,   715,   716,   717,   718,   719,   783,   785,   720,
     786,   281,   723,   822,   724,   725,   726,   727,   728,   729,
     730,   731,   732,   733,   734,   735,   736,   826,   737,   738,
     829,   840,   940,   717,   718,   945,   946,   841,   850,   859,
    1435,   723,   863,   724,   725,   726,   727,   728,   729,   730,
     731,   732,   733,   734,   735,   736,   864,   865,   883,   886,
     887,   899,   992,   900,   901,   908,   902,   918,   389,   910,
     389,   386,   390,   628,   390,   936,   282,   748,   749,   750,
     751,   752,   753,   754,   997,  1789,   386,   629,  1017,  1016,
    1008,   386,  1018,  1036,   755,   756,  1041,  1059,   387,   388,
    1069,  1071,  1623,  1075,  1077,  1090,  1091,  1092,  1093,  1094,
     752,   753,   754,   387,   388,  1163,  1095,  1101,   387,   388,
    1105,  1117,  1119,   755,   756,  1164,   386,  1165,  1025,  1175,
     391,   630,   391,  1177,   392,  1169,   392,   393,  1166,   393,
    1167,  1168,   386,  1170,  1171,  1172,  1040,  1173,   281,  1174,
     281,  1176,  1046,   387,   388,  1179,  1178,  1194,  1786,  1643,
    1180,   281,   394,   281,   394,   281,  1189,  1060,  1161,   387,
     388,  1190,  1195,  1196,  1205,  1207,  1793,  1209,  1654,   386,
    1217,  1635,   574,  1221,  1230,   389,  1231,  1232,  1238,   390,
    1246,  1247,  1086,  1087,  1248,  1269,  1628,   386,  1258,  1259,
     389,  1262,  1268,  1096,   390,   389,   387,   388,  1102,   390,
    1103,  1272,  1104,   282,  1278,   282,  1281,   869,  1285,  1786,
    1577,  1286,  1289,  1579,   387,   388,   282,  1290,   282,  1292,
     282,  1293,  1294,  1299,  1300,  1303,  1304,   679,  1306,  1324,
     389,   614,  1325,  1327,   390,  1161,  1336,   391,  1340,  1341,
    1342,   392,  1349,  1273,   393,  1390,   389,  1391,  1786,  1393,
     390,  1394,   391,  1400,  1161,  1401,   392,   391,  1274,   393,
    1405,   392,   281,  1275,   393,  1407,  1416,   386,  1419,   394,
    1417,   808,   809,   810,   811,   812,   813,   814,   815,  1424,
    1136,  1188,  1428,   389,   394,  1193,  1429,   390,  1433,   394,
     816,  1439,   391,  1440,   387,   388,   392,  1441,  1276,   393,
    1465,   389,  1461,  1467,   817,   390,   386,  1481,   391,  1484,
    1485,  1220,   392,  1486,  1277,   393,   818,   819,   820,   281,
    1487,  1488,  1493,  1495,   394,  1496,  1497,   282,   386,  1535,
    1750,  1498,  1502,   387,   388,  1754,  1503,  1508,   281,  1537,
     394,  1251,  1542,  1543,  1545,   391,  1547,  1562,   821,   392,
    1563,  1279,   393,  1549,  1551,   387,   388,  1555,  1566,   386,
    1564,  1565,  1568,   391,  1571,  1271,  1570,   392,  1575,  1287,
     393,   679,  1578,  1583,  1595,  1611,  1608,   394,   386,  1696,
    1588,   389,  1580,  1581,   282,   390,   387,   388,  1607,  1587,
    1790,  1791,  1616,  1589,  1590,   394,  1591,  1592,  1593,  1594,
    1597,  1599,  1610,   282,  1619,   387,   388,  1629,  1630,  1651,
    1644,  1016,  1645,  1700,  1679,  1648,  1662,  1663,  1320,  1664,
     389,  1323,  1665,  1666,   390,  1675,  1682,  1329,  1677,  1332,
    1680,  1688,  1689,  1715,   796,   797,   798,   799,   800,   801,
     802,   803,   389,   391,  1690,  1691,   390,   392,  1829,  1443,
     393,  1692,  1697,  1705,  1711,  1832,  1713,  1716,  1731,  1714,
    1372,  1836,  1374,  1732,  1379,  1743,  1380,   804,  1383,  1748,
    1733,  1734,  1385,   389,  1387,   394,  1389,   390,  1735,   805,
     806,   807,   391,  1736,  1742,  1395,   392,  1853,  1448,   393,
    1752,  1307,   389,  1016,  1765,  1767,   390,  1768,  1769,  1772,
    1773,  1774,  1776,  1803,   391,  1799,  1777,  1780,   392,  1800,
    1456,   393,  1801,   -87,   394,  1802,  1804,  1412,  1807,  1820,
    1849,  1815,   281,   898,   713,   714,  1837,  1418,  1787,  1834,
    1845,  1859,  1423,  1846,  1425,   391,   394,  1136,   126,   392,
      20,  1636,   393,  1432,   216,   129,  1434,  1860,  1858,   368,
    1023,  1024,  1507,   917,   391,  1244,    26,  1011,   392,   921,
    1640,   393,  1620,  1718,  1719,   869,  1760,   394,  1761,  1656,
    1762,    93,  1426,  1239,  1240,  1816,   641,   643,   467,   376,
     644,   645,   870,     0,     0,   647,   394,   282,  1466,   648,
       0,     0,     0,  1831,     0,     0,  1477,  1478,  1479,     0,
       0,     0,     0,     0,     0,     0,     0,  1838,     0,  1840,
       0,   713,   714,     0,  1136,  1492,     0,     0,  1494,     0,
       0,     0,     0,     0,     0,     0,     0,  1500,     0,     0,
       0,  1855,     0,  1136,  1352,  1353,  1354,  1355,  1356,  1357,
    1358,  1359,   715,   716,   717,   718,   719,  1360,  1361,   720,
     721,   722,   723,  1362,   724,   725,   726,   727,   728,   729,
     730,   731,   732,   733,   734,   735,   736,  1363,   737,   738,
    1364,  1365,   739,     0,     0,     0,   740,   741,   742,  1366,
    1367,  1368,   743,     0,     0,     0,     0,  1553,     0,  1554,
       0,     0,     0,     0,     0,     0,  1560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1369,   744,     0,   745,   746,   747,   748,   749,   750,
     751,   752,   753,   754,     0,     0,     0,     0,     0,   715,
     716,   717,   718,     0,   755,   756,  1582,     0,     0,   723,
       0,   724,   725,   726,   727,   728,   729,   730,   731,   732,
     733,   734,   735,   736,  1601,   737,   738,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1614,  1615,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1624,     0,     0,     0,     0,     0,  1626,  1627,
       0,     0,     0,     0,  1632,     0,   750,   751,   752,   753,
     754,     0,     0,     0,     0,     0,     0,   402,   403,     0,
       0,   755,   756,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,  1016,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1016,     0,     0,     0,     0,  1649,     0,
       0,  1650,     0,  1652,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   869,  1669,  1670,  1671,  1672,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1681,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1695,   405,   406,   407,
     408,   409,   410,   411,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   422,     0,     0,   423,   424,   425,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1726,
     426,   427,   428,   429,   430,     0,     0,     0,     0,   431,
     432,   433,   434,   435,   436,   437,  1741,     0,     0,     0,
       0,  1744,  1745,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,     0,     0,   449,
     450,     0,     0,     0,     0,     0,     0,   451,   452,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1779,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1806,  1121,     0,     0,
       0,   480,   481,     3,     0,  -121,  -106,  -106,     0,  -118,
    1814,   482,   483,   484,   485,   486,     0,     0,     0,  1819,
       0,   487,  1122,   488,  1123,  1124,     0,   489,     0,  1830,
       0,     0,     0,     0,  1125,   490,  1126,     0,  -123,     0,
    1127,   491,     0,     0,   492,     0,     8,   493,  1128,     0,
    1129,   494,     0,  1844,  1130,  1131,     0,     0,     0,     0,
       0,  1132,     0,     0,   496,   497,     0,   223,   224,   225,
       0,   227,   228,   229,   230,   231,   498,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   245,
     246,   247,     0,     0,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   499,   500,   501,  1133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   502,   503,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   505,   506,   507,   508,
     509,     0,   510,     0,   511,   512,   513,   514,   515,   516,
     517,   518,   519,    59,     0,    13,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   523,     0,    14,     0,     0,   524,
     525,     0,     0,     0,     0,     0,     0,     0,   526,     0,
     527,     0,   528,   529,    16,  1134,  1135,  1121,     0,     0,
       0,   480,   481,     3,     0,  -121,  -106,  -106,     0,  -118,
       0,   482,   483,   484,   485,   486,     0,     0,     0,     0,
       0,   487,  1122,   488,  1123,  1124,     0,   489,     0,     0,
       0,     0,     0,     0,  1125,   490,  1126,     0,  -123,     0,
    1127,   491,     0,     0,   492,     0,     8,   493,  1128,     0,
    1129,   494,     0,     0,  1130,  1131,     0,     0,     0,     0,
       0,  1132,     0,     0,   496,   497,     0,   223,   224,   225,
       0,   227,   228,   229,   230,   231,   498,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   245,
     246,   247,     0,     0,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   499,   500,   501,  1133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   502,   503,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   505,   506,   507,   508,
     509,     0,   510,     0,   511,   512,   513,   514,   515,   516,
     517,   518,   519,    59,     0,    13,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   523,     0,    14,     0,     0,   524,
     525,     0,     0,     0,     0,     0,     0,     0,   526,     0,
     527,     0,   528,   529,    16,  1134,  -332,  1121,     0,     0,
       0,   480,   481,     3,     0,  -121,  -106,  -106,     0,  -118,
       0,   482,   483,   484,   485,   486,     0,     0,     0,     0,
       0,   487,  1122,   488,  1123,  1124,     0,   489,     0,     0,
       0,     0,     0,     0,  1125,   490,  1126,     0,  -123,     0,
    1127,   491,     0,     0,   492,     0,     8,   493,  1128,     0,
    1129,   494,     0,     0,  1130,  1131,     0,     0,     0,     0,
       0,  1132,     0,     0,   496,   497,     0,   223,   224,   225,
       0,   227,   228,   229,   230,   231,   498,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   245,
     246,   247,     0,     0,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   499,   500,   501,  1133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   502,   503,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   505,   506,   507,   508,
     509,     0,   510,     0,   511,   512,   513,   514,   515,   516,
     517,   518,   519,    59,     0,    13,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   523,     0,    14,     0,     0,   524,
     525,     0,     0,     0,     0,     0,     0,     0,   526,     0,
     527,     0,   528,   529,    16,  1134,  -358,  1121,     0,     0,
       0,   480,   481,     3,     0,  -121,  -106,  -106,     0,  -118,
       0,   482,   483,   484,   485,   486,     0,     0,     0,     0,
       0,   487,  1122,   488,  1123,  1124,     0,   489,     0,     0,
       0,     0,     0,     0,  1125,   490,  1126,     0,  -123,     0,
    1127,   491,     0,     0,   492,     0,     8,   493,  1128,     0,
    1129,   494,     0,     0,  1130,  1131,     0,     0,     0,     0,
       0,  1132,     0,     0,   496,   497,     0,   223,   224,   225,
       0,   227,   228,   229,   230,   231,   498,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   245,
     246,   247,     0,     0,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   499,   500,   501,  1133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   502,   503,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   505,   506,   507,   508,
     509,     0,   510,     0,   511,   512,   513,   514,   515,   516,
     517,   518,   519,    59,     0,    13,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   523,     0,    14,     0,     0,   524,
     525,     0,     0,     0,     0,     0,     0,     0,   526,     0,
     527,     0,   528,   529,    16,  1134,  -329,  1121,     0,     0,
       0,   480,   481,     3,     0,  -121,  -106,  -106,     0,  -118,
       0,   482,   483,   484,   485,   486,     0,     0,     0,     0,
       0,   487,  1122,   488,  1123,  1124,     0,   489,     0,     0,
       0,     0,     0,     0,  1125,   490,  1126,     0,  -123,     0,
    1127,   491,     0,     0,   492,     0,     8,   493,  1128,     0,
    1129,   494,     0,     0,  1130,  1131,     0,     0,     0,     0,
       0,  1132,     0,     0,   496,   497,     0,   223,   224,   225,
       0,   227,   228,   229,   230,   231,   498,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   245,
     246,   247,     0,     0,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   499,   500,   501,  1133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   502,   503,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   505,   506,   507,   508,
     509,     0,   510,     0,   511,   512,   513,   514,   515,   516,
     517,   518,   519,    59,     0,    13,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   523,     0,    14,     0,     0,   524,
     525,     0,     0,     0,     0,     0,     0,     0,   526,     0,
     527,     0,   528,   529,    16,  1134,   -97,  1121,     0,     0,
       0,   480,   481,     3,     0,  -121,  -106,  -106,     0,  -118,
       0,   482,   483,   484,   485,   486,     0,     0,     0,     0,
       0,   487,  1122,   488,  1123,  1124,     0,   489,     0,     0,
       0,     0,     0,     0,  1125,   490,  1126,     0,  -123,     0,
    1127,   491,     0,     0,   492,     0,     8,   493,  1128,     0,
    1129,   494,     0,     0,  1130,  1131,     0,     0,     0,     0,
       0,  1132,     0,     0,   496,   497,     0,   223,   224,   225,
       0,   227,   228,   229,   230,   231,   498,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   245,
     246,   247,     0,     0,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   499,   500,   501,  1133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   502,   503,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   505,   506,   507,   508,
     509,     0,   510,     0,   511,   512,   513,   514,   515,   516,
     517,   518,   519,    59,     0,    13,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   523,     0,    14,     0,     0,   524,
     525,     0,     0,     0,     0,     0,     0,     0,   526,     0,
     527,     0,   528,   529,    16,  1134,  -102,   480,   481,     0,
       0,     0,     0,     0,     0,     0,     0,   482,   483,   484,
     485,   486,     0,     0,     0,     0,     0,   487,     0,   488,
       0,     0,     0,   489,     0,     0,     0,     0,     0,     0,
       0,   490,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
     496,   497,   912,   223,   224,   225,     0,   227,   228,   229,
     230,   231,   498,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   245,   246,   247,     0,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   499,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,     0,     0,     0,     0,     0,
       0,     0,   577,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   505,   506,   507,   508,   509,     0,   510,   696,
     511,   512,   513,   514,   515,   516,   517,   518,   519,   697,
       0,     0,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    14,     0,     0,   524,   525,     0,     0,     0,
       0,     0,   480,   481,   913,     0,   527,   914,   528,   529,
     693,   530,   482,   483,   484,   485,   486,     0,     0,     0,
       0,     0,   487,     0,   488,     0,     0,     0,   489,     0,
       0,     0,     0,     0,     0,     0,   490,     0,     0,     0,
       0,     0,   491,     0,     0,   492,   694,     0,   493,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,   496,   497,     0,   223,   224,
     225,     0,   227,   228,   229,   230,   231,   498,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     245,   246,   247,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   499,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
       0,     0,     0,     0,     0,     0,     0,   577,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   505,   506,   507,
     508,   509,     0,   510,   696,   511,   512,   513,   514,   515,
     516,   517,   518,   519,   697,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,   523,     0,    14,     0,     0,
     524,   525,     0,     0,     0,     0,     0,   480,   481,   526,
       0,   527,     0,   528,   529,   693,   530,   482,   483,   484,
     485,   486,     0,     0,     0,     0,     0,   487,     0,   488,
       0,     0,     0,   489,     0,     0,     0,     0,     0,     0,
       0,   490,     0,     0,     0,     0,     0,   491,     0,     0,
     492,   694,     0,   493,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
     496,   497,     0,   223,   224,   225,     0,   227,   228,   229,
     230,   231,   498,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   245,   246,   247,     0,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   499,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,     0,     0,     0,     0,     0,
       0,     0,   577,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   505,   506,   507,   508,   509,     0,   510,     0,
     511,   512,   513,   514,   515,   516,   517,   518,   519,    59,
       0,     0,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    14,     0,     0,   524,   525,     0,     0,     0,
       0,     0,   480,   481,   526,     0,   527,     0,   528,   529,
       0,   530,   482,   483,   484,   485,   486,     0,     0,     0,
       0,     0,   487,     0,   488,     0,     0,     0,   489,     0,
       0,     0,     0,     0,     0,     0,   490,     0,     0,     0,
       0,     0,   491,     0,     0,   492,     0,     0,   493,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,   496,   497,  1055,   223,   224,
     225,     0,   227,   228,   229,   230,   231,   498,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     245,   246,   247,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   499,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
       0,     0,     0,     0,     0,     0,     0,   577,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   505,   506,   507,
     508,   509,     0,   510,   696,   511,   512,   513,   514,   515,
     516,   517,   518,   519,   697,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,   523,     0,    14,     0,     0,
     524,   525,     0,     0,     0,     0,     0,   480,   481,   526,
       0,   527,     0,   528,   529,     0,   530,   482,   483,   484,
     485,   486,     0,     0,     0,     0,     0,   487,     0,   488,
       0,     0,     0,   489,     0,     0,     0,     0,     0,     0,
       0,   490,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
     496,   497,     0,   223,   224,   225,     0,   227,   228,   229,
     230,   231,   498,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   245,   246,   247,     0,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   499,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,     0,     0,     0,     0,     0,
       0,     0,   577,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   505,   506,   507,   508,   509,     0,   510,   696,
     511,   512,   513,   514,   515,   516,   517,   518,   519,   697,
       0,     0,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    14,     0,     0,   524,   525,     0,     0,     0,
       0,     0,   480,   481,  1375,     0,   527,  1376,   528,   529,
       0,   530,   482,   483,   484,   485,   486,     0,     0,     0,
       0,     0,   487,     0,   488,     0,     0,     0,   489,     0,
       0,     0,     0,     0,     0,     0,   490,     0,     0,     0,
       0,     0,   491,     0,     0,   492,     0,     0,   493,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,   496,   497,     0,   223,   224,
     225,     0,   227,   228,   229,   230,   231,   498,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     245,   246,   247,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   499,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
       0,     0,     0,     0,     0,     0,     0,   577,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   505,   506,   507,
     508,   509,     0,   510,   696,   511,   512,   513,   514,   515,
     516,   517,   518,   519,   697,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,   523,     0,    14,     0,     0,
     524,   525,     0,     0,     0,     0,     0,   480,   481,  1396,
       0,   527,  1397,   528,   529,     0,   530,   482,   483,   484,
     485,   486,     0,     0,     0,     0,     0,   487,     0,   488,
       0,     0,     0,   489,     0,     0,     0,     0,     0,     0,
       0,   490,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
     496,   497,     0,   223,   224,   225,     0,   227,   228,   229,
     230,   231,   498,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   245,   246,   247,     0,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   499,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,     0,     0,     0,     0,     0,
       0,     0,   577,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   505,   506,   507,   508,   509,     0,   510,   696,
     511,   512,   513,   514,   515,   516,   517,   518,   519,   697,
       0,     0,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    14,     0,     0,   524,   525,     0,     0,     0,
       0,     0,   480,   481,   526,     0,   527,     0,   528,   529,
       0,   530,   482,   483,   484,   485,   486,     0,     0,     0,
       0,     0,   487,     0,   488,     0,     0,     0,   489,     0,
       0,     0,     0,     0,     0,     0,   490,     0,     0,     0,
       0,     0,   491,     0,     0,   492,     0,     0,   493,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,   496,   497,     0,   223,   224,
     225,     0,   227,   228,   229,   230,   231,   498,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     245,   246,   247,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   499,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
       0,     0,     0,     0,     0,     0,     0,   577,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   505,   506,   507,
     508,   509,     0,   510,     0,   511,   512,   513,   514,   515,
     516,   517,   518,   519,    59,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,   523,     0,    14,     0,     0,
     524,   525,     0,     0,     0,     0,     0,   480,   481,   526,
       0,   527,   993,   528,   529,     0,   530,   482,   483,   484,
     485,   486,     0,     0,     0,     0,     0,   487,     0,   488,
       0,     0,     0,   489,     0,     0,     0,     0,     0,     0,
       0,   490,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
     496,   497,     0,   223,   224,   225,     0,   227,   228,   229,
     230,   231,   498,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   245,   246,   247,     0,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   499,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,     0,     0,     0,     0,     0,
       0,     0,  1015,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   505,   506,   507,   508,   509,     0,   510,   696,
     511,   512,   513,   514,   515,   516,   517,   518,   519,   697,
       0,     0,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    14,     0,     0,   524,   525,     0,     0,     0,
       0,     0,   480,   481,  1309,     0,   527,     0,   528,   529,
       0,   530,   482,   483,   484,   485,   486,     0,     0,     0,
       0,     0,   487,     0,   488,     0,     0,     0,   489,     0,
       0,     0,     0,     0,     0,     0,   490,     0,     0,     0,
       0,     0,   491,     0,     0,   492,     0,     0,   493,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,   496,   497,     0,   223,   224,
     225,     0,   227,   228,   229,   230,   231,   498,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     245,   246,   247,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   499,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
       0,     0,     0,     0,     0,     0,     0,   577,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   505,   506,   507,
     508,   509,     0,   510,     0,   511,   512,   513,   514,   515,
     516,   517,   518,   519,    59,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,   523,     0,    14,     0,     0,
     524,   525,     0,     0,     0,     0,     0,   480,   481,   526,
       0,   527,  1381,   528,   529,     0,   530,   482,   483,   484,
     485,   486,     0,     0,     0,     0,     0,   487,     0,   488,
       0,     0,     0,   489,     0,     0,     0,     0,     0,     0,
       0,   490,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
     496,   497,     0,   223,   224,   225,     0,   227,   228,   229,
     230,   231,   498,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   245,   246,   247,     0,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   499,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,     0,     0,     0,     0,     0,
       0,     0,   577,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   505,   506,   507,   508,   509,     0,   510,     0,
     511,   512,   513,   514,   515,   516,   517,   518,   519,    59,
       0,     0,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    14,     0,     0,   524,   525,     0,     0,     0,
       0,     0,   480,   481,   526,     0,   527,  1403,   528,   529,
       0,   530,   482,   483,   484,   485,   486,     0,     0,     0,
       0,     0,   487,     0,   488,     0,     0,     0,   489,     0,
       0,     0,     0,     0,     0,     0,   490,     0,     0,     0,
       0,     0,   491,     0,     0,   492,     0,     0,   493,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,   496,   497,     0,   223,   224,
     225,     0,   227,   228,   229,   230,   231,   498,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     245,   246,   247,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   499,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
       0,     0,     0,     0,     0,     0,     0,   577,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   505,   506,   507,
     508,   509,     0,   510,     0,   511,   512,   513,   514,   515,
     516,   517,   518,   519,    59,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,   523,     0,    14,     0,     0,
     524,   525,     0,     0,     0,     0,     0,   480,   481,   526,
       0,   527,  1462,   528,   529,     0,   530,   482,   483,   484,
     485,   486,     0,     0,     0,     0,     0,   487,     0,   488,
       0,     0,     0,   489,     0,     0,     0,     0,     0,     0,
       0,   490,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
     496,   497,     0,   223,   224,   225,     0,   227,   228,   229,
     230,   231,   498,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   245,   246,   247,     0,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   499,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,     0,     0,     0,     0,     0,
       0,     0,  1015,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   505,   506,   507,   508,   509,     0,   510,   696,
     511,   512,   513,   514,   515,   516,   517,   518,   519,   697,
       0,     0,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    14,     0,     0,   524,   525,     0,     0,     0,
       0,     0,   480,   481,   526,     0,   527,     0,   528,   529,
       0,   530,   482,   483,   484,   485,   486,     0,     0,     0,
       0,     0,   487,     0,   488,     0,     0,     0,   489,     0,
       0,     0,     0,     0,     0,     0,   490,     0,     0,     0,
       0,     0,   491,     0,     0,   492,     0,     0,   493,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,   496,   497,     0,   223,   224,
     225,     0,   227,   228,   229,   230,   231,   498,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     245,   246,   247,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   499,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
       0,     0,     0,     0,     0,     0,     0,   577,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   505,   506,   507,
     508,   509,     0,   510,     0,   511,   512,   513,   514,   515,
     516,   517,   518,   519,    59,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,   523,     0,    14,     0,     0,
     524,   525,     0,     0,     0,     0,     0,   480,   481,   526,
       0,   527,     0,   528,   529,     0,   530,   482,   483,   484,
     485,   486,     0,     0,     0,     0,     0,   487,     0,   488,
       0,     0,     0,   489,     0,     0,     0,     0,     0,     0,
       0,   490,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
     496,   497,     0,   223,   224,   225,     0,   227,   228,   229,
     230,   231,   498,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   245,   246,   247,     0,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   499,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   505,   506,   507,   508,   509,     0,   510,     0,
     511,   512,   513,   514,   515,   516,   517,   518,   519,    59,
       0,     0,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    14,     0,     0,   524,   525,     0,     0,     0,
       0,     0,   480,   481,   526,   587,   527,     0,   528,   529,
       0,   530,   482,   483,   484,   485,   486,     0,     0,     0,
       0,     0,   487,     0,   488,     0,     0,     0,   489,     0,
       0,     0,     0,     0,     0,     0,   490,     0,     0,     0,
       0,     0,   491,     0,     0,   492,     0,     0,   493,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,   496,   497,     0,   223,   224,
     225,     0,   227,   228,   229,   230,   231,   498,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     245,   246,   247,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   499,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   505,   506,   507,
     508,   509,     0,   510,     0,   511,   512,   513,   514,   515,
     516,   517,   518,   519,    59,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,   523,     0,    14,     0,     0,
     524,   525,     0,     0,     0,     0,     0,   480,   481,   526,
     787,   527,     0,   528,   529,     0,   530,   482,   483,   484,
     485,   486,     0,     0,     0,     0,     0,   487,     0,   488,
       0,     0,     0,   489,     0,     0,     0,     0,     0,     0,
       0,   490,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
     496,   497,     0,   223,   224,   225,     0,   227,   228,   229,
     230,   231,   498,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   245,   246,   247,     0,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   499,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,     0,     0,     0,     0,     0,
       0,     0,  1015,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   505,   506,   507,   508,   509,     0,   510,     0,
     511,   512,   513,   514,   515,   516,   517,   518,   519,    59,
       0,     0,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    14,     0,     0,   524,   525,     0,     0,     0,
       0,     0,   480,   481,   526,     0,   527,     0,   528,   529,
    1108,   530,   482,   483,   484,   485,   486,     0,     0,     0,
       0,     0,   487,     0,   488,     0,     0,     0,   489,     0,
       0,     0,     0,     0,     0,     0,   490,     0,     0,     0,
       0,     0,   491,     0,     0,   492,     0,     0,   493,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,   496,   497,     0,   223,   224,
     225,     0,   227,   228,   229,   230,   231,   498,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     245,   246,   247,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   499,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   505,   506,   507,
     508,   509,     0,   510,     0,   511,   512,   513,   514,   515,
     516,   517,   518,   519,    59,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,   523,     0,    14,     0,     0,
     524,   525,     0,     0,     0,     0,     0,   480,   481,   526,
       0,   527,     0,   528,   529,     0,   530,   482,   483,   484,
     485,   486,     0,     0,     0,     0,     0,   487,     0,   488,
       0,     0,     0,   489,     0,     0,     0,     0,     0,     0,
       0,   490,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
     496,   497,     0,   223,   224,   225,     0,   227,   228,   229,
     230,   231,   498,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   245,   246,   247,     0,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   499,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,     0,     0,     0,     0,     0,
       0,     0,  1319,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   505,   506,   507,   508,   509,     0,   510,     0,
     511,   512,   513,   514,   515,   516,   517,   518,   519,    59,
       0,     0,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    14,     0,     0,   524,   525,     0,     0,     0,
       0,     0,   480,   481,   526,     0,   527,     0,   528,   529,
       0,   530,   482,   483,   484,   485,   486,     0,     0,     0,
       0,     0,   487,     0,   488,     0,     0,     0,   489,     0,
       0,     0,     0,     0,     0,     0,   490,     0,     0,     0,
       0,     0,   491,     0,     0,   492,     0,     0,   493,     0,
       0,     0,   494,     0,     0,     0,     0,     0,  1322,     0,
       0,     0,   495,     0,     0,   496,   497,     0,   223,   224,
     225,     0,   227,   228,   229,   230,   231,   498,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     245,   246,   247,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   499,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   505,   506,   507,
     508,   509,     0,   510,     0,   511,   512,   513,   514,   515,
     516,   517,   518,   519,    59,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,   523,     0,    14,     0,     0,
     524,   525,     0,     0,     0,     0,     0,   480,   481,   526,
       0,   527,     0,   528,   529,     0,   530,   482,   483,   484,
     485,   486,     0,     0,     0,     0,     0,   487,     0,   488,
       0,     0,     0,   489,     0,     0,     0,     0,     0,     0,
       0,   490,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   494,     0,     0,
    1328,     0,     0,     0,     0,     0,     0,   495,     0,     0,
     496,   497,     0,   223,   224,   225,     0,   227,   228,   229,
     230,   231,   498,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   245,   246,   247,     0,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   499,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   505,   506,   507,   508,   509,     0,   510,     0,
     511,   512,   513,   514,   515,   516,   517,   518,   519,    59,
       0,     0,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    14,     0,     0,   524,   525,     0,     0,     0,
       0,     0,   480,   481,   526,     0,   527,     0,   528,   529,
       0,   530,   482,   483,   484,   485,   486,     0,     0,     0,
       0,     0,   487,     0,   488,     0,     0,     0,   489,     0,
       0,     0,     0,     0,     0,     0,   490,     0,     0,     0,
       0,     0,   491,     0,     0,   492,     0,     0,   493,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,   496,   497,     0,   223,   224,
     225,     0,   227,   228,   229,   230,   231,   498,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     245,   246,   247,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   499,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
       0,     0,     0,     0,     0,     0,     0,  1331,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   505,   506,   507,
     508,   509,     0,   510,     0,   511,   512,   513,   514,   515,
     516,   517,   518,   519,    59,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,   523,     0,    14,     0,     0,
     524,   525,     0,     0,     0,     0,     0,   480,   481,   526,
       0,   527,     0,   528,   529,     0,   530,   482,   483,   484,
     485,   486,     0,     0,     0,     0,     0,   487,     0,   488,
       0,     0,     0,   489,     0,     0,     0,     0,     0,     0,
       0,   490,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
     496,   497,     0,   223,   224,   225,     0,   227,   228,   229,
     230,   231,   498,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   245,   246,   247,     0,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   499,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   505,   506,   507,   508,   509,     0,   510,     0,
     511,   512,   513,   514,   515,   516,   517,   518,   519,    59,
       0,     0,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    14,     0,     0,   524,   525,     0,     0,     0,
       0,     0,   480,   481,   526,     0,   527,  1600,   528,   529,
       0,   530,   482,   483,   484,   485,   486,     0,     0,     0,
       0,     0,   487,     0,   488,     0,     0,     0,   489,     0,
       0,     0,     0,     0,     0,     0,   490,     0,     0,     0,
       0,     0,   491,     0,     0,   492,     0,     0,   493,     0,
       0,     0,   494,  1631,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,   496,   497,     0,   223,   224,
     225,     0,   227,   228,   229,   230,   231,   498,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     245,   246,   247,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   499,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   505,   506,   507,
     508,   509,     0,   510,     0,   511,   512,   513,   514,   515,
     516,   517,   518,   519,    59,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,   523,     0,    14,     0,     0,
     524,   525,     0,     0,     0,     0,     0,   480,   481,   526,
       0,   527,     0,   528,   529,     0,   530,   482,   483,   484,
     485,   486,     0,     0,     0,     0,     0,   487,     0,   488,
       0,     0,     0,   489,     0,     0,     0,     0,     0,     0,
       0,   490,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
     496,   497,     0,   223,   224,   225,     0,   227,   228,   229,
     230,   231,   498,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   245,   246,   247,     0,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   499,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   505,   506,   507,   508,   509,     0,   510,     0,
     511,   512,   513,   514,   515,   516,   517,   518,   519,    59,
       0,     0,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    14,     0,     0,   524,   525,     0,     0,     0,
       0,     0,   480,   481,   526,     0,   527,     0,   528,   529,
       0,   530,   482,   483,   484,   485,   486,     0,     0,     0,
       0,     0,   487,  1122,   488,  1123,     0,     0,   489,     0,
       0,     0,     0,     0,     0,     0,   490,     0,     0,     0,
       0,     0,   491,     0,     0,   492,     0,     0,   493,  1128,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,   496,   497,     0,   223,   224,
     225,     0,   227,   228,   229,   230,   231,   498,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     245,   246,   247,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   499,   500,   501,  1133,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   505,   506,   507,
     508,   509,     0,   510,     0,   511,   512,   513,   514,   515,
     516,   517,   518,   519,    59,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,   523,     0,    14,     0,     0,
     524,   525,     0,     0,     0,   480,   481,     0,     0,   526,
       0,   527,     0,   528,   529,   482,   483,   484,   485,   486,
       0,     0,     0,     0,     0,   487,     0,   488,     0,     0,
       0,   489,     0,     0,     0,     0,     0,     0,     0,   490,
       0,     0,     0,     0,     0,   491,     0,     0,   492,     0,
       0,   493,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   495,     0,     0,   496,   497,
       0,   223,   224,   225,     0,   227,   228,   229,   230,   231,
     498,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   245,   246,   247,     0,     0,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   499,   500,   501,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   502,   503,     0,     0,     0,     0,     0,     0,     0,
     832,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     504,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     505,   506,   507,   508,   509,     0,   510,     0,   511,   512,
     513,   514,   515,   516,   517,   518,   519,    59,     0,     0,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   522,   523,     0,
      14,     0,     0,   524,   525,     0,     0,     0,   480,   481,
       0,     0,   526,     0,   527,     0,   528,   529,   482,   483,
     484,   485,   486,     0,     0,   972,     0,     0,   487,     0,
     488,     0,     0,     0,   489,     0,     0,     0,     0,     0,
       0,     0,   490,     0,     0,     0,     0,     0,   491,     0,
       0,   492,     0,     0,   493,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   495,     0,
       0,   496,   497,     0,   223,   224,   225,     0,   227,   228,
     229,   230,   231,   498,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,     0,   245,   246,   247,     0,
       0,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   499,   500,
     501,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   502,   503,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   504,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   505,   506,   507,   508,   509,     0,   510,
       0,   511,   512,   513,   514,   515,   516,   517,   518,   519,
      59,     0,     0,   520,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   521,
     522,   523,     0,    14,     0,     0,   524,   525,     0,     0,
       0,   480,   481,     0,     0,   526,     0,   527,     0,   528,
     529,   482,   483,   484,   485,   486,     0,     0,     0,     0,
       0,   487,     0,   488,     0,     0,     0,   489,     0,     0,
       0,     0,     0,     0,     0,   490,     0,     0,     0,     0,
       0,   491,     0,     0,   492,     0,     0,   493,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,   496,   497,     0,   223,   224,   225,
       0,   227,   228,   229,   230,   231,   498,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   245,
     246,   247,     0,     0,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   499,   500,   501,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   502,   503,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   505,   506,   507,   508,
     509,     0,   510,     0,   511,   512,   513,   514,   515,   516,
     517,   518,   519,    59,     0,   217,   520,     0,     0,     0,
       0,   218,     0,     0,     0,   713,   714,   219,     0,     0,
       0,     0,   521,   522,   523,     0,    14,   220,     0,   524,
     525,     0,     0,     0,     0,   221,     0,     0,   526,     0,
     527,     0,   528,   529,     0,     0,     0,     0,     0,     0,
     222,     0,     0,     0,     0,     0,     0,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   715,   716,   717,   718,   719,     0,     0,
     720,   721,   722,   723,     0,   724,   725,   726,   727,   728,
     729,   730,   731,   732,   733,   734,   735,   736,    58,   737,
     738,     0,     0,     0,     0,     0,     0,   713,   714,     0,
       0,   279,     0,     0,     0,     0,   217,     0,     0,     0,
       0,     0,   218,    59,     0,     0,     0,     0,   219,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   220,     0,
       0,     0,     0,     0,     0,     0,   221,   747,   748,   749,
     750,   751,   752,   753,   754,     0,     0,     0,     0,     0,
       0,   222,   280,     0,     0,   755,   756,   590,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,     0,   715,   716,   717,   718,   719,
       0,     0,   720,   721,   722,   723,     0,   724,   725,   726,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
       0,   737,   738,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   279,     0,     0,   217,     0,     0,     0,     0,
       0,   218,     0,     0,   769,     0,    13,   219,   746,   747,
     748,   749,   750,   751,   752,   753,   754,   220,     0,     0,
       0,     0,     0,     0,     0,   221,     0,   755,   756,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,     0,     0,   280,     0,    16,     0,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,   713,   714,     0,     0,     0,     0,     0,
       0,   279,   217,     0,     0,     0,     0,     0,   218,     0,
       0,     0,     0,    59,   219,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   220,     0,     0,     0,     0,     0,
       0,     0,   221,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   222,     0,     0,
       0,     0,   280,     0,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
       0,   715,   716,   717,   718,   719,   713,   714,   720,   721,
     722,   723,     0,   724,   725,   726,   727,   728,   729,   730,
     731,   732,   733,   734,   735,   736,     0,   737,   738,     0,
       0,   739,     0,     0,     0,   740,   741,   742,     0,     0,
       0,   743,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   279,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     769,   744,  1185,   745,   746,   747,   748,   749,   750,   751,
     752,   753,   754,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   755,   756,     0,     0,     0,     0,     0,
     713,   714,     0,     0,     0,     0,     0,     0,     0,   280,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   715,   716,   717,   718,   719,     0,
       0,   720,   721,   722,   723,     0,   724,   725,   726,   727,
     728,   729,   730,   731,   732,   733,   734,   735,   736,     0,
     737,   738,     0,     0,   739,     0,     0,     0,   740,   741,
     742,     0,     0,     0,   743,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   713,   714,     0,
       0,     0,     0,     0,   744,     0,   745,   746,   747,   748,
     749,   750,   751,   752,   753,   754,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   755,   756,   715,   716,
     717,   718,   719,     0,     0,   720,   721,   722,   723,     0,
     724,   725,   726,   727,   728,   729,   730,   731,   732,   733,
     734,   735,   736,     0,   737,   738,     0,     0,  -936,     0,
       0,     0,   740,   741,   742,     0,     0,     0,  -936,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   713,   714,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   744,     0,
     745,   746,   747,   748,   749,   750,   751,   752,   753,   754,
       0,     0,     0,     0,     0,   715,   716,   717,   718,   719,
     755,   756,   720,   721,   722,   723,     0,   724,   725,   726,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
       0,   737,   738,     0,     0,     0,     0,     0,     0,   740,
     741,   742,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   713,   714,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   744,     0,   745,   746,   747,
     748,   749,   750,   751,   752,   753,   754,     0,     0,   713,
     714,     0,   715,   716,   717,   718,   719,   755,   756,   720,
     721,   722,   723,     0,   724,   725,   726,   727,   728,   729,
     730,   731,   732,   733,   734,   735,   736,     0,   737,   738,
       0,     0,     0,     0,     0,     0,   740,     0,   742,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   745,   746,   747,   748,   749,   750,
     751,   752,   753,   754,     0,     0,   713,   714,     0,   715,
     716,   717,   718,   719,   755,   756,   720,   721,   722,   723,
       0,   724,   725,   726,   727,   728,   729,   730,   731,   732,
     733,   734,   735,   736,     0,   737,   738,   715,   716,   717,
     718,   719,     0,   740,   720,   721,   722,   723,     0,   724,
     725,   726,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   736,     0,   737,   738,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   745,   746,   747,   748,   749,   750,   751,   752,   753,
     754,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   755,   756,     0,     0,     0,     0,     0,     0,   745,
     746,   747,   748,   749,   750,   751,   752,   753,   754,     0,
       0,     0,     0,     0,   715,   716,   717,   718,   719,   755,
     756,   720,   721,   722,   723,     0,   724,   725,   726,   727,
     728,   729,   730,   731,   732,   733,   734,   735,   736,     0,
     737,   738,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   922,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   748,
     749,   750,   751,   752,   753,   754,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   755,   756,   223,   224,
     225,   926,   227,   228,   229,   230,   231,   498,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     245,   246,   247,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,     0,   223,   224,   225,     0,   227,   228,   229,
     230,   231,   498,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   245,   246,   247,     0,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,     0,     0,     0,
       0,     0,     0,     0,   923,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   924,     0,     0,     0,   948,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   927,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   928,
     223,   224,   225,   952,   227,   228,   229,   230,   231,   498,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,     0,   245,   246,   247,     0,     0,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,     0,   223,   224,   225,     0,   227,
     228,   229,   230,   231,   498,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   245,   246,   247,
       0,     0,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,     0,
       0,     0,     0,     0,     0,     0,   949,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   950,     0,     0,     0,
     956,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   953,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   954,   223,   224,   225,  1181,   227,   228,   229,   230,
     231,   498,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,     0,   245,   246,   247,     0,     0,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,     0,   223,   224,   225,
       0,   227,   228,   229,   230,   231,   498,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   245,
     246,   247,     0,   300,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,     0,     0,     0,     0,     0,     0,     0,   957,   301,
       0,   302,     0,   303,   304,   305,   306,   307,   958,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
       0,   319,   320,   321,     0,     0,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,  1182,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1183,     0,     0,     0,     0,     0,   349,
     350,   223,   224,   225,     0,   227,   228,   229,   230,   231,
     498,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   245,   246,   247,     0,     0,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   351,     0,   301,     0,   302,
       0,   303,   304,   305,   306,   307,     0,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,     0,   319,
     320,   321,   984,   985,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,     0,     0,     0,     0,     0,     0,   986,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   987,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   988,   989,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   301,     0,   302,     0,   303,   304,
     305,   306,   307,   595,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,     0,   319,   320,   321,     0,
       0,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   301,     0,
     302,     0,   303,   304,   305,   306,   307,     0,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,     0,
     319,   320,   321,     0,     0,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     597,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   794
};

static const yytype_int16 yycheck[] =
{
       1,    14,    15,   145,   146,   103,   385,   623,   172,   192,
     193,   834,   627,    45,   484,   484,  1101,  1124,  1030,    20,
      21,    22,   594,   698,   699,   593,   635,  1269,   397,   702,
    1350,     5,    81,     8,  1491,    20,   402,   403,    20,    20,
    1013,    20,   169,  1019,    22,   881,  1690,  1460,  1748,    50,
      51,    64,    65,    66,    86,   764,    19,   766,  1748,   768,
    1748,     7,  1038,     5,     6,    40,    46,   921,    33,    33,
     565,   527,   567,  1748,   569,   171,  1748,  1676,   199,    57,
     210,   884,     8,    25,   214,   149,    19,    20,   184,    31,
     211,   104,   105,   106,   107,  1107,   149,   181,   130,   131,
     171,   133,   149,   778,    50,   474,   150,    15,    16,  1809,
      36,  1454,   239,   197,   150,   199,   197,   238,   574,  1809,
     164,  1809,   502,   503,    15,    16,    68,    69,   149,   172,
     170,   173,   174,   175,  1809,   189,    69,  1809,  1779,   235,
    1739,   521,   522,   523,   524,   525,   210,   150,  1848,   129,
    1759,   125,   126,   197,   238,   236,   240,   210,  1848,   208,
    1848,   232,   215,   210,   208,  1806,   775,   210,   215,   213,
     779,   207,   208,  1848,   210,   199,  1848,   213,  1635,   192,
     193,   234,   209,   125,   126,   199,   859,   234,  1797,   210,
     913,  1604,   867,   235,   215,   870,   236,   237,   362,   239,
     656,  1845,     0,   171,   879,   208,   207,   882,   211,     7,
     234,   667,   354,   234,   670,  1859,   184,   359,  1303,   207,
     234,   363,   207,  1330,   199,   207,   207,  1063,   207,   844,
     172,  1085,    30,   613,    32,   601,    34,  1580,  1581,   912,
     214,   219,    40,  1216,   192,   193,   702,  1223,   213,   213,
     238,   214,    50,   231,  1597,   197,  1599,   232,    56,   868,
     234,  1064,   170,  1239,   197,   240,   150,   280,   232,   739,
     739,  1661,   214,   187,   199,  1473,   210,   241,   199,   213,
     164,   189,    80,   197,  1674,   150,   194,  1299,   657,   197,
     232,   150,   200,    62,   477,   478,   479,   239,   189,   164,
     181,   757,   207,   194,   673,   164,   197,   172,   211,   200,
     215,  1509,   215,   238,  1634,   364,   197,   238,   207,   240,
     234,  1664,  1665,   172,   208,   208,   234,   125,   126,   149,
     472,   150,  1675,   238,   181,   715,   716,  1727,  1728,   719,
     720,   721,   722,   208,   724,   164,   235,  1589,   197,   208,
     197,   731,  1459,   234,   207,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
     750,   751,   752,   753,   754,  1832,   214,   560,   561,   179,
    1089,   564,  1055,   566,   397,   568,  1493,   234,    33,   208,
     210,   198,  1115,   576,   889,   215,   234,   197,   172,   207,
     207,   199,  1011,   859,   150,   207,   173,   181,   175,    33,
    1422,  1120,   150,   207,   234,    60,    61,   207,   164,    33,
     220,   207,   220,   197,   919,   233,   164,   610,   611,  1044,
     210,   231,   187,   235,   232,  1110,    60,    61,  1010,  1007,
     238,   235,   197,   207,  1167,   235,    60,    61,   207,   397,
    1059,  1023,   832,  1021,   197,   152,   153,   154,   155,   156,
     157,   474,   208,   181,   477,   478,   479,  1190,   207,   233,
     208,   484,   485,   207,   843,  1084,   235,   219,   149,   197,
     235,   152,   153,   154,   155,   156,   157,   856,   857,   231,
     207,   504,   207,   235,   207,   197,   235,   866,   197,   172,
     199,   235,   147,   872,   873,   874,   151,   876,   181,   878,
    1333,   880,   172,   186,   234,    33,   234,   207,   235,   888,
     235,   239,   235,   147,   197,   130,   474,   151,   207,   477,
     478,   479,   179,   147,   231,   232,   219,   151,   207,   238,
     920,   214,    60,    61,  1629,   235,   181,   560,   561,   207,
     197,   564,    57,   566,   207,   568,   235,   570,    63,   701,
     231,   232,   197,   576,   209,   207,   235,   189,   213,   219,
     215,   216,   219,   220,   198,   199,    12,   235,   207,   207,
     593,   207,   235,   207,   231,   209,  1309,    23,    24,   213,
     187,   215,   216,   235,   777,   209,   241,   610,   611,   213,
     197,   215,   216,   207,  1616,   618,   235,   235,   207,   235,
      21,    22,   560,   561,   238,    33,   564,   241,   566,   214,
     568,   207,   570,   207,    34,   187,   627,   241,   576,   147,
     207,   235,   197,   151,   197,   197,   235,   198,   199,   234,
      21,    22,    60,    61,   657,   593,   207,  1754,   207,   235,
     211,   235,  1375,    63,   207,  1035,   207,  1266,   235,   207,
     673,  1370,   610,   611,    64,    65,    66,  1162,   207,   214,
     207,   181,   855,  1396,   197,   858,   235,   238,   197,   862,
     233,   214,   233,  1790,  1791,   233,   181,   197,    79,   234,
     703,   209,   214,  1062,   233,   213,   233,   215,   216,  1068,
     201,   234,   197,    94,   104,   105,   106,   107,    99,   657,
     101,  1167,   234,   199,  1083,  1171,   214,   214,   198,   199,
     130,   197,  1829,   241,   211,   673,    57,   207,   215,   147,
     141,   142,    63,   151,  1190,   215,   234,   234,   149,  1195,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   163,   543,   219,   545,   546,   238,   187,
     141,   142,   172,    47,   777,   713,   714,   231,   149,   197,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    67,   732,   733,   734,   197,   736,   187,
     791,   209,   793,   181,    57,   213,   130,   215,   216,   197,
      63,   198,   199,   197,   198,  1185,    57,   755,   130,   197,
     207,    57,    63,   207,   224,   211,  1539,    63,   215,   215,
     231,   232,  1005,   241,   234,  1008,    21,    22,   211,   777,
     843,  1014,   215,  1289,  1557,   216,   217,   218,   219,   220,
    1296,   238,   855,   856,   857,   858,   847,   198,   199,   862,
     231,   232,   187,   866,     5,     6,   207,   198,   199,   872,
     873,   874,   197,   876,   215,   878,   207,   880,   881,   197,
     198,   239,   198,   199,   215,   888,    57,   198,   199,   207,
     187,   207,    63,   173,   174,   175,   207,   238,   372,   215,
     197,  1714,  1362,  1362,   215,   843,   187,   238,   382,   207,
     211,   197,   210,   197,   215,   213,   197,   855,   856,   857,
     858,   395,   238,   231,   862,   197,    57,   238,   866,   920,
     197,    33,    63,   211,   872,   873,   874,   215,   876,   208,
     878,   211,   880,   881,   208,   215,   207,  1660,  1661,   210,
     888,   211,   213,  1666,   210,   215,   141,   142,    60,    61,
     173,  1674,   211,  1676,   149,   224,   215,   152,   153,   154,
     155,   156,   157,   201,   159,   160,   161,   162,   163,   201,
     202,   203,  1352,  1353,  1354,  1355,  1356,  1357,  1358,  1359,
    1360,  1361,  1362,  1363,  1364,  1365,  1366,  1367,  1368,  1369,
    1132,   199,  1005,   197,  1007,  1008,   211,  1720,  1673,  1822,
     215,  1014,  1371,   171,  1727,  1728,   542,   543,  1021,   545,
     546,   238,   239,   211,    57,  1384,  1739,   215,  1474,   211,
    1021,   211,  1023,   215,   972,   215,   201,   202,  1851,    10,
      11,   198,   199,    66,  1035,   147,   231,   232,   150,   151,
     207,  1340,  1341,  1044,   198,  1046,   210,  1722,   215,  1062,
    1063,  1234,   164,   129,   171,  1068,   214,  1005,   197,  1007,
    1008,   214,   214,   198,   199,   234,  1014,   214,   198,   199,
    1083,   238,   207,  1021,   214,   234,  1799,   207,   214,   234,
     215,  1245,  1698,   214,   232,   215,   214,   199,   201,   202,
     203,   204,   214,   577,    21,    22,   208,   209,   201,   202,
     203,   213,   211,   238,   216,   589,   214,   198,   238,    35,
     826,   827,   828,   239,  1062,  1063,    55,    56,    57,   239,
    1068,  1501,    35,    21,    22,   197,   238,   234,  1129,   241,
     197,   214,   232,   239,   197,  1083,  1137,  1138,  1594,   233,
      22,  1756,  1284,   197,    75,   233,   239,   631,    79,  1150,
     210,  1152,  1153,  1154,  1155,  1156,   197,   214,   214,  1160,
     234,  1617,    93,    94,    33,   214,    33,    98,    99,   100,
     101,   235,   214,   234,   658,   659,  1318,   214,  1793,   663,
     214,   665,   214,   214,   234,   215,   234,   234,   234,   234,
    1373,    60,    61,    60,    61,   234,   234,   681,   682,   683,
     684,   685,   686,  1386,   232,  1388,   235,   234,  1587,   197,
     235,   235,   139,   140,   141,   142,   143,   207,   233,   146,
     233,  1234,   149,   234,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   238,   165,   166,
     235,   234,   726,   141,   142,   729,   730,    43,   215,   234,
    1251,   149,   214,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   214,   214,   214,   234,
     234,   234,   756,    13,   233,   207,   234,     4,   147,   235,
     147,    33,   151,   150,   151,   197,  1234,   214,   215,   216,
     217,   218,   219,   220,   197,  1751,    33,   164,   233,   783,
     210,    33,   197,   197,   231,   232,   197,   197,    60,    61,
     207,   235,  1495,   235,   235,   235,   235,   235,   235,   235,
     218,   219,   220,    60,    61,   214,   235,   234,    60,    61,
     235,   235,   234,   231,   232,   234,    33,   214,   822,   214,
     209,   208,   209,   214,   213,   233,   213,   216,   234,   216,
     234,   234,    33,   234,   234,   234,   840,   233,  1371,   233,
    1373,   234,   846,    60,    61,   214,   234,     1,  1748,  1552,
     234,  1384,   241,  1386,   241,  1388,   234,   861,  1758,    60,
      61,   234,   234,   233,   215,   215,  1755,   215,  1571,    33,
     235,  1533,   234,   208,   235,   147,   234,    43,   208,   151,
     235,   234,   886,   887,    43,   234,  1504,    33,   235,   235,
     147,   235,   235,   897,   151,   147,    60,    61,   902,   151,
     904,   235,   906,  1371,   235,  1373,   235,   911,   235,  1809,
    1431,   235,   234,  1434,    60,    61,  1384,   210,  1386,   234,
    1388,    43,   197,   235,   234,   234,   240,  1460,   235,   197,
     147,   239,   197,   189,   151,  1835,    10,   209,    13,     9,
      42,   213,    66,   215,   216,   214,   147,   234,  1848,   234,
     151,   233,   209,   234,  1854,   233,   213,   209,   215,   216,
     197,   213,  1495,   215,   216,   197,   240,    33,   197,   241,
     240,   131,   132,   133,   134,   135,   136,   137,   138,   240,
    1501,   985,   197,   147,   241,   989,     8,   151,   197,   241,
     150,   235,   209,   234,    60,    61,   213,   197,   215,   216,
     205,   147,   215,   197,   164,   151,    33,   235,   209,   197,
     197,  1015,   213,   234,   215,   216,   176,   177,   178,  1552,
     197,   197,    14,   208,   241,   208,   210,  1495,    33,   235,
    1692,   189,   234,    60,    61,  1697,   234,   234,  1571,   235,
     241,  1045,   235,   235,   235,   209,   235,   208,   208,   213,
     240,   215,   216,   235,   235,    60,    61,   235,    43,    33,
     235,   234,    37,   209,   208,  1069,   240,   213,    67,   215,
     216,  1604,   235,   215,    70,    43,   197,   241,    33,  1631,
     235,   147,   234,   234,  1552,   151,    60,    61,   235,   234,
    1752,  1753,   235,   234,   234,   241,   234,   234,   234,   234,
     234,   234,   234,  1571,   197,    60,    61,   234,   234,   197,
     235,  1115,   235,  1634,   201,   235,   235,   234,  1122,   234,
     147,  1125,   234,   234,   151,   234,   197,  1131,   235,  1133,
     238,   235,   235,  1654,   131,   132,   133,   134,   135,   136,
     137,   138,   147,   209,   240,   235,   151,   213,  1810,   215,
     216,   235,   235,   235,   235,  1817,   235,    33,   235,   239,
    1164,  1823,  1166,   235,  1168,   238,  1170,   164,  1172,    12,
     235,   235,  1176,   147,  1178,   241,  1180,   151,   235,   176,
     177,   178,   209,   235,   235,  1189,   213,  1849,   215,   216,
     235,   207,   147,  1197,   207,   235,   151,   235,   235,   235,
     235,   235,   235,    53,   209,   234,   238,   238,   213,   235,
     215,   216,   235,    10,   241,   235,   233,  1221,   239,   233,
     235,   240,  1755,   687,    21,    22,   234,  1231,  1749,   240,
     240,   240,  1236,   239,  1238,   209,   241,  1758,    80,   213,
       1,   215,   216,  1247,   191,    83,  1250,  1858,  1856,   214,
     793,   793,  1341,   703,   209,  1039,     1,   777,   213,   707,
     215,   216,  1489,  1656,  1659,  1269,  1718,   241,  1719,  1574,
    1719,    54,  1239,  1038,  1038,  1796,   484,   484,   356,   232,
     484,   484,   658,    -1,    -1,   484,   241,  1755,  1292,   484,
      -1,    -1,    -1,  1814,    -1,    -1,  1300,  1301,  1302,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1828,    -1,  1830,
      -1,    21,    22,    -1,  1835,  1319,    -1,    -1,  1322,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1331,    -1,    -1,
      -1,  1852,    -1,  1854,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,    -1,    -1,   173,   174,   175,   176,
     177,   178,   179,    -1,    -1,    -1,    -1,  1391,    -1,  1393,
      -1,    -1,    -1,    -1,    -1,    -1,  1400,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   208,   209,    -1,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,    -1,   231,   232,  1440,    -1,    -1,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,  1458,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1475,  1476,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1496,    -1,    -1,    -1,    -1,    -1,  1502,  1503,
      -1,    -1,    -1,    -1,  1508,    -1,   216,   217,   218,   219,
     220,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,   231,   232,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,  1539,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1557,    -1,    -1,    -1,    -1,  1562,    -1,
      -1,  1565,    -1,  1567,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1589,  1590,  1591,  1592,  1593,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1610,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1630,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,   151,   152,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1663,
     164,   165,   166,   167,   168,    -1,    -1,    -1,    -1,   173,
     174,   175,   176,   177,   178,   179,  1680,    -1,    -1,    -1,
      -1,  1685,  1686,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,    -1,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,    -1,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,   231,   232,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1743,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1780,     1,    -1,    -1,
      -1,     5,     6,     7,    -1,     9,    10,    11,    -1,    13,
    1794,    15,    16,    17,    18,    19,    -1,    -1,    -1,  1803,
      -1,    25,    26,    27,    28,    29,    -1,    31,    -1,  1813,
      -1,    -1,    -1,    -1,    38,    39,    40,    -1,    42,    -1,
      44,    45,    -1,    -1,    48,    -1,    50,    51,    52,    -1,
      54,    55,    -1,  1837,    58,    59,    -1,    -1,    -1,    -1,
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
     124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,    -1,   186,    -1,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,    -1,    19,   200,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    21,    22,    31,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,   220,    41,    -1,   223,
     224,    -1,    -1,    -1,    -1,    49,    -1,    -1,   232,    -1,
     234,    -1,   236,   237,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   172,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,   185,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    25,   197,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,   213,   214,   215,
     216,   217,   218,   219,   220,    -1,    -1,    -1,    -1,    -1,
      -1,    64,   236,    -1,    -1,   231,   232,   241,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,    -1,   139,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,   197,    -1,   199,    31,   212,   213,
     214,   215,   216,   217,   218,   219,   220,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    -1,   231,   232,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,   236,    -1,   238,    -1,    71,    72,    73,
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
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   185,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,   197,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,   236,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
      -1,   139,   140,   141,   142,   143,    21,    22,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,   165,   166,    -1,
      -1,   169,    -1,    -1,    -1,   173,   174,   175,    -1,    -1,
      -1,   179,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   231,   232,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   236,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
     165,   166,    -1,    -1,   169,    -1,    -1,    -1,   173,   174,
     175,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,   209,    -1,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   231,   232,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,   165,   166,    -1,    -1,   169,    -1,
      -1,    -1,   173,   174,   175,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
     231,   232,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,   173,
     174,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,    -1,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,    -1,    -1,    21,
      22,    -1,   139,   140,   141,   142,   143,   231,   232,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,    -1,    -1,    21,    22,    -1,   139,
     140,   141,   142,   143,   231,   232,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,   165,   166,   139,   140,   141,
     142,   143,    -1,   173,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   231,   232,    -1,    -1,    -1,    -1,    -1,    -1,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,   231,
     232,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
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
      -1,   197,    71,    72,    73,    19,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    35,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    71,
      -1,    73,    -1,    75,    76,    77,    78,    79,   197,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   187,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,   141,
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
     208,   207,   210,   207,   179,   197,   220,   231,   263,   263,
     197,   245,   245,   439,   448,   448,   448,   197,   172,   260,
     313,   313,   313,   234,   173,   174,   175,   207,   233,   130,
     321,   460,     5,     6,   466,    57,    63,   440,    15,    16,
     170,   189,   194,   197,   200,   234,   252,   307,   189,   269,
     219,   219,   263,   219,   219,   231,    22,    57,   266,   197,
     450,    57,    63,   247,   197,   197,   197,   197,   201,   258,
     235,   308,   313,   313,   313,   313,   199,   273,   274,    57,
      63,   323,   325,    57,    63,   453,   130,   130,    57,    63,
     467,   239,   444,   197,   201,   202,   203,   251,    15,    16,
     189,   194,   197,   252,   304,   305,   263,   263,   263,   197,
     197,   197,   208,   208,   224,   249,   210,   480,   274,   274,
     201,   235,   199,   326,   197,   454,   471,   441,   171,   309,
     409,   201,   202,   203,   207,   235,   266,    19,    25,    31,
      41,    49,    64,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   185,
     236,   341,   474,   476,   477,   481,   487,   489,   518,   518,
      66,    79,    94,    99,   101,   198,   457,   458,   521,   526,
      35,    71,    73,    75,    76,    77,    78,    79,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    93,
      94,    95,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   141,
     142,   197,   319,   320,   324,   210,   455,   129,   464,   465,
     240,   245,   442,   307,   171,   197,   435,   438,   304,   214,
     214,   214,   234,   214,   214,   234,   480,   214,   214,   214,
     214,   214,   234,   341,   214,   234,    33,    60,    61,   147,
     151,   209,   213,   216,   241,   232,   486,   211,   198,   531,
     239,   239,    21,    22,    38,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   151,   152,   153,   164,   165,   166,   167,
     168,   173,   174,   175,   176,   177,   178,   179,   209,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   223,
     224,   231,   232,    35,    35,   234,   317,   274,    75,    79,
      93,    94,    98,    99,   100,   101,   475,   458,   197,   274,
     409,   274,   307,   207,   210,   213,   433,   490,   496,   498,
       5,     6,    15,    16,    17,    18,    19,    25,    27,    31,
      39,    45,    48,    51,    55,    65,    68,    69,    80,   125,
     126,   127,   141,   142,   170,   180,   181,   182,   183,   184,
     186,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     200,   216,   217,   218,   223,   224,   232,   234,   236,   237,
     239,   257,   259,   333,   341,   346,   358,   365,   368,   371,
     375,   377,   379,   380,   382,   387,   390,   391,   392,   407,
     408,   474,   535,   543,   554,   557,   570,   571,   574,   575,
     500,   494,   197,   214,   502,   504,   506,   508,   510,   512,
     514,   516,   391,   214,   234,   488,   492,   150,   338,   369,
     391,    33,   213,    33,   213,   232,   241,   233,   391,   232,
     241,   487,   239,   522,   527,   197,   320,   197,   320,   197,
     233,    22,   197,   233,   184,   235,   409,   419,   420,   421,
     149,   210,   318,   169,   239,   330,   370,   239,   210,   463,
     472,   181,   197,   434,   437,   274,   197,   487,   150,   164,
     208,   432,   518,   518,   485,   518,   214,   214,   214,   341,
     343,   476,   534,   543,   554,   557,   570,   571,   574,   575,
     341,   214,     5,   125,   126,   214,   234,   214,   234,   234,
     214,   214,   214,   234,   214,   234,   214,   234,   214,   214,
     234,    19,   214,   214,   392,   392,   181,   186,   214,   341,
     381,   234,   234,   234,   234,   234,   234,   256,   392,   392,
     392,   392,   392,    13,    49,   338,   187,   197,   369,   536,
     538,   569,   234,   232,   315,   170,   239,   371,   376,   376,
     376,   376,   235,    21,    22,   139,   140,   141,   142,   143,
     146,   147,   148,   149,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   165,   166,   169,
     173,   174,   175,   179,   209,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   231,   232,   234,   518,   518,
     235,   215,   482,   518,   317,   518,   317,   518,   317,   197,
     422,   423,   518,   197,   425,   426,   235,   493,   369,   340,
     518,   391,   235,   207,   573,   233,   233,   233,   391,   532,
     422,   424,   425,   427,   197,   320,   131,   132,   133,   134,
     135,   136,   137,   138,   164,   176,   177,   178,   131,   132,
     133,   134,   135,   136,   137,   138,   150,   164,   176,   177,
     178,   208,   234,     7,    50,   352,   238,   207,   238,   235,
     518,   518,   150,   392,   331,   461,   341,   238,   239,   468,
     234,    43,   207,   210,   433,   245,   432,   391,   215,   215,
     215,   198,   207,   244,   245,   484,   544,   546,   344,   234,
     214,   234,   366,   214,   214,   214,   564,   369,   487,   391,
     568,   391,   359,   361,   393,   391,   363,   391,   566,   369,
     552,   555,   369,   214,   548,   487,   234,   234,   383,   385,
     391,   391,   391,   391,   391,   391,   203,   204,   251,   234,
      13,   233,   234,   150,   164,   208,   428,   573,   207,   573,
     235,   274,    70,   232,   235,   369,   538,   314,     4,   374,
     337,   315,    19,   187,   197,   474,    19,   187,   197,   474,
     392,   392,   392,   392,   392,   392,   197,   392,   187,   197,
     391,   187,   197,   187,   197,   391,   391,   392,    19,   187,
     197,   474,    19,   187,   197,   474,    19,   187,   197,   474,
     392,   392,   474,   392,   392,   392,   570,   575,   392,   392,
     392,   392,    22,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   152,   153,   187,   197,   231,   232,
     388,   474,   391,   235,   369,   215,   215,   197,   478,   215,
     318,   215,   318,   215,   318,   210,   215,   484,   210,   215,
     484,   340,   573,   215,   484,   150,   391,   233,   197,   479,
     245,   281,   282,   281,   282,   391,   181,   197,   429,   430,
     473,   421,   421,   421,   392,   337,   197,   445,   447,   415,
     391,   197,   181,   197,   487,   432,   391,   245,   491,   497,
     499,   518,   487,   487,   518,    70,   369,   538,   542,   197,
     391,   518,   558,   560,   562,   487,   573,   215,   484,   207,
     573,   235,   487,   487,   487,   235,   487,   235,   487,   573,
     487,   423,   573,   550,   426,   215,   391,   391,   487,   317,
     235,   235,   235,   235,   235,   235,   391,   181,   197,   234,
     295,   234,   391,   391,   391,   235,   187,   197,    13,   339,
     568,   197,   235,   538,   536,   207,   235,   235,   233,   234,
     317,     1,    26,    28,    29,    38,    40,    44,    52,    54,
      58,    59,    65,   128,   239,   240,   245,   270,   271,   280,
     291,   292,   294,   296,   297,   298,   299,   300,   301,   302,
     303,   334,   342,   347,   348,   349,   350,   351,   353,   357,
     378,   392,   374,   214,   234,   214,   234,   234,   234,   233,
     234,   234,   234,   233,   233,   214,   234,   214,   234,   214,
     234,    19,   187,   197,   474,   210,   187,   197,   391,   234,
     234,   187,   197,   391,     1,   234,   233,   207,   235,   501,
     495,   207,   215,   238,   503,   215,   507,   215,   511,   215,
     518,   515,   422,   518,   517,   425,   215,   235,   488,   518,
     391,   208,   244,   446,   456,   245,   422,   523,   425,   528,
     235,   234,    43,   207,   210,   213,   428,   332,   208,   446,
     456,    40,   199,   240,   316,   416,   235,   234,    43,   245,
     432,   391,   245,   215,   215,   215,   538,   235,   235,   235,
     215,   484,   235,   215,   487,   423,   426,   215,   235,   234,
     487,   391,   235,   215,   215,   215,   215,   215,   235,   215,
     215,   235,   487,   215,   374,   235,   235,   215,   318,   234,
     210,   254,   234,    43,   197,   355,    20,   207,   295,   235,
     234,   164,   428,   234,   240,   573,   235,   207,   233,   232,
     536,   150,   164,   197,   208,   213,   372,   373,   318,   150,
     391,   330,    61,   391,   197,   197,   245,   189,    58,   391,
     274,   150,   391,   335,   245,   245,    10,    10,    11,   278,
      13,     9,    42,   245,   245,   245,   245,   245,   245,    66,
     354,   245,   131,   132,   133,   134,   135,   136,   137,   138,
     144,   145,   150,   164,   167,   168,   176,   177,   178,   208,
     317,   395,   391,   399,   391,   232,   235,   369,   536,   391,
     391,   235,   369,   391,   397,   391,   401,   391,   405,   391,
     214,   234,   392,   234,   233,   391,   232,   235,   369,   536,
     234,   233,   389,   235,   369,   197,   483,   197,   505,   509,
     513,   488,   391,   197,   244,   533,   240,   240,   391,   197,
     181,   197,   518,   391,   240,   391,   445,   462,   197,     8,
     409,   414,   391,   197,   391,   245,   545,   547,   345,   235,
     234,   197,   367,   215,   215,   215,   565,   339,   215,   360,
     362,   394,   364,   567,   553,   556,   215,   549,   234,   274,
     384,   215,   235,   369,   255,   205,   391,   197,   207,   235,
     369,   181,   197,   234,    20,   164,   428,   391,   391,   391,
     295,   235,   536,   235,   197,   197,   234,   197,   197,   207,
     235,   274,   391,    14,   391,   208,   208,   210,   189,   330,
     391,   337,   234,   234,   232,   309,   310,   310,   234,   234,
     239,   356,   437,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   570,   575,   392,   392,   392,   392,
     392,   392,   392,   318,   487,   235,   518,   235,   536,   207,
     235,   235,   235,   235,   235,   235,   487,   235,   518,   235,
     518,   235,   403,   391,   391,   235,   536,   207,   235,   235,
     391,   235,   208,   240,   235,   234,    43,   428,    37,   327,
     240,   208,    57,    63,   412,    67,   413,   245,   235,   245,
     234,   234,   391,   215,   559,   561,   563,   234,   235,   234,
     234,   234,   234,   234,   234,    70,   542,   234,   551,   234,
     235,   391,   330,   381,   386,   235,   253,   235,   197,   235,
     234,    43,   355,   369,   391,   391,   235,    20,   233,   197,
     372,   370,   330,   518,   391,   336,   391,   391,   308,   234,
     234,    56,   391,   355,   436,   274,   215,   215,   233,   536,
     215,   215,   215,   518,   235,   235,   233,   536,   235,   391,
     391,   197,   391,   328,   518,    47,   413,    46,   129,   410,
     542,   542,   235,   234,   234,   234,   234,   338,   339,   391,
     391,   391,   391,   369,   542,   234,   542,   235,   381,   201,
     238,   391,   197,   235,   235,   164,   428,   369,   235,   235,
     240,   235,   235,   233,   295,   391,   263,   235,   235,   240,
     245,   437,   370,   396,   400,   235,   235,   398,   402,   406,
     215,   235,   235,   235,   239,   245,    33,   411,   410,   412,
     234,   536,   539,   540,   541,   541,   391,   542,   542,   536,
     537,   235,   235,   235,   235,   235,   235,   573,   541,   542,
     537,   391,   235,   238,   391,   391,   235,   327,    12,   279,
     274,    20,   235,   235,   274,   210,   433,   404,   337,   417,
     411,   429,   430,   431,   536,   207,   573,   235,   235,   235,
     541,   541,   235,   235,   235,   537,   235,   238,   572,   391,
     238,   280,   347,   348,   349,   350,   392,   245,   293,   369,
     274,   274,   330,   487,   432,   329,   324,   418,   235,   234,
     235,   235,   235,    53,   233,   572,   391,   239,   283,   286,
     235,   330,   330,   432,   391,   240,   245,   324,   536,   391,
     233,   572,   284,    12,    23,    24,   272,   275,   280,   274,
     391,   245,   274,   235,   240,   337,   274,   234,   245,   330,
     245,   370,   285,   276,   391,   240,   239,   287,   290,   235,
     327,   288,   280,   274,   337,   245,   277,   289,   287,   240,
     275,   327
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
     265,   265,   266,   266,   267,   268,   268,   269,   269,   270,
     271,   271,   272,   272,   273,   273,   274,   274,   275,   276,
     275,   277,   275,   278,   278,   279,   279,   280,   280,   280,
     280,   280,   281,   281,   282,   282,   284,   285,   283,   286,
     283,   288,   289,   287,   290,   287,   292,   293,   291,   294,
     295,   295,   295,   295,   295,   295,   295,   295,   297,   296,
     298,   300,   299,   301,   302,   302,   303,   303,   304,   304,
     304,   304,   304,   304,   305,   305,   306,   306,   306,   306,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   308,
     308,   309,   309,   310,   310,   310,   311,   311,   311,   311,
     312,   312,   313,   313,   313,   313,   313,   313,   313,   314,
     314,   315,   315,   316,   316,   317,   317,   317,   318,   318,
     318,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   321,   321,
     322,   323,   323,   323,   324,   326,   325,   327,   328,   329,
     327,   331,   332,   330,   333,   333,   333,   333,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   335,   336,   334,
     337,   337,   337,   338,   338,   339,   339,   340,   340,   341,
     341,   341,   342,   342,   344,   345,   343,   343,   346,   346,
     346,   346,   346,   346,   347,   348,   349,   349,   349,   350,
     350,   351,   352,   352,   352,   353,   353,   354,   354,   355,
     355,   356,   356,   357,   357,   357,   359,   360,   358,   361,
     362,   358,   363,   364,   358,   366,   367,   365,   368,   368,
     368,   369,   369,   369,   369,   370,   370,   370,   371,   371,
     371,   372,   372,   372,   372,   372,   373,   373,   374,   374,
     375,   376,   376,   377,   377,   377,   377,   377,   377,   377,
     377,   378,   378,   378,   378,   378,   378,   378,   378,   378,
     378,   378,   378,   378,   378,   378,   378,   378,   378,   378,
     378,   378,   379,   379,   379,   380,   380,   380,   380,   380,
     381,   381,   382,   383,   384,   382,   385,   386,   382,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   388,   389,   387,   390,   390,   390,   390,   390,   390,
     390,   391,   391,   391,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     393,   394,   392,   392,   392,   392,   392,   395,   396,   392,
     392,   392,   397,   398,   392,   392,   392,   392,   399,   400,
     392,   392,   392,   401,   402,   392,   392,   392,   403,   404,
     392,   392,   392,   405,   406,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   407,   407,   407,   408,   408,   408,   408,   408,
     408,   408,   408,   408,   408,   408,   408,   408,   408,   408,
     408,   408,   408,   408,   408,   408,   409,   409,   410,   410,
     410,   411,   411,   412,   412,   412,   413,   413,   414,   415,
     415,   415,   416,   415,   417,   415,   418,   415,   419,   420,
     420,   421,   421,   421,   421,   421,   422,   422,   423,   423,
     424,   424,   424,   425,   426,   426,   427,   427,   427,   428,
     428,   429,   429,   429,   430,   430,   431,   431,   432,   432,
     432,   433,   433,   434,   434,   434,   434,   434,   434,   435,
     435,   436,   436,   436,   437,   437,   437,   438,   438,   438,
     439,   439,   440,   440,   440,   441,   441,   442,   441,   443,
     444,   443,   445,   445,   446,   446,   447,   447,   447,   448,
     448,   448,   450,   449,   451,   451,   452,   453,   453,   453,
     454,   455,   455,   456,   456,   457,   457,   458,   458,   460,
     461,   462,   459,   463,   463,   464,   464,   465,   466,   466,
     466,   466,   467,   467,   467,   468,   468,   470,   471,   472,
     469,   473,   473,   473,   473,   473,   473,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   474,   475,   475,   475,
     475,   475,   475,   475,   475,   476,   477,   477,   477,   478,
     478,   478,   479,   479,   479,   479,   479,   480,   480,   480,
     480,   480,   481,   482,   483,   481,   484,   484,   485,   485,
     486,   486,   486,   486,   487,   487,   488,   488,   489,   489,
     489,   489,   490,   491,   489,   489,   489,   489,   492,   489,
     493,   489,   489,   489,   489,   489,   489,   489,   489,   489,
     489,   489,   489,   489,   494,   495,   489,   489,   496,   497,
     489,   498,   499,   489,   500,   501,   489,   489,   502,   503,
     489,   504,   505,   489,   489,   506,   507,   489,   508,   509,
     489,   489,   510,   511,   489,   512,   513,   489,   514,   515,
     489,   516,   517,   489,   518,   518,   518,   520,   521,   522,
     523,   519,   525,   526,   527,   528,   524,   530,   531,   532,
     533,   529,   534,   534,   534,   534,   534,   534,   534,   535,
     535,   535,   535,   535,   536,   536,   536,   536,   536,   536,
     536,   536,   537,   537,   538,   539,   539,   540,   540,   541,
     541,   542,   542,   544,   545,   543,   546,   547,   543,   548,
     549,   543,   550,   551,   543,   552,   553,   543,   554,   555,
     556,   554,   557,   558,   559,   557,   560,   561,   557,   562,
     563,   557,   557,   564,   565,   557,   557,   566,   567,   557,
     568,   568,   569,   570,   571,   571,   571,   572,   572,   573,
     573,   574,   574,   575
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
       3,     5,     0,     1,     2,     1,     3,     1,     3,     3,
       3,     2,     1,     1,     1,     2,     0,     1,     0,     0,
       4,     0,     8,     1,     1,     0,     2,     1,     1,     1,
       1,     1,     1,     2,     0,     1,     0,     0,     6,     0,
       3,     0,     0,     6,     0,     3,     0,     0,     9,     7,
       1,     4,     3,     3,     3,     6,     5,     5,     0,    10,
       3,     0,     8,     0,     7,     8,     4,     4,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     1,
       3,     3,     4,     3,     3,     3,     3,     1,     5,     1,
       3,     3,     4,     0,     3,     1,     1,     1,     1,     1,
       1,     4,     1,     2,     3,     3,     3,     3,     2,     1,
       3,     0,     3,     0,     4,     0,     2,     3,     0,     2,
       2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       3,     2,     2,     3,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     3,     2,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     4,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       4,     0,     1,     1,     3,     0,     5,     0,     0,     0,
       6,     0,     0,     6,     2,     2,     2,     2,     1,     2,
       2,     1,     1,     1,     1,     2,     1,     2,     2,     2,
       2,     1,     1,     1,     2,     2,     2,     0,     0,     6,
       0,     2,     2,     0,     2,     0,     2,     1,     3,     1,
       3,     2,     2,     3,     0,     0,     5,     1,     2,     5,
       5,     5,     6,     2,     1,     1,     1,     2,     3,     2,
       3,     4,     1,     1,     0,     1,     1,     1,     0,     1,
       3,     8,     7,     3,     3,     5,     0,     0,     9,     0,
       0,     9,     0,     0,     9,     0,     0,     6,     5,     8,
      10,     1,     2,     3,     4,     1,     2,     3,     1,     1,
       1,     2,     2,     2,     2,     4,     1,     3,     0,     4,
       7,     7,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     6,     8,     6,     5,     6,     8,     8,     6,
       1,     4,     2,     0,     0,     7,     0,     0,     8,     3,
       4,     3,     5,     6,     5,     6,     8,     8,     6,     5,
       6,     0,     0,     5,     3,     4,     4,     5,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     4,     3,     4,     5,     4,     4,
       5,     4,     3,     4,     3,     1,     1,     2,     4,     4,
       0,     0,     9,     1,     3,     3,     5,     0,     0,     8,
       3,     3,     0,     0,     8,     3,     3,     3,     0,     0,
       8,     3,     3,     0,     0,     8,     3,     4,     0,     0,
       9,     4,     3,     0,     0,     8,     3,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     1,     4,     3,
       3,     3,     7,     8,     7,     4,     4,     4,     4,     4,
       1,     6,     7,     6,     6,     7,     6,     7,     6,     6,
       6,     7,     6,     6,     6,     5,     0,     1,     0,     1,
       1,     0,     1,     0,     1,     1,     0,     1,     5,     0,
       2,     6,     0,     4,     0,     9,     0,    11,     3,     3,
       4,     1,     1,     3,     3,     3,     1,     3,     1,     3,
       0,     1,     3,     3,     1,     3,     0,     1,     3,     1,
       1,     1,     2,     3,     3,     5,     1,     1,     1,     1,
       1,     0,     1,     1,     4,     3,     3,     6,     5,     1,
       3,     0,     2,     2,     4,     6,     5,     4,     6,     5,
       0,     1,     0,     1,     1,     0,     2,     0,     4,     6,
       0,     6,     1,     3,     1,     2,     0,     1,     3,     0,
       1,     1,     0,     5,     3,     3,     5,     0,     1,     1,
       1,     0,     2,     0,     1,     1,     2,     0,     1,     0,
       0,     0,    13,     0,     2,     0,     1,     3,     1,     1,
       2,     2,     0,     1,     1,     1,     3,     0,     0,     0,
       9,     1,     4,     3,     3,     6,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     1,
       3,     3,     0,     1,     3,     3,     5,     0,     2,     2,
       2,     2,     4,     0,     0,     7,     1,     1,     1,     3,
       3,     2,     4,     3,     1,     2,     0,     4,     1,     1,
       1,     1,     0,     0,     6,     4,     4,     3,     0,     6,
       0,     7,     4,     2,     2,     3,     2,     3,     2,     2,
       3,     3,     3,     2,     0,     0,     6,     2,     0,     0,
       6,     0,     0,     6,     0,     0,     6,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     1,     0,     0,     6,     0,     0,     7,     0,     0,
       6,     0,     0,     6,     1,     3,     3,     0,     0,     0,
       0,    12,     0,     0,     0,     0,    12,     0,     0,     0,
       0,    13,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     5,     5,     6,     6,
       8,     8,     0,     1,     2,     3,     5,     1,     2,     1,
       0,     0,     1,     0,     0,    10,     0,     0,    10,     0,
       0,    10,     0,     0,    11,     0,     0,     7,     5,     0,
       0,    10,     3,     0,     0,    11,     0,     0,    11,     0,
       0,    10,     5,     0,     0,     9,     5,     0,     0,    10,
       1,     3,     0,     5,     5,     7,     9,     0,     3,     0,
       1,    11,    12,    13
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

  case 62: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 63: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 67: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 68: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 69: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 70: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 71: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 72: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 73: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 78: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 79: /* $@3: %empty  */
                                           {
    }
    break;

  case 80: /* expression_else: "else" optional_emit_semis $@3 expression_else_block  */
                                   {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 81: /* $@4: %empty  */
                                                                        {
    }
    break;

  case 82: /* expression_else: elif_or_static_elif '(' expr ')' optional_emit_semis $@4 expression_else_block expression_else  */
                                                         {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-7])),(yyvsp[-5].pExpression),(yyvsp[-1].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-7].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 83: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 84: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 85: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 86: /* expression_else_one_liner: "else" expression_if_one_liner  */
                                                      {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 87: /* expression_if_one_liner: expr_no_bracket  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 88: /* expression_if_one_liner: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 89: /* expression_if_one_liner: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 90: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 91: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 96: /* $@5: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 97: /* $@6: %empty  */
                         {
        yyextra->pop_nesteds();
    }
    break;

  case 98: /* expression_if_block: '{' $@5 expressions $@6 '}' expression_block_finally  */
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

  case 99: /* $@7: %empty  */
       {
        yyextra->das_keyword = false;
    }
    break;

  case 100: /* expression_if_block: $@7 expression_if_one_liner SEMICOLON  */
                                               {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 101: /* $@8: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 102: /* $@9: %empty  */
                         {
        yyextra->pop_nesteds();
    }
    break;

  case 103: /* expression_else_block: '{' $@8 expressions $@9 '}' expression_block_finally  */
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

  case 104: /* $@10: %empty  */
       {
        yyextra->das_keyword = false;
    }
    break;

  case 105: /* expression_else_block: $@10 expression_if_one_liner SEMICOLON  */
                                               {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 106: /* $@11: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 107: /* $@12: %empty  */
                                                                  {
    }
    break;

  case 108: /* expression_if_then_else: $@11 if_or_static_if '(' expr ')' optional_emit_semis $@12 expression_if_block expression_else  */
                                                       {
        yyextra->das_keyword = false;
        auto blk = (yyvsp[-1].pExpression)->rtti_isBlock() ? static_cast<ExprBlock *>((yyvsp[-1].pExpression)) : ast_wrapInBlock((yyvsp[-1].pExpression));
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-7])),(yyvsp[-5].pExpression),blk,(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-7].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 109: /* expression_if_then_else_oneliner: expression_if_one_liner "if" '(' expr ')' expression_else_one_liner SEMICOLON  */
                                                                                                                      {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ast_wrapInBlock((yyvsp[-6].pExpression)),(yyvsp[-1].pExpression) ? ast_wrapInBlock((yyvsp[-1].pExpression)) : nullptr);
    }
    break;

  case 110: /* for_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 111: /* for_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 112: /* for_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 113: /* for_variable_name_with_pos_list: '(' tuple_expansion ')'  */
                                       {
        auto pSL = new vector<VariableNameAndPosition>();
        for ( auto & x : *(yyvsp[-1].pNameList) ) {
            das_checkName(scanner,x,tokAt(scanner,(yylsp[-1])));
        }
        pSL->push_back(VariableNameAndPosition((yyvsp[-1].pNameList),tokAt(scanner,(yylsp[-1]))));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 114: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 115: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                               {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 116: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 117: /* for_variable_name_with_pos_list: for_variable_name_with_pos_list ',' '(' tuple_expansion ')'  */
                                                                                 {
        for ( auto & x : *(yyvsp[-1].pNameList) ) {
            das_checkName(scanner,x,tokAt(scanner,(yylsp[-1])));
        }
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition((yyvsp[-1].pNameList),tokAt(scanner,(yylsp[-1]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
    }
    break;

  case 118: /* $@13: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 119: /* expression_for_loop: $@13 "for" optional_for_annotations '(' for_variable_name_with_pos_list "in" expr_list ')' optional_emit_semis expression_block  */
                                                                                                                                                                    {
        yyextra->das_keyword = false;
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-5].pNameWithPosList),(yyvsp[-3].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-8])),tokAt(scanner,(yylsp[0])),(yyvsp[-7].aaList));
    }
    break;

  case 120: /* expression_unsafe: "unsafe" optional_emit_semis expression_block  */
                                                                    {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-2])));
        pUnsafe->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 121: /* $@14: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 122: /* expression_while_loop: $@14 "while" optional_for_annotations '(' expr ')' optional_emit_semis expression_block  */
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

  case 123: /* with_keyword_on: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 124: /* expression_with: with_keyword_on "with" '(' expr ')' optional_emit_semis expression_block  */
                                                                                                     {
        yyextra->das_keyword = false;
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-5])));
        pWith->with = (yyvsp[-3].pExpression);
        pWith->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pWith;
    }
    break;

  case 125: /* expression_with: with_keyword_on "with" '(' "module" require_module_name ')' optional_emit_semis expression_block  */
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

  case 126: /* expression_with_alias: "assume" "name" '=' expr  */
                                                      {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-2].s);
    }
    break;

  case 127: /* expression_with_alias: "typedef" "name" '=' type_declaration  */
                                                                {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), TypeDeclPtr((yyvsp[0].pTypeDecl)));
        delete (yyvsp[-2].s);
    }
    break;

  case 128: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 129: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 130: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 131: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 132: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 133: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 134: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 135: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 136: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 137: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 138: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 139: /* annotation_argument_name: "default"  */
                    { (yyval.s) = new string("default"); }
    break;

  case 140: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 141: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 142: /* annotation_argument: annotation_argument_name '=' "@@" "name"  */
                                                                      { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[0].s); delete (yyvsp[-3].s); }
    break;

  case 143: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 144: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 145: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 146: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 147: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 148: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 149: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 150: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 151: /* metadata_argument_list: "@field" annotation_argument optional_emit_semis  */
                                                              {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[-1].aa));
    }
    break;

  case 152: /* metadata_argument_list: metadata_argument_list "@field" annotation_argument optional_emit_semis  */
                                                                                           {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-3].aaList),(yyvsp[-1].aa));
    }
    break;

  case 153: /* optional_for_annotations: %empty  */
                    {
        (yyval.aaList) = nullptr;
    }
    break;

  case 154: /* optional_for_annotations: '[' annotation_argument_list ']'  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 155: /* optional_for_annotations: metadata_argument_list  */
                                     {
        (yyval.aaList) = (yyvsp[0].aaList);
    }
    break;

  case 156: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 157: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 158: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 159: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 160: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 161: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 162: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 163: /* annotation_declaration: '!' annotation_declaration  */
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

  case 164: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 165: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 166: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 167: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 168: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 169: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 170: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 171: /* optional_annotation_list: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 172: /* optional_annotation_list: '[' annotation_list ']'  */
                                       { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 173: /* optional_annotation_list_with_emit_semis: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 174: /* optional_annotation_list_with_emit_semis: '[' annotation_list ']' optional_emit_semis  */
                                                          { (yyval.faList) = (yyvsp[-2].faList); }
    break;

  case 175: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 176: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 177: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 178: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yyloc));
    }
    break;

  case 179: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 180: /* optional_function_type: "->" type_declaration  */
                                           {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 181: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 182: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 183: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 184: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 185: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 186: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 187: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 188: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 189: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 190: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 191: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 192: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 193: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 194: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 195: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 196: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 197: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 198: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 199: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 200: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 201: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 202: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 203: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 204: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 205: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 206: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 207: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 208: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 209: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 210: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 211: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 212: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 213: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 214: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 215: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 216: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 217: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 218: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 219: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 220: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 221: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 222: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 223: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 224: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 225: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 226: /* function_name: "operator" '[' ']' '='  */
                                 { (yyval.s) = new string("[]="); }
    break;

  case 227: /* function_name: "operator" '[' ']' "<-"  */
                                    { (yyval.s) = new string("[]<-"); }
    break;

  case 228: /* function_name: "operator" '[' ']' ":="  */
                                      { (yyval.s) = new string("[]:="); }
    break;

  case 229: /* function_name: "operator" '[' ']' "+="  */
                                     { (yyval.s) = new string("[]+="); }
    break;

  case 230: /* function_name: "operator" '[' ']' "-="  */
                                     { (yyval.s) = new string("[]-="); }
    break;

  case 231: /* function_name: "operator" '[' ']' "*="  */
                                     { (yyval.s) = new string("[]*="); }
    break;

  case 232: /* function_name: "operator" '[' ']' "/="  */
                                     { (yyval.s) = new string("[]/="); }
    break;

  case 233: /* function_name: "operator" '[' ']' "%="  */
                                     { (yyval.s) = new string("[]%="); }
    break;

  case 234: /* function_name: "operator" '[' ']' "&="  */
                                     { (yyval.s) = new string("[]&="); }
    break;

  case 235: /* function_name: "operator" '[' ']' "|="  */
                                     { (yyval.s) = new string("[]|="); }
    break;

  case 236: /* function_name: "operator" '[' ']' "^="  */
                                     { (yyval.s) = new string("[]^="); }
    break;

  case 237: /* function_name: "operator" '[' ']' "&&="  */
                                        { (yyval.s) = new string("[]&&="); }
    break;

  case 238: /* function_name: "operator" '[' ']' "||="  */
                                        { (yyval.s) = new string("[]||="); }
    break;

  case 239: /* function_name: "operator" '[' ']' "^^="  */
                                        { (yyval.s) = new string("[]^^="); }
    break;

  case 240: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 241: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 242: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 243: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 244: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 245: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 246: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 247: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 248: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 249: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 250: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 251: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 252: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 253: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 254: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 255: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 256: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 257: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 258: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 259: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 260: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 261: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 262: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 263: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 264: /* function_name: "operator" "is" das_type_name  */
                                                { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 265: /* function_name: "operator" "as" das_type_name  */
                                                { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 266: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 267: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 268: /* function_name: "operator" '?' "as" das_type_name  */
                                                    { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 269: /* function_name: das_type_name  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 270: /* das_type_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 271: /* das_type_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 272: /* das_type_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 273: /* das_type_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 274: /* das_type_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 275: /* das_type_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 276: /* das_type_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 277: /* das_type_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 278: /* das_type_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 279: /* das_type_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 280: /* das_type_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 281: /* das_type_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 282: /* das_type_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 283: /* das_type_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 284: /* das_type_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 285: /* das_type_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 286: /* das_type_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 287: /* das_type_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 288: /* das_type_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 289: /* das_type_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 290: /* das_type_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 291: /* das_type_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 292: /* das_type_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 293: /* das_type_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 294: /* das_type_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 295: /* das_type_name: "float16"  */
                     { (yyval.s) = new string("float16"); }
    break;

  case 296: /* das_type_name: "half2"  */
                     { (yyval.s) = new string("half2"); }
    break;

  case 297: /* das_type_name: "half3"  */
                     { (yyval.s) = new string("half3"); }
    break;

  case 298: /* das_type_name: "half4"  */
                     { (yyval.s) = new string("half4"); }
    break;

  case 299: /* das_type_name: "half8"  */
                     { (yyval.s) = new string("half8"); }
    break;

  case 300: /* das_type_name: "short2"  */
                     { (yyval.s) = new string("short2"); }
    break;

  case 301: /* das_type_name: "short3"  */
                     { (yyval.s) = new string("short3"); }
    break;

  case 302: /* das_type_name: "short4"  */
                     { (yyval.s) = new string("short4"); }
    break;

  case 303: /* das_type_name: "short8"  */
                     { (yyval.s) = new string("short8"); }
    break;

  case 304: /* das_type_name: "ushort2"  */
                     { (yyval.s) = new string("ushort2"); }
    break;

  case 305: /* das_type_name: "ushort3"  */
                     { (yyval.s) = new string("ushort3"); }
    break;

  case 306: /* das_type_name: "ushort4"  */
                     { (yyval.s) = new string("ushort4"); }
    break;

  case 307: /* das_type_name: "ushort8"  */
                     { (yyval.s) = new string("ushort8"); }
    break;

  case 308: /* das_type_name: "byte2"  */
                     { (yyval.s) = new string("byte2"); }
    break;

  case 309: /* das_type_name: "byte3"  */
                     { (yyval.s) = new string("byte3"); }
    break;

  case 310: /* das_type_name: "byte4"  */
                     { (yyval.s) = new string("byte4"); }
    break;

  case 311: /* das_type_name: "byte8"  */
                     { (yyval.s) = new string("byte8"); }
    break;

  case 312: /* das_type_name: "byte16"  */
                     { (yyval.s) = new string("byte16"); }
    break;

  case 313: /* das_type_name: "ubyte2"  */
                     { (yyval.s) = new string("ubyte2"); }
    break;

  case 314: /* das_type_name: "ubyte3"  */
                     { (yyval.s) = new string("ubyte3"); }
    break;

  case 315: /* das_type_name: "ubyte4"  */
                     { (yyval.s) = new string("ubyte4"); }
    break;

  case 316: /* das_type_name: "ubyte8"  */
                     { (yyval.s) = new string("ubyte8"); }
    break;

  case 317: /* das_type_name: "ubyte16"  */
                     { (yyval.s) = new string("ubyte16"); }
    break;

  case 318: /* optional_template: %empty  */
                                        { (yyval.b) = false; }
    break;

  case 319: /* optional_template: "template"  */
                                        { (yyval.b) = true; }
    break;

  case 320: /* global_function_declaration: optional_annotation_list_with_emit_semis "def" optional_template function_declaration  */
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

  case 321: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 322: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 323: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 324: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 325: /* $@15: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 326: /* function_declaration: optional_public_or_private_function $@15 function_declaration_header optional_emit_semis block_or_simple_block  */
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

  case 327: /* expression_block_finally: %empty  */
        {
        (yyval.pExpression) = nullptr;
    }
    break;

  case 328: /* $@16: %empty  */
                  {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 329: /* $@17: %empty  */
                             {
        yyextra->pop_nesteds();
    }
    break;

  case 330: /* expression_block_finally: "finally" $@16 '{' expressions $@17 '}'  */
          {
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 331: /* $@18: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 332: /* $@19: %empty  */
                         {
        yyextra->pop_nesteds();
    }
    break;

  case 333: /* expression_block: '{' $@18 expressions $@19 '}' expression_block_finally  */
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

  case 334: /* expr_call_pipe_no_bracket: expr_call expr_full_block_assumed_piped  */
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

  case 335: /* expr_call_pipe_no_bracket: expr_method_call_no_bracket expr_full_block_assumed_piped  */
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

  case 336: /* expr_call_pipe_no_bracket: expr_field_no_bracket expr_full_block_assumed_piped  */
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

  case 337: /* expr_call_pipe_no_bracket: expr_named_call expr_full_block_assumed_piped  */
                                                                 {
        // free-function named call + piped block: pad-aware resolution lands it on the block param
        auto nc = (ExprNamedCall *) (yyvsp[-1].pExpression);
        nc->nonNamedArguments.push_back((yyvsp[0].pExpression));
        nc->pipedCallArgument = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 338: /* expression_any: SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 339: /* expression_any: expr_assign_no_bracket SEMICOLON  */
                                                    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 340: /* expression_any: expression_delete SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 341: /* expression_any: expression_let  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 342: /* expression_any: expression_while_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 343: /* expression_any: expression_unsafe  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 344: /* expression_any: expression_with  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 345: /* expression_any: expression_with_alias SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 346: /* expression_any: expression_for_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 347: /* expression_any: expression_break SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 348: /* expression_any: expression_continue SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 349: /* expression_any: expression_return SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 350: /* expression_any: expression_yield SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 351: /* expression_any: expression_if_then_else  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 352: /* expression_any: expression_if_then_else_oneliner  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 353: /* expression_any: expression_try_catch  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 354: /* expression_any: expression_label SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 355: /* expression_any: expression_goto SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 356: /* expression_any: "pass" SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 357: /* $@20: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 358: /* $@21: %empty  */
                         {
        yyextra->pop_nesteds();
    }
    break;

  case 359: /* expression_any: '{' $@20 expressions $@21 '}' expression_block_finally  */
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

  case 360: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 361: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 362: /* expressions: expressions error  */
                                 {
        (void)(yyvsp[-1].pExpression); /* gc_node — don't delete Expression */ (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 363: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 364: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 365: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 366: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 367: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 368: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 369: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 370: /* name_in_namespace: "name" "::" "name"  */
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

  case 371: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 372: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 373: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 374: /* $@22: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 375: /* $@23: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 376: /* new_type_declaration: '<' $@22 type_declaration '>' $@23  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 377: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 378: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 379: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 380: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 381: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 382: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 383: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 384: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 385: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 386: /* expression_return: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 387: /* expression_return: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 388: /* expression_return: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 389: /* expression_yield: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 390: /* expression_yield: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 391: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 392: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 393: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 394: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 395: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 396: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 397: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 398: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 399: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 400: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 401: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 402: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                        {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-5]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 403: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 404: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 405: /* expression_let: kwd_let optional_in_scope '{' variable_declaration_list '}'  */
                                                                               {
        (yyval.pExpression) = ast_LetList(scanner,(yyvsp[-4].b),(yyvsp[-3].b),*(yyvsp[-1].pVarDeclList),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 406: /* $@24: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 407: /* $@25: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 408: /* expr_cast: "cast" '<' $@24 type_declaration_no_options '>' $@25 '(' expr ')'  */
                                                                                                                                                        {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-8])),(yyvsp[-1].pExpression),(yyvsp[-5].pTypeDecl));
    }
    break;

  case 409: /* $@26: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 410: /* $@27: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 411: /* expr_cast: "upcast" '<' $@26 type_declaration_no_options '>' $@27 '(' expr ')'  */
                                                                                                                                                          {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-8])),(yyvsp[-1].pExpression),(yyvsp[-5].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 412: /* $@28: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 413: /* $@29: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 414: /* expr_cast: "reinterpret" '<' $@28 type_declaration_no_options '>' $@29 '(' expr ')'  */
                                                                                                                                                               {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-8])),(yyvsp[-1].pExpression),(yyvsp[-5].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 415: /* $@30: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 416: /* $@31: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 417: /* expr_type_decl: "type" '<' $@30 type_declaration '>' $@31  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 418: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 419: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 420: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" c_or_s "name" '>' '(' expr ')'  */
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

  case 421: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 422: /* expr_list: "<-" expr  */
                             {
            (yyval.pExpression) = ast_makeMoveArgument(scanner, (yyvsp[0].pExpression), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 423: /* expr_list: expr_list ',' expr  */
                                        {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 424: /* expr_list: expr_list ',' "<-" expr  */
                                                   {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-3])),(yyvsp[-3].pExpression),ast_makeMoveArgument(scanner, (yyvsp[0].pExpression), tokAt(scanner,(yylsp[0]))));
    }
    break;

  case 425: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 426: /* block_or_simple_block: "=>" expr_no_bracket  */
                                                   {
            auto retE = new ExprReturn(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 427: /* block_or_simple_block: "=>" "<-" expr_no_bracket  */
                                                          {
            auto retE = new ExprReturn(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 428: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 429: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 430: /* block_or_lambda: "@@"  */
                  { (yyval.i) = 2;   /* local function */ }
    break;

  case 431: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 432: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 433: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 434: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 435: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 436: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 437: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 438: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 439: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 440: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 441: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 442: /* expr_full_block_assumed_piped: '{' expressions '}'  */
                                   {
        // block span is brace-to-brace (@$), not the statements' span (@block)
        (yyval.pExpression) = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,new TypeDecl(Type::autoinfer),(yyvsp[-1].pExpression),tokAt(scanner,(yyloc)),tokAt(scanner,(yyloc)),LineInfo());
    }
    break;

  case 443: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 444: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 445: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 446: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 447: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 448: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 449: /* expr_numeric_const: "float16 constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat16(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 450: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 451: /* expr_assign_no_bracket: expr_no_bracket  */
                                                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 452: /* expr_assign_no_bracket: expr_no_bracket '=' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 453: /* expr_assign_no_bracket: expr_no_bracket "<-" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 454: /* expr_assign_no_bracket: expr_no_bracket "<-" make_table_decl  */
                                                                   { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 455: /* expr_assign_no_bracket: expr_no_bracket "<-" array_comprehension  */
                                                                     { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 456: /* expr_assign_no_bracket: expr_no_bracket ":=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 457: /* expr_assign_no_bracket: expr_no_bracket "&=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 458: /* expr_assign_no_bracket: expr_no_bracket "|=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 459: /* expr_assign_no_bracket: expr_no_bracket "^=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 460: /* expr_assign_no_bracket: expr_no_bracket "&&=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 461: /* expr_assign_no_bracket: expr_no_bracket "||=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 462: /* expr_assign_no_bracket: expr_no_bracket "^^=" expr_no_bracket  */
                                                                      { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 463: /* expr_assign_no_bracket: expr_no_bracket "+=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 464: /* expr_assign_no_bracket: expr_no_bracket "-=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 465: /* expr_assign_no_bracket: expr_no_bracket "*=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 466: /* expr_assign_no_bracket: expr_no_bracket "/=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 467: /* expr_assign_no_bracket: expr_no_bracket "%=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 468: /* expr_assign_no_bracket: expr_no_bracket "<<=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 469: /* expr_assign_no_bracket: expr_no_bracket ">>=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr_assign_no_bracket: expr_no_bracket "<<<=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 471: /* expr_assign_no_bracket: expr_no_bracket ">>>=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 473: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 474: /* expr_named_call: name_in_namespace '(' expr_list ',' make_struct_fields ')'  */
                                                                                          {
        // bracket-less mixed named call: foo(pos..., name = value) -- named args are a strict suffix
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-3].pExpression));
        nc->arguments = (yyvsp[-1].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 475: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' ')'  */
                                                                    {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 476: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' expr_list ')'  */
                                                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 477: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                                     {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = (yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-7].pExpression));
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 478: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' expr_list ',' make_struct_fields ')'  */
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

  case 479: /* expr_method_call_no_bracket: expr_no_bracket "->" "name" '(' make_struct_fields ')'  */
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

  case 480: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 481: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 482: /* func_addr_expr: "@@" func_addr_name  */
                                            {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 483: /* $@32: %empty  */
                      { yyextra->das_arrow_depth ++; }
    break;

  case 484: /* $@33: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 485: /* func_addr_expr: "@@" '<' $@32 type_declaration_no_options '>' $@33 func_addr_name  */
                                                                                                                                                         {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 486: /* $@34: %empty  */
                      { yyextra->das_arrow_depth ++; }
    break;

  case 487: /* $@35: %empty  */
                                                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 488: /* func_addr_expr: "@@" '<' $@34 optional_function_argument_list optional_function_type '>' $@35 func_addr_name  */
                                                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value));
        expr->funcType = new TypeDecl(Type::tFunction);
        expr->funcType->at = expr->at;
        expr->funcType->firstType = (yyvsp[-3].pTypeDecl);
        if ( (yyvsp[-4].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, expr->funcType, (yyvsp[-4].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-4].pVarDeclList));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 489: /* expr_field_no_bracket: expr_no_bracket '.' "name"  */
                                                         {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 490: /* expr_field_no_bracket: expr_no_bracket '.' '.' "name"  */
                                                             {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 491: /* expr_field_no_bracket: expr_no_bracket "!." "name"  */
                                                            {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 492: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' ')'  */
                                                                 {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 493: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' expr_list ')'  */
                                                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 494: /* expr_field_no_bracket: expr_no_bracket "!." "name" '(' ')'  */
                                                                    {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s), true);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 495: /* expr_field_no_bracket: expr_no_bracket "!." "name" '(' expr_list ')'  */
                                                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s), true);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 496: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = (yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-7].pExpression));
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 497: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' expr_list ',' make_struct_fields ')'  */
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

  case 498: /* expr_field_no_bracket: expr_no_bracket '.' "name" '(' make_struct_fields ')'  */
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

  case 499: /* expr_field_no_bracket: expr_no_bracket '.' basic_type_declaration '(' ')'  */
                                                                                   {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 500: /* expr_field_no_bracket: expr_no_bracket '.' basic_type_declaration '(' expr_list ')'  */
                                                                                                        {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 501: /* $@36: %empty  */
                                          { yyextra->das_suppress_errors=true; }
    break;

  case 502: /* $@37: %empty  */
                                                                                       { yyextra->das_suppress_errors=false; }
    break;

  case 503: /* expr_field_no_bracket: expr_no_bracket '.' $@36 error $@37  */
                                                                                                                               {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 504: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 505: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 506: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 507: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 508: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 509: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 510: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 511: /* expr: expr_no_bracket  */
                                       { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 512: /* expr: make_table_decl  */
                                     { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 513: /* expr: array_comprehension  */
                                     { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 514: /* expr_no_bracket: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 515: /* expr_no_bracket: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 516: /* expr_no_bracket: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 517: /* expr_no_bracket: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 518: /* expr_no_bracket: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 519: /* expr_no_bracket: make_decl_no_bracket  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 520: /* expr_no_bracket: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 521: /* expr_no_bracket: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 522: /* expr_no_bracket: expr_field_no_bracket  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 523: /* expr_no_bracket: expr_mtag_no_bracket  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 524: /* expr_no_bracket: '!' expr_no_bracket  */
                                                         { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 525: /* expr_no_bracket: '~' expr_no_bracket  */
                                                         { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 526: /* expr_no_bracket: '+' expr_no_bracket  */
                                                             { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 527: /* expr_no_bracket: '-' expr_no_bracket  */
                                                             { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 528: /* expr_no_bracket: expr_no_bracket "<<" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 529: /* expr_no_bracket: expr_no_bracket ">>" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 530: /* expr_no_bracket: expr_no_bracket "<<<" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 531: /* expr_no_bracket: expr_no_bracket ">>>" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 532: /* expr_no_bracket: expr_no_bracket '+' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 533: /* expr_no_bracket: expr_no_bracket '-' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 534: /* expr_no_bracket: expr_no_bracket '*' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 535: /* expr_no_bracket: expr_no_bracket '/' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 536: /* expr_no_bracket: expr_no_bracket '%' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 537: /* expr_no_bracket: expr_no_bracket '<' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 538: /* expr_no_bracket: expr_no_bracket '>' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 539: /* expr_no_bracket: expr_no_bracket "==" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 540: /* expr_no_bracket: expr_no_bracket "!=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 541: /* expr_no_bracket: expr_no_bracket "<=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 542: /* expr_no_bracket: expr_no_bracket ">=" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 543: /* expr_no_bracket: expr_no_bracket '&' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 544: /* expr_no_bracket: expr_no_bracket '|' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 545: /* expr_no_bracket: expr_no_bracket '^' expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 546: /* expr_no_bracket: expr_no_bracket "&&" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 547: /* expr_no_bracket: expr_no_bracket "||" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 548: /* expr_no_bracket: expr_no_bracket "^^" expr_no_bracket  */
                                                                   { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 549: /* expr_no_bracket: expr_no_bracket ".." expr_no_bracket  */
                                                                   {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 550: /* expr_no_bracket: "++" expr_no_bracket  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 551: /* expr_no_bracket: "--" expr_no_bracket  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 552: /* expr_no_bracket: expr_no_bracket "++"  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 553: /* expr_no_bracket: expr_no_bracket "--"  */
                                                            { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 554: /* expr_no_bracket: '(' expr_list optional_comma ')'  */
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

  case 555: /* expr_no_bracket: '(' make_struct_single ')'  */
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

  case 556: /* expr_no_bracket: expr_no_bracket '[' expr ']'  */
                                                            { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 557: /* expr_no_bracket: expr_no_bracket '.' '[' expr ']'  */
                                                                { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 558: /* expr_no_bracket: expr_no_bracket "![" expr ']'  */
                                                               { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 559: /* expr_no_bracket: expr_no_bracket "?[" expr ']'  */
                                                            { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 560: /* expr_no_bracket: expr_no_bracket '.' "?[" expr ']'  */
                                                                { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 561: /* expr_no_bracket: expr_no_bracket "!?[" expr ']'  */
                                                               { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 562: /* expr_no_bracket: expr_no_bracket "?." "name"  */
                                                            { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 563: /* expr_no_bracket: expr_no_bracket '.' "?." "name"  */
                                                                { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 564: /* expr_no_bracket: expr_no_bracket "!?." "name"  */
                                                               { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 565: /* expr_no_bracket: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 566: /* expr_no_bracket: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 567: /* expr_no_bracket: '*' expr_no_bracket  */
                                                              { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 568: /* expr_no_bracket: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 569: /* expr_no_bracket: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 570: /* $@38: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 571: /* $@39: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 572: /* expr_no_bracket: "addr" '<' $@38 type_declaration_no_options '>' $@39 '(' expr ')'  */
                                                                                                                                                        {
        auto pRef2Ptr = new ExprRef2Ptr(tokAt(scanner,(yylsp[-8])),(yyvsp[-1].pExpression));
        pRef2Ptr->generated = true;
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-8])),pRef2Ptr,(yyvsp[-5].pTypeDecl));
        pCast->reinterpret = true;
        pCast->fromAddrSugar = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 573: /* expr_no_bracket: expr_generator  */
                                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 574: /* expr_no_bracket: expr_no_bracket "??" expr_no_bracket  */
                                                                         { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 575: /* expr_no_bracket: expr_no_bracket "!??" expr_no_bracket  */
                                                                            {
        auto nc = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        nc->no_promotion = true;
        (yyval.pExpression) = nc;
    }
    break;

  case 576: /* expr_no_bracket: expr_no_bracket '?' expr_no_bracket ':' expr_no_bracket  */
                                                                                           {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 577: /* $@40: %empty  */
                                                          { yyextra->das_arrow_depth ++; }
    break;

  case 578: /* $@41: %empty  */
                                                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 579: /* expr_no_bracket: expr_no_bracket "is" "type" '<' $@40 type_declaration_no_options '>' $@41  */
                                                                                                                                                                  {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 580: /* expr_no_bracket: expr_no_bracket "is" basic_type_declaration  */
                                                                          {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 581: /* expr_no_bracket: expr_no_bracket "is" "name"  */
                                                         {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 582: /* $@42: %empty  */
                                                         { yyextra->das_arrow_depth ++; }
    break;

  case 583: /* $@43: %empty  */
                                                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 584: /* expr_no_bracket: expr_no_bracket "!is" "type" '<' $@42 type_declaration_no_options '>' $@43  */
                                                                                                                                                                 {
        auto isx = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
        isx->no_promotion = true;
        (yyval.pExpression) = isx;
    }
    break;

  case 585: /* expr_no_bracket: expr_no_bracket "!is" basic_type_declaration  */
                                                                         {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        auto isx = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
        isx->no_promotion = true;
        (yyval.pExpression) = isx;
    }
    break;

  case 586: /* expr_no_bracket: expr_no_bracket "!is" "name"  */
                                                        {
        auto isv = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        isv->no_promotion = true;
        (yyval.pExpression) = isv;
        delete (yyvsp[0].s);
    }
    break;

  case 587: /* expr_no_bracket: expr_no_bracket "as" "name"  */
                                                         {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 588: /* $@44: %empty  */
                                                          { yyextra->das_arrow_depth ++; }
    break;

  case 589: /* $@45: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 590: /* expr_no_bracket: expr_no_bracket "as" "type" '<' $@44 type_declaration '>' $@45  */
                                                                                                                                                       {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 591: /* expr_no_bracket: expr_no_bracket "as" basic_type_declaration  */
                                                                          {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 592: /* expr_no_bracket: expr_no_bracket "!as" "name"  */
                                                        {
        auto asv = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
        delete (yyvsp[0].s);
    }
    break;

  case 593: /* $@46: %empty  */
                                                         { yyextra->das_arrow_depth ++; }
    break;

  case 594: /* $@47: %empty  */
                                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 595: /* expr_no_bracket: expr_no_bracket "!as" "type" '<' $@46 type_declaration '>' $@47  */
                                                                                                                                                      {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        auto asv = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 596: /* expr_no_bracket: expr_no_bracket "!as" basic_type_declaration  */
                                                                         {
        auto asv = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
    }
    break;

  case 597: /* expr_no_bracket: expr_no_bracket '?' "as" "name"  */
                                                             {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 598: /* $@48: %empty  */
                                                              { yyextra->das_arrow_depth ++; }
    break;

  case 599: /* $@49: %empty  */
                                                                                                                          { yyextra->das_arrow_depth --; }
    break;

  case 600: /* expr_no_bracket: expr_no_bracket '?' "as" "type" '<' $@48 type_declaration '>' $@49  */
                                                                                                                                                           {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 601: /* expr_no_bracket: expr_no_bracket '?' "as" basic_type_declaration  */
                                                                              {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 602: /* expr_no_bracket: expr_no_bracket "!?as" "name"  */
                                                         {
        auto asv = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
        delete (yyvsp[0].s);
    }
    break;

  case 603: /* $@50: %empty  */
                                                          { yyextra->das_arrow_depth ++; }
    break;

  case 604: /* $@51: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 605: /* expr_no_bracket: expr_no_bracket "!?as" "type" '<' $@50 type_declaration '>' $@51  */
                                                                                                                                                       {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        auto asv = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 606: /* expr_no_bracket: expr_no_bracket "!?as" basic_type_declaration  */
                                                                          {
        auto asv = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
        asv->no_promotion = true;
        (yyval.pExpression) = asv;
    }
    break;

  case 607: /* expr_no_bracket: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 608: /* expr_no_bracket: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 609: /* expr_no_bracket: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 610: /* expr_no_bracket: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 611: /* expr_no_bracket: expr_method_call_no_bracket  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 612: /* expr_no_bracket: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 613: /* expr_no_bracket: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 614: /* expr_no_bracket: expr_no_bracket "<|" expr_no_bracket  */
                                                                      { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1])),true); }
    break;

  case 615: /* expr_no_bracket: expr_no_bracket "|>" expr_no_bracket  */
                                                                      { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 616: /* expr_no_bracket: expr_no_bracket "|>" basic_type_declaration  */
                                                                     {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 617: /* expr_no_bracket: expr_call_pipe_no_bracket  */
                                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 618: /* expr_no_bracket: "unsafe" '(' expr ')'  */
                                         {
            (yyvsp[-1].pExpression)->alwaysSafe = true;
            (yyvsp[-1].pExpression)->userSaidItsSafe = true;
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    break;

  case 619: /* expr_no_bracket: expr_no_bracket "=>" expr_no_bracket  */
                                                               {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 620: /* expr_no_bracket: expr_no_bracket "=>" make_table_decl  */
                                                               {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 621: /* expr_no_bracket: expr_no_bracket "=>" array_comprehension  */
                                                                   {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 622: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 623: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 624: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list optional_emit_semis expression_block  */
                                                                                                                                                  {
        auto closure = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->returnType = new TypeDecl(Type::autoinfer);
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),closure,tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 625: /* expr_mtag_no_bracket: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 626: /* expr_mtag_no_bracket: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 627: /* expr_mtag_no_bracket: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 628: /* expr_mtag_no_bracket: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 629: /* expr_mtag_no_bracket: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 630: /* expr_mtag_no_bracket: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 631: /* expr_mtag_no_bracket: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 632: /* expr_mtag_no_bracket: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 633: /* expr_mtag_no_bracket: expr_no_bracket '.' "$f" '(' expr ')'  */
                                                                           {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 634: /* expr_mtag_no_bracket: expr_no_bracket "?." "$f" '(' expr ')'  */
                                                                            {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 635: /* expr_mtag_no_bracket: expr_no_bracket '.' '.' "$f" '(' expr ')'  */
                                                                               {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 636: /* expr_mtag_no_bracket: expr_no_bracket "!." "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 637: /* expr_mtag_no_bracket: expr_no_bracket '.' "?." "$f" '(' expr ')'  */
                                                                                {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 638: /* expr_mtag_no_bracket: expr_no_bracket "!?." "$f" '(' expr ')'  */
                                                                               {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 639: /* expr_mtag_no_bracket: expr_no_bracket "as" "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 640: /* expr_mtag_no_bracket: expr_no_bracket "!as" "$f" '(' expr ')'  */
                                                                             {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        cfield->no_promotion = true;
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 641: /* expr_mtag_no_bracket: expr_no_bracket '?' "as" "$f" '(' expr ')'  */
                                                                                  {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 642: /* expr_mtag_no_bracket: expr_no_bracket "!?as" "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        cfield->no_promotion = true;
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 643: /* expr_mtag_no_bracket: expr_no_bracket "is" "$f" '(' expr ')'  */
                                                                              {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 644: /* expr_mtag_no_bracket: expr_no_bracket "!is" "$f" '(' expr ')'  */
                                                                             {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        cfield->no_promotion = true;
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 645: /* expr_mtag_no_bracket: "@@" "$c" '(' expr ')'  */
                                                           {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 646: /* optional_field_annotation: %empty  */
                                      { (yyval.aaList) = nullptr; }
    break;

  case 647: /* optional_field_annotation: metadata_argument_list  */
                                      { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 648: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 649: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 650: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 651: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 652: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 653: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 654: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 655: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 656: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 657: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 658: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 659: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 660: /* struct_variable_declaration_list: struct_variable_declaration_list "new line, semicolon"  */
                                                                 { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 661: /* struct_variable_declaration_list: struct_variable_declaration_list "typedef" "name" '=' type_declaration SEMICOLON  */
                                                                                                                {
        (yyval.pVarDeclList) = (yyvsp[-5].pVarDeclList);
        ast_structureAlias(scanner,(yyvsp[-3].s),(yyvsp[-1].pTypeDecl),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 662: /* $@52: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 663: /* struct_variable_declaration_list: struct_variable_declaration_list $@52 structure_variable_declaration SEMICOLON  */
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

  case 664: /* $@53: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 665: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable "abstract" optional_constant $@53 function_declaration_header SEMICOLON  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 666: /* $@54: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 667: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@54 function_declaration_header optional_emit_semis block_or_simple_block  */
                                                                                                 {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-10].pVarDeclList),(yyvsp[-9].faList),(yyvsp[-6].b),(yyvsp[-7].b),(yyvsp[-5].i),(yyvsp[-4].b),(yyvsp[-2].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-8]),(yylsp[0])),tokAt(scanner,(yylsp[-9])));
            }
    break;

  case 668: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 669: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 670: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 671: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 672: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 673: /* function_argument_list: function_argument_declaration_no_type ';' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 674: /* function_argument_list: function_argument_declaration_type ';' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 675: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 676: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 677: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 678: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 679: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 680: /* tuple_alias_type_list: %empty  */
      {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 681: /* tuple_alias_type_list: tuple_type  */
                       {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 682: /* tuple_alias_type_list: tuple_alias_type_list semis tuple_type  */
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

  case 683: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 684: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 685: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 686: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 687: /* variant_alias_type_list: variant_type  */
                         {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 688: /* variant_alias_type_list: variant_alias_type_list semis variant_type  */
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

  case 689: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 690: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 691: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 692: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 693: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 694: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 695: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 696: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 697: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 698: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 699: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 700: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 701: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 702: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 703: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 704: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 705: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 706: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 707: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                               {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 708: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 709: /* global_let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 710: /* global_let_variable_name_with_pos_list: global_let_variable_name_with_pos_list ',' "name"  */
                                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 711: /* variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 712: /* variable_declaration_list: variable_declaration_list SEMICOLON  */
                                                  {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 713: /* variable_declaration_list: variable_declaration_list let_variable_declaration  */
                                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 714: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 715: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 716: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 717: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                         {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 718: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                               {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 719: /* global_let_variable_declaration: global_let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 720: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 721: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 722: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 723: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 724: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 725: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 726: /* global_variable_declaration_list: global_variable_declaration_list SEMICOLON  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 727: /* $@55: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 728: /* global_variable_declaration_list: global_variable_declaration_list $@55 optional_field_annotation let_variable_declaration  */
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

  case 729: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 730: /* $@56: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 731: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@56 optional_field_annotation global_let_variable_declaration  */
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

  case 732: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        delete (yyvsp[0].s);
    }
    break;

  case 733: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
        delete (yyvsp[-2].s);
    }
    break;

  case 736: /* enum_list: %empty  */
        {
        (yyval.pEnumList) = new Enumeration();
    }
    break;

  case 737: /* enum_list: enum_expression  */
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

  case 738: /* enum_list: enum_list commas enum_expression  */
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

  case 739: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 740: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 741: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 742: /* $@57: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 743: /* single_alias: optional_public_or_private_alias "name" $@57 '=' type_declaration  */
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

  case 746: /* distinct_alias: optional_public_or_private_alias "name" "name" '=' type_declaration  */
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

  case 747: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 748: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 749: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 750: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 751: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 752: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 759: /* $@58: %empty  */
                                                                     {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 760: /* $@59: %empty  */
                                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 761: /* $@60: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 762: /* enum_declaration: optional_annotation_list_with_emit_semis "enum" $@58 optional_public_or_private_enum enum_name optional_enum_basic_type_declaration optional_emit_commas '{' $@59 enum_list optional_commas $@60 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-8].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-12].faList),tokAt(scanner,(yylsp[-12])),(yyvsp[-9].b),(yyvsp[-8].pEnum),(yyvsp[-3].pEnumList),(yyvsp[-7].type));
    }
    break;

  case 763: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 764: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 765: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 766: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 767: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 768: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 769: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 770: /* class_or_struct: "class" "template"  */
                                  { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 771: /* class_or_struct: "struct" "template"  */
                                  { (yyval.i) = CorS_StructTemplate; }
    break;

  case 772: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 773: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 774: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 775: /* optional_struct_variable_declaration_list: ';'  */
            {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 776: /* optional_struct_variable_declaration_list: '{' struct_variable_declaration_list '}'  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 777: /* $@61: %empty  */
                                                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 778: /* $@62: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 779: /* $@63: %empty  */
                                             {
        if ( (yyvsp[-1].pStructure) ) {
            (yyvsp[-1].pStructure)->isClass = (yyvsp[-4].i)==CorS_Class || (yyvsp[-4].i)==CorS_ClassTemplate;
            (yyvsp[-1].pStructure)->isTemplate = (yyvsp[-4].i)==CorS_ClassTemplate || (yyvsp[-4].i)==CorS_StructTemplate;
            (yyvsp[-1].pStructure)->privateStructure = !(yyvsp[-3].b);
        }
    }
    break;

  case 780: /* structure_declaration: optional_annotation_list_with_emit_semis $@61 class_or_struct optional_public_or_private_structure $@62 structure_name optional_emit_semis $@63 optional_struct_variable_declaration_list  */
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

  case 781: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 782: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 783: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 784: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 785: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                           {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 786: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 787: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 788: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 789: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 790: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 791: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 792: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 793: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 794: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 795: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 796: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 797: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 798: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 799: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 800: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 801: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 802: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 803: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 804: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 805: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 806: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 807: /* basic_type_declaration: "float16"  */
                        { (yyval.type) = Type::tFloat16; }
    break;

  case 808: /* basic_type_declaration: "half2"  */
                        { (yyval.type) = Type::tHalf2; }
    break;

  case 809: /* basic_type_declaration: "half3"  */
                        { (yyval.type) = Type::tHalf3; }
    break;

  case 810: /* basic_type_declaration: "half4"  */
                        { (yyval.type) = Type::tHalf4; }
    break;

  case 811: /* basic_type_declaration: "half8"  */
                        { (yyval.type) = Type::tHalf8; }
    break;

  case 812: /* basic_type_declaration: "short2"  */
                        { (yyval.type) = Type::tShort2; }
    break;

  case 813: /* basic_type_declaration: "short3"  */
                        { (yyval.type) = Type::tShort3; }
    break;

  case 814: /* basic_type_declaration: "short4"  */
                        { (yyval.type) = Type::tShort4; }
    break;

  case 815: /* basic_type_declaration: "short8"  */
                        { (yyval.type) = Type::tShort8; }
    break;

  case 816: /* basic_type_declaration: "ushort2"  */
                        { (yyval.type) = Type::tUShort2; }
    break;

  case 817: /* basic_type_declaration: "ushort3"  */
                        { (yyval.type) = Type::tUShort3; }
    break;

  case 818: /* basic_type_declaration: "ushort4"  */
                        { (yyval.type) = Type::tUShort4; }
    break;

  case 819: /* basic_type_declaration: "ushort8"  */
                        { (yyval.type) = Type::tUShort8; }
    break;

  case 820: /* basic_type_declaration: "byte2"  */
                        { (yyval.type) = Type::tByte2; }
    break;

  case 821: /* basic_type_declaration: "byte3"  */
                        { (yyval.type) = Type::tByte3; }
    break;

  case 822: /* basic_type_declaration: "byte4"  */
                        { (yyval.type) = Type::tByte4; }
    break;

  case 823: /* basic_type_declaration: "byte8"  */
                        { (yyval.type) = Type::tByte8; }
    break;

  case 824: /* basic_type_declaration: "byte16"  */
                        { (yyval.type) = Type::tByte16; }
    break;

  case 825: /* basic_type_declaration: "ubyte2"  */
                        { (yyval.type) = Type::tUByte2; }
    break;

  case 826: /* basic_type_declaration: "ubyte3"  */
                        { (yyval.type) = Type::tUByte3; }
    break;

  case 827: /* basic_type_declaration: "ubyte4"  */
                        { (yyval.type) = Type::tUByte4; }
    break;

  case 828: /* basic_type_declaration: "ubyte8"  */
                        { (yyval.type) = Type::tUByte8; }
    break;

  case 829: /* basic_type_declaration: "ubyte16"  */
                        { (yyval.type) = Type::tUByte16; }
    break;

  case 830: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 831: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 832: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 833: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 834: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 835: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 836: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 837: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 838: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 839: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 840: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 841: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 842: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 843: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 844: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 845: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 846: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 847: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 848: /* auto_type_declaration: "$t" '(' expr ')'  */
                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::alias);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = "``MACRO``TAG``";
        (yyval.pTypeDecl)->isTag = true;
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner, (yylsp[-1]));
        (yyval.pTypeDecl)->firstType->typeMacroExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 849: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 850: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 851: /* bitfield_bits: bitfield_bits ',' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 852: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<tuple<string,Expression *>>();
        (yyval.pNameExprList) = pSL;

    }
    break;

  case 853: /* bitfield_alias_bits: "name"  */
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

  case 854: /* bitfield_alias_bits: "name" '=' expr  */
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

  case 855: /* bitfield_alias_bits: bitfield_alias_bits commas "name"  */
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

  case 856: /* bitfield_alias_bits: bitfield_alias_bits commas "name" '=' expr  */
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

  case 857: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 858: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 859: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 860: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 861: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 862: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' '>'  */
                                                                          {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-2].type));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-2]));
    }
    break;

  case 863: /* $@64: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 864: /* $@65: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 865: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@64 bitfield_bits '>' $@65  */
                                                                                                                                                             {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-5].type));
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

  case 868: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
        (yyval.aTypePair).secondType->at = (yyval.aTypePair).firstType->at;
    }
    break;

  case 869: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 870: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = appendDimExpr(nullptr, (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 871: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = appendDimExpr(nullptr, nullptr, tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 872: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = appendDimExpr((yyvsp[-3].pTypeDecl), (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 873: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = appendDimExpr((yyvsp[-2].pTypeDecl), nullptr, tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 874: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 875: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
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

  case 876: /* optional_expr_list_in_braces: %empty  */
            { (yyval.pExpression) = nullptr; }
    break;

  case 877: /* optional_expr_list_in_braces: '(' expr_list optional_comma ')'  */
                                                { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 878: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 879: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 880: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 881: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 882: /* $@66: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 883: /* $@67: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 884: /* type_declaration_no_options_no_dim: "type" '<' $@66 type_declaration '>' $@67  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 885: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->typeMacroExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 886: /* type_declaration_no_options_no_dim: name_in_namespace '(' optional_expr_list ')'  */
                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->typeMacroExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 887: /* type_declaration_no_options_no_dim: '$' name_in_namespace optional_expr_list_in_braces  */
                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-1]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = sequenceToList((yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-1])), *(yyvsp[-1].s)));
        delete (yyvsp[-1].s);
    }
    break;

  case 888: /* $@68: %empty  */
                                    { yyextra->das_arrow_depth ++; }
    break;

  case 889: /* type_declaration_no_options_no_dim: name_in_namespace '<' $@68 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 890: /* $@69: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 891: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@69 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 892: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 893: /* type_declaration_no_options_no_dim: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 894: /* type_declaration_no_options_no_dim: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 895: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 896: /* type_declaration_no_options_no_dim: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 897: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 898: /* type_declaration_no_options_no_dim: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 899: /* type_declaration_no_options_no_dim: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 900: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 901: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 902: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 903: /* type_declaration_no_options_no_dim: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 904: /* $@70: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 905: /* $@71: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 906: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@70 type_declaration '>' $@71  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 907: /* type_declaration_no_options_no_dim: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 908: /* $@72: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 909: /* $@73: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 910: /* type_declaration_no_options_no_dim: "array" '<' $@72 type_declaration '>' $@73  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 911: /* $@74: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 912: /* $@75: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 913: /* type_declaration_no_options_no_dim: "table" '<' $@74 table_type_pair '>' $@75  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 914: /* $@76: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 915: /* $@77: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 916: /* type_declaration_no_options_no_dim: "iterator" '<' $@76 type_declaration '>' $@77  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 917: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid);
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 918: /* $@78: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 919: /* $@79: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 920: /* type_declaration_no_options_no_dim: "block" '<' $@78 type_declaration '>' $@79  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 921: /* $@80: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 922: /* $@81: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 923: /* type_declaration_no_options_no_dim: "block" '<' $@80 optional_function_argument_list optional_function_type '>' $@81  */
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 924: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid);
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 925: /* $@82: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 926: /* $@83: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 927: /* type_declaration_no_options_no_dim: "function" '<' $@82 type_declaration '>' $@83  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 928: /* $@84: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 929: /* $@85: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 930: /* type_declaration_no_options_no_dim: "function" '<' $@84 optional_function_argument_list optional_function_type '>' $@85  */
                                                                                                                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 931: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid);
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 932: /* $@86: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 933: /* $@87: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 934: /* type_declaration_no_options_no_dim: "lambda" '<' $@86 type_declaration '>' $@87  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 935: /* $@88: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 936: /* $@89: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 937: /* type_declaration_no_options_no_dim: "lambda" '<' $@88 optional_function_argument_list optional_function_type '>' $@89  */
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 938: /* $@90: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 939: /* $@91: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 940: /* type_declaration_no_options_no_dim: "tuple" '<' $@90 tuple_type_list '>' $@91  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 941: /* $@92: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 942: /* $@93: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 943: /* type_declaration_no_options_no_dim: "variant" '<' $@92 variant_type_list '>' $@93  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 944: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 945: /* type_declaration: type_declaration '|' type_declaration_no_options  */
                                                                     {
        if ( (yyvsp[-2].pTypeDecl)->baseType==Type::option ) {
            (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[0].pTypeDecl));
        } else {
            (yyval.pTypeDecl) = new TypeDecl(Type::option);
            (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[-2].pTypeDecl));
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[0].pTypeDecl));
        }
    }
    break;

  case 946: /* type_declaration: type_declaration '|' '#'  */
                                             {
        if ( (yyvsp[-2].pTypeDecl)->baseType==Type::option ) {
            (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
            (yyval.pTypeDecl)->argTypes.push_back(new TypeDecl(*(yyvsp[-2].pTypeDecl)->argTypes.back()));
            (yyvsp[-2].pTypeDecl)->argTypes.back()->temporary ^= true;
        } else {
            (yyval.pTypeDecl) = new TypeDecl(Type::option);
            (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[-2].pTypeDecl));
            (yyval.pTypeDecl)->argTypes.push_back(new TypeDecl(*(yyvsp[-2].pTypeDecl)));
            (yyval.pTypeDecl)->argTypes.back()->temporary ^= true;
        }
    }
    break;

  case 947: /* $@94: %empty  */
                   {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 948: /* $@95: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 949: /* $@96: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 950: /* $@97: %empty  */
                                                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 951: /* tuple_alias_declaration: "tuple" $@94 optional_public_or_private_alias "name" optional_emit_semis $@95 '{' $@96 tuple_alias_type_list optional_semis $@97 '}'  */
          {
        auto vtype = new TypeDecl(Type::tTuple);
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

  case 952: /* $@98: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 953: /* $@99: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 954: /* $@100: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 955: /* $@101: %empty  */
                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 956: /* variant_alias_declaration: "variant" $@98 optional_public_or_private_alias "name" optional_emit_semis $@99 '{' $@100 variant_alias_type_list optional_semis $@101 '}'  */
          {
        auto vtype = new TypeDecl(Type::tVariant);
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

  case 957: /* $@102: %empty  */
                      {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 958: /* $@103: %empty  */
                                                                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 959: /* $@104: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 960: /* $@105: %empty  */
                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 961: /* bitfield_alias_declaration: "bitfield" $@102 optional_public_or_private_alias "name" bitfield_basic_type_declaration optional_emit_commas $@103 '{' $@104 bitfield_alias_bits optional_commas $@105 '}'  */
          {
        auto btype = new TypeDecl((yyvsp[-8].type));
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

  case 962: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 963: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 964: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 965: /* make_decl: make_table_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 966: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 967: /* make_decl: table_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 968: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 969: /* make_decl_no_bracket: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 970: /* make_decl_no_bracket: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 971: /* make_decl_no_bracket: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 972: /* make_decl_no_bracket: table_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 973: /* make_decl_no_bracket: make_table_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 974: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 975: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 976: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 977: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 978: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 979: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 980: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 981: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 982: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 983: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 984: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 985: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 986: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 987: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 988: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 989: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 990: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 991: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 992: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 993: /* $@106: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 994: /* $@107: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 995: /* make_struct_decl: "struct" '<' $@106 type_declaration_no_options '>' $@107 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 996: /* $@108: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 997: /* $@109: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 998: /* make_struct_decl: "class" '<' $@108 type_declaration_no_options '>' $@109 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 999: /* $@110: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 1000: /* $@111: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 1001: /* make_struct_decl: "variant" '<' $@110 variant_type_list '>' $@111 '(' use_initializer make_variant_dim ')'  */
                                                                                                                                                                                  {
        auto mkt = new TypeDecl(Type::tVariant);
        mkt->at = tokAt(scanner,(yylsp[-9]));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1002: /* $@112: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 1003: /* $@113: %empty  */
                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 1004: /* make_struct_decl: "variant" "type" '<' $@112 type_declaration_no_options '>' $@113 '(' use_initializer make_variant_dim ')'  */
                                                                                                                                                                                                    {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1005: /* $@114: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 1006: /* $@115: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 1007: /* make_struct_decl: "default" '<' $@114 type_declaration_no_options '>' $@115 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 1008: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = new TypeDecl(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 1009: /* $@116: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 1010: /* $@117: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 1011: /* make_tuple_call: "tuple" '<' $@116 tuple_type_list '>' $@117 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                 {
        auto mkt = new TypeDecl(Type::tTuple);
        mkt->at = tokAt(scanner,(yylsp[-9]));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 1012: /* make_dim_decl: '[' optional_expr_list ']'  */
                                                  {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-2])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = new TypeDecl(Type::autoinfer);
            mka->gen2 = true;
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto mks = new ExprMakeStruct();
            mks->at = tokAt(scanner,(yylsp[-2]));
            mks->makeType = new TypeDecl(Type::tArray);
            mks->makeType->at = mks->at;
            mks->makeType->firstType = new TypeDecl(Type::autoinfer);
            mks->makeType->firstType->at = mks->at;
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }
    }
    break;

  case 1013: /* $@118: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 1014: /* $@119: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 1015: /* make_dim_decl: "array" "struct" '<' $@118 type_declaration_no_options '>' $@119 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 1016: /* $@120: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 1017: /* $@121: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 1018: /* make_dim_decl: "array" "tuple" '<' $@120 tuple_type_list '>' $@121 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        auto mkt = new TypeDecl(Type::tTuple);
        mkt->at = tokAt(scanner,(yylsp[-10]));
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

  case 1019: /* $@122: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 1020: /* $@123: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 1021: /* make_dim_decl: "array" "variant" '<' $@122 variant_type_list '>' $@123 '(' make_variant_dim ')'  */
                                                                                                                                                                      {
        auto mkt = new TypeDecl(Type::tVariant);
        mkt->at = tokAt(scanner,(yylsp[-9]));
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

  case 1022: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer);
        mka->gen2 = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 1023: /* $@124: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 1024: /* $@125: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 1025: /* make_dim_decl: "array" '<' $@124 type_declaration_no_options '>' $@125 '(' optional_expr_list ')'  */
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
            msd->makeType = new TypeDecl(Type::tArray);
            msd->makeType->at = msd->at;
            msd->makeType->firstType = (yyvsp[-5].pTypeDecl);
            msd->at = tokAt(scanner,(yylsp[-5]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 1026: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 1027: /* $@126: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 1028: /* $@127: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 1029: /* make_dim_decl: "fixed_array" '<' $@126 type_declaration_no_options '>' $@127 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 1030: /* expr_map_tuple_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 1031: /* expr_map_tuple_list: expr_map_tuple_list ',' expr  */
                                                      {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 1032: /* push_table_nesting: %empty  */
                    {
        yyextra->das_nested_parentheses ++;
    }
    break;

  case 1033: /* make_table_decl: '{' push_table_nesting optional_emit_semis optional_expr_map_tuple_list '}'  */
                                                                                                     {
        yyextra->das_nested_parentheses --;
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = new TypeDecl(Type::autoinfer);
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto mks = new ExprMakeStruct();
            mks->at = tokAt(scanner,(yylsp[-4]));
            mks->makeType = new TypeDecl(Type::tTable);
            mks->makeType->firstType = new TypeDecl(Type::autoinfer);
            mks->makeType->secondType = new TypeDecl(Type::autoinfer);
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }
    }
    break;

  case 1034: /* make_table_call: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 1035: /* make_table_call: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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
            msd->makeType = new TypeDecl(Type::tTable);
            msd->makeType->at = msd->at;
            msd->makeType->firstType = (yyvsp[-4].pTypeDecl);
            msd->makeType->secondType = new TypeDecl(Type::tVoid);
            msd->makeType->secondType->at = tokAt(scanner,(yylsp[-6]));
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 1036: /* make_table_call: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                                                             {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = new TypeDecl(Type::tTuple);
            mka->makeType->argTypes.push_back((yyvsp[-6].pTypeDecl));
            mka->makeType->argTypes.push_back((yyvsp[-4].pTypeDecl));
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->makeType = new TypeDecl(Type::tTable);
            msd->makeType->at = msd->at;
            msd->makeType->firstType = (yyvsp[-6].pTypeDecl);
            msd->makeType->secondType = (yyvsp[-4].pTypeDecl);
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 1037: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 1038: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 1039: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 1040: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 1041: /* table_comprehension: '[' "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                               {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 1042: /* table_comprehension: '[' "iterator" "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 1043: /* array_comprehension: '{' push_table_nesting optional_emit_semis "for" '(' for_variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where '}'  */
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


