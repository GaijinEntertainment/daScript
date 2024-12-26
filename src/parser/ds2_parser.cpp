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
  YYSYMBOL_ADDEQU = 107,                   /* "+="  */
  YYSYMBOL_SUBEQU = 108,                   /* "-="  */
  YYSYMBOL_DIVEQU = 109,                   /* "/="  */
  YYSYMBOL_MULEQU = 110,                   /* "*="  */
  YYSYMBOL_MODEQU = 111,                   /* "%="  */
  YYSYMBOL_ANDEQU = 112,                   /* "&="  */
  YYSYMBOL_OREQU = 113,                    /* "|="  */
  YYSYMBOL_XOREQU = 114,                   /* "^="  */
  YYSYMBOL_SHL = 115,                      /* "<<"  */
  YYSYMBOL_SHR = 116,                      /* ">>"  */
  YYSYMBOL_ADDADD = 117,                   /* "++"  */
  YYSYMBOL_SUBSUB = 118,                   /* "--"  */
  YYSYMBOL_LEEQU = 119,                    /* "<="  */
  YYSYMBOL_SHLEQU = 120,                   /* "<<="  */
  YYSYMBOL_SHREQU = 121,                   /* ">>="  */
  YYSYMBOL_GREQU = 122,                    /* ">="  */
  YYSYMBOL_EQUEQU = 123,                   /* "=="  */
  YYSYMBOL_NOTEQU = 124,                   /* "!="  */
  YYSYMBOL_RARROW = 125,                   /* "->"  */
  YYSYMBOL_LARROW = 126,                   /* "<-"  */
  YYSYMBOL_QQ = 127,                       /* "??"  */
  YYSYMBOL_QDOT = 128,                     /* "?."  */
  YYSYMBOL_QBRA = 129,                     /* "?["  */
  YYSYMBOL_LPIPE = 130,                    /* "<|"  */
  YYSYMBOL_RPIPE = 131,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 132,                 /* ":="  */
  YYSYMBOL_ROTL = 133,                     /* "<<<"  */
  YYSYMBOL_ROTR = 134,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 135,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 136,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 137,                    /* "=>"  */
  YYSYMBOL_COLCOL = 138,                   /* "::"  */
  YYSYMBOL_ANDAND = 139,                   /* "&&"  */
  YYSYMBOL_OROR = 140,                     /* "||"  */
  YYSYMBOL_XORXOR = 141,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 142,                /* "&&="  */
  YYSYMBOL_OROREQU = 143,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 144,                /* "^^="  */
  YYSYMBOL_DOTDOT = 145,                   /* ".."  */
  YYSYMBOL_MTAG_E = 146,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 147,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 148,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 149,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 150,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 151,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 152,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 153,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 154,           /* "..."  */
  YYSYMBOL_INTEGER = 155,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 156,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 157,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 158,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 159,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 160,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 161,                   /* "double constant"  */
  YYSYMBOL_NAME = 162,                     /* "name"  */
  YYSYMBOL_DAS_EMIT_COMMA = 163,           /* "emitted ,"  */
  YYSYMBOL_DAS_EMIT_SEMICOLON = 164,       /* "emitted ;"  */
  YYSYMBOL_BEGIN_STRING = 165,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 166,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 167,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 168,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 169,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 170,          /* "}"  */
  YYSYMBOL_END_OF_READ = 171,              /* "end of failed eader macro"  */
  YYSYMBOL_172_ = 172,                     /* ','  */
  YYSYMBOL_173_ = 173,                     /* '='  */
  YYSYMBOL_174_ = 174,                     /* '?'  */
  YYSYMBOL_175_ = 175,                     /* ':'  */
  YYSYMBOL_176_ = 176,                     /* '|'  */
  YYSYMBOL_177_ = 177,                     /* '^'  */
  YYSYMBOL_178_ = 178,                     /* '&'  */
  YYSYMBOL_179_ = 179,                     /* '<'  */
  YYSYMBOL_180_ = 180,                     /* '>'  */
  YYSYMBOL_181_ = 181,                     /* '-'  */
  YYSYMBOL_182_ = 182,                     /* '+'  */
  YYSYMBOL_183_ = 183,                     /* '*'  */
  YYSYMBOL_184_ = 184,                     /* '/'  */
  YYSYMBOL_185_ = 185,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 186,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 187,               /* UNARY_PLUS  */
  YYSYMBOL_188_ = 188,                     /* '~'  */
  YYSYMBOL_189_ = 189,                     /* '!'  */
  YYSYMBOL_PRE_INC = 190,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 191,                  /* PRE_DEC  */
  YYSYMBOL_LLPIPE = 192,                   /* LLPIPE  */
  YYSYMBOL_POST_INC = 193,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 194,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 195,                    /* DEREF  */
  YYSYMBOL_196_ = 196,                     /* '.'  */
  YYSYMBOL_197_ = 197,                     /* '['  */
  YYSYMBOL_198_ = 198,                     /* ']'  */
  YYSYMBOL_199_ = 199,                     /* '('  */
  YYSYMBOL_200_ = 200,                     /* ')'  */
  YYSYMBOL_201_ = 201,                     /* '$'  */
  YYSYMBOL_202_ = 202,                     /* '@'  */
  YYSYMBOL_203_ = 203,                     /* ';'  */
  YYSYMBOL_204_ = 204,                     /* '{'  */
  YYSYMBOL_205_ = 205,                     /* '}'  */
  YYSYMBOL_206_ = 206,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 207,                 /* $accept  */
  YYSYMBOL_program = 208,                  /* program  */
  YYSYMBOL_COMMA = 209,                    /* COMMA  */
  YYSYMBOL_SEMICOLON = 210,                /* SEMICOLON  */
  YYSYMBOL_top_level_reader_macro = 211,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 212, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 213,              /* module_name  */
  YYSYMBOL_module_declaration = 214,       /* module_declaration  */
  YYSYMBOL_character_sequence = 215,       /* character_sequence  */
  YYSYMBOL_string_constant = 216,          /* string_constant  */
  YYSYMBOL_string_builder_body = 217,      /* string_builder_body  */
  YYSYMBOL_string_builder = 218,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 219, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 220,              /* expr_reader  */
  YYSYMBOL_221_1 = 221,                    /* $@1  */
  YYSYMBOL_options_declaration = 222,      /* options_declaration  */
  YYSYMBOL_require_declaration = 223,      /* require_declaration  */
  YYSYMBOL_require_module_name = 224,      /* require_module_name  */
  YYSYMBOL_require_module = 225,           /* require_module  */
  YYSYMBOL_is_public_module = 226,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 227,       /* expect_declaration  */
  YYSYMBOL_expect_list = 228,              /* expect_list  */
  YYSYMBOL_expect_error = 229,             /* expect_error  */
  YYSYMBOL_expression_label = 230,         /* expression_label  */
  YYSYMBOL_expression_goto = 231,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 232,      /* elif_or_static_elif  */
  YYSYMBOL_emit_semis = 233,               /* emit_semis  */
  YYSYMBOL_optional_emit_semis = 234,      /* optional_emit_semis  */
  YYSYMBOL_expression_else = 235,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 236,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 237, /* expression_else_one_liner  */
  YYSYMBOL_expression_if_one_liner = 238,  /* expression_if_one_liner  */
  YYSYMBOL_semis = 239,                    /* semis  */
  YYSYMBOL_optional_semis = 240,           /* optional_semis  */
  YYSYMBOL_expression_if_then_else = 241,  /* expression_if_then_else  */
  YYSYMBOL_242_2 = 242,                    /* $@2  */
  YYSYMBOL_expression_if_then_else_oneliner = 243, /* expression_if_then_else_oneliner  */
  YYSYMBOL_expression_for_loop = 244,      /* expression_for_loop  */
  YYSYMBOL_245_3 = 245,                    /* $@3  */
  YYSYMBOL_expression_unsafe = 246,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 247,    /* expression_while_loop  */
  YYSYMBOL_248_4 = 248,                    /* $@4  */
  YYSYMBOL_expression_with = 249,          /* expression_with  */
  YYSYMBOL_250_5 = 250,                    /* $@5  */
  YYSYMBOL_expression_with_alias = 251,    /* expression_with_alias  */
  YYSYMBOL_annotation_argument_value = 252, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 253, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 254, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 255,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 256, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 257,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 258, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 259, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 260,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 261,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 262, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 263, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 264,   /* optional_function_type  */
  YYSYMBOL_function_name = 265,            /* function_name  */
  YYSYMBOL_global_function_declaration = 266, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 267, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 268, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 269,     /* function_declaration  */
  YYSYMBOL_270_6 = 270,                    /* $@6  */
  YYSYMBOL_expression_block_finally = 271, /* expression_block_finally  */
  YYSYMBOL_272_7 = 272,                    /* $@7  */
  YYSYMBOL_273_8 = 273,                    /* $@8  */
  YYSYMBOL_expression_block = 274,         /* expression_block  */
  YYSYMBOL_275_9 = 275,                    /* $@9  */
  YYSYMBOL_276_10 = 276,                   /* $@10  */
  YYSYMBOL_expr_call_pipe = 277,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 278,           /* expression_any  */
  YYSYMBOL_expressions = 279,              /* expressions  */
  YYSYMBOL_optional_expr_list = 280,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_map_tuple_list = 281, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 282, /* type_declaration_no_options_list  */
  YYSYMBOL_name_in_namespace = 283,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 284,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 285,     /* new_type_declaration  */
  YYSYMBOL_286_11 = 286,                   /* $@11  */
  YYSYMBOL_287_12 = 287,                   /* $@12  */
  YYSYMBOL_expr_new = 288,                 /* expr_new  */
  YYSYMBOL_expression_break = 289,         /* expression_break  */
  YYSYMBOL_expression_continue = 290,      /* expression_continue  */
  YYSYMBOL_expression_return = 291,        /* expression_return  */
  YYSYMBOL_expression_yield = 292,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 293,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 294,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 295,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 296,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 297,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 298, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 299,           /* expression_let  */
  YYSYMBOL_expr_cast = 300,                /* expr_cast  */
  YYSYMBOL_301_13 = 301,                   /* $@13  */
  YYSYMBOL_302_14 = 302,                   /* $@14  */
  YYSYMBOL_303_15 = 303,                   /* $@15  */
  YYSYMBOL_304_16 = 304,                   /* $@16  */
  YYSYMBOL_305_17 = 305,                   /* $@17  */
  YYSYMBOL_306_18 = 306,                   /* $@18  */
  YYSYMBOL_expr_type_decl = 307,           /* expr_type_decl  */
  YYSYMBOL_308_19 = 308,                   /* $@19  */
  YYSYMBOL_309_20 = 309,                   /* $@20  */
  YYSYMBOL_expr_type_info = 310,           /* expr_type_info  */
  YYSYMBOL_expr_list = 311,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 312,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 313,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 314,            /* capture_entry  */
  YYSYMBOL_capture_list = 315,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 316,    /* optional_capture_list  */
  YYSYMBOL_expr_full_block = 317,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 318, /* expr_full_block_assumed_piped  */
  YYSYMBOL_expr_numeric_const = 319,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 320,              /* expr_assign  */
  YYSYMBOL_expr_named_call = 321,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 322,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 323,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 324,           /* func_addr_expr  */
  YYSYMBOL_325_21 = 325,                   /* $@21  */
  YYSYMBOL_326_22 = 326,                   /* $@22  */
  YYSYMBOL_327_23 = 327,                   /* $@23  */
  YYSYMBOL_328_24 = 328,                   /* $@24  */
  YYSYMBOL_expr_field = 329,               /* expr_field  */
  YYSYMBOL_330_25 = 330,                   /* $@25  */
  YYSYMBOL_331_26 = 331,                   /* $@26  */
  YYSYMBOL_expr_call = 332,                /* expr_call  */
  YYSYMBOL_expr = 333,                     /* expr  */
  YYSYMBOL_334_27 = 334,                   /* $@27  */
  YYSYMBOL_335_28 = 335,                   /* $@28  */
  YYSYMBOL_336_29 = 336,                   /* $@29  */
  YYSYMBOL_337_30 = 337,                   /* $@30  */
  YYSYMBOL_338_31 = 338,                   /* $@31  */
  YYSYMBOL_339_32 = 339,                   /* $@32  */
  YYSYMBOL_expr_generator = 340,           /* expr_generator  */
  YYSYMBOL_expr_mtag = 341,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 342, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 343,        /* optional_override  */
  YYSYMBOL_optional_constant = 344,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 345, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 346, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 347, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 348, /* struct_variable_declaration_list  */
  YYSYMBOL_349_33 = 349,                   /* $@33  */
  YYSYMBOL_350_34 = 350,                   /* $@34  */
  YYSYMBOL_351_35 = 351,                   /* $@35  */
  YYSYMBOL_function_argument_declaration = 352, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 353,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 354,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 355,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 356,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 357,             /* variant_type  */
  YYSYMBOL_variant_type_list = 358,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 359,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 360,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 361,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 362,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 363,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 364, /* let_variable_name_with_pos_list  */
  YYSYMBOL_global_let_variable_name_with_pos_list = 365, /* global_let_variable_name_with_pos_list  */
  YYSYMBOL_variable_declaration_list = 366, /* variable_declaration_list  */
  YYSYMBOL_let_variable_declaration = 367, /* let_variable_declaration  */
  YYSYMBOL_global_let_variable_declaration = 368, /* global_let_variable_declaration  */
  YYSYMBOL_optional_shared = 369,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 370, /* optional_public_or_private_variable  */
  YYSYMBOL_global_variable_declaration_list = 371, /* global_variable_declaration_list  */
  YYSYMBOL_372_36 = 372,                   /* $@36  */
  YYSYMBOL_global_let = 373,               /* global_let  */
  YYSYMBOL_374_37 = 374,                   /* $@37  */
  YYSYMBOL_enum_expression = 375,          /* enum_expression  */
  YYSYMBOL_commas = 376,                   /* commas  */
  YYSYMBOL_enum_list = 377,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 378, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 379,             /* single_alias  */
  YYSYMBOL_380_38 = 380,                   /* $@38  */
  YYSYMBOL_alias_declaration = 381,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 382, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 383,                /* enum_name  */
  YYSYMBOL_optional_enum_basic_type_declaration = 384, /* optional_enum_basic_type_declaration  */
  YYSYMBOL_optional_commas = 385,          /* optional_commas  */
  YYSYMBOL_emit_commas = 386,              /* emit_commas  */
  YYSYMBOL_optional_emit_commas = 387,     /* optional_emit_commas  */
  YYSYMBOL_enum_declaration = 388,         /* enum_declaration  */
  YYSYMBOL_389_39 = 389,                   /* $@39  */
  YYSYMBOL_390_40 = 390,                   /* $@40  */
  YYSYMBOL_391_41 = 391,                   /* $@41  */
  YYSYMBOL_optional_structure_parent = 392, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 393,          /* optional_sealed  */
  YYSYMBOL_structure_name = 394,           /* structure_name  */
  YYSYMBOL_class_or_struct = 395,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 396, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 397, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 398,    /* structure_declaration  */
  YYSYMBOL_399_42 = 399,                   /* $@42  */
  YYSYMBOL_400_43 = 400,                   /* $@43  */
  YYSYMBOL_variable_name_with_pos_list = 401, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 402,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 403, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 404, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 405,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 406,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 407,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 408, /* bitfield_type_declaration  */
  YYSYMBOL_409_44 = 409,                   /* $@44  */
  YYSYMBOL_410_45 = 410,                   /* $@45  */
  YYSYMBOL_c_or_s = 411,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 412,          /* table_type_pair  */
  YYSYMBOL_dim_list = 413,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 414, /* type_declaration_no_options  */
  YYSYMBOL_type_declaration_no_options_no_dim = 415, /* type_declaration_no_options_no_dim  */
  YYSYMBOL_416_46 = 416,                   /* $@46  */
  YYSYMBOL_417_47 = 417,                   /* $@47  */
  YYSYMBOL_418_48 = 418,                   /* $@48  */
  YYSYMBOL_419_49 = 419,                   /* $@49  */
  YYSYMBOL_420_50 = 420,                   /* $@50  */
  YYSYMBOL_421_51 = 421,                   /* $@51  */
  YYSYMBOL_422_52 = 422,                   /* $@52  */
  YYSYMBOL_423_53 = 423,                   /* $@53  */
  YYSYMBOL_424_54 = 424,                   /* $@54  */
  YYSYMBOL_425_55 = 425,                   /* $@55  */
  YYSYMBOL_426_56 = 426,                   /* $@56  */
  YYSYMBOL_427_57 = 427,                   /* $@57  */
  YYSYMBOL_428_58 = 428,                   /* $@58  */
  YYSYMBOL_429_59 = 429,                   /* $@59  */
  YYSYMBOL_430_60 = 430,                   /* $@60  */
  YYSYMBOL_431_61 = 431,                   /* $@61  */
  YYSYMBOL_432_62 = 432,                   /* $@62  */
  YYSYMBOL_433_63 = 433,                   /* $@63  */
  YYSYMBOL_434_64 = 434,                   /* $@64  */
  YYSYMBOL_435_65 = 435,                   /* $@65  */
  YYSYMBOL_436_66 = 436,                   /* $@66  */
  YYSYMBOL_437_67 = 437,                   /* $@67  */
  YYSYMBOL_438_68 = 438,                   /* $@68  */
  YYSYMBOL_439_69 = 439,                   /* $@69  */
  YYSYMBOL_440_70 = 440,                   /* $@70  */
  YYSYMBOL_441_71 = 441,                   /* $@71  */
  YYSYMBOL_442_72 = 442,                   /* $@72  */
  YYSYMBOL_type_declaration = 443,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 444,  /* tuple_alias_declaration  */
  YYSYMBOL_445_73 = 445,                   /* $@73  */
  YYSYMBOL_446_74 = 446,                   /* $@74  */
  YYSYMBOL_447_75 = 447,                   /* $@75  */
  YYSYMBOL_448_76 = 448,                   /* $@76  */
  YYSYMBOL_variant_alias_declaration = 449, /* variant_alias_declaration  */
  YYSYMBOL_450_77 = 450,                   /* $@77  */
  YYSYMBOL_451_78 = 451,                   /* $@78  */
  YYSYMBOL_452_79 = 452,                   /* $@79  */
  YYSYMBOL_453_80 = 453,                   /* $@80  */
  YYSYMBOL_bitfield_alias_declaration = 454, /* bitfield_alias_declaration  */
  YYSYMBOL_455_81 = 455,                   /* $@81  */
  YYSYMBOL_456_82 = 456,                   /* $@82  */
  YYSYMBOL_457_83 = 457,                   /* $@83  */
  YYSYMBOL_458_84 = 458,                   /* $@84  */
  YYSYMBOL_make_decl = 459,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 460,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 461,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 462,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim_list = 463,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 464,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 465, /* optional_make_struct_dim_decl  */
  YYSYMBOL_use_initializer = 466,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 467,         /* make_struct_decl  */
  YYSYMBOL_468_85 = 468,                   /* $@85  */
  YYSYMBOL_469_86 = 469,                   /* $@86  */
  YYSYMBOL_470_87 = 470,                   /* $@87  */
  YYSYMBOL_471_88 = 471,                   /* $@88  */
  YYSYMBOL_472_89 = 472,                   /* $@89  */
  YYSYMBOL_473_90 = 473,                   /* $@90  */
  YYSYMBOL_474_91 = 474,                   /* $@91  */
  YYSYMBOL_475_92 = 475,                   /* $@92  */
  YYSYMBOL_make_map_tuple = 476,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 477,          /* make_tuple_call  */
  YYSYMBOL_478_93 = 478,                   /* $@93  */
  YYSYMBOL_479_94 = 479,                   /* $@94  */
  YYSYMBOL_make_dim_decl = 480,            /* make_dim_decl  */
  YYSYMBOL_481_95 = 481,                   /* $@95  */
  YYSYMBOL_482_96 = 482,                   /* $@96  */
  YYSYMBOL_483_97 = 483,                   /* $@97  */
  YYSYMBOL_484_98 = 484,                   /* $@98  */
  YYSYMBOL_485_99 = 485,                   /* $@99  */
  YYSYMBOL_486_100 = 486,                  /* $@100  */
  YYSYMBOL_487_101 = 487,                  /* $@101  */
  YYSYMBOL_488_102 = 488,                  /* $@102  */
  YYSYMBOL_489_103 = 489,                  /* $@103  */
  YYSYMBOL_490_104 = 490,                  /* $@104  */
  YYSYMBOL_expr_map_tuple_list = 491,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 492,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 493, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 494,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 495       /* array_comprehension  */
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
#define YYLAST   11595

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  207
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  289
/* YYNRULES -- Number of rules.  */
#define YYNRULES  828
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1502

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   434


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
       2,     2,     2,   189,     2,   206,   201,   185,   178,     2,
     199,   200,   183,   182,   172,   181,   196,   184,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   175,   203,
     179,   173,   180,   174,   202,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   197,     2,   198,   177,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   204,   176,   205,   188,     2,     2,     2,
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
     165,   166,   167,   168,   169,   170,   171,   186,   187,   190,
     191,   192,   193,   194,   195
};

