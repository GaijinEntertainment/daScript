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
  YYSYMBOL_string_builder_body = 218,      /* string_builder_body  */
  YYSYMBOL_string_builder = 219,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 220, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 221,              /* expr_reader  */
  YYSYMBOL_222_1 = 222,                    /* $@1  */
  YYSYMBOL_options_declaration = 223,      /* options_declaration  */
  YYSYMBOL_require_declaration = 224,      /* require_declaration  */
  YYSYMBOL_require_module_name = 225,      /* require_module_name  */
  YYSYMBOL_require_module = 226,           /* require_module  */
  YYSYMBOL_is_public_module = 227,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 228,       /* expect_declaration  */
  YYSYMBOL_expect_list = 229,              /* expect_list  */
  YYSYMBOL_expect_error = 230,             /* expect_error  */
  YYSYMBOL_expression_label = 231,         /* expression_label  */
  YYSYMBOL_expression_goto = 232,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 233,      /* elif_or_static_elif  */
  YYSYMBOL_emit_semis = 234,               /* emit_semis  */
  YYSYMBOL_optional_emit_semis = 235,      /* optional_emit_semis  */
  YYSYMBOL_expression_else = 236,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 237,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 238, /* expression_else_one_liner  */
  YYSYMBOL_expression_if_one_liner = 239,  /* expression_if_one_liner  */
  YYSYMBOL_semis = 240,                    /* semis  */
  YYSYMBOL_optional_semis = 241,           /* optional_semis  */
  YYSYMBOL_expression_if_block = 242,      /* expression_if_block  */
  YYSYMBOL_243_2 = 243,                    /* $@2  */
  YYSYMBOL_244_3 = 244,                    /* $@3  */
  YYSYMBOL_245_4 = 245,                    /* $@4  */
  YYSYMBOL_expression_if_then_else = 246,  /* expression_if_then_else  */
  YYSYMBOL_247_5 = 247,                    /* $@5  */
  YYSYMBOL_expression_if_then_else_oneliner = 248, /* expression_if_then_else_oneliner  */
  YYSYMBOL_expression_for_loop = 249,      /* expression_for_loop  */
  YYSYMBOL_250_6 = 250,                    /* $@6  */
  YYSYMBOL_expression_unsafe = 251,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 252,    /* expression_while_loop  */
  YYSYMBOL_253_7 = 253,                    /* $@7  */
  YYSYMBOL_expression_with = 254,          /* expression_with  */
  YYSYMBOL_255_8 = 255,                    /* $@8  */
  YYSYMBOL_expression_with_alias = 256,    /* expression_with_alias  */
  YYSYMBOL_annotation_argument_value = 257, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 258, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 259, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 260,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 261, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 262,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 263, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 264, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 265,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 266,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 267, /* optional_annotation_list  */
  YYSYMBOL_optional_annotation_list_with_emit_semis = 268, /* optional_annotation_list_with_emit_semis  */
  YYSYMBOL_optional_function_argument_list = 269, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 270,   /* optional_function_type  */
  YYSYMBOL_function_name = 271,            /* function_name  */
  YYSYMBOL_global_function_declaration = 272, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 273, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 274, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 275,     /* function_declaration  */
  YYSYMBOL_276_9 = 276,                    /* $@9  */
  YYSYMBOL_expression_block_finally = 277, /* expression_block_finally  */
  YYSYMBOL_278_10 = 278,                   /* $@10  */
  YYSYMBOL_279_11 = 279,                   /* $@11  */
  YYSYMBOL_expression_block = 280,         /* expression_block  */
  YYSYMBOL_281_12 = 281,                   /* $@12  */
  YYSYMBOL_282_13 = 282,                   /* $@13  */
  YYSYMBOL_expr_call_pipe = 283,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 284,           /* expression_any  */
  YYSYMBOL_expressions = 285,              /* expressions  */
  YYSYMBOL_optional_expr_list = 286,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_map_tuple_list = 287, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 288, /* type_declaration_no_options_list  */
  YYSYMBOL_name_in_namespace = 289,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 290,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 291,     /* new_type_declaration  */
  YYSYMBOL_292_14 = 292,                   /* $@14  */
  YYSYMBOL_293_15 = 293,                   /* $@15  */
  YYSYMBOL_expr_new = 294,                 /* expr_new  */
  YYSYMBOL_expression_break = 295,         /* expression_break  */
  YYSYMBOL_expression_continue = 296,      /* expression_continue  */
  YYSYMBOL_expression_return = 297,        /* expression_return  */
  YYSYMBOL_expression_yield = 298,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 299,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 300,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 301,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 302,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 303,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 304, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 305,           /* expression_let  */
  YYSYMBOL_expr_cast = 306,                /* expr_cast  */
  YYSYMBOL_307_16 = 307,                   /* $@16  */
  YYSYMBOL_308_17 = 308,                   /* $@17  */
  YYSYMBOL_309_18 = 309,                   /* $@18  */
  YYSYMBOL_310_19 = 310,                   /* $@19  */
  YYSYMBOL_311_20 = 311,                   /* $@20  */
  YYSYMBOL_312_21 = 312,                   /* $@21  */
  YYSYMBOL_expr_type_decl = 313,           /* expr_type_decl  */
  YYSYMBOL_314_22 = 314,                   /* $@22  */
  YYSYMBOL_315_23 = 315,                   /* $@23  */
  YYSYMBOL_expr_type_info = 316,           /* expr_type_info  */
  YYSYMBOL_expr_list = 317,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 318,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 319,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 320,            /* capture_entry  */
  YYSYMBOL_capture_list = 321,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 322,    /* optional_capture_list  */
  YYSYMBOL_expr_full_block = 323,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 324, /* expr_full_block_assumed_piped  */
  YYSYMBOL_expr_numeric_const = 325,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 326,              /* expr_assign  */
  YYSYMBOL_expr_named_call = 327,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 328,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 329,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 330,           /* func_addr_expr  */
  YYSYMBOL_331_24 = 331,                   /* $@24  */
  YYSYMBOL_332_25 = 332,                   /* $@25  */
  YYSYMBOL_333_26 = 333,                   /* $@26  */
  YYSYMBOL_334_27 = 334,                   /* $@27  */
  YYSYMBOL_expr_field = 335,               /* expr_field  */
  YYSYMBOL_336_28 = 336,                   /* $@28  */
  YYSYMBOL_337_29 = 337,                   /* $@29  */
  YYSYMBOL_expr_call = 338,                /* expr_call  */
  YYSYMBOL_expr = 339,                     /* expr  */
  YYSYMBOL_340_30 = 340,                   /* $@30  */
  YYSYMBOL_341_31 = 341,                   /* $@31  */
  YYSYMBOL_342_32 = 342,                   /* $@32  */
  YYSYMBOL_343_33 = 343,                   /* $@33  */
  YYSYMBOL_344_34 = 344,                   /* $@34  */
  YYSYMBOL_345_35 = 345,                   /* $@35  */
  YYSYMBOL_expr_generator = 346,           /* expr_generator  */
  YYSYMBOL_expr_mtag = 347,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 348, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 349,        /* optional_override  */
  YYSYMBOL_optional_constant = 350,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 351, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 352, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 353, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 354, /* struct_variable_declaration_list  */
  YYSYMBOL_355_36 = 355,                   /* $@36  */
  YYSYMBOL_356_37 = 356,                   /* $@37  */
  YYSYMBOL_357_38 = 357,                   /* $@38  */
  YYSYMBOL_function_argument_declaration = 358, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 359,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 360,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 361,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 362,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 363,             /* variant_type  */
  YYSYMBOL_variant_type_list = 364,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 365,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 366,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 367,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 368,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 369,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 370, /* let_variable_name_with_pos_list  */
  YYSYMBOL_global_let_variable_name_with_pos_list = 371, /* global_let_variable_name_with_pos_list  */
  YYSYMBOL_variable_declaration_list = 372, /* variable_declaration_list  */
  YYSYMBOL_let_variable_declaration = 373, /* let_variable_declaration  */
  YYSYMBOL_global_let_variable_declaration = 374, /* global_let_variable_declaration  */
  YYSYMBOL_optional_shared = 375,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 376, /* optional_public_or_private_variable  */
  YYSYMBOL_global_variable_declaration_list = 377, /* global_variable_declaration_list  */
  YYSYMBOL_378_39 = 378,                   /* $@39  */
  YYSYMBOL_global_let = 379,               /* global_let  */
  YYSYMBOL_380_40 = 380,                   /* $@40  */
  YYSYMBOL_enum_expression = 381,          /* enum_expression  */
  YYSYMBOL_commas = 382,                   /* commas  */
  YYSYMBOL_enum_list = 383,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 384, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 385,             /* single_alias  */
  YYSYMBOL_386_41 = 386,                   /* $@41  */
  YYSYMBOL_alias_declaration = 387,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 388, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 389,                /* enum_name  */
  YYSYMBOL_optional_enum_basic_type_declaration = 390, /* optional_enum_basic_type_declaration  */
  YYSYMBOL_optional_commas = 391,          /* optional_commas  */
  YYSYMBOL_emit_commas = 392,              /* emit_commas  */
  YYSYMBOL_optional_emit_commas = 393,     /* optional_emit_commas  */
  YYSYMBOL_enum_declaration = 394,         /* enum_declaration  */
  YYSYMBOL_395_42 = 395,                   /* $@42  */
  YYSYMBOL_396_43 = 396,                   /* $@43  */
  YYSYMBOL_397_44 = 397,                   /* $@44  */
  YYSYMBOL_optional_structure_parent = 398, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 399,          /* optional_sealed  */
  YYSYMBOL_structure_name = 400,           /* structure_name  */
  YYSYMBOL_class_or_struct = 401,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 402, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 403, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 404,    /* structure_declaration  */
  YYSYMBOL_405_45 = 405,                   /* $@45  */
  YYSYMBOL_406_46 = 406,                   /* $@46  */
  YYSYMBOL_407_47 = 407,                   /* $@47  */
  YYSYMBOL_variable_name_with_pos_list = 408, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 409,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 410, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 411, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 412,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 413,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 414,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 415, /* bitfield_type_declaration  */
  YYSYMBOL_416_48 = 416,                   /* $@48  */
  YYSYMBOL_417_49 = 417,                   /* $@49  */
  YYSYMBOL_c_or_s = 418,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 419,          /* table_type_pair  */
  YYSYMBOL_dim_list = 420,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 421, /* type_declaration_no_options  */
  YYSYMBOL_type_declaration_no_options_no_dim = 422, /* type_declaration_no_options_no_dim  */
  YYSYMBOL_423_50 = 423,                   /* $@50  */
  YYSYMBOL_424_51 = 424,                   /* $@51  */
  YYSYMBOL_425_52 = 425,                   /* $@52  */
  YYSYMBOL_426_53 = 426,                   /* $@53  */
  YYSYMBOL_427_54 = 427,                   /* $@54  */
  YYSYMBOL_428_55 = 428,                   /* $@55  */
  YYSYMBOL_429_56 = 429,                   /* $@56  */
  YYSYMBOL_430_57 = 430,                   /* $@57  */
  YYSYMBOL_431_58 = 431,                   /* $@58  */
  YYSYMBOL_432_59 = 432,                   /* $@59  */
  YYSYMBOL_433_60 = 433,                   /* $@60  */
  YYSYMBOL_434_61 = 434,                   /* $@61  */
  YYSYMBOL_435_62 = 435,                   /* $@62  */
  YYSYMBOL_436_63 = 436,                   /* $@63  */
  YYSYMBOL_437_64 = 437,                   /* $@64  */
  YYSYMBOL_438_65 = 438,                   /* $@65  */
  YYSYMBOL_439_66 = 439,                   /* $@66  */
  YYSYMBOL_440_67 = 440,                   /* $@67  */
  YYSYMBOL_441_68 = 441,                   /* $@68  */
  YYSYMBOL_442_69 = 442,                   /* $@69  */
  YYSYMBOL_443_70 = 443,                   /* $@70  */
  YYSYMBOL_444_71 = 444,                   /* $@71  */
  YYSYMBOL_445_72 = 445,                   /* $@72  */
  YYSYMBOL_446_73 = 446,                   /* $@73  */
  YYSYMBOL_447_74 = 447,                   /* $@74  */
  YYSYMBOL_448_75 = 448,                   /* $@75  */
  YYSYMBOL_449_76 = 449,                   /* $@76  */
  YYSYMBOL_type_declaration = 450,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 451,  /* tuple_alias_declaration  */
  YYSYMBOL_452_77 = 452,                   /* $@77  */
  YYSYMBOL_453_78 = 453,                   /* $@78  */
  YYSYMBOL_454_79 = 454,                   /* $@79  */
  YYSYMBOL_455_80 = 455,                   /* $@80  */
  YYSYMBOL_variant_alias_declaration = 456, /* variant_alias_declaration  */
  YYSYMBOL_457_81 = 457,                   /* $@81  */
  YYSYMBOL_458_82 = 458,                   /* $@82  */
  YYSYMBOL_459_83 = 459,                   /* $@83  */
  YYSYMBOL_460_84 = 460,                   /* $@84  */
  YYSYMBOL_bitfield_alias_declaration = 461, /* bitfield_alias_declaration  */
  YYSYMBOL_462_85 = 462,                   /* $@85  */
  YYSYMBOL_463_86 = 463,                   /* $@86  */
  YYSYMBOL_464_87 = 464,                   /* $@87  */
  YYSYMBOL_465_88 = 465,                   /* $@88  */
  YYSYMBOL_make_decl = 466,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 467,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 468,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 469,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim_list = 470,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 471,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 472, /* optional_make_struct_dim_decl  */
  YYSYMBOL_use_initializer = 473,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 474,         /* make_struct_decl  */
  YYSYMBOL_475_89 = 475,                   /* $@89  */
  YYSYMBOL_476_90 = 476,                   /* $@90  */
  YYSYMBOL_477_91 = 477,                   /* $@91  */
  YYSYMBOL_478_92 = 478,                   /* $@92  */
  YYSYMBOL_479_93 = 479,                   /* $@93  */
  YYSYMBOL_480_94 = 480,                   /* $@94  */
  YYSYMBOL_481_95 = 481,                   /* $@95  */
  YYSYMBOL_482_96 = 482,                   /* $@96  */
  YYSYMBOL_make_map_tuple = 483,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 484,          /* make_tuple_call  */
  YYSYMBOL_485_97 = 485,                   /* $@97  */
  YYSYMBOL_486_98 = 486,                   /* $@98  */
  YYSYMBOL_make_dim_decl = 487,            /* make_dim_decl  */
  YYSYMBOL_488_99 = 488,                   /* $@99  */
  YYSYMBOL_489_100 = 489,                  /* $@100  */
  YYSYMBOL_490_101 = 490,                  /* $@101  */
  YYSYMBOL_491_102 = 491,                  /* $@102  */
  YYSYMBOL_492_103 = 492,                  /* $@103  */
  YYSYMBOL_493_104 = 493,                  /* $@104  */
  YYSYMBOL_494_105 = 494,                  /* $@105  */
  YYSYMBOL_495_106 = 495,                  /* $@106  */
  YYSYMBOL_496_107 = 496,                  /* $@107  */
  YYSYMBOL_497_108 = 497,                  /* $@108  */
  YYSYMBOL_expr_map_tuple_list = 498,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 499,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 500, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 501,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 502       /* array_comprehension  */
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
#define YYLAST   11739

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  208
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  295
/* YYNRULES -- Number of rules.  */
#define YYNRULES  840
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1534

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
       0,   551,   551,   552,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,   573,   574,   578,
     579,   583,   589,   590,   591,   595,   596,   600,   618,   619,
     620,   621,   625,   626,   630,   635,   644,   652,   668,   673,
     681,   681,   720,   738,   742,   745,   749,   755,   764,   767,
     773,   774,   778,   782,   783,   787,   790,   796,   802,   805,
     811,   812,   816,   817,   821,   822,   826,   827,   828,   837,
     838,   842,   843,   849,   850,   851,   852,   853,   857,   858,
     862,   863,   867,   869,   867,   881,   881,   889,   889,   905,
     912,   912,   921,   929,   929,   942,   942,   954,   961,   962,
     963,   964,   965,   966,   970,   975,   983,   984,   985,   989,
     990,   991,   992,   993,   994,   995,   996,  1002,  1005,  1011,
    1014,  1020,  1021,  1022,  1026,  1039,  1057,  1060,  1068,  1079,
    1090,  1101,  1104,  1111,  1115,  1122,  1123,  1127,  1128,  1132,
    1133,  1134,  1138,  1141,  1148,  1152,  1153,  1154,  1155,  1156,
    1157,  1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,  1166,
    1167,  1168,  1169,  1170,  1171,  1172,  1173,  1174,  1175,  1176,
    1177,  1178,  1179,  1180,  1181,  1182,  1183,  1184,  1185,  1186,
    1187,  1188,  1189,  1190,  1191,  1192,  1193,  1194,  1195,  1196,
    1197,  1198,  1199,  1200,  1201,  1202,  1203,  1204,  1205,  1206,
    1207,  1208,  1209,  1210,  1211,  1212,  1213,  1214,  1215,  1216,
    1217,  1218,  1219,  1220,  1221,  1222,  1223,  1224,  1225,  1226,
    1227,  1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,  1240,
    1258,  1259,  1260,  1264,  1270,  1270,  1287,  1290,  1292,  1290,
    1300,  1302,  1300,  1317,  1330,  1331,  1332,  1333,  1334,  1335,
    1336,  1337,  1338,  1339,  1340,  1341,  1342,  1343,  1344,  1345,
    1346,  1347,  1348,  1352,  1357,  1363,  1369,  1370,  1374,  1375,
    1379,  1383,  1390,  1391,  1402,  1406,  1409,  1417,  1417,  1417,
    1420,  1426,  1429,  1433,  1437,  1444,  1451,  1457,  1461,  1465,
    1468,  1471,  1479,  1482,  1490,  1496,  1497,  1498,  1502,  1503,
    1507,  1508,  1512,  1517,  1525,  1531,  1543,  1546,  1549,  1555,
    1555,  1555,  1558,  1558,  1558,  1563,  1563,  1563,  1571,  1571,
    1571,  1577,  1587,  1598,  1613,  1616,  1622,  1623,  1630,  1641,
    1642,  1643,  1647,  1648,  1649,  1650,  1651,  1655,  1660,  1668,
    1669,  1673,  1680,  1684,  1690,  1691,  1692,  1693,  1694,  1695,
    1696,  1700,  1701,  1702,  1703,  1704,  1705,  1706,  1707,  1708,
    1709,  1710,  1711,  1712,  1713,  1714,  1715,  1716,  1717,  1718,
    1722,  1729,  1741,  1746,  1756,  1760,  1767,  1770,  1770,  1770,
    1775,  1775,  1775,  1788,  1792,  1796,  1801,  1808,  1817,  1822,
    1829,  1829,  1829,  1836,  1840,  1850,  1859,  1868,  1872,  1875,
    1881,  1882,  1883,  1884,  1885,  1886,  1887,  1888,  1889,  1890,
    1891,  1892,  1893,  1894,  1895,  1896,  1897,  1898,  1899,  1900,
    1901,  1902,  1903,  1904,  1905,  1906,  1907,  1908,  1909,  1910,
    1911,  1912,  1913,  1914,  1915,  1916,  1922,  1923,  1924,  1925,
    1926,  1939,  1940,  1941,  1942,  1943,  1944,  1945,  1946,  1947,
    1948,  1949,  1950,  1951,  1952,  1955,  1955,  1955,  1958,  1963,
    1967,  1971,  1971,  1971,  1976,  1979,  1983,  1983,  1983,  1988,
    1991,  1992,  1993,  1994,  1995,  1996,  1997,  1998,  1999,  2001,
    2005,  2006,  2014,  2017,  2020,  2029,  2030,  2031,  2032,  2033,
    2034,  2035,  2039,  2043,  2047,  2051,  2055,  2059,  2063,  2067,
    2071,  2078,  2079,  2083,  2084,  2085,  2089,  2090,  2094,  2095,
    2096,  2100,  2101,  2105,  2116,  2119,  2120,  2120,  2139,  2138,
    2153,  2152,  2169,  2178,  2188,  2189,  2193,  2196,  2205,  2206,
    2210,  2213,  2217,  2231,  2240,  2241,  2245,  2248,  2252,  2266,
    2267,  2271,  2277,  2283,  2286,  2290,  2299,  2300,  2301,  2305,
    2306,  2310,  2317,  2322,  2331,  2337,  2348,  2355,  2364,  2367,
    2370,  2377,  2380,  2385,  2396,  2399,  2404,  2416,  2417,  2421,
    2422,  2423,  2427,  2430,  2433,  2433,  2453,  2456,  2456,  2474,
    2479,  2487,  2488,  2492,  2495,  2508,  2525,  2526,  2527,  2532,
    2532,  2558,  2562,  2563,  2564,  2568,  2578,  2581,  2587,  2588,
    2592,  2593,  2597,  2598,  2602,  2604,  2609,  2602,  2625,  2626,
    2630,  2631,  2635,  2641,  2642,  2643,  2644,  2648,  2649,  2650,
    2654,  2657,  2663,  2665,  2670,  2663,  2691,  2698,  2703,  2712,
    2718,  2729,  2730,  2731,  2732,  2733,  2734,  2735,  2736,  2737,
    2738,  2739,  2740,  2741,  2742,  2743,  2744,  2745,  2746,  2747,
    2748,  2749,  2750,  2751,  2752,  2753,  2754,  2755,  2759,  2760,
    2761,  2762,  2763,  2764,  2765,  2766,  2770,  2781,  2785,  2792,
    2804,  2811,  2820,  2825,  2835,  2848,  2848,  2848,  2861,  2862,
    2866,  2870,  2877,  2881,  2885,  2889,  2896,  2899,  2917,  2918,
    2919,  2920,  2921,  2921,  2921,  2925,  2930,  2937,  2937,  2944,
    2948,  2952,  2957,  2962,  2967,  2972,  2976,  2980,  2985,  2989,
    2993,  2998,  2998,  2998,  3004,  3011,  3011,  3011,  3016,  3016,
    3016,  3022,  3022,  3022,  3027,  3031,  3031,  3031,  3036,  3036,
    3036,  3045,  3049,  3049,  3049,  3054,  3054,  3054,  3063,  3067,
    3067,  3067,  3072,  3072,  3072,  3081,  3081,  3081,  3087,  3087,
    3087,  3096,  3099,  3110,  3126,  3128,  3133,  3138,  3126,  3164,
    3166,  3171,  3177,  3164,  3203,  3205,  3210,  3215,  3203,  3246,
    3247,  3248,  3249,  3250,  3254,  3261,  3268,  3274,  3280,  3287,
    3294,  3300,  3309,  3315,  3323,  3328,  3335,  3340,  3346,  3347,
    3351,  3352,  3356,  3356,  3356,  3364,  3364,  3364,  3371,  3371,
    3371,  3378,  3378,  3378,  3389,  3395,  3401,  3407,  3407,  3407,
    3417,  3426,  3426,  3426,  3436,  3436,  3436,  3446,  3446,  3446,
    3456,  3465,  3465,  3465,  3485,  3492,  3492,  3492,  3502,  3505,
    3511,  3519,  3527,  3547,  3572,  3573,  3577,  3578,  3582,  3585,
    3588
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
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "reader_character_sequence", "expr_reader", "$@1",
  "options_declaration", "require_declaration", "require_module_name",
  "require_module", "is_public_module", "expect_declaration",
  "expect_list", "expect_error", "expression_label", "expression_goto",
  "elif_or_static_elif", "emit_semis", "optional_emit_semis",
  "expression_else", "if_or_static_if", "expression_else_one_liner",
  "expression_if_one_liner", "semis", "optional_semis",
  "expression_if_block", "$@2", "$@3", "$@4", "expression_if_then_else",
  "$@5", "expression_if_then_else_oneliner", "expression_for_loop", "$@6",
  "expression_unsafe", "expression_while_loop", "$@7", "expression_with",
  "$@8", "expression_with_alias", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "metadata_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_annotation_list_with_emit_semis",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@9", "expression_block_finally", "$@10",
  "$@11", "expression_block", "$@12", "$@13", "expr_call_pipe",
  "expression_any", "expressions", "optional_expr_list",
  "optional_expr_map_tuple_list", "type_declaration_no_options_list",
  "name_in_namespace", "expression_delete", "new_type_declaration", "$@14",
  "$@15", "expr_new", "expression_break", "expression_continue",
  "expression_return", "expression_yield", "expression_try_catch",
  "kwd_let_var_or_nothing", "kwd_let", "optional_in_scope",
  "tuple_expansion", "tuple_expansion_variable_declaration",
  "expression_let", "expr_cast", "$@16", "$@17", "$@18", "$@19", "$@20",
  "$@21", "expr_type_decl", "$@22", "$@23", "expr_type_info", "expr_list",
  "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_full_block",
  "expr_full_block_assumed_piped", "expr_numeric_const", "expr_assign",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@24", "$@25", "$@26", "$@27", "expr_field", "$@28",
  "$@29", "expr_call", "expr", "$@30", "$@31", "$@32", "$@33", "$@34",
  "$@35", "expr_generator", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@36", "$@37", "$@38",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list",
  "global_let_variable_name_with_pos_list", "variable_declaration_list",
  "let_variable_declaration", "global_let_variable_declaration",
  "optional_shared", "optional_public_or_private_variable",
  "global_variable_declaration_list", "$@39", "global_let", "$@40",
  "enum_expression", "commas", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@41",
  "alias_declaration", "optional_public_or_private_enum", "enum_name",
  "optional_enum_basic_type_declaration", "optional_commas", "emit_commas",
  "optional_emit_commas", "enum_declaration", "$@42", "$@43", "$@44",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@45", "$@46", "$@47", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_alias_bits", "bitfield_type_declaration", "$@48", "$@49",
  "c_or_s", "table_type_pair", "dim_list", "type_declaration_no_options",
  "type_declaration_no_options_no_dim", "$@50", "$@51", "$@52", "$@53",
  "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61", "$@62",
  "$@63", "$@64", "$@65", "$@66", "$@67", "$@68", "$@69", "$@70", "$@71",
  "$@72", "$@73", "$@74", "$@75", "$@76", "type_declaration",
  "tuple_alias_declaration", "$@77", "$@78", "$@79", "$@80",
  "variant_alias_declaration", "$@81", "$@82", "$@83", "$@84",
  "bitfield_alias_declaration", "$@85", "$@86", "$@87", "$@88",
  "make_decl", "make_struct_fields", "make_variant_dim",
  "make_struct_single", "make_struct_dim_list", "make_struct_dim_decl",
  "optional_make_struct_dim_decl", "use_initializer", "make_struct_decl",
  "$@89", "$@90", "$@91", "$@92", "$@93", "$@94", "$@95", "$@96",
  "make_map_tuple", "make_tuple_call", "$@97", "$@98", "make_dim_decl",
  "$@99", "$@100", "$@101", "$@102", "$@103", "$@104", "$@105", "$@106",
  "$@107", "$@108", "expr_map_tuple_list", "make_table_decl",
  "array_comprehension_where", "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1308)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-743)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1308,    37, -1308, -1308,    86,    -4,   117,   571, -1308,   -55,
   -1308, -1308, -1308, -1308,   206,    87, -1308, -1308, -1308, -1308,
     132,   132,   132, -1308,    68,   175, -1308,   135, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,    66, -1308,
     105,    76,   121, -1308, -1308,   117,    18, -1308, -1308, -1308,
     169,   132, -1308, -1308,   135,   571,   571,   571,   199,   274,
   -1308, -1308, -1308,    87,    87,    87,   271, -1308,   605,   216,
   -1308, -1308, -1308, -1308, -1308,   584,    48, -1308,   589,    33,
      86,   345,    -4,   316,   378, -1308,   419,   432, -1308, -1308,
   -1308,   610,   453,   458,   496, -1308,   526,   254, -1308, -1308,
     -27,    86,    87,    87,    87,    87,    -2,   612, -1308, -1308,
   -1308, -1308, -1308, -1308,   452,   622, -1308, -1308,   510, -1308,
   -1308, -1308, -1308, -1308,   606,   104, -1308, -1308, -1308, -1308,
     599, -1308, -1308,   556, -1308, -1308, -1308,   569,   587,   587,
   -1308, -1308,   586, -1308,   157, -1308,   554,   619,   605, -1308,
     602, -1308, -1308, -1308,   598,  1890, -1308, -1308, -1308, -1308,
   -1308, -1308,   567, -1308, -1308, -1308,   611, -1308, -1308, -1308,
   -1308, -1308, -1308, -1308,   162, -1308,  7085, -1308,   608, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308,   600, 10711, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,
   -1308, -1308, -1308,   746,   751, -1308,   588,   587,   681,   -40,
      86,   590,   627, -1308, -1308, -1308,   104, -1308,   614,   615,
     617,   591,   618,   620, -1308, -1308, -1308,   592, -1308, -1308,
   -1308, -1308, -1308,   621, -1308, -1308, -1308, -1308, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308,   623, -1308, -1308, -1308,   628,
     630, -1308, -1308, -1308, -1308,   631,   632,   604,   206, -1308,
   -1308, -1308, -1308, -1308, -1308,    93,   625, -1308,   594,   609,
     613,   650,   569,   644,   652, -1308, -1308, -1308, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308,   653,   626, -1308, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,   791,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,
   -1308, -1308,   654,   633, -1308, -1308,   -52,   643, -1308, -1308,
     657,   587, -1308, -1308,   567,   587,    86, -1308,   226, -1308,
   -1308, -1308, -1308, -1308,  6653, -1308, -1308,   658, -1308,   369,
     380,   390, -1308, -1308,  6653,   332, -1308, -1308, -1308,    13,
   -1308, -1308, -1308,     2,  3788, -1308,   629,   951, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,
     634, -1308, -1308, -1308, -1308,   659,   690, -1308,   635, -1308,
      79, -1308,   402,  7085, -1308, -1308,   638,   648, -1308,   -74,
   -1308,   587,   667,  7085, -1308,   -26,  7085,  7085,  7085,   651,
     656, -1308, -1308,   238,   206,   660,    34, -1308,   354,   637,
     665,   666,   655,   670,   661,   383,   671, -1308,   385,   672,
     676,  6653,  6653,   662,   664,   668,   669,   673,   674, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,  6653,  6653,
    6653,  6653,  6653,  3406,  6653, -1308,   663,  3979, -1308, -1308,
   -1308,   675, -1308, -1308, -1308, -1308,   636, -1308, -1308, -1308,
   -1308, -1308, -1308,   221,  7267, -1308, -1308,   678, -1308, -1308,
   -1308, -1308, -1308, -1308,  7085,  7085,   679,   694,  7085,   588,
    7085,   588,  7085,   588,  7178,   695,  7353, -1308,  6653, -1308,
   -1308, -1308, -1308,   639, -1308, -1308, 10191,  4170, -1308, -1308,
     696,  7178,   695, -1308, -1308, -1308,  6653, -1308, -1308,   362,
   -1308,   -59,   625, -1308,   206, -1308,   466,   682,   790,   475,
   -1308, -1308, -1308,   -43, -1308, -1308, -1308,  6653,     5,   491,
     684,   317, -1308, -1308, -1308,   683, -1308, -1308,   401, -1308,
     680,   687,   691, -1308,  6653,  7085,  6653,  6653, -1308, -1308,
    6653, -1308,  6653, -1308,  6653, -1308, -1308,  6653, -1308,  7085,
      41,    41,  6653,  6653,  6653,  6653,  6653,  6653,   596,    41,
      41,   -70,    41,    41,   686,   859,   706, 11043,   706,   259,
     688, 10794, -1308,   708,  3024,    87,   880,   698, -1308,   636,
   -1308, 10866, 11421,  6653,  6653, -1308, -1308,  6653,  6653,  6653,
    6653,   724,  6653,   119,  6653,  6653,  6653,  6653,  6653,  6653,
    6653,  6653,  6653,  4361,  6653,  6653,  6653,  6653,  6653,  6653,
    6653,  6653,  6653,  6653, 11541,  6653, -1308,  4552,   503,   518,
   -1308, -1308,   144,   531,   643,   532,   643,   533,   643,    19,
   -1308,   403,   625,   714, -1308,   413, -1308,  7085,   697,   706,
   -1308, -1308, -1308, 10275, -1308,   371, -1308,   132, -1308,   132,
     736,  7439,   702,   848, -1308,   110, -1308,  2013, -1308, -1308,
   -1308, -1308,  6653,   741,   742,  7085,   -26, -1308,  6653,  1734,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308,  7085,  7085,  7085,
    7085,  3597,   744,  6653,  7085, -1308, -1308, -1308,  7085,   706,
     446,   708,  7525,  7085,  7085,  7611,  7085,  7697,  7085,   706,
    7085,  7085,   706,  7085,   729,  7783,  7869,  7955,  8041,  8127,
    8213, -1308,  6653,   515,   362,   709,  6653,   712,   711,   715,
     716,   415, -1308, -1308,   362,  6653,  6653,   719,   103,   722,
     253,  3215, -1308,   192,   753,   713,   261,   730,   588, -1308,
    2219,   880,   737,   731, -1308, -1308,   749,   732, -1308, -1308,
    1550,  1550,   333,   333, 11356, 11356,   733,   190,   734, -1308,
   10359,   185,   185,   678,  1550,  1550,  1527,  1253, 11158, 11126,
   11510, 10877, 11241, 11273,  1343,   333,   333,   507,   507,   190,
     190,   190,   386,  6653,   738,   739,   434,  6653,   936,   740,
   10443, -1308,   194, -1308, -1308, -1308,   778, -1308,   761, -1308,
     762, -1308,   763,  7085, -1308,  7178,  7085, -1308,   695,   459,
     625, -1308, -1308, -1308, -1308,   449, -1308, -1308,  6990, -1308,
     196, -1308,   771, -1308,   371, -1308,  6653,   783, -1308,   784,
   -1308,  7085, -1308,  6653, -1308,  4743, -1308, -1308,  4934,   785,
   -1308,   132,   793,  5125,   -97,  5316, -1308,   132,   132,   940,
   -1308,   717, -1308, -1308,   938, -1308, -1308,   943, -1308,   912,
     132,   750, -1308,   132,   132,   132,   132,   132, -1308,   889,
   -1308,   132,  1650,    22,  8299, -1308,   914,   -43,  6653,  1734,
   -1308, -1308, -1308, -1308,   625,   777,   779,   541,   451,   202,
     760,   764,   493,  8385,   558,  7085,  7085,  7085,   781,   765,
     767,  7085,   768, -1308,   782,   787, -1308,   792, -1308,   794,
     773,   796,   797,   780,   798,   880, -1308, -1308, -1308, -1308,
   -1308,   772, 10960,    25,   362, 11043, -1308, -1308,  6653,  6653,
    7085,   588,    52, 11043, -1308, -1308,   152, -1308,   786,  6653,
    6653,  6653,   280,  6844, -1308,   455, -1308, -1308,   165,   643,
   -1308,   588, -1308,  6653, -1308,  6653,  5507,  6653, -1308,   800,
     789, -1308, -1308,  6653,   795, -1308, 10527,  6653,  5698,   799,
   -1308, 10611, -1308,  5889, -1308,  6653, -1308, -1308, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308,   625, -1308, -1308,
     625, -1308, -1308,   801,  7085, -1308, -1308,   758, -1308, -1308,
     788, -1308,   806,  6653,   480, -1308,  8471, -1308,   941,   -39,
   11043,  6653, 11043,   969,  6653, 11043,   811, -1308,   820,   841,
   11043, -1308,  6653, 11043, -1308, -1308,   802, -1308, -1308,   803,
     804,   805,   813, -1308,   961, -1308, -1308, -1308, -1308, -1308,
   -1308,   -82, -1308,  6653,  6653,  6653,  6653,  6653,  6653,  6653,
    6653,  6653,  6653,  6653,  6653,  6653,  6653,  6653,  6653,  6653,
    6653, -1308,    87, -1308,   998,   567, -1308,   851, -1308,  6653,
    1734, -1308, -1308, -1308, -1308,   818, -1308, -1308, -1308,   821,
     857, -1308, -1308,   874,   875,   876, -1308, -1308,  6653,   877,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,
     -32,  6080, -1308,  6653,    56,  8557,  8643,   878,   643,  6653,
   -1308,  8729, 11043, 11043,   860,  3215,   862,   168,   897,   900,
     864,   902,   904, -1308,   224,   587,   643,  7085,  8815,  7085,
    8901, -1308,   241,  8987, -1308,  6653,  1086,  6653, -1308,  9073,
    3215, -1308,   246,  6653, -1308, -1308, -1308,   247, -1308, -1308,
   -1308,  6653,   625, -1308, -1308, -1308, 11043, -1308,   863, -1308,
     905,  6653, 11043, -1308, 11043,  6653, -1308, -1308, -1308, 11043,
    6653,  6653,   362,  6653,  6653, -1308, -1308,   907, -1308, -1308,
   -1308, 11043, 11043, 11043, 11043, 11043, 11043, 11043, 11043, 11043,
   11043, 11043, 11043, 11043, 11043, 11043, 11043, 11043, 11043,   281,
     624,  1005,   132, -1308,  1734, -1308,   873,   879, -1308, -1308,
    6653,   893, -1308, -1308, -1308, -1308,   881,   882,   708,   884,
    6653,  6653,  6653,   886,  1006,   887,   888,  6271, -1308, -1308,
     255,   265,  6653, -1308, -1308, -1308,   896,   297,   257, -1308,
     334,  6653,  6653,  6653, -1308, -1308,   915, -1308, -1308,   165,
   -1308,   942,   587,   901, -1308,   561, -1308, -1308, -1308,  7085,
    9159,  9245, -1308,   347, -1308,  9331, -1308,   890, -1308, -1308,
   11043, -1308, 11043,  9417,  9503,    63,  9589,  9675,   892, -1308,
     301,   -63,   587, -1308, -1308,   553, -1308,     6, -1308, -1308,
    1006,  1006,  9761,   895,   898,   906,   909,  6653, -1308, -1308,
    6653,   333,   333,   333,  6653, -1308, -1308,  1006,   451, -1308,
    9847, -1308, -1308,   885,   302,   142, -1308,   908,  6653,  6653,
     891,  9933, 11043, 11043,   899, -1308,  6462, -1308, -1308, -1308,
   -1308, -1308,   581, -1308, -1308,   903, -1308, -1308,  1081,   587,
    6653,   587,   587, -1308,   939,   372, -1308, -1308, -1308,  1068,
       6, -1308, -1308,   624,   -84,   -84, -1308,  6653,  1006,  1006,
     451,   910,   916,   706,   -84,   753,   919, -1308,  6653,   918,
   -1308,   142,  6653, 11043, 11043, -1308,   258, -1308,  6653, 11126,
   -1308, -1308, -1308, -1308, -1308,  2833,   132,   920,   303, -1308,
   -1308,  2425, -1308,  7085,   -26, -1308, -1308,  1068,   362,   451,
     753,   932, -1308,   922,   923, 10019,   -84,   -84,   925, -1308,
   -1308,   926,   927, -1308,  7174,  6653, -1308,   933,  6653,  6653,
    1086, -1308, -1308, -1308, -1308, -1308, -1308, 11043, -1308,   499,
     924,  2833,   587, -1308, -1308,   934,   -26,  6653,  1890, -1308,
   -1308,   304,   913, -1308, -1308, -1308, -1308,   929,   937, -1308,
   -1308, -1308,  1057,   944,  7174,   935, 11043, 11043,   587, -1308,
   -1308,   946, -1308, -1308,   132, -1308, -1308,  6653,  1734,   132,
    1890, -1308,   451, -1308, -1308,  6653, -1308,   945, -1308, -1308,
    6653,  2631, -1308, -1308,  1734, -1308, -1308,   587,   305, 11043,
   -1308, -1308, 10105,   948, -1308, -1308, -1308,   587,   961, -1308,
   -1308, -1308,   499, -1308
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   137,     1,   298,     0,     0,     0,   586,   299,     0,
     764,   754,   759,    20,     0,     0,    19,    16,    15,     3,
       0,     0,     0,     8,     0,   622,     7,   567,     6,    11,
       5,     4,    13,    12,    14,   107,   108,   106,   115,   117,
      42,    55,    52,    53,    44,     0,    50,    43,   588,   587,
       0,     0,    26,    25,   567,   586,   586,   586,     0,   272,
      40,   122,   123,     0,     0,     0,   124,   126,   133,     0,
     121,    21,    10,     9,   604,   230,     0,   568,   569,     0,
       0,     0,     0,    45,     0,    51,     0,     0,    48,   589,
     591,    22,     0,     0,     0,   274,     0,     0,   132,   127,
       0,     0,     0,     0,     0,     0,    64,   592,   232,   231,
     234,   229,   614,   613,     0,   617,   571,   570,   577,   113,
     114,   111,   112,   110,     0,     0,   109,   118,    56,    54,
      50,    47,    46,     0,    23,    24,    27,   602,    64,    64,
     273,    38,    41,   131,     0,   128,   129,   130,   134,    62,
      65,   138,   594,   593,     0,     0,   616,   615,   619,   618,
     623,   572,   501,    28,    29,    33,     0,   102,   103,   100,
     101,    99,    98,   104,     0,    49,     0,   600,   603,   765,
     755,   760,    39,   125,    63,   595,   596,     0,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,     0,     0,   144,   139,    64,   610,   574,
       0,   502,     0,    30,    31,    32,     0,   116,     0,     0,
       0,     0,     0,     0,   631,   651,   632,   667,   633,   637,
     638,   639,   640,   657,   644,   645,   646,   647,   648,   649,
     650,   652,   653,   654,   655,   724,   636,   643,   656,   731,
     738,   634,   641,   635,   642,     0,     0,     0,     0,   666,
     688,   691,   689,   690,   751,   686,   590,   601,     0,     0,
       0,     0,   602,   198,   199,   196,   147,   148,   150,   149,
     151,   152,   153,   154,   180,   181,   178,   179,   171,   182,
     183,   172,   169,   170,   197,   191,     0,   195,   184,   185,
     186,   187,   158,   159,   160,   155,   156,   157,   168,     0,
     174,   175,   173,   166,   167,   162,   161,   163,   164,   165,
     146,   145,   190,     0,   176,   177,   501,   142,   240,   611,
       0,    64,   576,   573,   501,    64,     0,   556,   549,   578,
     105,   692,   715,   718,     0,   721,   711,     0,   675,   725,
     732,   739,   745,   748,     0,     0,   701,   706,   700,     0,
     714,   710,   703,     0,     0,   705,   687,     0,   766,   756,
     761,   658,   661,   664,   665,   659,   662,   660,   663,   597,
       0,   200,   201,   194,   189,   202,   192,   188,     0,   140,
     297,   524,     0,     0,   233,   235,     0,   608,   624,     0,
     119,    64,     0,     0,   550,     0,     0,     0,     0,     0,
       0,   406,   407,     0,     0,     0,     0,   400,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   657,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   490,
     344,   346,   345,   347,   348,   349,   350,    34,     0,     0,
       0,     0,     0,     0,     0,   329,   330,     0,   404,   403,
     480,   401,   473,   472,   471,   470,   135,   476,   402,   475,
     474,   447,   408,   448,     0,   452,   409,     0,   405,   769,
     773,   770,   771,   772,     0,     0,     0,     0,     0,   139,
       0,   139,     0,   139,     0,     0,     0,   697,   266,   708,
     709,   702,   704,     0,   707,   683,     0,     0,   753,   752,
     672,   530,   536,   605,   203,   193,     0,   295,   296,     0,
     141,   501,   143,   263,     0,   612,     0,     0,   551,   549,
     575,   120,   557,     0,   547,   548,   546,     0,     0,     0,
       0,   680,   792,   795,   277,   281,   280,   286,     0,   318,
       0,     0,     0,   821,     0,     0,     0,     0,   309,   312,
       0,   315,     0,   825,     0,   801,   807,     0,   798,     0,
     436,   437,     0,     0,     0,     0,     0,     0,     0,   413,
     412,   449,   411,   410,     0,     0,   836,   324,   836,   331,
       0,   805,   828,   836,     0,     0,   339,   330,   263,   135,
     243,     0,     0,     0,     0,   438,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   390,     0,   695,     0,     0,     0,
     668,   670,     0,     0,   142,     0,   142,     0,   142,   272,
     528,     0,   526,     0,   534,     0,   669,     0,     0,   836,
     699,   682,   685,     0,   673,   598,   531,    80,   537,    80,
     583,     0,     0,   626,   522,   541,   525,     0,   609,   620,
     514,   625,     0,     0,     0,     0,     0,   564,     0,     0,
     693,   716,   719,    18,    17,   678,   679,     0,     0,     0,
       0,   790,     0,     0,     0,   811,   814,   817,     0,   836,
       0,   836,     0,     0,     0,     0,     0,     0,     0,   836,
       0,     0,   836,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,    35,     0,     0,   837,     0,     0,     0,
       0,   377,   374,   376,     0,     0,   837,     0,     0,     0,
     272,     0,   393,     0,   783,     0,     0,     0,   139,   331,
       0,   339,     0,     0,   459,   458,     0,     0,   460,   464,
     414,   415,   427,   428,   425,   426,     0,   453,     0,   445,
       0,   477,   478,   479,   416,   417,   432,   433,   434,   435,
       0,     0,   430,   431,   429,   423,   424,   419,   418,   420,
     421,   422,     0,     0,     0,   383,     0,     0,     0,     0,
       0,   398,     0,   722,   712,   676,     0,   726,     0,   733,
       0,   740,     0,     0,   746,     0,     0,   749,     0,     0,
     270,   696,   267,   684,   581,   599,   767,    78,    81,   757,
      81,   762,   579,   584,   598,   523,     0,     0,   540,     0,
     539,     0,   542,     0,   265,   289,   287,   240,     0,     0,
     288,     0,     0,     0,    64,     0,   244,     0,     0,     0,
     257,     0,   258,   252,     0,   249,   248,     0,   250,     0,
       0,     0,   264,     0,    76,    77,    74,    75,   259,   301,
     247,     0,   351,   516,     0,   553,   554,     0,     0,     0,
     566,   694,   717,   720,   681,     0,     0,     0,   791,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   450,     0,     0,   451,     0,   481,     0,
       0,     0,     0,     0,     0,   339,   485,   486,   487,   488,
     489,     0,     0,     0,     0,   325,   810,   440,     0,     0,
       0,   139,     0,   804,   829,   830,     0,   394,     0,     0,
       0,     0,     0,     0,   397,     0,   395,   136,     0,   142,
     343,   139,   455,     0,   461,     0,     0,     0,   443,     0,
       0,   465,   469,     0,     0,   446,     0,     0,     0,     0,
     384,     0,   391,     0,   441,     0,   399,   723,   713,   677,
     671,   727,   729,   734,   736,   741,   743,   527,   747,   529,
     533,   750,   535,     0,     0,   674,   582,     0,    79,   532,
       0,   538,     0,     0,   599,   606,     0,   628,   629,   543,
     545,     0,   290,     0,     0,   275,     0,   262,     0,     0,
      59,   240,     0,   292,   260,   261,     0,    69,    70,     0,
       0,     0,     0,   251,   236,   246,   253,   254,   255,   256,
     300,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   515,     0,   621,     0,   501,   552,     0,   561,     0,
       0,   565,   793,   796,   278,     0,   283,   284,   282,     0,
       0,   321,   319,     0,     0,     0,   822,   820,   268,     0,
     831,   310,   313,   316,   826,   824,   802,   808,   806,   799,
      64,     0,    36,     0,     0,     0,     0,     0,   142,     0,
     396,     0,   775,   774,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   337,     0,    64,   142,     0,     0,     0,
       0,   372,     0,     0,   466,     0,   454,     0,   444,     0,
       0,   385,     0,     0,   442,   392,   388,     0,   730,   737,
     744,   266,   271,   768,   758,   763,   580,   585,     0,   627,
       0,     0,   291,   240,   276,     0,    57,    58,    92,   293,
       0,     0,     0,     0,     0,   237,   242,     0,   558,   307,
     306,   361,   362,   364,   363,   365,   355,   356,   357,   366,
     367,   353,   354,   368,   369,   358,   359,   360,   352,     0,
     508,   511,     0,   555,     0,   563,     0,     0,   279,   285,
       0,     0,   320,   812,   815,   818,     0,     0,   836,     0,
       0,     0,     0,     0,   790,     0,     0,     0,   240,   491,
       0,     0,     0,   375,   500,   378,     0,     0,     0,   370,
       0,     0,     0,     0,   334,   335,     0,   333,   332,     0,
     340,   240,    64,     0,   499,     0,   497,   373,   494,     0,
       0,     0,   493,     0,   386,     0,   389,     0,   607,   630,
     544,   294,    97,     0,     0,     0,     0,     0,     0,   302,
       0,     0,    64,   509,   510,   511,   512,   503,   517,   562,
     790,   790,     0,     0,     0,     0,     0,   266,   832,   269,
     268,   311,   314,   317,     0,   791,   803,   790,     0,   482,
       0,   484,   492,     0,     0,     0,   381,     0,     0,     0,
       0,     0,   777,   776,     0,   338,     0,   326,   341,   240,
     456,   462,     0,   498,   496,     0,   495,   698,    71,    64,
       0,    64,    64,   263,     0,   549,   308,   559,   560,   506,
     503,   504,   505,   508,   789,   789,   322,     0,   790,   790,
       0,     0,     0,   836,   789,   782,     0,   483,     0,     0,
     379,     0,     0,   779,   778,   371,     0,   336,     0,   327,
     342,   457,   463,   467,   387,     0,     0,    85,     0,   240,
     240,     0,   303,     0,     0,   507,   518,   506,     0,     0,
     786,   836,   788,     0,     0,     0,   789,   789,     0,   823,
     833,     0,     0,   800,   834,     0,   382,   834,     0,     0,
     328,   468,    72,    76,    77,    74,    75,    73,    89,    66,
       0,     0,    64,    94,    96,     0,     0,     0,     0,   520,
     513,     0,   837,   787,   794,   797,   323,     0,     0,   819,
     827,   809,     0,     0,   834,     0,   781,   780,    64,    60,
      61,     0,    88,   263,     0,   240,   239,     0,     0,     0,
       0,   784,     0,   813,   816,     0,   838,     0,   840,   240,
       0,     0,    86,    91,     0,   305,   519,    64,     0,   835,
     839,    67,     0,     0,   304,   240,   785,    64,   236,   521,
     240,    84,    66,    68
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1308, -1308,  -661,    -1, -1308, -1308, -1308, -1308,   551,  1063,
   -1308, -1308, -1308,  1146, -1308, -1308, -1308,  1103, -1308,  1019,
   -1308, -1308,  1069, -1308, -1308, -1308, -1308,  -137,  -377, -1308,
   -1308, -1305,   477,   478, -1308, -1308, -1308, -1308, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,   967,
   -1308, -1308,   -58,  1058, -1308, -1308, -1308,   381,  -592,  -433,
     262,  -472,  -630, -1308, -1308, -1308, -1246, -1308, -1308,  -368,
   -1308, -1308,  -789, -1308, -1308, -1308, -1308,  -596, -1126,  -169,
   -1308,    -7, -1308, -1308, -1308, -1308, -1308, -1162, -1159, -1151,
   -1144, -1308, -1308,  1163, -1308, -1308, -1308, -1308, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,  -399,
   -1308,   689,  -109, -1308,  -729, -1308, -1308, -1308, -1308, -1308,
   -1308, -1307, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,
   -1308,   549, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,
    -159,  -185,  -231,  -186,  -117, -1308, -1308, -1308, -1308, -1308,
     677, -1308,  -488, -1308, -1308,  -504, -1308, -1308,  -675,  -229,
    -539,  -533, -1308, -1308, -1308, -1056, -1308,  1147, -1308, -1308,
   -1308, -1308, -1308,   166,   353, -1308,   728, -1308, -1308, -1308,
   -1308, -1308, -1308,   359, -1308,   947, -1308, -1308, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,
    -703,  -171, -1308,   807, -1308, -1308, -1308, -1308, -1308, -1308,
    -629, -1308, -1308,  -352, -1308, -1308, -1308, -1308, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,
   -1308, -1308,   -65, -1308, -1308, -1308, -1308, -1308, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,   814,  -581,
    -168,  -694, -1308, -1308, -1206, -1130, -1308, -1308, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308,  -747, -1308, -1308, -1308, -1308,
   -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308, -1308,
    -420, -1308, -1171,  -587, -1308
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   705,   706,    18,   136,    54,    19,   166,   172,
     588,   468,   142,   469,    97,    21,    22,    46,    47,    88,
      23,    42,    43,   877,   878,  1491,   150,   151,  1492,  1059,
    1416,   879,   848,   849,  1459,  1460,  1523,  1461,   880,   881,
     882,   883,   884,   885,   886,   887,   888,   889,   890,   173,
     174,    38,    39,    40,   221,    66,    67,    68,    69,    24,
      25,   337,   404,   216,    26,   110,   217,   111,   155,  1206,
    1308,  1465,   405,   406,   891,   470,   892,   687,   668,  1247,
     839,   471,   893,   555,   710,  1238,   472,   894,   895,   896,
     897,   898,   529,   899,  1071,  1310,  1209,   900,   473,   723,
    1250,   724,  1251,   726,  1252,   474,   714,  1242,   475,   596,
    1358,   476,  1153,  1154,   768,   477,   610,   478,   901,   479,
     480,   753,   481,   960,  1345,   961,  1401,   482,   818,  1175,
     483,   597,  1157,  1411,  1159,  1412,  1289,  1451,   485,   486,
     400,  1383,  1426,  1315,  1317,  1232,   903,  1095,  1468,  1500,
     401,   402,   660,   661,   677,   664,   665,   679,   971,   684,
     547,   415,   539,   348,  1311,   540,   349,    78,   118,   219,
     344,    28,   162,   853,   845,   854,    50,    51,   133,    29,
     154,   186,   282,   846,   178,   179,    30,   107,   680,  1188,
     535,   340,   341,   115,   160,   691,    31,    76,   218,   536,
     685,   487,   389,   271,   272,   652,   675,   273,   497,  1009,
     707,   550,   376,   274,   275,   416,   911,   667,   495,  1008,
     417,   912,   418,   913,   494,  1007,   498,  1011,   499,  1178,
     500,  1013,   501,  1179,   502,  1015,   503,  1180,   504,  1018,
     505,  1021,   662,    32,    56,   279,   521,  1030,    33,    57,
     280,   522,  1032,    34,    55,   278,   520,  1027,   488,  1430,
    1396,   765,  1431,  1432,  1433,   921,   489,   708,  1236,   709,
    1237,   733,  1256,   730,  1254,   602,   490,   731,  1255,   491,
     925,  1324,   926,  1325,   927,  1326,   718,  1246,   728,  1253,
    1248,   492,  1483,   747,   493
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,   180,   181,   222,   698,   270,   696,    60,    70,   964,
     863,   748,   770,   766,   844,  1210,   757,   920,   678,    71,
      72,    73,   127,   764,   828,   519,   830,   654,   832,   656,
    -137,   658,   835,   676,  -136,   511,   838,     2,  1400,   560,
      84,   953,   981,   606,     3,  1133,   509,   603,   119,   120,
      90,   962,  1381,   112,   113,  1297,    70,    70,    70,   623,
     624,   543,   611,   612,   968,   598,   537,     4,   149,     5,
     759,     6,  1139,  -135,   537,    85,  1262,     7,  1043,   966,
     338,   538,   842,  1370,   544,   537,   527,     8,   858,   538,
     545,   931,   398,     9,  1446,    70,    70,    70,    70,   398,
     538,   544,    13,   572,    74,    35,    36,   545,    52,   669,
    1452,   276,  1382,   102,   103,   104,  1429,    10,  1207,   167,
     168,    61,    13,  1208,  1336,    13,   366,   644,   645,   528,
     764,   546,   929,   149,   932,   860,   561,   562,   377,    11,
      12,    16,   940,  1376,   220,   943,   721,    53,   546,   399,
      62,   220,    41,   367,   368,   114,  1494,   908,    96,   615,
     616,    16,   345,   149,    16,   719,   342,   621,  1257,   269,
     623,   624,   625,   626,   143,   729,   771,   764,   732,  1434,
     972,   512,   377,    75,  1026,   409,   700,  1091,  1442,   121,
    1384,  1385,   510,   844,   122,   833,   123,    77,   859,   124,
     513,  1391,    13,    86,   408,   763,   270,  1394,   410,   514,
    1024,   611,   612,   720,   563,    87,  1130,   369,   343,    63,
    1092,   370,  1499,    14,  1105,   859,    58,   734,  1093,   859,
    1477,  1478,   270,   125,   564,    15,   859,   858,   644,   645,
      79,    16,   270,   419,   420,   270,   270,   270,   822,    37,
      59,  1134,    81,  1453,  1517,  1378,  1454,   759,  1436,  1437,
     169,   365,  1198,   426,  1455,   170,   966,   171,   371,   428,
     124,  1456,   372,   788,   541,   373,  1485,    64,    80,   858,
      44,    58,   789,   859,   860,   970,   861,    65,   411,   862,
     749,   374,  1148,  1110,    82,   858,   979,    13,  1149,  1453,
     375,  1272,  1454,    45,   967,    59,   435,   436,   615,   616,
    1455,   621,   919,  1507,   623,   624,   621,  1456,   622,   623,
     624,   625,   626,   270,   270,   825,   860,   270,  1150,   270,
      80,   270,    89,   270,  1022,   226,    16,   764,   532,  1151,
     438,   439,   860,   907,  1152,    58,  1031,  1019,   826,  1155,
     270,   548,   549,   551,   611,   612,   915,   916,   183,   663,
    1029,    13,    95,   227,  1191,   973,   928,  1005,  1099,    59,
     269,   934,   935,  1026,   937,  1005,   939,    58,   941,   942,
     858,   944,   644,   645,   858,   858,   970,   644,   645,   105,
    1348,  1448,    96,   974,   270,  1006,   269,  1279,    58,   412,
      16,    59,   413,  1106,  1301,   414,   269,   749,   270,   269,
     269,   269,   750,    96,  1005,   106,   269,   558,   554,  1005,
    1005,   141,    59,   465,   607,  1280,   608,   860,  1005,   648,
     649,   860,   860,   653,   105,   655,   463,   657,  1005,   751,
     775,   779,  1287,   467,    98,    99,   100,  1294,  1296,   613,
     614,   615,   616,   975,   105,   793,  1342,   156,   157,   621,
     977,   622,   623,   624,   625,   626,  1343,   627,   628,  1341,
    1005,   101,  1273,   819,  1374,  1005,  1005,   975,   975,  1144,
    1312,   703,    13,   145,   146,   147,   148,   269,   269,  1138,
     704,   269,  1357,   269,   377,   269,   270,   269,  1347,  1305,
    1229,   128,  1375,  1399,  1462,  1501,  1526,   975,  1266,  1156,
     682,  1488,   507,    87,   269,   639,   640,   641,   642,   643,
     975,    16,  1489,  1490,   270,   683,  1282,   688,   611,   612,
     644,   645,   508,  1350,   565,   703,   270,   270,   270,   270,
     994,   130,   697,   270,   704,  -728,  1365,   270,  1423,   995,
    -728,   414,   270,   270,   566,   270,  -735,   270,   269,   270,
     270,  -735,   270,   573,  1270,   576,  -742,   703,    13,  -728,
    1410,  -742,   269,  1113,  1114,  1115,   704,   703,    13,  1119,
    -735,   712,   131,   574,   834,   577,   704,  1162,   999,  1293,
    -742,  -380,   752,  1349,   837,   132,  -380,  1000,    70,  1172,
    1379,   713,   840,   530,  1177,   759,   531,    16,  1137,  1146,
     703,    13,  1025,   703,   966,  -380,   137,    16,  1147,   704,
    1316,   138,   704,   703,    13,   615,   616,   930,    48,   992,
    1463,  1464,   704,   621,    49,   622,   623,   624,   625,   626,
    1023,   108,   914,   852,   703,   917,   116,   109,   694,   924,
      16,   695,   117,   704,   414,  1447,    85,   703,    13,   139,
     269,  1329,   270,    16,   270,   270,   704,   134,   377,   152,
     689,   690,   701,   135,  1109,   153,   847,   270,   847,   158,
     377,  1313,   223,   224,   823,   159,   876,  1314,   269,   140,
     270,   641,   642,   643,   102,   377,   104,    16,   910,   824,
     269,   269,   269,   269,   644,   645,  1513,   269,   377,   377,
     377,   269,   827,   829,   831,   161,   269,   269,   377,   269,
    1521,   269,  1104,   269,   269,   381,   269,  1057,  1058,   382,
     176,  1449,  1260,   177,  1261,   377,  1529,  1051,   377,  1112,
    1267,  1532,  1361,   383,   384,   102,   103,   104,   385,   386,
     387,   388,   149,   182,   270,   270,   270,  1395,   377,   102,
     270,   185,  1413,   163,   164,   741,   742,   184,  1017,   876,
     220,  1020,   277,   163,   164,   165,   281,  1421,   223,   224,
     225,   334,   669,    92,    93,    94,   335,   339,   336,   270,
     347,   354,   357,   346,   351,   352,  1039,   353,   355,   378,
     356,   358,   377,   359,   364,  1283,  1441,   391,   360,  1395,
     361,   362,   363,   395,   379,   392,   393,   396,   380,   403,
     407,   496,   524,   525,   534,   394,   269,   517,   269,   269,
     542,   552,   397,   693,   605,   526,   553,   567,   670,   523,
     559,   269,  1424,   533,  1473,   568,   569,  1028,  1471,  1028,
     571,   575,   578,   270,   269,   570,   579,   651,   663,   674,
     715,   572,   582,  1344,   583,   702,   599,   716,   584,   585,
    1047,   717,   745,   586,   587,   604,  1054,  1055,   647,   746,
     650,   756,   692,   711,   767,  1467,   744,   786,   754,  1063,
     836,   857,  1065,  1066,  1067,  1068,  1069,  1511,   841,   852,
    1072,   769,   856,   484,   905,   906,  1098,   922,  1101,   954,
     945,   956,   957,   506,   976,   958,   959,   982,   269,   269,
     269,  1518,   969,   516,   269,   965,   975,  1497,   669,   984,
     978,   983,   985,   986,   987,  1393,  1231,  1002,   997,   998,
    1003,  1010,  1012,  1014,  1016,  1033,  1037,  1038,  1046,  1048,
    1056,  1060,  1061,   269,  1062,  1070,  1064,  1097,  1102,  1182,
    1103,  1107,  1116,  1121,  1183,  1108,  1117,  1118,  1122,  1120,
     228,  1418,  1131,  1123,  1125,  1124,   229,  1126,  1127,  1129,
    1164,  1128,   230,  1193,  1190,  1195,   270,  1140,   270,  1165,
     580,   581,   231,  1258,  1184,  1167,  1196,  1197,  1205,  1173,
     232,  1181,  1200,  1201,  1202,  1203,  1230,   589,   590,   591,
     592,   593,  1185,  1204,  1233,   233,   601,   269,  1281,  1239,
    1241,  1240,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,  1243,  1244,  1245,  1249,  1265,
    1274,  1269,  1271,  1275,  1276,  1277,   673,  1278,  1299,  1298,
    1309,  1466,  1316,  1320,  1323,   681,  1335,  1346,  1354,  1321,
    1356,  1327,  1360,  1328,  1330,    70,  1334,  1337,  1338,  1398,
      58,  1367,  1405,  1415,  1285,  1387,   699,  1373,  1388,  1235,
    1407,  1425,  1422,   267,  1414,  1472,  1389,   611,   612,  1390,
    1505,  1439,  1402,  1502,    59,   601,   722,  1440,   270,   725,
    1443,   727,  1445,  1474,  1475,   -82,  1479,  1480,  1481,  1493,
    1503,   735,   736,   737,   738,   739,   740,  1482,  1504,   743,
    1496,  1508,   126,  1506,  1520,  1359,  1510,    20,    83,   175,
     269,   129,   269,   268,  1528,  1533,   850,   851,   518,   144,
    1531,  1392,   780,   781,    27,  1094,   782,   783,   784,   785,
    1355,   787,   609,   790,   791,   792,   794,   795,   796,   797,
     798,   799,   801,   802,   803,   804,   805,   806,   807,   808,
     809,   810,   811,   350,   820,  1427,  1469,  1428,  1380,  1470,
    1187,    91,   613,   614,   615,   616,   617,  1034,   686,   618,
     619,   620,   621,  1035,   622,   623,   624,   625,   626,     0,
     627,   628,  1438,     0,  1362,     0,   629,   630,   631,   390,
     556,  1318,  1417,  1319,  1419,  1420,   902,   557,     0,     0,
       0,   904,     0,     0,     0,     0,     0,   909,     0,     0,
       0,     0,   270,     0,     0,     0,     0,     0,     0,     0,
       0,   633,   923,   634,   635,   636,   637,   638,   639,   640,
     641,   642,   643,     0,   611,   612,     0,     0,     0,     0,
       0,     0,   269,   644,   645,     0,     0,     0,     0,     0,
       0,   952,     0,     0,     0,   955,     0,     0,     0,     0,
       0,     0,     0,     0,   963,   601,     0,     0,     0,     0,
    1377,     0,     0,     0,     0,     0,     0,     0,     0,   902,
       0,     0,     0,     0,     0,  1495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   752,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1509,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   996,     0,   611,   612,  1001,     0,     0,   613,
     614,   615,   616,   617,     0,     0,   618,   619,   620,   621,
    1525,   622,   623,   624,   625,   626,     0,   627,   628,     0,
    1530,     0,     0,   629,   752,   631,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1036,     0,     0,     0,     0,
       0,     0,  1040,     0,  1042,  1458,   269,  1045,     0,     0,
     876,     0,  1050,     0,  1053,     0,     0,     0,     0,     0,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   643,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     644,   645,     0,     0,     0,     0,     0,  1100,     0,   613,
     614,   615,   616,   617,     0,     0,   618,   619,   620,   621,
       0,   622,   623,   624,   625,   626,     0,   627,   628,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1512,     0,     0,     0,  1515,  1516,     0,
       0,     0,     0,     0,     0,     0,     0,  1135,  1136,     0,
     876,     0,     0,  1524,     0,     0,     0,     0,  1141,  1142,
    1143,     0,   955,   637,   638,   639,   640,   641,   642,   643,
       0,     0,  1158,     0,  1160,     0,  1163,     0,     0,     0,
     644,   645,  1166,     0,     0,     0,  1169,     0,   611,   612,
       0,     0,     0,     0,   955,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   611,   612,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1186,     0,     0,     0,     0,     0,     0,     0,
    1192,     0,     0,  1194,     0,     0,     0,     0,     0,     0,
       0,  1199,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1211,  1212,  1213,  1214,  1215,  1216,  1217,  1218,
    1219,  1220,  1221,  1222,  1223,  1224,  1225,  1226,  1227,  1228,
       0,     0,     0,   613,   614,   615,   616,   617,  1234,     0,
     618,   619,   620,   621,     0,   622,   623,   624,   625,   626,
     -73,   627,   628,     0,     0,     0,     0,   601,   615,   616,
       0,   611,   612,     0,     0,     0,   621,     0,   622,   623,
     624,   625,   626,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   634,   635,   636,   637,   638,   639,
     640,   641,   642,   643,  1290,     0,  1291,     0,     0,     0,
       0,     0,  1295,     0,   644,   645,     0,     0,     0,     0,
       0,     0,   639,   640,   641,   642,   643,     0,     0,     0,
    1300,     0,     0,     0,  1302,     0,     0,   644,   645,  1303,
    1304,     0,  1306,  1307,     0,   611,   612,     0,  1073,  1074,
    1075,  1076,  1077,  1078,  1079,  1080,   613,   614,   615,   616,
     617,  1081,  1082,   618,   619,   620,   621,  1083,   622,   623,
     624,   625,   626,  1084,   627,   628,  1085,  1086,     0,  1322,
     629,   630,   631,  1087,  1088,  1089,   632,     0,     0,  1331,
    1332,  1333,     0,     0,     0,     0,  1340,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1351,  1352,  1353,     0,  1090,   633,     0,   634,   635,   636,
     637,   638,   639,   640,   641,   642,   643,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   644,   645,     0,
     613,   614,   615,   616,   617,     0,     0,   618,   619,   620,
     621,     0,   622,   623,   624,   625,   626,     0,   627,   628,
       0,     0,     0,     0,   629,   630,   631,     0,     0,   601,
     632,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1403,  1404,    13,
       0,     0,     0,     0,     0,  1409,     0,     0,     0,   633,
       0,   634,   635,   636,   637,   638,   639,   640,   641,   642,
     643,     0,     0,     0,     0,   187,     0,     0,     0,     0,
       0,   644,   645,     0,     0,     0,  1435,     0,    16,     0,
       0,     0,     0,     0,     0,     0,     0,  1444,     0,     0,
       0,   601,     0,     0,     0,     0,     0,  1450,     0,     0,
       0,   188,     0,   189,  1457,   190,   191,   192,   193,   194,
     902,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,     0,   206,   207,   208,     0,     0,   209,   210,
     211,   212,     0,     0,  1484,     0,     0,  1486,  1487,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   213,   214,
    1457,     0,     0,     0,   864,     0,  1498,     0,   419,   420,
       3,     0,   -93,   -87,   -87,     0,   -90,     0,   421,   422,
     423,   424,   425,     0,     0,     0,     0,     0,   426,   865,
     427,   866,   867,     0,   428,     0,  1514,     0,     0,     0,
       0,   868,   429,   215,  1519,   -95,     0,   869,   430,  1522,
     902,   431,     0,     8,   432,   870,     0,   871,   433,     0,
       0,   872,   873,     0,     0,     0,     0,     0,   874,     0,
       0,   435,   436,     0,   234,   235,   236,     0,   238,   239,
     240,   241,   242,   437,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,   256,   257,   258,     0,
       0,   261,   262,   263,   264,   438,   439,   440,   875,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   441,   442,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     443,   444,   445,   446,   447,     0,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,    59,     0,    13,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   458,   459,   460,     0,    14,
       0,     0,   461,   462,     0,     0,     0,     0,     0,     0,
       0,   463,     0,   464,     0,   465,   466,    16,   467,  -241,
     864,     0,     0,     0,   419,   420,     3,     0,   -93,   -87,
     -87,     0,   -90,     0,   421,   422,   423,   424,   425,     0,
       0,     0,     0,     0,   426,   865,   427,   866,   867,     0,
     428,     0,     0,     0,     0,     0,     0,   868,   429,     0,
       0,   -95,     0,   869,   430,     0,     0,   431,     0,     8,
     432,   870,     0,   871,   433,     0,     0,   872,   873,     0,
       0,     0,     0,     0,   874,     0,     0,   435,   436,     0,
     234,   235,   236,     0,   238,   239,   240,   241,   242,   437,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,   256,   257,   258,     0,     0,   261,   262,   263,
     264,   438,   439,   440,   875,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,   442,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   443,   444,   445,   446,
     447,     0,   448,     0,   449,   450,   451,   452,   453,   454,
     455,   456,    59,     0,    13,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   458,   459,   460,     0,    14,     0,     0,   461,   462,
       0,     0,     0,     0,     0,     0,     0,   463,     0,   464,
       0,   465,   466,    16,   467,   980,   864,     0,     0,     0,
     419,   420,     3,     0,   -93,   -87,   -87,     0,   -90,     0,
     421,   422,   423,   424,   425,     0,     0,     0,     0,     0,
     426,   865,   427,   866,   867,     0,   428,     0,     0,     0,
       0,     0,     0,   868,   429,     0,     0,   -95,     0,   869,
     430,     0,     0,   431,     0,     8,   432,   870,     0,   871,
     433,     0,     0,   872,   873,     0,     0,     0,     0,     0,
     874,     0,     0,   435,   436,     0,   234,   235,   236,     0,
     238,   239,   240,   241,   242,   437,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,   256,   257,
     258,     0,     0,   261,   262,   263,   264,   438,   439,   440,
     875,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   443,   444,   445,   446,   447,     0,   448,     0,
     449,   450,   451,   452,   453,   454,   455,   456,    59,     0,
      13,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   458,   459,   460,
       0,    14,     0,     0,   461,   462,     0,     0,     0,     0,
       0,     0,     0,   463,     0,   464,     0,   465,   466,    16,
     467,  -238,   864,     0,     0,     0,   419,   420,     3,     0,
     -93,   -87,   -87,     0,   -90,     0,   421,   422,   423,   424,
     425,     0,     0,     0,     0,     0,   426,   865,   427,   866,
     867,     0,   428,     0,     0,     0,     0,     0,     0,   868,
     429,     0,     0,   -95,     0,   869,   430,     0,     0,   431,
       0,     8,   432,   870,     0,   871,   433,     0,     0,   872,
     873,     0,     0,     0,     0,     0,   874,     0,     0,   435,
     436,     0,   234,   235,   236,     0,   238,   239,   240,   241,
     242,   437,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,   256,   257,   258,     0,     0,   261,
     262,   263,   264,   438,   439,   440,   875,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   443,   444,
     445,   446,   447,     0,   448,     0,   449,   450,   451,   452,
     453,   454,   455,   456,    59,     0,    13,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   458,   459,   460,     0,    14,     0,     0,
     461,   462,     0,     0,     0,     0,     0,     0,     0,   463,
       0,   464,     0,   465,   466,    16,   467,   -83,   419,   420,
       0,     0,     0,     0,     0,     0,     0,     0,   421,   422,
     423,   424,   425,     0,     0,     0,     0,     0,   426,   865,
     427,   866,     0,     0,   428,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,   430,     0,
       0,   431,     0,     0,   432,   870,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
       0,   435,   436,     0,   234,   235,   236,     0,   238,   239,
     240,   241,   242,   437,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,   256,   257,   258,     0,
       0,   261,   262,   263,   264,   438,   439,   440,   875,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   441,   442,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     443,   444,   445,   446,   447,     0,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,    59,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   458,   459,   460,     0,    14,
       0,     0,   461,   462,     0,     0,     0,     0,     0,   419,
     420,   463,     0,   464,     0,   465,   466,     0,   467,   421,
     422,   423,   424,   425,     0,     0,     0,     0,     0,   426,
       0,   427,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,   430,
       0,     0,   431,     0,     0,   432,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
       0,     0,   435,   436,   758,   234,   235,   236,     0,   238,
     239,   240,   241,   242,   437,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   256,   257,   258,
       0,     0,   261,   262,   263,   264,   438,   439,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   441,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   443,   444,   445,   446,   447,     0,   448,   759,   449,
     450,   451,   452,   453,   454,   455,   456,   760,     0,     0,
     457,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   458,   459,   460,     0,
      14,     0,     0,   461,   462,     0,     0,     0,     0,     0,
     419,   420,   761,     0,   464,   762,   465,   466,   594,   467,
     421,   422,   423,   424,   425,     0,     0,     0,     0,     0,
     426,     0,   427,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
     430,     0,     0,   431,   595,     0,   432,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,     0,     0,   435,   436,     0,   234,   235,   236,     0,
     238,   239,   240,   241,   242,   437,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,   256,   257,
     258,     0,     0,   261,   262,   263,   264,   438,   439,   440,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   443,   444,   445,   446,   447,     0,   448,   759,
     449,   450,   451,   452,   453,   454,   455,   456,   760,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   458,   459,   460,
       0,    14,     0,     0,   461,   462,     0,     0,     0,     0,
       0,   419,   420,   463,     0,   464,     0,   465,   466,   594,
     467,   421,   422,   423,   424,   425,     0,     0,     0,     0,
       0,   426,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,   430,     0,     0,   431,   595,     0,   432,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,     0,     0,   435,   436,     0,   234,   235,   236,
       0,   238,   239,   240,   241,   242,   437,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,   256,
     257,   258,     0,     0,   261,   262,   263,   264,   438,   439,
     440,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   441,   442,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   443,   444,   445,   446,   447,     0,   448,
       0,   449,   450,   451,   452,   453,   454,   455,   456,    59,
       0,     0,   457,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   458,   459,
     460,     0,    14,     0,     0,   461,   462,     0,     0,     0,
       0,     0,   419,   420,   463,     0,   464,     0,   465,   466,
       0,   467,   421,   422,   423,   424,   425,     0,     0,     0,
       0,     0,   426,     0,   427,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,   430,     0,     0,   431,     0,     0,   432,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,     0,     0,   435,   436,   918,   234,   235,
     236,     0,   238,   239,   240,   241,   242,   437,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
     256,   257,   258,     0,     0,   261,   262,   263,   264,   438,
     439,   440,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   441,   442,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   443,   444,   445,   446,   447,     0,
     448,   759,   449,   450,   451,   452,   453,   454,   455,   456,
     760,     0,     0,   457,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   443,   444,   445,   446,   447,
       0,   448,     0,   449,   450,   451,   452,   453,   454,   455,
     456,    59,     0,     0,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     458,   459,   460,     0,    14,     0,     0,   461,   462,     0,
       0,     0,     0,     0,   419,   420,   463,   515,   464,     0,
     465,   466,   600,   467,   421,   422,   423,   424,   425,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   443,   444,   445,   446,
     447,     0,   448,     0,   449,   450,   451,   452,   453,   454,
     455,   456,    59,     0,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   458,   459,   460,     0,    14,     0,     0,   461,   462,
       0,     0,     0,     0,     0,   419,   420,   463,     0,   464,
       0,   465,   466,     0,   467,   421,   422,   423,   424,   425,
       0,     0,     0,     0,     0,   426,     0,   427,     0,     0,
       0,   428,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,   430,     0,     0,   431,     0,
       0,   432,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,   435,   436,
       0,   234,   235,   236,     0,   238,   239,   240,   241,   242,
     437,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,   256,   257,   258,     0,     0,   261,   262,
     263,   264,   438,   439,   440,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   441,   442,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   443,   444,   445,
     446,   447,     0,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,    59,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   458,   459,   460,     0,    14,     0,     0,   461,
     462,     0,     0,     0,     0,     0,   419,   420,   463,   672,
     464,     0,   465,   466,     0,   467,   421,   422,   423,   424,
     425,     0,     0,   800,     0,     0,   426,     0,   427,     0,
       0,     0,   428,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,   430,     0,     0,   431,
       0,     0,   432,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,     0,     0,   435,
     436,     0,   234,   235,   236,     0,   238,   239,   240,   241,
     242,   437,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,   256,   257,   258,     0,     0,   261,
     262,   263,   264,   438,   439,   440,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   443,   444,
     445,   446,   447,     0,   448,     0,   449,   450,   451,   452,
     453,   454,   455,   456,    59,     0,     0,   457,     0,     0,
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
       0,    58,     0,     0,     0,     0,     0,     0,     0,   443,
     444,   445,   446,   447,     0,   448,     0,   449,   450,   451,
     452,   453,   454,   455,   456,    59,     0,     0,   457,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   458,   459,   460,     0,    14,     0,
       0,   461,   462,     0,     0,     0,     0,     0,   419,   420,
     463,     0,   464,   821,   465,   466,     0,   467,   421,   422,
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
    1041,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     443,   444,   445,   446,   447,     0,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,    59,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   458,   459,   460,     0,    14,
       0,     0,   461,   462,     0,     0,     0,     0,     0,   419,
     420,   463,     0,   464,     0,   465,   466,     0,   467,   421,
     422,   423,   424,   425,     0,     0,     0,     0,     0,   426,
       0,   427,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,   430,
       0,     0,   431,     0,     0,   432,     0,     0,     0,   433,
       0,     0,     0,     0,     0,  1044,     0,     0,     0,   434,
       0,     0,   435,   436,     0,   234,   235,   236,     0,   238,
     239,   240,   241,   242,   437,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   256,   257,   258,
       0,     0,   261,   262,   263,   264,   438,   439,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   441,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   443,   444,   445,   446,   447,     0,   448,     0,   449,
     450,   451,   452,   453,   454,   455,   456,    59,     0,     0,
     457,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   458,   459,   460,     0,
      14,     0,     0,   461,   462,     0,     0,     0,     0,     0,
     419,   420,   463,     0,   464,     0,   465,   466,     0,   467,
     421,   422,   423,   424,   425,     0,     0,     0,     0,     0,
     426,     0,   427,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
     430,     0,     0,   431,     0,     0,   432,     0,     0,     0,
     433,     0,     0,  1049,     0,     0,     0,     0,     0,     0,
     434,     0,     0,   435,   436,     0,   234,   235,   236,     0,
     238,   239,   240,   241,   242,   437,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,   256,   257,
     258,     0,     0,   261,   262,   263,   264,   438,   439,   440,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   443,   444,   445,   446,   447,     0,   448,     0,
     449,   450,   451,   452,   453,   454,   455,   456,    59,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   458,   459,   460,
       0,    14,     0,     0,   461,   462,     0,     0,     0,     0,
       0,   419,   420,   463,     0,   464,     0,   465,   466,     0,
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
       0,     0,     0,  1052,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   443,   444,   445,   446,   447,     0,   448,
       0,   449,   450,   451,   452,   453,   454,   455,   456,    59,
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
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   443,   444,   445,   446,   447,     0,
     448,     0,   449,   450,   451,   452,   453,   454,   455,   456,
      59,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   458,
     459,   460,     0,    14,     0,     0,   461,   462,     0,     0,
       0,     0,     0,   419,   420,   463,     0,   464,  1161,   465,
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
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   443,   444,   445,   446,   447,
       0,   448,     0,   449,   450,   451,   452,   453,   454,   455,
     456,    59,     0,     0,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     458,   459,   460,     0,    14,     0,     0,   461,   462,     0,
       0,     0,     0,     0,   419,   420,  1170,     0,   464,  1171,
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
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   443,   444,   445,   446,
     447,     0,   448,     0,   449,   450,   451,   452,   453,   454,
     455,   456,    59,     0,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   458,   459,   460,     0,    14,     0,     0,   461,   462,
       0,     0,     0,     0,     0,   419,   420,   463,     0,   464,
    1176,   465,   466,     0,   467,   421,   422,   423,   424,   425,
       0,     0,     0,     0,     0,   426,     0,   427,     0,     0,
       0,   428,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,   430,     0,     0,   431,     0,
       0,   432,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,   435,   436,
       0,   234,   235,   236,     0,   238,   239,   240,   241,   242,
     437,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,   256,   257,   258,     0,     0,   261,   262,
     263,   264,   438,   439,   440,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   441,   442,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   443,   444,   445,
     446,   447,     0,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,    59,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   458,   459,   460,     0,    14,     0,     0,   461,
     462,     0,     0,     0,     0,     0,   419,   420,   463,     0,
     464,  1259,   465,   466,     0,   467,   421,   422,   423,   424,
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
     442,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   443,   444,
     445,   446,   447,     0,   448,     0,   449,   450,   451,   452,
     453,   454,   455,   456,    59,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   458,   459,   460,     0,    14,     0,     0,
     461,   462,     0,     0,     0,     0,     0,   419,   420,   463,
       0,   464,  1339,   465,   466,     0,   467,   421,   422,   423,
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
     441,   442,     0,     0,     0,     0,     0,     0,     0,  1408,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   443,
     444,   445,   446,   447,     0,   448,     0,   449,   450,   451,
     452,   453,   454,   455,   456,    59,     0,     0,   457,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   458,   459,   460,     0,    14,     0,
       0,   461,   462,     0,     0,     0,     0,     0,   419,   420,
     463,     0,   464,     0,   465,   466,     0,   467,   421,   422,
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
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     443,   444,   445,   446,   447,     0,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,    59,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   458,   459,   460,     0,    14,
       0,     0,   461,   462,     0,     0,     0,     0,     0,   419,
     420,   463,     0,   464,     0,   465,   466,     0,   467,   421,
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
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   443,   444,   445,   446,   447,     0,   448,     0,   449,
     450,   451,   452,   453,   454,   455,   456,    59,     0,   228,
     457,     0,     0,     0,     0,   229,     0,     0,     0,     0,
       0,   230,     0,     0,     0,     0,   458,   459,   460,     0,
      14,   231,     0,   461,   462,     0,     0,     0,     0,   232,
       0,     0,  1145,     0,   464,     0,   465,   466,     0,   467,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   228,     0,     0,     0,     0,     0,
     229,     0,     0,     0,     0,     0,   230,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   231,     0,     0,    58,
       0,     0,     0,     0,   232,     0,     0,     0,     0,     0,
       0,     0,   267,     0,     0,     0,     0,     0,     0,   233,
       0,     0,     0,   659,     0,    13,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,     0,
       0,     0,   268,     0,    16,   611,   612,   228,     0,     0,
       0,     0,     0,   229,     0,     0,     0,     0,     0,   230,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   231,
       0,     0,     0,     0,    58,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   267,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,    59,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,     0,     0,     0,     0,     0,   268,   611,   612,
     613,   614,   615,   616,   617,     0,     0,   618,   619,   620,
     621,     0,   622,   623,   624,   625,   626,     0,   627,   628,
       0,     0,     0,     0,   629,   630,   631,    58,     0,     0,
     632,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     267,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   659,     0,     0,     0,     0,     0,     0,     0,   633,
       0,   634,   635,   636,   637,   638,   639,   640,   641,   642,
     643,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   644,   645,     0,   611,   612,     0,     0,  1482,     0,
     268,     0,     0,   613,   614,   615,   616,   617,     0,     0,
     618,   619,   620,   621,     0,   622,   623,   624,   625,   626,
       0,   627,   628,     0,     0,     0,     0,   629,   630,   631,
       0,     0,     0,   632,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   633,     0,   634,   635,   636,   637,   638,   639,
     640,   641,   642,   643,     0,     0,     0,     0,     0,     0,
     611,   612,     0,     0,   644,   645,     0,     0,   646,   613,
     614,   615,   616,   617,     0,     0,   618,   619,   620,   621,
       0,   622,   623,   624,   625,   626,     0,   627,   628,     0,
       0,     0,     0,   629,   630,   631,     0,     0,     0,   632,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   633,     0,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   643,
       0,     0,     0,     0,     0,     0,   611,   612,     0,     0,
     644,   645,     0,     0,   666,   613,   614,   615,   616,   617,
       0,     0,   618,   619,   620,   621,     0,   622,   623,   624,
     625,   626,     0,   627,   628,     0,     0,     0,     0,   629,
     630,   631,     0,     0,     0,   632,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   633,     0,   634,   635,   636,   637,
     638,   639,   640,   641,   642,   643,     0,     0,     0,     0,
       0,     0,   611,   612,     0,     0,   644,   645,     0,     0,
     855,   613,   614,   615,   616,   617,     0,     0,   618,   619,
     620,   621,     0,   622,   623,   624,   625,   626,     0,   627,
     628,     0,     0,     0,     0,   629,   630,   631,     0,     0,
       0,   632,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     633,     0,   634,   635,   636,   637,   638,   639,   640,   641,
     642,   643,     0,     0,     0,     0,     0,     0,   611,   612,
       0,     0,   644,   645,     0,     0,   933,   613,   614,   615,
     616,   617,     0,     0,   618,   619,   620,   621,     0,   622,
     623,   624,   625,   626,     0,   627,   628,     0,     0,     0,
       0,   629,   630,   631,     0,     0,     0,   632,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   633,     0,   634,   635,
     636,   637,   638,   639,   640,   641,   642,   643,     0,     0,
       0,     0,     0,     0,   611,   612,     0,     0,   644,   645,
       0,     0,   936,   613,   614,   615,   616,   617,     0,     0,
     618,   619,   620,   621,     0,   622,   623,   624,   625,   626,
       0,   627,   628,     0,     0,     0,     0,   629,   630,   631,
       0,     0,     0,   632,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   633,     0,   634,   635,   636,   637,   638,   639,
     640,   641,   642,   643,     0,     0,     0,     0,     0,     0,
     611,   612,     0,     0,   644,   645,     0,     0,   938,   613,
     614,   615,   616,   617,     0,     0,   618,   619,   620,   621,
       0,   622,   623,   624,   625,   626,     0,   627,   628,     0,
       0,     0,     0,   629,   630,   631,     0,     0,     0,   632,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   633,     0,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   643,
       0,     0,     0,     0,     0,     0,   611,   612,     0,     0,
     644,   645,     0,     0,   946,   613,   614,   615,   616,   617,
       0,     0,   618,   619,   620,   621,     0,   622,   623,   624,
     625,   626,     0,   627,   628,     0,     0,     0,     0,   629,
     630,   631,     0,     0,     0,   632,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   633,     0,   634,   635,   636,   637,
     638,   639,   640,   641,   642,   643,     0,     0,     0,     0,
       0,     0,   611,   612,     0,     0,   644,   645,     0,     0,
     947,   613,   614,   615,   616,   617,     0,     0,   618,   619,
     620,   621,     0,   622,   623,   624,   625,   626,     0,   627,
     628,     0,     0,     0,     0,   629,   630,   631,     0,     0,
       0,   632,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     633,     0,   634,   635,   636,   637,   638,   639,   640,   641,
     642,   643,     0,     0,     0,     0,     0,     0,   611,   612,
       0,     0,   644,   645,     0,     0,   948,   613,   614,   615,
     616,   617,     0,     0,   618,   619,   620,   621,     0,   622,
     623,   624,   625,   626,     0,   627,   628,     0,     0,     0,
       0,   629,   630,   631,     0,     0,     0,   632,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   633,     0,   634,   635,
     636,   637,   638,   639,   640,   641,   642,   643,     0,     0,
       0,     0,     0,     0,   611,   612,     0,     0,   644,   645,
       0,     0,   949,   613,   614,   615,   616,   617,     0,     0,
     618,   619,   620,   621,     0,   622,   623,   624,   625,   626,
       0,   627,   628,     0,     0,     0,     0,   629,   630,   631,
       0,     0,     0,   632,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   633,     0,   634,   635,   636,   637,   638,   639,
     640,   641,   642,   643,     0,     0,     0,     0,     0,     0,
     611,   612,     0,     0,   644,   645,     0,     0,   950,   613,
     614,   615,   616,   617,     0,     0,   618,   619,   620,   621,
       0,   622,   623,   624,   625,   626,     0,   627,   628,     0,
       0,     0,     0,   629,   630,   631,     0,     0,     0,   632,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   633,     0,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   643,
       0,     0,     0,     0,     0,     0,   611,   612,     0,     0,
     644,   645,     0,     0,   951,   613,   614,   615,   616,   617,
       0,     0,   618,   619,   620,   621,     0,   622,   623,   624,
     625,   626,     0,   627,   628,     0,     0,     0,     0,   629,
     630,   631,     0,     0,     0,   632,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   633,     0,   634,   635,   636,   637,
     638,   639,   640,   641,   642,   643,     0,     0,     0,     0,
       0,     0,   611,   612,     0,     0,   644,   645,     0,     0,
    1096,   613,   614,   615,   616,   617,     0,     0,   618,   619,
     620,   621,     0,   622,   623,   624,   625,   626,     0,   627,
     628,     0,     0,     0,     0,   629,   630,   631,     0,     0,
       0,   632,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     633,     0,   634,   635,   636,   637,   638,   639,   640,   641,
     642,   643,     0,     0,     0,     0,     0,     0,   611,   612,
       0,     0,   644,   645,     0,     0,  1111,   613,   614,   615,
     616,   617,     0,     0,   618,   619,   620,   621,     0,   622,
     623,   624,   625,   626,     0,   627,   628,     0,     0,     0,
       0,   629,   630,   631,     0,     0,     0,   632,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   633,     0,   634,   635,
     636,   637,   638,   639,   640,   641,   642,   643,     0,     0,
       0,     0,     0,     0,   611,   612,     0,     0,   644,   645,
       0,     0,  1189,   613,   614,   615,   616,   617,     0,     0,
     618,   619,   620,   621,     0,   622,   623,   624,   625,   626,
       0,   627,   628,     0,     0,     0,     0,   629,   630,   631,
       0,     0,     0,   632,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   633,     0,   634,   635,   636,   637,   638,   639,
     640,   641,   642,   643,     0,     0,     0,     0,     0,     0,
     611,   612,     0,     0,   644,   645,     0,     0,  1263,   613,
     614,   615,   616,   617,     0,     0,   618,   619,   620,   621,
       0,   622,   623,   624,   625,   626,     0,   627,   628,     0,
       0,     0,     0,   629,   630,   631,     0,     0,     0,   632,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   633,     0,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   643,
       0,     0,     0,     0,     0,     0,   611,   612,     0,     0,
     644,   645,     0,     0,  1264,   613,   614,   615,   616,   617,
       0,     0,   618,   619,   620,   621,     0,   622,   623,   624,
     625,   626,     0,   627,   628,     0,     0,     0,     0,   629,
     630,   631,     0,     0,     0,   632,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   633,     0,   634,   635,   636,   637,
     638,   639,   640,   641,   642,   643,     0,     0,     0,     0,
       0,     0,   611,   612,     0,     0,   644,   645,     0,     0,
    1268,   613,   614,   615,   616,   617,     0,     0,   618,   619,
     620,   621,     0,   622,   623,   624,   625,   626,     0,   627,
     628,     0,     0,     0,     0,   629,   630,   631,     0,     0,
       0,   632,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     633,     0,   634,   635,   636,   637,   638,   639,   640,   641,
     642,   643,     0,     0,     0,     0,     0,     0,   611,   612,
       0,     0,   644,   645,     0,     0,  1284,   613,   614,   615,
     616,   617,     0,     0,   618,   619,   620,   621,     0,   622,
     623,   624,   625,   626,     0,   627,   628,     0,     0,     0,
       0,   629,   630,   631,     0,     0,     0,   632,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   633,     0,   634,   635,
     636,   637,   638,   639,   640,   641,   642,   643,     0,     0,
       0,     0,     0,     0,   611,   612,     0,     0,   644,   645,
       0,     0,  1286,   613,   614,   615,   616,   617,     0,     0,
     618,   619,   620,   621,     0,   622,   623,   624,   625,   626,
       0,   627,   628,     0,     0,     0,     0,   629,   630,   631,
       0,     0,     0,   632,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   633,     0,   634,   635,   636,   637,   638,   639,
     640,   641,   642,   643,     0,     0,     0,     0,     0,     0,
     611,   612,     0,     0,   644,   645,     0,     0,  1288,   613,
     614,   615,   616,   617,     0,     0,   618,   619,   620,   621,
       0,   622,   623,   624,   625,   626,     0,   627,   628,     0,
       0,     0,     0,   629,   630,   631,     0,     0,     0,   632,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   633,     0,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   643,
       0,     0,     0,     0,     0,     0,   611,   612,     0,     0,
     644,   645,     0,     0,  1292,   613,   614,   615,   616,   617,
       0,     0,   618,   619,   620,   621,     0,   622,   623,   624,
     625,   626,     0,   627,   628,     0,     0,     0,     0,   629,
     630,   631,     0,     0,     0,   632,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   633,     0,   634,   635,   636,   637,
     638,   639,   640,   641,   642,   643,     0,     0,     0,     0,
       0,     0,   611,   612,     0,     0,   644,   645,     0,     0,
    1363,   613,   614,   615,   616,   617,     0,     0,   618,   619,
     620,   621,     0,   622,   623,   624,   625,   626,     0,   627,
     628,     0,     0,     0,     0,   629,   630,   631,     0,     0,
       0,   632,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     633,     0,   634,   635,   636,   637,   638,   639,   640,   641,
     642,   643,     0,     0,     0,     0,     0,     0,   611,   612,
       0,     0,   644,   645,     0,     0,  1364,   613,   614,   615,
     616,   617,     0,     0,   618,   619,   620,   621,     0,   622,
     623,   624,   625,   626,     0,   627,   628,     0,     0,     0,
       0,   629,   630,   631,     0,     0,     0,   632,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   633,     0,   634,   635,
     636,   637,   638,   639,   640,   641,   642,   643,     0,     0,
       0,     0,     0,     0,   611,   612,     0,     0,   644,   645,
       0,     0,  1366,   613,   614,   615,   616,   617,     0,     0,
     618,   619,   620,   621,     0,   622,   623,   624,   625,   626,
       0,   627,   628,     0,     0,     0,     0,   629,   630,   631,
       0,     0,     0,   632,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   633,     0,   634,   635,   636,   637,   638,   639,
     640,   641,   642,   643,     0,     0,     0,     0,     0,     0,
     611,   612,     0,     0,   644,   645,     0,     0,  1368,   613,
     614,   615,   616,   617,     0,     0,   618,   619,   620,   621,
       0,   622,   623,   624,   625,   626,     0,   627,   628,     0,
       0,     0,     0,   629,   630,   631,     0,     0,     0,   632,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   633,     0,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   643,
       0,     0,     0,     0,     0,     0,   611,   612,     0,     0,
     644,   645,     0,     0,  1369,   613,   614,   615,   616,   617,
       0,     0,   618,   619,   620,   621,     0,   622,   623,   624,
     625,   626,     0,   627,   628,     0,     0,     0,     0,   629,
     630,   631,     0,     0,     0,   632,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   633,     0,   634,   635,   636,   637,
     638,   639,   640,   641,   642,   643,     0,     0,     0,     0,
       0,     0,   611,   612,     0,     0,   644,   645,     0,     0,
    1371,   613,   614,   615,   616,   617,     0,     0,   618,   619,
     620,   621,     0,   622,   623,   624,   625,   626,     0,   627,
     628,     0,     0,     0,     0,   629,   630,   631,     0,     0,
       0,   632,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     633,     0,   634,   635,   636,   637,   638,   639,   640,   641,
     642,   643,     0,     0,     0,     0,     0,     0,   611,   612,
       0,     0,   644,   645,     0,     0,  1372,   613,   614,   615,
     616,   617,     0,     0,   618,   619,   620,   621,     0,   622,
     623,   624,   625,   626,     0,   627,   628,     0,     0,     0,
       0,   629,   630,   631,     0,     0,     0,   632,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   633,     0,   634,   635,
     636,   637,   638,   639,   640,   641,   642,   643,     0,     0,
       0,     0,     0,     0,   611,   612,     0,     0,   644,   645,
       0,     0,  1386,   613,   614,   615,   616,   617,     0,     0,
     618,   619,   620,   621,     0,   622,   623,   624,   625,   626,
       0,   627,   628,     0,     0,     0,     0,   629,   630,   631,
       0,     0,     0,   632,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   633,     0,   634,   635,   636,   637,   638,   639,
     640,   641,   642,   643,     0,     0,     0,     0,     0,     0,
     611,   612,     0,     0,   644,   645,     0,     0,  1397,   613,
     614,   615,   616,   617,     0,     0,   618,   619,   620,   621,
       0,   622,   623,   624,   625,   626,     0,   627,   628,     0,
       0,     0,     0,   629,   630,   631,     0,     0,     0,   632,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   633,     0,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   643,
       0,     0,     0,     0,     0,     0,   611,   612,     0,     0,
     644,   645,     0,     0,  1406,   613,   614,   615,   616,   617,
       0,     0,   618,   619,   620,   621,     0,   622,   623,   624,
     625,   626,     0,   627,   628,     0,     0,     0,     0,   629,
     630,   631,     0,     0,     0,   632,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   633,     0,   634,   635,   636,   637,
     638,   639,   640,   641,   642,   643,     0,     0,     0,     0,
       0,     0,   611,   612,     0,     0,   644,   645,     0,     0,
    1476,   613,   614,   615,   616,   617,     0,     0,   618,   619,
     620,   621,     0,   622,   623,   624,   625,   626,     0,   627,
     628,     0,     0,     0,     0,   629,   630,   631,     0,     0,
       0,   632,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     633,     0,   634,   635,   636,   637,   638,   639,   640,   641,
     642,   643,     0,     0,     0,     0,   611,   612,     0,     0,
       0,     0,   644,   645,     0,     0,  1527,   613,   614,   615,
     616,   617,     0,     0,   618,   619,   620,   621,     0,   622,
     623,   624,   625,   626,     0,   627,   628,     0,     0,     0,
       0,   629,   630,   631,     0,     0,     0,   632,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   633,     0,   634,   635,
     636,   637,   638,   639,   640,   641,   642,   643,     0,     0,
     611,   612,     0,     0,     0,     0,     0,     0,   644,   645,
     671,   613,   614,   615,   616,   617,     0,     0,   618,   619,
     620,   621,     0,   622,   623,   624,   625,   626,     0,   627,
     628,     0,     0,     0,     0,   629,   630,   631,     0,     0,
       0,   632,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     633,     0,   634,   635,   636,   637,   638,   639,   640,   641,
     642,   643,     0,     0,   611,   612,     0,     0,     0,     0,
       0,     0,   644,   645,   843,   613,   614,   615,   616,   617,
       0,     0,   618,   619,   620,   621,     0,   622,   623,   624,
     625,   626,     0,   627,   628,     0,     0,     0,     0,   629,
     630,   631,     0,     0,     0,   632,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   633,     0,   634,   635,   636,   637,
     638,   639,   640,   641,   642,   643,     0,     0,   611,   612,
       0,     0,     0,     0,     0,     0,   644,   645,   988,   613,
     614,   615,   616,   617,     0,     0,   618,   619,   620,   621,
       0,   622,   623,   624,   625,   626,     0,   627,   628,     0,
       0,     0,     0,   629,   630,   631,     0,     0,     0,   632,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   633,     0,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   643,
       0,     0,   611,   612,     0,     0,     0,     0,     0,     0,
     644,   645,  1004,   613,   614,   615,   616,   617,     0,     0,
     618,   619,   620,   621,     0,   622,   623,   624,   625,   626,
       0,   627,   628,     0,     0,     0,     0,   629,   630,   631,
       0,     0,     0,   632,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   633,     0,   634,   635,   636,   637,   638,   639,
     640,   641,   642,   643,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   644,   645,  1168,   613,   614,   615,
     616,   617,   283,   284,   618,   619,   620,   621,     0,   622,
     623,   624,   625,   626,     0,   627,   628,     0,     0,   285,
       0,   629,   630,   631,     0,     0,     0,   632,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   633,     0,   634,   635,
     636,   637,   638,   639,   640,   641,   642,   643,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   644,   645,
    1174,     0,     0,     0,     0,   611,   612,     0,     0,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,     0,     0,   304,
     305,   306,     0,     0,   307,   308,   309,   310,   311,     0,
       0,   312,   313,   314,   315,   316,   317,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   772,   319,     0,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   611,   612,
     330,   331,     0,     0,     0,     0,     0,     0,   332,   333,
     613,   614,   615,   616,   617,     0,     0,   618,   619,   620,
     621,     0,   622,   623,   624,   625,   626,     0,   627,   628,
       0,     0,   755,     0,   629,   630,   631,   234,   235,   236,
     632,   238,   239,   240,   241,   242,   437,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,   256,
     257,   258,     0,     0,   261,   262,   263,   264,     0,   633,
       0,   634,   635,   636,   637,   638,   639,   640,   641,   642,
     643,   611,   612,     0,     0,     0,     0,     0,     0,     0,
       0,   644,   645,   613,   614,   615,   616,   617,     0,     0,
     618,   619,   620,   621,     0,   622,   623,   624,   625,   626,
       0,   627,   628,     0,     0,     0,     0,   629,   630,   631,
     773,     0,     0,   632,     0,     0,     0,     0,     0,   774,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   633,   993,   634,   635,   636,   637,   638,   639,
     640,   641,   642,   643,   611,   612,     0,     0,     0,     0,
       0,     0,     0,     0,   644,   645,   613,   614,   615,   616,
     617,     0,     0,   618,   619,   620,   621,     0,   622,   623,
     624,   625,   626,     0,   627,   628,     0,     0,     0,     0,
     629,   630,   631,     0,     0,     0,   632,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1132,     0,     0,     0,   633,     0,   634,   635,   636,
     637,   638,   639,   640,   641,   642,   643,   611,   612,     0,
       0,     0,     0,     0,     0,     0,     0,   644,   645,   613,
     614,   615,   616,   617,     0,     0,   618,   619,   620,   621,
       0,   622,   623,   624,   625,   626,     0,   627,   628,   611,
     612,     0,     0,   629,   630,   631,     0,     0,     0,   632,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   633,     0,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   643,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     644,   645,   613,   614,   615,   616,   617,     0,     0,   618,
     619,   620,   621,     0,   622,   623,   624,   625,   626,     0,
     627,   628,   611,   612,     0,     0,   629,   630,   631,     0,
       0,     0,  -743,     0,   613,   614,   615,   616,   617,     0,
       0,   618,   619,   620,   621,     0,   622,   623,   624,   625,
     626,     0,   627,   628,   611,   612,     0,     0,   629,     0,
       0,   633,     0,   634,   635,   636,   637,   638,   639,   640,
     641,   642,   643,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   644,   645,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   634,   635,   636,   637,   638,
     639,   640,   641,   642,   643,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   644,   645,   613,   614,   615,
     616,   617,     0,     0,   618,   619,   620,   621,     0,   622,
     623,   624,   625,   626,     0,   627,   628,   611,   612,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   613,
     614,   615,   616,   617,     0,     0,   618,   619,   620,   621,
       0,   622,   623,   624,   625,   626,     0,   627,   628,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   635,
     636,   637,   638,   639,   640,   641,   642,   643,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   644,   645,
     776,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   636,   637,   638,   639,   640,   641,   642,   643,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     644,   645,   613,   614,   615,   616,   617,     0,     0,   618,
       0,     0,   621,     0,   622,   623,   624,   625,   626,     0,
     627,   628,   234,   235,   236,     0,   238,   239,   240,   241,
     242,   437,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,   256,   257,   258,     0,     0,   261,
     262,   263,   264,     0,     0,     0,     0,     0,     0,   989,
       0,     0,     0,     0,     0,     0,   637,   638,   639,   640,
     641,   642,   643,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   644,   645,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   777,     0,     0,     0,     0,
       0,   234,   235,   236,   778,   238,   239,   240,   241,   242,
     437,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,   256,   257,   258,     0,     0,   261,   262,
     263,   264,   234,   235,   236,     0,   238,   239,   240,   241,
     242,   437,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,   256,   257,   258,     0,     0,   261,
     262,   263,   264,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   990,     0,     0,     0,     0,     0,
     812,   813,     0,   991,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   814,     0,     0,     0,     0,
       0,     0,     0,     0,   815,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   816,   817
};

