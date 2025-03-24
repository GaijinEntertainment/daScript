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
  YYSYMBOL_DAS_TTUPLE = 102,               /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 103,             /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 104,            /* "generator"  */
  YYSYMBOL_DAS_YIELD = 105,                /* "yield"  */
  YYSYMBOL_DAS_SEALED = 106,               /* "sealed"  */
  YYSYMBOL_DAS_TEMPLATE = 107,             /* "template"  */
  YYSYMBOL_ADDEQU = 108,                   /* "+="  */
  YYSYMBOL_SUBEQU = 109,                   /* "-="  */
  YYSYMBOL_DIVEQU = 110,                   /* "/="  */
  YYSYMBOL_MULEQU = 111,                   /* "*="  */
  YYSYMBOL_MODEQU = 112,                   /* "%="  */
  YYSYMBOL_ANDEQU = 113,                   /* "&="  */
  YYSYMBOL_OREQU = 114,                    /* "|="  */
  YYSYMBOL_XOREQU = 115,                   /* "^="  */
  YYSYMBOL_SHL = 116,                      /* "<<"  */
  YYSYMBOL_SHR = 117,                      /* ">>"  */
  YYSYMBOL_ADDADD = 118,                   /* "++"  */
  YYSYMBOL_SUBSUB = 119,                   /* "--"  */
  YYSYMBOL_LEEQU = 120,                    /* "<="  */
  YYSYMBOL_SHLEQU = 121,                   /* "<<="  */
  YYSYMBOL_SHREQU = 122,                   /* ">>="  */
  YYSYMBOL_GREQU = 123,                    /* ">="  */
  YYSYMBOL_EQUEQU = 124,                   /* "=="  */
  YYSYMBOL_NOTEQU = 125,                   /* "!="  */
  YYSYMBOL_RARROW = 126,                   /* "->"  */
  YYSYMBOL_LARROW = 127,                   /* "<-"  */
  YYSYMBOL_QQ = 128,                       /* "??"  */
  YYSYMBOL_QDOT = 129,                     /* "?."  */
  YYSYMBOL_QBRA = 130,                     /* "?["  */
  YYSYMBOL_LPIPE = 131,                    /* "<|"  */
  YYSYMBOL_RPIPE = 132,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 133,                 /* ":="  */
  YYSYMBOL_ROTL = 134,                     /* "<<<"  */
  YYSYMBOL_ROTR = 135,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 136,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 137,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 138,                    /* "=>"  */
  YYSYMBOL_COLCOL = 139,                   /* "::"  */
  YYSYMBOL_ANDAND = 140,                   /* "&&"  */
  YYSYMBOL_OROR = 141,                     /* "||"  */
  YYSYMBOL_XORXOR = 142,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 143,                /* "&&="  */
  YYSYMBOL_OROREQU = 144,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 145,                /* "^^="  */
  YYSYMBOL_DOTDOT = 146,                   /* ".."  */
  YYSYMBOL_MTAG_E = 147,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 148,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 149,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 150,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 151,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 152,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 153,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 154,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 155,           /* "..."  */
  YYSYMBOL_INTEGER = 156,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 157,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 158,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 159,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 160,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 161,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 162,                   /* "double constant"  */
  YYSYMBOL_NAME = 163,                     /* "name"  */
  YYSYMBOL_DAS_EMIT_COMMA = 164,           /* "new line, comma"  */
  YYSYMBOL_DAS_EMIT_SEMICOLON = 165,       /* "new line, semicolon"  */
  YYSYMBOL_BEGIN_STRING = 166,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 167,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 168,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 169,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 170,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 171,          /* "}"  */
  YYSYMBOL_END_OF_READ = 172,              /* "end of failed eader macro"  */
  YYSYMBOL_173_ = 173,                     /* ','  */
  YYSYMBOL_174_ = 174,                     /* '='  */
  YYSYMBOL_175_ = 175,                     /* '?'  */
  YYSYMBOL_176_ = 176,                     /* ':'  */
  YYSYMBOL_177_ = 177,                     /* '|'  */
  YYSYMBOL_178_ = 178,                     /* '^'  */
  YYSYMBOL_179_ = 179,                     /* '&'  */
  YYSYMBOL_180_ = 180,                     /* '<'  */
  YYSYMBOL_181_ = 181,                     /* '>'  */
  YYSYMBOL_182_ = 182,                     /* '-'  */
  YYSYMBOL_183_ = 183,                     /* '+'  */
  YYSYMBOL_184_ = 184,                     /* '*'  */
  YYSYMBOL_185_ = 185,                     /* '/'  */
  YYSYMBOL_186_ = 186,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 187,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 188,               /* UNARY_PLUS  */
  YYSYMBOL_189_ = 189,                     /* '~'  */
  YYSYMBOL_190_ = 190,                     /* '!'  */
  YYSYMBOL_PRE_INC = 191,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 192,                  /* PRE_DEC  */
  YYSYMBOL_LLPIPE = 193,                   /* LLPIPE  */
  YYSYMBOL_POST_INC = 194,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 195,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 196,                    /* DEREF  */
  YYSYMBOL_197_ = 197,                     /* '.'  */
  YYSYMBOL_198_ = 198,                     /* '['  */
  YYSYMBOL_199_ = 199,                     /* ']'  */
  YYSYMBOL_200_ = 200,                     /* '('  */
  YYSYMBOL_201_ = 201,                     /* ')'  */
  YYSYMBOL_202_ = 202,                     /* '$'  */
  YYSYMBOL_203_ = 203,                     /* '@'  */
  YYSYMBOL_204_ = 204,                     /* ';'  */
  YYSYMBOL_205_ = 205,                     /* '{'  */
  YYSYMBOL_206_ = 206,                     /* '}'  */
  YYSYMBOL_207_ = 207,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 208,                 /* $accept  */
  YYSYMBOL_program = 209,                  /* program  */
  YYSYMBOL_COMMA = 210,                    /* COMMA  */
  YYSYMBOL_SEMICOLON = 211,                /* SEMICOLON  */
  YYSYMBOL_top_level_reader_macro = 212,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 213, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 214,              /* module_name  */
  YYSYMBOL_module_declaration = 215,       /* module_declaration  */
  YYSYMBOL_character_sequence = 216,       /* character_sequence  */
  YYSYMBOL_string_constant = 217,          /* string_constant  */
  YYSYMBOL_format_string = 218,            /* format_string  */
  YYSYMBOL_optional_format_string = 219,   /* optional_format_string  */
  YYSYMBOL_220_1 = 220,                    /* $@1  */
  YYSYMBOL_string_builder_body = 221,      /* string_builder_body  */
  YYSYMBOL_string_builder = 222,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 223, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 224,              /* expr_reader  */
  YYSYMBOL_225_2 = 225,                    /* $@2  */
  YYSYMBOL_options_declaration = 226,      /* options_declaration  */
  YYSYMBOL_require_declaration = 227,      /* require_declaration  */
  YYSYMBOL_require_module_name = 228,      /* require_module_name  */
  YYSYMBOL_require_module = 229,           /* require_module  */
  YYSYMBOL_is_public_module = 230,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 231,       /* expect_declaration  */
  YYSYMBOL_expect_list = 232,              /* expect_list  */
  YYSYMBOL_expect_error = 233,             /* expect_error  */
  YYSYMBOL_expression_label = 234,         /* expression_label  */
  YYSYMBOL_expression_goto = 235,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 236,      /* elif_or_static_elif  */
  YYSYMBOL_emit_semis = 237,               /* emit_semis  */
  YYSYMBOL_optional_emit_semis = 238,      /* optional_emit_semis  */
  YYSYMBOL_expression_else = 239,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 240,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 241, /* expression_else_one_liner  */
  YYSYMBOL_expression_if_one_liner = 242,  /* expression_if_one_liner  */
  YYSYMBOL_semis = 243,                    /* semis  */
  YYSYMBOL_optional_semis = 244,           /* optional_semis  */
  YYSYMBOL_expression_if_block = 245,      /* expression_if_block  */
  YYSYMBOL_246_3 = 246,                    /* $@3  */
  YYSYMBOL_247_4 = 247,                    /* $@4  */
  YYSYMBOL_248_5 = 248,                    /* $@5  */
  YYSYMBOL_expression_if_then_else = 249,  /* expression_if_then_else  */
  YYSYMBOL_250_6 = 250,                    /* $@6  */
  YYSYMBOL_expression_if_then_else_oneliner = 251, /* expression_if_then_else_oneliner  */
  YYSYMBOL_expression_for_loop = 252,      /* expression_for_loop  */
  YYSYMBOL_253_7 = 253,                    /* $@7  */
  YYSYMBOL_expression_unsafe = 254,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 255,    /* expression_while_loop  */
  YYSYMBOL_256_8 = 256,                    /* $@8  */
  YYSYMBOL_expression_with = 257,          /* expression_with  */
  YYSYMBOL_258_9 = 258,                    /* $@9  */
  YYSYMBOL_expression_with_alias = 259,    /* expression_with_alias  */
  YYSYMBOL_annotation_argument_value = 260, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 261, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 262, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 263,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 264, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 265,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 266, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 267, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 268,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 269,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 270, /* optional_annotation_list  */
  YYSYMBOL_optional_annotation_list_with_emit_semis = 271, /* optional_annotation_list_with_emit_semis  */
  YYSYMBOL_optional_function_argument_list = 272, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 273,   /* optional_function_type  */
  YYSYMBOL_function_name = 274,            /* function_name  */
  YYSYMBOL_global_function_declaration = 275, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 276, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 277, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 278,     /* function_declaration  */
  YYSYMBOL_279_10 = 279,                   /* $@10  */
  YYSYMBOL_expression_block_finally = 280, /* expression_block_finally  */
  YYSYMBOL_281_11 = 281,                   /* $@11  */
  YYSYMBOL_282_12 = 282,                   /* $@12  */
  YYSYMBOL_expression_block = 283,         /* expression_block  */
  YYSYMBOL_284_13 = 284,                   /* $@13  */
  YYSYMBOL_285_14 = 285,                   /* $@14  */
  YYSYMBOL_expr_call_pipe = 286,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 287,           /* expression_any  */
  YYSYMBOL_expressions = 288,              /* expressions  */
  YYSYMBOL_optional_expr_list = 289,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_map_tuple_list = 290, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 291, /* type_declaration_no_options_list  */
  YYSYMBOL_name_in_namespace = 292,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 293,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 294,     /* new_type_declaration  */
  YYSYMBOL_295_15 = 295,                   /* $@15  */
  YYSYMBOL_296_16 = 296,                   /* $@16  */
  YYSYMBOL_expr_new = 297,                 /* expr_new  */
  YYSYMBOL_expression_break = 298,         /* expression_break  */
  YYSYMBOL_expression_continue = 299,      /* expression_continue  */
  YYSYMBOL_expression_return = 300,        /* expression_return  */
  YYSYMBOL_expression_yield = 301,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 302,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 303,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 304,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 305,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 306,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 307, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 308,           /* expression_let  */
  YYSYMBOL_expr_cast = 309,                /* expr_cast  */
  YYSYMBOL_310_17 = 310,                   /* $@17  */
  YYSYMBOL_311_18 = 311,                   /* $@18  */
  YYSYMBOL_312_19 = 312,                   /* $@19  */
  YYSYMBOL_313_20 = 313,                   /* $@20  */
  YYSYMBOL_314_21 = 314,                   /* $@21  */
  YYSYMBOL_315_22 = 315,                   /* $@22  */
  YYSYMBOL_expr_type_decl = 316,           /* expr_type_decl  */
  YYSYMBOL_317_23 = 317,                   /* $@23  */
  YYSYMBOL_318_24 = 318,                   /* $@24  */
  YYSYMBOL_expr_type_info = 319,           /* expr_type_info  */
  YYSYMBOL_expr_list = 320,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 321,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 322,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 323,            /* capture_entry  */
  YYSYMBOL_capture_list = 324,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 325,    /* optional_capture_list  */
  YYSYMBOL_expr_full_block = 326,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 327, /* expr_full_block_assumed_piped  */
  YYSYMBOL_expr_numeric_const = 328,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 329,              /* expr_assign  */
  YYSYMBOL_expr_named_call = 330,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 331,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 332,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 333,           /* func_addr_expr  */
  YYSYMBOL_334_25 = 334,                   /* $@25  */
  YYSYMBOL_335_26 = 335,                   /* $@26  */
  YYSYMBOL_336_27 = 336,                   /* $@27  */
  YYSYMBOL_337_28 = 337,                   /* $@28  */
  YYSYMBOL_expr_field = 338,               /* expr_field  */
  YYSYMBOL_339_29 = 339,                   /* $@29  */
  YYSYMBOL_340_30 = 340,                   /* $@30  */
  YYSYMBOL_expr_call = 341,                /* expr_call  */
  YYSYMBOL_expr = 342,                     /* expr  */
  YYSYMBOL_343_31 = 343,                   /* $@31  */
  YYSYMBOL_344_32 = 344,                   /* $@32  */
  YYSYMBOL_345_33 = 345,                   /* $@33  */
  YYSYMBOL_346_34 = 346,                   /* $@34  */
  YYSYMBOL_347_35 = 347,                   /* $@35  */
  YYSYMBOL_348_36 = 348,                   /* $@36  */
  YYSYMBOL_expr_generator = 349,           /* expr_generator  */
  YYSYMBOL_expr_mtag = 350,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 351, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 352,        /* optional_override  */
  YYSYMBOL_optional_constant = 353,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 354, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 355, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 356, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 357, /* struct_variable_declaration_list  */
  YYSYMBOL_358_37 = 358,                   /* $@37  */
  YYSYMBOL_359_38 = 359,                   /* $@38  */
  YYSYMBOL_360_39 = 360,                   /* $@39  */
  YYSYMBOL_function_argument_declaration = 361, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 362,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 363,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 364,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 365,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 366,             /* variant_type  */
  YYSYMBOL_variant_type_list = 367,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 368,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 369,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 370,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 371,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 372,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 373, /* let_variable_name_with_pos_list  */
  YYSYMBOL_global_let_variable_name_with_pos_list = 374, /* global_let_variable_name_with_pos_list  */
  YYSYMBOL_variable_declaration_list = 375, /* variable_declaration_list  */
  YYSYMBOL_let_variable_declaration = 376, /* let_variable_declaration  */
  YYSYMBOL_global_let_variable_declaration = 377, /* global_let_variable_declaration  */
  YYSYMBOL_optional_shared = 378,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 379, /* optional_public_or_private_variable  */
  YYSYMBOL_global_variable_declaration_list = 380, /* global_variable_declaration_list  */
  YYSYMBOL_381_40 = 381,                   /* $@40  */
  YYSYMBOL_global_let = 382,               /* global_let  */
  YYSYMBOL_383_41 = 383,                   /* $@41  */
  YYSYMBOL_enum_expression = 384,          /* enum_expression  */
  YYSYMBOL_commas = 385,                   /* commas  */
  YYSYMBOL_enum_list = 386,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 387, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 388,             /* single_alias  */
  YYSYMBOL_389_42 = 389,                   /* $@42  */
  YYSYMBOL_alias_declaration = 390,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 391, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 392,                /* enum_name  */
  YYSYMBOL_optional_enum_basic_type_declaration = 393, /* optional_enum_basic_type_declaration  */
  YYSYMBOL_optional_commas = 394,          /* optional_commas  */
  YYSYMBOL_emit_commas = 395,              /* emit_commas  */
  YYSYMBOL_optional_emit_commas = 396,     /* optional_emit_commas  */
  YYSYMBOL_enum_declaration = 397,         /* enum_declaration  */
  YYSYMBOL_398_43 = 398,                   /* $@43  */
  YYSYMBOL_399_44 = 399,                   /* $@44  */
  YYSYMBOL_400_45 = 400,                   /* $@45  */
  YYSYMBOL_optional_structure_parent = 401, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 402,          /* optional_sealed  */
  YYSYMBOL_structure_name = 403,           /* structure_name  */
  YYSYMBOL_class_or_struct = 404,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 405, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 406, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 407,    /* structure_declaration  */
  YYSYMBOL_408_46 = 408,                   /* $@46  */
  YYSYMBOL_409_47 = 409,                   /* $@47  */
  YYSYMBOL_410_48 = 410,                   /* $@48  */
  YYSYMBOL_variable_name_with_pos_list = 411, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 412,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 413, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 414, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 415,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 416,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 417,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 418, /* bitfield_type_declaration  */
  YYSYMBOL_419_49 = 419,                   /* $@49  */
  YYSYMBOL_420_50 = 420,                   /* $@50  */
  YYSYMBOL_c_or_s = 421,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 422,          /* table_type_pair  */
  YYSYMBOL_dim_list = 423,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 424, /* type_declaration_no_options  */
  YYSYMBOL_type_declaration_no_options_no_dim = 425, /* type_declaration_no_options_no_dim  */
  YYSYMBOL_426_51 = 426,                   /* $@51  */
  YYSYMBOL_427_52 = 427,                   /* $@52  */
  YYSYMBOL_428_53 = 428,                   /* $@53  */
  YYSYMBOL_429_54 = 429,                   /* $@54  */
  YYSYMBOL_430_55 = 430,                   /* $@55  */
  YYSYMBOL_431_56 = 431,                   /* $@56  */
  YYSYMBOL_432_57 = 432,                   /* $@57  */
  YYSYMBOL_433_58 = 433,                   /* $@58  */
  YYSYMBOL_434_59 = 434,                   /* $@59  */
  YYSYMBOL_435_60 = 435,                   /* $@60  */
  YYSYMBOL_436_61 = 436,                   /* $@61  */
  YYSYMBOL_437_62 = 437,                   /* $@62  */
  YYSYMBOL_438_63 = 438,                   /* $@63  */
  YYSYMBOL_439_64 = 439,                   /* $@64  */
  YYSYMBOL_440_65 = 440,                   /* $@65  */
  YYSYMBOL_441_66 = 441,                   /* $@66  */
  YYSYMBOL_442_67 = 442,                   /* $@67  */
  YYSYMBOL_443_68 = 443,                   /* $@68  */
  YYSYMBOL_444_69 = 444,                   /* $@69  */
  YYSYMBOL_445_70 = 445,                   /* $@70  */
  YYSYMBOL_446_71 = 446,                   /* $@71  */
  YYSYMBOL_447_72 = 447,                   /* $@72  */
  YYSYMBOL_448_73 = 448,                   /* $@73  */
  YYSYMBOL_449_74 = 449,                   /* $@74  */
  YYSYMBOL_450_75 = 450,                   /* $@75  */
  YYSYMBOL_451_76 = 451,                   /* $@76  */
  YYSYMBOL_452_77 = 452,                   /* $@77  */
  YYSYMBOL_type_declaration = 453,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 454,  /* tuple_alias_declaration  */
  YYSYMBOL_455_78 = 455,                   /* $@78  */
  YYSYMBOL_456_79 = 456,                   /* $@79  */
  YYSYMBOL_457_80 = 457,                   /* $@80  */
  YYSYMBOL_458_81 = 458,                   /* $@81  */
  YYSYMBOL_variant_alias_declaration = 459, /* variant_alias_declaration  */
  YYSYMBOL_460_82 = 460,                   /* $@82  */
  YYSYMBOL_461_83 = 461,                   /* $@83  */
  YYSYMBOL_462_84 = 462,                   /* $@84  */
  YYSYMBOL_463_85 = 463,                   /* $@85  */
  YYSYMBOL_bitfield_alias_declaration = 464, /* bitfield_alias_declaration  */
  YYSYMBOL_465_86 = 465,                   /* $@86  */
  YYSYMBOL_466_87 = 466,                   /* $@87  */
  YYSYMBOL_467_88 = 467,                   /* $@88  */
  YYSYMBOL_468_89 = 468,                   /* $@89  */
  YYSYMBOL_make_decl = 469,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 470,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 471,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 472,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim_list = 473,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 474,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 475, /* optional_make_struct_dim_decl  */
  YYSYMBOL_use_initializer = 476,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 477,         /* make_struct_decl  */
  YYSYMBOL_478_90 = 478,                   /* $@90  */
  YYSYMBOL_479_91 = 479,                   /* $@91  */
  YYSYMBOL_480_92 = 480,                   /* $@92  */
  YYSYMBOL_481_93 = 481,                   /* $@93  */
  YYSYMBOL_482_94 = 482,                   /* $@94  */
  YYSYMBOL_483_95 = 483,                   /* $@95  */
  YYSYMBOL_484_96 = 484,                   /* $@96  */
  YYSYMBOL_485_97 = 485,                   /* $@97  */
  YYSYMBOL_make_map_tuple = 486,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 487,          /* make_tuple_call  */
  YYSYMBOL_488_98 = 488,                   /* $@98  */
  YYSYMBOL_489_99 = 489,                   /* $@99  */
  YYSYMBOL_make_dim_decl = 490,            /* make_dim_decl  */
  YYSYMBOL_491_100 = 491,                  /* $@100  */
  YYSYMBOL_492_101 = 492,                  /* $@101  */
  YYSYMBOL_493_102 = 493,                  /* $@102  */
  YYSYMBOL_494_103 = 494,                  /* $@103  */
  YYSYMBOL_495_104 = 495,                  /* $@104  */
  YYSYMBOL_496_105 = 496,                  /* $@105  */
  YYSYMBOL_497_106 = 497,                  /* $@106  */
  YYSYMBOL_498_107 = 498,                  /* $@107  */
  YYSYMBOL_499_108 = 499,                  /* $@108  */
  YYSYMBOL_500_109 = 500,                  /* $@109  */
  YYSYMBOL_expr_map_tuple_list = 501,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 502,          /* make_table_decl  */
  YYSYMBOL_503_110 = 503,                  /* $@110  */
  YYSYMBOL_array_comprehension_where = 504, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 505,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 506       /* array_comprehension  */
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
#define YYLAST   11657

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  208
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  299
/* YYNRULES -- Number of rules.  */
#define YYNRULES  849
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1539

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   435


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
       2,     2,     2,   190,     2,   207,   202,   186,   179,     2,
     200,   201,   184,   183,   173,   182,   197,   185,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   176,   204,
     180,   174,   181,   175,   203,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   198,     2,   199,   178,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   205,   177,   206,   189,     2,     2,     2,
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
     165,   166,   167,   168,   169,   170,   171,   172,   187,   188,
     191,   192,   193,   194,   195,   196
};