#if DAS2_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   534,   534,   535,   540,   541,   542,   543,   544,   545,
     546,   547,   548,   549,   550,   551,   552,   556,   557,   561,
     562,   566,   572,   573,   574,   578,   579,   583,   601,   602,
     603,   604,   608,   609,   613,   618,   627,   635,   651,   656,
     664,   664,   703,   721,   725,   728,   732,   738,   747,   750,
     756,   757,   761,   765,   766,   770,   773,   779,   785,   788,
     794,   795,   799,   800,   804,   805,   809,   810,   811,   820,
     821,   825,   826,   832,   833,   834,   835,   836,   840,   841,
     845,   846,   850,   850,   862,   869,   869,   878,   886,   886,
     899,   899,   911,   918,   919,   920,   921,   922,   923,   927,
     932,   940,   941,   942,   946,   947,   948,   949,   950,   951,
     952,   953,   959,   962,   968,   971,   977,   978,   979,   983,
     996,  1014,  1017,  1025,  1036,  1047,  1058,  1061,  1068,  1072,
    1079,  1080,  1084,  1085,  1086,  1090,  1093,  1100,  1104,  1105,
    1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,  1115,
    1116,  1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,  1125,
    1126,  1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,
    1136,  1137,  1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,
    1146,  1147,  1148,  1149,  1150,  1151,  1152,  1153,  1154,  1155,
    1156,  1157,  1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,
    1166,  1167,  1168,  1169,  1170,  1171,  1172,  1173,  1174,  1175,
    1176,  1177,  1178,  1179,  1180,  1181,  1182,  1183,  1184,  1185,
    1186,  1187,  1192,  1210,  1211,  1212,  1216,  1222,  1222,  1239,
    1242,  1244,  1242,  1252,  1254,  1252,  1269,  1282,  1283,  1284,
    1285,  1286,  1287,  1288,  1289,  1290,  1291,  1292,  1293,  1294,
    1295,  1296,  1297,  1298,  1299,  1300,  1304,  1309,  1315,  1321,
    1322,  1326,  1327,  1331,  1335,  1342,  1343,  1354,  1358,  1361,
    1369,  1369,  1369,  1372,  1378,  1381,  1385,  1389,  1396,  1403,
    1409,  1413,  1417,  1420,  1423,  1431,  1434,  1442,  1448,  1449,
    1450,  1454,  1455,  1459,  1460,  1464,  1469,  1477,  1483,  1495,
    1498,  1501,  1507,  1507,  1507,  1510,  1510,  1510,  1515,  1515,
    1515,  1523,  1523,  1523,  1529,  1539,  1550,  1565,  1568,  1574,
    1575,  1582,  1593,  1594,  1595,  1599,  1600,  1601,  1602,  1603,
    1607,  1612,  1620,  1621,  1625,  1632,  1636,  1642,  1643,  1644,
    1645,  1646,  1647,  1648,  1652,  1653,  1654,  1655,  1656,  1657,
    1658,  1659,  1660,  1661,  1662,  1663,  1664,  1665,  1666,  1667,
    1668,  1669,  1670,  1674,  1681,  1693,  1698,  1708,  1712,  1719,
    1722,  1722,  1722,  1727,  1727,  1727,  1740,  1744,  1748,  1753,
    1760,  1765,  1772,  1772,  1772,  1779,  1783,  1793,  1802,  1811,
    1815,  1818,  1824,  1825,  1826,  1827,  1828,  1829,  1830,  1831,
    1832,  1833,  1834,  1835,  1836,  1837,  1838,  1839,  1840,  1841,
    1842,  1843,  1844,  1845,  1846,  1847,  1848,  1849,  1850,  1851,
    1852,  1853,  1854,  1855,  1856,  1857,  1858,  1859,  1865,  1866,
    1867,  1868,  1869,  1882,  1883,  1884,  1885,  1886,  1887,  1888,
    1889,  1890,  1891,  1892,  1893,  1894,  1895,  1898,  1898,  1898,
    1901,  1906,  1910,  1914,  1914,  1914,  1919,  1922,  1926,  1926,
    1926,  1931,  1934,  1935,  1936,  1937,  1938,  1939,  1940,  1941,
    1942,  1944,  1948,  1949,  1957,  1960,  1963,  1972,  1973,  1974,
    1975,  1976,  1977,  1978,  1982,  1986,  1990,  1994,  1998,  2002,
    2006,  2010,  2014,  2021,  2022,  2026,  2027,  2028,  2032,  2033,
    2037,  2038,  2039,  2043,  2044,  2048,  2059,  2062,  2062,  2081,
    2080,  2094,  2093,  2109,  2118,  2128,  2129,  2133,  2136,  2145,
    2146,  2150,  2153,  2157,  2171,  2180,  2181,  2185,  2188,  2192,
    2206,  2207,  2211,  2217,  2223,  2226,  2230,  2239,  2240,  2241,
    2245,  2246,  2250,  2257,  2262,  2271,  2277,  2288,  2295,  2304,
    2307,  2310,  2317,  2320,  2325,  2336,  2339,  2344,  2356,  2357,
    2361,  2362,  2363,  2367,  2370,  2373,  2373,  2393,  2396,  2396,
    2414,  2419,  2427,  2428,  2432,  2435,  2448,  2465,  2466,  2467,
    2472,  2472,  2498,  2502,  2503,  2504,  2508,  2518,  2521,  2527,
    2528,  2532,  2533,  2537,  2538,  2542,  2544,  2549,  2542,  2565,
    2566,  2570,  2571,  2575,  2581,  2582,  2586,  2587,  2588,  2592,
    2595,  2601,  2606,  2601,  2620,  2627,  2632,  2641,  2647,  2658,
    2659,  2660,  2661,  2662,  2663,  2664,  2665,  2666,  2667,  2668,
    2669,  2670,  2671,  2672,  2673,  2674,  2675,  2676,  2677,  2678,
    2679,  2680,  2681,  2682,  2683,  2684,  2688,  2689,  2690,  2691,
    2692,  2693,  2694,  2695,  2699,  2710,  2714,  2721,  2733,  2740,
    2749,  2754,  2764,  2777,  2777,  2777,  2790,  2791,  2795,  2799,
    2806,  2810,  2814,  2818,  2825,  2828,  2846,  2847,  2848,  2849,
    2850,  2850,  2850,  2854,  2859,  2866,  2866,  2873,  2877,  2881,
    2886,  2891,  2896,  2901,  2905,  2909,  2914,  2918,  2922,  2927,
    2927,  2927,  2933,  2940,  2940,  2940,  2945,  2945,  2945,  2951,
    2951,  2951,  2956,  2960,  2960,  2960,  2965,  2965,  2965,  2974,
    2978,  2978,  2978,  2983,  2983,  2983,  2992,  2996,  2996,  2996,
    3001,  3001,  3001,  3010,  3010,  3010,  3016,  3016,  3016,  3025,
    3028,  3039,  3055,  3057,  3062,  3067,  3055,  3093,  3095,  3100,
    3106,  3093,  3132,  3134,  3139,  3144,  3132,  3175,  3176,  3177,
    3178,  3179,  3183,  3190,  3197,  3203,  3209,  3216,  3223,  3229,
    3238,  3244,  3252,  3257,  3264,  3269,  3275,  3276,  3280,  3281,
    3285,  3285,  3285,  3293,  3293,  3293,  3300,  3300,  3300,  3307,
    3307,  3307,  3318,  3324,  3330,  3336,  3336,  3336,  3346,  3355,
    3355,  3355,  3365,  3365,  3365,  3375,  3375,  3375,  3385,  3394,
    3394,  3394,  3414,  3421,  3421,  3421,  3431,  3434,  3440,  3448,
    3456,  3476,  3501,  3502,  3506,  3507,  3511,  3514,  3517
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
  "\"variant\"", "\"generator\"", "\"yield\"", "\"sealed\"", "\"+=\"",
  "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"",
  "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"",
  "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"", "\"?.\"",
  "\"?[\"", "\"<|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"",
  "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"",
  "\"||=\"", "\"^^=\"", "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"",
  "\"$a\"", "\"$t\"", "\"$c\"", "\"$f\"", "\"...\"",
  "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"unsigned int8 constant\"", "\"floating point constant\"",
  "\"double constant\"", "\"name\"", "\"emitted ,\"", "\"emitted ;\"",
  "\"start of the string\"", "STRING_CHARACTER", "STRING_CHARACTER_ESC",
  "\"end of the string\"", "\"{\"", "\"}\"",
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
  "expression_if_then_else", "$@2", "expression_if_then_else_oneliner",
  "expression_for_loop", "$@3", "expression_unsafe",
  "expression_while_loop", "$@4", "expression_with", "$@5",
  "expression_with_alias", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "metadata_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@6", "expression_block_finally", "$@7", "$@8",
  "expression_block", "$@9", "$@10", "expr_call_pipe", "expression_any",
  "expressions", "optional_expr_list", "optional_expr_map_tuple_list",
  "type_declaration_no_options_list", "name_in_namespace",
  "expression_delete", "new_type_declaration", "$@11", "$@12", "expr_new",
  "expression_break", "expression_continue", "expression_return",
  "expression_yield", "expression_try_catch", "kwd_let_var_or_nothing",
  "kwd_let", "optional_in_scope", "tuple_expansion",
  "tuple_expansion_variable_declaration", "expression_let", "expr_cast",
  "$@13", "$@14", "$@15", "$@16", "$@17", "$@18", "expr_type_decl", "$@19",
  "$@20", "expr_type_info", "expr_list", "block_or_simple_block",
  "block_or_lambda", "capture_entry", "capture_list",
  "optional_capture_list", "expr_full_block",
  "expr_full_block_assumed_piped", "expr_numeric_const", "expr_assign",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@21", "$@22", "$@23", "$@24", "expr_field", "$@25",
  "$@26", "expr_call", "expr", "$@27", "$@28", "$@29", "$@30", "$@31",
  "$@32", "expr_generator", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@33", "$@34", "$@35",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list",
  "global_let_variable_name_with_pos_list", "variable_declaration_list",
  "let_variable_declaration", "global_let_variable_declaration",
  "optional_shared", "optional_public_or_private_variable",
  "global_variable_declaration_list", "$@36", "global_let", "$@37",
  "enum_expression", "commas", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@38",
  "alias_declaration", "optional_public_or_private_enum", "enum_name",
  "optional_enum_basic_type_declaration", "optional_commas", "emit_commas",
  "optional_emit_commas", "enum_declaration", "$@39", "$@40", "$@41",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@42", "$@43", "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_alias_bits",
  "bitfield_type_declaration", "$@44", "$@45", "c_or_s", "table_type_pair",
  "dim_list", "type_declaration_no_options",
  "type_declaration_no_options_no_dim", "$@46", "$@47", "$@48", "$@49",
  "$@50", "$@51", "$@52", "$@53", "$@54", "$@55", "$@56", "$@57", "$@58",
  "$@59", "$@60", "$@61", "$@62", "$@63", "$@64", "$@65", "$@66", "$@67",
  "$@68", "$@69", "$@70", "$@71", "$@72", "type_declaration",
  "tuple_alias_declaration", "$@73", "$@74", "$@75", "$@76",
  "variant_alias_declaration", "$@77", "$@78", "$@79", "$@80",
  "bitfield_alias_declaration", "$@81", "$@82", "$@83", "$@84",
  "make_decl", "make_struct_fields", "make_variant_dim",
  "make_struct_single", "make_struct_dim_list", "make_struct_dim_decl",
  "optional_make_struct_dim_decl", "use_initializer", "make_struct_decl",
  "$@85", "$@86", "$@87", "$@88", "$@89", "$@90", "$@91", "$@92",
  "make_map_tuple", "make_tuple_call", "$@93", "$@94", "make_dim_decl",
  "$@95", "$@96", "$@97", "$@98", "$@99", "$@100", "$@101", "$@102",
  "$@103", "$@104", "expr_map_tuple_list", "make_table_decl",
  "array_comprehension_where", "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1320)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-731)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1320,    37, -1320, -1320,    31,   -92,   165,     7, -1320,   -44,
   -1320, -1320, -1320,    15,   234, -1320, -1320, -1320,   -75,   -47,
     -39, -1320,   439, -1320,   156, -1320, -1320, -1320, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320,   -78, -1320,     2,    48,   120,
   -1320, -1320,   165,    40, -1320, -1320, -1320,   125,   103, -1320,
   -1320,   156,     7,     7,     7,   157,   208, -1320, -1320, -1320,
     234,   234,   234,   187, -1320,   543,   -29, -1320, -1320, -1320,
   -1320, -1320, -1320,   291, -1320,   305, -1320,   393,    66,    31,
     209,   -92,   198,   240, -1320,   256,   292, -1320, -1320, -1320,
     409,   299,   306,   365, -1320,   372,   432, -1320, -1320,   -56,
      31,   234,   234,   234,   234, -1320, -1320, -1320, -1320, -1320,
     452, -1320, -1320, -1320, -1320, -1320,   424, -1320, -1320, -1320,
   -1320, -1320,   529,   107, -1320, -1320, -1320, -1320,   575, -1320,
   -1320,   494, -1320, -1320, -1320,   507,   471,   471, -1320, -1320,
     508, -1320,  -127, -1320,   411,   546,   543,  1188, -1320, -1320,
     527,   584, -1320,   491, -1320, -1320, -1320,   536, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320,    14, -1320,  6611, -1320,   542,
   -1320, -1320,   535, -1320, -1320, -1320, -1320, 10485, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,
   -1320, -1320, -1320,   672,   674, -1320,   513, -1320, -1320,   544,
   -1320,   556, -1320,   440,    31,   518,   559, -1320, -1320, -1320,
     107, -1320,   545,   550,   551,   523,   552,   558, -1320, -1320,
   -1320,   539, -1320, -1320, -1320, -1320, -1320,   560, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,   561,
   -1320, -1320, -1320,   566,   568, -1320, -1320, -1320, -1320,   571,
     573,   557,    15, -1320, -1320, -1320, -1320, -1320, -1320,   347,
     581, -1320,   555, -1320,   562,   563,   598,   599, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,   600,   565,
   -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,
   -1320, -1320,   742, -1320, -1320, -1320, -1320, -1320, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320,   603,   570, -1320, -1320,   -76,
     594, -1320,   572,   522,   507,   602,   574, -1320, -1320,   491,
   -1320,    31, -1320,   464, -1320, -1320, -1320, -1320, -1320,  6182,
   -1320, -1320,   613, -1320,   390,   412,   413, -1320, -1320,  6182,
     272, -1320, -1320, -1320,     0, -1320, -1320, -1320,    23,  3332,
   -1320,   585,  1526, -1320, -1320, -1320, -1320, -1320, -1320, -1320,
     622,   653, -1320,   587, -1320,   130, -1320,   232,  6611, -1320,
   -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,
     583,    15, -1320, -1320, -1320,     8, -1320,   627,  6611, -1320,
     112,  6611,  6611,  6611,   611,   614, -1320, -1320,    73,    15,
     615,    33, -1320,   318,   593,   616,   617,   601,   618,   604,
     378,   619, -1320,   379,   620,   623,  6182,  6182,   605,   606,
     608,   612,   621,   624, -1320, -1320, -1320, -1320, -1320, -1320,
   -1320, -1320, -1320,  6182,  6182,  6182,  6182,  6182,  2952,  6182,
   -1320,   610,  3522, -1320, -1320, -1320,   626, -1320, -1320, -1320,
   -1320,   625, -1320, -1320, -1320, -1320, -1320, -1320,   490,  7042,
   -1320, -1320,   628, -1320, -1320, -1320, -1320, -1320, -1320,  6611,
    6611,   630,   639,  6611,   513,  6611,   513,  6611,   513,  6703,
     648,  7128, -1320,  6182, -1320, -1320, -1320, -1320,   634, -1320,
   -1320,  9966,  3712, -1320, -1320,   654,  6703,   648, -1320, -1320,
    6182, -1320, -1320,    63, -1320,   -64,   581,  1771, -1320, -1320,
      22,   635,   774,   469, -1320, -1320,   -71, -1320, -1320, -1320,
    6182,   437,   472,   638,   252, -1320, -1320, -1320,   636, -1320,
   -1320,   382, -1320,   642,   647,   657, -1320,  6182,  6611,  6182,
    6182, -1320, -1320,  6182, -1320,  6182, -1320,  6182, -1320, -1320,
    6182, -1320,  6611,   371,   371,  6182,  6182,  6182,  6182,  6182,
    6182,   512,   371,   371,   -63,   371,   371,   640,   820,   666,
   10820,   666,   170,   641, 10568, -1320,   669,  2572,   838,   644,
   -1320,   625, -1320,  7113, 10639,  6182,  6182, -1320, -1320,  6182,
    6182,  6182,  6182,   681,  6182,   183,  6182,  6182,  6182,  6182,
    6182,  6182,  6182,  6182,  6182,  3902,  6182,  6182,  6182,  6182,
    6182,  6182,  6182,  6182,  6182,  6182, 11398,  6182, -1320,  4092,
     473,   474, -1320, -1320,   167,   475,   594,   480,   594,   481,
     594,    25, -1320,   250,   581,   676, -1320,   315, -1320,  6611,
     649,   666, -1320, -1320, -1320, 10050, -1320,   118, -1320,   -35,
   -1320,   -35,  7214,   655,   804, -1320,   -27, -1320, -1320,  4282,
   -1320, -1320,  4472,   693, -1320,   -35,   701,  4662,    79,  4852,
   -1320, -1320, -1320,   -35,   -35,   848, -1320,   564, -1320, -1320,
     846, -1320, -1320,   851, -1320,   819,   -35,   661, -1320,   -35,
     -35,   -35,   -35,   -35, -1320,   801, -1320,   -35,  1359,   706,
   -1320,   862,   491,  6182,   709,   710,  6611,   112, -1320,  6182,
    6793, -1320, -1320, -1320, -1320, -1320, -1320, -1320,  6611,  6611,
    6611,  6611,  3142,   711,  6182,  6611, -1320, -1320, -1320,  6611,
     666,   359,   669,  7300,  6611,  6611,  7386,  6611,  7472,  6611,
     666,  6611,  6611,   666,  6611,   698,  7558,  7644,  7730,  7816,
    7902,  7988, -1320,  6182,   429,    63,   682,  6182,   685,   684,
     686,   687,   428, -1320, -1320,    63,  6182,  6182,   683,   102,
     688,   128,  2762, -1320,    91,   726,   699,   702,   513, -1320,
    1976,   838,   721,   708, -1320, -1320,   729,   714, -1320, -1320,
     309,   309,   996,   996,   787,   787,   720,   182,   725, -1320,
   10134,    54,    54,   628,   309,   309, 11131, 11017, 11048, 10903,
   11310, 10654, 11162, 11245,  1326,   996,   996,   957,   957,   182,
     182,   182,   394,  6182,   731,   733,   457,  6182,   909,   760,
   10218, -1320,   152, -1320, -1320, -1320,   763, -1320,   780, -1320,
     782, -1320,   783,  6611, -1320,  6703,  6611, -1320,   648,   366,
     581, -1320, -1320, -1320, -1320,   401, -1320, -1320,  6517, -1320,
     164, -1320, -1320,  6182,   802, -1320,   803, -1320,  6611, -1320,
    6182,  6182, 10820,   959,  6182, 10820,   807, -1320,   799,   822,
   10820, -1320,  6182, 10820, -1320, -1320,   776, -1320, -1320,   786,
     788,   789,   790, -1320,   944, -1320, -1320, -1320, -1320, -1320,
   -1320,   -94, -1320,  6182,  6182,  6182,  6182,  6182,  6182,  6182,
    6182,  6182,  6182,  6182,  6182,  6182,  6182,  6182,  6182,  6182,
    6182,   809, -1320,   118,   503,   919,   791,  8074, -1320,   948,
     -72,  6182,  6876, -1320, -1320, -1320, -1320,   581,   812,   813,
     482,   476,   153,   798,   800,   369,  8160,   483,  6611,  6611,
    6611,   821,   805,   808,  6611,   810, -1320,   828,   829, -1320,
     831, -1320,   832,   814,   833,   835,   816,   839,   838, -1320,
   -1320, -1320, -1320, -1320,   823, 10737,    12,    63, 10820, -1320,
   -1320,  6182,  6182,  6611,   513,    16, 10820, -1320, -1320,   113,
   -1320,   824,  6182,  6182,  6182,   123,  6372, -1320,   478, -1320,
     -12,   594, -1320,   513, -1320,  6182, -1320,  6182,  5042,  6182,
   -1320,   841,   826, -1320, -1320,  6182,   830, -1320, 10302,  6182,
    5232,   834, -1320, 10386, -1320,  5422, -1320,  6182, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,   581,
   -1320, -1320,   581, -1320, -1320,   836,  6611, -1320, -1320,   797,
   -1320, -1320,   818, -1320,   825,  8246, -1320,   978,   -46, 10820,
   10820, -1320, 10820,  6182, -1320, -1320, -1320, 10820,  6182,  6182,
      63,  6182,  6182, -1320, -1320,   865, -1320, -1320, -1320, 10820,
   10820, 10820, 10820, 10820, 10820, 10820, 10820, 10820, 10820, 10820,
   10820, 10820, 10820, 10820, 10820, 10820, 10820,  6182,   462, -1320,
   -1320, -1320,   296, -1320,    -3, -1320, -1320,   866, -1320,  6182,
    6704, -1320, -1320, -1320, -1320,   837, -1320, -1320, -1320,   840,
     869, -1320, -1320,   856,   858,   861, -1320, -1320,  6182,   863,
   -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,
      95,  5612, -1320,  6182,    19,  8332,  8418,   867,   594,  6182,
   -1320,  8504, 10820, 10820,   842,  2762,   849,   115,   883,   887,
     853,   888,   892, -1320,   169,   471,   594,  6611,  8590,  6611,
    8676, -1320,   212,  8762, -1320,  6182, 10934,  6182, -1320,  8848,
   -1320,   225,  6182, -1320, -1320, -1320,   269, -1320, -1320, -1320,
    6182,   581, -1320, -1320, -1320, -1320,   893,  6182, -1320, 10820,
    8934,  9020,    20,  9106,  9192,   852, -1320,   311,   -90, 10820,
   -1320,   854,  1024,    -3, -1320, -1320,   503, -1320,  6704, -1320,
     859,   864, -1320, -1320,  6182,   880, -1320, -1320, -1320, -1320,
     868,   870,   669,   872,  6182,  6182,  6182,   873,   992,   874,
     877,  5802, -1320, -1320,   313,   326,  6182, -1320, -1320, -1320,
     884,   335,   147, -1320,   179,  6182,  6182,  6182, -1320, -1320,
     903, -1320, -1320,   -12, -1320,   929,   471,   889, -1320,   488,
   -1320, -1320, -1320,  6611,  9278,  9364, -1320, -1320,  9450, -1320,
     879, -1320, 10820,  1056,   471,  6182,   471,   471, -1320,   915,
     341, -1320, -1320, -1320, -1320, -1320, -1320,  1024,    63, -1320,
     992,   992,  9536,   881,   890,   894,   896,  6182, -1320, -1320,
    6182,   996,   996,   996,  6182, -1320, -1320,   992,   476, -1320,
    9622, -1320, -1320,   895,   336,   136, -1320,   898,  6182,  6182,
     891,  9708, 10820, 10820,   897, -1320,  5992, -1320, -1320, -1320,
   -1320, -1320,   489, -1320, -1320, -1320, -1320,  2382,   -35, -1320,
     340, -1320, -1320,  2181, -1320,  6611,   112,  1188, -1320, -1320,
     -41,   -41, -1320,  6182,   992,   992,   476,   899,   902,   666,
     -41,   726,   905, -1320,  6182,   904, -1320,   136,  6182, 10820,
   10820, -1320,   150, -1320,  6182, 10903, -1320, -1320, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320, 10820, -1320,   397,   471, -1320,
   -1320,   878,   112,  6182,   906,  1188,   476,   726,   920, -1320,
     908,   910,  9794,   -41,   -41,   916, -1320, -1320,   918,   922,
   -1320,  6959,  6182, -1320,   912,  6182,  6182, 10934, -1320,   471,
   -1320, -1320,   907, -1320, -1320, -1320,  6182,  6704, -1320, -1320,
     342,   921, -1320, -1320, -1320, -1320,   928,   931, -1320, -1320,
   -1320,  1043,   934,  6959,   914, 10820, 10820, -1320,  6182, -1320,
    6704, -1320, -1320, -1320,   476, -1320, -1320,  6182, -1320,   935,
   -1320, -1320,  9880, -1320,   348, 10820, -1320,   471, -1320, -1320,
     397, -1320
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   130,     1,   291,     0,     0,     0,   577,   292,     0,
     752,   742,   747,     0,     0,    16,    15,     3,     0,     0,
       0,     8,     0,     7,   558,     6,    11,     5,     4,    13,
      12,    14,   102,   103,   101,   110,   112,    42,    55,    52,
      53,    44,     0,    50,    43,   579,   578,     0,     0,    26,
      25,   558,   577,   577,   577,     0,   265,    40,   117,   118,
       0,     0,     0,   119,   121,   128,     0,   116,    21,    10,
       9,   605,   604,   223,   595,   606,   559,   560,     0,     0,
       0,     0,    45,     0,    51,     0,     0,    48,   580,   582,
      22,     0,     0,     0,   267,     0,     0,   127,   122,     0,
       0,     0,     0,     0,     0,   131,   225,   224,   227,   222,
     583,   608,   607,   611,   562,   561,   568,   108,   109,   106,
     107,   105,     0,     0,   104,   113,    56,    54,    50,    47,
      46,     0,    23,    24,    27,   593,    64,    64,   266,    38,
      41,   126,     0,   123,   124,   125,   129,     0,   585,   584,
       0,   601,   563,   493,    28,    29,    33,     0,    97,    98,
      95,    96,    94,    93,    99,     0,    49,     0,   591,   594,
     753,    62,    65,   743,   748,    39,   120,     0,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,     0,     0,   137,   132,   233,   586,   587,
     602,     0,   612,   565,     0,   494,     0,    30,    31,    32,
       0,   111,     0,     0,     0,     0,     0,     0,   619,   639,
     620,   655,   621,   625,   626,   627,   628,   645,   632,   633,
     634,   635,   636,   637,   638,   640,   641,   642,   643,   712,
     624,   631,   644,   719,   726,   622,   629,   623,   630,     0,
       0,     0,     0,   654,   676,   679,   677,   678,   739,   674,
     581,   592,     0,    63,     0,     0,   191,   192,   189,   140,
     141,   143,   142,   144,   145,   146,   147,   173,   174,   171,
     172,   164,   175,   176,   165,   162,   163,   190,   184,     0,
     188,   177,   178,   179,   180,   151,   152,   153,   148,   149,
     150,   161,     0,   167,   168,   166,   159,   160,   155,   154,
     156,   157,   158,   139,   138,   183,     0,   169,   170,   493,
     135,   228,     0,     0,   593,   599,   609,   564,   567,   493,
     114,     0,   547,   540,   569,   100,   680,   703,   706,     0,
     709,   699,     0,   663,   713,   720,   727,   733,   736,     0,
       0,   689,   694,   688,     0,   702,   698,   691,     0,     0,
     693,   675,     0,   754,   744,   749,   193,   194,   187,   182,
     195,   185,   181,     0,   133,   290,   515,     0,     0,   226,
     256,   646,   649,   652,   653,   647,   650,   648,   651,   588,
       0,     0,   603,   506,   613,     0,   115,     0,     0,   541,
       0,     0,     0,     0,     0,     0,   398,   399,     0,     0,
       0,     0,   392,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   645,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   482,   337,   339,   338,   340,   341,
     342,   343,    34,     0,     0,     0,     0,     0,     0,     0,
     322,   323,     0,   396,   395,   472,   393,   465,   464,   463,
     462,   130,   468,   394,   467,   466,   439,   400,   440,     0,
     444,   401,     0,   397,   757,   761,   758,   759,   760,     0,
       0,     0,     0,     0,   132,     0,   132,     0,   132,     0,
       0,     0,   685,   259,   696,   697,   690,   692,     0,   695,
     671,     0,     0,   741,   740,   660,   521,   527,   196,   186,
       0,   288,   289,     0,   134,   493,   136,     0,   596,   600,
     507,     0,   542,   540,   566,   548,     0,   538,   539,   537,
       0,     0,     0,     0,   668,   780,   783,   270,   274,   273,
     279,     0,   311,     0,     0,     0,   809,     0,     0,     0,
       0,   302,   305,     0,   308,     0,   813,     0,   789,   795,
       0,   786,     0,   428,   429,     0,     0,     0,     0,     0,
       0,     0,   405,   404,   441,   403,   402,     0,     0,   824,
     317,   824,   324,     0,   793,   816,   824,     0,   332,   323,
     256,   130,   236,     0,     0,     0,     0,   430,   431,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   382,     0,   683,     0,
       0,     0,   656,   658,     0,     0,   135,     0,   135,     0,
     135,   265,   519,     0,   517,     0,   525,     0,   657,     0,
       0,   824,   687,   670,   673,     0,   661,   589,   522,    80,
     528,    80,     0,     0,   614,   513,   532,   516,   258,   282,
     280,   233,     0,     0,   281,     0,     0,     0,    64,     0,
      20,    19,   237,     0,     0,     0,   250,     0,   251,   245,
       0,   242,   241,     0,   243,     0,     0,     0,   257,     0,
      76,    77,    74,    75,   252,   294,   240,     0,   344,   574,
     610,     0,   493,     0,     0,     0,     0,     0,   555,     0,
       0,   681,   704,   707,    18,    17,   666,   667,     0,     0,
       0,     0,   778,     0,     0,     0,   799,   802,   805,     0,
     824,     0,   824,     0,     0,     0,     0,     0,     0,     0,
     824,     0,     0,   824,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,    35,     0,     0,   825,     0,     0,
       0,     0,   370,   367,   369,     0,     0,   825,     0,     0,
       0,   265,     0,   385,     0,   771,     0,     0,   132,   324,
       0,   332,     0,     0,   451,   450,     0,     0,   452,   456,
     406,   407,   419,   420,   417,   418,     0,   445,     0,   437,
       0,   469,   470,   471,   408,   409,   424,   425,   426,   427,
       0,     0,   422,   423,   421,   415,   416,   411,   410,   412,
     413,   414,     0,     0,     0,   376,     0,     0,     0,     0,
       0,   390,     0,   710,   700,   664,     0,   714,     0,   721,
       0,   728,     0,     0,   734,     0,     0,   737,     0,     0,
     263,   684,   260,   672,   572,   590,   755,    78,    81,   745,
      81,   750,   514,     0,     0,   531,     0,   530,     0,   533,
       0,     0,   283,     0,     0,   268,     0,   255,     0,     0,
      59,   233,     0,   285,   253,   254,     0,    69,    70,     0,
       0,     0,     0,   244,   229,   239,   246,   247,   248,   249,
     293,     0,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,   575,   589,   500,   503,     0,     0,   544,   545,
       0,     0,     0,   557,   682,   705,   708,   669,     0,     0,
       0,   779,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   442,     0,     0,   443,
       0,   473,     0,     0,     0,     0,     0,     0,   332,   477,
     478,   479,   480,   481,     0,     0,     0,     0,   318,   798,
     432,     0,     0,     0,   132,     0,   792,   817,   818,     0,
     386,     0,     0,     0,     0,     0,     0,   389,     0,   387,
       0,   135,   336,   132,   447,     0,   453,     0,     0,     0,
     435,     0,     0,   457,   461,     0,     0,   438,     0,     0,
       0,     0,   377,     0,   383,     0,   433,     0,   391,   711,
     701,   665,   659,   715,   717,   722,   724,   729,   731,   518,
     735,   520,   524,   738,   526,     0,     0,   662,   573,     0,
      79,   523,     0,   529,     0,     0,   616,   617,   534,   536,
     284,   233,   269,     0,    57,    58,    87,   286,     0,     0,
       0,     0,     0,   230,   235,     0,   549,   300,   299,   354,
     355,   357,   356,   358,   348,   349,   350,   359,   360,   346,
     347,   361,   362,   351,   352,   353,   345,     0,   590,   597,
     501,   502,   503,   504,   495,   508,   543,     0,   552,     0,
       0,   556,   781,   784,   271,     0,   276,   277,   275,     0,
       0,   314,   312,     0,     0,     0,   810,   808,   261,     0,
     819,   303,   306,   309,   814,   812,   790,   796,   794,   787,
      64,     0,    36,     0,     0,     0,     0,     0,   135,     0,
     388,     0,   763,   762,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   330,     0,    64,   135,     0,     0,     0,
       0,   365,     0,     0,   458,     0,   446,     0,   436,     0,
     378,     0,     0,   434,   384,   380,     0,   718,   725,   732,
     259,   264,   756,   746,   751,   615,     0,     0,   287,    92,
       0,     0,     0,     0,     0,     0,   295,     0,     0,   571,
     576,     0,   498,   495,   496,   497,   500,   546,     0,   554,
       0,     0,   272,   278,     0,     0,   313,   800,   803,   806,
       0,     0,   824,     0,     0,     0,     0,     0,   778,     0,
       0,     0,   233,   483,     0,     0,     0,   368,   492,   371,
       0,     0,     0,   363,     0,     0,     0,     0,   327,   328,
       0,   326,   325,     0,   333,   233,    64,     0,   491,     0,
     489,   366,   486,     0,     0,     0,   485,   379,     0,   381,
       0,   618,   535,    71,    64,     0,    64,    64,   256,     0,
     540,   550,   301,   551,   598,   499,   509,   498,     0,   553,
     778,   778,     0,     0,     0,     0,     0,   259,   820,   262,
     261,   304,   307,   310,     0,   779,   791,   778,     0,   474,
       0,   476,   484,     0,     0,     0,   374,     0,     0,     0,
       0,     0,   765,   764,     0,   331,     0,   319,   334,   233,
     448,   454,     0,   490,   488,   487,   686,     0,     0,   233,
       0,   233,   233,     0,   296,     0,     0,     0,   511,   505,
     777,   777,   315,     0,   778,   778,     0,     0,     0,   824,
     777,   770,     0,   475,     0,     0,   372,     0,     0,   767,
     766,   364,     0,   329,     0,   320,   335,   449,   455,   459,
      72,    76,    77,    74,    75,    73,    84,    66,    64,    89,
      91,     0,     0,     0,     0,     0,     0,   774,   824,   776,
       0,     0,     0,   777,   777,     0,   811,   821,     0,     0,
     788,   822,     0,   375,   822,     0,     0,   321,   460,    64,
      60,    61,     0,    83,   233,   232,     0,     0,   510,   233,
       0,   825,   775,   782,   785,   316,     0,     0,   807,   815,
     797,     0,     0,   822,     0,   769,   768,   233,     0,    86,
       0,   298,   512,   772,     0,   801,   804,     0,   826,     0,
     828,    67,     0,   297,     0,   823,   827,    64,   773,   233,
      66,    68
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1320, -1320,  -659,  -504, -1320, -1320, -1320, -1320,   553,  1057,
   -1320, -1320, -1320,  1135, -1320, -1320, -1320,  1095, -1320,  1015,
   -1320, -1320,  1063, -1320, -1320, -1320, -1320,  -134,  -355, -1320,
   -1320,  -221,   477,   479, -1320, -1320, -1320, -1320, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320,   927, -1320, -1320,   -52,  1049,
   -1320, -1320, -1320,   254, -1320,  -430,  -480,  -631, -1320, -1320,
   -1320, -1319, -1320, -1320, -1320, -1320, -1320,  -671, -1320, -1320,
   -1320, -1320,  -594, -1175,  -179, -1320,   -13, -1320, -1320, -1320,
   -1320, -1320,  -212,  -211,  -210,  -209, -1320, -1320,  1158, -1320,
   -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,
   -1320, -1320, -1320, -1320,  -166, -1320,   689,  -121, -1320,  -773,
   -1320, -1320, -1320, -1320, -1320, -1320, -1303, -1320, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320, -1320,   317, -1320, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320,  -140,   -70,  -152,   -67,    44,
   -1320, -1320, -1320, -1320, -1320,   646, -1320,  -505, -1320, -1320,
    -497, -1320, -1320,  -667,  -143,  -531,  -529, -1320, -1320, -1320,
    -899, -1320,  1131, -1320, -1320, -1320, -1320, -1320,    65,   241,
   -1320,   663, -1320, -1320, -1320, -1320, -1320, -1320,   243, -1320,
     855, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,
   -1320, -1320, -1320, -1320,  -751,  -155, -1320,   769, -1320, -1320,
   -1320, -1320, -1320, -1320,  -632, -1320, -1320,  -297, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320, -1320,  -108, -1320, -1320, -1320,
   -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,
   -1320, -1320,   770,  -595,  -196,  -713, -1320, -1320, -1274, -1212,
   -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,  -780,
   -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320, -1320,
   -1320, -1320, -1320, -1320,  -410, -1320, -1202,  -565, -1320
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   736,   737,    16,   134,    51,    17,   157,   163,
     581,   463,   140,   464,    96,    19,    20,    43,    44,    87,
      21,    39,    40,   693,   694,  1452,   172,   173,  1453,   909,
    1368,   695,   878,   879,   696,   697,   698,   699,   700,   701,
     702,   703,   704,   705,   706,   164,   165,    35,    36,    37,
     215,    63,    64,    65,    66,    22,   330,   389,   206,    23,
     108,   207,   109,   147,  1094,  1225,  1421,   331,   332,   707,
     465,   708,   527,   660,  1251,   869,   466,   709,   548,   741,
    1242,   467,   710,   711,   712,   713,   714,   523,   715,   921,
    1227,  1097,   716,   468,   754,  1254,   755,  1255,   757,  1256,
     469,   745,  1246,   470,   589,  1358,   471,  1183,  1184,   798,
     472,   602,   473,   717,   474,   475,   784,   476,  1003,  1345,
    1004,  1397,   477,   848,  1204,   478,   590,  1187,  1407,  1189,
    1408,  1293,  1448,   480,   481,   385,  1236,  1316,  1122,  1124,
     946,   530,   722,  1377,  1425,   386,   387,   652,   653,   669,
     656,   657,   671,  1014,   675,   540,   410,   533,   343,  1228,
     534,   344,    77,   116,   213,   339,    25,   153,   942,   875,
     943,    47,    48,   131,    26,   150,   209,   334,   876,   169,
     170,    27,   110,   719,  1231,   402,   211,   212,    75,   113,
     404,    28,   151,   336,   676,   482,   399,   265,   266,   644,
     667,   267,   492,  1051,   738,   543,   371,   268,   269,   411,
     954,   659,   490,  1050,   412,   955,   413,   956,   489,  1049,
     493,  1053,   494,  1207,   495,  1055,   496,  1208,   497,  1057,
     498,  1209,   499,  1060,   500,  1063,   654,    29,    53,   274,
     516,  1072,    30,    54,   275,   517,  1074,    31,    52,   272,
     515,  1069,   483,  1427,  1392,   796,  1428,  1429,  1430,   964,
     484,   739,  1240,   740,  1241,   764,  1260,   761,  1258,   595,
     485,   762,  1259,   486,   968,  1324,   969,  1325,   970,  1326,
     749,  1250,   759,  1257,  1252,   487,  1472,   778,   488
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      57,    67,   795,   174,   727,   729,   800,  1007,   874,   890,
     893,   668,   264,   216,   646,   858,   648,   860,   650,   862,
     670,   865,  1098,   692,   996,   868,   779,   125,  1023,   963,
    -130,   788,  1163,   504,  1005,  1300,  1169,     2,   553,  1266,
    1305,   598,  1396,  1234,     3,    79,  1336,    67,    67,    67,
      32,    33,   596,   531,   537,   537,   506,   531,  1424,   270,
     538,   538,    83,    38,    45,   615,   616,     4,   532,     5,
      46,     6,   532,   176,   383,   514,  1011,     7,   414,   415,
     885,   117,   118,   101,   102,   103,   383,     8,    67,    67,
      67,    67,   690,     9,  1443,    78,   872,    84,   421,   885,
     721,   539,   539,  1235,   423,  1095,  1459,  1431,  1380,  1381,
    1096,   536,   790,  1311,  1178,  1312,  1439,    10,    49,   974,
    1179,  1009,   158,   159,   384,  1390,   214,   887,    68,   690,
     372,   691,   728,   636,   637,   554,   555,   521,   214,    11,
      12,   430,   431,   104,   141,   886,   887,   795,   888,   752,
    1180,   889,  1387,    55,   263,   531,    69,    50,  1426,  1466,
    1467,  1181,   340,    95,    70,   877,  1182,   877,   691,   105,
     532,   801,  1433,  1434,    79,   433,   434,    56,   505,   613,
     522,   897,   615,   616,   886,   972,   220,   975,   886,   904,
     905,   886,   886,    34,   795,   983,   951,  1015,   986,   405,
     863,   507,   913,   603,   604,   915,   916,   917,   918,   919,
     673,    55,   556,   922,   221,  1160,  1068,   264,    76,    14,
     508,   119,    13,    80,    85,   674,   120,   720,   121,   509,
    1086,   122,   557,   264,    14,    56,    86,  1066,   537,   885,
      15,   885,  1474,   171,   538,  1013,  1164,  1276,  1135,   360,
     636,   637,   547,   264,   885,   790,   264,   264,   264,   171,
    1013,   751,   160,  1016,  1009,   123,    95,   161,    58,   162,
     458,  1489,   122,   885,    55,   765,   885,   462,   565,  1348,
     526,   734,  1445,   780,   874,   539,   887,    88,   887,   406,
     735,  1017,    81,   591,  1261,  1018,   692,    59,    56,   607,
     608,   887,  1010,   541,   542,   544,    89,   613,    55,   614,
     615,   616,   617,   618,    97,    98,    99,   780,  1021,    94,
     887,  1174,   781,   887,  1047,  1047,   655,    41,   690,  1313,
     603,   604,    56,  1140,   264,   264,   818,   661,   264,  1222,
     264,  1283,   264,  1232,   264,   819,    95,   855,   106,   782,
      42,  1018,  1048,  1136,   107,   143,   144,   145,   146,   263,
    1061,   264,   111,  1123,   126,    60,   795,   691,   112,  1284,
     856,  1064,    55,  1071,  1070,   263,  1070,  1350,   636,   637,
     361,   640,   641,  1073,  1047,   645,   100,   647,   529,   649,
    1185,   750,   603,   604,    86,   263,    56,  1047,   263,   263,
     263,   760,   128,   264,   763,   263,   551,   362,   363,  1449,
    1218,  1217,  1291,   734,   690,   734,   690,   264,   129,  1129,
    1450,  1451,   735,    61,   735,  1297,   607,   608,   372,   950,
     864,   794,   524,    62,   613,   525,   614,   615,   616,   617,
     618,  1047,   958,   959,    71,    72,  1128,    73,   805,   809,
     114,   502,   971,   691,   130,   691,   115,   977,   978,  1068,
     980,   135,   982,   823,   984,   985,   132,   987,   136,  1299,
     364,   503,   133,   852,   365,    74,   263,   263,   734,   690,
     263,   849,   263,  1309,   263,  1047,   263,   735,   607,   608,
     631,   632,   633,   634,   635,   867,   613,   558,  1047,   615,
     616,   617,   618,   263,   264,   636,   637,  1047,  1047,   148,
    1277,  1310,  1047,  1342,  1018,   149,  1375,   559,   691,   409,
    1018,   366,   734,   690,  1168,   367,  1343,   137,   368,   734,
     690,   735,   734,   690,   138,  1347,  1395,  1270,   735,   973,
    1418,   735,  1483,  1186,   369,   263,  1065,  1036,  1498,  1139,
     101,   870,   103,   370,   901,  1286,  1037,   566,   569,   263,
    1120,   743,   691,  1067,   734,  -716,  1121,   636,   637,   691,
    -716,   264,   691,   735,   907,   908,   962,   567,   570,   783,
    1274,   744,   945,   264,   264,   264,   264,  -723,  -730,  -716,
     264,  1341,  -723,  -730,   264,   217,   218,   391,   139,   264,
     264,   392,   264,  -373,   264,  1349,   264,   264,  -373,   264,
    1041,  -723,  -730,   372,  1357,   393,   394,   731,  1444,  1042,
     395,   396,   397,   398,   941,   734,  1239,  -373,   152,   790,
     957,  1176,    84,   960,   735,   171,   407,   967,  1009,   408,
    1177,   725,   409,   337,   726,   338,   263,   409,   372,   372,
     372,   372,   732,   853,   854,   857,   372,   372,   372,   372,
     859,   861,  1134,  1142,   372,   372,   479,   167,  1361,  1409,
     168,  1143,  1144,  1145,   175,  1034,   501,  1149,   154,   155,
     772,   773,   101,   102,   103,   101,   511,  1329,  1406,   208,
     210,   460,   599,   214,   600,   154,   155,   156,  1417,   273,
    1419,  1420,   217,   218,   219,   271,  1167,   327,   264,   328,
     264,   264,   329,   263,  1373,    91,    92,    93,   335,   333,
     341,   342,   349,   264,   346,   263,   263,   263,   263,   347,
     348,   350,   263,   264,  1319,  1446,   263,   351,   352,   353,
     354,   263,   263,  1391,   263,   355,   263,   356,   263,   263,
     357,   263,   358,   573,   574,  1059,   359,   372,  1062,   373,
     376,   377,   378,   379,   380,   381,   374,   375,   382,   388,
     582,   583,   584,   585,   586,   491,   390,   401,   403,   594,
    1078,  1376,   512,  1479,   518,   519,   520,   528,  1482,   535,
     545,  1391,   560,   546,   552,   561,   562,   564,   568,   571,
     563,   643,   572,   565,   575,   576,  1491,   577,   603,   604,
     655,   578,   592,   264,   264,   264,   666,   724,   733,   264,
     579,   746,    14,   580,  1438,   597,   747,   639,  1500,   665,
     642,  1460,   662,   776,   723,   742,   748,   672,   777,   775,
     785,   787,   797,   816,   718,  1423,   799,   884,   264,   871,
     263,   866,   263,   263,   883,   896,   898,   730,   906,   910,
     911,   912,  1192,  1462,  1416,   263,   914,   920,   941,   692,
     944,   948,   949,   965,  1201,   263,   594,   753,   988,  1206,
     756,   997,   758,   999,  1000,  1001,  1002,  1012,  1008,  1494,
    1287,  1456,   766,   767,   768,   769,   770,   771,  1018,  1019,
    1024,  1020,   605,   606,   607,   608,   609,  1025,  1026,   610,
    1044,   264,   613,  1027,   614,   615,   616,   617,   618,  1028,
     619,   620,   810,   811,  1029,  1052,   812,   813,   814,   815,
    1039,   817,  1040,   820,   821,   822,   824,   825,   826,   827,
     828,   829,   831,   832,   833,   834,   835,   836,   837,   838,
     839,   840,   841,  1481,   850,   263,   263,   263,  1211,  1045,
    1054,   263,  1056,  1058,  1076,  1077,   629,   630,   631,   632,
     633,   634,   635,  1081,  1084,  1088,  1493,  1085,   603,   604,
    1083,  1093,  1117,   636,   637,  1089,  1123,  1090,  1091,  1092,
     263,  1127,  1132,  1133,  1125,  1264,   892,  1265,  1137,   895,
    1138,  1146,  1212,  1271,   900,  1147,   903,  1148,  1151,  1152,
    1150,  1153,  1154,  1156,  1155,  1157,  1158,   603,   604,  1159,
    1194,  1216,  1161,  1213,  1170,  1195,  1262,  1226,  1237,  1197,
    1214,  1245,   264,  1202,   264,  1210,  1247,  1243,  1248,  1244,
     947,  1249,  1273,  1253,   661,  1278,   952,  1269,  1275,  1279,
    1281,  1285,  1280,   263,  1282,  1301,  1308,  1315,  1320,  1314,
    1323,   966,  1335,  1321,  1346,  1354,  1356,  1327,  1367,  1360,
    1328,  1330,  1334,  1337,   607,   608,  1338,  1374,  1422,  1366,
    1383,  1289,   613,  1455,   614,   615,   616,   617,   618,  1384,
     995,  1401,  1461,  1385,   998,  1386,  1487,  1403,  1394,  1436,
    1344,  1398,  1437,  1006,   594,  1440,  1478,  1442,  1463,  1458,
    1464,   605,   606,   607,   608,  1471,  1468,   718,  1469,  1490,
    1484,   613,  1470,   614,   615,   616,   617,   618,  1485,   619,
     620,  1486,  1488,  1496,   774,   124,    18,    82,   264,  1370,
     633,   634,   635,   166,   127,  1501,  1410,   345,   880,   142,
     881,  1388,  1359,   636,   637,  1411,  1412,  1413,  1414,    24,
    1038,   661,  1355,  1317,  1043,  1378,  1233,   601,  1389,  1318,
    1369,   677,  1371,  1372,   263,  1379,   263,   631,   632,   633,
     634,   635,    90,  1230,  1118,  1362,  1119,   549,   550,   400,
    1435,     0,   636,   637,     0,     0,     0,     0,     0,     0,
    1075,     0,     0,     0,     0,     0,     0,  1079,  1080,     0,
       0,  1082,     0,     0,     0,     0,     0,     0,     0,  1087,
     264,     0,     0,   177,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1099,  1100,  1101,  1102,  1103,  1104,  1105,  1106,  1107,  1108,
    1109,  1110,  1111,  1112,  1113,  1114,  1115,  1116,     0,   178,
       0,   179,     0,   180,   181,   182,   183,   184,  1130,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     263,   196,   197,   198,  1454,     0,   199,   200,   201,   202,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   203,   204,     0,     0,     0,
       0,     0,     0,     0,     0,  1477,     0,     0,  1165,  1166,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1171,
    1172,  1173,   783,   998,     0,     0,     0,     0,     0,     0,
       0,     0,  1188,     0,  1190,     0,  1193,   603,   604,     0,
     205,     0,  1196,     0,     0,     0,  1199,     0,     0,     0,
       0,     0,   263,  1499,   998,     0,     0,     0,     0,   -73,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     603,   604,     0,     0,   783,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1219,     0,     0,     0,     0,  1220,  1221,     0,  1223,  1224,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1229,     0,     0,     0,     0,     0,
       0,   605,   606,   607,   608,   609,  1238,     0,   610,   611,
     612,   613,     0,   614,   615,   616,   617,   618,     0,   619,
     620,     0,     0,     0,     0,   594,   923,   924,   925,   926,
     927,   928,   929,   930,   605,   606,   607,   608,   609,   931,
     932,   610,   611,   612,   613,   933,   614,   615,   616,   617,
     618,   934,   619,   620,   935,   936,     0,     0,   621,   622,
     623,   937,   938,   939,   624,   629,   630,   631,   632,   633,
     634,   635,  1294,     0,  1295,     0,     0,     0,     0,  1298,
       0,     0,   636,   637,     0,     0,     0,     0,     0,     0,
       0,     0,   940,   625,  1302,   626,   627,   628,   629,   630,
     631,   632,   633,   634,   635,   222,     0,     0,     0,     0,
       0,   223,     0,     0,     0,   636,   637,   224,     0,     0,
       0,  1322,     0,     0,     0,     0,     0,   225,     0,     0,
       0,  1331,  1332,  1333,     0,   226,     0,     0,  1340,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     227,     0,  1351,  1352,  1353,     0,     0,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   594,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,  1399,  1400,     0,     0,     0,
       0,     0,     0,  1405,     0,     0,     0,   261,     0,     0,
       0,     0,     0,     0,  1415,     0,     0,     0,    56,     0,
     718,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1441,     0,     0,     0,   594,     0,     0,     0,     0,
       0,  1447,     0,     0,     0,     0,     0,   262,     0,     0,
       0,     0,   513,     0,     0,     0,     0,     0,     0,     0,
    1457,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1473,
       0,     0,  1475,  1476,     0,     0,     0,     0,     0,     0,
       0,     0,   678,  1480,     0,     0,   414,   415,     3,     0,
     -88,   -82,   -82,     0,   -85,     0,   416,   417,   418,   419,
     420,     0,     0,     0,     0,  1492,   421,   679,   422,   680,
     681,     0,   423,     0,  1495,     0,     0,     0,     0,   682,
     424,     0,     0,   -90,     0,   683,   425,     0,     0,   426,
       0,     8,   427,   684,     0,   685,   428,     0,     0,   686,
     687,     0,     0,     0,     0,     0,   688,     0,     0,   430,
     431,     0,   228,   229,   230,     0,   232,   233,   234,   235,
     236,   432,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,     0,   250,   251,   252,     0,     0,   255,
     256,   257,   258,   433,   434,   435,   689,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   436,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,   438,   439,   440,
     441,   442,     0,   443,     0,   444,   445,   446,   447,   448,
     449,   450,   451,    56,     0,   690,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   453,   454,   455,     0,    13,     0,     0,   456,
     457,     0,     0,     0,     0,     0,     0,     0,   458,     0,
     459,     0,   460,   461,   691,   462,  -234,   678,     0,     0,
       0,   414,   415,     3,     0,   -88,   -82,   -82,     0,   -85,
       0,   416,   417,   418,   419,   420,     0,     0,     0,     0,
       0,   421,   679,   422,   680,   681,     0,   423,     0,     0,
       0,     0,     0,     0,   682,   424,     0,     0,   -90,     0,
     683,   425,     0,     0,   426,     0,     8,   427,   684,     0,
     685,   428,     0,     0,   686,   687,     0,     0,     0,     0,
       0,   688,     0,     0,   430,   431,     0,   228,   229,   230,
       0,   232,   233,   234,   235,   236,   432,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,     0,   250,
     251,   252,     0,     0,   255,   256,   257,   258,   433,   434,
     435,   689,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   437,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,   438,   439,   440,   441,   442,     0,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,    56,     0,
     690,   452,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   453,   454,   455,
       0,    13,     0,     0,   456,   457,     0,     0,     0,     0,
       0,     0,     0,   458,     0,   459,     0,   460,   461,   691,
     462,  1022,   678,     0,     0,     0,   414,   415,     3,     0,
     -88,   -82,   -82,     0,   -85,     0,   416,   417,   418,   419,
     420,     0,     0,     0,     0,     0,   421,   679,   422,   680,
     681,     0,   423,     0,     0,     0,     0,     0,     0,   682,
     424,     0,     0,   -90,     0,   683,   425,     0,     0,   426,
       0,     8,   427,   684,     0,   685,   428,     0,     0,   686,
     687,     0,     0,     0,     0,     0,   688,     0,     0,   430,
     431,     0,   228,   229,   230,     0,   232,   233,   234,   235,
     236,   432,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,     0,   250,   251,   252,     0,     0,   255,
     256,   257,   258,   433,   434,   435,   689,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   436,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,   438,   439,   440,
     441,   442,     0,   443,     0,   444,   445,   446,   447,   448,
     449,   450,   451,    56,     0,   690,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   453,   454,   455,     0,    13,     0,     0,   456,
     457,     0,     0,     0,     0,     0,     0,     0,   458,     0,
     459,     0,   460,   461,   691,   462,  -231,   414,   415,     0,
       0,     0,     0,     0,     0,     0,     0,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,   679,   422,
     680,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,   684,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,   689,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,     0,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,   789,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,   790,   444,   445,   446,   447,
     448,   449,   450,   451,   791,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   792,
       0,   459,   793,   460,   461,   587,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,   588,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,   790,   444,   445,   446,   447,
     448,   449,   450,   451,   791,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,     0,   460,   461,   587,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,   588,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,     0,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,   961,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,   790,   444,   445,   446,   447,
     448,   449,   450,   451,   791,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,     0,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
     510,   459,     0,   460,   461,   593,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,     0,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
     664,   459,     0,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,   830,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,     0,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,   851,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,   891,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,     0,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,   894,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,     0,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
     899,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,     0,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,   902,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,     0,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,  1191,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,  1200,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,  1205,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,  1263,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,  1339,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,  1404,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,     0,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,     0,    13,     0,     0,
     456,   457,     0,     0,     0,     0,     0,   414,   415,   458,
       0,   459,     0,   460,   461,     0,   462,   416,   417,   418,
     419,   420,     0,     0,     0,     0,     0,   421,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
     426,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
     430,   431,     0,   228,   229,   230,     0,   232,   233,   234,
     235,   236,   432,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,     0,   250,   251,   252,     0,     0,
     255,   256,   257,   258,   433,   434,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,   441,   442,     0,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,    56,     0,   222,   452,     0,     0,
       0,     0,   223,     0,     0,     0,     0,     0,   224,     0,
       0,     0,     0,   453,   454,   455,     0,    13,   225,     0,
     456,   457,     0,     0,     0,     0,   226,     0,     0,  1175,
       0,   459,     0,   460,   461,     0,   462,     0,     0,     0,
       0,   227,     0,     0,     0,     0,     0,     0,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,     0,   224,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   225,     0,     0,    55,     0,     0,     0,     0,
     226,     0,     0,     0,     0,     0,     0,     0,   261,     0,
       0,     0,     0,     0,     0,   227,     0,     0,     0,   651,
       0,   690,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,     0,     0,     0,   262,     0,
     691,     0,   222,     0,     0,   603,   604,     0,   223,     0,
       0,     0,     0,     0,   224,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   225,     0,     0,     0,     0,    55,
       0,     0,   226,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   261,     0,     0,     0,     0,   227,     0,     0,
       0,     0,     0,    56,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,     0,     0,     0,
       0,     0,   262,     0,   603,   604,     0,     0,     0,   605,
     606,   607,   608,   609,     0,     0,   610,   611,   612,   613,
       0,   614,   615,   616,   617,   618,     0,   619,   620,     0,
       0,    55,     0,   621,   622,   623,     0,     0,     0,   624,
       0,     0,     0,     0,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   651,     0,     0,   690,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   625,     0,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
       0,     0,     0,     0,     0,     0,     0,   603,   604,     0,
     636,   637,     0,     0,   262,     0,     0,   691,   605,   606,
     607,   608,   609,     0,     0,   610,   611,   612,   613,     0,
     614,   615,   616,   617,   618,     0,   619,   620,     0,     0,
       0,     0,   621,   622,   623,     0,     0,     0,   624,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   625,     0,   626,
     627,   628,   629,   630,   631,   632,   633,   634,   635,     0,
     603,   604,     0,     0,     0,     0,     0,     0,     0,   636,
     637,   605,   606,   607,   608,   609,   953,     0,   610,   611,
     612,   613,     0,   614,   615,   616,   617,   618,     0,   619,
     620,     0,     0,     0,     0,   621,   622,   623,     0,     0,
       0,   624,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     625,     0,   626,   627,   628,   629,   630,   631,   632,   633,
     634,   635,     0,   603,   604,     0,     0,     0,     0,     0,
       0,     0,   636,   637,   605,   606,   607,   608,   609,  1131,
       0,   610,   611,   612,   613,     0,   614,   615,   616,   617,
     618,     0,   619,   620,     0,     0,     0,     0,   621,   622,
     623,     0,     0,     0,   624,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   802,   625,     0,   626,   627,   628,   629,   630,
     631,   632,   633,   634,   635,     0,     0,     0,     0,   603,
     604,     0,     0,     0,     0,   636,   637,   605,   606,   607,
     608,   609,  1471,     0,   610,   611,   612,   613,     0,   614,
     615,   616,   617,   618,     0,   619,   620,     0,     0,     0,
       0,   621,   622,   623,   228,   229,   230,   624,   232,   233,
     234,   235,   236,   432,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,     0,   250,   251,   252,     0,
       0,   255,   256,   257,   258,     0,   625,     0,   626,   627,
     628,   629,   630,   631,   632,   633,   634,   635,     0,     0,
       0,     0,     0,     0,     0,   603,   604,     0,   636,   637,
       0,     0,   638,   605,   606,   607,   608,   609,     0,     0,
     610,   611,   612,   613,     0,   614,   615,   616,   617,   618,
       0,   619,   620,     0,     0,     0,   803,   621,   622,   623,
       0,     0,     0,   624,     0,   804,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   625,     0,   626,   627,   628,   629,   630,   631,
     632,   633,   634,   635,     0,     0,     0,     0,     0,     0,
       0,   603,   604,     0,   636,   637,     0,     0,   658,   605,
     606,   607,   608,   609,     0,     0,   610,   611,   612,   613,
       0,   614,   615,   616,   617,   618,     0,   619,   620,     0,
       0,     0,     0,   621,   622,   623,     0,     0,     0,   624,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   625,     0,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
       0,     0,     0,     0,     0,     0,     0,   603,   604,     0,
     636,   637,     0,     0,   882,   605,   606,   607,   608,   609,
       0,     0,   610,   611,   612,   613,     0,   614,   615,   616,
     617,   618,     0,   619,   620,     0,     0,     0,     0,   621,
     622,   623,     0,     0,     0,   624,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   625,     0,   626,   627,   628,   629,
     630,   631,   632,   633,   634,   635,     0,     0,     0,     0,
       0,     0,     0,   603,   604,     0,   636,   637,     0,     0,
     976,   605,   606,   607,   608,   609,     0,     0,   610,   611,
     612,   613,     0,   614,   615,   616,   617,   618,     0,   619,
     620,     0,     0,     0,     0,   621,   622,   623,     0,     0,
       0,   624,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     625,     0,   626,   627,   628,   629,   630,   631,   632,   633,
     634,   635,     0,     0,     0,     0,     0,     0,     0,   603,
     604,     0,   636,   637,     0,     0,   979,   605,   606,   607,
     608,   609,     0,     0,   610,   611,   612,   613,     0,   614,
     615,   616,   617,   618,     0,   619,   620,     0,     0,     0,
       0,   621,   622,   623,     0,     0,     0,   624,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   625,     0,   626,   627,
     628,   629,   630,   631,   632,   633,   634,   635,     0,     0,
       0,     0,     0,     0,     0,   603,   604,     0,   636,   637,
       0,     0,   981,   605,   606,   607,   608,   609,     0,     0,
     610,   611,   612,   613,     0,   614,   615,   616,   617,   618,
       0,   619,   620,     0,     0,     0,     0,   621,   622,   623,
       0,     0,     0,   624,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   625,     0,   626,   627,   628,   629,   630,   631,
     632,   633,   634,   635,     0,     0,     0,     0,     0,     0,
       0,   603,   604,     0,   636,   637,     0,     0,   989,   605,
     606,   607,   608,   609,     0,     0,   610,   611,   612,   613,
       0,   614,   615,   616,   617,   618,     0,   619,   620,     0,
       0,     0,     0,   621,   622,   623,     0,     0,     0,   624,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   625,     0,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
       0,     0,     0,     0,     0,     0,     0,   603,   604,     0,
     636,   637,     0,     0,   990,   605,   606,   607,   608,   609,
       0,     0,   610,   611,   612,   613,     0,   614,   615,   616,
     617,   618,     0,   619,   620,     0,     0,     0,     0,   621,
     622,   623,     0,     0,     0,   624,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   625,     0,   626,   627,   628,   629,
     630,   631,   632,   633,   634,   635,     0,     0,     0,     0,
       0,     0,     0,   603,   604,     0,   636,   637,     0,     0,
     991,   605,   606,   607,   608,   609,     0,     0,   610,   611,
     612,   613,     0,   614,   615,   616,   617,   618,     0,   619,
     620,     0,     0,     0,     0,   621,   622,   623,     0,     0,
       0,   624,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     625,     0,   626,   627,   628,   629,   630,   631,   632,   633,
     634,   635,     0,     0,     0,     0,     0,     0,     0,   603,
     604,     0,   636,   637,     0,     0,   992,   605,   606,   607,
     608,   609,     0,     0,   610,   611,   612,   613,     0,   614,
     615,   616,   617,   618,     0,   619,   620,     0,     0,     0,
       0,   621,   622,   623,     0,     0,     0,   624,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   625,     0,   626,   627,
     628,   629,   630,   631,   632,   633,   634,   635,     0,     0,
       0,     0,     0,     0,     0,   603,   604,     0,   636,   637,
       0,     0,   993,   605,   606,   607,   608,   609,     0,     0,
     610,   611,   612,   613,     0,   614,   615,   616,   617,   618,
       0,   619,   620,     0,     0,     0,     0,   621,   622,   623,
       0,     0,     0,   624,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   625,     0,   626,   627,   628,   629,   630,   631,
     632,   633,   634,   635,     0,     0,     0,     0,     0,     0,
       0,   603,   604,     0,   636,   637,     0,     0,   994,   605,
     606,   607,   608,   609,     0,     0,   610,   611,   612,   613,
       0,   614,   615,   616,   617,   618,     0,   619,   620,     0,
       0,     0,     0,   621,   622,   623,     0,     0,     0,   624,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   625,     0,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
       0,     0,     0,     0,     0,     0,     0,   603,   604,     0,
     636,   637,     0,     0,  1126,   605,   606,   607,   608,   609,
       0,     0,   610,   611,   612,   613,     0,   614,   615,   616,
     617,   618,     0,   619,   620,     0,     0,     0,     0,   621,
     622,   623,     0,     0,     0,   624,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   625,     0,   626,   627,   628,   629,
     630,   631,   632,   633,   634,   635,     0,     0,     0,     0,
       0,     0,     0,   603,   604,     0,   636,   637,     0,     0,
    1141,   605,   606,   607,   608,   609,     0,     0,   610,   611,
     612,   613,     0,   614,   615,   616,   617,   618,     0,   619,
     620,     0,     0,     0,     0,   621,   622,   623,     0,     0,
       0,   624,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     625,     0,   626,   627,   628,   629,   630,   631,   632,   633,
     634,   635,     0,     0,     0,     0,     0,     0,     0,   603,
     604,     0,   636,   637,     0,     0,  1215,   605,   606,   607,
     608,   609,     0,     0,   610,   611,   612,   613,     0,   614,
     615,   616,   617,   618,     0,   619,   620,     0,     0,     0,
       0,   621,   622,   623,     0,     0,     0,   624,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   625,     0,   626,   627,
     628,   629,   630,   631,   632,   633,   634,   635,     0,     0,
       0,     0,     0,     0,     0,   603,   604,     0,   636,   637,
       0,     0,  1267,   605,   606,   607,   608,   609,     0,     0,
     610,   611,   612,   613,     0,   614,   615,   616,   617,   618,
       0,   619,   620,     0,     0,     0,     0,   621,   622,   623,
       0,     0,     0,   624,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   625,     0,   626,   627,   628,   629,   630,   631,
     632,   633,   634,   635,     0,     0,     0,     0,     0,     0,
       0,   603,   604,     0,   636,   637,     0,     0,  1268,   605,
     606,   607,   608,   609,     0,     0,   610,   611,   612,   613,
       0,   614,   615,   616,   617,   618,     0,   619,   620,     0,
       0,     0,     0,   621,   622,   623,     0,     0,     0,   624,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   625,     0,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
       0,     0,     0,     0,     0,     0,     0,   603,   604,     0,
     636,   637,     0,     0,  1272,   605,   606,   607,   608,   609,
       0,     0,   610,   611,   612,   613,     0,   614,   615,   616,
     617,   618,     0,   619,   620,     0,     0,     0,     0,   621,
     622,   623,     0,     0,     0,   624,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   625,     0,   626,   627,   628,   629,
     630,   631,   632,   633,   634,   635,     0,     0,     0,     0,
       0,     0,     0,   603,   604,     0,   636,   637,     0,     0,
    1288,   605,   606,   607,   608,   609,     0,     0,   610,   611,
     612,   613,     0,   614,   615,   616,   617,   618,     0,   619,
     620,     0,     0,     0,     0,   621,   622,   623,     0,     0,
       0,   624,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     625,     0,   626,   627,   628,   629,   630,   631,   632,   633,
     634,   635,     0,     0,     0,     0,     0,     0,     0,   603,
     604,     0,   636,   637,     0,     0,  1290,   605,   606,   607,
     608,   609,     0,     0,   610,   611,   612,   613,     0,   614,
     615,   616,   617,   618,     0,   619,   620,     0,     0,     0,
       0,   621,   622,   623,     0,     0,     0,   624,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   625,     0,   626,   627,
     628,   629,   630,   631,   632,   633,   634,   635,     0,     0,
       0,     0,     0,     0,     0,   603,   604,     0,   636,   637,
       0,     0,  1292,   605,   606,   607,   608,   609,     0,     0,
     610,   611,   612,   613,     0,   614,   615,   616,   617,   618,
       0,   619,   620,     0,     0,     0,     0,   621,   622,   623,
       0,     0,     0,   624,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   625,     0,   626,   627,   628,   629,   630,   631,
     632,   633,   634,   635,     0,     0,     0,     0,     0,     0,
       0,   603,   604,     0,   636,   637,     0,     0,  1296,   605,
     606,   607,   608,   609,     0,     0,   610,   611,   612,   613,
       0,   614,   615,   616,   617,   618,     0,   619,   620,     0,
       0,     0,     0,   621,   622,   623,     0,     0,     0,   624,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   625,     0,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
       0,     0,     0,     0,     0,     0,     0,   603,   604,     0,
     636,   637,     0,     0,  1303,   605,   606,   607,   608,   609,
       0,     0,   610,   611,   612,   613,     0,   614,   615,   616,
     617,   618,     0,   619,   620,     0,     0,     0,     0,   621,
     622,   623,     0,     0,     0,   624,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   625,     0,   626,   627,   628,   629,
     630,   631,   632,   633,   634,   635,     0,     0,     0,     0,
       0,     0,     0,   603,   604,     0,   636,   637,     0,     0,
    1304,   605,   606,   607,   608,   609,     0,     0,   610,   611,
     612,   613,     0,   614,   615,   616,   617,   618,     0,   619,
     620,     0,     0,     0,     0,   621,   622,   623,     0,     0,
       0,   624,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     625,     0,   626,   627,   628,   629,   630,   631,   632,   633,
     634,   635,     0,     0,     0,     0,     0,     0,     0,   603,
     604,     0,   636,   637,     0,     0,  1306,   605,   606,   607,
     608,   609,     0,     0,   610,   611,   612,   613,     0,   614,
     615,   616,   617,   618,     0,   619,   620,     0,     0,     0,
       0,   621,   622,   623,     0,     0,     0,   624,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   625,     0,   626,   627,
     628,   629,   630,   631,   632,   633,   634,   635,     0,     0,
       0,     0,     0,     0,     0,   603,   604,     0,   636,   637,
       0,     0,  1307,   605,   606,   607,   608,   609,     0,     0,
     610,   611,   612,   613,     0,   614,   615,   616,   617,   618,
       0,   619,   620,     0,     0,     0,     0,   621,   622,   623,
       0,     0,     0,   624,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   625,     0,   626,   627,   628,   629,   630,   631,
     632,   633,   634,   635,     0,     0,     0,     0,     0,     0,
       0,   603,   604,     0,   636,   637,     0,     0,  1363,   605,
     606,   607,   608,   609,     0,     0,   610,   611,   612,   613,
       0,   614,   615,   616,   617,   618,     0,   619,   620,     0,
       0,     0,     0,   621,   622,   623,     0,     0,     0,   624,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   625,     0,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
       0,     0,     0,     0,     0,     0,     0,   603,   604,     0,
     636,   637,     0,     0,  1364,   605,   606,   607,   608,   609,
       0,     0,   610,   611,   612,   613,     0,   614,   615,   616,
     617,   618,     0,   619,   620,     0,     0,     0,     0,   621,
     622,   623,     0,     0,     0,   624,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   625,     0,   626,   627,   628,   629,
     630,   631,   632,   633,   634,   635,     0,     0,     0,     0,
       0,     0,     0,   603,   604,     0,   636,   637,     0,     0,
    1365,   605,   606,   607,   608,   609,     0,     0,   610,   611,
     612,   613,     0,   614,   615,   616,   617,   618,     0,   619,
     620,     0,     0,     0,     0,   621,   622,   623,     0,     0,
       0,   624,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     625,     0,   626,   627,   628,   629,   630,   631,   632,   633,
     634,   635,     0,     0,     0,     0,     0,     0,     0,   603,
     604,     0,   636,   637,     0,     0,  1382,   605,   606,   607,
     608,   609,     0,     0,   610,   611,   612,   613,     0,   614,
     615,   616,   617,   618,     0,   619,   620,     0,     0,     0,
       0,   621,   622,   623,     0,     0,     0,   624,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   625,     0,   626,   627,
     628,   629,   630,   631,   632,   633,   634,   635,     0,     0,
       0,     0,     0,     0,     0,   603,   604,     0,   636,   637,
       0,     0,  1393,   605,   606,   607,   608,   609,     0,     0,
     610,   611,   612,   613,     0,   614,   615,   616,   617,   618,
       0,   619,   620,     0,     0,     0,     0,   621,   622,   623,
       0,     0,     0,   624,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   625,     0,   626,   627,   628,   629,   630,   631,
     632,   633,   634,   635,     0,     0,     0,     0,     0,     0,
       0,   603,   604,     0,   636,   637,     0,     0,  1402,   605,
     606,   607,   608,   609,     0,     0,   610,   611,   612,   613,
       0,   614,   615,   616,   617,   618,     0,   619,   620,     0,
       0,     0,     0,   621,   622,   623,     0,     0,     0,   624,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   625,     0,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
       0,     0,     0,     0,     0,     0,     0,   603,   604,     0,
     636,   637,     0,     0,  1465,   605,   606,   607,   608,   609,
       0,     0,   610,   611,   612,   613,     0,   614,   615,   616,
     617,   618,     0,   619,   620,     0,     0,     0,     0,   621,
     622,   623,     0,     0,     0,   624,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   625,     0,   626,   627,   628,   629,
     630,   631,   632,   633,   634,   635,     0,     0,     0,     0,
       0,   603,   604,     0,     0,     0,   636,   637,     0,     0,
    1497,   605,   606,   607,   608,   609,     0,     0,   610,   611,
     612,   613,     0,   614,   615,   616,   617,   618,     0,   619,
     620,     0,     0,     0,     0,   621,   622,   623,     0,     0,
       0,   624,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     625,     0,   626,   627,   628,   629,   630,   631,   632,   633,
     634,   635,     0,     0,     0,   603,   604,     0,     0,     0,
       0,     0,   636,   637,   663,   605,   606,   607,   608,   609,
       0,     0,   610,   611,   612,   613,     0,   614,   615,   616,
     617,   618,     0,   619,   620,     0,     0,     0,     0,   621,
     622,   623,     0,     0,     0,   624,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   625,     0,   626,   627,   628,   629,
     630,   631,   632,   633,   634,   635,     0,     0,     0,   603,
     604,     0,     0,     0,     0,     0,   636,   637,   873,   605,
     606,   607,   608,   609,     0,     0,   610,   611,   612,   613,
       0,   614,   615,   616,   617,   618,     0,   619,   620,     0,
       0,     0,     0,   621,   622,   623,     0,     0,     0,   624,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   625,     0,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
       0,     0,     0,   603,   604,     0,     0,     0,     0,     0,
     636,   637,  1030,   605,   606,   607,   608,   609,     0,     0,
     610,   611,   612,   613,     0,   614,   615,   616,   617,   618,
       0,   619,   620,     0,     0,     0,     0,   621,   622,   623,
       0,     0,     0,   624,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   625,     0,   626,   627,   628,   629,   630,   631,
     632,   633,   634,   635,     0,     0,     0,   603,   604,     0,
       0,     0,     0,     0,   636,   637,  1046,   605,   606,   607,
     608,   609,     0,     0,   610,   611,   612,   613,     0,   614,
     615,   616,   617,   618,     0,   619,   620,     0,     0,     0,
       0,   621,   622,   623,     0,     0,     0,   624,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   625,     0,   626,   627,
     628,   629,   630,   631,   632,   633,   634,   635,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   636,   637,
    1198,   605,   606,   607,   608,   609,   276,   277,   610,   611,
     612,   613,     0,   614,   615,   616,   617,   618,     0,   619,
     620,     0,     0,   278,     0,   621,   622,   623,     0,     0,
       0,   624,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     625,     0,   626,   627,   628,   629,   630,   631,   632,   633,
     634,   635,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   636,   637,  1203,     0,     0,     0,     0,   603,
     604,     0,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
       0,     0,   297,   298,   299,     0,     0,   300,   301,   302,
     303,   304,     0,     0,   305,   306,   307,   308,   309,   310,
     311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   806,   312,
       0,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,     0,     0,   323,   324,   603,   604,     0,     0,     0,
       0,   325,   326,   605,   606,   607,   608,   609,     0,     0,
     610,   611,   612,   613,     0,   614,   615,   616,   617,   618,
       0,   619,   620,     0,     0,   786,     0,   621,   622,   623,
     228,   229,   230,   624,   232,   233,   234,   235,   236,   432,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,     0,   250,   251,   252,     0,     0,   255,   256,   257,
     258,     0,   625,     0,   626,   627,   628,   629,   630,   631,
     632,   633,   634,   635,     0,     0,     0,     0,   603,   604,
       0,     0,     0,     0,   636,   637,     0,     0,     0,   605,
     606,   607,   608,   609,     0,     0,   610,   611,   612,   613,
       0,   614,   615,   616,   617,   618,     0,   619,   620,     0,
       0,     0,   807,   621,   622,   623,     0,     0,     0,   624,
       0,   808,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   625,  1035,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
       0,   603,   604,     0,     0,     0,     0,     0,     0,     0,
     636,   637,   605,   606,   607,   608,   609,     0,     0,   610,
     611,   612,   613,     0,   614,   615,   616,   617,   618,     0,
     619,   620,     0,     0,     0,     0,   621,   622,   623,     0,
       0,     0,   624,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1162,     0,     0,
       0,   625,     0,   626,   627,   628,   629,   630,   631,   632,
     633,   634,   635,     0,   603,   604,     0,     0,     0,     0,
       0,     0,     0,   636,   637,   605,   606,   607,   608,   609,
       0,     0,   610,   611,   612,   613,     0,   614,   615,   616,
     617,   618,     0,   619,   620,   603,   604,     0,     0,   621,
     622,   623,     0,     0,     0,   624,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   625,     0,   626,   627,   628,   629,
     630,   631,   632,   633,   634,   635,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   636,   637,   605,   606,
     607,   608,   609,     0,     0,   610,   611,   612,   613,     0,
     614,   615,   616,   617,   618,     0,   619,   620,   603,   604,
       0,     0,   621,   622,   623,     0,     0,     0,  -731,   605,
     606,   607,   608,   609,     0,     0,   610,   611,   612,   613,
       0,   614,   615,   616,   617,   618,     0,   619,   620,   603,
     604,     0,     0,   621,   622,   623,     0,   625,     0,   626,
     627,   628,   629,   630,   631,   632,   633,   634,   635,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   636,
     637,     0,     0,     0,     0,     0,     0,     0,   625,     0,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     636,   637,   605,   606,   607,   608,   609,     0,     0,   610,
     611,   612,   613,     0,   614,   615,   616,   617,   618,     0,
     619,   620,   603,   604,     0,     0,   621,     0,   623,     0,
       0,     0,     0,   605,   606,   607,   608,   609,     0,     0,
     610,   611,   612,   613,     0,   614,   615,   616,   617,   618,
       0,   619,   620,   603,   604,     0,     0,   621,     0,     0,
       0,     0,     0,   626,   627,   628,   629,   630,   631,   632,
     633,   634,   635,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   636,   637,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   626,   627,   628,   629,   630,   631,
     632,   633,   634,   635,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   636,   637,   605,   606,   607,   608,
     609,     0,     0,   610,   611,   612,   613,     0,   614,   615,
     616,   617,   618,     0,   619,   620,   603,   604,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   605,   606,   607,
     608,   609,     0,     0,   610,   611,   612,   613,     0,   614,
     615,   616,   617,   618,     0,   619,   620,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   626,   627,   628,
     629,   630,   631,   632,   633,   634,   635,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   636,   637,  1031,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   627,
     628,   629,   630,   631,   632,   633,   634,   635,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   636,   637,
     605,   606,   607,   608,   609,     0,     0,   610,   611,   612,
     613,     0,   614,   615,   616,   617,   618,     0,   619,   620,
       0,   228,   229,   230,     0,   232,   233,   234,   235,   236,
     432,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,     0,   250,   251,   252,     0,     0,   255,   256,
     257,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   628,   629,   630,   631,   632,   633,   634,
     635,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   636,   637,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1032,     0,     0,     0,     0,     0,   228,
     229,   230,  1033,   232,   233,   234,   235,   236,   432,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
       0,   250,   251,   252,     0,     0,   255,   256,   257,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   842,   843,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   844,     0,     0,     0,     0,     0,     0,     0,     0,
     845,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   846,   847
};