static const yytype_int16 yycheck[] =
{
       1,   138,   139,   162,   543,   176,   539,    14,    15,   756,
     685,   598,   608,   605,   675,  1071,   603,   711,   522,    20,
      21,    22,    80,   604,   654,   377,   656,   499,   658,   501,
       8,   503,   661,   521,    36,    33,   665,     0,  1345,     5,
      22,   744,   771,   476,     7,    20,    33,   467,    15,    16,
      51,   754,    46,     5,     6,  1181,    63,    64,    65,   129,
     130,   413,    21,    22,   758,   464,   148,    30,   165,    32,
     154,    34,    20,    36,   148,    57,    20,    40,   867,   163,
     217,   163,   669,    20,   127,   148,     7,    50,   127,   163,
     133,   720,   151,    56,  1401,   102,   103,   104,   105,   151,
     163,   127,   165,   200,    36,    19,    20,   133,   163,   508,
    1415,   176,   106,   140,   141,   142,   200,    80,   200,    15,
      16,    34,   165,   205,  1254,   165,    33,   197,   198,    50,
     711,   174,   719,   165,   721,   174,   102,   103,   177,   102,
     103,   204,   729,   206,   203,   732,   566,   202,   174,   201,
      63,   203,   156,    60,    61,   107,  1461,   696,   139,   118,
     119,   204,   220,   165,   204,   564,   206,   126,   200,   176,
     129,   130,   131,   132,   201,   574,   609,   758,   577,  1385,
     761,   179,   177,     8,   845,   344,   181,   165,  1394,   156,
    1320,  1321,   179,   854,   161,   176,   163,    62,   173,   166,
     198,  1327,   165,   185,   341,   604,   377,  1337,   345,   207,
     839,    21,    22,   565,   180,   197,   945,   124,   219,   132,
     198,   128,  1468,   186,   918,   173,   139,   579,   206,   173,
    1436,  1437,   403,   200,   200,   198,   173,   127,   197,   198,
     174,   204,   413,     5,     6,   416,   417,   418,   647,   163,
     163,   954,   176,  1415,  1500,  1311,  1415,   154,  1388,  1389,
     156,   268,  1051,    25,  1415,   161,   163,   163,   175,    31,
     166,  1415,   179,   154,   411,   182,  1447,   190,   173,   127,
     163,   139,   163,   173,   174,   133,   176,   200,   346,   179,
     148,   198,   127,   922,   173,   127,   768,   165,   133,  1461,
     207,   133,  1461,   186,   201,   163,    68,    69,   118,   119,
    1461,   126,   711,  1484,   129,   130,   126,  1461,   128,   129,
     130,   131,   132,   494,   495,   181,   174,   498,   163,   500,
     173,   502,   163,   504,   838,   173,   204,   918,   403,   174,
     102,   103,   174,   695,   179,   139,   850,   835,   204,   979,
     521,   416,   417,   418,    21,    22,   708,   709,   201,   163,
     848,   165,   163,   201,  1039,   173,   718,   173,   907,   163,
     377,   723,   724,  1034,   726,   173,   728,   139,   730,   731,
     127,   733,   197,   198,   127,   127,   133,   197,   198,   173,
     133,   133,   139,   201,   565,   201,   403,   173,   139,   173,
     204,   163,   176,   201,  1193,   179,   413,   148,   579,   416,
     417,   418,   153,   139,   173,   199,   423,   424,   180,   173,
     173,   167,   163,   202,   203,   201,   205,   174,   173,   494,
     495,   174,   174,   498,   173,   500,   198,   502,   173,   180,
     611,   612,   201,   205,    63,    64,    65,   201,   201,   116,
     117,   118,   119,   173,   173,   626,   201,     5,     6,   126,
     199,   128,   129,   130,   131,   132,   201,   134,   135,  1258,
     173,   200,  1147,   644,   173,   173,   173,   173,   173,   199,
     199,   164,   165,   102,   103,   104,   105,   494,   495,   961,
     173,   498,  1281,   500,   177,   502,   667,   504,   201,  1202,
    1092,   156,   201,   201,   201,   201,   201,   173,  1138,   981,
     148,    12,   180,   197,   521,   182,   183,   184,   185,   186,
     173,   204,    23,    24,   695,   163,  1156,   534,    21,    22,
     197,   198,   200,   199,   180,   164,   707,   708,   709,   710,
     154,   163,   543,   714,   173,   176,   199,   718,   176,   163,
     181,   179,   723,   724,   200,   726,   176,   728,   565,   730,
     731,   181,   733,   180,  1145,   180,   176,   164,   165,   200,
    1359,   181,   579,   925,   926,   927,   173,   164,   165,   931,
     200,   180,   163,   200,   181,   200,   173,   986,   154,  1170,
     200,   176,   599,  1268,   181,   163,   181,   163,   605,   998,
      47,   200,   667,   201,  1003,   154,   204,   204,   960,   154,
     164,   165,   163,   164,   163,   200,   163,   204,   163,   173,
      67,   163,   173,   164,   165,   118,   119,   181,    57,   800,
    1419,  1420,   173,   126,    63,   128,   129,   130,   131,   132,
     181,    57,   707,   163,   164,   710,    57,    63,   173,   714,
     204,   176,    63,   173,   179,  1402,    57,   164,   165,   163,
     667,  1248,   833,   204,   835,   836,   173,    57,   177,    57,
     204,   205,   181,    63,   181,    63,   677,   848,   679,    57,
     177,    57,   167,   168,   181,    63,   687,    63,   695,   163,
     861,   184,   185,   186,   140,   177,   142,   204,   699,   181,
     707,   708,   709,   710,   197,   198,  1495,   714,   177,   177,
     177,   718,   181,   181,   181,   205,   723,   724,   177,   726,
    1509,   728,   181,   730,   731,    75,   733,    10,    11,    79,
     174,  1406,  1131,   164,  1133,   177,  1525,   874,   177,   181,
    1139,  1530,   181,    93,    94,   140,   141,   142,    98,    99,
     100,   101,   165,   167,   925,   926,   927,  1338,   177,   140,
     931,   163,   181,   167,   168,   169,   170,   165,   833,   770,
     203,   836,   164,   167,   168,   169,   176,  1373,   167,   168,
     169,    35,  1181,    55,    56,    57,    35,   106,   200,   960,
     163,   200,   200,   203,   180,   180,   861,   180,   180,   205,
     180,   180,   177,   180,   200,  1157,  1393,   163,   180,  1390,
     180,   180,   180,    22,   205,   163,   163,   163,   205,   176,
     163,   163,   163,   133,   176,   199,   833,   198,   835,   836,
     163,   180,   199,    43,   198,   200,   180,   200,   199,   205,
     180,   848,  1375,   205,  1431,   180,   180,   848,  1429,   850,
     180,   180,   180,  1024,   861,   200,   180,   163,   163,   163,
     180,   200,   200,  1262,   200,   181,   203,   180,   200,   200,
     871,   180,    13,   200,   200,   200,   877,   878,   200,   173,
     201,   173,   200,   200,     4,  1424,   200,   163,   200,   890,
     176,    43,   893,   894,   895,   896,   897,  1493,   201,   163,
     901,   203,   200,   354,   163,   163,   907,   163,   909,   200,
     181,   199,   201,   364,   201,   200,   200,   180,   925,   926,
     927,  1502,   200,   374,   931,   206,   173,  1466,  1327,   180,
     200,   200,   200,   200,   200,  1334,  1095,     1,   200,   200,
     200,   163,   181,   181,   181,   174,   163,   163,   163,   156,
      10,    13,     9,   960,    42,    66,   206,    43,   181,  1024,
     181,   201,   181,   181,   206,   201,   201,   200,   181,   201,
      19,  1370,   200,   181,   201,   181,    25,   181,   181,   181,
     180,   201,    31,    14,    43,   174,  1157,   201,  1159,   200,
     441,   442,    41,  1130,   206,   200,   176,   156,    37,   200,
      49,   200,   200,   200,   200,   200,     8,   458,   459,   460,
     461,   462,   206,   200,   163,    64,   467,  1024,  1155,   201,
     163,   200,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   181,   181,   181,   181,   181,
     163,   201,   200,   163,   200,   163,   517,   163,   163,   206,
     163,  1423,    67,   200,   181,   526,    70,   181,   163,   200,
     138,   200,   181,   201,   200,  1092,   200,   200,   200,   204,
     139,   201,   201,    12,  1159,   200,   547,   205,   200,  1100,
     201,    33,   163,   152,   201,   173,   200,    21,    22,   200,
      53,   201,   204,   200,   163,   566,   567,   201,  1289,   570,
     201,   572,   204,   201,   201,   205,   201,   201,   201,   205,
     201,   582,   583,   584,   585,   586,   587,   204,   201,   588,
     206,   206,    79,   199,   199,  1282,   200,     1,    45,   130,
    1157,    82,  1159,   202,   206,  1532,   679,   679,   207,   101,
    1528,  1330,   613,   614,     1,   903,   617,   618,   619,   620,
    1279,   622,   483,   624,   625,   626,   627,   628,   629,   630,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
     641,   642,   643,   226,   645,  1380,  1427,  1383,  1315,  1428,
    1034,    54,   116,   117,   118,   119,   120,   854,   531,   123,
     124,   125,   126,   854,   128,   129,   130,   131,   132,    -1,
     134,   135,  1390,    -1,  1289,    -1,   140,   141,   142,   282,
     423,  1232,  1369,  1234,  1371,  1372,   687,   423,    -1,    -1,
      -1,   692,    -1,    -1,    -1,    -1,    -1,   698,    -1,    -1,
      -1,    -1,  1423,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,   713,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,  1289,   197,   198,    -1,    -1,    -1,    -1,    -1,
      -1,   742,    -1,    -1,    -1,   746,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   755,   756,    -1,    -1,    -1,    -1,
    1311,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   770,
      -1,    -1,    -1,    -1,    -1,  1462,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1345,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1488,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   813,    -1,    21,    22,   817,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
    1517,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
    1527,    -1,    -1,   140,  1401,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   856,    -1,    -1,    -1,    -1,
      -1,    -1,   863,    -1,   865,  1416,  1423,   868,    -1,    -1,
    1421,    -1,   873,    -1,   875,    -1,    -1,    -1,    -1,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,    -1,    -1,    -1,    -1,    -1,   908,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1494,    -1,    -1,    -1,  1498,  1499,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   958,   959,    -1,
    1511,    -1,    -1,  1514,    -1,    -1,    -1,    -1,   969,   970,
     971,    -1,   973,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,   983,    -1,   985,    -1,   987,    -1,    -1,    -1,
     197,   198,   993,    -1,    -1,    -1,   997,    -1,    21,    22,
      -1,    -1,    -1,    -1,  1005,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1033,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1041,    -1,    -1,  1044,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1052,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1073,  1074,  1075,  1076,  1077,  1078,  1079,  1080,
    1081,  1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,
      -1,    -1,    -1,   116,   117,   118,   119,   120,  1099,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      10,   134,   135,    -1,    -1,    -1,    -1,  1118,   118,   119,
      -1,    21,    22,    -1,    -1,    -1,   126,    -1,   128,   129,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,  1165,    -1,  1167,    -1,    -1,    -1,
      -1,    -1,  1173,    -1,   197,   198,    -1,    -1,    -1,    -1,
      -1,    -1,   182,   183,   184,   185,   186,    -1,    -1,    -1,
    1191,    -1,    -1,    -1,  1195,    -1,    -1,   197,   198,  1200,
    1201,    -1,  1203,  1204,    -1,    21,    22,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,  1240,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,  1250,
    1251,  1252,    -1,    -1,    -1,    -1,  1257,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1271,  1272,  1273,    -1,   174,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,  1330,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1348,  1349,   165,
      -1,    -1,    -1,    -1,    -1,  1356,    -1,    -1,    -1,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,   197,   198,    -1,    -1,    -1,  1387,    -1,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1398,    -1,    -1,
      -1,  1402,    -1,    -1,    -1,    -1,    -1,  1408,    -1,    -1,
      -1,    71,    -1,    73,  1415,    75,    76,    77,    78,    79,
    1421,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,    -1,    -1,  1445,    -1,    -1,  1448,  1449,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
    1461,    -1,    -1,    -1,     1,    -1,  1467,    -1,     5,     6,
       7,    -1,     9,    10,    11,    -1,    13,    -1,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    -1,    31,    -1,  1497,    -1,    -1,    -1,
      -1,    38,    39,   163,  1505,    42,    -1,    44,    45,  1510,
    1511,    48,    -1,    50,    51,    52,    -1,    54,    55,    -1,
      -1,    58,    59,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,   150,   151,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,
      -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    -1,   200,    -1,   202,   203,   204,   205,   206,
       1,    -1,    -1,    -1,     5,     6,     7,    -1,     9,    10,
      11,    -1,    13,    -1,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,
      -1,    42,    -1,    44,    45,    -1,    -1,    48,    -1,    50,
      51,    52,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,
     151,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,   190,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,   200,
      -1,   202,   203,   204,   205,   206,     1,    -1,    -1,    -1,
       5,     6,     7,    -1,     9,    10,    11,    -1,    13,    -1,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    39,    -1,    -1,    42,    -1,    44,
      45,    -1,    -1,    48,    -1,    50,    51,    52,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,   150,   151,    -1,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,
      -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   198,    -1,   200,    -1,   202,   203,   204,
     205,   206,     1,    -1,    -1,    -1,     5,     6,     7,    -1,
       9,    10,    11,    -1,    13,    -1,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    42,    -1,    44,    45,    -1,    -1,    48,
      -1,    50,    51,    52,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,   151,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,
      -1,   200,    -1,   202,   203,   204,   205,   206,     5,     6,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    52,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,   150,   151,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,
      -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,     5,
       6,   198,    -1,   200,    -1,   202,   203,    -1,   205,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    70,    71,    72,    73,    -1,    75,
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
       5,     6,   198,    -1,   200,   201,   202,   203,    13,   205,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,
      -1,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,
      -1,     5,     6,   198,    -1,   200,    -1,   202,   203,    13,
     205,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    -1,
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
      -1,    -1,    65,    -1,    -1,    68,    69,    70,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
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
      -1,    -1,    -1,    -1,     5,     6,   198,   199,   200,    -1,
     202,   203,    13,   205,    15,    16,    17,    18,    19,    -1,
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
     161,   162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,   190,
      -1,    -1,    -1,    -1,    -1,     5,     6,   198,    -1,   200,
      -1,   202,   203,    -1,   205,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
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
     150,   151,    -1,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,
     190,    -1,    -1,    -1,    -1,    -1,     5,     6,   198,   199,
     200,    -1,   202,   203,    -1,   205,    15,    16,    17,    18,
      19,    -1,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,   150,   151,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,
      -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,     5,
       6,   198,    -1,   200,    -1,   202,   203,    -1,   205,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    65,
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
       5,     6,   198,    -1,   200,    -1,   202,   203,    -1,   205,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,     5,     6,   198,    -1,   200,    -1,   202,   203,    -1,
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
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,   150,   151,
      -1,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,   183,   184,    -1,   186,    -1,    -1,   189,   190,    -1,
      -1,    -1,    -1,    -1,     5,     6,   198,    -1,   200,   201,
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
     161,   162,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,   190,
      -1,    -1,    -1,    -1,    -1,     5,     6,   198,    -1,   200,
     201,   202,   203,    -1,   205,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
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
     150,   151,    -1,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,   189,
     190,    -1,    -1,    -1,    -1,    -1,     5,     6,   198,    -1,
     200,   201,   202,   203,    -1,   205,    15,    16,    17,    18,
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
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,   151,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   182,   183,   184,    -1,   186,    -1,    -1,
     189,   190,    -1,    -1,    -1,    -1,    -1,     5,     6,   198,
      -1,   200,   201,   202,   203,    -1,   205,    15,    16,    17,
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
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
     148,   149,   150,   151,    -1,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,    -1,
      -1,   189,   190,    -1,    -1,    -1,    -1,    -1,     5,     6,
     198,    -1,   200,    -1,   202,   203,    -1,   205,    15,    16,
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
       6,   198,    -1,   200,    -1,   202,   203,    -1,   205,    15,
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
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    19,
     166,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,
     186,    41,    -1,   189,   190,    -1,    -1,    -1,    -1,    49,
      -1,    -1,   198,    -1,   200,    -1,   202,   203,    -1,   205,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,   163,    -1,   165,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,   202,    -1,   204,    21,    22,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   163,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,   202,    21,    22,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      -1,    -1,    -1,    -1,   140,   141,   142,   139,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,    -1,    21,    22,    -1,    -1,   204,    -1,
     202,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
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
     185,   186,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,   197,   198,    -1,    -1,   201,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     199,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,   134,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,   199,   116,   117,   118,   119,   120,
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
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,   199,   116,   117,   118,
     119,   120,    21,    22,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    -1,    -1,    38,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     199,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,    -1,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    21,    22,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   132,    -1,   134,   135,
      -1,    -1,   138,    -1,   140,   141,   142,    71,    72,    73,
     146,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,    -1,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,   141,   142,
     154,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,   132,    -1,   134,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    21,
      22,    -1,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    21,    22,    -1,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,    -1,   134,   135,    21,    22,    -1,    -1,   140,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   132,    -1,   134,   135,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,   132,    -1,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,   116,   117,   118,   119,   120,    -1,    -1,   123,
      -1,    -1,   126,    -1,   128,   129,   130,   131,   132,    -1,
     134,   135,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,   163,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,
     129,   130,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   209,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   186,   198,   204,   211,   212,   215,
     221,   223,   224,   228,   267,   268,   272,   301,   379,   387,
     394,   404,   451,   456,   461,    19,    20,   163,   259,   260,
     261,   156,   229,   230,   163,   186,   225,   226,    57,    63,
     384,   385,   163,   202,   214,   462,   452,   457,   139,   163,
     289,    34,    63,   132,   190,   200,   263,   264,   265,   266,
     289,   211,   211,   211,    36,     8,   405,    62,   375,   174,
     173,   176,   173,   225,    22,    57,   185,   197,   227,   163,
     211,   375,   384,   384,   384,   163,   139,   222,   265,   265,
     265,   200,   140,   141,   142,   173,   199,   395,    57,    63,
     273,   275,     5,     6,   107,   401,    57,    63,   376,    15,
      16,   156,   161,   163,   166,   200,   217,   260,   156,   230,
     163,   163,   163,   386,    57,    63,   213,   163,   163,   163,
     163,   167,   220,   201,   261,   265,   265,   265,   265,   165,
     234,   235,    57,    63,   388,   276,     5,     6,    57,    63,
     402,   205,   380,   167,   168,   169,   216,    15,    16,   156,
     161,   163,   217,   257,   258,   227,   174,   164,   392,   393,
     235,   235,   167,   201,   165,   163,   389,    35,    71,    73,
      75,    76,    77,    78,    79,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    93,    94,    95,    98,
      99,   100,   101,   118,   119,   163,   271,   274,   406,   377,
     203,   262,   348,   167,   168,   169,   173,   201,    19,    25,
      31,    41,    49,    64,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   152,   202,   289,
     409,   411,   412,   415,   421,   422,   450,   164,   463,   453,
     458,   176,   390,    21,    22,    38,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   128,   129,   130,   133,   134,   135,
     136,   137,   140,   141,   142,   143,   144,   145,   146,   175,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     189,   190,   197,   198,    35,    35,   200,   269,   235,   106,
     399,   400,   206,   211,   378,   260,   203,   163,   371,   374,
     257,   180,   180,   180,   200,   180,   180,   200,   180,   180,
     180,   180,   180,   180,   200,   289,    33,    60,    61,   124,
     128,   175,   179,   182,   198,   207,   420,   177,   205,   205,
     205,    75,    79,    93,    94,    98,    99,   100,   101,   410,
     393,   163,   163,   163,   199,    22,   163,   199,   151,   201,
     348,   358,   359,   176,   270,   280,   281,   163,   235,   348,
     235,   260,   173,   176,   179,   369,   423,   428,   430,     5,
       6,    15,    16,    17,    18,    19,    25,    27,    31,    39,
      45,    48,    51,    55,    65,    68,    69,    80,   102,   103,
     104,   118,   119,   147,   148,   149,   150,   151,   153,   155,
     156,   157,   158,   159,   160,   161,   162,   166,   182,   183,
     184,   189,   190,   198,   200,   202,   203,   205,   219,   221,
     283,   289,   294,   306,   313,   316,   319,   323,   325,   327,
     328,   330,   335,   338,   339,   346,   347,   409,   466,   474,
     484,   487,   499,   502,   432,   426,   163,   416,   434,   436,
     438,   440,   442,   444,   446,   448,   339,   180,   200,    33,
     179,    33,   179,   198,   207,   199,   339,   198,   207,   421,
     464,   454,   459,   205,   163,   133,   200,     7,    50,   300,
     201,   204,   450,   205,   176,   398,   407,   148,   163,   370,
     373,   235,   163,   421,   127,   133,   174,   368,   450,   450,
     419,   450,   180,   180,   180,   291,   411,   466,   289,   180,
       5,   102,   103,   180,   200,   180,   200,   200,   180,   180,
     200,   180,   200,   180,   200,   180,   180,   200,   180,   180,
     339,   339,   200,   200,   200,   200,   200,   200,   218,   339,
     339,   339,   339,   339,    13,    49,   317,   339,   317,   203,
      13,   339,   483,   498,   200,   198,   267,   203,   205,   319,
     324,    21,    22,   116,   117,   118,   119,   120,   123,   124,
     125,   126,   128,   129,   130,   131,   132,   134,   135,   140,
     141,   142,   146,   175,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   197,   198,   201,   200,   450,   450,
     201,   163,   413,   450,   269,   450,   269,   450,   269,   163,
     360,   361,   450,   163,   363,   364,   201,   425,   286,   317,
     199,   199,   199,   339,   163,   414,   360,   362,   363,   365,
     396,   339,   148,   163,   367,   408,   358,   285,   289,   204,
     205,   403,   200,    43,   173,   176,   369,   211,   368,   339,
     181,   181,   181,   164,   173,   210,   211,   418,   475,   477,
     292,   200,   180,   200,   314,   180,   180,   180,   494,   317,
     421,   498,   339,   307,   309,   339,   311,   339,   496,   317,
     481,   485,   317,   479,   421,   339,   339,   339,   339,   339,
     339,   169,   170,   216,   200,    13,   173,   501,   501,   148,
     153,   180,   289,   329,   200,   138,   173,   501,    70,   154,
     163,   198,   201,   317,   467,   469,   266,     4,   322,   203,
     285,   267,    19,   154,   163,   409,    19,   154,   163,   409,
     339,   339,   339,   339,   339,   339,   163,   339,   154,   163,
     339,   339,   339,   409,   339,   339,   339,   339,   339,   339,
      22,   339,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,   129,   130,   154,   163,   197,   198,   336,   409,
     339,   201,   317,   181,   181,   181,   204,   181,   270,   181,
     270,   181,   270,   176,   181,   418,   176,   181,   418,   288,
     450,   201,   501,   199,   210,   382,   391,   211,   240,   241,
     240,   241,   163,   381,   383,   201,   200,    43,   127,   173,
     174,   176,   179,   366,     1,    26,    28,    29,    38,    44,
      52,    54,    58,    59,    65,   105,   211,   231,   232,   239,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   282,   284,   290,   295,   296,   297,   298,   299,   301,
     305,   326,   339,   354,   339,   163,   163,   421,   368,   339,
     211,   424,   429,   431,   450,   421,   421,   450,    70,   317,
     469,   473,   163,   339,   450,   488,   490,   492,   421,   501,
     181,   418,   501,   201,   421,   421,   201,   421,   201,   421,
     501,   421,   421,   501,   421,   181,   201,   201,   201,   201,
     201,   201,   339,   408,   200,   339,   199,   201,   200,   200,
     331,   333,   408,   339,   483,   206,   163,   201,   469,   200,
     133,   366,   467,   173,   201,   173,   201,   199,   200,   269,
     206,   322,   180,   200,   180,   200,   200,   200,   199,    19,
     154,   163,   409,   176,   154,   163,   339,   200,   200,   154,
     163,   339,     1,   200,   199,   173,   201,   433,   427,   417,
     163,   435,   181,   439,   181,   443,   181,   450,   447,   360,
     450,   449,   363,   181,   418,   163,   210,   465,   211,   360,
     455,   363,   460,   174,   382,   391,   339,   163,   163,   450,
     339,   127,   339,   280,    61,   339,   163,   211,   156,    58,
     339,   235,   127,   339,   211,   211,    10,    10,    11,   237,
      13,     9,    42,   211,   206,   211,   211,   211,   211,   211,
      66,   302,   211,   108,   109,   110,   111,   112,   113,   114,
     115,   121,   122,   127,   133,   136,   137,   143,   144,   145,
     174,   165,   198,   206,   268,   355,   201,    43,   211,   368,
     339,   211,   181,   181,   181,   469,   201,   201,   201,   181,
     418,   201,   181,   421,   421,   421,   181,   201,   200,   421,
     201,   181,   181,   181,   181,   201,   181,   181,   201,   181,
     322,   200,   171,    20,   408,   339,   339,   421,   269,    20,
     201,   339,   339,   339,   199,   198,   154,   163,   127,   133,
     163,   174,   179,   320,   321,   270,   269,   340,   339,   342,
     339,   201,   317,   339,   180,   200,   339,   200,   199,   339,
     198,   201,   317,   200,   199,   337,   201,   317,   437,   441,
     445,   200,   450,   206,   206,   206,   339,   381,   397,   201,
      43,   366,   339,    14,   339,   174,   176,   156,   280,   339,
     200,   200,   200,   200,   200,    37,   277,   200,   205,   304,
     373,   339,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,   339,   339,   339,   339,   339,   339,   339,   266,
       8,   348,   353,   163,   339,   211,   476,   478,   293,   201,
     200,   163,   315,   181,   181,   181,   495,   287,   498,   181,
     308,   310,   312,   497,   482,   486,   480,   200,   235,   201,
     317,   317,    20,   201,   201,   181,   270,   317,   201,   201,
     467,   200,   133,   366,   163,   163,   200,   163,   163,   173,
     201,   235,   270,   421,   201,   450,   201,   201,   201,   344,
     339,   339,   201,   467,   201,   339,   201,   286,   206,   163,
     339,   280,   339,   339,   339,   408,   339,   339,   278,   163,
     303,   372,   199,    57,    63,   351,    67,   352,   211,   211,
     200,   200,   339,   181,   489,   491,   493,   200,   201,   501,
     200,   339,   339,   339,   200,    70,   473,   200,   200,   201,
     339,   280,   201,   201,   317,   332,   181,   201,   133,   366,
     199,   339,   339,   339,   163,   320,   138,   280,   318,   235,
     181,   181,   450,   201,   201,   199,   201,   201,   201,   201,
      20,   201,   201,   205,   173,   201,   206,   211,   373,    47,
     352,    46,   106,   349,   473,   473,   201,   200,   200,   200,
     200,   286,   287,   317,   473,   467,   468,   201,   204,   201,
     329,   334,   204,   339,   339,   201,   201,   201,   127,   339,
     280,   341,   343,   181,   201,    12,   238,   235,   317,   235,
     235,   285,   163,   176,   369,    33,   350,   349,   351,   200,
     467,   470,   471,   472,   472,   339,   473,   473,   468,   201,
     201,   501,   472,   201,   339,   204,   329,   483,   133,   366,
     339,   345,   239,   295,   296,   297,   298,   339,   211,   242,
     243,   245,   201,   280,   280,   279,   421,   368,   356,   350,
     367,   467,   173,   501,   201,   201,   201,   472,   472,   201,
     201,   201,   204,   500,   339,   500,   339,   339,    12,    23,
      24,   233,   236,   205,   239,   235,   206,   368,   339,   274,
     357,   201,   200,   201,   201,    53,   199,   500,   206,   235,
     200,   285,   211,   280,   339,   211,   211,   274,   467,   339,
     199,   280,   339,   244,   211,   235,   201,   201,   206,   280,
     235,   277,   280,   236
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   208,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   210,   210,   211,
     211,   212,   213,   213,   213,   214,   214,   215,   216,   216,
     216,   216,   217,   217,   218,   218,   218,   219,   220,   220,
     222,   221,   223,   224,   225,   225,   225,   225,   226,   226,
     227,   227,   228,   229,   229,   230,   230,   231,   232,   232,
     233,   233,   234,   234,   235,   235,   236,   236,   236,   237,
     237,   238,   238,   239,   239,   239,   239,   239,   240,   240,
     241,   241,   243,   244,   242,   245,   242,   247,   246,   248,
     250,   249,   251,   253,   252,   255,   254,   256,   257,   257,
     257,   257,   257,   257,   258,   258,   259,   259,   259,   260,
     260,   260,   260,   260,   260,   260,   260,   261,   261,   262,
     262,   263,   263,   263,   264,   264,   265,   265,   265,   265,
     265,   265,   265,   266,   266,   267,   267,   268,   268,   269,
     269,   269,   270,   270,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   272,
     273,   273,   273,   274,   276,   275,   277,   278,   279,   277,
     281,   282,   280,   283,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   285,   285,   285,   286,   286,   287,   287,
     288,   288,   289,   289,   289,   290,   290,   292,   293,   291,
     291,   294,   294,   294,   294,   294,   294,   295,   296,   297,
     297,   297,   298,   298,   299,   300,   300,   300,   301,   301,
     302,   302,   303,   303,   304,   304,   305,   305,   305,   307,
     308,   306,   309,   310,   306,   311,   312,   306,   314,   315,
     313,   316,   316,   316,   317,   317,   318,   318,   318,   319,
     319,   319,   320,   320,   320,   320,   320,   321,   321,   322,
     322,   323,   324,   324,   325,   325,   325,   325,   325,   325,
     325,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     327,   327,   328,   328,   329,   329,   330,   331,   332,   330,
     333,   334,   330,   335,   335,   335,   335,   335,   335,   335,
     336,   337,   335,   338,   338,   338,   338,   338,   338,   338,
     339,   339,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,   339,   339,   339,   340,   341,   339,   339,   339,
     339,   342,   343,   339,   339,   339,   344,   345,   339,   339,
     339,   339,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,   346,   346,   346,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   348,   348,   349,   349,   349,   350,   350,   351,   351,
     351,   352,   352,   353,   354,   354,   355,   354,   356,   354,
     357,   354,   358,   358,   359,   359,   360,   360,   361,   361,
     362,   362,   362,   363,   364,   364,   365,   365,   365,   366,
     366,   367,   367,   367,   367,   367,   368,   368,   368,   369,
     369,   370,   370,   370,   370,   370,   371,   371,   372,   372,
     372,   373,   373,   373,   374,   374,   374,   375,   375,   376,
     376,   376,   377,   377,   378,   377,   379,   380,   379,   381,
     381,   382,   382,   383,   383,   383,   384,   384,   384,   386,
     385,   387,   388,   388,   388,   389,   390,   390,   391,   391,
     392,   392,   393,   393,   395,   396,   397,   394,   398,   398,
     399,   399,   400,   401,   401,   401,   401,   402,   402,   402,
     403,   403,   405,   406,   407,   404,   408,   408,   408,   408,
     408,   409,   409,   409,   409,   409,   409,   409,   409,   409,
     409,   409,   409,   409,   409,   409,   409,   409,   409,   409,
     409,   409,   409,   409,   409,   409,   409,   409,   410,   410,
     410,   410,   410,   410,   410,   410,   411,   412,   412,   412,
     413,   413,   414,   414,   414,   416,   417,   415,   418,   418,
     419,   419,   420,   420,   420,   420,   421,   421,   422,   422,
     422,   422,   423,   424,   422,   422,   422,   425,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   426,   427,   422,   422,   428,   429,   422,   430,   431,
     422,   432,   433,   422,   422,   434,   435,   422,   436,   437,
     422,   422,   438,   439,   422,   440,   441,   422,   422,   442,
     443,   422,   444,   445,   422,   446,   447,   422,   448,   449,
     422,   450,   450,   450,   452,   453,   454,   455,   451,   457,
     458,   459,   460,   456,   462,   463,   464,   465,   461,   466,
     466,   466,   466,   466,   467,   467,   467,   467,   467,   467,
     467,   467,   468,   469,   470,   470,   471,   471,   472,   472,
     473,   473,   475,   476,   474,   477,   478,   474,   479,   480,
     474,   481,   482,   474,   483,   483,   484,   485,   486,   484,
     487,   488,   489,   487,   490,   491,   487,   492,   493,   487,
     487,   494,   495,   487,   487,   496,   497,   487,   498,   498,
     499,   499,   499,   499,   500,   500,   501,   501,   502,   502,
     502
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     3,
       3,     2,     2,     2,     2,     2,     2,     1,     1,     1,
       1,     2,     0,     1,     1,     1,     1,     4,     1,     1,
       2,     2,     3,     2,     0,     2,     4,     3,     1,     2,
       0,     4,     2,     2,     1,     2,     3,     3,     2,     4,
       0,     1,     2,     1,     3,     1,     3,     3,     3,     2,
       1,     1,     1,     2,     0,     1,     0,     3,     7,     1,
       1,     0,     2,     1,     1,     1,     1,     1,     1,     2,
       0,     1,     0,     0,     6,     0,     3,     0,     8,     7,
       0,     9,     3,     0,     7,     0,     7,     4,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     1,     5,     1,     3,     3,
       4,     1,     1,     1,     1,     4,     1,     2,     3,     3,
       3,     3,     2,     1,     3,     0,     3,     0,     4,     0,
       2,     3,     0,     2,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     3,     3,
       2,     2,     3,     4,     3,     2,     2,     2,     2,     2,
       3,     3,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     1,     1,     3,     0,     5,     0,     0,     0,     6,
       0,     0,     6,     2,     1,     2,     2,     1,     1,     1,
       1,     2,     1,     2,     2,     2,     2,     1,     1,     1,
       2,     2,     2,     0,     2,     2,     0,     2,     0,     2,
       1,     3,     1,     3,     2,     2,     3,     0,     0,     5,
       1,     2,     5,     5,     5,     6,     2,     1,     1,     1,
       2,     3,     2,     3,     4,     1,     1,     0,     1,     1,
       1,     0,     1,     3,     8,     7,     3,     3,     5,     0,
       0,     7,     0,     0,     7,     0,     0,     7,     0,     0,
       6,     5,     8,    10,     1,     3,     1,     2,     3,     1,
       1,     2,     2,     2,     2,     2,     4,     1,     3,     0,
       4,     7,     7,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       6,     8,     5,     6,     1,     4,     3,     0,     0,     8,
       0,     0,     9,     3,     4,     5,     6,     8,     5,     6,
       0,     0,     5,     3,     4,     4,     5,     4,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       4,     4,     5,     4,     5,     3,     4,     1,     1,     2,
       4,     4,     1,     3,     5,     0,     0,     8,     3,     3,
       3,     0,     0,     8,     3,     4,     0,     0,     9,     4,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       1,     4,     7,     8,     7,     4,     4,     4,     4,     4,
       1,     6,     7,     6,     6,     7,     7,     6,     7,     6,
       6,     0,     1,     0,     1,     1,     0,     1,     0,     1,
       1,     0,     1,     5,     0,     2,     0,     4,     0,     9,
       0,    11,     3,     4,     1,     3,     1,     3,     1,     3,
       0,     1,     3,     3,     1,     3,     0,     1,     3,     1,
       1,     1,     2,     3,     5,     3,     1,     1,     1,     0,
       1,     1,     4,     3,     3,     5,     1,     3,     0,     2,
       2,     4,     6,     5,     4,     6,     5,     0,     1,     0,
       1,     1,     0,     2,     0,     4,     6,     0,     6,     1,
       3,     1,     2,     0,     1,     3,     0,     1,     1,     0,
       5,     3,     0,     1,     1,     1,     0,     2,     0,     1,
       1,     2,     0,     1,     0,     0,     0,    13,     0,     2,
       0,     1,     3,     1,     1,     2,     2,     0,     1,     1,
       1,     3,     0,     0,     0,     9,     1,     4,     3,     3,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       1,     3,     0,     1,     3,     0,     0,     6,     1,     1,
       1,     3,     3,     2,     4,     3,     1,     2,     1,     1,
       1,     1,     0,     0,     6,     4,     5,     0,     9,     4,
       2,     2,     3,     2,     3,     2,     2,     3,     3,     3,
       2,     0,     0,     6,     2,     0,     0,     6,     0,     0,
       6,     0,     0,     6,     1,     0,     0,     6,     0,     0,
       7,     1,     0,     0,     6,     0,     0,     7,     1,     0,
       0,     6,     0,     0,     7,     0,     0,     6,     0,     0,
       6,     1,     3,     3,     0,     0,     0,     0,    12,     0,
       0,     0,     0,    12,     0,     0,     0,     0,    12,     1,
       1,     1,     1,     1,     3,     3,     5,     5,     6,     6,
       8,     8,     1,     1,     3,     5,     1,     2,     1,     0,
       0,     1,     0,     0,    10,     0,     0,    10,     0,     0,
       9,     0,     0,     7,     3,     1,     5,     0,     0,    10,
       4,     0,     0,    11,     0,     0,    11,     0,     0,    10,
       5,     0,     0,     9,     5,     0,     0,    10,     1,     3,
       4,     5,     7,     9,     0,     3,     0,     1,    11,    12,
      11
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
            yyextra->g_Program->thisModule->name = *(yyvsp[-2].s);
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

  case 34: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 35: /* string_builder_body: string_builder_body character_sequence  */
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

  case 36: /* string_builder_body: string_builder_body "{" expr "}"  */
                                                                                                        {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 37: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 38: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 39: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 40: /* $@1: %empty  */
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

  case 41: /* expr_reader: '%' name_in_namespace $@1 reader_character_sequence  */
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

  case 42: /* options_declaration: "options" annotation_argument_list  */
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

  case 44: /* require_module_name: "name"  */
                   {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 45: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 46: /* require_module_name: require_module_name '.' "name"  */
                                                {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 47: /* require_module_name: require_module_name '/' "name"  */
                                                {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 48: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 49: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 50: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 51: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 55: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 56: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 57: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 58: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 59: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 60: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 61: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 66: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 67: /* expression_else: "else" optional_emit_semis expression_block  */
                                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 68: /* expression_else: elif_or_static_elif '(' expr ')' optional_emit_semis expression_block expression_else  */
                                                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-4].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-6].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 69: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 70: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 71: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 72: /* expression_else_one_liner: "else" expression_if_one_liner  */
                                                      {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 73: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 74: /* expression_if_one_liner: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 75: /* expression_if_one_liner: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 76: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 77: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 82: /* $@2: %empty  */
        {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 83: /* $@3: %empty  */
                                      {
        yyextra->pop_nesteds();
    }
    break;

  case 84: /* expression_if_block: $@2 '{' expressions $@3 '}' expression_block_finally  */
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

  case 85: /* $@4: %empty  */
       {
        yyextra->das_keyword = false;
    }
    break;

  case 86: /* expression_if_block: $@4 expression_if_one_liner SEMICOLON  */
                                               {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 87: /* $@5: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 88: /* expression_if_then_else: $@5 if_or_static_if '(' expr ')' optional_emit_semis expression_if_block expression_else  */
                                                                                                                   {
        yyextra->das_keyword = false;
        if ( !(yyvsp[-1].pExpression)->rtti_isBlock() && (yyvsp[0].pExpression)) {
            das2_yyerror(scanner,"if one-liner can't have else clause",tokAt(scanner,(yylsp[-6])), CompilationError::syntax_error);
        }
        auto blk = (yyvsp[-1].pExpression)->rtti_isBlock() ? static_cast<ExprBlock *>((yyvsp[-1].pExpression)) : ast_wrapInBlock((yyvsp[-1].pExpression));
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-4].pExpression)),
            blk,ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-6].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 89: /* expression_if_then_else_oneliner: expression_if_one_liner "if" '(' expr ')' expression_else_one_liner SEMICOLON  */
                                                                                                                      {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-6].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 90: /* $@6: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 91: /* expression_for_loop: $@6 "for" '(' variable_name_with_pos_list "in" expr_list ')' optional_emit_semis expression_block  */
                                                                                                                                 {
        yyextra->das_keyword = false;
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-5].pNameWithPosList),(yyvsp[-3].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 92: /* expression_unsafe: "unsafe" optional_emit_semis expression_block  */
                                                                    {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-2])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 93: /* $@7: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 94: /* expression_while_loop: $@7 "while" '(' expr ')' optional_emit_semis expression_block  */
                                                                                         {
        yyextra->das_keyword = false;
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-5])));
        pWhile->cond = ExpressionPtr((yyvsp[-3].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 95: /* $@8: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 96: /* expression_with: $@8 "with" '(' expr ')' optional_emit_semis expression_block  */
                                                                                   {
        yyextra->das_keyword = false;
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-5])));
        pWith->with = ExpressionPtr((yyvsp[-3].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 97: /* expression_with_alias: "assume" "name" '=' expr  */
                                                      {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-2].s);
    }
    break;

  case 98: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 99: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 100: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 101: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 102: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 103: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 104: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 105: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 106: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 107: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 108: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 109: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 110: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 111: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 112: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 113: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 114: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 115: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 116: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 117: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 118: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 119: /* metadata_argument_list: '@' annotation_argument optional_emit_semis  */
                                                         {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[-1].aa));
    }
    break;

  case 120: /* metadata_argument_list: metadata_argument_list '@' annotation_argument optional_emit_semis  */
                                                                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-3].aaList),(yyvsp[-1].aa));
    }
    break;

  case 121: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 122: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 123: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 124: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 125: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 126: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 127: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 128: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 129: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 130: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 131: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 132: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 133: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 134: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 135: /* optional_annotation_list: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 136: /* optional_annotation_list: '[' annotation_list ']'  */
                                       { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 137: /* optional_annotation_list_with_emit_semis: %empty  */
                                       { (yyval.faList) = nullptr; }
    break;

  case 138: /* optional_annotation_list_with_emit_semis: '[' annotation_list ']' optional_emit_semis  */
                                                          { (yyval.faList) = (yyvsp[-2].faList); }
    break;

  case 139: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 140: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 141: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 142: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 143: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 144: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 145: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 146: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 147: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 148: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 149: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 150: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 151: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 152: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 153: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 154: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 155: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 156: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 157: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 158: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 159: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 160: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 161: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 162: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 163: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 164: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 165: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 166: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 167: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 168: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 169: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 170: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 171: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 172: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 173: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 174: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 175: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 176: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 177: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 178: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 179: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 180: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 181: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 182: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 183: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 184: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 185: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 186: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 187: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 188: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 189: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 190: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 191: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 192: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 193: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 194: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 195: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 196: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 197: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 198: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 199: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 200: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 201: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 202: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 203: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 204: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 205: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 206: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 207: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 208: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 209: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 210: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 211: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 212: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 213: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 214: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 215: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 216: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 217: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 218: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 219: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 220: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 221: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 222: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 223: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 224: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 225: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 226: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 227: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 228: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 229: /* global_function_declaration: optional_annotation_list_with_emit_semis "def" function_declaration  */
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

  case 230: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 231: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 232: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 233: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 234: /* $@9: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 235: /* function_declaration: optional_public_or_private_function $@9 function_declaration_header optional_emit_semis expression_block  */
                                                                                    {
        (yyvsp[-2].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyvsp[-2].pFuncDecl)->privateFunction = !(yyvsp[-4].b);
        (yyval.pFuncDecl) = (yyvsp[-2].pFuncDecl);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
        }
    }
    break;

  case 236: /* expression_block_finally: %empty  */
        {
        (yyval.pExpression) = nullptr;
    }
    break;

  case 237: /* $@10: %empty  */
                  {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 238: /* $@11: %empty  */
                             {
        yyextra->pop_nesteds();
    }
    break;

  case 239: /* expression_block_finally: "finally" $@10 '{' expressions $@11 '}'  */
          {
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 240: /* $@12: %empty  */
        {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 241: /* $@13: %empty  */
                                      {
        yyextra->pop_nesteds();
    }
    break;

  case 242: /* expression_block: $@12 '{' expressions $@13 '}' expression_block_finally  */
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

  case 243: /* expr_call_pipe: expr_call expr_full_block_assumed_piped  */
                                                           {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            ((ExprLooksLikeCall *)(yyvsp[-1].pExpression))->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
            delete (yyvsp[0].pExpression);
        }
    }
    break;

  case 244: /* expression_any: SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 245: /* expression_any: expr_assign SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 246: /* expression_any: expression_delete SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 247: /* expression_any: expression_let  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 248: /* expression_any: expression_while_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 249: /* expression_any: expression_unsafe  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 250: /* expression_any: expression_with  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 251: /* expression_any: expression_with_alias SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 252: /* expression_any: expression_for_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 253: /* expression_any: expression_break SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 254: /* expression_any: expression_continue SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 255: /* expression_any: expression_return SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 256: /* expression_any: expression_yield SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 257: /* expression_any: expression_if_then_else  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 258: /* expression_any: expression_if_then_else_oneliner  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 259: /* expression_any: expression_try_catch  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 260: /* expression_any: expression_label SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 261: /* expression_any: expression_goto SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 262: /* expression_any: "pass" SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 263: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 264: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 265: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 266: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 267: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 268: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 269: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 270: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 271: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 272: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 273: /* name_in_namespace: "name" "::" "name"  */
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

  case 274: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 275: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 276: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 277: /* $@14: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 278: /* $@15: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 279: /* new_type_declaration: '<' $@14 type_declaration '>' $@15  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 280: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 281: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 282: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 283: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 284: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 285: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 286: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 287: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 288: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 289: /* expression_return: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 290: /* expression_return: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 291: /* expression_return: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 292: /* expression_yield: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 293: /* expression_yield: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 294: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 295: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 296: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 297: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 298: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 299: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 300: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 301: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 302: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 303: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 304: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 305: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr SEMICOLON  */
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

  case 306: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 307: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 308: /* expression_let: kwd_let optional_in_scope '{' variable_declaration_list '}'  */
                                                                               {
        (yyval.pExpression) = ast_LetList(scanner,(yyvsp[-4].b),(yyvsp[-3].b),*(yyvsp[-1].pVarDeclList),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 309: /* $@16: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 310: /* $@17: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 311: /* expr_cast: "cast" '<' $@16 type_declaration_no_options '>' $@17 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 312: /* $@18: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 313: /* $@19: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 314: /* expr_cast: "upcast" '<' $@18 type_declaration_no_options '>' $@19 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 315: /* $@20: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 316: /* $@21: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 317: /* expr_cast: "reinterpret" '<' $@20 type_declaration_no_options '>' $@21 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 318: /* $@22: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 319: /* $@23: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 320: /* expr_type_decl: "type" '<' $@22 type_declaration '>' $@23  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 321: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
                                                                          {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),ptd->typeexpr);
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)));
            }
            delete (yyvsp[-3].s);
    }
    break;

  case 322: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
                                                                                                {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),ptd->typeexpr,*(yyvsp[-4].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-4].s));
            }
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 323: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" c_or_s "name" '>' '(' expr ')'  */
                                                                                                                        {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),ptd->typeexpr,*(yyvsp[-6].s),*(yyvsp[-4].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            }
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 324: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 325: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 326: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 327: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 328: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 329: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 330: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 331: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 332: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 333: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 334: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 335: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 336: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 337: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 338: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 339: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 340: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 341: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 342: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 343: /* expr_full_block_assumed_piped: '{' expressions '}'  */
                                   {
        (yyval.pExpression) = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,new TypeDecl(Type::autoinfer),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 344: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 345: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 346: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 347: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 348: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 349: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 350: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 351: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 352: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 353: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 354: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 355: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 356: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 357: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 358: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 359: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 360: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 362: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 363: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 364: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 365: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 367: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 369: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 371: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 372: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 373: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 374: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 375: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 376: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 377: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 378: /* $@25: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 379: /* func_addr_expr: '@' '@' '<' $@24 type_declaration_no_options '>' $@25 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 380: /* $@26: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 381: /* $@27: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 382: /* func_addr_expr: '@' '@' '<' $@26 optional_function_argument_list optional_function_type '>' $@27 func_addr_name  */
                                                                                                                                                                                     {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        if ( (yyvsp[-4].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, expr->funcType.get(), (yyvsp[-4].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-4].pVarDeclList));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 383: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 384: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 385: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 386: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 387: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                       {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back(ExpressionPtr((yyvsp[-7].pExpression)));
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 388: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 389: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 390: /* $@28: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 391: /* $@29: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 392: /* expr_field: expr '.' $@28 error $@29  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 393: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 394: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
                                                          {
            auto dd = new ExprMakeStruct(tokAt(scanner,(yylsp[-3])));
            dd->at = tokAt(scanner,(yylsp[-3]));
            dd->makeType = new TypeDecl(Type::alias);
            dd->makeType->alias = *(yyvsp[-3].s);
            dd->useInitializer = false;
            dd->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = dd;
    }
    break;

  case 395: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = new TypeDecl(Type::alias);
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType->alias = *(yyvsp[-3].s);
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 396: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = new TypeDecl(Type::alias);
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType->alias = *(yyvsp[-4].s);
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 397: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 398: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 399: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 400: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 401: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 402: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 403: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 404: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 405: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 406: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 407: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 408: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 409: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 410: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 411: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 412: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 413: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 414: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 415: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 416: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 417: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 418: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 419: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 420: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 421: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 422: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 423: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 424: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 425: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 426: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 427: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 428: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 429: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 430: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 431: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 432: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 433: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 434: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 435: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 436: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 437: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 438: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 439: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 440: /* expr: '(' expr_list optional_comma ')'  */
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

  case 441: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 442: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 443: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 444: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 445: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 446: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 447: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 448: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 449: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 450: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 451: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 452: /* expr: expr_generator  */
                                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 453: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 454: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 455: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 456: /* $@31: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 457: /* expr: expr "is" "type" '<' $@30 type_declaration_no_options '>' $@31  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 458: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 459: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 460: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 461: /* $@32: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 462: /* $@33: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 463: /* expr: expr "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 464: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 465: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 466: /* $@34: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 467: /* $@35: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 468: /* expr: expr '?' "as" "type" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 469: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 470: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 471: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 472: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 473: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 474: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 475: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 476: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 477: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 478: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 479: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 480: /* expr: expr_call_pipe  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 481: /* expr: "unsafe" '(' expr ')'  */
                                         {
            (yyvsp[-1].pExpression)->alwaysSafe = true;
            (yyvsp[-1].pExpression)->userSaidItsSafe = true;
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    break;

  case 482: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 483: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 484: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list optional_emit_semis expression_block  */
                                                                                                                                                  {
        auto closure = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        ((ExprBlock *)(yyvsp[0].pExpression))->returnType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),closure,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 485: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 486: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 487: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 488: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 489: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 490: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 491: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 492: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 493: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 494: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 495: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 496: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 497: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 498: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 499: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 500: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 501: /* optional_field_annotation: %empty  */
                                      { (yyval.aaList) = nullptr; }
    break;

  case 502: /* optional_field_annotation: metadata_argument_list  */
                                      { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 503: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 504: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 505: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 506: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 507: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 508: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 509: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 510: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 511: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 512: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 513: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 514: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 515: /* struct_variable_declaration_list: struct_variable_declaration_list "new line, semicolon"  */
                                                                 { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 516: /* $@36: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 517: /* struct_variable_declaration_list: struct_variable_declaration_list $@36 structure_variable_declaration SEMICOLON  */
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

  case 518: /* $@37: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 519: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable "abstract" optional_constant $@37 function_declaration_header SEMICOLON  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 520: /* $@38: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 521: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list_with_emit_semis "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@38 function_declaration_header optional_emit_semis expression_block  */
                                                                                            {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-10].pVarDeclList),(yyvsp[-9].faList),(yyvsp[-6].b),(yyvsp[-7].b),(yyvsp[-5].i),(yyvsp[-4].b),(yyvsp[-2].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-8]),(yylsp[0])),tokAt(scanner,(yylsp[-9])));
            }
    break;

  case 522: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 523: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 524: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 525: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 526: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 527: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 528: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 529: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 530: /* tuple_alias_type_list: %empty  */
      {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 531: /* tuple_alias_type_list: tuple_type  */
                       {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 532: /* tuple_alias_type_list: tuple_alias_type_list semis tuple_type  */
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

  case 533: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 534: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 535: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 536: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 537: /* variant_alias_type_list: variant_type  */
                         {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 538: /* variant_alias_type_list: variant_alias_type_list semis variant_type  */
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

  case 539: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 540: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 541: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 542: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 543: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 544: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 545: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 546: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 547: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 548: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 549: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 550: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 551: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 552: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 553: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 554: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 555: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 556: /* global_let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 557: /* global_let_variable_name_with_pos_list: global_let_variable_name_with_pos_list ',' "name"  */
                                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 558: /* variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 559: /* variable_declaration_list: variable_declaration_list SEMICOLON  */
                                                  {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 560: /* variable_declaration_list: variable_declaration_list let_variable_declaration  */
                                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 561: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 562: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 563: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 564: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                         {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 565: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                               {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 566: /* global_let_variable_declaration: global_let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 567: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 568: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 569: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 570: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 571: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 572: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 573: /* global_variable_declaration_list: global_variable_declaration_list SEMICOLON  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 574: /* $@39: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 575: /* global_variable_declaration_list: global_variable_declaration_list $@39 optional_field_annotation let_variable_declaration  */
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

  case 576: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 577: /* $@40: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 578: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@40 optional_field_annotation global_let_variable_declaration  */
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

  case 579: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        delete (yyvsp[0].s);
    }
    break;

  case 580: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
        delete (yyvsp[-2].s);
    }
    break;

  case 583: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 584: /* enum_list: enum_expression  */
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

  case 585: /* enum_list: enum_list commas enum_expression  */
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

  case 586: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 587: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 588: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 589: /* $@41: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 590: /* single_alias: optional_public_or_private_alias "name" $@41 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->isPrivateAlias = !(yyvsp[-4].b);
        if ( (yyvsp[0].pTypeDecl)->baseType == Type::alias ) {
            das2_yyerror(scanner,"alias cannot be defined in terms of another alias "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::invalid_type);
        }
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !yyextra->g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
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

  case 592: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 593: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 594: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 595: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 596: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 597: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 604: /* $@42: %empty  */
                                                     {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 605: /* $@43: %empty  */
                                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 606: /* $@44: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 607: /* enum_declaration: optional_annotation_list "enum" $@42 optional_public_or_private_enum enum_name optional_enum_basic_type_declaration optional_emit_commas '{' $@43 enum_list optional_commas $@44 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-8].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-12].faList),tokAt(scanner,(yylsp[-12])),(yyvsp[-9].b),(yyvsp[-8].pEnum),(yyvsp[-3].pEnum),(yyvsp[-7].type));
    }
    break;

  case 608: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 609: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 610: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 611: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 612: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 613: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 614: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 615: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 616: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 617: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 618: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 619: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 620: /* optional_struct_variable_declaration_list: ';'  */
            {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 621: /* optional_struct_variable_declaration_list: '{' struct_variable_declaration_list '}'  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 622: /* $@45: %empty  */
                                                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 623: /* $@46: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 624: /* $@47: %empty  */
                                             {
        if ( (yyvsp[-1].pStructure) ) {
            (yyvsp[-1].pStructure)->isClass = (yyvsp[-4].i)==CorS_Class || (yyvsp[-4].i)==CorS_ClassTemplate;
            (yyvsp[-1].pStructure)->isTemplate = (yyvsp[-4].i)==CorS_ClassTemplate || (yyvsp[-4].i)==CorS_StructTemplate;
            (yyvsp[-1].pStructure)->privateStructure = !(yyvsp[-3].b);
        }
    }
    break;

  case 625: /* structure_declaration: optional_annotation_list_with_emit_semis $@45 class_or_struct optional_public_or_private_structure $@46 structure_name optional_emit_semis $@47 optional_struct_variable_declaration_list  */
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

  case 626: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 627: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 628: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 629: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 630: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 631: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 632: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 633: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 634: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 635: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 636: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 637: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 638: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 639: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 640: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 641: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 642: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 643: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 644: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 645: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 646: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 647: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 648: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 649: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 650: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 651: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 652: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 653: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 654: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 655: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 656: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 657: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 658: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 659: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 660: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 661: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 662: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 663: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 664: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 665: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 666: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 667: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 668: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 669: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 670: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 671: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 672: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 673: /* bitfield_alias_bits: "name"  */
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

  case 674: /* bitfield_alias_bits: bitfield_alias_bits commas "name"  */
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

  case 675: /* $@48: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 676: /* $@49: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 677: /* bitfield_type_declaration: "bitfield" '<' $@48 bitfield_bits '>' $@49  */
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

  case 680: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 681: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 682: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 683: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 684: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 685: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 686: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 687: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
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

  case 688: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 689: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 690: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 691: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 692: /* $@50: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 693: /* $@51: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 694: /* type_declaration_no_options_no_dim: "type" '<' $@50 type_declaration '>' $@51  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 695: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 696: /* type_declaration_no_options_no_dim: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 697: /* $@52: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 698: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@52 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 699: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '[' ']'  */
                                                                 {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 700: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "explicit"  */
                                                                  {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 701: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "const"  */
                                                               {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 702: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' "const"  */
                                                                   {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 703: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '&'  */
                                                         {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 704: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '&'  */
                                                             {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 705: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '#'  */
                                                         {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 706: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "implicit"  */
                                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 707: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '#'  */
                                                             {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 708: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "==" "const"  */
                                                                      {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 709: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "==" '&'  */
                                                                {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 710: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '?'  */
                                                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 711: /* $@53: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 712: /* $@54: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 713: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@53 type_declaration '>' $@54  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 714: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "??"  */
                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 715: /* $@55: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 716: /* $@56: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 717: /* type_declaration_no_options_no_dim: "array" '<' $@55 type_declaration '>' $@56  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 718: /* $@57: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 719: /* $@58: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 720: /* type_declaration_no_options_no_dim: "table" '<' $@57 table_type_pair '>' $@58  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 721: /* $@59: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 722: /* $@60: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 723: /* type_declaration_no_options_no_dim: "iterator" '<' $@59 type_declaration '>' $@60  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 724: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 725: /* $@61: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 726: /* $@62: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 727: /* type_declaration_no_options_no_dim: "block" '<' $@61 type_declaration '>' $@62  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 728: /* $@63: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 729: /* $@64: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 730: /* type_declaration_no_options_no_dim: "block" '<' $@63 optional_function_argument_list optional_function_type '>' $@64  */
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 731: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 732: /* $@65: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 733: /* $@66: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 734: /* type_declaration_no_options_no_dim: "function" '<' $@65 type_declaration '>' $@66  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 735: /* $@67: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 736: /* $@68: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 737: /* type_declaration_no_options_no_dim: "function" '<' $@67 optional_function_argument_list optional_function_type '>' $@68  */
                                                                                                                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 738: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 739: /* $@69: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 740: /* $@70: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 741: /* type_declaration_no_options_no_dim: "lambda" '<' $@69 type_declaration '>' $@70  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 742: /* $@71: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 743: /* $@72: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 744: /* type_declaration_no_options_no_dim: "lambda" '<' $@71 optional_function_argument_list optional_function_type '>' $@72  */
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 745: /* $@73: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 746: /* $@74: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 747: /* type_declaration_no_options_no_dim: "tuple" '<' $@73 tuple_type_list '>' $@74  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 748: /* $@75: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 749: /* $@76: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 750: /* type_declaration_no_options_no_dim: "variant" '<' $@75 variant_type_list '>' $@76  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 751: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 752: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 753: /* type_declaration: type_declaration '|' '#'  */
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

  case 754: /* $@77: %empty  */
                   {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 755: /* $@78: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 756: /* $@79: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 757: /* $@80: %empty  */
                                                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 758: /* tuple_alias_declaration: "tuple" $@77 optional_public_or_private_alias "name" optional_emit_semis $@78 '{' $@79 tuple_alias_type_list optional_semis $@80 '}'  */
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

  case 759: /* $@81: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 760: /* $@82: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 761: /* $@83: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 762: /* $@84: %empty  */
                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 763: /* variant_alias_declaration: "variant" $@81 optional_public_or_private_alias "name" optional_emit_semis $@82 '{' $@83 variant_alias_type_list optional_semis $@84 '}'  */
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

  case 764: /* $@85: %empty  */
                      {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 765: /* $@86: %empty  */
                                                                              {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 766: /* $@87: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 767: /* $@88: %empty  */
                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 768: /* bitfield_alias_declaration: "bitfield" $@85 optional_public_or_private_alias "name" optional_emit_commas $@86 '{' $@87 bitfield_alias_bits optional_commas $@88 '}'  */
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

  case 769: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 770: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 771: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 772: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 773: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 774: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 775: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 776: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 777: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 778: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 779: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 780: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 781: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 782: /* make_variant_dim: make_struct_fields  */
                                {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 783: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 784: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 785: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 786: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 787: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 788: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 789: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 790: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 791: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 792: /* $@89: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 793: /* $@90: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 794: /* make_struct_decl: "struct" '<' $@89 type_declaration_no_options '>' $@90 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 795: /* $@91: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 796: /* $@92: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 797: /* make_struct_decl: "class" '<' $@91 type_declaration_no_options '>' $@92 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 798: /* $@93: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 799: /* $@94: %empty  */
                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 800: /* make_struct_decl: "variant" '<' $@93 type_declaration_no_options '>' $@94 '(' make_variant_dim ')'  */
                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 801: /* $@95: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 802: /* $@96: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 803: /* make_struct_decl: "default" '<' $@95 type_declaration_no_options '>' $@96 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 804: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 805: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 806: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 807: /* $@97: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 808: /* $@98: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 809: /* make_tuple_call: "tuple" '<' $@97 type_declaration_no_options '>' $@98 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 810: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 811: /* $@99: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 812: /* $@100: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 813: /* make_dim_decl: "array" "struct" '<' $@99 type_declaration_no_options '>' $@100 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-10])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 814: /* $@101: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 815: /* $@102: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 816: /* make_dim_decl: "array" "tuple" '<' $@101 type_declaration_no_options '>' $@102 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-10])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 817: /* $@103: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 818: /* $@104: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 819: /* make_dim_decl: "array" "variant" '<' $@103 type_declaration_no_options '>' $@104 '(' make_variant_dim ')'  */
                                                                                                                                                                               {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 820: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 821: /* $@105: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 822: /* $@106: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 823: /* make_dim_decl: "array" '<' $@105 type_declaration_no_options '>' $@106 '(' optional_expr_list ')'  */
                                                                                                                                                                        {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
            mka->gen2 = true;
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->makeType = make_smart<TypeDecl>(Type::tArray);
            msd->makeType->firstType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
            msd->at = tokAt(scanner,(yylsp[-5]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 824: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 825: /* $@107: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 826: /* $@108: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 827: /* make_dim_decl: "fixed_array" '<' $@107 type_declaration_no_options '>' $@108 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 828: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 829: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 830: /* make_table_decl: '{' expr_map_tuple_list optional_comma '}'  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 831: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 832: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                 {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-6])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->makeType = make_smart<TypeDecl>(Type::tTable);
            msd->makeType->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
            msd->makeType->secondType = make_smart<TypeDecl>(Type::tVoid);
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 833: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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
            msd->makeType->firstType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
            msd->makeType->secondType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 834: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 835: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 836: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 837: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 838: /* array_comprehension: '[' "for" '(' variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                           {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 839: /* array_comprehension: '[' "iterator" "for" '(' variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                                        {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 840: /* array_comprehension: '{' "for" '(' variable_name_with_pos_list "in" expr_list ')' ';' make_map_tuple array_comprehension_where '}'  */
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