#if DAS2_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   554,   554,   555,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,   576,   577,   581,
     582,   586,   592,   593,   594,   598,   599,   603,   621,   622,
     623,   624,   628,   629,   633,   634,   638,   639,   639,   643,
     648,   657,   672,   688,   693,   701,   701,   740,   758,   762,
     765,   769,   775,   784,   787,   793,   794,   798,   802,   803,
     807,   810,   816,   822,   825,   831,   832,   836,   837,   841,
     842,   846,   847,   848,   856,   857,   861,   862,   868,   869,
     870,   871,   872,   876,   877,   881,   882,   886,   888,   886,
     900,   900,   908,   908,   923,   930,   930,   939,   947,   947,
     960,   960,   972,   979,   980,   981,   982,   983,   984,   988,
     993,  1001,  1002,  1003,  1007,  1008,  1009,  1010,  1011,  1012,
    1013,  1014,  1020,  1023,  1029,  1032,  1038,  1039,  1040,  1041,
    1045,  1058,  1076,  1079,  1087,  1098,  1109,  1120,  1123,  1130,
    1134,  1141,  1142,  1146,  1147,  1151,  1152,  1153,  1157,  1160,
    1167,  1171,  1172,  1173,  1174,  1175,  1176,  1177,  1178,  1179,
    1180,  1181,  1182,  1183,  1184,  1185,  1186,  1187,  1188,  1189,
    1190,  1191,  1192,  1193,  1194,  1195,  1196,  1197,  1198,  1199,
    1200,  1201,  1202,  1203,  1204,  1205,  1206,  1207,  1208,  1209,
    1210,  1211,  1212,  1213,  1214,  1215,  1216,  1217,  1218,  1219,
    1220,  1221,  1222,  1223,  1224,  1225,  1226,  1227,  1228,  1229,
    1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,  1238,  1239,
    1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,
    1250,  1251,  1252,  1253,  1254,  1259,  1277,  1278,  1279,  1283,
    1289,  1289,  1306,  1309,  1311,  1309,  1319,  1321,  1319,  1336,
    1349,  1350,  1351,  1352,  1353,  1354,  1355,  1356,  1357,  1358,
    1359,  1360,  1361,  1362,  1363,  1364,  1365,  1366,  1367,  1371,
    1376,  1382,  1388,  1389,  1393,  1394,  1398,  1402,  1409,  1410,
    1421,  1425,  1428,  1436,  1436,  1436,  1439,  1445,  1448,  1452,
    1456,  1463,  1470,  1476,  1480,  1484,  1487,  1490,  1498,  1501,
    1509,  1515,  1516,  1517,  1521,  1522,  1526,  1527,  1531,  1536,
    1544,  1550,  1562,  1565,  1568,  1574,  1574,  1574,  1577,  1577,
    1577,  1582,  1582,  1582,  1590,  1590,  1590,  1596,  1606,  1617,
    1632,  1635,  1641,  1642,  1649,  1660,  1661,  1662,  1666,  1667,
    1668,  1669,  1670,  1674,  1679,  1687,  1688,  1692,  1699,  1703,
    1709,  1710,  1711,  1712,  1713,  1714,  1715,  1719,  1720,  1721,
    1722,  1723,  1724,  1725,  1726,  1727,  1728,  1729,  1730,  1731,
    1732,  1733,  1734,  1735,  1736,  1737,  1741,  1748,  1760,  1765,
    1775,  1779,  1786,  1789,  1789,  1789,  1794,  1794,  1794,  1807,
    1811,  1815,  1820,  1827,  1836,  1841,  1848,  1848,  1848,  1855,
    1859,  1868,  1876,  1884,  1888,  1891,  1897,  1898,  1899,  1900,
    1901,  1902,  1903,  1904,  1905,  1906,  1907,  1908,  1909,  1910,
    1911,  1912,  1913,  1914,  1915,  1916,  1917,  1918,  1919,  1920,
    1921,  1922,  1923,  1924,  1925,  1926,  1927,  1928,  1929,  1930,
    1931,  1932,  1938,  1939,  1940,  1941,  1942,  1955,  1964,  1965,
    1966,  1967,  1968,  1969,  1970,  1971,  1972,  1973,  1974,  1975,
    1976,  1977,  1980,  1980,  1980,  1983,  1988,  1992,  1996,  1996,
    1996,  2001,  2004,  2008,  2008,  2008,  2013,  2016,  2017,  2018,
    2019,  2020,  2021,  2022,  2023,  2024,  2026,  2030,  2031,  2039,
    2042,  2045,  2054,  2055,  2056,  2057,  2058,  2059,  2060,  2064,
    2068,  2072,  2076,  2080,  2084,  2088,  2092,  2096,  2103,  2104,
    2108,  2109,  2110,  2114,  2115,  2119,  2120,  2121,  2125,  2126,
    2130,  2141,  2144,  2145,  2145,  2164,  2163,  2178,  2177,  2194,
    2203,  2213,  2214,  2218,  2221,  2230,  2231,  2235,  2238,  2242,
    2256,  2265,  2266,  2270,  2273,  2277,  2291,  2292,  2296,  2302,
    2308,  2311,  2315,  2324,  2325,  2326,  2330,  2331,  2335,  2342,
    2347,  2356,  2362,  2373,  2380,  2389,  2392,  2395,  2402,  2405,
    2410,  2421,  2424,  2429,  2441,  2442,  2446,  2447,  2448,  2452,
    2455,  2458,  2458,  2478,  2481,  2481,  2499,  2504,  2512,  2513,
    2517,  2520,  2533,  2550,  2551,  2552,  2557,  2557,  2583,  2587,
    2588,  2589,  2593,  2603,  2606,  2612,  2613,  2617,  2618,  2622,
    2623,  2627,  2629,  2634,  2627,  2650,  2651,  2655,  2656,  2660,
    2666,  2667,  2668,  2669,  2673,  2674,  2675,  2679,  2682,  2688,
    2690,  2695,  2688,  2716,  2723,  2728,  2737,  2743,  2754,  2755,
    2756,  2757,  2758,  2759,  2760,  2761,  2762,  2763,  2764,  2765,
    2766,  2767,  2768,  2769,  2770,  2771,  2772,  2773,  2774,  2775,
    2776,  2777,  2778,  2779,  2780,  2784,  2785,  2786,  2787,  2788,
    2789,  2790,  2791,  2795,  2806,  2810,  2817,  2829,  2836,  2845,
    2850,  2860,  2873,  2873,  2873,  2886,  2887,  2891,  2895,  2902,
    2906,  2910,  2914,  2921,  2924,  2942,  2943,  2944,  2945,  2946,
    2946,  2946,  2950,  2955,  2962,  2962,  2969,  2973,  2977,  2982,
    2987,  2992,  2997,  3001,  3005,  3010,  3014,  3018,  3023,  3023,
    3023,  3029,  3036,  3036,  3036,  3041,  3041,  3041,  3047,  3047,
    3047,  3052,  3056,  3056,  3056,  3061,  3061,  3061,  3070,  3074,
    3074,  3074,  3079,  3079,  3079,  3088,  3092,  3092,  3092,  3097,
    3097,  3097,  3106,  3106,  3106,  3112,  3112,  3112,  3121,  3124,
    3135,  3151,  3153,  3158,  3163,  3151,  3189,  3191,  3196,  3202,
    3189,  3228,  3230,  3235,  3240,  3228,  3271,  3272,  3273,  3274,
    3275,  3279,  3286,  3293,  3299,  3305,  3312,  3319,  3325,  3334,
    3337,  3343,  3351,  3356,  3363,  3368,  3374,  3375,  3379,  3380,
    3384,  3384,  3384,  3392,  3392,  3392,  3399,  3399,  3399,  3410,
    3410,  3410,  3421,  3427,  3433,  3439,  3439,  3439,  3453,  3472,
    3472,  3472,  3482,  3482,  3482,  3496,  3496,  3496,  3510,  3519,
    3519,  3519,  3539,  3546,  3546,  3546,  3556,  3559,  3565,  3565,
    3587,  3595,  3615,  3640,  3641,  3645,  3646,  3650,  3653,  3656
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
  "\"int8\"", "\"uint8\"", "\"int16\"", "\"uint16\"", "\"tuple\"",
  "\"variant\"", "\"generator\"", "\"yield\"", "\"sealed\"",
  "\"template\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"",
  "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"",
  "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"",
  "\"<-\"", "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\"|>\"", "\":=\"",
  "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"::\"",
  "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"", "\"^^=\"", "\"..\"",
  "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"", "\"$a\"", "\"$t\"", "\"$c\"",
  "\"$f\"", "\"...\"", "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"unsigned int8 constant\"", "\"floating point constant\"",
  "\"double constant\"", "\"name\"", "\"new line, comma\"",
  "\"new line, semicolon\"", "\"start of the string\"", "STRING_CHARACTER",
  "STRING_CHARACTER_ESC", "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "','", "'='", "'?'", "':'", "'|'",
  "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'",
  "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
  "LLPIPE", "POST_INC", "POST_DEC", "DEREF", "'.'", "'['", "']'", "'('",
  "')'", "'$'", "'@'", "';'", "'{'", "'}'", "'#'", "$accept", "program",
  "COMMA", "SEMICOLON", "top_level_reader_macro",
  "optional_public_or_private_module", "module_name", "module_declaration",
  "character_sequence", "string_constant", "format_string",
  "optional_format_string", "$@1", "string_builder_body", "string_builder",
  "reader_character_sequence", "expr_reader", "$@2", "options_declaration",
  "require_declaration", "require_module_name", "require_module",
  "is_public_module", "expect_declaration", "expect_list", "expect_error",
  "expression_label", "expression_goto", "elif_or_static_elif",
  "emit_semis", "optional_emit_semis", "expression_else",
  "if_or_static_if", "expression_else_one_liner",
  "expression_if_one_liner", "semis", "optional_semis",
  "expression_if_block", "$@3", "$@4", "$@5", "expression_if_then_else",
  "$@6", "expression_if_then_else_oneliner", "expression_for_loop", "$@7",
  "expression_unsafe", "expression_while_loop", "$@8", "expression_with",
  "$@9", "expression_with_alias", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "metadata_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_annotation_list_with_emit_semis",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@10", "expression_block_finally", "$@11",
  "$@12", "expression_block", "$@13", "$@14", "expr_call_pipe",
  "expression_any", "expressions", "optional_expr_list",
  "optional_expr_map_tuple_list", "type_declaration_no_options_list",
  "name_in_namespace", "expression_delete", "new_type_declaration", "$@15",
  "$@16", "expr_new", "expression_break", "expression_continue",
  "expression_return", "expression_yield", "expression_try_catch",
  "kwd_let_var_or_nothing", "kwd_let", "optional_in_scope",
  "tuple_expansion", "tuple_expansion_variable_declaration",
  "expression_let", "expr_cast", "$@17", "$@18", "$@19", "$@20", "$@21",
  "$@22", "expr_type_decl", "$@23", "$@24", "expr_type_info", "expr_list",
  "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_full_block",
  "expr_full_block_assumed_piped", "expr_numeric_const", "expr_assign",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@25", "$@26", "$@27", "$@28", "expr_field", "$@29",
  "$@30", "expr_call", "expr", "$@31", "$@32", "$@33", "$@34", "$@35",
  "$@36", "expr_generator", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@37", "$@38", "$@39",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list",
  "global_let_variable_name_with_pos_list", "variable_declaration_list",
  "let_variable_declaration", "global_let_variable_declaration",
  "optional_shared", "optional_public_or_private_variable",
  "global_variable_declaration_list", "$@40", "global_let", "$@41",
  "enum_expression", "commas", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@42",
  "alias_declaration", "optional_public_or_private_enum", "enum_name",
  "optional_enum_basic_type_declaration", "optional_commas", "emit_commas",
  "optional_emit_commas", "enum_declaration", "$@43", "$@44", "$@45",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@46", "$@47", "$@48", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_alias_bits", "bitfield_type_declaration", "$@49", "$@50",
  "c_or_s", "table_type_pair", "dim_list", "type_declaration_no_options",
  "type_declaration_no_options_no_dim", "$@51", "$@52", "$@53", "$@54",
  "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61", "$@62", "$@63",
  "$@64", "$@65", "$@66", "$@67", "$@68", "$@69", "$@70", "$@71", "$@72",
  "$@73", "$@74", "$@75", "$@76", "$@77", "type_declaration",
  "tuple_alias_declaration", "$@78", "$@79", "$@80", "$@81",
  "variant_alias_declaration", "$@82", "$@83", "$@84", "$@85",
  "bitfield_alias_declaration", "$@86", "$@87", "$@88", "$@89",
  "make_decl", "make_struct_fields", "make_variant_dim",
  "make_struct_single", "make_struct_dim_list", "make_struct_dim_decl",
  "optional_make_struct_dim_decl", "use_initializer", "make_struct_decl",
  "$@90", "$@91", "$@92", "$@93", "$@94", "$@95", "$@96", "$@97",
  "make_map_tuple", "make_tuple_call", "$@98", "$@99", "make_dim_decl",
  "$@100", "$@101", "$@102", "$@103", "$@104", "$@105", "$@106", "$@107",
  "$@108", "$@109", "expr_map_tuple_list", "make_table_decl", "$@110",
  "array_comprehension_where", "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1401)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-750)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1401,    37, -1401, -1401,    46,   -70,   142,   236, -1401,    14,
   -1401, -1401, -1401, -1401,    21,   270, -1401, -1401, -1401, -1401,
      60,    60,    60, -1401,   151, -1401,    16, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401, -1401,    -3, -1401,   -12,
      12,    17, -1401, -1401,   142,    42, -1401, -1401, -1401,   117,
      60, -1401, -1401,    16,   236,   236,   236,   172,   206, -1401,
   -1401, -1401, -1401,   270,   270,   270,   166, -1401,   702,   -72,
   -1401, -1401, -1401, -1401,   386, -1401,    50, -1401,   537,    91,
      46,   305,   -70,   191,   284, -1401,   335,   382, -1401, -1401,
   -1401,   565,   390,   433,   435, -1401,   451,   316, -1401, -1401,
     -81,    46,   270,   270,   270,   270,   482, -1401, -1401, -1401,
   -1401,   614, -1401, -1401,   728,   661, -1401, -1401,   444, -1401,
   -1401, -1401, -1401, -1401,   634,    97, -1401, -1401, -1401, -1401,
     602, -1401, -1401,   494, -1401, -1401, -1401,   502,   482,   482,
   -1401, -1401,   508, -1401,    89, -1401,   -46,   543,   702, -1401,
     523, -1401, 10427, -1401, -1401,   548, -1401, -1401, -1401, -1401,
   -1401, -1401,   520, -1401, -1401, -1401,   679, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401,   137, -1401,  7164, -1401,   531, -1401,
   -1401, -1401, -1401, -1401, -1401, 10539, -1401, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401,
   -1401,   690,   705, -1401,   547,   482, -1401,   582,   649,    64,
      46,   561,   609, -1401, -1401, -1401,    97, -1401,   600,   612,
     618,   574,   630,   635, -1401, -1401, -1401,   626, -1401, -1401,
   -1401, -1401, -1401,   645, -1401, -1401, -1401, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401,   648, -1401, -1401, -1401,   651,
     653, -1401, -1401, -1401, -1401,   674,   680,   664,    21, -1401,
   -1401, -1401, -1401, -1401,  3579,   637,   628, -1401,   577,   658,
     660,   703,   704, -1401, -1401, -1401, -1401, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401,
   -1401, -1401, -1401,   706,   671, -1401, -1401, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401, -1401,   850, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401,
     710,   675, -1401, -1401,   -75,   700, -1401,   720,   502, -1401,
     718,   482, -1401, -1401,   520,   482,    46, -1401,   448, -1401,
   -1401, -1401, -1401, -1401,  6638, -1401, -1401,   719, -1401,   365,
     402,   416, -1401, -1401,  6638,   -27, -1401, -1401, -1401,     6,
   -1401, -1401, -1401,    24, -1401,  3773,   685,  6975, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401,   722,   753, -1401,   687, -1401,
     147, -1401,   428,  7164, -1401, -1401,   683, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401,   684,   715, -1401,    88,
   -1401,   482,   729,  7164, -1401,    47,  7164,  7164,  7164,   723,
     724, -1401, -1401,    98,    21,   725,    30, -1401,   207,   707,
     726,   731,   708,   733,   714,   232,   738, -1401,   370,   741,
     747,  6638,  6638,   734,   736,   740,   742,   745,   746, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401,  6638,  6638,
    6638,  6638,  6638,  3391,  3964, -1401,   699,   928, -1401, -1401,
   -1401,   748, -1401, -1401, -1401, -1401,   749, -1401, -1401, -1401,
   -1401, -1401, -1401,   585,  1310, -1401, -1401,   750, -1401, -1401,
   -1401, -1401, -1401, -1401,  7164,  7164,   752,   786,  7164,   547,
    7164,   547,  7164,   547,  7257,   788,  1738, -1401,  6638, -1401,
   -1401, -1401, -1401,   755, -1401, -1401, 10019,  4155, -1401,  3579,
     789,  7257,   788, -1401, -1401,  6638, -1401, -1401,   291, -1401,
     -78,   628, -1401, -1401,    21, -1401,   308,   759,   917,   500,
   -1401, -1401, -1401,   248, -1401, -1401, -1401,  6638,    61,   269,
     780,   271, -1401, -1401, -1401,   762, -1401, -1401,   410, -1401,
     785,   787,   795, -1401,  6638,  7164,  6638,  6638, -1401, -1401,
    6638, -1401,  6638, -1401,  6638, -1401, -1401,  6638, -1401,  7164,
     456,   456,  6638,  6638,  6638,  6638,  6638,  6638,   670,   456,
     456,   -82,   456,   456,   768,   956,   772,   804, 10871,   778,
      43,   804,   806,   783,   347,   781,   482,  3009,   270,   981,
     790, -1401,   749, -1401,  7424, 10694,  6638,  6638, -1401, -1401,
    6638,  6638,  6638,  6638,   825,  6638,   215,  6638,  6638,  6638,
    6638,  6638,  6638,  6638,  6638,  6638,  4346,  6638,  6638,  6638,
    6638,  6638,  6638,  6638,  6638,  6638,  6638, 11459,  6638, -1401,
    4537,   471,   529, -1401, -1401,   204,   532,   700,   550,   700,
     555,   700,    80, -1401,   303,   628,   813, -1401,   352, -1401,
    7164,   796, -1401, -1401, -1401, 10103, -1401,   332, -1401,    60,
   -1401,    60,  7252,   799,   947, -1401,   -55, -1401,  1998,   828,
   -1401, -1401, -1401, -1401,  6638,   833,   837,  7164,    47, -1401,
    6638,   902, -1401, -1401, -1401, -1401, -1401, -1401, -1401,  7164,
    7164,  7164,  7164,  3582,   838,  6638,  7164, -1401, -1401, -1401,
    7164,   804,   791, 10622, -1401,   836,  7352,  7164,  7164,  7439,
    7164,  7525,  7164,   804,  7164,  7257,   804,   788,   398,  7611,
    7697,  7783,  7869,  7955,  8041, -1401,  6638,   655,   291,   810,
   -1401,  6638, -1401,  6638, -1401,  6638, -1401,  6638,   811,   357,
   -1401,   814,   815,   442, -1401, -1401,   291,  6638,    32,  3200,
   -1401,   146,   812,   293,   817,   547, -1401,  2204,   981,   831,
     823, -1401, -1401,   844,   835, -1401, -1401,  1209,  1209,  1410,
    1410, 11331, 11331,   839,   157,   840, -1401, 10187,   -47,   -47,
     750,  1209,  1209,  1527, 11069, 11101, 10954, 10860, 10705, 11184,
   11216, 11299,  1410,  1410,   507,   507,   157,   157,   157,   397,
    6638,   841,   845,   409,  6638,  1037,   846, 10271, -1401,   150,
   -1401, -1401, -1401,   884, -1401,   868, -1401,   869, -1401,   870,
    7164, -1401,  7257,  7164, -1401,   788,   367,   628, -1401, -1401,
   -1401,   499, -1401, -1401,  7068, -1401,   -95, -1401, -1401,  6638,
     889,   890,  7164, -1401,  6638, -1401,  4728, -1401, -1401,  4919,
     891, -1401,    60,   899,  5110,  -102,  5301, -1401,    60,    60,
    1046, -1401,   524, -1401, -1401,  1045, -1401, -1401,  1050, -1401,
    1018,    60,   855, -1401,    60,    60,    60,    60,    60, -1401,
     997, -1401,    60,  1653,   894, -1401,   332,    28,  8127, -1401,
    1021,   248,  6638,   902, -1401, -1401, -1401, -1401,   628,   584,
     625,   573,   492,   159,   864,   871,   403,  8213,   594,  7164,
    7257,   788,   751,   872,   874,  7164,  6638,  6638,   875, -1401,
     756,   801, -1401,  1175, -1401,  1192,   877,  1490,   439,   888,
     564,   981, -1401, -1401, -1401, -1401, -1401,   893, 10788,    23,
     291, 10871,  8299, 10871, 10871, -1401,   895,   138,  6638,  6638,
    7164,   547,    25,   863,   836,   140, -1401,   896,   320,  6829,
   -1401, -1401, -1401,   139,   700, -1401,   547, -1401,  6638, -1401,
    6638,  5492,  6638, -1401,   910,   898, -1401, -1401,  6638,   901,
   -1401, 10355,  6638,  5683,   904, -1401, 10439, -1401,  5874, -1401,
    6638, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401,
   -1401, -1401,   628, -1401, -1401,   628, -1401, -1401,   905,  7164,
   -1401, -1401,   865, -1401, -1401,   885, -1401,   903,  8385, -1401,
    1032,   -39, 10871,  6638, 10871,  1080,  6638, 10871,   922, -1401,
     926,   954, 10871, -1401,  6638, 10871, -1401, -1401,   913, -1401,
   -1401,   914,   916,   918,   919, -1401,  1083, -1401, -1401, -1401,
   -1401, -1401, -1401,   -53, -1401,  6638,  6638,  6638,  6638,  6638,
    6638,  6638,  6638,  6638,  6638,  6638,  6638,  6638,  6638,  6638,
    6638,  6638,  6638,  6638,   501, -1401, -1401, -1401,  1113,   520,
   -1401,   959, -1401,  6638,   902, -1401, -1401, -1401, -1401,   923,
   -1401, -1401, -1401,   933,   960, -1401, -1401,  1540,   579,   605,
   -1401, -1401,  6638,  1781, 10871, -1401, -1401, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401,   -19,  6065, -1401,   955,
    6638,    26,   197,  6638,  6638,  6638,  8471,  8557,  2751,   700,
    6638, -1401, -1401, -1401,   934,  3200,   971,   973,   937,   975,
     976, -1401,   161,   482,   700,  7164,  8643,  7164,  8729, -1401,
     185,  8815, -1401,  6638, 10986,  6638, -1401,  8901,  3200, -1401,
     194,  6638, -1401, -1401, -1401,   216, -1401, -1401, -1401,  6638,
     628, -1401, -1401, -1401, -1401,   977,  6638, 10871, -1401, 10871,
    6638, -1401, -1401, -1401, 10871,  6638,  6638,   291,  6638,  6638,
   -1401, -1401,   978, -1401, -1401, -1401, 10871, 10871, 10871, 10871,
   10871, 10871, 10871, 10871, 10871, 10871, 10871, 10871, 10871, 10871,
   10871, 10871, 10871, 10871, 10871, -1401,   936,   663,  1076,    60,
   -1401,   902, -1401,   944,   945, -1401, -1401,  6638,   965, -1401,
   -1401, -1401, -1401,   948,   946,   949,  6638,  6638,  6638,   950,
    1081,   953,   958,  6256, -1401, -1401,   217, -1401, -1401,   218,
    6638,  6638,  6638,  8987, 10871, 10871, -1401, -1401, -1401,   974,
     223, -1401,   345, -1401, -1401,   993, -1401, -1401,   139, -1401,
    1022,   482,  3388, -1401,   596, -1401, -1401, -1401,  7164,  9073,
    9159, -1401,   350, -1401,  9245, -1401,   962, -1401, 10871, -1401,
   10871,  9331,  9417,    33,  9503,  9589,   964, -1401,   225,   -63,
   -1401, -1401, -1401,   546, -1401,     8, -1401, -1401,  1081,  1081,
    9675,   992,   994,   995,   996,  6638, -1401,  6638,  1410,  1410,
    1410,  6638, -1401, -1401,  1081,  1081, -1401,  9761, -1401, -1401,
    1004,  1003,   256, 10871, 10871,   210,   351, -1401,  1009,   963,
     998, -1401,  6447, -1401, -1401, -1401, -1401, -1401,   604, -1401,
   -1401,   999, -1401, -1401,  1147,   482,  6638,   482,   482, -1401,
    1035,   617, -1401, -1401, -1401,  1168,     8, -1401, -1401,   663,
     -32,   -32, -1401,  6638,  1081,  1081,   492,  1023,  1028,   804,
     -32,   492, -1401, -1401,  6638,  1015,  6638,  6638, -1401,   351,
    6638, -1401, -1401,  6638, 10954, -1401, -1401, -1401, -1401, -1401,
    2818,    60,  1017,   264, -1401, -1401,  2410, -1401,  7164,    47,
   -1401, -1401,  1168,   291,   492,   806,  1061, -1401,  1036,  1041,
    9847,   -32,   -32,   806,  1047, -1401, -1401,  1048,  1049,  1053,
    1086,  6638, 10871, 10871, -1401,  1019, 10986, -1401, -1401, -1401,
   -1401, -1401, -1401, 10871, -1401,   607,  1042,  2818,   482, -1401,
   -1401,  1040,   246,  6638, 10427, -1401, -1401,   272,  1051, -1401,
   -1401, -1401, -1401,  1055,  1056, -1401, -1401, -1401, -1401,  1205,
    1060,  1086,  1067,   482, -1401, -1401,  1062, -1401, -1401,    60,
   -1401, -1401,  6638,   902,    60, 10427, -1401,   492, -1401, -1401,
    6638, -1401,  1075, -1401, -1401,  6638,  2616, -1401, -1401,   902,
   -1401, -1401,   482,   278, 10871, -1401, -1401,  9933,  1069, -1401,
   -1401, -1401,   482,  1083, -1401, -1401, -1401,   607, -1401
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   143,     1,   304,     0,     0,     0,   593,   305,     0,
     771,   761,   766,    20,     0,     0,    19,    16,    15,     3,
       0,     0,     0,     8,   629,     7,   574,     6,    11,     5,
       4,    13,    12,    14,   112,   113,   111,   120,   122,    47,
      60,    57,    58,    49,     0,    55,    48,   595,   594,     0,
       0,    26,    25,   574,   593,   593,   593,     0,   278,    45,
     127,   128,   129,     0,     0,     0,   130,   132,   139,     0,
     126,    21,    10,     9,   236,   611,     0,   575,   576,     0,
       0,     0,     0,    50,     0,    56,     0,     0,    53,   596,
     598,    22,     0,     0,     0,   280,     0,     0,   138,   133,
       0,     0,     0,     0,     0,     0,    69,   238,   237,   240,
     235,   599,   621,   620,     0,   624,   578,   577,   584,   118,
     119,   116,   117,   115,     0,     0,   114,   123,    61,    59,
      55,    52,    51,     0,    23,    24,    27,   609,    69,    69,
     279,    43,    46,   137,     0,   134,   135,   136,   140,    67,
      70,   144,     0,   601,   600,     0,   623,   622,   626,   625,
     630,   579,   508,    28,    29,    33,     0,   107,   108,   105,
     106,   104,   103,   109,     0,    54,     0,   607,   610,   772,
     762,   767,    44,   131,    68,     0,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,     0,     0,   150,   145,    69,   602,   603,   617,   581,
       0,   509,     0,    30,    31,    32,     0,   121,     0,     0,
       0,     0,     0,     0,   638,   658,   639,   674,   640,   644,
     645,   646,   647,   664,   651,   652,   653,   654,   655,   656,
     657,   659,   660,   661,   662,   731,   643,   650,   663,   738,
     745,   641,   648,   642,   649,     0,     0,     0,     0,   673,
     695,   698,   696,   697,   758,   693,   597,   608,     0,     0,
       0,   204,   205,   202,   153,   154,   156,   155,   157,   158,
     159,   160,   186,   187,   184,   185,   177,   188,   189,   178,
     175,   176,   203,   197,     0,   201,   190,   191,   192,   193,
     164,   165,   166,   161,   162,   163,   174,     0,   180,   181,
     179,   172,   173,   168,   167,   169,   170,   171,   152,   151,
     196,     0,   182,   183,   508,   148,   246,     0,   609,   618,
       0,    69,   583,   580,   508,    69,     0,   563,   556,   585,
     110,   699,   722,   725,     0,   728,   718,     0,   682,   732,
     739,   746,   752,   755,     0,     0,   708,   713,   707,     0,
     721,   717,   710,     0,   712,     0,   694,     0,   773,   763,
     768,   206,   207,   200,   195,   208,   198,   194,     0,   146,
     303,   531,     0,     0,   239,   241,     0,   665,   668,   671,
     672,   666,   669,   667,   670,   604,     0,   615,   631,     0,
     124,    69,     0,     0,   557,     0,     0,     0,     0,     0,
       0,   412,   413,     0,     0,     0,     0,   406,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   664,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   497,
     350,   352,   351,   353,   354,   355,   356,    39,     0,     0,
       0,     0,     0,   272,     0,   335,   336,   838,   410,   409,
     487,   407,   480,   479,   478,   477,   141,   483,   408,   482,
     481,   454,   414,   455,     0,   459,   415,     0,   411,   776,
     780,   777,   778,   779,     0,     0,     0,     0,     0,   145,
       0,   145,     0,   145,     0,     0,     0,   704,   272,   715,
     716,   709,   711,     0,   714,   690,     0,     0,   760,   759,
     679,   537,   543,   209,   199,     0,   301,   302,     0,   147,
     508,   149,   269,   612,     0,   619,     0,     0,   558,   556,
     582,   125,   564,     0,   554,   555,   553,     0,     0,     0,
       0,   687,   800,   803,   283,   287,   286,   292,     0,   324,
       0,     0,     0,   829,     0,     0,     0,     0,   315,   318,
       0,   321,     0,   833,     0,   809,   815,     0,   806,     0,
     442,   443,     0,     0,     0,     0,     0,     0,     0,   419,
     418,   456,   417,   416,     0,     0,     0,   845,   330,     0,
     278,   845,   791,     0,   337,     0,    69,     0,     0,   345,
     336,   269,   141,   249,     0,     0,     0,     0,   444,   445,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   396,     0,   702,
       0,     0,     0,   675,   677,     0,     0,   148,     0,   148,
       0,   148,   278,   535,     0,   533,     0,   541,     0,   676,
       0,     0,   706,   689,   692,     0,   680,   605,   538,    85,
     544,    85,     0,     0,   633,   529,   548,   532,     0,   590,
     616,   627,   521,   632,     0,     0,     0,     0,     0,   571,
       0,     0,   700,   723,   726,    18,    17,   685,   686,     0,
       0,     0,     0,   798,     0,     0,     0,   819,   822,   825,
       0,   845,     0,   813,   836,   845,     0,     0,     0,     0,
       0,     0,     0,   845,     0,     0,   845,     0,     0,     0,
       0,     0,     0,     0,     0,    42,     0,    40,     0,     0,
     818,   846,   273,     0,   547,     0,   546,     0,     0,     0,
     447,     0,     0,   383,   380,   382,     0,   274,     0,   272,
     399,     0,     0,     0,     0,   145,   337,     0,   345,     0,
       0,   466,   465,     0,     0,   467,   471,   420,   421,   433,
     434,   431,   432,     0,   460,     0,   452,     0,   484,   485,
     486,   422,   423,   438,   439,   440,   441,     0,     0,   436,
     437,   435,   429,   430,   425,   424,   426,   427,   428,     0,
       0,     0,   389,     0,     0,     0,     0,     0,   404,     0,
     729,   719,   683,     0,   733,     0,   740,     0,   747,     0,
       0,   753,     0,     0,   756,     0,     0,   276,   703,   691,
     588,   606,   774,    83,    86,   764,    86,   769,   530,     0,
       0,     0,     0,   549,     0,   271,   295,   293,   246,     0,
       0,   294,     0,     0,     0,    69,     0,   250,     0,     0,
       0,   263,     0,   264,   258,     0,   255,   254,     0,   256,
       0,     0,     0,   270,     0,    81,    82,    79,    80,   265,
     307,   253,     0,   357,   586,   591,   605,   523,     0,   560,
     561,     0,     0,     0,   573,   701,   724,   727,   688,     0,
       0,     0,   799,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   846,     0,   457,
       0,     0,   458,     0,   488,     0,     0,     0,     0,     0,
       0,   345,   492,   493,   494,   495,   496,     0,    36,     0,
       0,   331,     0,   782,   781,   446,     0,     0,     0,     0,
       0,   145,     0,     0,   845,     0,   400,     0,     0,     0,
     403,   401,   142,     0,   148,   349,   145,   462,     0,   468,
       0,     0,     0,   450,     0,     0,   472,   476,     0,     0,
     453,     0,     0,     0,     0,   390,     0,   397,     0,   448,
       0,   405,   730,   720,   684,   678,   734,   736,   741,   743,
     748,   750,   534,   754,   536,   540,   757,   542,     0,     0,
     681,   589,     0,    84,   539,     0,   545,     0,     0,   635,
     636,   550,   552,     0,   296,     0,     0,   281,     0,   268,
       0,     0,    64,   246,     0,   298,   266,   267,     0,    74,
      75,     0,     0,     0,     0,   257,   242,   252,   259,   260,
     261,   262,   306,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   606,   613,   522,   628,     0,   508,
     559,     0,   568,     0,     0,   572,   801,   804,   284,     0,
     289,   290,   288,     0,     0,   327,   325,     0,     0,     0,
     830,   828,   274,     0,   812,   837,   840,   316,   319,   322,
     834,   832,   810,   816,   814,   807,    69,     0,    37,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   148,
       0,   839,   275,   402,     0,   272,     0,     0,     0,     0,
       0,   343,     0,    69,   148,     0,     0,     0,     0,   378,
       0,     0,   473,     0,   461,     0,   451,     0,   272,   391,
       0,     0,   449,   398,   394,     0,   737,   744,   751,   272,
     277,   775,   765,   770,   634,     0,     0,   297,   246,   282,
       0,    62,    63,    97,   299,     0,     0,     0,     0,     0,
     243,   248,     0,   565,   313,   312,   367,   368,   370,   369,
     371,   361,   362,   363,   372,   373,   359,   360,   374,   375,
     364,   365,   366,   358,   587,   592,     0,   515,   518,     0,
     562,     0,   570,     0,     0,   285,   291,     0,     0,   326,
     820,   823,   826,     0,     0,     0,     0,     0,     0,     0,
     798,     0,     0,     0,   246,   498,     0,    34,    41,     0,
       0,     0,     0,     0,   784,   783,   381,   507,   384,     0,
       0,   376,     0,   340,   341,     0,   339,   338,     0,   346,
     246,    69,     0,   506,     0,   504,   379,   501,     0,     0,
       0,   500,     0,   392,     0,   395,     0,   637,   551,   300,
     102,     0,     0,     0,     0,     0,     0,   308,     0,     0,
     614,   516,   517,   518,   519,   510,   524,   569,   798,   798,
       0,     0,     0,     0,     0,   272,   841,   274,   317,   320,
     323,     0,   799,   811,   798,   798,   489,     0,   491,   499,
      38,     0,     0,   786,   785,     0,     0,   387,     0,     0,
       0,   344,     0,   332,   347,   246,   463,   469,     0,   505,
     503,     0,   502,   705,    76,    69,     0,    69,    69,   269,
       0,   556,   314,   566,   567,   513,   510,   511,   512,   515,
     797,   797,   328,     0,   798,   798,   789,     0,     0,   845,
     797,   789,   490,    35,     0,     0,     0,     0,   385,     0,
       0,   377,   342,     0,   333,   348,   464,   470,   474,   393,
       0,     0,    90,     0,   246,   246,     0,   309,     0,     0,
     514,   525,   513,     0,     0,   794,   845,   796,     0,     0,
       0,   797,   797,   790,     0,   831,   842,     0,     0,     0,
     843,     0,   788,   787,   388,   843,   334,   475,    77,    81,
      82,    79,    80,    78,    94,    71,     0,     0,    69,    99,
     101,     0,     0,     0,     0,   527,   520,     0,   846,   795,
     802,   805,   329,     0,     0,   827,   835,   817,   808,     0,
       0,   843,     0,    69,    65,    66,     0,    93,   269,     0,
     246,   245,     0,     0,     0,     0,   792,     0,   821,   824,
       0,   847,     0,   849,   246,     0,     0,    91,    96,     0,
     311,   526,    69,     0,   844,   848,    72,     0,     0,   310,
     246,   793,    69,   242,   528,   246,    89,    71,    73
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1401, -1401,  -660,    -1, -1401, -1401, -1401, -1401,   688,  1198,
   -1401, -1401, -1401, -1401, -1401, -1401,  1277, -1401, -1401, -1401,
    1235, -1401,  1150, -1401, -1401,  1199, -1401, -1401, -1401, -1401,
    -134,  -255, -1401, -1401, -1323,   606,   608, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401,
   -1401, -1401,  1066, -1401, -1401,   -64,  1185, -1401, -1401, -1401,
     399,   686,   676,   388,  -477,  -634, -1401, -1401, -1401, -1400,
   -1401, -1401,  -240, -1401, -1401,  -784, -1401, -1401, -1401, -1401,
    -600,  -499, -1088, -1401,   -13, -1401, -1401, -1401, -1401, -1401,
   -1312, -1284, -1125, -1123, -1401, -1401,  1296, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401,
   -1401, -1401,  -402, -1401,   818,    18, -1401,  -738, -1401, -1401,
   -1401, -1401, -1401, -1401, -1298, -1401, -1401, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401,   545, -1401, -1401, -1401, -1401, -1401,
   -1401, -1401, -1401,  -155,   -79,  -128,   -84,   -15, -1401, -1401,
   -1401, -1401, -1401,   779, -1401,  -489,  -702, -1401,  -504,  -699,
   -1401,  -673,  -123,  -529,  -531, -1401, -1401, -1401, -1045, -1401,
    1258, -1401, -1401, -1401, -1401, -1401,   219,   408, -1401,   803,
   -1401, -1401, -1401, -1401, -1401, -1401,   411, -1401,   983, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401,
   -1401, -1401, -1401,  -736,  -173, -1401,   892, -1401, -1401, -1401,
   -1401, -1401, -1401,  -510, -1401, -1401,  -371, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401,  -101, -1401, -1401, -1401, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401,
   -1401,   900,  -740,   -77,  -576, -1401, -1401, -1141, -1180, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401,  -922, -1401,
   -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401, -1401,
   -1401, -1401, -1401,   760, -1401, -1401, -1171,  -591, -1401
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   707,   708,    18,   136,    53,    19,   166,   172,
    1350,  1139,  1267,   588,   468,   142,   469,    97,    21,    22,
      45,    46,    88,    23,    41,    42,   878,   879,  1496,   150,
     151,  1497,  1061,  1421,   880,   854,   855,  1465,  1466,  1528,
    1467,   881,   882,   883,   884,   885,   886,   887,   888,   889,
     890,   891,   173,   174,    37,    38,    39,   221,    66,    67,
      68,    69,   609,    24,   335,   394,   214,    25,   109,   215,
     110,   152,  1211,  1316,  1471,   395,   396,   892,   470,   893,
     688,   596,   973,   846,   471,   894,   555,   712,  1245,   472,
     895,   896,   897,   898,   899,   528,   900,  1073,  1318,  1214,
     901,   473,   727,  1256,   728,  1257,   730,  1258,   474,   716,
    1249,   475,   597,  1364,   476,  1161,  1162,   775,   477,   613,
     478,   902,   479,   480,   765,   481,   970,  1356,   971,  1409,
     482,   825,  1183,   483,   598,  1165,  1416,  1167,  1417,  1298,
    1457,   485,   486,   390,  1389,  1431,  1323,  1325,  1239,   907,
    1099,  1474,  1505,   391,   392,   663,   664,   679,   667,   668,
     681,   757,   685,   547,   415,   539,   348,  1319,   540,   349,
      78,   118,   219,   344,    27,   162,   905,   851,   906,    49,
      50,   133,    28,   155,   217,   338,   852,   178,   179,    29,
     111,   689,  1236,   535,   340,   341,   115,   160,   693,    30,
      76,   218,   536,   686,   487,   405,   271,   272,   655,   677,
     273,   497,  1014,   842,   550,   376,   274,   275,   416,   915,
     670,   495,  1013,   417,   916,   418,   917,   494,  1012,   498,
    1016,   499,  1186,   500,  1018,   501,  1187,   502,  1020,   503,
    1188,   504,  1023,   505,  1026,   665,    31,    55,   279,   521,
    1035,    32,    56,   280,   522,  1037,    33,    54,   278,   520,
    1032,   488,   602,  1444,   603,  1436,  1437,  1438,   925,   489,
     710,  1243,   711,  1244,   737,  1262,   734,  1260,   724,   490,
     735,  1261,   491,   929,  1332,   930,  1333,   931,  1334,   720,
    1253,   732,  1259,   974,   492,   606,  1490,   752,   493
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,    59,    70,   270,   180,   181,   519,   222,   698,   671,
     758,   777,   959,   864,   700,  1125,   127,   850,   680,    71,
      72,    73,   657,   835,   659,   837,   661,   839,  1215,   978,
     972,   772,   678,   948,  1254,   560,  -143,     2,   950,   509,
     986,   709,   543,  1140,     3,  1150,  1270,   626,   627,    90,
      70,    70,    70,  1376,  1387,   112,   113,   511,  1408,   102,
     103,   104,   601,   149,    84,    34,    35,     4,   666,     5,
      13,     6,   754,   388,  1504,   276,   388,     7,    77,   624,
    1343,   336,   626,   627,  1045,   537,    40,     8,   754,    70,
      70,    70,    70,     9,   102,   537,   104,  1458,   572,    85,
     538,   105,    13,   419,   420,  1522,   119,   120,  1459,    16,
     538,  1454,   167,   168,  1388,   647,   648,    10,   861,   756,
     143,   862,   599,   426,   863,   220,   389,   106,   220,   428,
     933,   975,   561,   562,   938,   756,  1460,   924,   377,    11,
      12,    16,   946,  1382,  1499,   949,   149,  1212,  1390,  1391,
     647,   648,  1213,   507,   526,  1459,   345,   114,   845,    74,
      57,    80,   721,   269,  1400,  1401,   435,   436,  1434,   912,
     754,    79,   733,   508,   544,   736,   755,    51,   614,   615,
     545,  1263,    96,  1460,    58,   510,   599,    75,    81,   409,
      82,  1031,   977,  1096,   722,   975,   861,   527,   861,   861,
     438,   439,    13,   512,   270,   771,   861,   408,   738,    36,
     563,   410,   935,  1136,  1441,  1442,    52,   756,   343,    96,
     270,   546,   513,    14,  1141,    13,    15,    86,  1118,    13,
     564,   514,  1119,   976,  1097,    15,   537,    57,   377,    87,
     270,    16,   702,   270,   270,   270,   850,   121,   829,  1398,
    1439,   538,   122,   169,   123,   365,   840,   124,   170,  1448,
     171,    58,    80,   124,    16,   754,  1156,   754,    16,  1203,
     342,  1144,  1157,   755,  1384,   618,   619,   541,   554,   366,
      89,   366,   411,   624,  1492,   625,   626,   627,   628,   629,
     183,   125,   531,    47,  1145,  1461,   463,  1462,   984,    48,
    1483,  1484,  1158,   467,    60,    43,   367,   368,   367,   368,
     226,   923,   756,  1159,   756,   548,   549,   551,  1160,   979,
    1512,   270,   270,  1010,   754,   270,   911,   270,    44,   270,
    1271,   270,  1010,    61,  1288,    95,  1029,   754,   227,   919,
     920,  1027,  1461,  1406,  1462,    96,  1109,   980,   270,   932,
    1163,  1011,  1036,  1024,   647,   648,   940,   941,  1010,   943,
    1110,   945,  1289,   947,   269,  1034,   101,  1010,  1196,   795,
     369,   756,   369,   544,   370,   544,   370,    62,   796,   545,
     269,   545,  1103,  1152,   756,   832,  1296,   565,    87,  1010,
    1010,  1010,   270,   651,   652,  1303,  1010,   656,  1380,   658,
     269,   660,    63,   269,   269,   269,   270,   566,   833,    57,
     269,   558,   573,    13,  1309,  1282,  1114,  1305,  1349,  1351,
     546,   371,   546,   371,  1358,   372,  1381,   372,   373,  1010,
     373,   366,   574,    58,  1031,   705,    13,  1010,  1302,   683,
     845,   782,   786,   107,   706,   759,   377,   130,   377,   108,
     703,   759,    16,   374,   684,   374,   800,  1405,   367,   368,
      64,   128,    98,    99,   100,  1468,   105,   705,    13,  1272,
      65,  1313,   767,  1506,   826,    16,   706,   614,   615,  1531,
    1348,   269,   269,   141,   841,   269,    57,   269,  1455,   269,
      57,   269,   982,   759,  1149,   761,   705,   270,   131,   761,
     762,   145,   146,   147,   148,   706,  1363,    16,   269,  1164,
      58,   966,   691,   692,    58,  1279,   705,    13,   759,  1154,
     967,   690,   369,   759,   270,   706,   370,   763,   614,   615,
    1291,   705,    13,   844,  1059,  1060,   270,   270,   270,   270,
     706,  -735,   699,   270,  1359,   132,  -735,   270,  1028,  1371,
     576,   999,   269,   137,   270,   270,    16,   270,  1117,   270,
    1000,   270,   270,  1004,  1123,  -735,   269,   705,    13,   847,
     577,    16,  1005,   371,   618,   619,   706,   372,  -742,   951,
     373,  1415,   624,  -742,  1113,   626,   627,   628,   629,  1170,
     714,   764,  -749,  1385,   116,    70,   138,  -749,   139,  1148,
     117,  1180,  -742,   705,    13,   374,  1185,    16,   918,   845,
     715,   921,   706,  1324,   140,   928,  -749,   366,  -386,  1493,
    1133,   412,   134,  -386,   413,   618,   619,   414,   135,   529,
    1494,  1495,   530,   624,   997,   625,   626,   627,   628,   629,
    1469,  1470,  -386,    16,   367,   368,   599,   149,   377,   161,
    1435,  1435,   830,   647,   648,   975,  1443,   269,   366,    85,
    1435,  1443,  1030,   705,   904,   705,   177,   270,   176,   270,
     270,   153,   706,   696,   706,   182,   697,   154,   853,   414,
     853,   270,  1407,   102,   269,   367,   368,   877,   184,   270,
    1306,   644,   645,   646,  1477,   277,   269,   269,   269,   269,
     914,  1435,  1435,   269,   647,   648,   377,   269,   369,   377,
     831,   216,   370,   834,   269,   269,  1518,   269,   158,   269,
    1321,   269,   269,   220,   159,   332,  1322,   377,   705,    13,
    1526,   836,   377,   156,   157,  1266,   838,   706,  1269,  1022,
     333,  1053,  1025,   705,    13,  1135,  1534,   334,  1280,   369,
     377,  1537,   706,   370,  1108,   339,   270,   270,   337,   371,
    1251,  1041,   270,   372,   346,  1106,   373,  1523,    16,   705,
      13,   377,   347,   377,   354,  1116,   877,  1367,   706,  1426,
     351,   377,   378,    16,   366,  1418,  1252,   465,   610,   366,
     611,   374,   352,  1428,  1292,   397,   414,   270,   353,   398,
     371,   163,   164,   165,   372,   377,  1107,   373,  1447,    16,
     355,   367,   368,   399,   400,   356,   367,   368,   401,   402,
     403,   404,   223,   224,   366,   358,   357,   269,   359,   269,
     269,   360,   374,   361,   366,   375,  1397,   163,   164,   745,
     746,   269,   102,   103,   104,  1479,   223,   224,   225,   269,
    1429,   367,   368,  1033,   362,  1033,   270,    92,    93,    94,
     363,   367,   368,   379,   364,   380,   381,   382,  1352,   383,
     384,  1049,   385,   386,   387,   369,   393,  1056,  1057,   370,
     369,   407,   496,   517,   370,   523,   524,   525,   532,   533,
    1065,   534,   542,  1067,  1068,  1069,  1070,  1071,  1516,   484,
    1473,  1074,   604,   552,   553,   559,   568,   567,   570,   506,
    1102,   569,  1105,   571,   572,   369,   269,   269,   575,   370,
     516,   578,   269,   614,   615,   369,   371,   579,  1190,   370,
     372,   371,  1120,   373,   582,   372,   583,  1127,   373,  1399,
     584,   605,   585,  1502,  1238,   586,   587,   608,   607,   654,
     650,   666,   676,   653,   672,   705,    13,   269,   374,   694,
     695,   704,   713,   374,   706,   717,   371,   718,   748,   749,
     372,   750,   934,   373,  1423,   719,   371,   751,   753,   759,
     372,   766,  1128,   373,   760,   774,   580,   581,   793,   843,
     860,   904,   270,   776,   270,    16,   909,   848,   374,   859,
     910,   926,  1264,   589,   590,   591,   592,   593,   374,   937,
     960,   987,   965,   981,   968,   969,   269,   983,   616,   617,
     618,   619,   620,   988,   989,   621,   622,   623,   624,  1290,
     625,   626,   627,   628,   629,   990,   630,   631,  1007,   991,
     992,  1002,   632,   633,   634,  1003,  1008,  1015,   635,  1017,
    1019,  1021,  1039,  1040,  1048,  1050,  1058,  1472,  1062,  1063,
    1064,  1066,   675,  1072,  1101,  1111,  1294,    13,  1093,  1151,
     682,  1191,  1112,  1121,  1122,  1195,  1126,   636,  1131,   637,
     638,   639,   640,   641,   642,   643,   644,   645,   646,  1134,
    1172,  1192,   701,  1137,  1198,  1143,  1200,  1153,  1173,   647,
     648,  1175,  1201,  1242,  1181,  1189,    16,   614,   615,  1193,
    1202,   723,   726,  1205,  1206,   729,  1207,   731,  1208,  1209,
    1210,  1237,  1240,  1248,  1246,   270,  1268,   739,   740,   741,
     742,   743,   744,  1247,  1283,  1281,  1284,  1285,  1286,  1287,
    1307,  1317,  1320,  1324,  1328,  1329,  1331,  1336,  1335,  1337,
    1341,  1342,   269,  1344,   269,  1357,  1360,  1365,  1345,  1420,
    1362,   787,   788,  1373,  1411,   789,   790,   791,   792,  1379,
     794,  1403,   797,   798,   799,   801,   802,   803,   804,   805,
     806,   808,   809,   810,   811,   812,   813,   814,   815,   816,
     817,   818,  1393,   827,  1394,  1395,  1396,  1368,  1427,  1412,
    1419,  1430,   616,   617,   618,   619,   620,  1404,   366,   621,
     622,   623,   624,  1410,   625,   626,   627,   628,   629,  1451,
     630,   631,   -87,  1489,  1445,   366,   632,   633,   634,  1446,
     614,   615,   635,   903,  1478,   367,   368,  1480,  1326,   908,
    1327,  1422,  1481,  1424,  1425,   913,  1501,  1498,  1485,  1486,
    1487,  1507,   367,   368,  1488,   270,  1508,  1509,  1510,  1511,
     927,   636,  1515,   637,   638,   639,   640,   641,   642,   643,
     644,   645,   646,  1513,  1525,  1533,   747,   126,    20,    83,
     175,   129,  1538,   647,   648,   269,   144,   856,   778,   857,
    1489,   958,   350,  1536,   773,  1098,   961,    26,   962,   369,
     963,   612,   964,   370,  1475,  1433,  1361,  1432,  1386,   687,
    1476,    91,   723,  1235,  1094,   556,   369,  1095,  1383,     0,
     370,   406,   903,   557,  1449,     0,   725,   618,   619,     0,
       0,   614,   615,     0,  1500,   624,     0,   625,   626,   627,
     628,   629,     0,   764,     0,     0,     0,     0,     0,     0,
     371,     0,     0,     0,   372,     0,  1129,   373,     0,  1514,
       0,     0,     0,     0,     0,  1001,     0,   371,     0,  1006,
       0,   372,     0,  1130,   373,     0,     0,     0,     0,     0,
       0,     0,   374,     0,     0,     0,     0,     0,  1530,     0,
       0,   642,   643,   644,   645,   646,   764,     0,  1535,   374,
       0,     0,     0,     0,  1038,     0,   647,   648,     0,  1042,
       0,  1044,     0,     0,  1047,   269,     0,     0,     0,  1052,
    1464,  1055,     0,     0,     0,   877,   616,   617,   618,   619,
     620,   614,   615,   621,   622,   623,   624,     0,   625,   626,
     627,   628,   629,     0,   630,   631,     0,     0,     0,     0,
     632,   633,   634,     0,     0,     0,   635,  1104,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1124,   723,     0,     0,   636,     0,   637,   638,   639,
     640,   641,   642,   643,   644,   645,   646,     0,  1517,     0,
       0,     0,  1520,  1521,     0,     0,     0,   647,   648,     0,
       0,   649,     0,  1146,  1147,   877,     0,     0,  1529,     0,
       0,     0,     0,   366,   961,     0,   616,   617,   618,   619,
       0,     0,     0,  1166,     0,  1168,   624,  1171,   625,   626,
     627,   628,   629,  1174,   630,   631,     0,  1177,   614,   615,
     367,   368,     0,     0,     0,   961,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1197,     0,
       0,  1199,   642,   643,   644,   645,   646,     0,     0,  1204,
     367,   368,     0,     0,     0,     0,     0,   647,   648,     0,
       0,     0,     0,     0,   369,     0,     0,     0,   370,     0,
    1216,  1217,  1218,  1219,  1220,  1221,  1222,  1223,  1224,  1225,
    1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,  1234,     0,
       0,     0,     0,   616,   617,   618,   619,   620,  1241,     0,
     621,   622,   623,   624,     0,   625,   626,   627,   628,   629,
       0,   630,   631,   -78,   369,   371,     0,   723,   370,   372,
       0,  1132,   373,     0,   614,   615,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1273,  1274,
    1275,     0,     0,     0,     0,     0,     0,   374,     0,     0,
       0,     0,     0,     0,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,     0,   371,     0,     0,  1299,   372,
    1300,  1250,   373,     0,   647,   648,  1304,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1308,     0,     0,     0,  1310,     0,   374,     0,     0,
    1311,  1312,     0,  1314,  1315,     0,     0,     0,     0,   614,
     615,  1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,   616,
     617,   618,   619,   620,  1083,  1084,   621,   622,   623,   624,
    1085,   625,   626,   627,   628,   629,  1086,   630,   631,  1087,
    1088,     0,  1330,   632,   633,   634,  1089,  1090,  1091,   635,
       0,  1338,  1339,  1340,     0,     0,     0,     0,  1347,     0,
       0,     0,     0,     0,   366,     0,  1353,  1354,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1092,   636,     0,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
       0,   367,   368,     0,     0,     0,     0,     0,     0,     0,
     647,   648,     0,     0,   616,   617,   618,   619,   620,     0,
       0,   621,   622,   623,   624,     0,   625,   626,   627,   628,
     629,     0,   630,   631,     0,     0,     0,     0,   632,   633,
     634,     0,   723,     0,   635,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   369,     0,  1414,     0,   370,
       0,     0,     0,   636,     0,   637,   638,   639,   640,   641,
     642,   643,   644,   645,   646,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   647,   648,     0,  1440,   669,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1450,
       0,  1452,  1453,     0,     0,   723,   371,     0,  1456,     0,
     372,     0,  1255,   373,     0,  1463,     0,     0,     0,     0,
       0,   903,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,  1491,     0,     0,   865,
       0,     0,     0,   419,   420,     3,     0,   -98,   -92,   -92,
       0,   -95,  1463,   421,   422,   423,   424,   425,  1503,     0,
       0,     0,     0,   426,   866,   427,   867,   868,     0,   428,
       0,     0,     0,     0,     0,     0,   869,   429,     0,     0,
    -100,     0,   870,   430,     0,     0,   431,  1519,     8,   432,
     871,     0,   872,   433,     0,  1524,   873,   874,     0,     0,
    1527,   903,     0,   875,     0,     0,   435,   436,     0,   234,
     235,   236,     0,   238,   239,   240,   241,   242,   437,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,   256,   257,   258,     0,     0,   261,   262,   263,   264,
     438,   439,   440,   876,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   441,   442,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,   443,   444,   445,   446,   447,
       0,   448,     0,   449,   450,   451,   452,   453,   454,   455,
     456,    58,     0,    13,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     458,   459,   460,     0,    14,     0,     0,   461,   462,     0,
       0,     0,     0,     0,     0,     0,   463,     0,   464,     0,
     465,   466,    16,   467,  -247,   865,     0,     0,     0,   419,
     420,     3,     0,   -98,   -92,   -92,     0,   -95,     0,   421,
     422,   423,   424,   425,     0,     0,     0,     0,     0,   426,
     866,   427,   867,   868,     0,   428,     0,     0,     0,     0,
       0,     0,   869,   429,     0,     0,  -100,     0,   870,   430,
       0,     0,   431,     0,     8,   432,   871,     0,   872,   433,
       0,     0,   873,   874,     0,     0,     0,     0,     0,   875,
       0,     0,   435,   436,     0,   234,   235,   236,     0,   238,
     239,   240,   241,   242,   437,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   256,   257,   258,
       0,     0,   261,   262,   263,   264,   438,   439,   440,   876,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   441,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   443,   444,   445,   446,   447,     0,   448,     0,   449,
     450,   451,   452,   453,   454,   455,   456,    58,     0,    13,
     457,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   458,   459,   460,     0,
      14,     0,     0,   461,   462,     0,     0,     0,     0,     0,
       0,     0,   463,     0,   464,     0,   465,   466,    16,   467,
     985,   865,     0,     0,     0,   419,   420,     3,     0,   -98,
     -92,   -92,     0,   -95,     0,   421,   422,   423,   424,   425,
       0,     0,     0,     0,     0,   426,   866,   427,   867,   868,
       0,   428,     0,     0,     0,     0,     0,     0,   869,   429,
       0,     0,  -100,     0,   870,   430,     0,     0,   431,     0,
       8,   432,   871,     0,   872,   433,     0,     0,   873,   874,
       0,     0,     0,     0,     0,   875,     0,     0,   435,   436,
       0,   234,   235,   236,     0,   238,   239,   240,   241,   242,
     437,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,   256,   257,   258,     0,     0,   261,   262,
     263,   264,   438,   439,   440,   876,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   441,   442,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,   443,   444,   445,
     446,   447,     0,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,    58,     0,    13,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   458,   459,   460,     0,    14,     0,     0,   461,
     462,     0,     0,     0,     0,     0,     0,     0,   463,     0,
     464,     0,   465,   466,    16,   467,  -244,   865,     0,     0,
       0,   419,   420,     3,     0,   -98,   -92,   -92,     0,   -95,
       0,   421,   422,   423,   424,   425,     0,     0,     0,     0,
       0,   426,   866,   427,   867,   868,     0,   428,     0,     0,
       0,     0,     0,     0,   869,   429,     0,     0,  -100,     0,
     870,   430,     0,     0,   431,     0,     8,   432,   871,     0,
     872,   433,     0,     0,   873,   874,     0,     0,     0,     0,
       0,   875,     0,     0,   435,   436,     0,   234,   235,   236,
       0,   238,   239,   240,   241,   242,   437,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,   256,
     257,   258,     0,     0,   261,   262,   263,   264,   438,   439,
     440,   876,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   441,   442,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   443,   444,   445,   446,   447,     0,   448,
       0,   449,   450,   451,   452,   453,   454,   455,   456,    58,
       0,    13,   457,     0,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   458,   459,
     460,     0,    14,     0,     0,   461,   462,     0,     0,     0,
       0,   367,   368,     0,   463,     0,   464,     0,   465,   466,
      16,   467,   -88,   419,   420,     0,     0,     0,     0,     0,
       0,     0,     0,   421,   422,   423,   424,   425,     0,     0,
       0,     0,     0,   426,   866,   427,   867,     0,     0,   428,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
       0,     0,     0,   430,     0,     0,   431,     0,     0,   432,
     871,     0,     0,   433,     0,   369,     0,     0,     0,   370,
       0,     0,     0,   434,     0,     0,   435,   436,     0,   234,
     235,   236,     0,   238,   239,   240,   241,   242,   437,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,   256,   257,   258,     0,     0,   261,   262,   263,   264,
     438,   439,   440,   876,     0,     0,   371,     0,     0,     0,
     372,     0,  1278,   373,     0,     0,   441,   442,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,   374,     0,
       0,     0,     0,     0,     0,   443,   444,   445,   446,   447,
       0,   448,     0,   449,   450,   451,   452,   453,   454,   455,
     456,    58,     0,     0,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     458,   459,   460,     0,    14,     0,     0,   461,   462,     0,
       0,     0,     0,     0,   419,   420,   463,     0,   464,     0,
     465,   466,     0,   467,   421,   422,   423,   424,   425,     0,
       0,     0,     0,     0,   426,     0,   427,     0,     0,     0,
     428,     0,     0,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,   430,     0,     0,   431,     0,     0,
     432,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,     0,     0,   435,   436,   768,
     234,   235,   236,     0,   238,   239,   240,   241,   242,   437,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,   256,   257,   258,     0,     0,   261,   262,   263,
     264,   438,   439,   440,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,   442,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   443,   444,   445,   446,
     447,     0,   448,   599,   449,   450,   451,   452,   453,   454,
     455,   456,   600,     0,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   458,   459,   460,     0,    14,     0,     0,   461,   462,
       0,     0,     0,     0,     0,   419,   420,   769,     0,   464,
     770,   465,   466,   594,   467,   421,   422,   423,   424,   425,
       0,     0,     0,     0,     0,   426,     0,   427,     0,     0,
       0,   428,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,   430,     0,     0,   431,   595,
       0,   432,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,   435,   436,
       0,   234,   235,   236,     0,   238,   239,   240,   241,   242,
     437,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,   256,   257,   258,     0,     0,   261,   262,
     263,   264,   438,   439,   440,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   441,   442,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,   443,   444,   445,
     446,   447,     0,   448,   599,   449,   450,   451,   452,   453,
     454,   455,   456,   600,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   458,   459,   460,     0,    14,     0,     0,   461,
     462,     0,     0,     0,     0,     0,   419,   420,   463,     0,
     464,     0,   465,   466,   594,   467,   421,   422,   423,   424,
     425,     0,     0,     0,     0,     0,   426,     0,   427,     0,
       0,   366,   428,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,   430,     0,     0,   431,
     595,     0,   432,     0,     0,     0,   433,     0,   367,   368,
       0,     0,     0,     0,     0,     0,   434,     0,     0,   435,
     436,     0,   234,   235,   236,     0,   238,   239,   240,   241,
     242,   437,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,   256,   257,   258,     0,     0,   261,
     262,   263,   264,   438,   439,   440,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,     0,   369,     0,     0,     0,   370,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,   443,   444,
     445,   446,   447,     0,   448,     0,   449,   450,   451,   452,
     453,   454,   455,   456,    58,     0,     0,   457,     0,     0,
       0,     0,     0,   371,     0,     0,     0,   372,     0,  1366,
     373,     0,     0,   458,   459,   460,     0,    14,     0,     0,
     461,   462,     0,     0,     0,     0,     0,   419,   420,   463,
       0,   464,     0,   465,   466,   374,   467,   421,   422,   423,
     424,   425,     0,     0,     0,     0,     0,   426,     0,   427,
       0,     0,   366,   428,     0,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,   430,     0,     0,
     431,     0,     0,   432,     0,     0,     0,   433,     0,   367,
     368,     0,     0,     0,     0,     0,     0,   434,     0,     0,
     435,   436,   922,   234,   235,   236,     0,   238,   239,   240,
     241,   242,   437,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,   256,   257,   258,     0,     0,
     261,   262,   263,   264,   438,   439,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     441,   442,     0,   369,     0,     0,     0,   370,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,   443,
     444,   445,   446,   447,     0,   448,   599,   449,   450,   451,
     452,   453,   454,   455,   456,   600,     0,     0,   457,     0,
       0,     0,     0,     0,   371,     0,     0,     0,   372,     0,
       0,   373,     0,     0,   458,   459,   460,     0,    14,     0,
       0,   461,   462,     0,     0,     0,     0,     0,   419,   420,
     463,     0,   464,     0,   465,   466,   374,   467,   421,   422,
     423,   424,   425,     0,     0,     0,     0,     0,   426,     0,
     427,     0,     0,     0,   428,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,   430,     0,
       0,   431,     0,     0,   432,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
       0,   435,   436,     0,   234,   235,   236,     0,   238,   239,
     240,   241,   242,   437,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,   256,   257,   258,     0,
       0,   261,   262,   263,   264,   438,   439,   440,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   441,   442,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
     443,   444,   445,   446,   447,     0,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,    58,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   458,   459,   460,     0,    14,
       0,     0,   461,   462,     0,     0,     0,     0,     0,   419,
     420,   463,   515,   464,     0,   465,   466,     0,   467,   421,
     422,   423,   424,   425,     0,     0,     0,     0,     0,   426,
       0,   427,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,   430,
       0,     0,   431,     0,     0,   432,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
       0,     0,   435,   436,     0,   234,   235,   236,     0,   238,
     239,   240,   241,   242,   437,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   256,   257,   258,
       0,     0,   261,   262,   263,   264,   438,   439,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   441,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   443,   444,   445,   446,   447,     0,   448,   599,   449,
     450,   451,   452,   453,   454,   455,   456,   600,     0,     0,
     457,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   458,   459,   460,     0,
      14,     0,     0,   461,   462,     0,     0,     0,     0,     0,
     419,   420,   463,     0,   464,     0,   465,   466,     0,   467,
     421,   422,   423,   424,   425,     0,     0,     0,     0,     0,
     426,     0,   427,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
     430,     0,     0,   431,     0,     0,   432,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,     0,     0,   435,   436,     0,   234,   235,   236,     0,
     238,   239,   240,   241,   242,   437,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,   256,   257,
     258,     0,     0,   261,   262,   263,   264,   438,   439,   440,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,   443,   444,   445,   446,   447,     0,   448,     0,
     449,   450,   451,   452,   453,   454,   455,   456,    58,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   458,   459,   460,
       0,    14,     0,     0,   461,   462,     0,     0,     0,     0,
       0,   419,   420,   463,   674,   464,     0,   465,   466,     0,
     467,   421,   422,   423,   424,   425,     0,     0,   807,     0,
       0,   426,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,   430,     0,     0,   431,     0,     0,   432,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,     0,     0,   435,   436,     0,   234,   235,   236,
       0,   238,   239,   240,   241,   242,   437,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,   256,
     257,   258,     0,     0,   261,   262,   263,   264,   438,   439,
     440,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   441,   442,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   443,   444,   445,   446,   447,     0,   448,
       0,   449,   450,   451,   452,   453,   454,   455,   456,    58,
       0,     0,   457,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   458,   459,
     460,     0,    14,     0,     0,   461,   462,     0,     0,     0,
       0,     0,   419,   420,   463,     0,   464,     0,   465,   466,
       0,   467,   421,   422,   423,   424,   425,     0,     0,     0,
       0,     0,   426,     0,   427,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,   430,     0,     0,   431,     0,     0,   432,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,     0,     0,   435,   436,     0,   234,   235,
     236,     0,   238,   239,   240,   241,   242,   437,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
     256,   257,   258,     0,     0,   261,   262,   263,   264,   438,
     439,   440,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   441,   442,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   443,   444,   445,   446,   447,     0,
     448,     0,   449,   450,   451,   452,   453,   454,   455,   456,
      58,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   458,
     459,   460,     0,    14,     0,     0,   461,   462,     0,     0,
       0,     0,     0,   419,   420,   463,     0,   464,   828,   465,
     466,     0,   467,   421,   422,   423,   424,   425,     0,     0,
       0,     0,     0,   426,     0,   427,     0,     0,     0,   428,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
       0,     0,     0,   430,     0,     0,   431,     0,     0,   432,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   434,     0,     0,   435,   436,     0,   234,
     235,   236,     0,   238,   239,   240,   241,   242,   437,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,   256,   257,   258,     0,     0,   261,   262,   263,   264,
     438,   439,   440,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   441,   442,     0,     0,
       0,     0,     0,     0,     0,  1043,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,   443,   444,   445,   446,   447,
       0,   448,     0,   449,   450,   451,   452,   453,   454,   455,
     456,    58,     0,     0,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     458,   459,   460,     0,    14,     0,     0,   461,   462,     0,
       0,     0,     0,     0,   419,   420,   463,     0,   464,     0,
     465,   466,     0,   467,   421,   422,   423,   424,   425,     0,
       0,     0,     0,     0,   426,     0,   427,     0,     0,     0,
     428,     0,     0,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,   430,     0,     0,   431,     0,     0,
     432,     0,     0,     0,   433,     0,     0,     0,     0,     0,
    1046,     0,     0,     0,   434,     0,     0,   435,   436,     0,
     234,   235,   236,     0,   238,   239,   240,   241,   242,   437,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,   256,   257,   258,     0,     0,   261,   262,   263,
     264,   438,   439,   440,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,   442,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   443,   444,   445,   446,
     447,     0,   448,     0,   449,   450,   451,   452,   453,   454,
     455,   456,    58,     0,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   458,   459,   460,     0,    14,     0,     0,   461,   462,
       0,     0,     0,     0,     0,   419,   420,   463,     0,   464,
       0,   465,   466,     0,   467,   421,   422,   423,   424,   425,
       0,     0,     0,     0,     0,   426,     0,   427,     0,     0,
       0,   428,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,   430,     0,     0,   431,     0,
       0,   432,     0,     0,     0,   433,     0,     0,  1051,     0,
       0,     0,     0,     0,     0,   434,     0,     0,   435,   436,
       0,   234,   235,   236,     0,   238,   239,   240,   241,   242,
     437,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,   256,   257,   258,     0,     0,   261,   262,
     263,   264,   438,   439,   440,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   441,   442,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,   443,   444,   445,
     446,   447,     0,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,    58,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   458,   459,   460,     0,    14,     0,     0,   461,
     462,     0,     0,     0,     0,     0,   419,   420,   463,     0,
     464,     0,   465,   466,     0,   467,   421,   422,   423,   424,
     425,     0,     0,     0,     0,     0,   426,     0,   427,     0,
       0,     0,   428,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,   430,     0,     0,   431,
       0,     0,   432,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,     0,     0,   435,
     436,     0,   234,   235,   236,     0,   238,   239,   240,   241,
     242,   437,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,   256,   257,   258,     0,     0,   261,
     262,   263,   264,   438,   439,   440,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,     0,     0,     0,     0,     0,     0,     0,  1054,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,   443,   444,
     445,   446,   447,     0,   448,     0,   449,   450,   451,   452,
     453,   454,   455,   456,    58,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   458,   459,   460,     0,    14,     0,     0,
     461,   462,     0,     0,     0,     0,     0,   419,   420,   463,
       0,   464,     0,   465,   466,     0,   467,   421,   422,   423,
     424,   425,     0,     0,     0,     0,     0,   426,     0,   427,
       0,     0,     0,   428,     0,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,   430,     0,     0,
     431,     0,     0,   432,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   434,     0,     0,
     435,   436,     0,   234,   235,   236,     0,   238,   239,   240,
     241,   242,   437,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,   256,   257,   258,     0,     0,
     261,   262,   263,   264,   438,   439,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     441,   442,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,   443,
     444,   445,   446,   447,     0,   448,     0,   449,   450,   451,
     452,   453,   454,   455,   456,    58,     0,     0,   457,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   458,   459,   460,     0,    14,     0,
       0,   461,   462,     0,     0,     0,     0,     0,   419,   420,
     463,     0,   464,  1169,   465,   466,     0,   467,   421,   422,
     423,   424,   425,     0,     0,     0,     0,     0,   426,     0,
     427,     0,     0,     0,   428,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,   430,     0,
       0,   431,     0,     0,   432,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
       0,   435,   436,     0,   234,   235,   236,     0,   238,   239,
     240,   241,   242,   437,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,   256,   257,   258,     0,
       0,   261,   262,   263,   264,   438,   439,   440,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   441,   442,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
     443,   444,   445,   446,   447,     0,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,    58,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   458,   459,   460,     0,    14,
       0,     0,   461,   462,     0,     0,     0,     0,     0,   419,
     420,  1178,     0,   464,  1179,   465,   466,     0,   467,   421,
     422,   423,   424,   425,     0,     0,     0,     0,     0,   426,
       0,   427,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,   430,
       0,     0,   431,     0,     0,   432,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
       0,     0,   435,   436,     0,   234,   235,   236,     0,   238,
     239,   240,   241,   242,   437,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   256,   257,   258,
       0,     0,   261,   262,   263,   264,   438,   439,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   441,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   443,   444,   445,   446,   447,     0,   448,     0,   449,
     450,   451,   452,   453,   454,   455,   456,    58,     0,     0,
     457,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   458,   459,   460,     0,
      14,     0,     0,   461,   462,     0,     0,     0,     0,     0,
     419,   420,   463,     0,   464,  1184,   465,   466,     0,   467,
     421,   422,   423,   424,   425,     0,     0,     0,     0,     0,
     426,     0,   427,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
     430,     0,     0,   431,     0,     0,   432,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,     0,     0,   435,   436,     0,   234,   235,   236,     0,
     238,   239,   240,   241,   242,   437,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,   256,   257,
     258,     0,     0,   261,   262,   263,   264,   438,   439,   440,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,   443,   444,   445,   446,   447,     0,   448,     0,
     449,   450,   451,   452,   453,   454,   455,   456,    58,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   458,   459,   460,
       0,    14,     0,     0,   461,   462,     0,     0,     0,     0,
       0,   419,   420,   463,     0,   464,  1265,   465,   466,     0,
     467,   421,   422,   423,   424,   425,     0,     0,     0,     0,
       0,   426,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,   430,     0,     0,   431,     0,     0,   432,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,     0,     0,   435,   436,     0,   234,   235,   236,
       0,   238,   239,   240,   241,   242,   437,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,   256,
     257,   258,     0,     0,   261,   262,   263,   264,   438,   439,
     440,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   441,   442,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   443,   444,   445,   446,   447,     0,   448,
       0,   449,   450,   451,   452,   453,   454,   455,   456,    58,
       0,     0,   457,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   458,   459,
     460,     0,    14,     0,     0,   461,   462,     0,     0,     0,
       0,     0,   419,   420,   463,     0,   464,  1346,   465,   466,
       0,   467,   421,   422,   423,   424,   425,     0,     0,     0,
       0,     0,   426,     0,   427,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,   430,     0,     0,   431,     0,     0,   432,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,     0,     0,   435,   436,     0,   234,   235,
     236,     0,   238,   239,   240,   241,   242,   437,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
     256,   257,   258,     0,     0,   261,   262,   263,   264,   438,
     439,   440,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   441,   442,     0,     0,     0,
       0,     0,     0,     0,  1413,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   443,   444,   445,   446,   447,     0,
     448,     0,   449,   450,   451,   452,   453,   454,   455,   456,
      58,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   458,
     459,   460,     0,    14,     0,     0,   461,   462,     0,     0,
       0,     0,     0,   419,   420,   463,     0,   464,     0,   465,
     466,     0,   467,   421,   422,   423,   424,   425,     0,     0,
       0,     0,     0,   426,     0,   427,     0,     0,     0,   428,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
       0,     0,     0,   430,     0,     0,   431,     0,     0,   432,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   434,     0,     0,   435,   436,     0,   234,
     235,   236,     0,   238,   239,   240,   241,   242,   437,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,   256,   257,   258,     0,     0,   261,   262,   263,   264,
     438,   439,   440,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   441,   442,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,   443,   444,   445,   446,   447,
       0,   448,     0,   449,   450,   451,   452,   453,   454,   455,
     456,    58,     0,     0,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     458,   459,   460,     0,    14,     0,     0,   461,   462,     0,
       0,     0,     0,     0,   419,   420,   463,     0,   464,     0,
     465,   466,     0,   467,   421,   422,   423,   424,   425,     0,
       0,     0,     0,     0,   426,     0,   427,     0,     0,     0,
     428,     0,     0,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,   430,     0,     0,   431,     0,     0,
     432,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,     0,     0,   435,   436,     0,
     234,   235,   236,     0,   238,   239,   240,   241,   242,   437,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,   256,   257,   258,     0,     0,   261,   262,   263,
     264,   438,   439,   440,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,   442,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   443,   444,   445,   446,
     447,     0,   448,     0,   449,   450,   451,   452,   453,   454,
     455,   456,    58,     0,   228,   457,     0,     0,     0,     0,
     229,     0,     0,     0,     0,     0,   230,     0,     0,     0,
       0,   458,   459,   460,     0,    14,   231,     0,   461,   462,
       0,     0,     0,     0,   232,     0,     0,  1155,     0,   464,
       0,   465,   466,     0,   467,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,     0,
       0,     0,     0,     0,     0,     0,     0,   228,     0,     0,
       0,     0,     0,   229,     0,     0,     0,     0,     0,   230,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   231,
       0,     0,     0,     0,    57,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   267,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,    58,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,     0,     0,     0,     0,     0,   268,     0,     0,
       0,     0,   518,   228,     0,     0,     0,     0,     0,   229,
       0,     0,     0,     0,     0,   230,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   231,     0,    57,     0,     0,
       0,     0,     0,   232,     0,     0,     0,     0,     0,     0,
     267,     0,     0,     0,     0,     0,     0,     0,   233,     0,
       0,   662,     0,    13,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,     0,     0,
     268,     0,    16,   614,   615,     0,   228,     0,     0,     0,
       0,     0,   229,     0,     0,     0,     0,     0,   230,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   231,     0,
       0,     0,     0,    57,     0,     0,   232,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   267,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,    58,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,     0,     0,     0,     0,     0,   268,     0,   616,   617,
     618,   619,   620,   614,   615,   621,   622,   623,   624,     0,
     625,   626,   627,   628,   629,     0,   630,   631,     0,     0,
       0,     0,   632,   633,   634,     0,    57,     0,   635,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     662,     0,     0,     0,     0,     0,     0,   636,     0,   637,
     638,   639,   640,   641,   642,   643,   644,   645,   646,     0,
       0,     0,     0,   779,     0,     0,     0,     0,     0,   647,
     648,     0,     0,   858,     0,     0,     0,     0,     0,   268,
     614,   615,     0,     0,     0,     0,     0,     0,   616,   617,
     618,   619,   620,     0,     0,   621,   622,   623,   624,     0,
     625,   626,   627,   628,   629,     0,   630,   631,     0,     0,
       0,     0,   632,   633,   634,   234,   235,   236,   635,   238,
     239,   240,   241,   242,   437,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   256,   257,   258,
       0,     0,   261,   262,   263,   264,     0,   636,     0,   637,
     638,   639,   640,   641,   642,   643,   644,   645,   646,     0,
       0,     0,     0,     0,     0,     0,   614,   615,     0,   647,
     648,     0,     0,   939,     0,   616,   617,   618,   619,   620,
       0,     0,   621,   622,   623,   624,     0,   625,   626,   627,
     628,   629,     0,   630,   631,     0,     0,     0,   780,   632,
     633,   634,     0,     0,     0,   635,     0,   781,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   636,     0,   637,   638,   639,   640,
     641,   642,   643,   644,   645,   646,     0,     0,     0,     0,
       0,     0,   614,   615,     0,     0,   647,   648,     0,     0,
     942,   616,   617,   618,   619,   620,     0,     0,   621,   622,
     623,   624,     0,   625,   626,   627,   628,   629,     0,   630,
     631,     0,     0,     0,     0,   632,   633,   634,     0,     0,
       0,   635,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     636,     0,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,     0,     0,     0,     0,     0,     0,   614,   615,
       0,     0,   647,   648,     0,     0,   944,   616,   617,   618,
     619,   620,     0,     0,   621,   622,   623,   624,     0,   625,
     626,   627,   628,   629,     0,   630,   631,     0,     0,     0,
       0,   632,   633,   634,     0,     0,     0,   635,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   636,     0,   637,   638,
     639,   640,   641,   642,   643,   644,   645,   646,     0,     0,
       0,     0,     0,     0,   614,   615,     0,     0,   647,   648,
       0,     0,   952,   616,   617,   618,   619,   620,     0,     0,
     621,   622,   623,   624,     0,   625,   626,   627,   628,   629,
       0,   630,   631,     0,     0,     0,     0,   632,   633,   634,
       0,     0,     0,   635,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   636,     0,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,     0,     0,     0,     0,     0,     0,
     614,   615,     0,     0,   647,   648,     0,     0,   953,   616,
     617,   618,   619,   620,     0,     0,   621,   622,   623,   624,
       0,   625,   626,   627,   628,   629,     0,   630,   631,     0,
       0,     0,     0,   632,   633,   634,     0,     0,     0,   635,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   636,     0,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
       0,     0,     0,     0,     0,     0,   614,   615,     0,     0,
     647,   648,     0,     0,   954,   616,   617,   618,   619,   620,
       0,     0,   621,   622,   623,   624,     0,   625,   626,   627,
     628,   629,     0,   630,   631,     0,     0,     0,     0,   632,
     633,   634,     0,     0,     0,   635,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   636,     0,   637,   638,   639,   640,
     641,   642,   643,   644,   645,   646,     0,     0,     0,     0,
       0,     0,   614,   615,     0,     0,   647,   648,     0,     0,
     955,   616,   617,   618,   619,   620,     0,     0,   621,   622,
     623,   624,     0,   625,   626,   627,   628,   629,     0,   630,
     631,     0,     0,     0,     0,   632,   633,   634,     0,     0,
       0,   635,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     636,     0,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,     0,     0,     0,     0,     0,     0,   614,   615,
       0,     0,   647,   648,     0,     0,   956,   616,   617,   618,
     619,   620,     0,     0,   621,   622,   623,   624,     0,   625,
     626,   627,   628,   629,     0,   630,   631,     0,     0,     0,
       0,   632,   633,   634,     0,     0,     0,   635,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   636,     0,   637,   638,
     639,   640,   641,   642,   643,   644,   645,   646,     0,     0,
       0,     0,     0,     0,   614,   615,     0,     0,   647,   648,
       0,     0,   957,   616,   617,   618,   619,   620,     0,     0,
     621,   622,   623,   624,     0,   625,   626,   627,   628,   629,
       0,   630,   631,     0,     0,     0,     0,   632,   633,   634,
       0,     0,     0,   635,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   636,     0,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,     0,     0,     0,     0,     0,     0,
     614,   615,     0,     0,   647,   648,     0,     0,  1100,   616,
     617,   618,   619,   620,     0,     0,   621,   622,   623,   624,
       0,   625,   626,   627,   628,   629,     0,   630,   631,     0,
       0,     0,     0,   632,   633,   634,     0,     0,     0,   635,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   636,     0,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
       0,     0,     0,     0,     0,     0,   614,   615,     0,     0,
     647,   648,     0,     0,  1115,   616,   617,   618,   619,   620,
       0,     0,   621,   622,   623,   624,     0,   625,   626,   627,
     628,   629,     0,   630,   631,     0,     0,     0,     0,   632,
     633,   634,     0,     0,     0,   635,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   636,     0,   637,   638,   639,   640,
     641,   642,   643,   644,   645,   646,     0,     0,     0,     0,
       0,     0,   614,   615,     0,     0,   647,   648,     0,     0,
    1142,   616,   617,   618,   619,   620,     0,     0,   621,   622,
     623,   624,     0,   625,   626,   627,   628,   629,     0,   630,
     631,     0,     0,     0,     0,   632,   633,   634,     0,     0,
       0,   635,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     636,     0,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,     0,     0,     0,     0,     0,     0,   614,   615,
       0,     0,   647,   648,     0,     0,  1194,   616,   617,   618,
     619,   620,     0,     0,   621,   622,   623,   624,     0,   625,
     626,   627,   628,   629,     0,   630,   631,     0,     0,     0,
       0,   632,   633,   634,     0,     0,     0,   635,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   636,     0,   637,   638,
     639,   640,   641,   642,   643,   644,   645,   646,     0,     0,
       0,     0,     0,     0,   614,   615,     0,     0,   647,   648,
       0,     0,  1276,   616,   617,   618,   619,   620,     0,     0,
     621,   622,   623,   624,     0,   625,   626,   627,   628,   629,
       0,   630,   631,     0,     0,     0,     0,   632,   633,   634,
       0,     0,     0,   635,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   636,     0,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,     0,     0,     0,     0,     0,     0,
     614,   615,     0,     0,   647,   648,     0,     0,  1277,   616,
     617,   618,   619,   620,     0,     0,   621,   622,   623,   624,
       0,   625,   626,   627,   628,   629,     0,   630,   631,     0,
       0,     0,     0,   632,   633,   634,     0,     0,     0,   635,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   636,     0,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
       0,     0,     0,     0,     0,     0,   614,   615,     0,     0,
     647,   648,     0,     0,  1293,   616,   617,   618,   619,   620,
       0,     0,   621,   622,   623,   624,     0,   625,   626,   627,
     628,   629,     0,   630,   631,     0,     0,     0,     0,   632,
     633,   634,     0,     0,     0,   635,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   636,     0,   637,   638,   639,   640,
     641,   642,   643,   644,   645,   646,     0,     0,     0,     0,
       0,     0,   614,   615,     0,     0,   647,   648,     0,     0,
    1295,   616,   617,   618,   619,   620,     0,     0,   621,   622,
     623,   624,     0,   625,   626,   627,   628,   629,     0,   630,
     631,     0,     0,     0,     0,   632,   633,   634,     0,     0,
       0,   635,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     636,     0,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,     0,     0,     0,     0,     0,     0,   614,   615,
       0,     0,   647,   648,     0,     0,  1297,   616,   617,   618,
     619,   620,     0,     0,   621,   622,   623,   624,     0,   625,
     626,   627,   628,   629,     0,   630,   631,     0,     0,     0,
       0,   632,   633,   634,     0,     0,     0,   635,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   636,     0,   637,   638,
     639,   640,   641,   642,   643,   644,   645,   646,     0,     0,
       0,     0,     0,     0,   614,   615,     0,     0,   647,   648,
       0,     0,  1301,   616,   617,   618,   619,   620,     0,     0,
     621,   622,   623,   624,     0,   625,   626,   627,   628,   629,
       0,   630,   631,     0,     0,     0,     0,   632,   633,   634,
       0,     0,     0,   635,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   636,     0,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,     0,     0,     0,     0,     0,     0,
     614,   615,     0,     0,   647,   648,     0,     0,  1355,   616,
     617,   618,   619,   620,     0,     0,   621,   622,   623,   624,
       0,   625,   626,   627,   628,   629,     0,   630,   631,     0,
       0,     0,     0,   632,   633,   634,     0,     0,     0,   635,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   636,     0,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
       0,     0,     0,     0,     0,     0,   614,   615,     0,     0,
     647,   648,     0,     0,  1369,   616,   617,   618,   619,   620,
       0,     0,   621,   622,   623,   624,     0,   625,   626,   627,
     628,   629,     0,   630,   631,     0,     0,     0,     0,   632,
     633,   634,     0,     0,     0,   635,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   636,     0,   637,   638,   639,   640,
     641,   642,   643,   644,   645,   646,     0,     0,     0,     0,
       0,     0,   614,   615,     0,     0,   647,   648,     0,     0,
    1370,   616,   617,   618,   619,   620,     0,     0,   621,   622,
     623,   624,     0,   625,   626,   627,   628,   629,     0,   630,
     631,     0,     0,     0,     0,   632,   633,   634,     0,     0,
       0,   635,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     636,     0,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,     0,     0,     0,     0,     0,     0,   614,   615,
       0,     0,   647,   648,     0,     0,  1372,   616,   617,   618,
     619,   620,     0,     0,   621,   622,   623,   624,     0,   625,
     626,   627,   628,   629,     0,   630,   631,     0,     0,     0,
       0,   632,   633,   634,     0,     0,     0,   635,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   636,     0,   637,   638,
     639,   640,   641,   642,   643,   644,   645,   646,     0,     0,
       0,     0,     0,     0,   614,   615,     0,     0,   647,   648,
       0,     0,  1374,   616,   617,   618,   619,   620,     0,     0,
     621,   622,   623,   624,     0,   625,   626,   627,   628,   629,
       0,   630,   631,     0,     0,     0,     0,   632,   633,   634,
       0,     0,     0,   635,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   636,     0,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,     0,     0,     0,     0,     0,     0,
     614,   615,     0,     0,   647,   648,     0,     0,  1375,   616,
     617,   618,   619,   620,     0,     0,   621,   622,   623,   624,
       0,   625,   626,   627,   628,   629,     0,   630,   631,     0,
       0,     0,     0,   632,   633,   634,     0,     0,     0,   635,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   636,     0,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
       0,     0,     0,     0,     0,     0,   614,   615,     0,     0,
     647,   648,     0,     0,  1377,   616,   617,   618,   619,   620,
       0,     0,   621,   622,   623,   624,     0,   625,   626,   627,
     628,   629,     0,   630,   631,     0,     0,     0,     0,   632,
     633,   634,     0,     0,     0,   635,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   636,     0,   637,   638,   639,   640,
     641,   642,   643,   644,   645,   646,     0,     0,     0,     0,
       0,     0,   614,   615,     0,     0,   647,   648,     0,     0,
    1378,   616,   617,   618,   619,   620,     0,     0,   621,   622,
     623,   624,     0,   625,   626,   627,   628,   629,     0,   630,
     631,     0,     0,     0,     0,   632,   633,   634,     0,     0,
       0,   635,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     636,     0,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,     0,     0,     0,     0,     0,     0,   614,   615,
       0,     0,   647,   648,     0,     0,  1392,   616,   617,   618,
     619,   620,     0,     0,   621,   622,   623,   624,     0,   625,
     626,   627,   628,   629,     0,   630,   631,     0,     0,     0,
       0,   632,   633,   634,     0,     0,     0,   635,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   636,     0,   637,   638,
     639,   640,   641,   642,   643,   644,   645,   646,     0,     0,
       0,     0,     0,     0,   614,   615,     0,     0,   647,   648,
       0,     0,  1402,   616,   617,   618,   619,   620,     0,     0,
     621,   622,   623,   624,     0,   625,   626,   627,   628,   629,
       0,   630,   631,     0,     0,     0,     0,   632,   633,   634,
       0,     0,     0,   635,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   636,     0,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,     0,     0,     0,     0,     0,     0,
     614,   615,     0,     0,   647,   648,     0,     0,  1482,   616,
     617,   618,   619,   620,     0,     0,   621,   622,   623,   624,
       0,   625,   626,   627,   628,   629,     0,   630,   631,     0,
       0,     0,     0,   632,   633,   634,     0,     0,     0,   635,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   636,     0,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
       0,     0,     0,     0,   614,   615,     0,     0,     0,     0,
     647,   648,     0,     0,  1532,   616,   617,   618,   619,   620,
       0,     0,   621,   622,   623,   624,     0,   625,   626,   627,
     628,   629,     0,   630,   631,     0,     0,     0,     0,   632,
     633,   634,     0,     0,     0,   635,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   636,     0,   637,   638,   639,   640,
     641,   642,   643,   644,   645,   646,     0,     0,   614,   615,
       0,     0,     0,     0,     0,     0,   647,   648,   673,   616,
     617,   618,   619,   620,     0,     0,   621,   622,   623,   624,
       0,   625,   626,   627,   628,   629,     0,   630,   631,     0,
       0,     0,     0,   632,   633,   634,     0,     0,     0,   635,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   636,     0,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
       0,     0,   614,   615,     0,     0,     0,     0,     0,     0,
     647,   648,   849,   616,   617,   618,   619,   620,     0,     0,
     621,   622,   623,   624,     0,   625,   626,   627,   628,   629,
       0,   630,   631,     0,     0,     0,     0,   632,   633,   634,
       0,     0,     0,   635,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   636,     0,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,     0,     0,   614,   615,     0,     0,
       0,     0,     0,     0,   647,   648,   993,   616,   617,   618,
     619,   620,     0,     0,   621,   622,   623,   624,     0,   625,
     626,   627,   628,   629,     0,   630,   631,     0,     0,     0,
       0,   632,   633,   634,     0,     0,     0,   635,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   636,     0,   637,   638,
     639,   640,   641,   642,   643,   644,   645,   646,     0,     0,
     614,   615,   185,     0,     0,     0,     0,     0,   647,   648,
    1009,   616,   617,   618,   619,   620,     0,     0,   621,   622,
     623,   624,     0,   625,   626,   627,   628,   629,     0,   630,
     631,     0,     0,     0,     0,   632,   633,   634,   186,     0,
     187,   635,   188,   189,   190,   191,   192,     0,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,     0,
     204,   205,   206,     0,     0,   207,   208,   209,   210,     0,
     636,     0,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,     0,     0,     0,   211,   212,     0,     0,     0,
       0,     0,   647,   648,  1176,   616,   617,   618,   619,   620,
     281,   282,   621,   622,   623,   624,     0,   625,   626,   627,
     628,   629,     0,   630,   631,     0,     0,   283,     0,   632,
     633,   634,     0,     0,     0,   635,     0,     0,     0,     0,
     213,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   636,     0,   637,   638,   639,   640,
     641,   642,   643,   644,   645,   646,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   647,   648,  1182,     0,
       0,     0,     0,   614,   615,     0,     0,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,     0,     0,   302,   303,   304,
       0,     0,   305,   306,   307,   308,   309,     0,     0,   310,
     311,   312,   313,   314,   315,   316,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   783,   317,     0,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   614,   615,   328,   329,
       0,     0,     0,     0,     0,     0,   330,   331,   616,   617,
     618,   619,   620,     0,     0,   621,   622,   623,   624,     0,
     625,   626,   627,   628,   629,     0,   630,   631,     0,     0,
     936,     0,   632,   633,   634,   234,   235,   236,   635,   238,
     239,   240,   241,   242,   437,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   256,   257,   258,
       0,     0,   261,   262,   263,   264,     0,   636,     0,   637,
     638,   639,   640,   641,   642,   643,   644,   645,   646,   614,
     615,     0,     0,     0,     0,     0,     0,     0,     0,   647,
     648,   616,   617,   618,   619,   620,     0,     0,   621,   622,
     623,   624,     0,   625,   626,   627,   628,   629,     0,   630,
     631,     0,     0,     0,     0,   632,   633,   634,   784,     0,
       0,   635,     0,     0,     0,     0,     0,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   994,
     636,   998,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,   614,   615,     0,     0,     0,     0,     0,     0,
       0,     0,   647,   648,   616,   617,   618,   619,   620,     0,
       0,   621,   622,   623,   624,     0,   625,   626,   627,   628,
     629,     0,   630,   631,     0,     0,     0,     0,   632,   633,
     634,   234,   235,   236,   635,   238,   239,   240,   241,   242,
     437,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,   256,   257,   258,     0,     0,   261,   262,
     263,   264,     0,   636,  1138,   637,   638,   639,   640,   641,
     642,   643,   644,   645,   646,   614,   615,     0,     0,     0,
       0,     0,     0,     0,     0,   647,   648,   616,   617,   618,
     619,   620,     0,     0,   621,   622,   623,   624,     0,   625,
     626,   627,   628,   629,     0,   630,   631,   614,   615,     0,
       0,   632,   633,   634,   995,     0,     0,   635,     0,     0,
       0,     0,     0,   996,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   636,     0,   637,   638,
     639,   640,   641,   642,   643,   644,   645,   646,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   647,   648,
     616,   617,   618,   619,   620,     0,     0,   621,   622,   623,
     624,     0,   625,   626,   627,   628,   629,     0,   630,   631,
     614,   615,     0,     0,   632,   633,   634,     0,     0,     0,
    -750,     0,   616,   617,   618,   619,   620,     0,     0,   621,
     622,   623,   624,     0,   625,   626,   627,   628,   629,     0,
     630,   631,   614,   615,     0,     0,   632,   633,   634,   636,
       0,   637,   638,   639,   640,   641,   642,   643,   644,   645,
     646,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   647,   648,     0,     0,     0,     0,     0,     0,     0,
       0,   636,     0,   637,   638,   639,   640,   641,   642,   643,
     644,   645,   646,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   647,   648,   616,   617,   618,   619,   620,
       0,     0,   621,   622,   623,   624,     0,   625,   626,   627,
     628,   629,     0,   630,   631,   614,   615,     0,     0,   632,
       0,   634,     0,     0,     0,     0,     0,   616,   617,   618,
     619,   620,     0,     0,   621,   622,   623,   624,     0,   625,
     626,   627,   628,   629,     0,   630,   631,   614,   615,     0,
       0,   632,     0,     0,     0,     0,   637,   638,   639,   640,
     641,   642,   643,   644,   645,   646,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   647,   648,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   637,   638,
     639,   640,   641,   642,   643,   644,   645,   646,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   647,   648,
     616,   617,   618,   619,   620,     0,     0,   621,   622,   623,
     624,     0,   625,   626,   627,   628,   629,     0,   630,   631,
     614,   615,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   616,   617,   618,   619,   620,     0,     0,   621,
     622,   623,   624,     0,   625,   626,   627,   628,   629,     0,
     630,   631,   614,   615,     0,     0,     0,     0,     0,     0,
       0,     0,   638,   639,   640,   641,   642,   643,   644,   645,
     646,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   647,   648,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   639,   640,   641,   642,   643,
     644,   645,   646,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   647,   648,   616,   617,   618,   619,   620,
       0,     0,   621,   622,   623,   624,     0,   625,   626,   627,
     628,   629,     0,   630,   631,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   616,   617,   618,
     619,   620,     0,     0,   621,     0,     0,   624,     0,   625,
     626,   627,   628,   629,     0,   630,   631,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   640,
     641,   642,   643,   644,   645,   646,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   647,   648,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   640,   641,   642,   643,   644,   645,   646,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   647,   648,
     234,   235,   236,     0,   238,   239,   240,   241,   242,   437,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,   256,   257,   258,     0,     0,   261,   262,   263,
     264,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   819,   820,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   821,     0,     0,     0,     0,     0,     0,
       0,     0,   822,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   823,   824
};