static const yytype_int16 yycheck[] =
{
      13,    14,   597,   137,   533,   536,   600,   787,   667,   676,
     681,   516,   167,   153,   494,   646,   496,   648,   498,   650,
     517,   653,   921,   527,   775,   657,   591,    79,   801,   742,
       8,   596,    20,    33,   785,  1210,    20,     0,     5,    20,
      20,   471,  1345,    46,     7,   172,  1258,    60,    61,    62,
      19,    20,   462,   147,   126,   126,    33,   147,  1377,   167,
     132,   132,    22,   155,    57,   128,   129,    30,   162,    32,
      63,    34,   162,   200,   150,   372,   789,    40,     5,     6,
     126,    15,    16,   139,   140,   141,   150,    50,   101,   102,
     103,   104,   164,    56,  1397,   173,   661,    57,    25,   126,
     530,   173,   173,   106,    31,   199,  1425,  1381,  1320,  1321,
     204,   408,   153,   203,   126,   205,  1390,    80,   162,   751,
     132,   162,    15,    16,   200,  1337,   202,   173,   203,   164,
     176,   203,   203,   196,   197,   102,   103,     7,   202,   102,
     103,    68,    69,   172,   200,   172,   173,   742,   175,   559,
     162,   178,  1327,   138,   167,   147,   203,   201,   199,  1433,
    1434,   173,   214,   138,   203,   669,   178,   671,   203,   198,
     162,   601,  1384,  1385,   172,   102,   103,   162,   178,   125,
      50,   685,   128,   129,   172,   750,   172,   752,   172,   693,
     694,   172,   172,   162,   789,   760,   727,   792,   763,   339,
     175,   178,   706,    21,    22,   709,   710,   711,   712,   713,
     147,   138,   179,   717,   200,   988,   875,   372,    62,   197,
     197,   155,   185,   175,   184,   162,   160,   205,   162,   206,
     901,   165,   199,   388,   197,   162,   196,   869,   126,   126,
     203,   126,  1444,   164,   132,   132,   997,   132,   961,   262,
     196,   197,   179,   408,   126,   153,   411,   412,   413,   164,
     132,   558,   155,   172,   162,   199,   138,   160,    34,   162,
     197,  1473,   165,   126,   138,   572,   126,   204,   199,   132,
     388,   163,   132,   147,   943,   173,   173,   162,   173,   341,
     172,   200,   172,   459,   199,   172,   800,    63,   162,   117,
     118,   173,   200,   411,   412,   413,   203,   125,   138,   127,
     128,   129,   130,   131,    60,    61,    62,   147,   798,   162,
     173,   198,   152,   173,   172,   172,   162,   162,   164,  1228,
      21,    22,   162,   965,   489,   490,   153,   503,   493,  1090,
     495,   172,   497,    47,   499,   162,   138,   180,    57,   179,
     185,   172,   200,   200,    63,   101,   102,   103,   104,   372,
     865,   516,    57,    67,   155,   131,   961,   203,    63,   200,
     203,   868,   138,   878,   878,   388,   880,   198,   196,   197,
      33,   489,   490,   880,   172,   493,   199,   495,   401,   497,
    1021,   557,    21,    22,   196,   408,   162,   172,   411,   412,
     413,   567,   162,   558,   570,   418,   419,    60,    61,    12,
    1081,  1078,   200,   163,   164,   163,   164,   572,   162,   950,
      23,    24,   172,   189,   172,   200,   117,   118,   176,   726,
     180,   597,   200,   199,   125,   203,   127,   128,   129,   130,
     131,   172,   739,   740,     5,     6,   950,     8,   603,   604,
      57,   179,   749,   203,   162,   203,    63,   754,   755,  1118,
     757,   162,   759,   618,   761,   762,    57,   764,   162,   200,
     123,   199,    63,   639,   127,    36,   489,   490,   163,   164,
     493,   636,   495,   172,   497,   172,   499,   172,   117,   118,
     181,   182,   183,   184,   185,   180,   125,   179,   172,   128,
     129,   130,   131,   516,   659,   196,   197,   172,   172,    57,
    1177,   200,   172,   200,   172,    63,   175,   199,   203,   178,
     172,   174,   163,   164,  1004,   178,   200,   162,   181,   163,
     164,   172,   163,   164,   162,   200,   200,  1168,   172,   180,
     200,   172,   200,  1023,   197,   558,   180,   153,   200,   180,
     139,   659,   141,   206,   688,  1186,   162,   179,   179,   572,
      57,   179,   203,   162,   163,   175,    63,   196,   197,   203,
     180,   726,   203,   172,    10,    11,   742,   199,   199,   592,
    1175,   199,   722,   738,   739,   740,   741,   175,   175,   199,
     745,  1262,   180,   180,   749,   166,   167,    75,   166,   754,
     755,    79,   757,   175,   759,  1272,   761,   762,   180,   764,
     153,   199,   199,   176,  1285,    93,    94,   180,  1398,   162,
      98,    99,   100,   101,   162,   163,  1130,   199,   204,   153,
     738,   153,    57,   741,   172,   164,   172,   745,   162,   175,
     162,   172,   178,   203,   175,   205,   659,   178,   176,   176,
     176,   176,   180,   180,   180,   180,   176,   176,   176,   176,
     180,   180,   180,   180,   176,   176,   349,   173,   180,   180,
     163,   968,   969,   970,   166,   830,   359,   974,   166,   167,
     168,   169,   139,   140,   141,   139,   369,  1252,  1359,   162,
     106,   201,   202,   202,   204,   166,   167,   168,  1369,   164,
    1371,  1372,   166,   167,   168,   163,  1003,    35,   863,    35,
     865,   866,   199,   726,  1308,    52,    53,    54,   162,   175,
     202,   162,   199,   878,   179,   738,   739,   740,   741,   179,
     179,   179,   745,   888,  1238,  1402,   749,   179,   199,   179,
     179,   754,   755,  1338,   757,   179,   759,   179,   761,   762,
     179,   764,   179,   436,   437,   863,   199,   176,   866,   204,
     162,   162,   162,   198,    22,   162,   204,   204,   198,   175,
     453,   454,   455,   456,   457,   162,   204,   175,   204,   462,
     888,  1310,   197,  1454,   162,   132,   199,   204,  1459,   162,
     179,  1386,   199,   179,   179,   179,   179,   179,   179,   179,
     199,   162,   179,   199,   199,   199,  1477,   199,    21,    22,
     162,   199,   202,   968,   969,   970,   162,    43,   180,   974,
     199,   179,   197,   199,  1389,   199,   179,   199,  1499,   512,
     200,  1426,   198,    13,   199,   199,   179,   520,   172,   199,
     199,   172,     4,   162,   527,  1376,   202,    43,  1003,   200,
     863,   175,   865,   866,   199,   162,   155,   540,    10,    13,
       9,    42,  1028,  1428,  1368,   878,   205,    66,   162,  1373,
       8,   162,   162,   162,  1040,   888,   559,   560,   180,  1045,
     563,   199,   565,   198,   200,   199,   199,   199,   205,  1484,
    1187,  1422,   575,   576,   577,   578,   579,   580,   172,   200,
     179,   199,   115,   116,   117,   118,   119,   199,   179,   122,
       1,  1066,   125,   199,   127,   128,   129,   130,   131,   199,
     133,   134,   605,   606,   199,   162,   609,   610,   611,   612,
     199,   614,   199,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,   628,   629,   630,   631,   632,
     633,   634,   635,  1457,   637,   968,   969,   970,  1066,   199,
     180,   974,   180,   180,   162,   162,   179,   180,   181,   182,
     183,   184,   185,    14,   175,   199,  1480,   155,    21,    22,
     173,    37,   173,   196,   197,   199,    67,   199,   199,   199,
    1003,    43,   180,   180,   203,  1161,   679,  1163,   200,   682,
     200,   180,   205,  1169,   687,   200,   689,   199,   180,   180,
     200,   180,   180,   180,   200,   180,   200,    21,    22,   180,
     179,    43,   199,   205,   200,   199,  1160,   162,   162,   199,
     205,   162,  1187,   199,  1189,   199,   180,   200,   180,   199,
     723,   180,   200,   180,  1210,   162,   729,   180,   199,   162,
     162,  1185,   199,  1066,   162,   162,   204,    33,   199,   205,
     180,   744,    70,   199,   180,   162,   137,   199,    12,   180,
     200,   199,   199,   199,   117,   118,   199,   162,  1375,   200,
     199,  1189,   125,   205,   127,   128,   129,   130,   131,   199,
     773,   200,   172,   199,   777,   199,    53,   200,   203,   200,
    1266,   203,   200,   786,   787,   200,   199,   203,   200,   203,
     200,   115,   116,   117,   118,   203,   200,   800,   200,   205,
     199,   125,   200,   127,   128,   129,   130,   131,   200,   133,
     134,   200,   198,   198,   581,    78,     1,    42,  1293,  1305,
     183,   184,   185,   128,    81,  1500,  1367,   220,   671,   100,
     671,  1330,  1286,   196,   197,  1367,  1367,  1367,  1367,     1,
     843,  1327,  1283,  1233,   847,  1317,  1122,   478,  1334,  1236,
    1304,   525,  1306,  1307,  1187,  1318,  1189,   181,   182,   183,
     184,   185,    51,  1118,   943,  1293,   943,   418,   418,   334,
    1386,    -1,   196,   197,    -1,    -1,    -1,    -1,    -1,    -1,
     883,    -1,    -1,    -1,    -1,    -1,    -1,   890,   891,    -1,
      -1,   894,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   902,
    1375,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     923,   924,   925,   926,   927,   928,   929,   930,   931,   932,
     933,   934,   935,   936,   937,   938,   939,   940,    -1,    71,
      -1,    73,    -1,    75,    76,    77,    78,    79,   951,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
    1293,    93,    94,    95,  1418,    -1,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1449,    -1,    -1,  1001,  1002,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1012,
    1013,  1014,  1345,  1016,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1025,    -1,  1027,    -1,  1029,    21,    22,    -1,
     162,    -1,  1035,    -1,    -1,    -1,  1039,    -1,    -1,    -1,
      -1,    -1,  1375,  1497,  1047,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,  1397,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1083,    -1,    -1,    -1,    -1,  1088,  1089,    -1,  1091,  1092,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1117,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,   117,   118,   119,  1129,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,  1148,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,    -1,    -1,   139,   140,
     141,   142,   143,   144,   145,   179,   180,   181,   182,   183,
     184,   185,  1195,    -1,  1197,    -1,    -1,    -1,    -1,  1202,
      -1,    -1,   196,   197,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,   174,  1217,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,   196,   197,    31,    -1,    -1,
      -1,  1244,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,  1254,  1255,  1256,    -1,    49,    -1,    -1,  1261,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,  1275,  1276,  1277,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1330,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,  1348,  1349,    -1,    -1,    -1,
      -1,    -1,    -1,  1356,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,  1367,    -1,    -1,    -1,   162,    -1,
    1373,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1394,    -1,    -1,    -1,  1398,    -1,    -1,    -1,    -1,
      -1,  1404,    -1,    -1,    -1,    -1,    -1,   201,    -1,    -1,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1423,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1442,
      -1,    -1,  1445,  1446,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,  1456,    -1,    -1,     5,     6,     7,    -1,
       9,    10,    11,    -1,    13,    -1,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,  1478,    25,    26,    27,    28,
      29,    -1,    31,    -1,  1487,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    42,    -1,    44,    45,    -1,    -1,    48,
      -1,    50,    51,    52,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,    -1,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,   188,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,
     199,    -1,   201,   202,   203,   204,   205,     1,    -1,    -1,
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
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,   149,   150,    -1,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,   182,   183,
      -1,   185,    -1,    -1,   188,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,    -1,   199,    -1,   201,   202,   203,
     204,   205,     1,    -1,    -1,    -1,     5,     6,     7,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,    -1,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,   188,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,
     199,    -1,   201,   202,   203,   204,   205,     5,     6,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    52,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,    -1,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    70,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,   200,   201,   202,    13,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,    -1,   201,   202,    13,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,    -1,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    70,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,    -1,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
     198,   199,    -1,   201,   202,    13,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,    -1,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
     198,   199,    -1,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    22,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,    -1,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,   200,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,    -1,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,    -1,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,    -1,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,    -1,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,   200,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,   200,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,   200,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,   200,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,   200,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,    -1,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,     5,     6,   197,
      -1,   199,    -1,   201,   202,    -1,   204,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    19,   165,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    41,    -1,
     188,   189,    -1,    -1,    -1,    -1,    49,    -1,    -1,   197,
      -1,   199,    -1,   201,   202,    -1,   204,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,   162,
      -1,   164,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,   201,    -1,
     203,    -1,    19,    -1,    -1,    21,    22,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,   162,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,   201,    -1,    21,    22,    -1,    -1,    -1,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,   138,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
     196,   197,    -1,    -1,   201,    -1,    -1,   203,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,
     197,   115,   116,   117,   118,   119,   203,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,   115,   116,   117,   118,   119,   203,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,   174,    -1,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,   196,   197,   115,   116,   117,
     118,   119,   203,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    71,    72,    73,   145,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   196,   197,
      -1,    -1,   200,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,   153,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,   196,   197,    -1,    -1,   200,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
     196,   197,    -1,    -1,   200,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,   196,   197,    -1,    -1,
     200,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,   196,   197,    -1,    -1,   200,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   196,   197,
      -1,    -1,   200,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,   196,   197,    -1,    -1,   200,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
     196,   197,    -1,    -1,   200,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,   196,   197,    -1,    -1,
     200,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,   196,   197,    -1,    -1,   200,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   196,   197,
      -1,    -1,   200,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,   196,   197,    -1,    -1,   200,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
     196,   197,    -1,    -1,   200,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,   196,   197,    -1,    -1,
     200,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,   196,   197,    -1,    -1,   200,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   196,   197,
      -1,    -1,   200,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,   196,   197,    -1,    -1,   200,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
     196,   197,    -1,    -1,   200,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,   196,   197,    -1,    -1,
     200,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,   196,   197,    -1,    -1,   200,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   196,   197,
      -1,    -1,   200,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,   196,   197,    -1,    -1,   200,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
     196,   197,    -1,    -1,   200,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,   196,   197,    -1,    -1,
     200,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,   196,   197,    -1,    -1,   200,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   196,   197,
      -1,    -1,   200,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,   196,   197,    -1,    -1,   200,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
     196,   197,    -1,    -1,   200,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,   196,   197,    -1,    -1,
     200,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,   196,   197,    -1,    -1,   200,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   196,   197,
      -1,    -1,   200,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,   196,   197,    -1,    -1,   200,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
     196,   197,    -1,    -1,   200,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,   196,   197,    -1,    -1,
     200,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,   196,   197,   198,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    -1,   196,   197,   198,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
     196,   197,   198,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,   196,   197,   198,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,
     198,   115,   116,   117,   118,   119,    21,    22,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    38,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,   198,    -1,    -1,    -1,    -1,    21,
      22,    -1,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      -1,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134,
     135,   136,    -1,    -1,   139,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,   174,
      -1,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,   188,   189,    21,    22,    -1,    -1,    -1,
      -1,   196,   197,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,   137,    -1,   139,   140,   141,
      71,    72,    73,   145,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,   196,   197,    -1,    -1,    -1,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,   153,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     196,   197,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,   174,    -1,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   196,   197,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    21,    22,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    21,    22,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    21,
      22,    -1,    -1,   139,   140,   141,    -1,   174,    -1,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     196,   197,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    21,    22,    -1,    -1,   139,    -1,   141,    -1,
      -1,    -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    21,    22,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   196,   197,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   196,   197,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   196,   197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,   162,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   128,   129,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   196,   197
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   208,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   185,   197,   203,   211,   214,   220,   222,
     223,   227,   262,   266,   295,   373,   381,   388,   398,   444,
     449,   454,    19,    20,   162,   254,   255,   256,   155,   228,
     229,   162,   185,   224,   225,    57,    63,   378,   379,   162,
     201,   213,   455,   445,   450,   138,   162,   283,    34,    63,
     131,   189,   199,   258,   259,   260,   261,   283,   203,   203,
     203,     5,     6,     8,    36,   395,    62,   369,   173,   172,
     175,   172,   224,    22,    57,   184,   196,   226,   162,   203,
     369,   378,   378,   378,   162,   138,   221,   260,   260,   260,
     199,   139,   140,   141,   172,   198,    57,    63,   267,   269,
     389,    57,    63,   396,    57,    63,   370,    15,    16,   155,
     160,   162,   165,   199,   216,   255,   155,   229,   162,   162,
     162,   380,    57,    63,   212,   162,   162,   162,   162,   166,
     219,   200,   256,   260,   260,   260,   260,   270,    57,    63,
     382,   399,   204,   374,   166,   167,   168,   215,    15,    16,
     155,   160,   162,   216,   252,   253,   226,   173,   163,   386,
     387,   164,   233,   234,   234,   166,   200,    35,    71,    73,
      75,    76,    77,    78,    79,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    93,    94,    95,    98,
      99,   100,   101,   117,   118,   162,   265,   268,   162,   383,
     106,   393,   394,   371,   202,   257,   342,   166,   167,   168,
     172,   200,    19,    25,    31,    41,    49,    64,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   151,   201,   283,   402,   404,   405,   408,   414,   415,
     443,   163,   456,   164,   446,   451,    21,    22,    38,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   127,   128,   129,
     132,   133,   134,   135,   136,   139,   140,   141,   142,   143,
     144,   145,   174,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   188,   189,   196,   197,    35,    35,   199,
     263,   274,   275,   175,   384,   162,   400,   203,   205,   372,
     255,   202,   162,   365,   368,   252,   179,   179,   179,   199,
     179,   179,   199,   179,   179,   179,   179,   179,   179,   199,
     283,    33,    60,    61,   123,   127,   174,   178,   181,   197,
     206,   413,   176,   204,   204,   204,   162,   162,   162,   198,
      22,   162,   198,   150,   200,   342,   352,   353,   175,   264,
     204,    75,    79,    93,    94,    98,    99,   100,   101,   403,
     387,   175,   392,   204,   397,   342,   255,   172,   175,   178,
     363,   416,   421,   423,     5,     6,    15,    16,    17,    18,
      19,    25,    27,    31,    39,    45,    48,    51,    55,    65,
      68,    69,    80,   102,   103,   104,   117,   118,   146,   147,
     148,   149,   150,   152,   154,   155,   156,   157,   158,   159,
     160,   161,   165,   181,   182,   183,   188,   189,   197,   199,
     201,   202,   204,   218,   220,   277,   283,   288,   300,   307,
     310,   313,   317,   319,   321,   322,   324,   329,   332,   333,
     340,   341,   402,   459,   467,   477,   480,   492,   495,   425,
     419,   162,   409,   427,   429,   431,   433,   435,   437,   439,
     441,   333,   179,   199,    33,   178,    33,   178,   197,   206,
     198,   333,   197,   206,   414,   457,   447,   452,   162,   132,
     199,     7,    50,   294,   200,   203,   443,   279,   204,   283,
     348,   147,   162,   364,   367,   162,   414,   126,   132,   173,
     362,   443,   443,   412,   443,   179,   179,   179,   285,   404,
     459,   283,   179,     5,   102,   103,   179,   199,   179,   199,
     199,   179,   179,   199,   179,   199,   179,   199,   179,   179,
     199,   179,   179,   333,   333,   199,   199,   199,   199,   199,
     199,   217,   333,   333,   333,   333,   333,    13,    49,   311,
     333,   311,   202,    13,   333,   476,   491,   199,   262,   202,
     204,   313,   318,    21,    22,   115,   116,   117,   118,   119,
     122,   123,   124,   125,   127,   128,   129,   130,   131,   133,
     134,   139,   140,   141,   145,   174,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   196,   197,   200,   199,
     443,   443,   200,   162,   406,   443,   263,   443,   263,   443,
     263,   162,   354,   355,   443,   162,   357,   358,   200,   418,
     280,   311,   198,   198,   198,   333,   162,   407,   354,   356,
     357,   359,   333,   147,   162,   361,   401,   352,     1,    26,
      28,    29,    38,    44,    52,    54,    58,    59,    65,   105,
     164,   203,   210,   230,   231,   238,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   276,   278,   284,
     289,   290,   291,   292,   293,   295,   299,   320,   333,   390,
     205,   262,   349,   199,    43,   172,   175,   363,   203,   362,
     333,   180,   180,   180,   163,   172,   209,   210,   411,   468,
     470,   286,   199,   179,   199,   308,   179,   179,   179,   487,
     311,   414,   491,   333,   301,   303,   333,   305,   333,   489,
     311,   474,   478,   311,   472,   414,   333,   333,   333,   333,
     333,   333,   168,   169,   215,   199,    13,   172,   494,   494,
     147,   152,   179,   283,   323,   199,   137,   172,   494,    70,
     153,   162,   197,   200,   311,   460,   462,     4,   316,   202,
     279,   262,    19,   153,   162,   402,    19,   153,   162,   402,
     333,   333,   333,   333,   333,   333,   162,   333,   153,   162,
     333,   333,   333,   402,   333,   333,   333,   333,   333,   333,
      22,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   128,   129,   153,   162,   196,   197,   330,   402,
     333,   200,   311,   180,   180,   180,   203,   180,   264,   180,
     264,   180,   264,   175,   180,   411,   175,   180,   411,   282,
     443,   200,   494,   198,   209,   376,   385,   210,   239,   240,
     239,   240,   200,   199,    43,   126,   172,   173,   175,   178,
     360,   126,   333,   274,    61,   333,   162,   210,   155,    58,
     333,   234,   126,   333,   210,   210,    10,    10,    11,   236,
      13,     9,    42,   210,   205,   210,   210,   210,   210,   210,
      66,   296,   210,   107,   108,   109,   110,   111,   112,   113,
     114,   120,   121,   126,   132,   135,   136,   142,   143,   144,
     173,   162,   375,   377,     8,   342,   347,   333,   162,   162,
     414,   362,   333,   203,   417,   422,   424,   443,   414,   414,
     443,    70,   311,   462,   466,   162,   333,   443,   481,   483,
     485,   414,   494,   180,   411,   494,   200,   414,   414,   200,
     414,   200,   414,   494,   414,   414,   494,   414,   180,   200,
     200,   200,   200,   200,   200,   333,   401,   199,   333,   198,
     200,   199,   199,   325,   327,   401,   333,   476,   205,   162,
     200,   462,   199,   132,   360,   460,   172,   200,   172,   200,
     199,   263,   205,   316,   179,   199,   179,   199,   199,   199,
     198,    19,   153,   162,   402,   175,   153,   162,   333,   199,
     199,   153,   162,   333,     1,   199,   198,   172,   200,   426,
     420,   410,   162,   428,   180,   432,   180,   436,   180,   443,
     440,   354,   443,   442,   357,   180,   411,   162,   209,   458,
     210,   354,   448,   357,   453,   333,   162,   162,   443,   333,
     333,    14,   333,   173,   175,   155,   274,   333,   199,   199,
     199,   199,   199,    37,   271,   199,   204,   298,   367,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   173,   376,   385,
      57,    63,   345,    67,   346,   203,   200,    43,   210,   362,
     333,   203,   180,   180,   180,   462,   200,   200,   200,   180,
     411,   200,   180,   414,   414,   414,   180,   200,   199,   414,
     200,   180,   180,   180,   180,   200,   180,   180,   200,   180,
     316,   199,   170,    20,   401,   333,   333,   414,   263,    20,
     200,   333,   333,   333,   198,   197,   153,   162,   126,   132,
     162,   173,   178,   314,   315,   264,   263,   334,   333,   336,
     333,   200,   311,   333,   179,   199,   333,   199,   198,   333,
     200,   311,   199,   198,   331,   200,   311,   430,   434,   438,
     199,   443,   205,   205,   205,   200,    43,   360,   274,   333,
     333,   333,   401,   333,   333,   272,   162,   297,   366,   333,
     375,   391,    47,   346,    46,   106,   343,   162,   333,   210,
     469,   471,   287,   200,   199,   162,   309,   180,   180,   180,
     488,   281,   491,   180,   302,   304,   306,   490,   475,   479,
     473,   199,   234,   200,   311,   311,    20,   200,   200,   180,
     264,   311,   200,   200,   460,   199,   132,   360,   162,   162,
     199,   162,   162,   172,   200,   234,   264,   414,   200,   443,
     200,   200,   200,   338,   333,   333,   200,   200,   333,   200,
     280,   162,   333,   200,   200,    20,   200,   200,   204,   172,
     200,   203,   205,   367,   205,    33,   344,   343,   345,   210,
     199,   199,   333,   180,   482,   484,   486,   199,   200,   494,
     199,   333,   333,   333,   199,    70,   466,   199,   199,   200,
     333,   274,   200,   200,   311,   326,   180,   200,   132,   360,
     198,   333,   333,   333,   162,   314,   137,   274,   312,   234,
     180,   180,   443,   200,   200,   200,   200,    12,   237,   234,
     311,   234,   234,   279,   162,   175,   363,   350,   344,   361,
     466,   466,   200,   199,   199,   199,   199,   280,   281,   311,
     466,   460,   461,   200,   203,   200,   323,   328,   203,   333,
     333,   200,   200,   200,   126,   333,   274,   335,   337,   180,
     238,   289,   290,   291,   292,   333,   210,   274,   200,   274,
     274,   273,   414,   362,   268,   351,   199,   460,   463,   464,
     465,   465,   333,   466,   466,   461,   200,   200,   494,   465,
     200,   333,   203,   323,   476,   132,   360,   333,   339,    12,
      23,    24,   232,   235,   234,   205,   362,   333,   203,   268,
     460,   172,   494,   200,   200,   200,   465,   465,   200,   200,
     200,   203,   493,   333,   493,   333,   333,   234,   199,   274,
     333,   210,   274,   200,   199,   200,   200,    53,   198,   493,
     205,   274,   333,   210,   460,   333,   198,   200,   200,   234,
     274,   235
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   207,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   209,   209,   210,
     210,   211,   212,   212,   212,   213,   213,   214,   215,   215,
     215,   215,   216,   216,   217,   217,   217,   218,   219,   219,
     221,   220,   222,   223,   224,   224,   224,   224,   225,   225,
     226,   226,   227,   228,   228,   229,   229,   230,   231,   231,
     232,   232,   233,   233,   234,   234,   235,   235,   235,   236,
     236,   237,   237,   238,   238,   238,   238,   238,   239,   239,
     240,   240,   242,   241,   243,   245,   244,   246,   248,   247,
     250,   249,   251,   252,   252,   252,   252,   252,   252,   253,
     253,   254,   254,   254,   255,   255,   255,   255,   255,   255,
     255,   255,   256,   256,   257,   257,   258,   258,   258,   259,
     259,   260,   260,   260,   260,   260,   260,   260,   261,   261,
     262,   262,   263,   263,   263,   264,   264,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   266,   267,   267,   267,   268,   270,   269,   271,
     272,   273,   271,   275,   276,   274,   277,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   279,   279,   279,   280,
     280,   281,   281,   282,   282,   283,   283,   283,   284,   284,
     286,   287,   285,   285,   288,   288,   288,   288,   288,   288,
     289,   290,   291,   291,   291,   292,   292,   293,   294,   294,
     294,   295,   295,   296,   296,   297,   297,   298,   298,   299,
     299,   299,   301,   302,   300,   303,   304,   300,   305,   306,
     300,   308,   309,   307,   310,   310,   310,   311,   311,   312,
     312,   312,   313,   313,   313,   314,   314,   314,   314,   314,
     315,   315,   316,   316,   317,   318,   318,   319,   319,   319,
     319,   319,   319,   319,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   321,   321,   322,   322,   323,   323,   324,
     325,   326,   324,   327,   328,   324,   329,   329,   329,   329,
     329,   329,   330,   331,   329,   332,   332,   332,   332,   332,
     332,   332,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   334,   335,   333,
     333,   333,   333,   336,   337,   333,   333,   333,   338,   339,
     333,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   340,   340,   340,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   342,   342,   343,   343,   343,   344,   344,
     345,   345,   345,   346,   346,   347,   348,   349,   348,   350,
     348,   351,   348,   352,   352,   353,   353,   354,   354,   355,
     355,   356,   356,   356,   357,   358,   358,   359,   359,   359,
     360,   360,   361,   361,   361,   361,   361,   362,   362,   362,
     363,   363,   364,   364,   364,   364,   364,   365,   365,   366,
     366,   366,   367,   367,   367,   368,   368,   368,   369,   369,
     370,   370,   370,   371,   371,   372,   371,   373,   374,   373,
     375,   375,   376,   376,   377,   377,   377,   378,   378,   378,
     380,   379,   381,   382,   382,   382,   383,   384,   384,   385,
     385,   386,   386,   387,   387,   389,   390,   391,   388,   392,
     392,   393,   393,   394,   395,   395,   396,   396,   396,   397,
     397,   399,   400,   398,   401,   401,   401,   401,   401,   402,
     402,   402,   402,   402,   402,   402,   402,   402,   402,   402,
     402,   402,   402,   402,   402,   402,   402,   402,   402,   402,
     402,   402,   402,   402,   402,   402,   403,   403,   403,   403,
     403,   403,   403,   403,   404,   405,   405,   405,   406,   406,
     407,   407,   407,   409,   410,   408,   411,   411,   412,   412,
     413,   413,   413,   413,   414,   414,   415,   415,   415,   415,
     416,   417,   415,   415,   415,   418,   415,   415,   415,   415,
     415,   415,   415,   415,   415,   415,   415,   415,   415,   419,
     420,   415,   415,   421,   422,   415,   423,   424,   415,   425,
     426,   415,   415,   427,   428,   415,   429,   430,   415,   415,
     431,   432,   415,   433,   434,   415,   415,   435,   436,   415,
     437,   438,   415,   439,   440,   415,   441,   442,   415,   443,
     443,   443,   445,   446,   447,   448,   444,   450,   451,   452,
     453,   449,   455,   456,   457,   458,   454,   459,   459,   459,
     459,   459,   460,   460,   460,   460,   460,   460,   460,   460,
     461,   462,   463,   463,   464,   464,   465,   465,   466,   466,
     468,   469,   467,   470,   471,   467,   472,   473,   467,   474,
     475,   467,   476,   476,   477,   478,   479,   477,   480,   481,
     482,   480,   483,   484,   480,   485,   486,   480,   480,   487,
     488,   480,   480,   489,   490,   480,   491,   491,   492,   492,
     492,   492,   493,   493,   494,   494,   495,   495,   495
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
       0,     1,     0,     8,     7,     0,     9,     3,     0,     7,
       0,     7,     4,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       1,     5,     1,     3,     2,     3,     1,     1,     1,     1,
       4,     1,     2,     3,     3,     3,     3,     2,     1,     3,
       0,     3,     0,     2,     3,     0,     2,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     3,     2,     2,     3,     4,     3,     2,     2,
       2,     2,     2,     3,     3,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     0,     1,     1,     3,     0,     4,     0,
       0,     0,     6,     0,     0,     6,     2,     1,     2,     2,
       1,     1,     1,     1,     2,     1,     2,     2,     2,     2,
       1,     1,     1,     2,     2,     2,     0,     2,     2,     0,
       2,     0,     2,     1,     3,     1,     3,     2,     2,     3,
       0,     0,     5,     1,     2,     5,     5,     5,     6,     2,
       1,     1,     1,     2,     3,     2,     3,     4,     1,     1,
       0,     1,     1,     1,     0,     1,     3,     8,     7,     3,
       3,     5,     0,     0,     7,     0,     0,     7,     0,     0,
       7,     0,     0,     6,     5,     8,    10,     1,     3,     1,
       2,     3,     1,     1,     2,     2,     2,     2,     2,     4,
       1,     3,     0,     4,     7,     7,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     6,     8,     5,     6,     1,     4,     3,
       0,     0,     8,     0,     0,     9,     3,     4,     5,     6,
       5,     6,     0,     0,     5,     3,     4,     4,     5,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     4,     4,     5,     4,     5,     3,     4,     1,
       1,     2,     4,     4,     1,     3,     5,     0,     0,     8,
       3,     3,     3,     0,     0,     8,     3,     4,     0,     0,
       9,     4,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     1,     4,     7,     8,     7,     4,     4,     4,
       4,     4,     1,     6,     7,     6,     6,     7,     7,     6,
       7,     6,     6,     0,     1,     0,     1,     1,     0,     1,
       0,     1,     1,     0,     1,     5,     0,     0,     4,     0,
       9,     0,    10,     3,     4,     1,     3,     1,     3,     1,
       3,     0,     1,     3,     3,     1,     3,     0,     1,     3,
       1,     1,     1,     2,     3,     5,     3,     1,     1,     1,
       0,     1,     1,     4,     3,     3,     5,     1,     3,     0,
       2,     2,     4,     6,     5,     4,     6,     5,     0,     1,
       0,     1,     1,     0,     2,     0,     4,     6,     0,     6,
       1,     3,     1,     2,     0,     1,     3,     0,     1,     1,
       0,     5,     3,     0,     1,     1,     1,     0,     2,     0,
       1,     1,     2,     0,     1,     0,     0,     0,    13,     0,
       2,     0,     1,     3,     1,     1,     0,     1,     1,     0,
       3,     0,     0,     7,     1,     4,     3,     3,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     1,     3,
       0,     1,     3,     0,     0,     6,     1,     1,     1,     3,
       3,     2,     4,     3,     1,     2,     1,     1,     1,     1,
       0,     0,     6,     4,     5,     0,     9,     4,     2,     2,
       3,     2,     3,     2,     2,     3,     3,     3,     2,     0,
       0,     6,     2,     0,     0,     6,     0,     0,     6,     0,
       0,     6,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     0,     0,     6,     0,     0,     6,     1,
       3,     3,     0,     0,     0,     0,    12,     0,     0,     0,
       0,    12,     0,     0,     0,     0,    12,     1,     1,     1,
       1,     1,     3,     3,     5,     5,     6,     6,     8,     8,
       1,     1,     3,     5,     1,     2,     1,     0,     0,     1,
       0,     0,    10,     0,     0,    10,     0,     0,     9,     0,
       0,     7,     3,     1,     5,     0,     0,    10,     4,     0,
       0,    11,     0,     0,    11,     0,     0,    10,     5,     0,
       0,     9,     5,     0,     0,    10,     1,     3,     4,     5,
       7,     9,     0,     3,     0,     1,    11,    12,    11
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

  case 21: /* top_level_reader_macro: expr_reader ';'  */
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
        yyextra->das_keyword = true;
    }
    break;

  case 83: /* expression_if_then_else: $@2 if_or_static_if '(' expr ')' optional_emit_semis expression_block expression_else  */
                                                                                                                {
        yyextra->das_keyword = false;
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-4].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-6].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 84: /* expression_if_then_else_oneliner: expression_if_one_liner "if" '(' expr ')' expression_else_one_liner SEMICOLON  */
                                                                                                                      {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-6].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 85: /* $@3: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 86: /* expression_for_loop: $@3 "for" '(' variable_name_with_pos_list "in" expr_list ')' optional_emit_semis expression_block  */
                                                                                                                                 {
        yyextra->das_keyword = false;
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-5].pNameWithPosList),(yyvsp[-3].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 87: /* expression_unsafe: "unsafe" optional_emit_semis expression_block  */
                                                                    {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-2])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 88: /* $@4: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 89: /* expression_while_loop: $@4 "while" '(' expr ')' optional_emit_semis expression_block  */
                                                                                         {
        yyextra->das_keyword = false;
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-5])));
        pWhile->cond = ExpressionPtr((yyvsp[-3].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 90: /* $@5: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 91: /* expression_with: $@5 "with" '(' expr ')' optional_emit_semis expression_block  */
                                                                                   {
        yyextra->das_keyword = false;
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-5])));
        pWith->with = ExpressionPtr((yyvsp[-3].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 92: /* expression_with_alias: "assume" "name" '=' expr  */
                                                      {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-2].s);
    }
    break;

  case 93: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 94: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 95: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 96: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 97: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 98: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 99: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 100: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 101: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 102: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 103: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 104: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 105: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 106: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 107: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 108: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 109: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 110: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 111: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 112: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 113: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 114: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 115: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 116: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 117: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 118: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 119: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 120: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 121: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 122: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 123: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 124: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 125: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 126: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 127: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 128: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 129: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 130: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 131: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 132: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 133: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 134: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 135: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 136: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 137: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 138: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 139: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 140: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 141: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 142: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 143: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 144: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 145: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 146: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 147: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 148: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 149: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 150: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 151: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 152: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 153: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 154: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 155: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 156: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 157: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 158: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 159: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 160: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 161: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 162: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 163: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 164: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 165: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 166: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 167: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 168: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 169: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 170: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 171: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 172: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 173: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 174: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 175: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 176: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 177: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 178: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 179: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 180: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 181: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 182: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 183: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 184: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 185: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 186: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 187: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 188: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 189: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 190: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 191: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 192: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 193: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 194: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 195: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 196: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 197: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 198: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 199: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 200: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 201: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 202: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 203: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 204: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 205: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 206: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 207: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 208: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 209: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 210: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 211: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 212: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 213: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 214: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 215: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 216: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 217: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 218: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 219: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 220: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 221: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 222: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 223: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 224: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 225: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 226: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 227: /* $@6: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 228: /* function_declaration: optional_public_or_private_function $@6 function_declaration_header expression_block  */
                                                                {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyvsp[-1].pFuncDecl)->privateFunction = !(yyvsp[-3].b);
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
        }
    }
    break;

  case 229: /* expression_block_finally: %empty  */
        {
        (yyval.pExpression) = nullptr;
    }
    break;

  case 230: /* $@7: %empty  */
                  {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 231: /* $@8: %empty  */
                             {
        yyextra->pop_nesteds();
    }
    break;

  case 232: /* expression_block_finally: "finally" $@7 '{' expressions $@8 '}'  */
          {
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 233: /* $@9: %empty  */
        {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 234: /* $@10: %empty  */
                                      {
        yyextra->pop_nesteds();
    }
    break;

  case 235: /* expression_block: $@9 '{' expressions $@10 '}' expression_block_finally  */
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

  case 236: /* expr_call_pipe: expr_call expr_full_block_assumed_piped  */
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

  case 237: /* expression_any: SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 238: /* expression_any: expr_assign SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 239: /* expression_any: expression_delete SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 240: /* expression_any: expression_let  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 241: /* expression_any: expression_while_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 242: /* expression_any: expression_unsafe  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 243: /* expression_any: expression_with  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 244: /* expression_any: expression_with_alias SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 245: /* expression_any: expression_for_loop  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 246: /* expression_any: expression_break SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 247: /* expression_any: expression_continue SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 248: /* expression_any: expression_return SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 249: /* expression_any: expression_yield SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 250: /* expression_any: expression_if_then_else  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 251: /* expression_any: expression_if_then_else_oneliner  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 252: /* expression_any: expression_try_catch  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 253: /* expression_any: expression_label SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 254: /* expression_any: expression_goto SEMICOLON  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 255: /* expression_any: "pass" SEMICOLON  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 256: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 257: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 258: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 259: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 260: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 261: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 262: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 263: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 264: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 265: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 266: /* name_in_namespace: "name" "::" "name"  */
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

  case 267: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 268: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 269: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 270: /* $@11: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 271: /* $@12: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 272: /* new_type_declaration: '<' $@11 type_declaration '>' $@12  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 273: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 274: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 275: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 276: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 277: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 278: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 279: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 280: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 281: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 282: /* expression_return: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 283: /* expression_return: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 284: /* expression_return: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 285: /* expression_yield: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 286: /* expression_yield: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 287: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 288: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 289: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 290: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 291: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 292: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 293: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 294: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 295: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 296: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 297: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 298: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr SEMICOLON  */
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

  case 299: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 300: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 301: /* expression_let: kwd_let optional_in_scope '{' variable_declaration_list '}'  */
                                                                               {
        (yyval.pExpression) = ast_LetList(scanner,(yyvsp[-4].b),(yyvsp[-3].b),*(yyvsp[-1].pVarDeclList),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 302: /* $@13: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 303: /* $@14: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 304: /* expr_cast: "cast" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 305: /* $@15: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 306: /* $@16: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 307: /* expr_cast: "upcast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 308: /* $@17: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 309: /* $@18: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 310: /* expr_cast: "reinterpret" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 311: /* $@19: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 312: /* $@20: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 313: /* expr_type_decl: "type" '<' $@19 type_declaration '>' $@20  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 314: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 315: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 316: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" c_or_s "name" '>' '(' expr ')'  */
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

  case 317: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 318: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 319: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 320: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 321: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 322: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 323: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 324: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 325: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 326: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 327: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 328: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 329: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 330: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 331: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 332: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 333: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 334: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis block_or_simple_block  */
                                                                                                                {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 335: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_emit_semis expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 336: /* expr_full_block_assumed_piped: '{' expressions '}'  */
                                   {
        (yyval.pExpression) = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,new TypeDecl(Type::autoinfer),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 337: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 338: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 339: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 340: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 341: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 342: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 343: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 344: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 345: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 346: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 347: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 348: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 349: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 350: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 351: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 352: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 353: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 354: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 355: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 356: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 357: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 358: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 359: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 360: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 362: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 363: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 364: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 365: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 366: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 367: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 368: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 369: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 370: /* $@21: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 371: /* $@22: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 372: /* func_addr_expr: '@' '@' '<' $@21 type_declaration_no_options '>' $@22 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 373: /* $@23: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 374: /* $@24: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 375: /* func_addr_expr: '@' '@' '<' $@23 optional_function_argument_list optional_function_type '>' $@24 func_addr_name  */
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

  case 376: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 377: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 378: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 379: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 380: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 381: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 382: /* $@25: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 383: /* $@26: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 384: /* expr_field: expr '.' $@25 error $@26  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 385: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 386: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 387: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 388: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
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

  case 389: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 390: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 391: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 392: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 393: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 394: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 395: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 396: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 397: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 398: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 399: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 400: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 401: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 402: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 403: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 404: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 405: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 406: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 407: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 408: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 409: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 410: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 411: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 412: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 413: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 414: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 415: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 416: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 417: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 418: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 419: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 420: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 421: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 422: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 423: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 424: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 425: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 426: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 427: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 428: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 429: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 430: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 431: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 432: /* expr: '(' expr_list optional_comma ')'  */
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

  case 433: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 434: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 435: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 436: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 437: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 438: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 439: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 440: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 441: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 442: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 443: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 444: /* expr: expr_generator  */
                                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 445: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 446: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 447: /* $@27: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 448: /* $@28: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 449: /* expr: expr "is" "type" '<' $@27 type_declaration_no_options '>' $@28  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 450: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 451: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 452: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 453: /* $@29: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 454: /* $@30: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 455: /* expr: expr "as" "type" '<' $@29 type_declaration '>' $@30  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 456: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 457: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 458: /* $@31: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 459: /* $@32: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 460: /* expr: expr '?' "as" "type" '<' $@31 type_declaration '>' $@32  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 461: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 462: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 463: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 464: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 465: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 466: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 467: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 468: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 469: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 470: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 471: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 472: /* expr: expr_call_pipe  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 473: /* expr: "unsafe" '(' expr ')'  */
                                         {
            (yyvsp[-1].pExpression)->alwaysSafe = true;
            (yyvsp[-1].pExpression)->userSaidItsSafe = true;
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    break;

  case 474: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 475: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 476: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list optional_emit_semis expression_block  */
                                                                                                                                                  {
        auto closure = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        ((ExprBlock *)(yyvsp[0].pExpression))->returnType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),closure,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 477: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 478: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 479: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 480: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 481: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 482: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 483: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 484: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 485: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 486: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 487: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 488: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 489: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 490: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 491: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 492: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 493: /* optional_field_annotation: %empty  */
                                      { (yyval.aaList) = nullptr; }
    break;

  case 494: /* optional_field_annotation: metadata_argument_list  */
                                      { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 495: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 496: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 497: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 498: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 499: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 500: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 501: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 502: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 503: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 504: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 505: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 506: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 507: /* $@33: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 508: /* struct_variable_declaration_list: struct_variable_declaration_list $@33 structure_variable_declaration ';'  */
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

  case 509: /* $@34: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 510: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@34 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 511: /* $@35: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 512: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@35 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 513: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 514: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 515: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 516: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 517: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 518: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 519: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 520: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 521: /* tuple_alias_type_list: %empty  */
      {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 522: /* tuple_alias_type_list: tuple_type  */
                       {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 523: /* tuple_alias_type_list: tuple_alias_type_list semis tuple_type  */
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

  case 524: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 525: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 526: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 527: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 528: /* variant_alias_type_list: variant_type  */
                         {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
        (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 529: /* variant_alias_type_list: variant_alias_type_list semis variant_type  */
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

  case 530: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 531: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 532: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 533: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 534: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 535: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 536: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 537: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 538: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 539: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 540: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 541: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 542: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 543: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 544: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 545: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 546: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 547: /* global_let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 548: /* global_let_variable_name_with_pos_list: global_let_variable_name_with_pos_list ',' "name"  */
                                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 549: /* variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 550: /* variable_declaration_list: variable_declaration_list ';'  */
                                            {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 551: /* variable_declaration_list: variable_declaration_list let_variable_declaration  */
                                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 552: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options SEMICOLON  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 553: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 554: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr SEMICOLON  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 555: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 556: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                         {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 557: /* global_let_variable_declaration: global_let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                                 {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 558: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 559: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 560: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 561: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 562: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 563: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 564: /* global_variable_declaration_list: global_variable_declaration_list ';'  */
                                                   {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 565: /* $@36: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 566: /* global_variable_declaration_list: global_variable_declaration_list $@36 optional_field_annotation let_variable_declaration  */
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

  case 567: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 568: /* $@37: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 569: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@37 optional_field_annotation global_let_variable_declaration  */
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

  case 570: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        delete (yyvsp[0].s);
    }
    break;

  case 571: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
        delete (yyvsp[-2].s);
    }
    break;

  case 574: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 575: /* enum_list: enum_expression  */
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

  case 576: /* enum_list: enum_list commas enum_expression  */
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

  case 577: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 578: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 579: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 580: /* $@38: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 581: /* single_alias: optional_public_or_private_alias "name" $@38 '=' type_declaration  */
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

  case 583: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 584: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 585: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 586: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 587: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 588: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 595: /* $@39: %empty  */
                                                     {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 596: /* $@40: %empty  */
                                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 597: /* $@41: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 598: /* enum_declaration: optional_annotation_list "enum" $@39 optional_public_or_private_enum enum_name optional_enum_basic_type_declaration optional_emit_commas '{' $@40 enum_list optional_commas $@41 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-8].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-12].faList),tokAt(scanner,(yylsp[-12])),(yyvsp[-9].b),(yyvsp[-8].pEnum),(yyvsp[-3].pEnum),(yyvsp[-7].type));
    }
    break;

  case 599: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 600: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 601: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 602: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 603: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 604: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 605: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 606: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 607: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 608: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 609: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 610: /* optional_struct_variable_declaration_list: '{' struct_variable_declaration_list '}'  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 611: /* $@42: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 612: /* $@43: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 613: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@42 structure_name $@43 optional_struct_variable_declaration_list  */
                                                                                                                                                    {
        if ( (yyvsp[-2].pStructure) ) {
            ast_structureDeclaration ( scanner, (yyvsp[-6].faList), tokAt(scanner,(yylsp[-5])), (yyvsp[-2].pStructure), tokAt(scanner,(yylsp[-2])), (yyvsp[0].pVarDeclList) );
            if ( !yyextra->g_CommentReaders.empty() ) {
                auto tak = tokAt(scanner,(yylsp[-5]));
                for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-2].pStructure),tak);
            }
        } else {
            deleteVariableDeclarationList((yyvsp[0].pVarDeclList));
        }
    }
    break;

  case 614: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 615: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 616: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 617: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 618: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 619: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 620: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 621: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 622: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 623: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 624: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 625: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 626: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 627: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 628: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 629: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 630: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 631: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 632: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 633: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 634: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 635: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 636: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 637: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 638: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 639: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 640: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 641: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 642: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 643: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 644: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 645: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 646: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 647: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 648: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 649: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 650: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 651: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 652: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 653: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 654: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 655: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 656: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 657: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 658: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 659: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 660: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 661: /* bitfield_alias_bits: "name"  */
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

  case 662: /* bitfield_alias_bits: bitfield_alias_bits commas "name"  */
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

  case 663: /* $@44: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 664: /* $@45: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 665: /* bitfield_type_declaration: "bitfield" '<' $@44 bitfield_bits '>' $@45  */
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

  case 668: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 669: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 670: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 671: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 672: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 673: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 674: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 675: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
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

  case 676: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 677: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 678: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 679: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 680: /* $@46: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 681: /* $@47: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 682: /* type_declaration_no_options_no_dim: "type" '<' $@46 type_declaration '>' $@47  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 683: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 684: /* type_declaration_no_options_no_dim: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 685: /* $@48: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 686: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@48 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 687: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '[' ']'  */
                                                                 {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 688: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "explicit"  */
                                                                  {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 689: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "const"  */
                                                               {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 690: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' "const"  */
                                                                   {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 691: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '&'  */
                                                         {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 692: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '&'  */
                                                             {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 693: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '#'  */
                                                         {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 694: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "implicit"  */
                                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 695: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '#'  */
                                                             {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 696: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "==" "const"  */
                                                                      {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 697: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "==" '&'  */
                                                                {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 698: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '?'  */
                                                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 699: /* $@49: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 700: /* $@50: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 701: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@49 type_declaration '>' $@50  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 702: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "??"  */
                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 703: /* $@51: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 704: /* $@52: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 705: /* type_declaration_no_options_no_dim: "array" '<' $@51 type_declaration '>' $@52  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 706: /* $@53: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 707: /* $@54: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 708: /* type_declaration_no_options_no_dim: "table" '<' $@53 table_type_pair '>' $@54  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 709: /* $@55: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 710: /* $@56: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 711: /* type_declaration_no_options_no_dim: "iterator" '<' $@55 type_declaration '>' $@56  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 712: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 713: /* $@57: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 714: /* $@58: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 715: /* type_declaration_no_options_no_dim: "block" '<' $@57 type_declaration '>' $@58  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 716: /* $@59: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 717: /* $@60: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 718: /* type_declaration_no_options_no_dim: "block" '<' $@59 optional_function_argument_list optional_function_type '>' $@60  */
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

  case 719: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 720: /* $@61: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 721: /* $@62: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 722: /* type_declaration_no_options_no_dim: "function" '<' $@61 type_declaration '>' $@62  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 723: /* $@63: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 724: /* $@64: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 725: /* type_declaration_no_options_no_dim: "function" '<' $@63 optional_function_argument_list optional_function_type '>' $@64  */
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

  case 726: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 727: /* $@65: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 728: /* $@66: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 729: /* type_declaration_no_options_no_dim: "lambda" '<' $@65 type_declaration '>' $@66  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 730: /* $@67: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 731: /* $@68: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 732: /* type_declaration_no_options_no_dim: "lambda" '<' $@67 optional_function_argument_list optional_function_type '>' $@68  */
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

  case 733: /* $@69: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 734: /* $@70: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 735: /* type_declaration_no_options_no_dim: "tuple" '<' $@69 tuple_type_list '>' $@70  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 736: /* $@71: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 737: /* $@72: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 738: /* type_declaration_no_options_no_dim: "variant" '<' $@71 variant_type_list '>' $@72  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 739: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 740: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 741: /* type_declaration: type_declaration '|' '#'  */
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

  case 742: /* $@73: %empty  */
                   {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 743: /* $@74: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 744: /* $@75: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 745: /* $@76: %empty  */
                                                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 746: /* tuple_alias_declaration: "tuple" $@73 optional_public_or_private_alias "name" optional_emit_semis $@74 '{' $@75 tuple_alias_type_list optional_semis $@76 '}'  */
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

  case 747: /* $@77: %empty  */
                     {
        yyextra->push_nesteds(DAS_EMIT_SEMICOLON);
    }
    break;

  case 748: /* $@78: %empty  */
                                                                             {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 749: /* $@79: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 750: /* $@80: %empty  */
                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 751: /* variant_alias_declaration: "variant" $@77 optional_public_or_private_alias "name" optional_emit_semis $@78 '{' $@79 variant_alias_type_list optional_semis $@80 '}'  */
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

  case 752: /* $@81: %empty  */
                      {
        yyextra->push_nesteds(DAS_EMIT_COMMA);
    }
    break;

  case 753: /* $@82: %empty  */
                                                                              {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 754: /* $@83: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 755: /* $@84: %empty  */
                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        yyextra->pop_nesteds();
    }
    break;

  case 756: /* bitfield_alias_declaration: "bitfield" $@81 optional_public_or_private_alias "name" optional_emit_commas $@82 '{' $@83 bitfield_alias_bits optional_commas $@84 '}'  */
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

  case 757: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 758: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 759: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 760: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 761: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 762: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 763: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 764: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 765: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 766: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 767: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 768: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 769: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 770: /* make_variant_dim: make_struct_fields  */
                                {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 771: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 772: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 773: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 774: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 775: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 776: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 777: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 778: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 779: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 780: /* $@85: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 781: /* $@86: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 782: /* make_struct_decl: "struct" '<' $@85 type_declaration_no_options '>' $@86 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 783: /* $@87: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 784: /* $@88: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 785: /* make_struct_decl: "class" '<' $@87 type_declaration_no_options '>' $@88 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 786: /* $@89: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 787: /* $@90: %empty  */
                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 788: /* make_struct_decl: "variant" '<' $@89 type_declaration_no_options '>' $@90 '(' make_variant_dim ')'  */
                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 789: /* $@91: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 790: /* $@92: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 791: /* make_struct_decl: "default" '<' $@91 type_declaration_no_options '>' $@92 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 792: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 793: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 794: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 795: /* $@93: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 796: /* $@94: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 797: /* make_tuple_call: "tuple" '<' $@93 type_declaration_no_options '>' $@94 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 798: /* make_dim_decl: '[' expr_list optional_comma ']'  */
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

  case 799: /* $@95: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 800: /* $@96: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 801: /* make_dim_decl: "array" "struct" '<' $@95 type_declaration_no_options '>' $@96 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 802: /* $@97: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 803: /* $@98: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 804: /* make_dim_decl: "array" "tuple" '<' $@97 type_declaration_no_options '>' $@98 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 805: /* $@99: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 806: /* $@100: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 807: /* make_dim_decl: "array" "variant" '<' $@99 type_declaration_no_options '>' $@100 '(' make_variant_dim ')'  */
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

  case 808: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 809: /* $@101: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 810: /* $@102: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 811: /* make_dim_decl: "array" '<' $@101 type_declaration_no_options '>' $@102 '(' optional_expr_list ')'  */
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

  case 812: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 813: /* $@103: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 814: /* $@104: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 815: /* make_dim_decl: "fixed_array" '<' $@103 type_declaration_no_options '>' $@104 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 816: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 817: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 818: /* make_table_decl: '{' expr_map_tuple_list optional_comma '}'  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 819: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 820: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 821: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 822: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 823: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 824: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 825: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 826: /* array_comprehension: '[' "for" '(' variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                           {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 827: /* array_comprehension: '[' "iterator" "for" '(' variable_name_with_pos_list "in" expr_list ')' ';' expr array_comprehension_where ']'  */
                                                                                                                                                                        {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 828: /* array_comprehension: '{' "for" '(' variable_name_with_pos_list "in" expr_list ')' ';' make_map_tuple array_comprehension_where '}'  */
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