static const yytype_int16 yycheck[] =
{
       1,    14,    15,   176,   138,   139,   377,   162,   539,   508,
     601,   611,   748,   686,   543,   937,    80,   677,   522,    20,
      21,    22,   499,   657,   501,   659,   503,   661,  1073,   769,
     766,   607,   521,   735,  1122,     5,     8,     0,   737,    33,
     778,   551,   413,    20,     7,    20,    20,   129,   130,    50,
      63,    64,    65,    20,    46,     5,     6,    33,  1356,   140,
     141,   142,   464,   165,    22,    19,    20,    30,   163,    32,
     165,    34,   127,   151,  1474,   176,   151,    40,    62,   126,
    1260,   215,   129,   130,   868,   148,   156,    50,   127,   102,
     103,   104,   105,    56,   140,   148,   142,  1420,   200,    57,
     163,   173,   165,     5,     6,  1505,    15,    16,  1420,   204,
     163,  1409,    15,    16,   106,   197,   198,    80,   173,   174,
     201,   176,   154,    25,   179,   203,   201,   199,   203,    31,
     721,   163,   102,   103,   725,   174,  1420,   713,   177,   102,
     103,   204,   733,   206,  1467,   736,   165,   200,  1328,  1329,
     197,   198,   205,   180,     7,  1467,   220,   107,   668,     8,
     139,   173,   564,   176,  1344,  1345,    68,    69,   200,   698,
     127,   174,   574,   200,   127,   577,   133,   163,    21,    22,
     133,   200,   139,  1467,   163,   179,   154,    36,   176,   344,
     173,   851,   768,   165,   565,   163,   173,    50,   173,   173,
     102,   103,   165,   179,   377,   607,   173,   341,   579,   163,
     180,   345,   722,   951,  1394,  1395,   202,   174,   219,   139,
     393,   174,   198,   186,   960,   165,   198,   185,   930,   165,
     200,   207,   931,   201,   206,   198,   148,   139,   177,   197,
     413,   204,   181,   416,   417,   418,   906,   156,   650,  1337,
    1391,   163,   161,   156,   163,   268,   176,   166,   161,  1400,
     163,   163,   173,   166,   204,   127,   127,   127,   204,  1053,
     206,   133,   133,   133,  1319,   118,   119,   411,   180,    33,
     163,    33,   346,   126,  1455,   128,   129,   130,   131,   132,
     201,   200,   393,    57,   967,  1420,   198,  1420,   775,    63,
    1441,  1442,   163,   205,    34,   163,    60,    61,    60,    61,
     173,   713,   174,   174,   174,   416,   417,   418,   179,   173,
    1491,   494,   495,   173,   127,   498,   697,   500,   186,   502,
     133,   504,   173,    63,   173,   163,   846,   127,   201,   710,
     711,   845,  1467,   133,  1467,   139,   922,   201,   521,   720,
     984,   201,   856,   842,   197,   198,   727,   728,   173,   730,
     201,   732,   201,   734,   377,   854,   200,   173,  1041,   154,
     124,   174,   124,   127,   128,   127,   128,   107,   163,   133,
     393,   133,   911,   974,   174,   181,   201,   180,   197,   173,
     173,   173,   565,   494,   495,   201,   173,   498,   173,   500,
     413,   502,   132,   416,   417,   418,   579,   200,   204,   139,
     423,   424,   180,   165,  1198,  1155,   926,   201,   201,   201,
     174,   175,   174,   175,   201,   179,   201,   179,   182,   173,
     182,    33,   200,   163,  1094,   164,   165,   173,  1178,   148,
     950,   614,   615,    57,   173,   173,   177,   163,   177,    63,
     181,   173,   204,   207,   163,   207,   629,   201,    60,    61,
     190,   156,    63,    64,    65,   201,   173,   164,   165,  1142,
     200,  1207,   606,   201,   647,   204,   173,    21,    22,   201,
    1264,   494,   495,   167,   181,   498,   139,   500,  1410,   502,
     139,   504,   199,   173,   971,   148,   164,   670,   163,   148,
     153,   102,   103,   104,   105,   173,  1290,   204,   521,   986,
     163,   154,   204,   205,   163,  1149,   164,   165,   173,   199,
     163,   534,   124,   173,   697,   173,   128,   180,    21,    22,
    1164,   164,   165,   181,    10,    11,   709,   710,   711,   712,
     173,   176,   543,   716,   199,   163,   181,   720,   181,   199,
     180,   154,   565,   163,   727,   728,   204,   730,   929,   732,
     163,   734,   735,   154,   935,   200,   579,   164,   165,   670,
     200,   204,   163,   175,   118,   119,   173,   179,   176,   181,
     182,  1365,   126,   181,   181,   129,   130,   131,   132,   991,
     180,   604,   176,    47,    57,   608,   163,   181,   163,   970,
      63,  1003,   200,   164,   165,   207,  1008,   204,   709,  1119,
     200,   712,   173,    67,   163,   716,   200,    33,   176,    12,
     181,   173,    57,   181,   176,   118,   119,   179,    63,   201,
      23,    24,   204,   126,   807,   128,   129,   130,   131,   132,
    1424,  1425,   200,   204,    60,    61,   154,   165,   177,   205,
    1390,  1391,   181,   197,   198,   163,  1396,   670,    33,    57,
    1400,  1401,   163,   164,   163,   164,   164,   840,   174,   842,
     843,    57,   173,   173,   173,   167,   176,    63,   679,   179,
     681,   854,  1355,   140,   697,    60,    61,   688,   165,   862,
    1189,   184,   185,   186,  1434,   164,   709,   710,   711,   712,
     701,  1441,  1442,   716,   197,   198,   177,   720,   124,   177,
     181,   163,   128,   181,   727,   728,  1500,   730,    57,   732,
      57,   734,   735,   203,    63,    35,    63,   177,   164,   165,
    1514,   181,   177,     5,     6,  1137,   181,   173,  1140,   840,
      35,   875,   843,   164,   165,   181,  1530,   200,  1150,   124,
     177,  1535,   173,   128,   181,   106,   929,   930,   176,   175,
     181,   862,   935,   179,   203,   181,   182,  1507,   204,   164,
     165,   177,   163,   177,   200,   181,   777,   181,   173,  1379,
     180,   177,   205,   204,    33,   181,   181,   202,   203,    33,
     205,   207,   180,   176,  1165,    75,   179,   970,   180,    79,
     175,   167,   168,   169,   179,   177,   181,   182,  1399,   204,
     180,    60,    61,    93,    94,   180,    60,    61,    98,    99,
     100,   101,   167,   168,    33,   180,   200,   840,   180,   842,
     843,   180,   207,   180,    33,   198,  1335,   167,   168,   169,
     170,   854,   140,   141,   142,  1436,   167,   168,   169,   862,
    1381,    60,    61,   854,   180,   856,  1029,    54,    55,    56,
     180,    60,    61,   205,   200,   205,   163,   163,  1270,   163,
     199,   872,    22,   163,   199,   124,   176,   878,   879,   128,
     124,   163,   163,   198,   128,   163,   133,   200,   205,   205,
     891,   176,   163,   894,   895,   896,   897,   898,  1498,   354,
    1429,   902,   203,   180,   180,   180,   180,   200,   200,   364,
     911,   180,   913,   180,   200,   124,   929,   930,   180,   128,
     375,   180,   935,    21,    22,   124,   175,   180,  1029,   128,
     179,   175,   181,   182,   200,   179,   200,   181,   182,  1341,
     200,    13,   200,  1472,  1099,   200,   200,   198,   200,   163,
     200,   163,   163,   201,   199,   164,   165,   970,   207,   200,
      43,   181,   200,   207,   173,   180,   175,   180,   200,    13,
     179,   199,   181,   182,  1376,   180,   175,   173,   200,   173,
     179,   200,   181,   182,   201,     4,   441,   442,   163,   176,
      43,   163,  1165,   203,  1167,   204,   163,   201,   207,   200,
     163,   163,  1136,   458,   459,   460,   461,   462,   207,   173,
     200,   180,   201,   201,   200,   200,  1029,   200,   116,   117,
     118,   119,   120,   200,   180,   123,   124,   125,   126,  1163,
     128,   129,   130,   131,   132,   200,   134,   135,     1,   200,
     200,   200,   140,   141,   142,   200,   200,   163,   146,   181,
     181,   181,   163,   163,   163,   156,    10,  1428,    13,     9,
      42,   206,   517,    66,    43,   201,  1167,   165,   174,   206,
     525,   206,   201,   201,   200,    43,   201,   175,   201,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   201,
     180,   206,   547,   200,    14,   200,   174,   201,   200,   197,
     198,   200,   176,  1104,   200,   200,   204,    21,    22,   206,
     156,   566,   567,   200,   200,   570,   200,   572,   200,   200,
      37,     8,   163,   163,   201,  1298,   171,   582,   583,   584,
     585,   586,   587,   200,   163,   201,   163,   200,   163,   163,
     163,   163,   206,    67,   200,   200,   181,   201,   200,   200,
     200,    70,  1165,   200,  1167,   181,   163,  1291,   200,    12,
     138,   616,   617,   201,   201,   620,   621,   622,   623,   205,
     625,   167,   627,   628,   629,   630,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,   200,   648,   200,   200,   200,  1298,   163,   201,
     201,    33,   116,   117,   118,   119,   120,   204,    33,   123,
     124,   125,   126,   204,   128,   129,   130,   131,   132,   204,
     134,   135,   205,   204,   201,    33,   140,   141,   142,   201,
      21,    22,   146,   688,   173,    60,    61,   201,  1239,   694,
    1241,  1375,   201,  1377,  1378,   700,   206,   205,   201,   201,
     201,   200,    60,    61,   201,  1428,   201,   201,    53,   199,
     715,   175,   200,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   206,   199,   206,   588,    79,     1,    44,
     130,    82,  1537,   197,   198,  1298,   101,   681,   612,   681,
     204,   746,   226,  1533,   608,   907,   751,     1,   753,   124,
     755,   483,   757,   128,  1432,  1389,  1288,  1386,  1323,   530,
    1433,    53,   767,  1094,   906,   423,   124,   906,  1319,    -1,
     128,   338,   777,   423,  1401,    -1,   566,   118,   119,    -1,
      -1,    21,    22,    -1,  1468,   126,    -1,   128,   129,   130,
     131,   132,    -1,  1356,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,   179,    -1,   181,   182,    -1,  1493,
      -1,    -1,    -1,    -1,    -1,   820,    -1,   175,    -1,   824,
      -1,   179,    -1,   181,   182,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,  1522,    -1,
      -1,   182,   183,   184,   185,   186,  1409,    -1,  1532,   207,
      -1,    -1,    -1,    -1,   859,    -1,   197,   198,    -1,   864,
      -1,   866,    -1,    -1,   869,  1428,    -1,    -1,    -1,   874,
    1421,   876,    -1,    -1,    -1,  1426,   116,   117,   118,   119,
     120,    21,    22,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,   912,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   936,   937,    -1,    -1,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,  1499,    -1,
      -1,    -1,  1503,  1504,    -1,    -1,    -1,   197,   198,    -1,
      -1,   201,    -1,   968,   969,  1516,    -1,    -1,  1519,    -1,
      -1,    -1,    -1,    33,   979,    -1,   116,   117,   118,   119,
      -1,    -1,    -1,   988,    -1,   990,   126,   992,   128,   129,
     130,   131,   132,   998,   134,   135,    -1,  1002,    21,    22,
      60,    61,    -1,    -1,    -1,  1010,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1043,    -1,
      -1,  1046,   182,   183,   184,   185,   186,    -1,    -1,  1054,
      60,    61,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   128,    -1,
    1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,
    1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,  1103,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    10,   124,   175,    -1,  1122,   128,   179,
      -1,   181,   182,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1143,  1144,
    1145,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,   175,    -1,    -1,  1173,   179,
    1175,   181,   182,    -1,   197,   198,  1181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1196,    -1,    -1,    -1,  1200,    -1,   207,    -1,    -1,
    1205,  1206,    -1,  1208,  1209,    -1,    -1,    -1,    -1,    21,
      22,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,  1247,   140,   141,   142,   143,   144,   145,   146,
      -1,  1256,  1257,  1258,    -1,    -1,    -1,    -1,  1263,    -1,
      -1,    -1,    -1,    -1,    33,    -1,  1271,  1272,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    -1,  1337,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,  1362,    -1,   128,
      -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,    -1,  1393,   201,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1404,
      -1,  1406,  1407,    -1,    -1,  1410,   175,    -1,  1413,    -1,
     179,    -1,   181,   182,    -1,  1420,    -1,    -1,    -1,    -1,
      -1,  1426,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1451,    -1,    -1,     1,
      -1,    -1,    -1,     5,     6,     7,    -1,     9,    10,    11,
      -1,    13,  1467,    15,    16,    17,    18,    19,  1473,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      42,    -1,    44,    45,    -1,    -1,    48,  1502,    50,    51,
      52,    -1,    54,    55,    -1,  1510,    58,    59,    -1,    -1,
    1515,  1516,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,
      -1,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,   183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,   200,    -1,
     202,   203,   204,   205,   206,     1,    -1,    -1,    -1,     5,
       6,     7,    -1,     9,    10,    11,    -1,    13,    -1,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    39,    -1,    -1,    42,    -1,    44,    45,
      -1,    -1,    48,    -1,    50,    51,    52,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,   148,   149,   150,   151,    -1,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,
     186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   198,    -1,   200,    -1,   202,   203,   204,   205,
     206,     1,    -1,    -1,    -1,     5,     6,     7,    -1,     9,
      10,    11,    -1,    13,    -1,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    -1,    42,    -1,    44,    45,    -1,    -1,    48,    -1,
      50,    51,    52,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
     150,   151,    -1,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,
     190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,
     200,    -1,   202,   203,   204,   205,   206,     1,    -1,    -1,
      -1,     5,     6,     7,    -1,     9,    10,    11,    -1,    13,
      -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    29,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    42,    -1,
      44,    45,    -1,    -1,    48,    -1,    50,    51,    52,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,   165,   166,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,
     184,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,
      -1,    60,    61,    -1,   198,    -1,   200,    -1,   202,   203,
     204,   205,   206,     5,     6,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      52,    -1,    -1,    55,    -1,   124,    -1,    -1,    -1,   128,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,    -1,    -1,   175,    -1,    -1,    -1,
     179,    -1,   181,   182,    -1,    -1,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   207,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,
      -1,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,   183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,
      -1,    -1,    -1,    -1,     5,     6,   198,    -1,   200,    -1,
     202,   203,    -1,   205,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    70,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,   190,
      -1,    -1,    -1,    -1,    -1,     5,     6,   198,    -1,   200,
     201,   202,   203,    13,   205,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,
     190,    -1,    -1,    -1,    -1,    -1,     5,     6,   198,    -1,
     200,    -1,   202,   203,    13,   205,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    33,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,   151,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,   179,    -1,   181,
     182,    -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,
     189,   190,    -1,    -1,    -1,    -1,    -1,     5,     6,   198,
      -1,   200,    -1,   202,   203,   207,   205,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    33,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    70,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
     148,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,   179,    -1,
      -1,   182,    -1,    -1,   182,   183,   184,    -1,   186,    -1,
      -1,   189,   190,    -1,    -1,    -1,    -1,    -1,     5,     6,
     198,    -1,   200,    -1,   202,   203,   207,   205,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,   150,   151,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,
      -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,     5,
       6,   198,   199,   200,    -1,   202,   203,    -1,   205,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,   148,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,
     186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,
       5,     6,   198,    -1,   200,    -1,   202,   203,    -1,   205,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,   150,   151,    -1,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,
      -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,
      -1,     5,     6,   198,   199,   200,    -1,   202,   203,    -1,
     205,    15,    16,    17,    18,    19,    -1,    -1,    22,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,
     184,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,
      -1,    -1,     5,     6,   198,    -1,   200,    -1,   202,   203,
      -1,   205,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,
     183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,
      -1,    -1,    -1,     5,     6,   198,    -1,   200,   201,   202,
     203,    -1,   205,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,
      -1,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,   183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,
      -1,    -1,    -1,    -1,     5,     6,   198,    -1,   200,    -1,
     202,   203,    -1,   205,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,
     151,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,   190,
      -1,    -1,    -1,    -1,    -1,     5,     6,   198,    -1,   200,
      -1,   202,   203,    -1,   205,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
     150,   151,    -1,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,
     190,    -1,    -1,    -1,    -1,    -1,     5,     6,   198,    -1,
     200,    -1,   202,   203,    -1,   205,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,   151,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,
     189,   190,    -1,    -1,    -1,    -1,    -1,     5,     6,   198,
      -1,   200,    -1,   202,   203,    -1,   205,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
     148,   149,   150,   151,    -1,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,    -1,
      -1,   189,   190,    -1,    -1,    -1,    -1,    -1,     5,     6,
     198,    -1,   200,   201,   202,   203,    -1,   205,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,   150,   151,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,
      -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,     5,
       6,   198,    -1,   200,   201,   202,   203,    -1,   205,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,   148,   149,   150,   151,    -1,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,
     186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,
       5,     6,   198,    -1,   200,   201,   202,   203,    -1,   205,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,   150,   151,    -1,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,
      -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,
      -1,     5,     6,   198,    -1,   200,   201,   202,   203,    -1,
     205,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,
     184,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,
      -1,    -1,     5,     6,   198,    -1,   200,   201,   202,   203,
      -1,   205,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,
     183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,
      -1,    -1,    -1,     5,     6,   198,    -1,   200,    -1,   202,
     203,    -1,   205,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,
      -1,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,   183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,
      -1,    -1,    -1,    -1,     5,     6,   198,    -1,   200,    -1,
     202,   203,    -1,   205,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,
     151,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    19,   166,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,   182,   183,   184,    -1,   186,    41,    -1,   189,   190,
      -1,    -1,    -1,    -1,    49,    -1,    -1,   198,    -1,   200,
      -1,   202,   203,    -1,   205,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   163,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,   202,    -1,    -1,
      -1,    -1,   207,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,   163,    -1,   165,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
     202,    -1,   204,    21,    22,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,   163,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,   202,    -1,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    -1,   139,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   197,
     198,    -1,    -1,   201,    -1,    -1,    -1,    -1,    -1,   202,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,   134,   135,    -1,    -1,
      -1,    -1,   140,   141,   142,    71,    72,    73,   146,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,    -1,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   197,
     198,    -1,    -1,   201,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,   154,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,
     201,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,
      -1,    -1,   201,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
     197,   198,    -1,    -1,   201,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,
     201,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,
      -1,    -1,   201,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
     197,   198,    -1,    -1,   201,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,
     201,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,
      -1,    -1,   201,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
     197,   198,    -1,    -1,   201,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,
     201,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,
      -1,    -1,   201,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
     197,   198,    -1,    -1,   201,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,
     201,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,
      -1,    -1,   201,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
     197,   198,    -1,    -1,   201,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,   197,   198,    -1,    -1,
     201,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,   198,
      -1,    -1,   201,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,   197,   198,    -1,    -1,   201,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
     197,   198,    -1,    -1,   201,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,   199,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,   199,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      21,    22,    35,    -1,    -1,    -1,    -1,    -1,   197,   198,
     199,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    71,    -1,
      73,   146,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,   197,   198,   199,   116,   117,   118,   119,   120,
      21,    22,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    38,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,    -1,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    21,    22,   189,   190,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,   134,   135,    -1,    -1,
     138,    -1,   140,   141,   142,    71,    72,    73,   146,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,    -1,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,   154,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,
     142,    71,    72,    73,   146,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,    -1,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    21,    22,    -1,
      -1,   140,   141,   142,   154,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      21,    22,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    21,    22,    -1,    -1,   140,   141,   142,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    21,    22,    -1,    -1,   140,
      -1,   142,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    21,    22,    -1,
      -1,   140,    -1,    -1,    -1,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,    -1,    -1,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   209,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   186,   198,   204,   211,   212,   215,
     224,   226,   227,   231,   271,   275,   304,   382,   390,   397,
     407,   454,   459,   464,    19,    20,   163,   262,   263,   264,
     156,   232,   233,   163,   186,   228,   229,    57,    63,   387,
     388,   163,   202,   214,   465,   455,   460,   139,   163,   292,
      34,    63,   107,   132,   190,   200,   266,   267,   268,   269,
     292,   211,   211,   211,     8,    36,   408,    62,   378,   174,
     173,   176,   173,   228,    22,    57,   185,   197,   230,   163,
     211,   378,   387,   387,   387,   163,   139,   225,   268,   268,
     268,   200,   140,   141,   142,   173,   199,    57,    63,   276,
     278,   398,     5,     6,   107,   404,    57,    63,   379,    15,
      16,   156,   161,   163,   166,   200,   217,   263,   156,   233,
     163,   163,   163,   389,    57,    63,   213,   163,   163,   163,
     163,   167,   223,   201,   264,   268,   268,   268,   268,   165,
     237,   238,   279,    57,    63,   391,     5,     6,    57,    63,
     405,   205,   383,   167,   168,   169,   216,    15,    16,   156,
     161,   163,   217,   260,   261,   230,   174,   164,   395,   396,
     238,   238,   167,   201,   165,    35,    71,    73,    75,    76,
      77,    78,    79,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    93,    94,    95,    98,    99,   100,
     101,   118,   119,   163,   274,   277,   163,   392,   409,   380,
     203,   265,   351,   167,   168,   169,   173,   201,    19,    25,
      31,    41,    49,    64,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   152,   202,   292,
     412,   414,   415,   418,   424,   425,   453,   164,   466,   456,
     461,    21,    22,    38,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   128,   129,   130,   133,   134,   135,   136,   137,
     140,   141,   142,   143,   144,   145,   146,   175,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   189,   190,
     197,   198,    35,    35,   200,   272,   238,   176,   393,   106,
     402,   403,   206,   211,   381,   263,   203,   163,   374,   377,
     260,   180,   180,   180,   200,   180,   180,   200,   180,   180,
     180,   180,   180,   180,   200,   292,    33,    60,    61,   124,
     128,   175,   179,   182,   207,   198,   423,   177,   205,   205,
     205,   163,   163,   163,   199,    22,   163,   199,   151,   201,
     351,   361,   362,   176,   273,   283,   284,    75,    79,    93,
      94,    98,    99,   100,   101,   413,   396,   163,   238,   351,
     238,   263,   173,   176,   179,   372,   426,   431,   433,     5,
       6,    15,    16,    17,    18,    19,    25,    27,    31,    39,
      45,    48,    51,    55,    65,    68,    69,    80,   102,   103,
     104,   118,   119,   147,   148,   149,   150,   151,   153,   155,
     156,   157,   158,   159,   160,   161,   162,   166,   182,   183,
     184,   189,   190,   198,   200,   202,   203,   205,   222,   224,
     286,   292,   297,   309,   316,   319,   322,   326,   328,   330,
     331,   333,   338,   341,   342,   349,   350,   412,   469,   477,
     487,   490,   502,   506,   435,   429,   163,   419,   437,   439,
     441,   443,   445,   447,   449,   451,   342,   180,   200,    33,
     179,    33,   179,   198,   207,   199,   342,   198,   207,   424,
     467,   457,   462,   163,   133,   200,     7,    50,   303,   201,
     204,   453,   205,   205,   176,   401,   410,   148,   163,   373,
     376,   238,   163,   424,   127,   133,   174,   371,   453,   453,
     422,   453,   180,   180,   180,   294,   414,   469,   292,   180,
       5,   102,   103,   180,   200,   180,   200,   200,   180,   180,
     200,   180,   200,   180,   200,   180,   180,   200,   180,   180,
     342,   342,   200,   200,   200,   200,   200,   200,   221,   342,
     342,   342,   342,   342,    13,    49,   289,   320,   342,   154,
     163,   320,   470,   472,   203,    13,   503,   200,   198,   270,
     203,   205,   322,   327,    21,    22,   116,   117,   118,   119,
     120,   123,   124,   125,   126,   128,   129,   130,   131,   132,
     134,   135,   140,   141,   142,   146,   175,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   197,   198,   201,
     200,   453,   453,   201,   163,   416,   453,   272,   453,   272,
     453,   272,   163,   363,   364,   453,   163,   366,   367,   201,
     428,   289,   199,   199,   199,   342,   163,   417,   363,   365,
     366,   368,   342,   148,   163,   370,   411,   361,   288,   399,
     292,   204,   205,   406,   200,    43,   173,   176,   372,   211,
     371,   342,   181,   181,   181,   164,   173,   210,   211,   421,
     478,   480,   295,   200,   180,   200,   317,   180,   180,   180,
     497,   320,   424,   342,   486,   501,   342,   310,   312,   342,
     314,   342,   499,   320,   484,   488,   320,   482,   424,   342,
     342,   342,   342,   342,   342,   169,   170,   216,   200,    13,
     199,   173,   505,   200,   127,   133,   174,   369,   505,   173,
     201,   148,   153,   180,   292,   332,   200,   238,    70,   198,
     201,   320,   472,   269,     4,   325,   203,   288,   270,    19,
     154,   163,   412,    19,   154,   163,   412,   342,   342,   342,
     342,   342,   342,   163,   342,   154,   163,   342,   342,   342,
     412,   342,   342,   342,   342,   342,   342,    22,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   129,
     130,   154,   163,   197,   198,   339,   412,   342,   201,   320,
     181,   181,   181,   204,   181,   273,   181,   273,   181,   273,
     176,   181,   421,   176,   181,   421,   291,   453,   201,   199,
     210,   385,   394,   211,   243,   244,   243,   244,   201,   200,
      43,   173,   176,   179,   369,     1,    26,    28,    29,    38,
      44,    52,    54,    58,    59,    65,   105,   211,   234,   235,
     242,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   285,   287,   293,   298,   299,   300,   301,   302,
     304,   308,   329,   342,   163,   384,   386,   357,   342,   163,
     163,   424,   371,   342,   211,   427,   432,   434,   453,   424,
     424,   453,    70,   320,   472,   476,   163,   342,   453,   491,
     493,   495,   424,   505,   181,   421,   138,   173,   505,   201,
     424,   424,   201,   424,   201,   424,   505,   424,   364,   505,
     367,   181,   201,   201,   201,   201,   201,   201,   342,   411,
     200,   342,   342,   342,   342,   201,   154,   163,   200,   200,
     334,   336,   411,   290,   501,   163,   201,   472,   470,   173,
     201,   201,   199,   200,   272,   206,   325,   180,   200,   180,
     200,   200,   200,   199,    19,   154,   163,   412,   176,   154,
     163,   342,   200,   200,   154,   163,   342,     1,   200,   199,
     173,   201,   436,   430,   420,   163,   438,   181,   442,   181,
     446,   181,   453,   450,   363,   453,   452,   366,   181,   421,
     163,   210,   468,   211,   363,   458,   366,   463,   342,   163,
     163,   453,   342,   127,   342,   283,    61,   342,   163,   211,
     156,    58,   342,   238,   127,   342,   211,   211,    10,    10,
      11,   240,    13,     9,    42,   211,   206,   211,   211,   211,
     211,   211,    66,   305,   211,   108,   109,   110,   111,   112,
     113,   114,   115,   121,   122,   127,   133,   136,   137,   143,
     144,   145,   174,   174,   385,   394,   165,   206,   271,   358,
     201,    43,   211,   371,   342,   211,   181,   181,   181,   472,
     201,   201,   201,   181,   421,   201,   181,   424,   364,   367,
     181,   201,   200,   424,   342,   486,   201,   181,   181,   181,
     181,   201,   181,   181,   201,   181,   325,   200,   176,   219,
      20,   411,   201,   200,   133,   369,   342,   342,   424,   272,
      20,   206,   505,   201,   199,   198,   127,   133,   163,   174,
     179,   323,   324,   273,   272,   343,   342,   345,   342,   201,
     320,   342,   180,   200,   342,   200,   199,   342,   198,   201,
     320,   200,   199,   340,   201,   320,   440,   444,   448,   200,
     453,   206,   206,   206,   201,    43,   369,   342,    14,   342,
     174,   176,   156,   283,   342,   200,   200,   200,   200,   200,
      37,   280,   200,   205,   307,   376,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   384,   400,     8,   351,   356,
     163,   342,   211,   479,   481,   296,   201,   200,   163,   318,
     181,   181,   181,   498,   290,   181,   311,   313,   315,   500,
     485,   489,   483,   200,   238,   201,   320,   220,   171,   320,
      20,   133,   369,   342,   342,   342,   201,   201,   181,   273,
     320,   201,   470,   163,   163,   200,   163,   163,   173,   201,
     238,   273,   424,   201,   453,   201,   201,   201,   347,   342,
     342,   201,   470,   201,   342,   201,   289,   163,   342,   283,
     342,   342,   342,   411,   342,   342,   281,   163,   306,   375,
     206,    57,    63,   354,    67,   355,   211,   211,   200,   200,
     342,   181,   492,   494,   496,   200,   201,   200,   342,   342,
     342,   200,    70,   476,   200,   200,   201,   342,   283,   201,
     218,   201,   320,   342,   342,   201,   335,   181,   201,   199,
     163,   323,   138,   283,   321,   238,   181,   181,   453,   201,
     201,   199,   201,   201,   201,   201,    20,   201,   201,   205,
     173,   201,   206,   211,   376,    47,   355,    46,   106,   352,
     476,   476,   201,   200,   200,   200,   200,   289,   290,   320,
     476,   476,   201,   167,   204,   201,   133,   369,   332,   337,
     204,   201,   201,   127,   342,   283,   344,   346,   181,   201,
      12,   241,   238,   320,   238,   238,   288,   163,   176,   372,
      33,   353,   352,   354,   200,   470,   473,   474,   475,   475,
     342,   476,   476,   470,   471,   201,   201,   505,   475,   471,
     342,   204,   342,   342,   332,   486,   342,   348,   242,   298,
     299,   300,   301,   342,   211,   245,   246,   248,   201,   283,
     283,   282,   424,   371,   359,   353,   370,   470,   173,   505,
     201,   201,   201,   475,   475,   201,   201,   201,   201,   204,
     504,   342,   504,    12,    23,    24,   236,   239,   205,   242,
     238,   206,   371,   342,   277,   360,   201,   200,   201,   201,
      53,   199,   504,   206,   238,   200,   288,   211,   283,   342,
     211,   211,   277,   470,   342,   199,   283,   342,   247,   211,
     238,   201,   201,   206,   283,   238,   280,   283,   239
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   208,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   210,   210,   211,
     211,   212,   213,   213,   213,   214,   214,   215,   216,   216,
     216,   216,   217,   217,   218,   218,   219,   220,   219,   221,
     221,   221,   222,   223,   223,   225,   224,   226,   227,   228,
     228,   228,   228,   229,   229,   230,   230,   231,   232,   232,
     233,   233,   234,   235,   235,   236,   236,   237,   237,   238,
     238,   239,   239,   239,   240,   240,   241,   241,   242,   242,
     242,   242,   242,   243,   243,   244,   244,   246,   247,   245,
     248,   245,   250,   249,   251,   253,   252,   254,   256,   255,
     258,   257,   259,   260,   260,   260,   260,   260,   260,   261,
     261,   262,   262,   262,   263,   263,   263,   263,   263,   263,
     263,   263,   264,   264,   265,   265,   266,   266,   266,   266,
     267,   267,   268,   268,   268,   268,   268,   268,   268,   269,
     269,   270,   270,   271,   271,   272,   272,   272,   273,   273,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   275,   276,   276,   276,   277,
     279,   278,   280,   281,   282,   280,   284,   285,   283,   286,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   288,
     288,   288,   289,   289,   290,   290,   291,   291,   292,   292,
     292,   293,   293,   295,   296,   294,   294,   297,   297,   297,
     297,   297,   297,   298,   299,   300,   300,   300,   301,   301,
     302,   303,   303,   303,   304,   304,   305,   305,   306,   306,
     307,   307,   308,   308,   308,   310,   311,   309,   312,   313,
     309,   314,   315,   309,   317,   318,   316,   319,   319,   319,
     320,   320,   321,   321,   321,   322,   322,   322,   323,   323,
     323,   323,   323,   324,   324,   325,   325,   326,   327,   327,
     328,   328,   328,   328,   328,   328,   328,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   330,   330,   331,   331,
     332,   332,   333,   334,   335,   333,   336,   337,   333,   338,
     338,   338,   338,   338,   338,   338,   339,   340,   338,   341,
     341,   341,   341,   341,   341,   341,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   343,   344,   342,   342,   342,   342,   345,   346,
     342,   342,   342,   347,   348,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   349,
     349,   349,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   351,   351,
     352,   352,   352,   353,   353,   354,   354,   354,   355,   355,
     356,   357,   357,   358,   357,   359,   357,   360,   357,   361,
     361,   362,   362,   363,   363,   364,   364,   365,   365,   365,
     366,   367,   367,   368,   368,   368,   369,   369,   370,   370,
     370,   370,   370,   371,   371,   371,   372,   372,   373,   373,
     373,   373,   373,   374,   374,   375,   375,   375,   376,   376,
     376,   377,   377,   377,   378,   378,   379,   379,   379,   380,
     380,   381,   380,   382,   383,   382,   384,   384,   385,   385,
     386,   386,   386,   387,   387,   387,   389,   388,   390,   391,
     391,   391,   392,   393,   393,   394,   394,   395,   395,   396,
     396,   398,   399,   400,   397,   401,   401,   402,   402,   403,
     404,   404,   404,   404,   405,   405,   405,   406,   406,   408,
     409,   410,   407,   411,   411,   411,   411,   411,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   413,   413,   413,   413,   413,
     413,   413,   413,   414,   415,   415,   415,   416,   416,   417,
     417,   417,   419,   420,   418,   421,   421,   422,   422,   423,
     423,   423,   423,   424,   424,   425,   425,   425,   425,   426,
     427,   425,   425,   425,   428,   425,   425,   425,   425,   425,
     425,   425,   425,   425,   425,   425,   425,   425,   429,   430,
     425,   425,   431,   432,   425,   433,   434,   425,   435,   436,
     425,   425,   437,   438,   425,   439,   440,   425,   425,   441,
     442,   425,   443,   444,   425,   425,   445,   446,   425,   447,
     448,   425,   449,   450,   425,   451,   452,   425,   453,   453,
     453,   455,   456,   457,   458,   454,   460,   461,   462,   463,
     459,   465,   466,   467,   468,   464,   469,   469,   469,   469,
     469,   470,   470,   470,   470,   470,   470,   470,   470,   471,
     471,   472,   473,   473,   474,   474,   475,   475,   476,   476,
     478,   479,   477,   480,   481,   477,   482,   483,   477,   484,
     485,   477,   486,   486,   487,   488,   489,   487,   490,   491,
     492,   490,   493,   494,   490,   495,   496,   490,   490,   497,
     498,   490,   490,   499,   500,   490,   501,   501,   503,   502,
     502,   502,   502,   504,   504,   505,   505,   506,   506,   506
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     3,
       3,     2,     2,     2,     2,     2,     2,     1,     1,     1,
       1,     2,     0,     1,     1,     1,     1,     4,     1,     1,
       2,     2,     3,     2,     0,     2,     0,     0,     3,     0,
       2,     5,     3,     1,     2,     0,     4,     2,     2,     1,
       2,     3,     3,     2,     4,     0,     1,     2,     1,     3,
       1,     3,     3,     3,     2,     1,     1,     1,     2,     0,
       1,     0,     3,     7,     1,     1,     0,     2,     1,     1,
       1,     1,     1,     1,     2,     0,     1,     0,     0,     6,
       0,     3,     0,     8,     7,     0,     9,     3,     0,     7,
       0,     7,     4,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       1,     5,     1,     3,     3,     4,     1,     1,     1,     1,
       1,     4,     1,     2,     3,     3,     3,     3,     2,     1,
       3,     0,     3,     0,     4,     0,     2,     3,     0,     2,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     2,     2,     3,     4,
       3,     2,     2,     2,     2,     2,     3,     3,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     1,     1,     3,
       0,     5,     0,     0,     0,     6,     0,     0,     6,     2,
       1,     2,     2,     1,     1,     1,     1,     2,     1,     2,
       2,     2,     2,     1,     1,     1,     2,     2,     2,     0,
       2,     2,     0,     2,     0,     2,     1,     3,     1,     3,
       2,     2,     3,     0,     0,     5,     1,     2,     5,     5,
       5,     6,     2,     1,     1,     1,     2,     3,     2,     3,
       4,     1,     1,     0,     1,     1,     1,     0,     1,     3,
       8,     7,     3,     3,     5,     0,     0,     7,     0,     0,
       7,     0,     0,     7,     0,     0,     6,     5,     8,    10,
       1,     3,     1,     2,     3,     1,     1,     2,     2,     2,
       2,     2,     4,     1,     3,     0,     4,     7,     7,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     6,     8,     5,     6,
       1,     4,     3,     0,     0,     8,     0,     0,     9,     3,
       4,     5,     6,     8,     5,     6,     0,     0,     5,     3,
       4,     4,     5,     4,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     4,     3,     4,     5,
       4,     5,     3,     4,     1,     1,     2,     4,     4,     1,
       3,     5,     0,     0,     8,     3,     3,     3,     0,     0,
       8,     3,     4,     0,     0,     9,     4,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     1,     4,     7,
       8,     7,     4,     4,     4,     4,     4,     1,     6,     7,
       6,     6,     7,     7,     6,     7,     6,     6,     0,     1,
       0,     1,     1,     0,     1,     0,     1,     1,     0,     1,
       5,     0,     2,     0,     4,     0,     9,     0,    11,     3,
       4,     1,     3,     1,     3,     1,     3,     0,     1,     3,
       3,     1,     3,     0,     1,     3,     1,     1,     1,     2,
       3,     5,     3,     1,     1,     1,     0,     1,     1,     4,
       3,     3,     5,     1,     3,     0,     2,     2,     4,     6,
       5,     4,     6,     5,     0,     1,     0,     1,     1,     0,
       2,     0,     4,     6,     0,     6,     1,     3,     1,     2,
       0,     1,     3,     0,     1,     1,     0,     5,     3,     0,
       1,     1,     1,     0,     2,     0,     1,     1,     2,     0,
       1,     0,     0,     0,    13,     0,     2,     0,     1,     3,
       1,     1,     2,     2,     0,     1,     1,     1,     3,     0,
       0,     0,     9,     1,     4,     3,     3,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     1,     3,     0,
       1,     3,     0,     0,     6,     1,     1,     1,     3,     3,
       2,     4,     3,     1,     2,     1,     1,     1,     1,     0,
       0,     6,     4,     5,     0,     9,     4,     2,     2,     3,
       2,     3,     2,     2,     3,     3,     3,     2,     0,     0,
       6,     2,     0,     0,     6,     0,     0,     6,     0,     0,
       6,     1,     0,     0,     6,     0,     0,     7,     1,     0,
       0,     6,     0,     0,     7,     1,     0,     0,     6,     0,
       0,     7,     0,     0,     6,     0,     0,     6,     1,     3,
       3,     0,     0,     0,     0,    12,     0,     0,     0,     0,
      12,     0,     0,     0,     0,    12,     1,     1,     1,     1,
       1,     3,     3,     5,     5,     6,     6,     8,     8,     0,
       1,     1,     3,     5,     1,     2,     1,     0,     0,     1,
       0,     0,    10,     0,     0,    10,     0,     0,    10,     0,
       0,     7,     3,     1,     5,     0,     0,    10,     3,     0,
       0,    11,     0,     0,    11,     0,     0,    10,     5,     0,
       0,     9,     5,     0,     0,    10,     1,     3,     0,     5,
       5,     7,     9,     0,     3,     0,     1,    11,    12,    11
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
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_string_builder: /* string_builder  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_reader: /* expr_reader  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_require_module_name: /* require_module_name  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_expression_label: /* expression_label  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_goto: /* expression_goto  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_else: /* expression_else  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_else_one_liner: /* expression_else_one_liner  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_if_one_liner: /* expression_if_one_liner  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_if_block: /* expression_if_block  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_if_then_else: /* expression_if_then_else  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_if_then_else_oneliner: /* expression_if_then_else_oneliner  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_for_loop: /* expression_for_loop  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_unsafe: /* expression_unsafe  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_while_loop: /* expression_while_loop  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_with: /* expression_with  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_with_alias: /* expression_with_alias  */
            { delete ((*yyvaluep).pExpression); }
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

    case YYSYMBOL_annotation_declaration_name: /* annotation_declaration_name  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_annotation_declaration_basic: /* annotation_declaration_basic  */
            { delete ((*yyvaluep).fa); }
        break;

    case YYSYMBOL_annotation_declaration: /* annotation_declaration  */
            { delete ((*yyvaluep).fa); }
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
            { delete ((*yyvaluep).pTypeDecl); }
        break;

    case YYSYMBOL_function_name: /* function_name  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_function_declaration_header: /* function_declaration_header  */
            { ((*yyvaluep).pFuncDecl)->delRef(); }
        break;

    case YYSYMBOL_function_declaration: /* function_declaration  */
            { ((*yyvaluep).pFuncDecl)->delRef(); }
        break;

    case YYSYMBOL_expression_block_finally: /* expression_block_finally  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_block: /* expression_block  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_call_pipe: /* expr_call_pipe  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_any: /* expression_any  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expressions: /* expressions  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_optional_expr_list: /* optional_expr_list  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_optional_expr_map_tuple_list: /* optional_expr_map_tuple_list  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_type_declaration_no_options_list: /* type_declaration_no_options_list  */
            { deleteTypeDeclarationList(((*yyvaluep).pTypeDeclList)); }
        break;

    case YYSYMBOL_name_in_namespace: /* name_in_namespace  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_expression_delete: /* expression_delete  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_new_type_declaration: /* new_type_declaration  */
            { delete ((*yyvaluep).pTypeDecl); }
        break;

    case YYSYMBOL_expr_new: /* expr_new  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_break: /* expression_break  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_continue: /* expression_continue  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_return: /* expression_return  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_yield: /* expression_yield  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_try_catch: /* expression_try_catch  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_tuple_expansion: /* tuple_expansion  */
            { delete ((*yyvaluep).pNameList); }
        break;

    case YYSYMBOL_tuple_expansion_variable_declaration: /* tuple_expansion_variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_expression_let: /* expression_let  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_cast: /* expr_cast  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_type_decl: /* expr_type_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_type_info: /* expr_type_info  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_list: /* expr_list  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_block_or_simple_block: /* block_or_simple_block  */
            { delete ((*yyvaluep).pExpression); }
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
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_full_block_assumed_piped: /* expr_full_block_assumed_piped  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_numeric_const: /* expr_numeric_const  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_assign: /* expr_assign  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_named_call: /* expr_named_call  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_method_call: /* expr_method_call  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_func_addr_name: /* func_addr_name  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_func_addr_expr: /* func_addr_expr  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_field: /* expr_field  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_call: /* expr_call  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr: /* expr  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_generator: /* expr_generator  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_mtag: /* expr_mtag  */
            { delete ((*yyvaluep).pExpression); }
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

    case YYSYMBOL_function_argument_declaration: /* function_argument_declaration  */
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
            { if ( ((*yyvaluep).pEnum)->use_count()==1 ) delete ((*yyvaluep).pEnum); }
        break;

    case YYSYMBOL_enum_name: /* enum_name  */
            { if ( ((*yyvaluep).pEnum)->use_count()==1 ) delete ((*yyvaluep).pEnum); }
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
            { delete ((*yyvaluep).pTypeDecl); }
        break;

    case YYSYMBOL_auto_type_declaration: /* auto_type_declaration  */
            { delete ((*yyvaluep).pTypeDecl); }
        break;

    case YYSYMBOL_bitfield_bits: /* bitfield_bits  */
            { delete ((*yyvaluep).pNameList); }
        break;

    case YYSYMBOL_bitfield_alias_bits: /* bitfield_alias_bits  */
            { delete ((*yyvaluep).pNameList); }
        break;

    case YYSYMBOL_bitfield_type_declaration: /* bitfield_type_declaration  */
            { delete ((*yyvaluep).pTypeDecl); }
        break;

    case YYSYMBOL_table_type_pair: /* table_type_pair  */
            { delete ((*yyvaluep).aTypePair).firstType; delete ((*yyvaluep).aTypePair).secondType; }
        break;

    case YYSYMBOL_dim_list: /* dim_list  */
            { delete ((*yyvaluep).pTypeDecl); }
        break;

    case YYSYMBOL_type_declaration_no_options: /* type_declaration_no_options  */
            { delete ((*yyvaluep).pTypeDecl); }
        break;

    case YYSYMBOL_type_declaration_no_options_no_dim: /* type_declaration_no_options_no_dim  */
            { delete ((*yyvaluep).pTypeDecl); }
        break;

    case YYSYMBOL_type_declaration: /* type_declaration  */
            { delete ((*yyvaluep).pTypeDecl); }
        break;

    case YYSYMBOL_make_decl: /* make_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_struct_fields: /* make_struct_fields  */
            { delete ((*yyvaluep).pMakeStruct); }
        break;

    case YYSYMBOL_make_variant_dim: /* make_variant_dim  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_struct_single: /* make_struct_single  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_struct_dim_list: /* make_struct_dim_list  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_struct_dim_decl: /* make_struct_dim_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_optional_make_struct_dim_decl: /* optional_make_struct_dim_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_struct_decl: /* make_struct_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_map_tuple: /* make_map_tuple  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_tuple_call: /* make_tuple_call  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_dim_decl: /* make_dim_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_map_tuple_list: /* expr_map_tuple_list  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_table_decl: /* make_table_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_array_comprehension_where: /* array_comprehension_where  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_array_comprehension: /* array_comprehension  */
            { delete ((*yyvaluep).pExpression); }
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
                das2_yyerror(scanner,"module name has to be first declaration",tokAt(scanner,(yylsp[0])), CompilationError::syntax_error);
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
        delete (yyvsp[-1].pExpression);    // we do nothing, we don't even attemp to 'visit'
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

  case 27: /* module_declaration: "module" module_name optional_shared optional_public_or_private_module  */
                                                                                                      {
        yyextra->g_Program->thisModuleName = *(yyvsp[-2].s);
        yyextra->g_Program->thisModule->isPublic = (yyvsp[0].b);
        yyextra->g_Program->thisModule->isModule = true;
        if ( yyextra->g_Program->thisModule->name.empty() ) {
            yyextra->g_Program->library.renameModule(yyextra->g_Program->thisModule.get(),*(yyvsp[-2].s));
        } else if ( yyextra->g_Program->thisModule->name != *(yyvsp[-2].s) ){
            das2_yyerror(scanner,"this module already has a name " + yyextra->g_Program->thisModule->name,tokAt(scanner,(yylsp[-2])),
                CompilationError::module_already_has_a_name);
        }
        if ( !yyextra->g_Program->policies.ignore_shared_modules ) {
            yyextra->g_Program->promoteToBuiltin = (yyvsp[-1].b);
        }
        delete (yyvsp[-2].s);
    }
    break;

  case 28: /* character_sequence: STRING_CHARACTER  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
    break;

  case 29: /* character_sequence: STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
    break;

  case 30: /* character_sequence: character_sequence STRING_CHARACTER  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
    break;

  case 31: /* character_sequence: character_sequence STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
    break;

  case 32: /* string_constant: "start of the string" character_sequence "end of the string"  */
                                                           { (yyval.s) = (yyvsp[-1].s); }
    break;

  case 33: /* string_constant: "start of the string" "end of the string"  */
                                                           { (yyval.s) = new string(); }
    break;

  case 34: /* format_string: %empty  */
        { (yyval.s) = new string(); }
    break;

  case 35: /* format_string: format_string STRING_CHARACTER  */
                                                 { (yyval.s) = (yyvsp[-1].s); (yyvsp[-1].s)->push_back((yyvsp[0].ch)); }
    break;

  case 36: /* optional_format_string: %empty  */
        { (yyval.s) = new string(""); }
    break;

  case 37: /* $@1: %empty  */
            { das2_strfmt(scanner); }
    break;

  case 38: /* optional_format_string: ':' $@1 format_string  */
                                                         { (yyval.s) = (yyvsp[0].s); }
    break;

  case 39: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 40: /* string_builder_body: string_builder_body character_sequence  */
                                                                                  {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das2_yyerror(scanner,"invalid escape sequence",tokAt(scanner,(yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt(scanner,(yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 41: /* string_builder_body: string_builder_body "{" expr optional_format_string "}"  */
                                                                                                                                     {
        auto se = (yyvsp[-2].pExpression);
        if ( !(yyvsp[-1].s)->empty() ) {
            auto call_fmt = new ExprCall(tokAt(scanner,(yylsp[-1])), "_::fmt");
            call_fmt->arguments.push_back(make_smart<ExprConstString>(tokAt(scanner,(yylsp[-1])),":" + *(yyvsp[-1].s)));
            call_fmt->arguments.push_back(se);
            se = call_fmt;
        }
        static_cast<ExprStringBuilder *>((yyvsp[-4].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-4].pExpression);
        delete (yyvsp[-1].s);
    }
    break;

  case 42: /* string_builder: "start of the string" string_builder_body "end of the string"  */
                                                                   {
        auto strb = static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression));
        if ( strb->elements.size()==0 ) {
            (yyval.pExpression) = new ExprConstString(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),"");
            delete (yyvsp[-1].pExpression);
        } else if ( strb->elements.size()==1 && strb->elements[0]->rtti_isStringConstant() ) {
            auto sconst = static_pointer_cast<ExprConstString>(strb->elements[0]);
            (yyval.pExpression) = new ExprConstString(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),sconst->text);
            delete (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    }
    break;

  case 43: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 44: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 45: /* $@2: %empty  */
                                        {
        auto macros = yyextra->g_Program->getReaderMacro(*(yyvsp[0].s));
        if ( macros.size()==0 ) {
            das2_yyerror(scanner,"reader macro " + *(yyvsp[0].s) + " not found",tokAt(scanner,(yylsp[0])),
                CompilationError::unsupported_read_macro);
        } else if ( macros.size()>1 ) {
            string options;
            for ( auto & x : macros ) {
                options += "\t" + x->module->name + "::" + x->name + "\n";
            }
            das2_yyerror(scanner,"too many options for the reader macro " + *(yyvsp[0].s) +  "\n" + options, tokAt(scanner,(yylsp[0])),
                CompilationError::unsupported_read_macro);
        } else if ( yychar != '~' ) {
            das2_yyerror(scanner,"expecting ~ after the reader macro", tokAt(scanner,(yylsp[0])),
                CompilationError::syntax_error);
        } else {
            yyextra->g_ReaderMacro = macros.back().get();
            yyextra->g_ReaderExpr = new ExprReader(tokAt(scanner,(yylsp[-1])),yyextra->g_ReaderMacro);
            yyclearin ;
            das2_yybegin_reader(scanner);
        }
    }
    break;

  case 46: /* expr_reader: '%' name_in_namespace $@2 reader_character_sequence  */
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

  case 47: /* options_declaration: "options" annotation_argument_list  */
                                                   {
        if ( yyextra->g_Program->options.size() ) {
            for ( auto & opt : *(yyvsp[0].aaList) ) {
                if ( yyextra->g_Access->isOptionAllowed(opt.name, yyextra->g_Program->thisModuleName) ) {
                    yyextra->g_Program->options.push_back(opt);
                } else {
                    das2_yyerror(scanner,"option " + opt.name + " is not allowed here",
                        tokAt(scanner,(yylsp[0])), CompilationError::invalid_option);
                }
            }
        } else {
            swap ( yyextra->g_Program->options, *(yyvsp[0].aaList) );
        }
        delete (yyvsp[0].aaList);
    }
    break;

  case 49: /* require_module_name: "name"  */
                   {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 50: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 51: /* require_module_name: require_module_name '.' "name"  */
                                                {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 52: /* require_module_name: require_module_name '/' "name"  */
                                                {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 53: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 54: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 55: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 56: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 60: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 61: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 62: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 63: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 64: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 65: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 66: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 71: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 72: /* expression_else: "else" optional_emit_semis expression_block  */
                                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 73: /* expression_else: elif_or_static_elif '(' expr ')' optional_emit_semis expression_block expression_else  */
                                                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),(yyvsp[-1].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-6].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 74: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 75: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 76: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 77: /* expression_else_one_liner: "else" expression_if_one_liner  */
                                                      {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 78: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 79: /* expression_if_one_liner: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 80: /* expression_if_one_liner: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 81: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 82: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 87: /* $@3: %empty  */
        {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 88: /* $@4: %empty  */
                                      {
        yyextra->pop_nesteds();
    }
    break;

  case 89: /* expression_if_block: $@3 '{' expressions $@4 '}' expression_block_finally  */
                                        {
        (yyval.pExpression) = (yyvsp[-3].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-4]),(yylsp[0]));
        if ( (yyvsp[0].pExpression) ) {
            auto pF = (ExprBlock *) (yyvsp[0].pExpression);
            auto pB = (ExprBlock *) (yyval.pExpression);
            swap ( pB->finalList, pF->list );
            delete pF;
        }
    }
    break;

  case 90: /* $@5: %empty  */
       {
        yyextra->das_keyword = false;
    }
    break;

  case 91: /* expression_if_block: $@5 expression_if_one_liner SEMICOLON  */
                                               {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 92: /* $@6: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 93: /* expression_if_then_else: $@6 if_or_static_if '(' expr ')' optional_emit_semis expression_if_block expression_else  */
                                                                                                                   {
        yyextra->das_keyword = false;
        if ( !(yyvsp[-1].pExpression)->rtti_isBlock() && (yyvsp[0].pExpression)) {
            das2_yyerror(scanner,"if one-liner can't have else clause",tokAt(scanner,(yylsp[-6])), CompilationError::syntax_error);
        }
        auto blk = (yyvsp[-1].pExpression)->rtti_isBlock() ? static_cast<ExprBlock *>((yyvsp[-1].pExpression)) : ast_wrapInBlock((yyvsp[-1].pExpression));
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),blk,(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-6].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 94: /* expression_if_then_else_oneliner: expression_if_one_liner "if" '(' expr ')' expression_else_one_liner SEMICOLON  */
                                                                                                                      {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ast_wrapInBlock((yyvsp[-6].pExpression)),(yyvsp[-1].pExpression) ? ast_wrapInBlock((yyvsp[-1].pExpression)) : nullptr);
    }
    break;

  case 95: /* $@7: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 96: /* expression_for_loop: $@7 "for" '(' variable_name_with_pos_list "in" expr_list ')' optional_emit_semis expression_block  */
                                                                                                                                 {
        yyextra->das_keyword = false;
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-5].pNameWithPosList),(yyvsp[-3].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 97: /* expression_unsafe: "unsafe" optional_emit_semis expression_block  */
                                                                    {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-2])));
        pUnsafe->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 98: /* $@8: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 99: /* expression_while_loop: $@8 "while" '(' expr ')' optional_emit_semis expression_block  */
                                                                                         {
        yyextra->das_keyword = false;
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-5])));
        pWhile->cond = (yyvsp[-3].pExpression);
        pWhile->body = (yyvsp[0].pExpression);
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 100: /* $@9: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 101: /* expression_with: $@9 "with" '(' expr ')' optional_emit_semis expression_block  */
                                                                                   {
        yyextra->das_keyword = false;
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-5])));
        pWith->with = (yyvsp[-3].pExpression);
        pWith->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pWith;
    }
    break;

  case 102: /* expression_with_alias: "assume" "name" '=' expr  */
                                                      {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-2].s);
    }
    break;

  case 103: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 104: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 105: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 106: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 107: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 108: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 109: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 110: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 111: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 112: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 113: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 114: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 115: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 116: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 117: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 118: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 119: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 120: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 121: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 122: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 123: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 124: /* metadata_argument_list: '@' annotation_argument optional_emit_semis  */
                                                         {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[-1].aa));
    }
    break;

  case 125: /* metadata_argument_list: metadata_argument_list '@' annotation_argument optional_emit_semis  */
                                                                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-3].aaList),(yyvsp[-1].aa));
    }
    break;

  case 126: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 127: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 128: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 129: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 130: /* annotation_declaration_basic: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( yyextra->g_Access->isAnnotationAllowed(*(yyvsp[0].s), yyextra->g_Program->thisModuleName) ) {
            if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
                (yyval.fa)->annotation = ann;
            }
        } else {
            das2_yyerror(scanner,"annotation " + *(yyvsp[0].s) + " is not allowed here",
                        tokAt(scanner,(yylsp[0])), CompilationError::invalid_annotation);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 131: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
                                                                                 {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[-3]));
        if ( yyextra->g_Access->isAnnotationAllowed(*(yyvsp[-3].s), yyextra->g_Program->thisModuleName) ) {
            if ( auto ann = findAnnotation(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3]))) ) {
                (yyval.fa)->annotation = ann;
            }
        } else {
            das2_yyerror(scanner,"annotation " + *(yyvsp[-3].s) + " is not allowed here",
                        tokAt(scanner,(yylsp[-3])), CompilationError::invalid_annotation);
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
    break;

  case 132: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 133: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 134: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::And,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 135: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
                                                                            {
        if ( !(yyvsp[-2].fa)->annotation || !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation || !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Or,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 136: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Xor,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 137: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 138: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 139: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 140: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 141: /* optional_annotation_list: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 142: /* optional_annotation_list: '[' annotation_list ']'  */
                                       { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 143: /* optional_annotation_list_with_emit_semis: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 144: /* optional_annotation_list_with_emit_semis: '[' annotation_list ']' optional_emit_semis  */
                                                          { (yyval.faList) = (yyvsp[-2].faList); }
    break;

  case 145: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 146: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 147: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 148: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 149: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 150: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 151: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 152: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 153: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 154: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 155: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 156: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 157: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 158: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 159: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 160: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 161: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 162: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 163: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 164: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 165: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 166: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 167: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 168: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 169: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 170: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 171: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 172: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 173: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 174: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 175: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 176: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 177: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 178: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 179: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 180: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 181: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 182: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 183: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 184: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 185: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 186: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 187: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 188: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 189: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 190: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 191: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 192: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 193: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 194: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 195: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 196: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 197: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 198: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 199: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 200: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 201: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 202: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 203: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 204: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 205: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 206: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 207: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 208: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 209: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 210: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 211: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 212: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 213: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 214: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 215: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 216: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 217: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 218: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 219: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 220: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 221: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 222: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 223: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 224: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 225: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 226: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 227: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 228: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 229: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 230: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 231: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 232: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 233: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 234: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 235: /* global_function_declaration: optional_annotation_list_with_emit_semis "def" function_declaration  */
                                                                                                {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt(scanner,(yylsp[-1]),(yylsp[0]));
        assignDefaultArguments((yyvsp[0].pFuncDecl));
        runFunctionAnnotations(scanner, yyextra, (yyvsp[0].pFuncDecl), (yyvsp[-2].faList), tokAt(scanner,(yylsp[-2])));
        if ( (yyvsp[0].pFuncDecl)->isGeneric() ) {
            implAddGenericFunction(scanner,(yyvsp[0].pFuncDecl));
        } else {
            if ( !yyextra->g_Program->addFunction((yyvsp[0].pFuncDecl)) ) {
                das2_yyerror(scanner,"function is already defined " +
                    (yyvsp[0].pFuncDecl)->getMangledName(),(yyvsp[0].pFuncDecl)->at,
                        CompilationError::function_already_declared);
            }
        }
        (yyvsp[0].pFuncDecl)->delRef();
    }
    break;

  case 236: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 237: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 238: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 239: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 240: /* $@10: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 241: /* function_declaration: optional_public_or_private_function $@10 function_declaration_header optional_emit_semis expression_block  */
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

  case 242: /* expression_block_finally: %empty  */
        {
        (yyval.pExpression) = nullptr;
    }
    break;

  case 243: /* $@11: %empty  */
                  {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 244: /* $@12: %empty  */
                             {
        yyextra->pop_nesteds();
    }
    break;

  case 245: /* expression_block_finally: "finally" $@11 '{' expressions $@12 '}'  */
          {
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 246: /* $@13: %empty  */
        {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 247: /* $@14: %empty  */
                                      {
        yyextra->pop_nesteds();
    }
    break;

  case 248: /* expression_block: $@13 '{' expressions $@14 '}' expression_block_finally  */
                                        {
        (yyval.pExpression) = (yyvsp[-3].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-4]),(yylsp[0]));
        if ( (yyvsp[0].pExpression) ) {
            auto pF = (ExprBlock *) (yyvsp[0].pExpression);
            auto pB = (ExprBlock *) (yyval.pExpression);
            swap ( pB->finalList, pF->list );
            delete pF;
        }
    }
    break;

  case 249: /* expr_call_pipe: expr_call expr_full_block_assumed_piped  */
                                                           {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            ((ExprLooksLikeCall *)(yyvsp[-1].pExpression))->arguments.push_back((yyvsp[0].pExpression));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
            delete (yyvsp[0].pExpression);
        }
    }
    break;

  case 250: /* expression_any: SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 251: /* expression_any: expr_assign SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 252: /* expression_any: expression_delete SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 253: /* expression_any: expression_let  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 254: /* expression_any: expression_while_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 255: /* expression_any: expression_unsafe  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 256: /* expression_any: expression_with  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 257: /* expression_any: expression_with_alias SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 258: /* expression_any: expression_for_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 259: /* expression_any: expression_break SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 260: /* expression_any: expression_continue SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 261: /* expression_any: expression_return SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 262: /* expression_any: expression_yield SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 263: /* expression_any: expression_if_then_else  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 264: /* expression_any: expression_if_then_else_oneliner  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 265: /* expression_any: expression_try_catch  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 266: /* expression_any: expression_label SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 267: /* expression_any: expression_goto SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 268: /* expression_any: "pass" SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 269: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 270: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 271: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 272: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 273: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 274: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 275: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 276: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 277: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 278: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 279: /* name_in_namespace: "name" "::" "name"  */
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

  case 280: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 281: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 282: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 283: /* $@15: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 284: /* $@16: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 285: /* new_type_declaration: '<' $@15 type_declaration '>' $@16  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 286: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 287: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 288: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 289: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 290: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 291: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 292: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 293: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 294: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 295: /* expression_return: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 296: /* expression_return: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 297: /* expression_return: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 298: /* expression_yield: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 299: /* expression_yield: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 300: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 301: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 302: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 303: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 304: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 305: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 306: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 307: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 308: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 309: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 310: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 311: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                        {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-5]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 312: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 313: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 314: /* expression_let: kwd_let optional_in_scope '{' variable_declaration_list '}'  */
                                                                               {
        (yyval.pExpression) = ast_LetList(scanner,(yyvsp[-4].b),(yyvsp[-3].b),*(yyvsp[-1].pVarDeclList),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 315: /* $@17: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 316: /* $@18: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 317: /* expr_cast: "cast" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 318: /* $@19: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 319: /* $@20: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 320: /* expr_cast: "upcast" '<' $@19 type_declaration_no_options '>' $@20 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 321: /* $@21: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 322: /* $@22: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 323: /* expr_cast: "reinterpret" '<' $@21 type_declaration_no_options '>' $@22 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 324: /* $@23: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 325: /* $@24: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 326: /* expr_type_decl: "type" '<' $@23 type_declaration '>' $@24  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 327: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
                                                                          {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),ptd->typeexpr);
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),(yyvsp[-1].pExpression));
            }
            delete (yyvsp[-3].s);
    }
    break;

  case 328: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
                                                                                                {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),ptd->typeexpr,*(yyvsp[-4].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),(yyvsp[-1].pExpression),*(yyvsp[-4].s));
            }
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 329: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" c_or_s "name" '>' '(' expr ')'  */
                                                                                                                        {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),ptd->typeexpr,*(yyvsp[-6].s),*(yyvsp[-4].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),(yyvsp[-1].pExpression),*(yyvsp[-6].s),*(yyvsp[-4].s));
            }
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 330: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 331: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 332: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 333: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 334: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 335: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 336: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 337: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 338: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 339: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 340: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 341: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 342: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 343: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 344: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 345: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 346: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 347: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 348: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 349: /* expr_full_block_assumed_piped: '{' expressions '}'  */
                                   {
        (yyval.pExpression) = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,new TypeDecl(Type::autoinfer),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 350: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 351: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 352: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 353: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 354: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 355: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 356: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 357: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 358: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 359: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 360: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 361: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 362: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 363: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 364: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 365: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 366: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 367: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 368: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 369: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 370: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 371: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 372: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 373: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 374: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 375: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 376: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 377: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 378: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 379: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 380: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 381: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 382: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 383: /* $@25: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 384: /* $@26: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 385: /* func_addr_expr: '@' '@' '<' $@25 type_declaration_no_options '>' $@26 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 386: /* $@27: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 387: /* $@28: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 388: /* func_addr_expr: '@' '@' '<' $@27 optional_function_argument_list optional_function_type '>' $@28 func_addr_name  */
                                                                                                                                                                                     {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = (yyvsp[-3].pTypeDecl);
        if ( (yyvsp[-4].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, expr->funcType.get(), (yyvsp[-4].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-4].pVarDeclList));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 389: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 390: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 391: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 392: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 393: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                       {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-7].pExpression));
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 394: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 395: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 396: /* $@29: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 397: /* $@30: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 398: /* expr_field: expr '.' $@29 error $@30  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 399: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 400: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 401: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 402: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 403: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 404: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 405: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 406: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 407: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 408: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 409: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 410: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 411: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 412: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 413: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 414: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 415: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 416: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 417: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 418: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 419: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 420: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 421: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 422: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 423: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 424: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 425: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 426: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 427: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 428: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 429: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 430: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 431: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 432: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 433: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 434: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 435: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 436: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 437: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 438: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 439: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 440: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 441: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 442: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 443: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 444: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 445: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 446: /* expr: '(' expr_list optional_comma ')'  */
                                                         {
            if ( (yyvsp[-2].pExpression)->rtti_isSequence() ) {
                auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
                mkt->values = sequenceToList((yyvsp[-2].pExpression));
                (yyval.pExpression) = mkt;
            } else if ( (yyvsp[-1].b) ) {
                auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
                mkt->values.push_back((yyvsp[-2].pExpression));
                (yyval.pExpression) = mkt;
            } else {
                (yyval.pExpression) = (yyvsp[-2].pExpression);
            }
        }
    break;

  case 447: /* expr: '(' make_struct_single ')'  */
                                      {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        for ( auto & arg : *(((ExprMakeStruct *)(yyvsp[-1].pExpression))->structs.back()) ) {
            mkt->values.push_back(arg->value);
            mkt->recordNames.push_back(arg->name);
        }
        delete (yyvsp[-1].pExpression);
        (yyval.pExpression) = mkt;
    }
    break;

  case 448: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 449: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 450: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 451: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 452: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 453: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 454: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 455: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 456: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 457: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 458: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 459: /* expr: expr_generator  */
                                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 460: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 461: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 462: /* $@31: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 463: /* $@32: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 464: /* expr: expr "is" "type" '<' $@31 type_declaration_no_options '>' $@32  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 465: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 466: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 467: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 468: /* $@33: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 469: /* $@34: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 470: /* expr: expr "as" "type" '<' $@33 type_declaration '>' $@34  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 471: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 472: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 473: /* $@35: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 474: /* $@36: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 475: /* expr: expr '?' "as" "type" '<' $@35 type_declaration '>' $@36  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 476: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 477: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 478: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 479: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 480: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 481: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 482: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 483: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 484: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 485: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 486: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 487: /* expr: expr_call_pipe  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 488: /* expr: "unsafe" '(' expr ')'  */
                                         {
            (yyvsp[-1].pExpression)->alwaysSafe = true;
            (yyvsp[-1].pExpression)->userSaidItsSafe = true;
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    break;

  case 489: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 490: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 491: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list optional_emit_semis expression_block  */
                                                                                                                                                  {
        auto closure = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->returnType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),closure,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 492: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 493: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 494: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 495: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 496: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 497: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 498: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 499: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 500: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 501: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 502: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 503: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 504: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 505: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 506: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 507: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 508: /* optional_field_annotation: %empty  */
                                      { (yyval.aaList) = nullptr; }
    break;

  case 509: /* optional_field_annotation: metadata_argument_list  */
                                      { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 510: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 511: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 512: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 513: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 514: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 515: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 516: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 517: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 518: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 519: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 520: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 521: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 522: /* struct_variable_declaration_list: struct_variable_declaration_list "new line, semicolon"  */
                                                                 { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 523: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 524: /* struct_variable_declaration_list: struct_variable_declaration_list $@37 structure_variable_declaration SEMICOLON  */
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

  case 525: /* $@38: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 526: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable "abstract" optional_constant $@38 function_declaration_header SEMICOLON  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 527: /* $@39: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 528: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@39 function_declaration_header optional_emit_semis expression_block  */
                                                                                            {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-10].pVarDeclList),(yyvsp[-9].faList),(yyvsp[-6].b),(yyvsp[-7].b),(yyvsp[-5].i),(yyvsp[-4].b),(yyvsp[-2].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-8]),(yylsp[0])),tokAt(scanner,(yylsp[-9])));
            }
    break;

  case 529: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 530: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 531: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 532: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 533: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 534: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 535: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 536: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 537: /* tuple_alias_type_list: %empty  */
      {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 538: /* tuple_alias_type_list: tuple_type  */
                       {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 539: /* tuple_alias_type_list: tuple_alias_type_list semis tuple_type  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tokName = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *((yyvsp[0].pVarDecl)->pNameList) ) {
                    crd->afterTupleEntry(nl.name.c_str(), nl.at);
                }
            }
        }
    }
    break;

  case 540: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 541: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 542: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 543: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 544: /* variant_alias_type_list: variant_type  */
                         {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 545: /* variant_alias_type_list: variant_alias_type_list semis variant_type  */
                                                               {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tokName = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *((yyvsp[0].pVarDecl)->pNameList) ) {
                    crd->afterVariantEntry(nl.name.c_str(), nl.at);
                }
            }
        }
    }
    break;

  case 546: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 547: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 548: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 549: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 550: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 551: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 552: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 553: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 554: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 555: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 556: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 557: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 558: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 559: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 560: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
                                         {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 561: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 562: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 563: /* global_let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 564: /* global_let_variable_name_with_pos_list: global_let_variable_name_with_pos_list ',' "name"  */
                                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 565: /* variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 566: /* variable_declaration_list: variable_declaration_list SEMICOLON  */
                                                  {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 567: /* variable_declaration_list: variable_declaration_list let_variable_declaration  */
                                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 568: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 569: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 570: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 571: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                         {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 572: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                               {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 573: /* global_let_variable_declaration: global_let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 574: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 575: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 576: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 577: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 578: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 579: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 580: /* global_variable_declaration_list: global_variable_declaration_list SEMICOLON  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 581: /* $@40: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 582: /* global_variable_declaration_list: global_variable_declaration_list $@40 optional_field_annotation let_variable_declaration  */
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

  case 583: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 584: /* $@41: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 585: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@41 optional_field_annotation global_let_variable_declaration  */
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

  case 586: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        delete (yyvsp[0].s);
    }
    break;

  case 587: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
        delete (yyvsp[-2].s);
    }
    break;

  case 590: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 591: /* enum_list: enum_expression  */
                            {
        (yyval.pEnum) = new Enumeration();
        if ( !(yyval.pEnum)->add((yyvsp[0].pEnumPair)->name,(yyvsp[0].pEnumPair)->expr,(yyvsp[0].pEnumPair)->at) ) {
            das2_yyerror(scanner,"enumeration already declared " + (yyvsp[0].pEnumPair)->name, (yyvsp[0].pEnumPair)->at,
                CompilationError::enumeration_value_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            for ( auto & crd : yyextra->g_CommentReaders ) {
                crd->afterEnumerationEntry((yyvsp[0].pEnumPair)->name.c_str(), (yyvsp[0].pEnumPair)->at);
            }
        }
        delete (yyvsp[0].pEnumPair);
    }
    break;

  case 592: /* enum_list: enum_list commas enum_expression  */
                                                 {
        if ( !(yyvsp[-2].pEnum)->add((yyvsp[0].pEnumPair)->name,(yyvsp[0].pEnumPair)->expr,(yyvsp[0].pEnumPair)->at) ) {
            das2_yyerror(scanner,"enumeration already declared " + (yyvsp[0].pEnumPair)->name, (yyvsp[0].pEnumPair)->at,
                CompilationError::enumeration_value_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            for ( auto & crd : yyextra->g_CommentReaders ) {
                crd->afterEnumerationEntry((yyvsp[0].pEnumPair)->name.c_str(), (yyvsp[0].pEnumPair)->at);
            }
        }
        delete (yyvsp[0].pEnumPair);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
    break;

  case 593: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 594: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 595: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 596: /* $@42: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 597: /* single_alias: optional_public_or_private_alias "name" $@42 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->isPrivateAlias = !(yyvsp[-4].b);
        if ( (yyvsp[0].pTypeDecl)->baseType == Type::alias ) {
            das2_yyerror(scanner,"alias cannot be defined in terms of another alias "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::invalid_type);
        }
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !yyextra->g_Program->addAlias((yyvsp[0].pTypeDecl)) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterAlias((yyvsp[-3].s)->c_str(),pubename);
        }
        delete (yyvsp[-3].s);
    }
    break;

  case 599: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 600: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 601: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 602: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 603: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 604: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 611: /* $@43: %empty  */
                                                                     {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 612: /* $@44: %empty  */
                                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 613: /* $@45: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 614: /* enum_declaration: optional_annotation_list_with_emit_semis "enum" $@43 optional_public_or_private_enum enum_name optional_enum_basic_type_declaration optional_emit_commas '{' $@44 enum_list optional_commas $@45 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-8].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-12].faList),tokAt(scanner,(yylsp[-12])),(yyvsp[-9].b),(yyvsp[-8].pEnum),(yyvsp[-3].pEnum),(yyvsp[-7].type));
    }
    break;

  case 615: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 616: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 617: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 618: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 619: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 620: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 621: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 622: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 623: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 624: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 625: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 626: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 627: /* optional_struct_variable_declaration_list: ';'  */
            {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 628: /* optional_struct_variable_declaration_list: '{' struct_variable_declaration_list '}'  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 629: /* $@46: %empty  */
                                                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 630: /* $@47: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 631: /* $@48: %empty  */
                                             {
        if ( (yyvsp[-1].pStructure) ) {
            (yyvsp[-1].pStructure)->isClass = (yyvsp[-4].i)==CorS_Class || (yyvsp[-4].i)==CorS_ClassTemplate;
            (yyvsp[-1].pStructure)->isTemplate = (yyvsp[-4].i)==CorS_ClassTemplate || (yyvsp[-4].i)==CorS_StructTemplate;
            (yyvsp[-1].pStructure)->privateStructure = !(yyvsp[-3].b);
        }
    }
    break;

  case 632: /* structure_declaration: optional_annotation_list_with_emit_semis $@46 class_or_struct optional_public_or_private_structure $@47 structure_name optional_emit_semis $@48 optional_struct_variable_declaration_list  */
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

  case 633: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 634: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 635: /* variable_name_with_pos_list: "name" "aka" "name"  */
                                         {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 636: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 637: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 638: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 639: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 640: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 641: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 642: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 643: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 644: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 645: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 646: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 647: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 648: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 649: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 650: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 651: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 652: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 653: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 654: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 655: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 656: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 657: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 658: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 659: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 660: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 661: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 662: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 663: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 664: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 665: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 666: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 667: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 668: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 669: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 670: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 671: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 672: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 673: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 674: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 675: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 676: /* auto_type_declaration: "$t" '(' expr ')'  */
                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::alias);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = "``MACRO``TAG``";
        (yyval.pTypeDecl)->isTag = true;
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner, (yylsp[-1]));
        (yyval.pTypeDecl)->firstType->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 677: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 678: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 679: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 680: /* bitfield_alias_bits: "name"  */
                   {
        (yyval.pNameList) = new vector<string>();
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[0].s)->c_str(),atvname);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 681: /* bitfield_alias_bits: bitfield_alias_bits commas "name"  */
                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[0].s)->c_str(),atvname);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 682: /* $@49: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 683: /* $@50: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 684: /* bitfield_type_declaration: "bitfield" '<' $@49 bitfield_bits '>' $@50  */
                                                                                                                             {
            (yyval.pTypeDecl) = new TypeDecl(Type::tBitfield);
            (yyval.pTypeDecl)->argNames = *(yyvsp[-2].pNameList);
            if ( (yyval.pTypeDecl)->argNames.size()>32 ) {
                das2_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-5])),
                    CompilationError::invalid_type);
            }
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
            delete (yyvsp[-2].pNameList);
    }
    break;

  case 687: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 688: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 689: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 690: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 691: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 692: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 693: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 694: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
                                                                       {
        if ( (yyvsp[-1].pTypeDecl)->baseType==Type::typeDecl ) {
            das2_yyerror(scanner,"type declaration can`t be used as array base type",tokAt(scanner,(yylsp[-1])),
                CompilationError::invalid_type);
        } else if ( (yyvsp[-1].pTypeDecl)->baseType==Type::typeMacro ) {
            das2_yyerror(scanner,"macro can`t be used as array base type",tokAt(scanner,(yylsp[-1])),
                CompilationError::invalid_type);
        }
        (yyvsp[-1].pTypeDecl)->dim.insert((yyvsp[-1].pTypeDecl)->dim.begin(), (yyvsp[0].pTypeDecl)->dim.begin(), (yyvsp[0].pTypeDecl)->dim.end());
        (yyvsp[-1].pTypeDecl)->dimExpr.insert((yyvsp[-1].pTypeDecl)->dimExpr.begin(), (yyvsp[0].pTypeDecl)->dimExpr.begin(), (yyvsp[0].pTypeDecl)->dimExpr.end());
        (yyvsp[-1].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyvsp[0].pTypeDecl)->dimExpr.clear();
        delete (yyvsp[0].pTypeDecl);
    }
    break;

  case 695: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 696: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 697: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 698: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 699: /* $@51: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 700: /* $@52: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 701: /* type_declaration_no_options_no_dim: "type" '<' $@51 type_declaration '>' $@52  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 702: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 703: /* type_declaration_no_options_no_dim: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 704: /* $@53: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 705: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@53 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 706: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 707: /* type_declaration_no_options_no_dim: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 708: /* type_declaration_no_options_no_dim: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 709: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 710: /* type_declaration_no_options_no_dim: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 711: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 712: /* type_declaration_no_options_no_dim: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 713: /* type_declaration_no_options_no_dim: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 714: /* type_declaration_no_options_no_dim: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 715: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 716: /* type_declaration_no_options_no_dim: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 717: /* type_declaration_no_options_no_dim: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 718: /* $@54: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 719: /* $@55: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 720: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@54 type_declaration '>' $@55  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 721: /* type_declaration_no_options_no_dim: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 722: /* $@56: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 723: /* $@57: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 724: /* type_declaration_no_options_no_dim: "array" '<' $@56 type_declaration '>' $@57  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 725: /* $@58: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 726: /* $@59: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 727: /* type_declaration_no_options_no_dim: "table" '<' $@58 table_type_pair '>' $@59  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 728: /* $@60: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 729: /* $@61: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 730: /* type_declaration_no_options_no_dim: "iterator" '<' $@60 type_declaration '>' $@61  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 731: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 732: /* $@62: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 733: /* $@63: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 734: /* type_declaration_no_options_no_dim: "block" '<' $@62 type_declaration '>' $@63  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 735: /* $@64: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 736: /* $@65: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 737: /* type_declaration_no_options_no_dim: "block" '<' $@64 optional_function_argument_list optional_function_type '>' $@65  */
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

  case 738: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 739: /* $@66: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 740: /* $@67: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 741: /* type_declaration_no_options_no_dim: "function" '<' $@66 type_declaration '>' $@67  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 742: /* $@68: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 743: /* $@69: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 744: /* type_declaration_no_options_no_dim: "function" '<' $@68 optional_function_argument_list optional_function_type '>' $@69  */
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

  case 745: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 746: /* $@70: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 747: /* $@71: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 748: /* type_declaration_no_options_no_dim: "lambda" '<' $@70 type_declaration '>' $@71  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 749: /* $@72: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 750: /* $@73: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 751: /* type_declaration_no_options_no_dim: "lambda" '<' $@72 optional_function_argument_list optional_function_type '>' $@73  */
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

  case 752: /* $@74: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 753: /* $@75: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 754: /* type_declaration_no_options_no_dim: "tuple" '<' $@74 tuple_type_list '>' $@75  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 755: /* $@76: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 756: /* $@77: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 757: /* type_declaration_no_options_no_dim: "variant" '<' $@76 variant_type_list '>' $@77  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 758: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 759: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 760: /* type_declaration: type_declaration '|' '#'  */
                                             {
        if ( (yyvsp[-2].pTypeDecl)->baseType==Type::option ) {
            (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
            (yyval.pTypeDecl)->argTypes.push_back(make_smart<TypeDecl>(*(yyvsp[-2].pTypeDecl)->argTypes.back()));
            (yyvsp[-2].pTypeDecl)->argTypes.back()->temporary ^= true;
        } else {
            (yyval.pTypeDecl) = new TypeDecl(Type::option);
            (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[-2].pTypeDecl));
            (yyval.pTypeDecl)->argTypes.push_back(make_smart<TypeDecl>(*(yyvsp[-2].pTypeDecl)));
            (yyval.pTypeDecl)->argTypes.back()->temporary ^= true;
        }
    }
    break;

  case 761: /* $@78: %empty  */
                   {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 762: /* $@79: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 763: /* $@80: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 764: /* $@81: %empty  */
                                                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 765: /* tuple_alias_declaration: "tuple" $@78 optional_public_or_private_alias "name" optional_emit_semis $@79 '{' $@80 tuple_alias_type_list optional_semis $@81 '}'  */
          {
        auto vtype = make_smart<TypeDecl>(Type::tTuple);
        vtype->alias = *(yyvsp[-8].s);
        vtype->at = tokAt(scanner,(yylsp[-8]));
        vtype->isPrivateAlias = !(yyvsp[-9].b);
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-3].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-8].s),tokAt(scanner,(yylsp[-8])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-8]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTuple((yyvsp[-8].s)->c_str(),atvname);
        }
        delete (yyvsp[-8].s);
    }
    break;

  case 766: /* $@82: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 767: /* $@83: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 768: /* $@84: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 769: /* $@85: %empty  */
                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 770: /* variant_alias_declaration: "variant" $@82 optional_public_or_private_alias "name" optional_emit_semis $@83 '{' $@84 variant_alias_type_list optional_semis $@85 '}'  */
          {
        auto vtype = make_smart<TypeDecl>(Type::tVariant);
        vtype->alias = *(yyvsp[-8].s);
        vtype->at = tokAt(scanner,(yylsp[-8]));
        vtype->isPrivateAlias = !(yyvsp[-9].b);
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-3].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-8].s),tokAt(scanner,(yylsp[-8])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-8]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariant((yyvsp[-8].s)->c_str(),atvname);
        }
        delete (yyvsp[-8].s);
    }
    break;

  case 771: /* $@86: %empty  */
                      {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 772: /* $@87: %empty  */
                                                                              {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 773: /* $@88: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 774: /* $@89: %empty  */
                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 775: /* bitfield_alias_declaration: "bitfield" $@86 optional_public_or_private_alias "name" optional_emit_commas $@87 '{' $@88 bitfield_alias_bits optional_commas $@89 '}'  */
          {
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-8].s);
        btype->at = tokAt(scanner,(yylsp[-8]));
        btype->argNames = *(yyvsp[-3].pNameList);
        btype->isPrivateAlias = !(yyvsp[-9].b);
        if ( btype->argNames.size()>32 ) {
            das2_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-8])),
                CompilationError::invalid_type);
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-8].s),tokAt(scanner,(yylsp[-8])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-8]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfield((yyvsp[-8].s)->c_str(),atvname);
        }
        delete (yyvsp[-8].s);
        delete (yyvsp[-3].pNameList);
    }
    break;

  case 776: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 777: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 778: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 779: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 780: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 781: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 782: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 783: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 784: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 785: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 786: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 787: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 788: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 789: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 790: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 791: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 792: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 793: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 794: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 795: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 796: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 797: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 798: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 799: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 800: /* $@90: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 801: /* $@91: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 802: /* make_struct_decl: "struct" '<' $@90 type_declaration_no_options '>' $@91 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 803: /* $@92: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 804: /* $@93: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 805: /* make_struct_decl: "class" '<' $@92 type_declaration_no_options '>' $@93 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 806: /* $@94: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 807: /* $@95: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 808: /* make_struct_decl: "variant" '<' $@94 variant_type_list '>' $@95 '(' use_initializer make_variant_dim ')'  */
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

  case 809: /* $@96: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 810: /* $@97: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 811: /* make_struct_decl: "default" '<' $@96 type_declaration_no_options '>' $@97 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 812: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 813: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 814: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 815: /* $@98: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 816: /* $@99: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 817: /* make_tuple_call: "tuple" '<' $@98 tuple_type_list '>' $@99 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 818: /* make_dim_decl: '[' optional_expr_list ']'  */
                                                  {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-2])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
            mka->gen2 = true;
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto mks = new ExprMakeStruct();
            mks->at = tokAt(scanner,(yylsp[-2]));
            mks->makeType = make_smart<TypeDecl>(Type::tArray);
            mks->makeType->firstType = make_smart<TypeDecl>(Type::autoinfer);
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }
    }
    break;

  case 819: /* $@100: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 820: /* $@101: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 821: /* make_dim_decl: "array" "struct" '<' $@100 type_declaration_no_options '>' $@101 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 822: /* $@102: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 823: /* $@103: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 824: /* make_dim_decl: "array" "tuple" '<' $@102 tuple_type_list '>' $@103 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 825: /* $@104: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 826: /* $@105: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 827: /* make_dim_decl: "array" "variant" '<' $@104 variant_type_list '>' $@105 '(' make_variant_dim ')'  */
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

  case 828: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 829: /* $@106: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 830: /* $@107: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 831: /* make_dim_decl: "array" '<' $@106 type_declaration_no_options '>' $@107 '(' optional_expr_list ')'  */
                                                                                                                                                                        {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = (yyvsp[-5].pTypeDecl);
            mka->gen2 = true;
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->makeType = make_smart<TypeDecl>(Type::tArray);
            msd->makeType->firstType = (yyvsp[-5].pTypeDecl);
            msd->at = tokAt(scanner,(yylsp[-5]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 832: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 833: /* $@108: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 834: /* $@109: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 835: /* make_dim_decl: "fixed_array" '<' $@108 type_declaration_no_options '>' $@109 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 836: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 837: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 838: /* $@110: %empty  */
                  {
        yyextra->das_nested_parentheses ++;
    }
    break;

  case 839: /* make_table_decl: '{' $@110 optional_emit_semis optional_expr_map_tuple_list '}'  */
                                                                      {
        yyextra->das_nested_parentheses --;
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto mks = new ExprMakeStruct();
            mks->at = tokAt(scanner,(yylsp[-4]));
            mks->makeType = make_smart<TypeDecl>(Type::tTable);
            mks->makeType->firstType = make_smart<TypeDecl>(Type::autoinfer);
            mks->makeType->secondType = make_smart<TypeDecl>(Type::autoinfer);
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }
    }
    break;

  case 840: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 841: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                 {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-6])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = (yyvsp[-4].pTypeDecl);
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->makeType = make_smart<TypeDecl>(Type::tTable);
            msd->makeType->firstType = (yyvsp[-4].pTypeDecl);
            msd->makeType->secondType = make_smart<TypeDecl>(Type::tVoid);
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 842: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                                                             {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = make_smart<TypeDecl>(Type::tTuple);
            mka->makeType->argTypes.push_back((yyvsp[-6].pTypeDecl));
            mka->makeType->argTypes.push_back((yyvsp[-4].pTypeDecl));
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->makeType = make_smart<TypeDecl>(Type::tTable);
            msd->makeType->firstType = (yyvsp[-6].pTypeDecl);
            msd->makeType->secondType = (yyvsp[-4].pTypeDecl);
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 843: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 844: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 845: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 846: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 847: /* array_comprehension: '[' "for" '(' variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                           {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 848: /* array_comprehension: '[' "iterator" "for" '(' variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                                        {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 849: /* array_comprehension: '{' "for" '(' variable_name_with_pos_list "in" expr_list ')' ';' make_map_tuple array_comprehension_where '}'  */
                                                                                                                                                                     {
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
                CompilationError::syntax_error);
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


